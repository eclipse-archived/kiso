/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

/* Include all headers which are needed by this file in the following order:
 * Own public header
 * Own protected header
 * Own private header
 * System header (if necessary)
 * Other headers
 */
#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_AT_3GPP_27007

#include "At3Gpp27007.h"

#include "Engine.h"
#include "AtResponseQueue.h"
#include "AtUtils.h"

#include "Kiso_Basics.h"
#include "Kiso_Logging.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#define URC_3GPP_27007_TIMEOUT (UINT32_C(120)) /* msec */

#define CMD_3GPP_27007_SHORT_TIMEOUT (UINT32_C(120))   /* msec */
#define CMD_3GPP_27007_CFUN_TIMEOUT (UINT32_C(180000)) /* msec */
#define CMD_3GPP_27007_LONG_TIMEOUT (UINT32_C(150000)) /* msec */

#define AT_3GPP_27007_IPV4_BYTE_COUNT (UINT32_C(4))
#define AT_3GPP_27007_IPV6_BYTE_COUNT (UINT32_C(16))
#define AT_3GPP_27007_MAX_IP_STR_LENGTH (UINT32_C(65)) /* "255.255.255.255" or "255.255.255.255.255.255.255.255.255.255.255.255.255.255.255.255" */

#define CMD_3GPP_27007_ATCREG "CREG"
#define CMD_3GPP_27007_SET_ATCREG_FMT ("AT+" CMD_3GPP_27007_ATCREG "=%d\r\n")
#define CMD_3GPP_27007_GET_ATCREG ("AT+" CMD_3GPP_27007_ATCREG "?\r\n")

#define CMD_3GPP_27007_ATCGREG "CGREG"
#define CMD_3GPP_27007_SET_ATCGREG_FMT ("AT+" CMD_3GPP_27007_ATCGREG "=%d\r\n")
#define CMD_3GPP_27007_GET_ATCGREG ("AT+" CMD_3GPP_27007_ATCGREG "?\r\n")

#define CMD_3GPP_27007_ATCEREG "CEREG"
#define CMD_3GPP_27007_SET_ATCEREG_FMT ("AT+" CMD_3GPP_27007_ATCEREG "=%d\r\n")
#define CMD_3GPP_27007_GET_ATCEREG ("AT+" CMD_3GPP_27007_ATCEREG "?\r\n")

#define CMD_3GPP_27007_ATCOPS "COPS"
#define CMD_3GPP_27007_SET_ATCOPS_FMT ("AT+" CMD_3GPP_27007_ATCOPS "=%d\r\n")

#define CMD_3GPP_27007_ATCGDCONT "CGDCONT"
#define CMD_3GPP_27007_SET_ATCGDCONT_FMT1 ("AT+" CMD_3GPP_27007_ATCGDCONT "=%d\r\n")
#define CMD_3GPP_27007_SET_ATCGDCONT_FMT2 ("AT+" CMD_3GPP_27007_ATCGDCONT "=%d,%s\r\n")
#define CMD_3GPP_27007_SET_ATCGDCONT_FMT3 ("AT+" CMD_3GPP_27007_ATCGDCONT "=%d,%s,\"%s\"\r\n")
#define ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IP ("\"IP\"")
#define ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IPV6 ("\"IPV6\"")
#define ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IPV4V6 ("\"IPV4V6\"")

#define CMD_3GPP_27007_ATCGACT "CGACT"
#define CMD_3GPP_27007_SET_ATCGACT_FMT ("AT+" CMD_3GPP_27007_ATCGACT "=%d,%d\r\n")

#define CMD_3GPP_27007_ATCGPADDR "CGPADDR"
#define CMD_3GPP_27007_EXE_ATCGPADDR ("AT+" CMD_3GPP_27007_ATCGPADDR "=%d\r\n")

#define CMD_3GPP_27007_ATCPIN "CPIN"
#define CMD_3GPP_27007_SET_ATCPIN_FMT ("AT+" CMD_3GPP_27007_ATCPIN "=\"%s\"\r\n")

#define CMD_3GPP_27007_SET_AT ("AT\r\n")
#define CMD_3GPP_27007_SET_ATE_FMT ("ATE%d\r\n")

#define CMD_3GPP_27007_ATCFUN "CFUN"
#define CMD_3GPP_27007_SET_ATCFUN_FMT1 ("AT+" CMD_3GPP_27007_ATCFUN "=%d\r\n")
#define CMD_3GPP_27007_SET_ATCFUN_FMT2 ("AT+" CMD_3GPP_27007_ATCFUN "=%d,%d\r\n")
#define CMD_3GPP_27007_GET_ATCFUN ("AT+" CMD_3GPP_27007_ATCFUN "?\r\n")

#define CMD_3GPP_27007_ATCMEE "CMEE"
#define CMD_3GPP_27007_SET_ATCMEE_FMT ("AT+" CMD_3GPP_27007_ATCMEE "=%d\r\n")

static Retcode_T ExtractCxregN(const uint8_t *data, uint32_t length,
                               AT_CXREG_N_T *value)
{
    if (NULL == data || 2 < length || NULL == value) //-- number range expected: 0..5 never 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR,
                       RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    int32_t number = 0;
    Retcode_T ret = Utils_StrtolBounds(data, length, &number,
                                       (int32_t)AT_CXREG_N_DISABLED,
                                       (int32_t)AT_CXREG_N_URC_LOC_PSM_CAUSE);
    if (RETCODE_OK == ret)
    {
        *value = (AT_CXREG_N_T)number;
        return RETCODE_OK;
    }

    *value = AT_CXREG_N_INVALID;
    LOG_ERROR("AT_3GPP_27007 ExtractCregN value %" PRIi32 " out of range", number); //LCOV_EXCL_BR_LINE
    return RETCODE(RETCODE_SEVERITY_ERROR,
                   RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
}

static Retcode_T ExtractCxregStat(const uint8_t *data, uint32_t length,
                                  AT_CXREG_Stat_T *value)
{
    if (NULL == data || 2 < length || NULL == value) //-- number range expected: 1..10 without 8 and never 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR,
                       RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    int32_t number = 0;
    Retcode_T ret = Utils_StrtolBounds(data, length, &number,
                                       (int32_t)AT_CXREG_STAT_NOT,
                                       (int32_t)AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING);
    if (RETCODE_OK == ret)
    {
        if (8 != number) //-- enum value 8 does not exist
        {
            *value = (AT_CXREG_Stat_T)number;
            return RETCODE_OK;
        }
    }

    *value = AT_CXREG_STAT_INVALID;
    LOG_ERROR("AT_3GPP_27007 ExtractCregStat value %" PRIi32 " out of range", number); //LCOV_EXCL_BR_LINE
    return RETCODE(RETCODE_SEVERITY_ERROR,
                   RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
}

static Retcode_T IdFromHex(const uint8_t *data, uint32_t length,
                           uint32_t *value, uint32_t errorValue)
{
    if (NULL == data || 0 == length || NULL == value)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR,
                       RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    const char delimiter[] = " \"";
    char buffer[length + 1];
    char *ptr = NULL;
    strncpy(buffer, (const char *)data, length);
    buffer[length] = 0;
    ptr = strtok(buffer, delimiter);
    if (NULL != ptr)
    {
        *value = (uint32_t)strtoul(ptr, NULL, 16);
        if ((UINT32_MAX == *value) || ((0 == *value) && ('0' != ptr[0])))
        {
            *value = errorValue;
            return RETCODE(RETCODE_SEVERITY_ERROR,
                           RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }
    }
    else
    {
        *value = errorValue;
        return RETCODE(RETCODE_SEVERITY_ERROR,
                       RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    return RETCODE_OK;
}

static Retcode_T ExtractCxregCi(const uint8_t *data, uint32_t length,
                                uint32_t *value)
{
    return IdFromHex(data, length, value, 0xFFFFFFFF);
}

static Retcode_T ExtractCxregLac(const uint8_t *data, uint32_t length,
                                 uint16_t *value)
{
    uint32_t tmp = UINT16_MAX;
    Retcode_T retcode = IdFromHex(data, length, &tmp, UINT16_MAX);
    if (RETCODE_OK == retcode && tmp > UINT16_MAX)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR,
                          RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    else
    {
        *value = tmp;
    }

    return retcode;
}

static Retcode_T ExtractCxregAct(const uint8_t *data, uint32_t length,
                                 AT_CXREG_AcT_T *value)
{
    if (NULL == data || 0 == length || 3 < length || NULL == value) //-- number range expected: 0..7 and 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR,
                       RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    int32_t number = 0;
    Retcode_T ret = Utils_StrtolBounds(data, length, &number,
                                       (int32_t)AT_CXREG_ACT_GSM,
                                       (int32_t)AT_CXREG_ACT_INVALID);
    if (RETCODE_OK == ret)
    {
        if ((((int32_t)AT_CXREG_ACT_GSM <= number) && ((int32_t)AT_CXREG_ACT_EUTRA_NR >= number)) || ((int32_t)AT_CXREG_ACT_INVALID == number))
        {
            *value = (AT_CXREG_AcT_T)number;
            return RETCODE_OK;
        }
    }

    LOG_ERROR("AT_3GPP_27007 ExtractCregAct value %" PRIi32 " out of range", number); //LCOV_EXCL_BR_LINE
    return RETCODE(RETCODE_SEVERITY_ERROR,
                   RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
}

static Retcode_T ExtractCgregRac(const uint8_t *data, uint32_t length,
                                 uint8_t *value)
{
    uint32_t tmp = 0;
    Retcode_T ret = IdFromHex(data, length, &tmp, UINT8_MAX);
    if (RETCODE_OK == ret)
    {
        if (UINT8_MAX < tmp)
        {
            ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            *value = UINT8_MAX;
        }
        else
        {
            *value = tmp;
        }
    }
    return ret;
}

static Retcode_T ExtractCeregTac(const uint8_t *data, uint32_t length,
                                 uint16_t *value)
{
    uint32_t tmp = 0;
    Retcode_T ret = IdFromHex(data, length, &tmp, UINT16_MAX);
    if (RETCODE_OK == ret)
    {
        if (UINT16_MAX < tmp)
        {
            ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            *value = UINT16_MAX;
        }
        else
        {
            *value = tmp;
        }
    }
    return ret;
}

static Retcode_T ExtractCgpaddrAddress(const uint8_t *addressBuff, uint32_t addressBuffLen, AT_CGPADDR_Address_T *parsedAddress)
{
    if (NULL == addressBuff || NULL == parsedAddress || 0 == addressBuffLen || addressBuffLen > AT_3GPP_27007_MAX_IP_STR_LENGTH)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = RETCODE_OK;
    uint8_t result[AT_3GPP_27007_IPV6_BYTE_COUNT] = {0};

    uint32_t currentByte = 0;
    uint8_t positionInCurrentByte = 0;
    uint8_t bytesParsed = 0;
    for (uint32_t i = 0; i < addressBuffLen; i++)
    {
        uint8_t currentChar = addressBuff[i];

        if ('.' == currentChar)
        {
            result[sizeof(result) - bytesParsed - 1] = currentByte;

            bytesParsed++;
            currentByte = 0;
            positionInCurrentByte = 0;
        }
        else if (isdigit((int)currentChar))
        {
            currentByte *= 10;
            currentByte += (currentChar - '0');
            positionInCurrentByte++;

            if (currentByte > 0xFF || positionInCurrentByte > 3)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                break;
            }
        }
        else if ('"' == currentChar && (0 == i || (addressBuffLen - 1) == i))
        {
            /* Ignore quote chars at start and end.
             * e.g. "127.0.0.1" */
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            break;
        }

        if ((addressBuffLen - 1) == i)
        {
            // end of IP address
            result[sizeof(result) - bytesParsed - 1] = currentByte;
            bytesParsed++;
        }
    }

    switch (bytesParsed)
    {
    case AT_3GPP_27007_IPV4_BYTE_COUNT:
        parsedAddress->Type = AT_CGPADDR_ADDRESSTYPE_IPV4;
        memcpy(parsedAddress->Address.IPv4, result + (sizeof(result) - bytesParsed), bytesParsed);
        retcode = RETCODE_OK;
        break;
    case AT_3GPP_27007_IPV6_BYTE_COUNT:
        parsedAddress->Type = AT_CGPADDR_ADDRESSTYPE_IPV6;
        memcpy(parsedAddress->Address.IPv6, result + (sizeof(result) - bytesParsed), bytesParsed);
        retcode = RETCODE_OK;
        break;
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        break;
    }

    return retcode;
}

/* *** NETWORK COMMANDS ****************************************************** */

static Retcode_T Set_CXREG(const char *fmt, AT_CXREG_N_T n)
{
    Retcode_T retcode = RETCODE_OK;

    assert(NULL != fmt);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    int32_t len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), fmt, (int)n);
#pragma GCC diagnostic pop
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }
    else
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_3GPP_27007_SHORT_TIMEOUT);
        if (RETCODE_OK == retcode)
        {
            retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
        }
        return retcode;
    }

    return retcode;
}

static Retcode_T Send_Get_CXREG(const char *atcmd, const char *cmd)
{
    Retcode_T retcode = RETCODE_OK;

    assert(NULL != atcmd);
    assert(NULL != cmd);

    retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)atcmd, (uint32_t)strlen(atcmd), CMD_3GPP_27007_SHORT_TIMEOUT);
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_3GPP_27007_SHORT_TIMEOUT, (const uint8_t *)cmd, strlen(cmd)); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

static Retcode_T Handle_Get_CXREG_N(AT_CXREG_N_T *n)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t *buffer = NULL;
    uint32_t bufferLen = 0;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        retcode = ExtractCxregN(buffer, bufferLen, n);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    else
    {
        *n = AT_CXREG_N_INVALID;
    }

    return retcode;
}

static Retcode_T Handle_Get_CXREG_Stat(AT_CXREG_Stat_T *stat)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *buffer = NULL;
    uint32_t bufferLen = 0;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        retcode = ExtractCxregStat(buffer, bufferLen, stat); //LCOV_EXCL_BR_LINE

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    else
    {
        *stat = AT_CXREG_STAT_INVALID;
    }

    return retcode;
}

static Retcode_T Handle_Get_CXREG_Lac(uint16_t *lac)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *buffer = NULL;
    uint32_t bufferLen = 0;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        retcode = ExtractCxregLac(buffer, bufferLen, lac);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    else
    {
        *lac = AT_INVALID_LAC;
    }

    return retcode;
}

static Retcode_T Handle_Get_CXREG_Ci(uint32_t *ci)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *buffer = NULL;
    uint32_t bufferLen = 0;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        retcode = ExtractCxregCi(buffer, bufferLen, ci);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    else
    {
        *ci = AT_INVALID_CI;
    }

    return retcode;
}

static Retcode_T Handle_Get_CXREG_AcT(AT_CXREG_AcT_T *act)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *buffer = NULL;
    uint32_t bufferLen = 0;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT, &buffer, &bufferLen);
    if (RETCODE_OK == retcode)
    {
        retcode = ExtractCxregAct(buffer, bufferLen, act); //LCOV_EXCL_BR_LINE

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    else
    {
        *act = AT_CXREG_ACT_INVALID;
    }

    return retcode;
}

static Retcode_T Handle_Get_CGREG_Rac(uint8_t *rac)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *buffer = NULL;
    uint32_t bufferLen = 0;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        retcode = ExtractCgregRac(buffer, bufferLen, rac);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    else
    {
        *rac = AT_INVALID_RAC;
    }

    return retcode;
}

static Retcode_T Handle_Get_CEREG_Tac(uint16_t *tac)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *buffer = NULL;
    uint32_t bufferLen = 0;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        retcode = ExtractCeregTac(buffer, bufferLen, tac);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    else
    {
        *tac = AT_INVALID_TAC;
    }

    return retcode;
}

Retcode_T At_Set_CREG(AT_CXREG_N_T n)
{
    return Set_CXREG(CMD_3GPP_27007_SET_ATCREG_FMT, n);
}

Retcode_T At_Set_CGREG(AT_CXREG_N_T n)
{
    return Set_CXREG(CMD_3GPP_27007_SET_ATCGREG_FMT, n);
}

Retcode_T At_Set_CEREG(AT_CXREG_N_T n)
{
    return Set_CXREG(CMD_3GPP_27007_SET_ATCEREG_FMT, n);
}

Retcode_T At_Get_CREG(AT_CREG_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    Retcode_T retcodeOpt = RETCODE_OK;

    assert(NULL != param);

    retcode = Send_Get_CXREG(CMD_3GPP_27007_GET_ATCREG, CMD_3GPP_27007_ATCREG);

    if (RETCODE_OK == retcode)
    {
        retcode = Handle_Get_CXREG_N(&param->N);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Handle_Get_CXREG_Stat(&param->Stat);
    }

    if (RETCODE_OK == retcode)
    {
        switch (param->N)
        {
        case AT_CXREG_N_DISABLED:
        case AT_CXREG_N_URC:
            /* Done. AT_CXREG_N_DISABLED and AT_CXREG_N_URC both only carry the
             * 'stat' parameter */
            break;
        case AT_CXREG_N_URC_LOC:
        case AT_CXREG_N_URC_LOC_CAUSE:
            /** \todo: cause_type and reject_cause not supported! */
            retcode = Handle_Get_CXREG_Lac(&param->Lac);

            if (RETCODE_OK == retcode)
            {
                retcode = Handle_Get_CXREG_Ci(&param->Ci);
            }

            if (RETCODE_OK == retcode)
            {
                retcodeOpt = Handle_Get_CXREG_AcT(&param->AcT);
                /* Some modems don't report AcT every time. Will be ignored. */
                KISO_UNUSED(retcodeOpt);
            }
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR,
                              RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            break;
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Get_CGREG(AT_CGREG_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;

    assert(NULL != param);

    retcode = Send_Get_CXREG(CMD_3GPP_27007_GET_ATCGREG, CMD_3GPP_27007_ATCGREG);

    if (RETCODE_OK == retcode)
    {
        retcode = Handle_Get_CXREG_N(&param->N);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Handle_Get_CXREG_Stat(&param->Stat);
    }

    if (RETCODE_OK == retcode)
    {
        switch (param->N)
        {
        case AT_CXREG_N_DISABLED:
        case AT_CXREG_N_URC:
            /* Done. AT_CXREG_N_DISABLED and AT_CXREG_N_URC both only carry the
             * 'stat' parameter */
            break;
        case AT_CXREG_N_URC_LOC:
        case AT_CXREG_N_URC_LOC_CAUSE:
        case AT_CXREG_N_URC_LOC_PSM:
        case AT_CXREG_N_URC_LOC_PSM_CAUSE:
            /** \todo: cause_type, reject_cause, Active-Time, Periodic-RAU,
             * GPRS-READY-timer not supported! */
            retcode = Handle_Get_CXREG_Lac(&param->Lac);

            if (RETCODE_OK == retcode)
            {
                retcode = Handle_Get_CXREG_Ci(&param->Ci);
            }

            if (RETCODE_OK == retcode)
            {
                retcode = Handle_Get_CXREG_AcT(&param->AcT);
            }

            if (RETCODE_OK == retcode)
            {
                retcode = Handle_Get_CGREG_Rac(&param->Rac);
            }
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR,
                              RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            break;
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Get_CEREG(AT_CEREG_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;

    assert(NULL != param);

    retcode = Send_Get_CXREG(CMD_3GPP_27007_GET_ATCEREG, CMD_3GPP_27007_ATCEREG);

    if (RETCODE_OK == retcode)
    {
        retcode = Handle_Get_CXREG_N(&param->N);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Handle_Get_CXREG_Stat(&param->Stat);
    }

    if (RETCODE_OK == retcode)
    {
        switch (param->N)
        {
        case AT_CXREG_N_DISABLED:
        case AT_CXREG_N_URC:
            /* Done. AT_CXREG_N_DISABLED and AT_CXREG_N_URC both only carry the
             * 'stat' parameter */
            break;
        case AT_CXREG_N_URC_LOC:
        case AT_CXREG_N_URC_LOC_CAUSE:
        case AT_CXREG_N_URC_LOC_PSM:
        case AT_CXREG_N_URC_LOC_PSM_CAUSE:
            /** \todo: cause_type, reject_cause, Active-Time, Periodic-TAU not
             * supported! */
            retcode = Handle_Get_CEREG_Tac(&param->Tac);

            if (RETCODE_OK == retcode)
            {
                retcode = Handle_Get_CXREG_Ci(&param->Ci);
            }

            if (RETCODE_OK == retcode)
            {
                retcode = Handle_Get_CXREG_AcT(&param->AcT);
            }
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR,
                              RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            break;
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_COPS(const AT_COPS_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    assert(NULL != param);

    switch (param->Mode)
    {
    case AT_COPS_MODE_AUTOMATIC:
    case AT_COPS_MODE_DEREGISTER:
        /* continue */
        break;
    case AT_COPS_MODE_MANUAL:
    case AT_COPS_MODE_SET_FORMAT_ONLY:
    case AT_COPS_MODE_MANUAL_THEN_AUTOMATIC:
        /* Currently not supported, sorry! ... maybe your first PR? :) */
        /** \todo Implement remaining 3GPP 27.007 command modes for AT+COPS
         * setter. */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        break;
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }

    if (RETCODE_OK == retcode)
    {
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCOPS_FMT, (int)param->Mode);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_3GPP_27007_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_CGDCONT(const AT_CGDCONT_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;
    const char *pdpType = NULL;
    bool clearCtx = false;

    assert(NULL != param);

    switch (param->PdpType)
    {
    case AT_CGDCONT_PDPTYPE_IP:
        pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IP;
        break;
    case AT_CGDCONT_PDPTYPE_IPV6:
        pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IPV6;
        break;
    case AT_CGDCONT_PDPTYPE_IPV4V6:
        pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IPV4V6;
        break;
    case AT_CGDCONT_PDPTYPE_INVALID:
        /* No <PDP_Type> means clear the context! */
        clearCtx = true;
        break;
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        break;
    }

    if (RETCODE_OK == retcode)
    {
        if (clearCtx)
        {
            len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCGDCONT_FMT1,
                           param->Cid);
            if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }
        }
        else
        {
            if (NULL == param->Apn)
            {
                len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCGDCONT_FMT2,
                               param->Cid, pdpType);
                if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
                }
            }
            else
            {
                len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCGDCONT_FMT3,
                               param->Cid, pdpType, param->Apn);
                if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
                }
            }
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_3GPP_27007_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_CGACT(const AT_CGACT_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCGACT_FMT,
                   param->State, param->Cid);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_3GPP_27007_LONG_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_LONG_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_CGPADDR(const AT_CGPADDR_Param_T *param, AT_CGPADDR_Resp_T *resp)
{
    Retcode_T retcode = RETCODE_OK;
    Retcode_T retOptional = RETCODE_OK;
    uint8_t *rxBuffer = NULL;
    uint32_t rxBufferLen = 0;
    int32_t len = 0;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_EXE_ATCGPADDR,
                   param->Cid);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_3GPP_27007_SHORT_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        //LCOV_EXCL_BR_START
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_3GPP_27007_SHORT_TIMEOUT,
                                                  (const uint8_t *)CMD_3GPP_27007_ATCGPADDR,
                                                  (uint32_t)strlen(CMD_3GPP_27007_ATCGPADDR));
        //LCOV_EXCL_BR_STOP
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait for cid in response. */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT,
                                                         &rxBuffer, &rxBufferLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t respCid = 0;
        retcode = Utils_Strtol(rxBuffer, rxBufferLen, &respCid);
        if (RETCODE_OK == retcode && respCid != param->Cid)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        AtResponseQueue_MarkBufferAsUnused(); /* clear cid from queue */ //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait for PDP_addr1 in response. */ //LCOV_EXCL_BR_START
        retOptional = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT,
                                                             &rxBuffer, &rxBufferLen); //LCOV_EXCL_BR_STOP
        if (RETCODE_OK == retOptional)
        {
            retcode = ExtractCgpaddrAddress(rxBuffer, rxBufferLen, &resp->Address1); //LCOV_EXCL_BR_LINE
            AtResponseQueue_MarkBufferAsUnused(); /* clear PDP_addr from queue */    //LCOV_EXCL_BR_LINE

            /* Wait for PDP_addr2 in response. */
            //LCOV_EXCL_BR_START
            retOptional = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_SHORT_TIMEOUT,
                                                                 &rxBuffer, &rxBufferLen); //LCOV_EXCL_BR_STOP
            if (RETCODE_OK == retOptional)
            {
                retcode = ExtractCgpaddrAddress(rxBuffer, rxBufferLen, &resp->Address2);
                AtResponseQueue_MarkBufferAsUnused(); /* clear PDP_addr from queue */ //LCOV_EXCL_BR_LINE
            }
            else
            {
                memset(&resp->Address2, 0, sizeof(resp->Address2));
                resp->Address2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
            }
        }
        else
        {
            memset(&resp->Address1, 0, sizeof(resp->Address1));
            resp->Address1.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
            memset(&resp->Address2, 0, sizeof(resp->Address2));
            resp->Address2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

/*** AT Error handling code mode **/
Retcode_T At_Set_CMEE(uint32_t mode)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCMEE_FMT, (int)mode);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_3GPP_27007_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

/* *** SIM COMMANDS ********************************************************* */

Retcode_T At_Set_CPIN(const char *pin)
{
    Retcode_T retcode = RETCODE_OK;

    int32_t len = 0;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCPIN_FMT,
                   pin);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, len, CMD_3GPP_27007_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

/* *** TE-TA INTERFACE COMMANDS ********************************************** */

Retcode_T At_Test_AT(void)
{
    Retcode_T retcode = RETCODE_OK;
    AtResponseQueueEntry_T *e;

    retcode = Engine_SendAtCommand((const uint8_t *)CMD_3GPP_27007_SET_AT, strlen(CMD_3GPP_27007_SET_AT));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    while (AtResponseQueue_GetEvent(CMD_3GPP_27007_SHORT_TIMEOUT, &e) == RETCODE_OK)
    {
        if (AT_EVENT_TYPE_MISC == e->Type && e->BufferLength > 0 && 0 == memcmp(CMD_3GPP_27007_SET_AT, e->Buffer, e->BufferLength))
        {
            retcode = RETCODE_OK;
        }
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Set_ATE(bool enableEcho)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATE_FMT, enableEcho ? 1 : 0);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }
    else
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_3GPP_27007_SHORT_TIMEOUT);
        if (RETCODE_OK == retcode)
        {
            Engine_EchoModeEnabled(enableEcho);
            retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_SHORT_TIMEOUT, retcode);
        }

        return retcode;
    }
}

/* *** POWER CONTROL COMMANDS *********************************************** */

Retcode_T At_Set_CFUN(const AT_CFUN_Param_T *param)
{
    switch (param->Fun)
    {
    case AT_CFUN_FUN_MINIMUM:
    case AT_CFUN_FUN_FULL:
    case AT_CFUN_FUN_DISABLETX:
    case AT_CFUN_FUN_DISABLERX:
    case AT_CFUN_FUN_DISABLERXTX:
    case AT_CFUN_FUN_RESERVEDSTART:
    case AT_CFUN_FUN_RESERVEDEND:
    case AT_CFUN_FUN_PREPARESHUTDOWN:
        /* do nothing */
        break;
    case AT_CFUN_FUN_INVALID:
    default:
        if (AT_CFUN_FUN_RESERVEDSTART > param->Fun || AT_CFUN_FUN_RESERVEDEND < param->Fun)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        break;
    }

    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    if (param->Rst == AT_CFUN_RST_INVALID)
    {
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCFUN_FMT1,
                       (int)param->Fun);
    }
    else
    {
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_3GPP_27007_SET_ATCFUN_FMT2,
                       (int)param->Fun, (int)param->Rst);
    }
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_3GPP_27007_CFUN_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_CFUN_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Get_CFUN(AT_CFUN_Resp_T *resp)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;

    retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)CMD_3GPP_27007_GET_ATCFUN, strlen(CMD_3GPP_27007_GET_ATCFUN), CMD_3GPP_27007_CFUN_TIMEOUT);

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_3GPP_27007_CFUN_TIMEOUT, (const uint8_t *)CMD_3GPP_27007_ATCFUN, strlen(CMD_3GPP_27007_ATCFUN)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_3GPP_27007_CFUN_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_StrtolBounds(arg, argLen, (int32_t *)&resp->Fun, AT_CFUN_FUN_MINIMUM, AT_CFUN_FUN_PREPARESHUTDOWN);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_3GPP_27007_CFUN_TIMEOUT, retcode);
    }

    return retcode;
}

/* *** URC HANDLERS ********************************************************** */

Retcode_T At_HandleUrc_CREG(void)
{
    Retcode_T ret = RETCODE_OK;
    Retcode_T retStat = RETCODE_OK;
    Retcode_T retLac = RETCODE_OK;
    Retcode_T retCi = RETCODE_OK;
    Retcode_T retAcT = RETCODE_OK;
    Retcode_T retOptional = RETCODE_OK;
    bool urcFound = false;
    AT_CREG_Param_T data = {
        .N = AT_CXREG_N_INVALID,
        .Stat = AT_CXREG_STAT_INVALID,
        .Lac = AT_INVALID_LAC,
        .Ci = AT_INVALID_CI,
        .AcT = AT_CXREG_ACT_INVALID,
    };
    uint8_t *cmdArg = NULL;
    uint32_t lengthCmdArg = 0;

    ret = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)CMD_3GPP_27007_ATCREG,
                                          strlen(CMD_3GPP_27007_ATCREG)); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == ret)
    {
        memset(&data, 0, sizeof(data));
        urcFound = true;
        ret = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT,
                                                     &cmdArg, &lengthCmdArg); //-- stat//LCOV_EXCL_BR_LINE
    }
    if (RETCODE_OK == ret)
    {
        retStat = ExtractCxregStat(cmdArg, lengthCmdArg, &data.Stat);
        if (RETCODE_OK == retStat)
        {
            switch (data.Stat)
            {
            case AT_CXREG_STAT_HOME:
            case AT_CXREG_STAT_ROAMING:
            case AT_CXREG_STAT_CSFB_NOT_PREF_HOME:
            case AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING:
                /* set device status and notify event */
                /** \todo Propagate type of CxREG to client-callback via
                 * param. */
                Engine_NotifyNewState(CELLULAR_STATE_REGISTERED, NULL, 0);
                break;
            case AT_CXREG_STAT_NOT:
            case AT_CXREG_STAT_DENIED:
                Engine_NotifyNewState(CELLULAR_STATE_POWERON, NULL, 0);
                break;
            case AT_CXREG_STAT_NOT_AND_SEARCH:
            case AT_CXREG_STAT_UNKNOWN:
                Engine_NotifyNewState(CELLULAR_STATE_REGISTERING, NULL, 0);
                break;
            default:
                /* do nothing */
                break;
            }
        }

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
        cmdArg = NULL;
        lengthCmdArg = 0;
        retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- lac//LCOV_EXCL_BR_LINE
        if (RETCODE_OK == retOptional)
        {
            retLac = ExtractCxregLac(cmdArg, lengthCmdArg, &data.Lac);
            if (RETCODE_OK != retLac)
                data.Lac = AT_INVALID_LAC;
            AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
            cmdArg = NULL;
            lengthCmdArg = 0;
            retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- ci//LCOV_EXCL_BR_LINE
            if (RETCODE_OK == retOptional)
            {
                retCi = ExtractCxregCi(cmdArg, lengthCmdArg, &data.Ci);
                if (RETCODE_OK != retCi)
                    data.Ci = AT_INVALID_CI;
                AtResponseQueue_MarkBufferAsUnused();
                cmdArg = NULL;
                lengthCmdArg = 0;
                retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- AcT//LCOV_EXCL_BR_LINE
                if (RETCODE_OK == retOptional)
                {
                    retAcT = ExtractCxregAct(cmdArg, lengthCmdArg, &data.AcT);
                    if (RETCODE_OK != retAcT)
                        data.AcT = AT_CXREG_ACT_INVALID;
                    AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
                }
            }
        }
        //-- RETCODE_OK == ret means all events received successfully, but we have to check if
        //   all received data is in range even if optional
        if ((RETCODE_OK != retStat) || (RETCODE_OK != retLac) || (RETCODE_OK != retCi) || (RETCODE_OK != retAcT))
        {
            ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }
    }
    if (!urcFound)
    {
        ret = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }
    else
    {
        if (AT_INVALID_LAC != data.Lac && AT_INVALID_CI != data.Ci)
        {
            LOG_DEBUG("CREG stat:%d lac:%" PRIu32 " ci:%" PRIu32 " AcT:%d", (int)data.Stat, data.Lac, data.Ci, data.AcT); //LCOV_EXCL_BR_LINE
        }
        else
        {
            LOG_DEBUG("CREG stat:%d", (int)data.Stat); //LCOV_EXCL_BR_LINE
        }
    }
    ret = Utils_ConvertAtResponseRetcodeToCellularRetcode(ret);
    return ret;
}

Retcode_T At_HandleUrc_CGREG(void)
{
    Retcode_T ret = RETCODE_OK;
    Retcode_T retStat = RETCODE_OK;
    Retcode_T retLac = RETCODE_OK;
    Retcode_T retCi = RETCODE_OK;
    Retcode_T retAcT = RETCODE_OK;
    Retcode_T retRac = RETCODE_OK;
    Retcode_T retOptional = RETCODE_OK;
    bool urcFound = false;
    AT_CGREG_Param_T data = {
        .N = AT_CXREG_N_INVALID,
        .Stat = AT_CXREG_STAT_INVALID,
        .Lac = AT_INVALID_LAC,
        .Ci = AT_INVALID_CI,
        .AcT = AT_CXREG_ACT_INVALID,
        .Rac = AT_INVALID_RAC};
    uint8_t *cmdArg = NULL;
    uint32_t lengthCmdArg = 0;

    ret = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)CMD_3GPP_27007_ATCGREG, strlen(CMD_3GPP_27007_ATCGREG)); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == ret)
    {
        memset(&data, 0, sizeof(data));
        urcFound = true;
        ret = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- stat//LCOV_EXCL_BR_LINE
    }
    if (RETCODE_OK == ret)
    {
        retStat = ExtractCxregStat(cmdArg, lengthCmdArg, &data.Stat); //LCOV_EXCL_BR_LINE
        if (RETCODE_OK == retStat)
        {
            switch (data.Stat)
            {
            case AT_CXREG_STAT_HOME:
            case AT_CXREG_STAT_ROAMING:
            case AT_CXREG_STAT_CSFB_NOT_PREF_HOME:
            case AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING:
                /* set device status and notify event */
                /** \todo Propagate type of CxREG to client-callback via
                 * param. */
                Engine_NotifyNewState(CELLULAR_STATE_REGISTERED, NULL, 0); //LCOV_EXCL_BR_LINE
                break;
            case AT_CXREG_STAT_NOT:
            case AT_CXREG_STAT_DENIED:
                Engine_NotifyNewState(CELLULAR_STATE_POWERON, NULL, 0); //LCOV_EXCL_BR_LINE
                break;
            case AT_CXREG_STAT_NOT_AND_SEARCH:
            case AT_CXREG_STAT_UNKNOWN:
                Engine_NotifyNewState(CELLULAR_STATE_REGISTERING, NULL, 0); //LCOV_EXCL_BR_LINE
                break;
            default:
                /* do nothing */
                break;
            }
        }

        AtResponseQueue_MarkBufferAsUnused();

        cmdArg = NULL;
        lengthCmdArg = 0;
        retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- lac//LCOV_EXCL_BR_LINE
        if (RETCODE_OK == retOptional)
        {
            retLac = ExtractCxregLac(cmdArg, lengthCmdArg, &data.Lac);
            if (RETCODE_OK != retLac)
                data.Lac = AT_INVALID_LAC;
            AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

            cmdArg = NULL;
            lengthCmdArg = 0;
            retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- ci//LCOV_EXCL_BR_LINE
            if (RETCODE_OK == retOptional)
            {
                retCi = ExtractCxregCi(cmdArg, lengthCmdArg, &data.Ci);
                if (RETCODE_OK != retCi)
                    data.Ci = AT_INVALID_CI;
                AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

                cmdArg = NULL;
                lengthCmdArg = 0;
                retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- AcT//LCOV_EXCL_BR_LINE
                if (RETCODE_OK == retOptional)
                {
                    retAcT = ExtractCxregAct(cmdArg, lengthCmdArg, &data.AcT);
                    if (RETCODE_OK != retAcT)
                        data.AcT = AT_CXREG_ACT_INVALID;
                    AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

                    cmdArg = NULL;
                    lengthCmdArg = 0;
                    retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); // Rac//LCOV_EXCL_BR_LINE
                    if (RETCODE_OK == retOptional)
                    {
                        retRac = ExtractCgregRac(cmdArg, lengthCmdArg, &data.Rac);
                        if (RETCODE_OK == retRac)
                            data.Rac = AT_INVALID_RAC;
                        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
                    }
                }
            }
        }
        //-- RETCODE_OK == ret means all events received successfully, but we have to check if
        //   all received data is in range even if optional
        if ((RETCODE_OK != retStat) || (RETCODE_OK != retLac) || (RETCODE_OK != retCi) || (RETCODE_OK != retAcT))
        {
            ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }
    }
    if (!urcFound)
    {
        ret = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }
    else
    {
        if (AT_INVALID_LAC != data.Lac && AT_INVALID_CI != data.Ci)
        {
            LOG_DEBUG("CGREG stat:%d lac:%" PRIu32 " ci:%" PRIu32 " AcT:%d", (int)data.Stat, data.Lac, data.Ci, data.AcT); //LCOV_EXCL_BR_LINE
        }
        else
        {
            LOG_DEBUG("CGREG stat:%d", (int)data.Stat); //LCOV_EXCL_BR_LINE
        }
    }
    ret = Utils_ConvertAtResponseRetcodeToCellularRetcode(ret);
    return ret;
}

Retcode_T At_HandleUrc_CEREG(void)
{
    Retcode_T ret = RETCODE_OK;
    Retcode_T retStat = RETCODE_OK;
    Retcode_T retTac = RETCODE_OK;
    Retcode_T retCi = RETCODE_OK;
    Retcode_T retAcT = RETCODE_OK;
    Retcode_T retOptional = RETCODE_OK;
    bool urcFound = false;
    AT_CEREG_Param_T data = {
        .N = AT_CXREG_N_INVALID,
        .Stat = AT_CXREG_STAT_INVALID,
        .Tac = AT_INVALID_TAC,
        .Ci = AT_INVALID_CI,
        .AcT = AT_CXREG_ACT_INVALID};
    uint8_t *cmdArg = NULL;
    uint32_t lengthCmdArg = 0;

    ret = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)CMD_3GPP_27007_ATCEREG,
                                          strlen(CMD_3GPP_27007_ATCEREG)); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == ret)
    {
        memset(&data, 0, sizeof(data));
        urcFound = true;
        ret = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT,
                                                     &cmdArg, &lengthCmdArg); //-- stat//LCOV_EXCL_BR_LINE
    }
    if (RETCODE_OK == ret)
    {
        retStat = ExtractCxregStat(cmdArg, lengthCmdArg, &data.Stat);
        if (RETCODE_OK == retStat)
        {
            switch (data.Stat)
            {
            case AT_CXREG_STAT_HOME:
            case AT_CXREG_STAT_ROAMING:
            case AT_CXREG_STAT_CSFB_NOT_PREF_HOME:
            case AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING:
                /* set device status and notify event */
                /** \todo Propagate type of CxREG to client-callback via param. */
                Engine_NotifyNewState(CELLULAR_STATE_REGISTERED, NULL, 0);
                break;
            case AT_CXREG_STAT_NOT:
            case AT_CXREG_STAT_DENIED:
                Engine_NotifyNewState(CELLULAR_STATE_POWERON, NULL, 0);
                break;
            case AT_CXREG_STAT_NOT_AND_SEARCH:
            case AT_CXREG_STAT_UNKNOWN:
                Engine_NotifyNewState(CELLULAR_STATE_REGISTERING, NULL, 0);
                break;
            default:
                /* do nothing */
                break;
            }
        }

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

        cmdArg = NULL;
        lengthCmdArg = 0;
        retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- tac
        if (RETCODE_OK == retOptional)
        {
            retTac = ExtractCeregTac(cmdArg, lengthCmdArg, &data.Tac);
            if (RETCODE_OK != retTac)
                data.Tac = AT_INVALID_TAC;
            AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

            cmdArg = NULL;
            lengthCmdArg = 0;
            retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- ci
            if (RETCODE_OK == retOptional)
            {
                retCi = ExtractCxregCi(cmdArg, lengthCmdArg, &data.Ci);
                if (RETCODE_OK != retCi)
                    data.Ci = AT_INVALID_CI;
                AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

                cmdArg = NULL;
                lengthCmdArg = 0;
                retOptional = AtResponseQueue_WaitForArbitraryCmdArg(URC_3GPP_27007_TIMEOUT, &cmdArg, &lengthCmdArg); //-- AcT//LCOV_EXCL_BR_LINE
                if (RETCODE_OK == retOptional)
                {
                    retAcT = ExtractCxregAct(cmdArg, lengthCmdArg, &data.AcT);
                    if (RETCODE_OK == retAcT)
                        data.AcT = AT_CXREG_ACT_INVALID;
                    AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
                }
            }
        }
        //-- RETCODE_OK == ret means all events received successfully, but we have to check if
        //   all received data is in range even if optional
        if ((RETCODE_OK != retStat) || (RETCODE_OK != retTac) || (RETCODE_OK != retCi) || (RETCODE_OK != retAcT))
        {
            ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }
    }

    if (!urcFound)
    {
        ret = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }
    else
    {
        if (AT_INVALID_TAC != data.Tac && AT_INVALID_CI != data.Ci)
        {
            LOG_DEBUG("CEREG stat:%d tac:%" PRIu32 " ci:%" PRIu32 " AcT:%d", (int)data.Stat, data.Tac, data.Ci, data.AcT); //LCOV_EXCL_BR_LINE
        }
        else
        {
            LOG_DEBUG("CEREG stat:%d", (int)data.Stat); //LCOV_EXCL_BR_LINE
        }
    }
    ret = Utils_ConvertAtResponseRetcodeToCellularRetcode(ret);
    return ret;
}
