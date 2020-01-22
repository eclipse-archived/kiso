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

/**
 * @file
 *
 * @brief
 * Implements Ublox specific AT commands
 */

/*###################### INCLUDED HEADERS ###########################################################################*/

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_AT_UBLOX

#include "AT_UBlox.h"
#include "AtResponseQueue.h"
#include "AtUtils.h"
#include "Engine.h"
#include "SocketService.h"
#include "HttpService.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"
#include "Kiso_Logging.h"

#include "FreeRTOS.h"
#include "task.h"

#include <inttypes.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>

/*###################### MACROS DEFINITION ##########################################################################*/
#define CMD_UBLOX_SHORT_TIMEOUT (UINT32_C(120))  /* msec */
#define CMD_UBLOX_FILE_TIMEOUT (UINT32_C(1000))  /* msec */
#define CMD_UBLOX_SIM_TIMEOUT (UINT32_C(1000))   /* msec */
#define CMD_UBLOX_HTTP_TIMEOUT (UINT32_C(4800))  /* msec */
#define CMD_UBLOX_DNS_TIMEOUT (UINT32_C(70000))  /* msec */
#define CMD_UBLOX_LONG_TIMEOUT (UINT32_C(12000)) /* msec */

#define AT_UBLOX_MAX_IP_STR_LENGTH (UINT32_C(41)) /* "255.255.255.255" or "FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF" */
#define AT_UBLOX_IPV4_GROUP_COUNT (UINT32_C(4))
#define AT_UBLOX_IPV6_GROUP_COUNT (UINT32_C(8))
#define AT_UBLOX_EMPTY_DATA ("\"\"")

#define CMD_UBLOX_ATULSTFILE "ULSTFILE"
#define CMD_UBLOX_SET_ATULSTFILE_FMT1 ("AT+" CMD_UBLOX_ATULSTFILE "=%d\r\n")
#define CMD_UBLOX_SET_ATULSTFILE_FMT2 ("AT+" CMD_UBLOX_ATULSTFILE "=%d,\"%s\"\r\n")

#define CMD_UBLOX_ATUDELFILE "UDELFILE"
#define CMD_UBLOX_SET_ATUDELFILE_FMT ("AT+" CMD_UBLOX_ATUDELFILE "=\"%s\"\r\n")

#define CMD_UBLOX_ATURDFILE "URDFILE"
#define CMD_UBLOX_SET_ATURDFILE_FMT ("AT+" CMD_UBLOX_ATURDFILE "=\"%s\"\r\n")

#define CMD_UBLOX_ATUDWNFILE "UDWNFILE"
#define CMD_UBLOX_SET_ATUDWNFILE_FMT ("AT+" CMD_UBLOX_ATUDWNFILE "=\"%s\",%d\r\n")

#define CMD_UBLOX_ATURDBLOCK "URDBLOCK"
#define CMD_UBLOX_SET_ATURDBLOCK_FMT ("AT+" CMD_UBLOX_ATURDBLOCK "=\"%s\",%d,%d\r\n")

#define CMD_UBLOX_ATUHTTP "UHTTP"
#define CMD_UBLOX_SET_ATUHTTP1_FMT ("AT+" CMD_UBLOX_ATUHTTP "=%d\r\n")
#define CMD_UBLOX_SET_ATUHTTP2_FMT ("AT+" CMD_UBLOX_ATUHTTP "=%d,%d,\"%s\"\r\n")
#define CMD_UBLOX_SET_ATUHTTP3_FMT ("AT+" CMD_UBLOX_ATUHTTP "=%d,%d,%d\r\n")

#define CMD_UBLOX_ATUHTTPC "UHTTPC"
#define CMD_UBLOX_ATUUHTTPCR "UUHTTPCR"
#define CMD_UBLOX_SET_ATUHTTPC1_FMT ("AT+" CMD_UBLOX_ATUHTTPC "=%d,%d,\"%s\",\"%s\"\r\n")
#define CMD_UBLOX_SET_ATUHTTPC2_FMT ("AT+" CMD_UBLOX_ATUHTTPC "=%d,%d,\"%s\",\"%s\",\"%s\",%d\r\n")
#define CMD_UBLOX_SET_ATUHTTPC3_FMT ("AT+" CMD_UBLOX_ATUHTTPC "=%d,%d,\"%s\",\"%s\",\"%s\",%d,\"%s\"\r\n")

#define CMD_UBLOX_ATURAT "URAT"
#define CMD_UBLOX_SET_ATURAT1_FMT ("AT+" CMD_UBLOX_ATURAT "=%d\r\n")
#define CMD_UBLOX_SET_ATURAT2_FMT ("AT+" CMD_UBLOX_ATURAT "=%d,%d\r\n")
#define CMD_UBLOX_SET_ATURAT3_FMT ("AT+" CMD_UBLOX_ATURAT "=%d,%d,%d\r\n")

#define CMD_UBLOX_ATUMNOPROF "UMNOPROF"
#define CMD_UBLOX_SET_ATUMNOPROF_FMT ("AT+" CMD_UBLOX_ATUMNOPROF "=%d\r\n")
#define CMD_UBLOX_GET_ATUMNOPROF ("AT+" CMD_UBLOX_ATUMNOPROF "?\r\n")

#define CMD_UBLOX_ATUBANDMASK "UBANDMASK"
#define CMD_UBLOX_SET_ATUBANDMASK_FMT ("AT+" CMD_UBLOX_ATUBANDMASK "=%d,%d,%d\r\n")

#define CMD_UBLOX_ATUSOCR "USOCR"
#define CMD_UBLOX_SET_ATUSOCR_FMT ("AT+" CMD_UBLOX_ATUSOCR "=%d,%d\r\n")

#define CMD_UBLOX_ATUSOCO "USOCO"
#define CMD_UBLOX_SET_ATUSOCO_FMTIPV4 ("AT+" CMD_UBLOX_ATUSOCO "=%d,\"%d.%d.%d.%d\",%d\r\n")
#define CMD_UBLOX_SET_ATUSOCO_FMTIPV6 ("AT+" CMD_UBLOX_ATUSOCO "=%d,\"%x:%x:%x:%x:%x:%x:%x:%x\",%d\r\n")

#define CMD_UBLOX_ATUSOWR "USOWR"
#define CMD_UBLOX_SET_ATUSOWR_FMTBASE ("AT+" CMD_UBLOX_ATUSOWR "=%d,%d,\"%.*s\"\r\n")
#define CMD_UBLOX_SET_ATUSOWR_FMTHEX ("AT+" CMD_UBLOX_ATUSOWR "=%d,%d,\"%n%.*s\"\r\n")
#define CMD_UBLOX_ATUSOWR_FOOTER ("\"\r\n")

#define CMD_UBLOX_ATUSOST "USOST"
#define CMD_UBLOX_SET_ATUSOST_FMTIPV4BASE ("AT+" CMD_UBLOX_ATUSOST "=%d,\"%d.%d.%d.%d\",%d,%d,\"%.*s\"\r\n")
#define CMD_UBLOX_SET_ATUSOST_FMTIPV6BASE ("AT+" CMD_UBLOX_ATUSOST "=%d,\"%x:%x:%x:%x:%x:%x:%x:%x\",%d,%d,\"%.*s\"\r\n")
#define CMD_UBLOX_SET_ATUSOST_FMTIPV4HEX ("AT+" CMD_UBLOX_ATUSOST "=%d,\"%d.%d.%d.%d\",%d,%d,\"%n%.*s\"\r\n")
#define CMD_UBLOX_SET_ATUSOST_FMTIPV6HEX ("AT+" CMD_UBLOX_ATUSOST "=%d,\"%x:%x:%x:%x:%x:%x:%x:%x\",%d,%d,\"%n%.*s\"\r\n")

#define CMD_UBLOX_ATUSORD "USORD"
#define CMD_UBLOX_ATUUSORD "UUSORD"
#define CMD_UBLOX_SET_ATUSORD_FMTHEX ("AT+" CMD_UBLOX_ATUSORD "=%d,%d\r\n")

#define CMD_UBLOX_ATUSORF "USORF"
#define CMD_UBLOX_ATUUSORF "UUSORF"
#define CMD_UBLOX_SET_ATUSORF_FMTHEX ("AT+" CMD_UBLOX_ATUSORF "=%d,%d\r\n")

#define CMD_UBLOX_ATUSOLI "USOLI"
#define CMD_UBLOX_ATUUSOLI "UUSOLI"
#define CMD_UBLOX_SET_ATUSOLI_FMT ("AT+" CMD_UBLOX_ATUSOLI "=%d,%d\r\n")

#define CMD_UBLOX_ATUSOCL "USOCL"
#define CMD_UBLOX_ATUUSOCL "UUSOCL"
#define CMD_UBLOX_SET_ATUSOCL_FMT ("AT+" CMD_UBLOX_ATUSOCL "=%d\r\n")

#define CMD_UBLOX_ATUDCONF "UDCONF"
#define CMD_UBLOX_SET_ATUDCONF_FMTHEXMODE ("AT+" CMD_UBLOX_ATUDCONF "=%d,%d\r\n")
#define CMD_UBLOX_GET_ATUDCONF_FMT ("AT+" CMD_UBLOX_ATUDCONF "=%d\r\n")

#define CMD_UBLOX_ATCCID "CCID"
#define CMD_UBLOX_GET_ATCCID ("AT+" CMD_UBLOX_ATCCID "?\r\n")

#define CMD_UBLOX_ATUTEST "UTEST"
#define CMD_UBLOX_SET_ATUTEST_FMT ("AT+" CMD_UBLOX_ATUTEST "=%d\r\n")

#define CMD_UBLOX_ATUDNSRN "UDNSRN"
#define CMD_UBLOX_SET_ATUDNSRN_FMT1 ("AT+" CMD_UBLOX_ATUDNSRN "=%d,\"%s\"\r\n")
#define CMD_UBLOX_SET_ATUDNSRN_FMT2 ("AT+" CMD_UBLOX_ATUDNSRN "=%d,\"%d.%d.%d.%d\"\r\n")
#define CMD_UBLOX_SET_ATUDNSRN_FMT3 ("AT+" CMD_UBLOX_ATUDNSRN "=%d,\"%x:%x:%x:%x:%x:%x:%x:%x\"\r\n")

/*###################### LOCAL FUNCTIONS DECLARATION ################################################################*/

static Retcode_T ExtractUmnoprofMno(const uint8_t *data, uint32_t length, AT_UMNOPROF_Mno_T *value);
static Retcode_T ExtractUbloxAddress(const uint8_t *addressBuff, uint32_t addressBuffLen, AT_UBlox_Address_T *parsedAddress);
static Retcode_T HandleDomainToIpUDNSRN(const AT_UDNSRN_Param_T *param, AT_UDNSRN_Resp_T *resp);
static Retcode_T HandleIpToDomainUDNSRN(const AT_UDNSRN_Param_T *param, AT_UDNSRN_Resp_T *resp);
static uint8_t ToHexChar(uint8_t quintetIndex, uint8_t byte);
static uint8_t FromHexChar(uint8_t HexChar);
static Retcode_T EncodePayloadAsHex(const uint8_t *binData, uint32_t binLength, char *hexData, uint32_t hexLength);
static Retcode_T CheckForIllegalCharacters(const uint8_t *data, uint32_t length);
static Retcode_T PrepareSendingWithBaseEncoding(char *sendBuffer, uint32_t sendBufferLength, const AT_USOWR_Param_T *param, uint32_t *length);
static Retcode_T PrepareSendToWithBaseEncoding(char *sendBuffer, uint32_t sendBufferLength, const AT_USOST_Param_T *param, uint32_t *length);
static Retcode_T PrepareSendingWithHexEncoding(char *sendBuffer, uint32_t sendBufferLength, const AT_USOWR_Param_T *param, uint32_t *length);
static Retcode_T PrepareSendToWithHexEncoding(char *sendBuffer, uint32_t sendBufferLength, const AT_USOST_Param_T *param, uint32_t *length);
static Retcode_T HandleHexModeUSORD(const AT_USORD_Param_T *param, AT_USORD_Resp_T *resp);
static Retcode_T HandleHexModeUSORF(const AT_USORF_Param_T *param, AT_USORF_Resp_T *resp);
static Retcode_T ParseIPv6RightToLeft(const uint8_t *addressBuff, uint32_t addressBuffLen, AT_UBlox_Address_T *parsedAddress, uint32_t alreadyParsedGroups);
static Retcode_T ParseIPv6LeftToRight(const uint8_t *addressBuff, uint32_t addressBuffLen, AT_UBlox_Address_T *parsedAddress);
static Retcode_T ParseIPv4(const uint8_t *addressBuff, uint32_t addressBuffLen, AT_UBlox_Address_T *parsedAddress);
static Retcode_T HandleUrcUUSORX(const char *cmd);

/*###################### VARIABLES DECLARATION ######################################################################*/

/*###################### EXPOSED FUNCTIONS IMPLEMENTATION ###########################################################*/

Retcode_T At_Set_UHTTP(const AT_UHTTP_Param_T *param)
{
    assert(NULL != param);

    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;
    uint32_t timeout = CMD_UBLOX_SHORT_TIMEOUT;
    switch (param->Opcode)
    {
    case AT_UHTTP_OPCODE_SERVER_NAME:
        timeout = CMD_UBLOX_DNS_TIMEOUT;
        // intentional fall through
    case AT_UHTTP_OPCODE_SERVER_IP:
    case AT_UHTTP_OPCODE_USERNAME:
    case AT_UHTTP_OPCODE_PASSWORD:
        len = snprintf(Engine_AtSendBuffer,
                       sizeof(Engine_AtSendBuffer),
                       CMD_UBLOX_SET_ATUHTTP2_FMT,
                       (int)param->ProfileId,
                       param->Opcode,
                       param->Value.String);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
        break;
    case AT_UHTTP_OPCODE_SERVER_PORT:
    case AT_UHTTP_OPCODE_SECURE_OPTION:
        len = snprintf(Engine_AtSendBuffer,
                       sizeof(Engine_AtSendBuffer),
                       CMD_UBLOX_SET_ATUHTTP3_FMT,
                       (int)param->ProfileId,
                       param->Opcode,
                       (int)param->Value.Numeric);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
        break;
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_LONG_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(timeout, retcode);
    }
    return retcode;
}

Retcode_T At_Set_UHTTPC(const AT_UHTTPC_Param_T *param)
{
    assert(NULL != param);

    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;
    switch (param->Command)
    {
    case AT_UHTTPC_COMMAND_HEAD:
    case AT_UHTTPC_COMMAND_GET:
    case AT_UHTTPC_COMMAND_DELETE:
        len = snprintf(Engine_AtSendBuffer,
                       sizeof(Engine_AtSendBuffer),
                       CMD_UBLOX_SET_ATUHTTPC1_FMT,
                       (int)param->ProfileId, param->Command,
                       param->PathOnServer,
                       param->ResponseFilename);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
        break;
    case AT_UHTTPC_COMMAND_POST_DATA:
    case AT_UHTTPC_COMMAND_POST_FILE:
    case AT_UHTTPC_COMMAND_PUT:
        len = snprintf(Engine_AtSendBuffer,
                       sizeof(Engine_AtSendBuffer),
                       CMD_UBLOX_SET_ATUHTTPC2_FMT,
                       (int)param->ProfileId,
                       param->Command,
                       param->PathOnServer,
                       param->ResponseFilename,
                       param->Payload,
                       param->ContentType);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
        break;

    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, 2 * CMD_UBLOX_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(5000, retcode);
    }
    return retcode;
}

Retcode_T At_Set_URAT(const AT_URAT_Param_T *param)
{
    assert(NULL != param);

    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    switch (param->SelectedAcT)
    {
    case AT_URAT_SELECTEDACT_GSM_GPRS_EGPRS:
    case AT_URAT_SELECTEDACT_UMTS:
    case AT_URAT_SELECTEDACT_LTE:
    case AT_URAT_SELECTEDACT_LTEM1:
    case AT_URAT_SELECTEDACT_LTENB1:
    case AT_URAT_SELECTEDACT_GPRS_EGPRS:
        /* single mode */
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATURAT1_FMT,
                       (int)param->SelectedAcT);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
        break;
    case AT_URAT_SELECTEDACT_GSM_UMTS:
    case AT_URAT_SELECTEDACT_GSM_LTE:
    case AT_URAT_SELECTEDACT_UMTS_LTE:
        /* dual mode */
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATURAT2_FMT,
                       (int)param->SelectedAcT, (int)param->PreferredAcT);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
        break;
    case AT_URAT_SELECTEDACT_GSM_UMTS_LTE:
        /* tri mode */
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATURAT3_FMT,
                       (int)param->SelectedAcT, (int)param->PreferredAcT, (int)param->SecondPreferredAcT);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
        break;
    case AT_URAT_SELECTEDACT_INVALID:
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_UMNOPROF(AT_UMNOPROF_Mno_T mno)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUMNOPROF_FMT,
                   (int)mno);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Get_UMNOPROF(AT_UMNOPROF_Mno_T *mno)
{
    assert(NULL != mno);

    Retcode_T retcode = RETCODE_OK;
    uint8_t *buffer = NULL;
    uint32_t bufferLen = 0;
    //LCOV_EXCL_BR_START
    retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)CMD_UBLOX_GET_ATUMNOPROF,
                                           (uint32_t)strlen(CMD_UBLOX_GET_ATUMNOPROF), CMD_UBLOX_SHORT_TIMEOUT); //LCOV_EXCL_BR_STOP

    if (RETCODE_OK == retcode)
    {
        //LCOV_EXCL_BR_START
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SHORT_TIMEOUT,
                                                  (const uint8_t *)CMD_UBLOX_ATUMNOPROF,
                                                  (uint32_t)strlen(CMD_UBLOX_ATUMNOPROF)); //LCOV_EXCL_BR_STOP
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

        retcode = ExtractUmnoprofMno(buffer, bufferLen, mno);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Set_UDWNFILE(const AT_UDWNFILE_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len;
    AtResponseQueueEntry_T *entry;
    uint32_t eventCount;
    uint32_t i;
    assert(NULL != param);

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUDWNFILE_FMT,
                   param->Filename, (int)param->DataSize);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    AtResponseQueue_SetEventMask(AT_EVENT_TYPE_ALL);

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_FILE_TIMEOUT); //LCOV_EXCL_BR_LINE
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommand(param->Data, param->DataSize); //LCOV_EXCL_BR_LINE
    }

    /* we wait some time in order to fill up the queue with responses*/
    vTaskDelay(2 * CMD_UBLOX_FILE_TIMEOUT);

    eventCount = AtResponseQueue_GetEventCount(); //LCOV_EXCL_BR_LINE
    for (i = 0; i < eventCount; i++)
    {
        retcode = AtResponseQueue_GetEvent(1000, &entry); //LCOV_EXCL_BR_LINE
        if (entry->Type == AT_EVENT_TYPE_MISC)
        {
            AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
        }
        else
        {
            retcode = Utils_WaitForAndHandleResponseCode(4 * CMD_UBLOX_FILE_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
        }
    }
    AtResponseQueue_SetEventMask(AT_EVENT_TYPE_ALL - AT_EVENT_TYPE_MISC);
    return retcode;
}

Retcode_T At_Set_UDELFILE(const AT_UDELFILE_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len;
    assert(NULL != param);
    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUDELFILE_FMT,
                   param->Filename);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_FILE_TIMEOUT);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_FILE_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_ULSTFILE(AT_ULSTFILE_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len;
    uint8_t *buffer;
    uint32_t bufferLen;

    assert(NULL != param);
    switch (param->Opcode)
    {
    case AT_ULSTFILE_OPCODE_SIZE:
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATULSTFILE_FMT2,
                       (int)param->Opcode, param->Filename);
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }

        break;

    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        break;
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SHORT_TIMEOUT,
                                                  (const uint8_t *)CMD_UBLOX_ATULSTFILE, (uint32_t)strlen(CMD_UBLOX_ATULSTFILE)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT,
                                                         &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
    }
    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_Strtol(buffer, bufferLen, &tmp);
        param->Filesize = (uint32_t)tmp;
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Set_URDBLOCK(const AT_URDBLOCK_Param_T *param, AT_URDBLOCK_Resp_T *resp)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *buffer;
    uint32_t bufferLen;
    int32_t len = 0;
    uint32_t readLength = 0;
    AtResponseQueueEntry_T *entry;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATURDBLOCK_FMT,
                   param->Filename, (int)param->Offset, (int)param->Size);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    AtResponseQueue_SetEventMask(AT_EVENT_TYPE_ALL);

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_FILE_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    retcode = AtResponseQueue_GetEvent(1000, &entry); //LCOV_EXCL_BR_LINE
    if (entry->Type == AT_EVENT_TYPE_MISC)
    {
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_FILE_TIMEOUT,
                                                  (const uint8_t *)CMD_UBLOX_ATURDBLOCK, (uint32_t)strlen(CMD_UBLOX_ATURDBLOCK)); //LCOV_EXCL_BR_LINE
    }
    // here we read back the Filename:
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT,
                                                         &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
        if (RETCODE_OK == retcode)
        {
            if (bufferLen > UBLOX_MAX_FILE_NAME_LENGTH + 2 || bufferLen <= 2) /* filename + two " */
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
            }
            else
            {
                memcpy(resp->Filename, buffer + 1, bufferLen - 1);
                /* ensure zero terminaltion, account for the " at the beginning. */
                resp->Filename[bufferLen - 2] = '\0';
            }
        };
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    // here we read back the length:
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT,
                                                         &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
        if (RETCODE_OK == retcode)
        {
            int32_t size = 0;
            retcode = Utils_Strtol(buffer, bufferLen, &size);
            resp->Size = (uint32_t)size;
        }
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    // here we read back the first line of data
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_FILE_TIMEOUT, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
        if (RETCODE_OK == retcode)
        {
            /* FIXME: This is right now very HTTP specific! */
            if (param->Size > bufferLen - 1)
            {
                memcpy(resp->Data, buffer + 1, bufferLen - 1);
                readLength = bufferLen - 1;
                if (bufferLen - param->Size >= 1)
                {
                    resp->Data[bufferLen - 1] = '\r';
                    readLength++;
                }
                if (bufferLen - param->Size >= 2)
                {
                    resp->Data[bufferLen] = '\n';
                    readLength++;
                }
            }
            else
            {
                memcpy(resp->Data, buffer + 1, param->Size);
                readLength = param->Size;
            }
        }
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }
    // here we read back the rest of the data
    while (retcode == RETCODE_OK && readLength < param->Size)
    {
        retcode = AtResponseQueue_WaitForMiscContent(10000, &buffer, &bufferLen); //LCOV_EXCL_BR_LINE
        uint32_t readLimit = param->Size - readLength;
        memcpy(resp->Data + readLength, buffer, bufferLen > readLimit ? readLimit : bufferLen);
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
        readLength += bufferLen;
    }
    retcode = AtResponseQueue_GetEvent(1000, &entry);
    if (entry->Type == AT_EVENT_TYPE_MISC)
    {
        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_FILE_TIMEOUT, retcode);
    }

    AtResponseQueue_SetEventMask(AT_EVENT_TYPE_ALL - AT_EVENT_TYPE_MISC);

    return retcode;
}

Retcode_T At_Set_UBANDMASK(const AT_UBANDMASK_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUBANDMASK_FMT,
                   (int)param->Rat, (int)param->Bitmask1 & 0xFFFFFFFF, (int)param->Bitmask2 & 0xFFFFFFFF);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_USOCR(const AT_USOCR_Param_T *param, uint32_t *socketId)
{
    assert(NULL != param);
    assert(NULL != socketId);

    Retcode_T retcode = RETCODE_OK;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;
    int32_t len = 0;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUSOCR_FMT,
                   (int)param->Protocol, (int)param->LocalPort);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SHORT_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATUSOCR, (uint32_t)strlen(CMD_UBLOX_ATUSOCR)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_Strtol(arg, argLen, &tmp);
        *socketId = (uint32_t)tmp;

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Set_USOCO(const AT_USOCO_Param_T *param)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    switch (param->RemoteAddr.Type)
    {
    case AT_UBLOX_ADDRESSTYPE_IPV4:
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUSOCO_FMTIPV4,
                       (int)param->Socket,
                       (int)param->RemoteAddr.Address.IPv4[3],
                       (int)param->RemoteAddr.Address.IPv4[2],
                       (int)param->RemoteAddr.Address.IPv4[1],
                       (int)param->RemoteAddr.Address.IPv4[0],
                       (int)param->RemotePort);
        break;
    case AT_UBLOX_ADDRESSTYPE_IPV6:
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUSOCO_FMTIPV6,
                       (int)param->Socket,
                       (int)param->RemoteAddr.Address.IPv6[7],
                       (int)param->RemoteAddr.Address.IPv6[6],
                       (int)param->RemoteAddr.Address.IPv6[5],
                       (int)param->RemoteAddr.Address.IPv6[4],
                       (int)param->RemoteAddr.Address.IPv6[3],
                       (int)param->RemoteAddr.Address.IPv6[2],
                       (int)param->RemoteAddr.Address.IPv6[1],
                       (int)param->RemoteAddr.Address.IPv6[0],
                       (int)param->RemotePort);
        break;
    default:
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_LONG_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_USOWR(const AT_USOWR_Param_T *param, AT_USOWR_Resp_T *resp)
{
    assert(NULL != param);
    assert(NULL != resp);

    Retcode_T retcode = RETCODE_OK;
    uint32_t len = 0;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;

    if (0 >= param->Length)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (RETCODE_OK == retcode)
    {
        switch (param->Encoding)
        {
        case AT_UBLOX_PAYLOADENCODING_BASE:
            retcode = PrepareSendingWithBaseEncoding(
                Engine_AtSendBuffer,
                sizeof(Engine_AtSendBuffer),
                param,
                &len);
            break;
        case AT_UBLOX_PAYLOADENCODING_HEX:
            retcode = PrepareSendingWithHexEncoding(
                Engine_AtSendBuffer,
                sizeof(Engine_AtSendBuffer),
                param,
                &len);
            break;
        case AT_UBLOX_PAYLOADENCODING_BINARY:
            /* Currently not supported, sorry! ... maybe your first PR? :) */
            /** \todo: u-blox out-of-AT sender mode (initiated by '@' during sending)
             * needs to be implemented. One idea would be to introduce a new
             * AT-parser state which triggers on the '@' char. Other vendors rely
             * this out-of-AT mode (e.g. Quectel) to send data over sockets. */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            break;
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SHORT_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATUSOWR, (uint32_t)strlen(CMD_UBLOX_ATUSOWR)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, INT32_MAX);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

        resp->Socket = (uint32_t)tmp;
        assert(resp->Socket == param->Socket);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, INT32_MAX);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

        resp->Length = (uint32_t)tmp;
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Set_USOST(const AT_USOST_Param_T *param, AT_USOST_Resp_T *resp)
{
    assert(NULL != param);
    assert(NULL != resp);

    Retcode_T retcode = RETCODE_OK;
    uint32_t len = 0;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;

    if (0 >= param->Length)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (RETCODE_OK == retcode)
    {
        switch (param->Encoding)
        {
        case AT_UBLOX_PAYLOADENCODING_BASE:
            retcode = PrepareSendToWithBaseEncoding(
                Engine_AtSendBuffer,
                sizeof(Engine_AtSendBuffer),
                param,
                &len);
            break;
        case AT_UBLOX_PAYLOADENCODING_HEX:
            retcode = PrepareSendToWithHexEncoding(
                Engine_AtSendBuffer,
                sizeof(Engine_AtSendBuffer),
                param,
                &len);
            break;
        case AT_UBLOX_PAYLOADENCODING_BINARY:
            /* Currently not supported, sorry! ... maybe your first PR? :) */
            /** \todo: u-blox out-of-AT sender mode (initiated by '@' during sending)
             * needs to be implemented. One idea would be to introduce a new
             * AT-parser state which triggers on the '@' char. Other vendors rely
             * this out-of-AT mode (e.g. Quectel) to send data over sockets. */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            break;
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SHORT_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATUSOST, (uint32_t)strlen(CMD_UBLOX_ATUSOST)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, INT32_MAX);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

        resp->Socket = (uint32_t)tmp;
        assert(resp->Socket == param->Socket);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, INT32_MAX);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

        resp->Length = (uint32_t)tmp;
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Set_USORD(const AT_USORD_Param_T *param, AT_USORD_Resp_T *resp)
{
    assert(NULL != param);
    assert(NULL != resp);
    assert(NULL != resp->Data || 0 >= param->Length);

    Retcode_T retcode = RETCODE_OK;

    switch (param->Encoding)
    {
    case AT_UBLOX_PAYLOADENCODING_HEX:
        retcode = HandleHexModeUSORD(param, resp);
        break;
    case AT_UBLOX_PAYLOADENCODING_BINARY:
        /* Currently not supported, sorry! ... maybe your first PR? :) */
        /** \todo: The current parser does not support arbitrary ASCII
         * characters [0x00,0xFF]. */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        break;
    default:
    case AT_UBLOX_PAYLOADENCODING_BASE:
        /* Not applicable for receiving, rx bytes are always considered
         * binary. */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }

    return retcode;
}

Retcode_T At_Set_USORF(const AT_USORF_Param_T *param, AT_USORF_Resp_T *resp)
{
    assert(NULL != param);
    assert(NULL != resp);
    assert(NULL != resp->Data || 0 >= param->Length);

    Retcode_T retcode = RETCODE_OK;

    switch (param->Encoding)
    {
    case AT_UBLOX_PAYLOADENCODING_HEX:
        retcode = HandleHexModeUSORF(param, resp);
        break;
    case AT_UBLOX_PAYLOADENCODING_BINARY:
        /* Currently not supported, sorry! ... maybe your first PR? :) */
        /** \todo: The current parser does not support arbitrary ASCII
         * characters [0x00,0xFF]. */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        break;
    default:
    case AT_UBLOX_PAYLOADENCODING_BASE:
        /* Not applicable for receiving, rx bytes are always considered
         * binary. */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }

    return retcode;
}

Retcode_T At_Set_USOLI(const AT_USOLI_Param_T *param)
{
    assert(NULL != param);

    Retcode_T retcode = RETCODE_OK;

    int32_t len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUSOLI_FMT,
                           (int)param->Socket,
                           (int)param->Port);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_USOCL(const AT_USOCL_Param_T *param)
{
    assert(NULL != param);

    Retcode_T retcode = RETCODE_OK;

    int32_t len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUSOCL_FMT,
                           (int)param->Socket);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_LONG_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Set_UDCONF(const AT_UDCONF_Param_T *param)
{
    assert(NULL != param);
    assert(NULL != param->Value);

    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;

    switch (param->Config)
    {
    case AT_UDCONF_CONFIG_HEXMODE:
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUDCONF_FMTHEXMODE,
                       (int)param->Config,
                       (int)*((bool *)param->Value));
        break;
    case AT_UDCONF_CONFIG_INVALID:
    default:
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

Retcode_T At_Get_UDCONF(AT_UDCONF_Param_T *param)
{
    assert(NULL != param);
    assert(NULL != param->Value);

    Retcode_T retcode = RETCODE_OK;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;

    int32_t len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_GET_ATUDCONF_FMT,
                           (int)param->Config);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SHORT_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATUDCONF, strlen(CMD_UBLOX_ATUDCONF)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        switch (param->Config)
        {
        case AT_UDCONF_CONFIG_HEXMODE:
            retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE

            if (RETCODE_OK == retcode)
            {
                int32_t tmp = 0;
                retcode = Utils_Strtol(arg, argLen, &tmp);
                if (RETCODE_OK == retcode && tmp != (int32_t)param->Config)
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
                }

                AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
            }

            if (RETCODE_OK == retcode)
            {
                retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
            }

            if (RETCODE_OK == retcode)
            {
                int32_t tmp = 0;
                retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, 1);
                *((bool *)param->Value) = (bool)tmp;

                AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
            }
            break;
        case AT_UDCONF_CONFIG_INVALID:
        default:
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Get_CCID(AT_CCID_Resp_T *resp)
{
    assert(NULL != resp);

    Retcode_T retcode = RETCODE_OK;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;

    retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)CMD_UBLOX_GET_ATCCID, strlen(CMD_UBLOX_GET_ATCCID), CMD_UBLOX_SIM_TIMEOUT); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SIM_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATCCID, strlen(CMD_UBLOX_ATCCID)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SIM_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        memset(resp->Iccid, 0, sizeof(resp->Iccid));
        switch (argLen)
        {
        case AT_CCID_TYPE_19CHAR:
            resp->Type = AT_CCID_TYPE_19CHAR;
            memcpy(resp->Iccid, arg, (size_t)resp->Type);
            break;
        case AT_CCID_TYPE_20CHAR:
            resp->Type = AT_CCID_TYPE_20CHAR;
            memcpy(resp->Iccid, arg, (size_t)resp->Type);
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            break;
        }

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SIM_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Set_UDNSRN(const AT_UDNSRN_Param_T *param, AT_UDNSRN_Resp_T *resp)
{
    assert(NULL != param);
    assert(NULL != resp);

    switch (param->ResolutionType)
    {
    case AT_UDNSRN_RESOLUTIONTYPE_DOMAINTOIP:
        return HandleDomainToIpUDNSRN(param, resp);
    case AT_UDNSRN_RESOLUTIONTYPE_IPTODOMAIN:
        return HandleIpToDomainUDNSRN(param, resp);
    case AT_UDNSRN_RESOLUTIONTYPE_INVALID:
    default:
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
}

/* *** URC HANDLERS ********************************************************* */

Retcode_T At_HandleUrc_UUSORD(void)
{
    return HandleUrcUUSORX(CMD_UBLOX_ATUUSORD);
}

Retcode_T At_HandleUrc_UUSORF(void)
{
    return HandleUrcUUSORX(CMD_UBLOX_ATUUSORF);
}

Retcode_T At_HandleUrc_UUSOLI(void)
{
    Retcode_T retcode = RETCODE_OK;
    bool urcFound = false;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;
    uint32_t createdSocketId = 0;
    AT_UBlox_Address_T remoteIp;
    remoteIp.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    uint16_t remotePort = 0;
    uint32_t listeningSocketId = 0;
    AT_UBlox_Address_T localIp;
    localIp.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    uint16_t listeningPort = 0;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)CMD_UBLOX_ATUUSOLI, (uint32_t)strlen(CMD_UBLOX_ATUUSOLI)); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        urcFound = true;
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Parse <socket> */
        int32_t tmp = 0;
        retcode = Utils_Strtol(arg, argLen, &tmp);
        createdSocketId = (uint32_t)tmp;

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Parse <ip_address> */
        retcode = ExtractUbloxAddress(arg, argLen, &remoteIp);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Parse <port> */
        int32_t tmp = 0;
        retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, UINT16_MAX);
        remotePort = (uint16_t)tmp;

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Parse <listening_socket> */
        int32_t tmp = 0;
        retcode = Utils_Strtol(arg, argLen, &tmp);
        listeningSocketId = (uint32_t)tmp;

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Parse <local_ip_address> */
        retcode = ExtractUbloxAddress(arg, argLen, &localIp);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Parse <listening_port> */
        int32_t tmp = 0;
        retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, UINT16_MAX);
        listeningPort = (uint32_t)tmp;

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        switch (remoteIp.Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            switch (localIp.Type)
            {
            case AT_UBLOX_ADDRESSTYPE_IPV4:
                //LCOV_EXCL_BR_START
                LOG_DEBUG("UUSOLI socket:%d "
                          "ip_address:\"%d.%d.%d.%d\" port:%d "
                          "listening_socket:%d local_ip_address:\"%d.%d.%d.%d\" listening_port:%d",
                          (int)createdSocketId,
                          (int)remoteIp.Address.IPv4[3],
                          (int)remoteIp.Address.IPv4[2],
                          (int)remoteIp.Address.IPv4[1],
                          (int)remoteIp.Address.IPv4[0],
                          (int)remotePort,
                          (int)listeningSocketId,
                          (int)localIp.Address.IPv4[3],
                          (int)localIp.Address.IPv4[2],
                          (int)localIp.Address.IPv4[1],
                          (int)localIp.Address.IPv4[0],
                          (int)listeningPort); //LCOV_EXCL_BR_STOP
                break;
            case AT_UBLOX_ADDRESSTYPE_IPV6:
                //LCOV_EXCL_BR_START
                LOG_DEBUG("UUSOLI socket:%d "
                          "ip_address:\"%d.%d.%d.%d\" port:%d "
                          "listening_socket:%d local_ip_address:\"%d:%d:%d:%d:%d:%d:%d:%d\" listening_port:%d",
                          (int)createdSocketId,
                          (int)remoteIp.Address.IPv4[3],
                          (int)remoteIp.Address.IPv4[2],
                          (int)remoteIp.Address.IPv4[1],
                          (int)remoteIp.Address.IPv4[0],
                          (int)remotePort,
                          (int)listeningSocketId,
                          (int)localIp.Address.IPv6[7],
                          (int)localIp.Address.IPv6[6],
                          (int)localIp.Address.IPv6[5],
                          (int)localIp.Address.IPv6[4],
                          (int)localIp.Address.IPv6[3],
                          (int)localIp.Address.IPv6[2],
                          (int)localIp.Address.IPv6[1],
                          (int)localIp.Address.IPv6[0],
                          (int)listeningPort); //LCOV_EXCL_BR_STOP
                break;
            default:
                break;
            }
            break;
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            switch (localIp.Type)
            {
            case AT_UBLOX_ADDRESSTYPE_IPV4:
                //LCOV_EXCL_BR_START
                LOG_DEBUG("UUSOLI socket:%d "
                          "ip_address:\"%d:%d:%d:%d:%d:%d:%d:%d\" port:%d "
                          "listening_socket:%d local_ip_address:\"%d.%d.%d.%d\" listening_port:%d",
                          (int)createdSocketId,
                          (int)remoteIp.Address.IPv4[3],
                          (int)remoteIp.Address.IPv4[2],
                          (int)remoteIp.Address.IPv4[1],
                          (int)remoteIp.Address.IPv4[0],
                          (int)remotePort,
                          (int)listeningSocketId,
                          (int)localIp.Address.IPv4[3],
                          (int)localIp.Address.IPv4[2],
                          (int)localIp.Address.IPv4[1],
                          (int)localIp.Address.IPv4[0],
                          (int)listeningPort); //LCOV_EXCL_BR_STOP
                break;
            case AT_UBLOX_ADDRESSTYPE_IPV6:
                //LCOV_EXCL_BR_START
                LOG_DEBUG("UUSOLI socket:%d "
                          "ip_address:\"%d:%d:%d:%d:%d:%d:%d:%d\" port:%d "
                          "listening_socket:%d local_ip_address:\"%d:%d:%d:%d:%d:%d:%d:%d\" listening_port:%d",
                          (int)createdSocketId,
                          (int)remoteIp.Address.IPv4[3],
                          (int)remoteIp.Address.IPv4[2],
                          (int)remoteIp.Address.IPv4[1],
                          (int)remoteIp.Address.IPv4[0],
                          (int)remotePort,
                          (int)listeningSocketId,
                          (int)localIp.Address.IPv6[7],
                          (int)localIp.Address.IPv6[6],
                          (int)localIp.Address.IPv6[5],
                          (int)localIp.Address.IPv6[4],
                          (int)localIp.Address.IPv6[3],
                          (int)localIp.Address.IPv6[2],
                          (int)localIp.Address.IPv6[1],
                          (int)localIp.Address.IPv6[0],
                          (int)listeningPort); //LCOV_EXCL_BR_STOP
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }

        SocketService_NotifySocketAccepted(createdSocketId, &remoteIp, remotePort, listeningSocketId, &localIp, listeningPort);
    }

    if (!urcFound)
    {
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    return Utils_ConvertAtResponseRetcodeToCellularRetcode(retcode);
}

Retcode_T At_HandleUrc_UUSOCL(void)
{
    Retcode_T retcode = RETCODE_OK;
    bool urcFound = false;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;
    uint32_t socketId = 0;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)CMD_UBLOX_ATUUSOCL, (uint32_t)strlen(CMD_UBLOX_ATUUSOCL)); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        urcFound = true;
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_Strtol(arg, argLen, &tmp);
        socketId = (uint32_t)tmp;

        AtResponseQueue_MarkBufferAsUnused();
    }

    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("UUSOCL socket:%d", (int)socketId);
        SocketService_NotifySocketClosed(socketId);
    }

    if (!urcFound)
    {
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    return Utils_ConvertAtResponseRetcodeToCellularRetcode(retcode);
}

Retcode_T At_HandleUrc_UUHTTPCR(void)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;
    uint32_t response[3];
    uint32_t argCnt;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)CMD_UBLOX_ATUUHTTPCR, (uint32_t)strlen(CMD_UBLOX_ATUUHTTPCR)); //LCOV_EXCL_BR_LINE
    argCnt = 0;

    while (retcode == RETCODE_OK && argCnt < 3)
    {

        if (RETCODE_OK == retcode)
        {
            retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
        }

        /* read cmd args 1,2,3 */
        if (RETCODE_OK == retcode)
        {
            int32_t tmp = 0;
            retcode = Utils_Strtol(arg, argLen, &tmp);
            response[argCnt++] = (uint32_t)tmp;
            AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
        }
    }
    if (retcode == RETCODE_OK)
    {
        /* report the result */
        HttpService_NotifyResult((AT_UHTTP_ProfileId_T)response[0], (AT_UHTTPC_Command_T)response[1], (AT_UHTTPC_Result_T)response[2]); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

Retcode_T At_Set_UTEST(uint32_t param)
{
    Retcode_T retcode = RETCODE_OK;

    int32_t len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUTEST_FMT,
                           (int)param);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode);
    }

    return retcode;
}

/*###################### LOCAL FUNCTIONS IMPLEMENTATION #############################################################*/

static Retcode_T ExtractUmnoprofMno(const uint8_t *data, uint32_t length, AT_UMNOPROF_Mno_T *value)
{
    int32_t tmp = 0;
    Retcode_T retcode = Utils_StrtolBounds(data, length, &tmp, AT_UMNOPROF_MNO_DEFAULT, AT_UMNOPROF_MNO_CT);

    /* We assume strtol did bounds checking */
    *value = (AT_UMNOPROF_Mno_T)tmp;

    return retcode;
}
/**
 * @brief Converts a number to a single character representing a hexadecimal
 * number.
 * @param quintetIndex
 * @param byte
 * @return
 */
static uint8_t ToHexChar(uint8_t quintetIndex, uint8_t byte)
{
    const uint8_t *hexChars = (const uint8_t *)"0123456789ABCDEF";
    uint8_t mask = (0 == quintetIndex) ? 0x0F : 0xF0;
    uint8_t shift = (0 == quintetIndex) ? 0 : 4;
    uint8_t quintet = (byte & mask) >> shift;

    return hexChars[quintet];
}

/**
 * @brief Converts a single character representing a hexadecimal number to the
 * number. If the character is not a valid hex char, 0xFF is returned.
 */
static uint8_t FromHexChar(uint8_t HexChar)
{
    uint8_t result;

    if ('0' <= HexChar && HexChar <= '9')
    {
        result = HexChar - '0';
    }
    else if ('A' <= HexChar && HexChar <= 'F')
    {
        result = 10 + (HexChar - 'A');
    }
    else if ('a' <= HexChar && HexChar <= 'f')
    {
        result = 10 + (HexChar - 'a');
    }
    else
    {
        result = UINT8_MAX;
    }

    return result;
}

static Retcode_T HexToBin(const uint8_t *hex, uint8_t *bin, uint32_t binLength)
{
    for (uint32_t i = 0; i < binLength; i++)
    {
        uint8_t high = FromHexChar(hex[2 * i]);
        uint8_t low = FromHexChar(hex[2 * i + 1]);

        if (0xFF == high || 0xFF == low)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        bin[i] = (high << 4) | low;
    }

    return RETCODE_OK;
}

/**
 * @brief Encode the given input data as ASCII HEX string according to u-blox
 * specification.
 *
 * @note To properly fit the ASCII HEX string outLength has to be at least twice the
 * size of inLength.
 *
 * @param[in] binData
 * Input data to encode.
 *
 * @param[in] binLength
 * Length of input data.
 *
 * @param[out] hexData
 * Output buffer to store the encoded bytes.
 *
 * @param[in] hexLength
 * Length of the output buffer.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
static Retcode_T EncodePayloadAsHex(const uint8_t *binData, uint32_t binLength,
                                    char *hexData, uint32_t hexLength)
{
    if (hexLength < binLength * 2)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }
    for (uint32_t i = 0; i < binLength; i++)
    {
        hexData[2 * i] = ToHexChar(1, binData[i]);
        hexData[2 * i + 1] = ToHexChar(0, binData[i]);
    }
    return RETCODE_OK;
}
/**
 * @brief Inspect the given buffer for illegal AT characters.
 *
 * @note This is used to verify the contents of the payload when sending via
 * #AT_UBLOX_PAYLOADENCODING_BASE encoding.
 *
 * @param[in] data
 * Data to verify.
 *
 * @param[in] length
 * Length of data.
 *
 * @return A #Retcode_T indicating the result of the requested action. In case
 * an illegal character is found, a #RETCODE_INVALID_PARAM will be returned.
 */
static Retcode_T CheckForIllegalCharacters(const uint8_t *data, uint32_t length)
{
    for (uint32_t i = 0; i < length; ++i)
    {
        if (0x7F <= data[i] || 0x20 > data[i] || '"' == data[i])
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
    }
    return RETCODE_OK;
}

static Retcode_T PrepareSendingWithBaseEncoding(char *sendBuffer,
                                                uint32_t sendBufferLength, const AT_USOWR_Param_T *param, uint32_t *length)
{
    Retcode_T retcode = CheckForIllegalCharacters(param->Data, param->Length);
    int32_t len = 0;

    if (RETCODE_OK == retcode)
    {
        len = snprintf(sendBuffer, sendBufferLength, CMD_UBLOX_SET_ATUSOWR_FMTBASE,
                       (int)param->Socket,
                       (int)param->Length,
                       (int)param->Length,
                       param->Data);
        if ((size_t)len >= sendBufferLength || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }

    if (RETCODE_OK == retcode)
    {
        *length = (uint32_t)len;
    }

    return retcode;
}

static Retcode_T PrepareSendToWithBaseEncoding(char *sendBuffer,
                                               uint32_t sendBufferLength, const AT_USOST_Param_T *param, uint32_t *length)
{
    Retcode_T retcode = CheckForIllegalCharacters(param->Data, param->Length);
    int32_t len = 0;

    if (RETCODE_OK == retcode)
    {
        switch (param->RemoteIp.Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            len = snprintf(sendBuffer, sendBufferLength, CMD_UBLOX_SET_ATUSOST_FMTIPV4BASE,
                           (int)param->Socket,
                           (int)param->RemoteIp.Address.IPv4[3],
                           (int)param->RemoteIp.Address.IPv4[2],
                           (int)param->RemoteIp.Address.IPv4[1],
                           (int)param->RemoteIp.Address.IPv4[0],
                           (int)param->RemotePort,
                           (int)param->Length,
                           (int)param->Length,
                           param->Data);
            break;
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            len = snprintf(sendBuffer, sendBufferLength, CMD_UBLOX_SET_ATUSOST_FMTIPV6BASE,
                           (int)param->Socket,
                           (int)param->RemoteIp.Address.IPv6[7],
                           (int)param->RemoteIp.Address.IPv6[6],
                           (int)param->RemoteIp.Address.IPv6[5],
                           (int)param->RemoteIp.Address.IPv6[4],
                           (int)param->RemoteIp.Address.IPv6[3],
                           (int)param->RemoteIp.Address.IPv6[2],
                           (int)param->RemoteIp.Address.IPv6[1],
                           (int)param->RemoteIp.Address.IPv6[0],
                           (int)param->RemotePort,
                           (int)param->Length,
                           (int)param->Length,
                           param->Data);
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            break;
        }
        if ((RETCODE_OK == retcode) && ((size_t)len >= sendBufferLength || len < 0))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }

    if (RETCODE_OK == retcode)
    {
        *length = (uint32_t)len;
    }

    return retcode;
}

static Retcode_T PrepareSendingWithHexEncoding(char *sendBuffer,
                                               uint32_t sendBufferLength, const AT_USOWR_Param_T *param, uint32_t *length)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;
    int payloadPos = -1;

    len = snprintf(sendBuffer, sendBufferLength, CMD_UBLOX_SET_ATUSOWR_FMTHEX,
                   (int)param->Socket,
                   (int)param->Length,
                   &payloadPos,
                   0,
                   "");
    if ((size_t)len >= sendBufferLength || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }
    else if (0 >= payloadPos)
    {
        /* It appears our %n parameter was not written. */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }

    if (RETCODE_OK == retcode)
    {
        /* Encode the bin data as hex and insert them into the AT send
         * buffer. We make use of the %n to identify where the payload
         * should start. */
        retcode = EncodePayloadAsHex(param->Data, param->Length,
                                     sendBuffer + payloadPos,
                                     sendBufferLength - payloadPos - strlen(CMD_UBLOX_ATUSOWR_FOOTER));
    }

    if (RETCODE_OK == retcode)
    {
        /* We still need to insert the end of the AT command as that was
         * overwritten by the HEX insert. */
        memcpy(sendBuffer + payloadPos + param->Length * 2, CMD_UBLOX_ATUSOWR_FOOTER, strlen(CMD_UBLOX_ATUSOWR_FOOTER));
        *length = payloadPos + param->Length * 2 + strlen(CMD_UBLOX_ATUSOWR_FOOTER);
    }

    return retcode;
}

static Retcode_T PrepareSendToWithHexEncoding(char *sendBuffer,
                                              uint32_t sendBufferLength, const AT_USOST_Param_T *param, uint32_t *length)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;
    int payloadPos = -1;

    switch (param->RemoteIp.Type)
    {
    case AT_UBLOX_ADDRESSTYPE_IPV4:
        len = snprintf(sendBuffer, sendBufferLength, CMD_UBLOX_SET_ATUSOST_FMTIPV4HEX,
                       (int)param->Socket,
                       (int)param->RemoteIp.Address.IPv4[3],
                       (int)param->RemoteIp.Address.IPv4[2],
                       (int)param->RemoteIp.Address.IPv4[1],
                       (int)param->RemoteIp.Address.IPv4[0],
                       (int)param->RemotePort,
                       (int)param->Length,
                       &payloadPos,
                       (int)0,
                       "");
        break;
    case AT_UBLOX_ADDRESSTYPE_IPV6:
        len = snprintf(sendBuffer, sendBufferLength, CMD_UBLOX_SET_ATUSOST_FMTIPV6HEX,
                       (int)param->Socket,
                       (int)param->RemoteIp.Address.IPv6[7],
                       (int)param->RemoteIp.Address.IPv6[6],
                       (int)param->RemoteIp.Address.IPv6[5],
                       (int)param->RemoteIp.Address.IPv6[4],
                       (int)param->RemoteIp.Address.IPv6[3],
                       (int)param->RemoteIp.Address.IPv6[2],
                       (int)param->RemoteIp.Address.IPv6[1],
                       (int)param->RemoteIp.Address.IPv6[0],
                       (int)param->RemotePort,
                       (int)param->Length,
                       &payloadPos,
                       0,
                       "");
        break;
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }
    if (RETCODE_OK == retcode)
    {
        if ((size_t)len > sendBufferLength || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
        else if (0 >= payloadPos)
        {
            /* It appears our %n parameter was not written. */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
        }
    }

    if (RETCODE_OK == retcode)
    {
        /* Encode the bin data as hex and insert them into the AT send
         * buffer. We make use of the %n to identify where the payload
         * should start. */
        retcode = EncodePayloadAsHex(param->Data, param->Length,
                                     sendBuffer + payloadPos,
                                     sendBufferLength - payloadPos - strlen(CMD_UBLOX_ATUSOWR_FOOTER));
    }

    if (RETCODE_OK == retcode)
    {
        /* We still need to insert the end of the AT command as that was
         * overwritten by the HEX insert. */
        memcpy(sendBuffer + payloadPos + param->Length * 2, CMD_UBLOX_ATUSOWR_FOOTER, strlen(CMD_UBLOX_ATUSOWR_FOOTER));
        *length = payloadPos + param->Length * 2 + strlen(CMD_UBLOX_ATUSOWR_FOOTER);
    }

    return retcode;
}

static Retcode_T ParseIPv6RightToLeft(const uint8_t *addressBuff, uint32_t addressBuffLen,
                                      AT_UBlox_Address_T *parsedAddress, uint32_t alreadyParsedGroups)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t currentGroup = 0;
    uint8_t positionInCurrentGroup = 0;
    uint8_t groupsParsed = 0;
    int32_t prevColon = 0;

    for (int32_t i = addressBuffLen - 1; i >= 0; i--)
    {
        char c = (char)toupper(addressBuff[i]);
        uint8_t numVal = FromHexChar(addressBuff[i]);
        if (':' == c)
        {
            if (i != 0)
            {
                if (prevColon == i + 1 || AT_UBLOX_IPV6_GROUP_COUNT <= alreadyParsedGroups)
                {
                    /* We found another skip segment? Or found one segment too
                     * many? IPv6 is malformed! */
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                    break;
                }
            }
            prevColon = i;
            parsedAddress->Address.IPv6[groupsParsed] = currentGroup;
            groupsParsed++;
            alreadyParsedGroups++;

            positionInCurrentGroup = 0;
            currentGroup = 0;
        }
        else if (UINT8_MAX != numVal)
        {
            /* We found a hex number, add it to our current group. */
            uint32_t pow = 1;
            for (uint32_t j = 0; j < positionInCurrentGroup; ++j)
            {
                pow *= 16;
            }
            positionInCurrentGroup++;
            currentGroup += numVal * pow;

            if (positionInCurrentGroup > 4 || currentGroup > UINT16_MAX)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        else if ('"' == c)
        {
            if (i == 0 || i == (int32_t)addressBuffLen - 1)
            {
                /* Ignore '"' at the start and end of the string. */
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        else
        {
            /* Unrecognizable char in byte buffer! */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            break;
        }

        if (i == 0 && ':' != c)
        {
            parsedAddress->Address.IPv6[groupsParsed] = currentGroup;
            groupsParsed++;
        }
    }

    return retcode;
}

static Retcode_T ParseIPv6LeftToRight(const uint8_t *addressBuff, uint32_t addressBuffLen,
                                      AT_UBlox_Address_T *parsedAddress)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t currentGroup = 0;
    uint8_t positionInCurrentGroup = 0;
    uint8_t groupsParsed = 0;
    uint32_t lastColon = 0;

    for (uint32_t i = 0; i < addressBuffLen; i++)
    {
        char c = (char)toupper(addressBuff[i]);
        uint8_t numVal = FromHexChar(addressBuff[i]);
        if (':' == c)
        {
            groupsParsed++;
            if (lastColon == i - 1)
            {
                /* Found a zero segment skip, switch to parsing from right
                 * to left now */
                return ParseIPv6RightToLeft(addressBuff + i, addressBuffLen - i, parsedAddress, groupsParsed);
            }
            else if (AT_UBLOX_IPV6_GROUP_COUNT <= groupsParsed)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            else
            {
                lastColon = i;
                parsedAddress->Address.IPv6[AT_UBLOX_IPV6_GROUP_COUNT - groupsParsed] = currentGroup;

                positionInCurrentGroup = 0;
                currentGroup = 0;
            }
        }
        else if (UINT8_MAX != numVal)
        {
            /* We found a hex number, add it to our current group. */
            currentGroup *= 16;
            currentGroup += numVal;

            positionInCurrentGroup++;

            if (positionInCurrentGroup > 4 || currentGroup > UINT16_MAX)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        else if ('"' == c)
        {
            if (i == 0 || i == addressBuffLen - 1)
            {
                /* Ignore '"' at the start and end of the string. */
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        else
        {
            /* Unrecognizable char in byte buffer! */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            break;
        }

        if (RETCODE_OK == retcode && i == addressBuffLen - 1)
        {
            groupsParsed++;
            parsedAddress->Address.IPv6[AT_UBLOX_IPV6_GROUP_COUNT - groupsParsed] = currentGroup;
        }
    }

    return retcode;
}

static Retcode_T ParseIPv4(const uint8_t *addressBuff, uint32_t addressBuffLen,
                           AT_UBlox_Address_T *parsedAddress)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t currentGroup = 0;
    uint8_t positionInCurrentGroup = 0;
    uint8_t groupsParsed = 0;

    for (uint32_t i = 0; i < addressBuffLen; i++)
    {
        char c = (char)toupper(addressBuff[i]);
        if ('.' == c)
        {
            groupsParsed++;
            if (AT_UBLOX_IPV4_GROUP_COUNT <= groupsParsed)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            else
            {
                parsedAddress->Address.IPv4[AT_UBLOX_IPV4_GROUP_COUNT - groupsParsed] = currentGroup;

                positionInCurrentGroup = 0;
                currentGroup = 0;
            }
        }
        else if (isdigit((int)c))
        {
            uint8_t numVal = addressBuff[i] - '0';
            /* We found a hex number, add it to our current group. */
            currentGroup *= 10;
            currentGroup += numVal;

            positionInCurrentGroup++;

            if (positionInCurrentGroup > 3 || currentGroup > UINT8_MAX)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        else if ('"' == c)
        {
            if (i == 0 || i == addressBuffLen - 1)
            {
                /* Ignore '"' at the start and end of the string. */
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        else
        {
            /* Unrecognizable char in byte buffer! */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            break;
        }

        if (RETCODE_OK == retcode && i == addressBuffLen - 1)
        {
            groupsParsed++;
            parsedAddress->Address.IPv4[AT_UBLOX_IPV4_GROUP_COUNT - groupsParsed] = currentGroup;
        }
    }

    return retcode;
}

static Retcode_T ExtractUbloxAddress(const uint8_t *addressBuff, uint32_t addressBuffLen,
                                     AT_UBlox_Address_T *parsedAddress)
{
    if (NULL == addressBuff || NULL == parsedAddress || 0 == addressBuffLen || addressBuffLen > AT_UBLOX_MAX_IP_STR_LENGTH)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = RETCODE_OK;

    /* First figure out if this is a IPv4 or IPv6 address. */
    parsedAddress->Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    for (uint32_t i = 0; i < addressBuffLen; i++)
    {
        if ('.' == addressBuff[i])
        {
            parsedAddress->Type = AT_UBLOX_ADDRESSTYPE_IPV4;
            break;
        }
        else if (':' == addressBuff[i])
        {
            parsedAddress->Type = AT_UBLOX_ADDRESSTYPE_IPV6;
            break;
        }
    }

    switch (parsedAddress->Type)
    {
    case AT_UBLOX_ADDRESSTYPE_IPV4:
        memset(parsedAddress->Address.IPv4, 0, sizeof(parsedAddress->Address.IPv4));
        retcode = ParseIPv4(addressBuff, addressBuffLen, parsedAddress);
        break;
    case AT_UBLOX_ADDRESSTYPE_IPV6:
        memset(parsedAddress->Address.IPv6, 0, sizeof(parsedAddress->Address.IPv6));
        retcode = ParseIPv6LeftToRight(addressBuff, addressBuffLen, parsedAddress);
        break;
    case AT_UBLOX_ADDRESSTYPE_INVALID:
    default:
        memset(parsedAddress->Address.IPv6, 0, sizeof(parsedAddress->Address.IPv6));
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        break;
    }

    return retcode;
}

static Retcode_T HandleDomainToIpUDNSRN(const AT_UDNSRN_Param_T *param, AT_UDNSRN_Resp_T *resp)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;

    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUDNSRN_FMT1,
                   (int)param->ResolutionType,
                   param->DomainIpString.Domain);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_LONG_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_LONG_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATUDNSRN, strlen(CMD_UBLOX_ATUDNSRN)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait for <resolved_ip_address> */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_LONG_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = ExtractUbloxAddress(arg, argLen, &resp->DomainIpString.Ip);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_LONG_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

static Retcode_T HandleIpToDomainUDNSRN(const AT_UDNSRN_Param_T *param, AT_UDNSRN_Resp_T *resp)
{
    Retcode_T retcode = RETCODE_OK;
    int32_t len = 0;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;

    switch (param->DomainIpString.Ip->Type)
    {
    case AT_UBLOX_ADDRESSTYPE_IPV4:
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUDNSRN_FMT2,
                       (int)param->ResolutionType,
                       param->DomainIpString.Ip->Address.IPv4[3],
                       param->DomainIpString.Ip->Address.IPv4[2],
                       param->DomainIpString.Ip->Address.IPv4[1],
                       param->DomainIpString.Ip->Address.IPv4[0]);
        break;
    case AT_UBLOX_ADDRESSTYPE_IPV6:
        len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUDNSRN_FMT3,
                       (int)param->ResolutionType,
                       param->DomainIpString.Ip->Address.IPv6[7],
                       param->DomainIpString.Ip->Address.IPv6[6],
                       param->DomainIpString.Ip->Address.IPv6[5],
                       param->DomainIpString.Ip->Address.IPv6[4],
                       param->DomainIpString.Ip->Address.IPv6[3],
                       param->DomainIpString.Ip->Address.IPv6[2],
                       param->DomainIpString.Ip->Address.IPv6[1],
                       param->DomainIpString.Ip->Address.IPv6[0]);
        break;
    case AT_UBLOX_ADDRESSTYPE_INVALID:
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }

    if (RETCODE_OK == retcode)
    {
        if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_LONG_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_LONG_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATUDNSRN, strlen(CMD_UBLOX_ATUDNSRN)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait for <resolved_domain_name> */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_LONG_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        if (UBLOX_MAX_DNS_DOMAIN_NAME_LENGTH >= argLen && 2 < argLen)
        {
            memcpy(resp->DomainIpString.Domain, arg + 1, argLen - 2);
            resp->DomainIpString.Domain[argLen - 2] = '\0'; /* the "" are not part of the domain. */
        }
        else
        {
            /* Domain name too long or malformed answer! */
            resp->DomainIpString.Domain[0] = '\0';
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_LONG_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

static Retcode_T HandleHexModeUSORD(const AT_USORD_Param_T *param, AT_USORD_Resp_T *resp)
{
    Retcode_T retcode = RETCODE_OK;
    Retcode_T optRetcode = RETCODE_OK;
    int32_t len = 0;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;
    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUSORD_FMTHEX,
                   (int)param->Socket, (int)param->Length);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SHORT_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATUSORD, (uint32_t)strlen(CMD_UBLOX_ATUSORD)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait for <socket> */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, INT32_MAX);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

        resp->Socket = (uint32_t)tmp;
        assert(resp->Socket == param->Socket);
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait for <length> or <data> ("") if no data is available */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        optRetcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, INT32_MAX);
        if (RETCODE_OK == optRetcode)
        {
            resp->Length = (uint32_t)tmp;
            assert(0 == param->Length || resp->Length <= param->Length);
        }
        else
        {
            /* Probably not a number, check if it's empty data (an oddity of the
             * u-blox AT interface).
             *
             * If AT+USORD=0,10 is called but the modem has no bytes stored,
             * it will answer with +USORD:0,"". Note that in-place of a <length>
             * argument this time a pair of "" is returned ... annoying. */
            if (strlen(AT_UBLOX_EMPTY_DATA) == argLen && 0 == memcmp(arg, AT_UBLOX_EMPTY_DATA, argLen) && 0 < param->Length)
            {
                resp->Length = 0;
            }
            else
            {
                /* OK, this seems to be an actual error. */
                retcode = optRetcode;
            }
        }

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode && 0U < param->Length && 0U < resp->Length)
    {
        /* Wait for <data> */
        optRetcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE

        if (RETCODE_OK == optRetcode)
        {
            /* Skip '"' in arg (arg+1) */
            retcode = HexToBin(arg + 1, resp->Data, resp->Length);

            AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

static Retcode_T HandleHexModeUSORF(const AT_USORF_Param_T *param, AT_USORF_Resp_T *resp)
{
    Retcode_T retcode = RETCODE_OK;
    Retcode_T optRetcode = RETCODE_OK;
    int32_t len = 0;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;
    len = snprintf(Engine_AtSendBuffer, sizeof(Engine_AtSendBuffer), CMD_UBLOX_SET_ATUSORF_FMTHEX,
                   (int)param->Socket, (int)param->Length);
    if ((size_t)len > sizeof(Engine_AtSendBuffer) || len < 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)Engine_AtSendBuffer, (uint32_t)len, CMD_UBLOX_SHORT_TIMEOUT); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CMD_UBLOX_SHORT_TIMEOUT, (const uint8_t *)CMD_UBLOX_ATUSORF, (uint32_t)strlen(CMD_UBLOX_ATUSORF)); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait for <socket> */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, INT32_MAX);

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

        resp->Socket = (uint32_t)tmp;
        assert(resp->Socket == param->Socket);
    }

    if (RETCODE_OK == retcode && 0U < param->Length)
    {
        /* Wait for <remote_ip_addr> or <data> ("") if no data is available */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE

        if (RETCODE_OK == retcode)
        {
            optRetcode = ExtractUbloxAddress(arg, argLen, &resp->RemoteIp);
            if (RETCODE_OK != optRetcode)
            {
                /* Probably not an IP, check if it's empty data (an oddity of
                 * the u-blox AT interface).
                 *
                 * If AT+USORF=0,10 is called but the modem has no bytes stored,
                 * it will answer with +USORF:0,"". Note that in-place of a
                 * <remote_ip_addr> argument this time a pair of "" is returned
                 * ... annoying. */
                if (strlen(AT_UBLOX_EMPTY_DATA) == argLen && 0 == memcmp(arg, AT_UBLOX_EMPTY_DATA, argLen) && 0 < param->Length)
                {
                    resp->Length = 0;
                    resp->RemotePort = 0;
                    /* Should have been set by extractor function. */
                    assert(AT_UBLOX_ADDRESSTYPE_INVALID == resp->RemoteIp.Type);
                }
                else
                {
                    /* OK, this seems to be an actual error. Perhaps a malformed
                     * IP-address? */
                    retcode = optRetcode;
                }
            }

            AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
        }

        if (RETCODE_OK == retcode && AT_UBLOX_ADDRESSTYPE_INVALID != resp->RemoteIp.Type)
        {
            /* Wait for <remote_port> */
            retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE

            if (RETCODE_OK == retcode)
            {
                int32_t tmp = 0;
                retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, UINT16_MAX);
                resp->RemotePort = (uint16_t)tmp;

                AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
            }
        }
    }

    if (RETCODE_OK == retcode && (0 == param->Length || AT_UBLOX_ADDRESSTYPE_INVALID != resp->RemoteIp.Type))
    {
        /* Wait for <length> */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE

        if (RETCODE_OK == retcode)
        {
            int32_t tmp = 0;
            retcode = Utils_StrtolBounds(arg, argLen, &tmp, 0, INT32_MAX);

            AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE

            resp->Length = (uint32_t)tmp;
            assert(0 == param->Length || resp->Length <= param->Length);
        }

        if (RETCODE_OK == retcode && 0U < param->Length)
        {
            /* Wait for <data> */
            retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE

            if (RETCODE_OK == retcode)
            {
                /* Skip '"' in arg (arg+1) */
                retcode = HexToBin(arg + 1, resp->Data, resp->Length);

                AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
            }
        }
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Utils_WaitForAndHandleResponseCode(CMD_UBLOX_SHORT_TIMEOUT, retcode); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

static Retcode_T HandleUrcUUSORX(const char *cmd)
{
    Retcode_T retcode = RETCODE_OK;
    bool urcFound = false;
    uint8_t *arg = NULL;
    uint32_t argLen = 0;
    uint32_t socketId = 0;
    uint32_t length = 0;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)cmd, (uint32_t)strlen(cmd)); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        urcFound = true;
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_Strtol(arg, argLen, &tmp);
        socketId = (uint32_t)tmp;

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CMD_UBLOX_SHORT_TIMEOUT, &arg, &argLen); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        int32_t tmp = 0;
        retcode = Utils_Strtol(arg, argLen, &tmp);
        length = (uint32_t)tmp;

        AtResponseQueue_MarkBufferAsUnused(); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("%s socket:%d length:%d", cmd, (int)socketId, (int)length); //LCOV_EXCL_BR_LINE
        SocketService_NotifySocketDataReceived(socketId, length);             //LCOV_EXCL_BR_LINE
    }

    if (!urcFound)
    {
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    return Utils_ConvertAtResponseRetcodeToCellularRetcode(retcode);
}