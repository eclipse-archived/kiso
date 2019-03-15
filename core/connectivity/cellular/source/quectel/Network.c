/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @file
 *
 * @brief Provides functionality for the modem interaction with the network
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_NETWORK

#include "CellularModule.h"
#include "CellularBsp.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <inttypes.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "CellularCommon.h"
#include "CellularAtResponseParser.h"
#include "CellularResponseQueue.h"
#include "CellularEngine.h"
#include "Control.h"
#include "CellularUtils.h"
#include "CellularEngine.h"
#include "Socket.h"
#include "Command.h"
#include "BCDS_Logging.h"
#include "Network.h"

#define CELLULAR_NETWORK_TIMEOUT_ENQUEUE           (UINT32_C(1000))
#define CELLULAR_NETWORK_RESPONSE_TIMEOUT          (UINT32_C(20000))
#define CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT    (UINT32_C(10000))
#define CELLULAR_NETWORK_URC_TIMEOUT               (UINT32_C(120))
#define CELLULAR_NETWORK_PREF_RAT_DEFAULT          CELLULAR_NW_PARAM_NW_SCAN_SEQ_WCDMA_GSM
#define CELLULAR_NETWORK_SEL_RAT_DEFAULT           CELLULAR_NW_PARAM_NW_SCAN_MODE_AUTO

typedef struct
{
    uint32_t ContextId;
    uint32_t ContextState;
    uint32_t ContextType;
    uint32_t IpAddress;
} CellularNetwork_AssignedParameterQuery_T;

static const char *CELLULAR_NW_PARAM_STR[CELLULAR_NW_PARAM_TYPE_COUNT] =
        {
                "gprsattach",
                "nwscanmode",
                "nwscanseq",
                "nwsel",
                "roamservice",
                "servicedomain",
                "band"
        };

//
//----------------------------------------------------------------------
//-- AT+CREG?
//----------------------------------------------------------------------
//

/**
 * @brief Converts AT+CREG? command arg n from at command responce parser event
 * @return RETCODE_OK only if success
 * @see  Ref 1, p 89, 7.10 Network registration status +CREG
 */
static Retcode_T ConvertToNRegStatusN(uint8_t* data, uint32_t length, CellularNetwork_RegistrationInfo_T* value)
{
    if (NULL == data || 1 != length || NULL == value) //-- number range expected: 1..2 and never 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    int32_t number = 0;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, &number,
            (int32_t) CELLULAR_NETWORK_REG_INFO_DEFAULT, //-- 0
            (int32_t) CELLULAR_NETWORK_REG_INFO_URC_LOC); //-- 2
    if (RETCODE_OK == ret)
    {
        *value = (CellularNetwork_RegistrationInfo_T) number;
        return RETCODE_OK;
    }
    *value = CELLULAR_NETWORK_REG_INFO_INVALID;
    LOG_ERROR("CellularNetwork ConvertToNRegStatusN value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

/**
 * @brief Converts AT+CREG? command arg stat from at command responce parser event
 * @return RETCODE_OK only if success
 * @see  Ref 1, p 89, 7.10 Network registration status +CREG
 */
static Retcode_T ConvertToNRegStatusStat(uint8_t* data, uint32_t length, CellularNetwork_RegistrationStatusStat_T* value)
{
    if (NULL == data || 2 < length || NULL == value) //-- number range expected: 1..10 without 8 and never 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    int32_t number = 0;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, &number,
            (int32_t) CELLULAR_NETWORK_REG_STAT_NOT, //-- 0
            (int32_t) CELLULAR_NETWORK_REG_STAT_CSFB_NOT_PREF_ROAMING);
    if (RETCODE_OK == ret)
    {
        if (8 != number) //-- enum value 8 does not exist
        {
            *value = (CellularNetwork_RegistrationStatusStat_T) number;
            return RETCODE_OK;
        }
    }

    *value = CELLULAR_NETWORK_REG_STAT_INVALID;
    LOG_ERROR("CellularNetwork ConvertToNRegStatusStat value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

static Retcode_T GetIdFromHex(uint8_t* data, uint32_t length, uint32_t* value, uint32_t ErrorValue)
{
    if (NULL == data || 0 == length || NULL == value)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    const char Delimiter[] = " \"";
    char Buffer[length + 1];
    char* ptr = NULL;
    strncpy(Buffer, (char*) data, length);
    Buffer[length] = 0;
    ptr = strtok(Buffer, Delimiter);
    if (NULL != ptr)
    {
        *value = (uint32_t) strtoul(ptr, NULL, 16);
        if ((LONG_MAX == *value) || ((0 == *value) && ('0' != ptr[0])))
        {
            *value = ErrorValue;
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }
    }
    else
    {
        *value = ErrorValue;
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    return RETCODE_OK;
}

/**
 * @brief Copies AT+CREG? command arg ci from at command responce parser event
 * @return RETCODE_OK only if success
 * @see  Ref 1, p 89, 7.10 Network registration status +CREG
 */
static Retcode_T GetNRegStatusCi(uint8_t* data, uint32_t length, uint32_t* value)
{
    //-- note: data is not zero terminated
    //--2-4 byte ==> 4-8 hex char
    // <ci> String From 2 to 4 bytes cell ID in hexadecimal format (e.g. "A13F" ). The value FFFFFFFF
    // means that the current <ci> value is invalid.
    // Note: Possible error in doc Ref 1. Ci might only be 2 octets long instead 4 octets as stated in Ref 1
    return GetIdFromHex(data, length, value, 0xFFFFFFFF);
}

/**
 * @brief Copies AT+CREG? command arg lac from at command responce parser event
 * @return RETCODE_OK only if success
 * @see  Ref 1, p 89, 7.10 Network registration status +CREG
 */
static Retcode_T GetNRegStatusLac(uint8_t* data, uint32_t length, uint32_t* value)
{
    // <lac> String Two bytes location area code or tracking area code (if <Rat>=7) in hexadecimal format
    // (( (e.g. "00C3"). The value FFFF means that the current <lac> value is invalid.
    return GetIdFromHex(data, length, value, 0xFFFF);
}

/**
 * @brief Converts AT+CREG? command arg Rat from at command responce parser event
 * @return RETCODE_OK only if success
 * @see  Ref 1, p 89, 7.10 Network registration status +CREG
 */
static Retcode_T ConvertToRadioAccessTechnology(uint8_t* data, uint32_t length, CellularNetwork_Rat_T* value) //-- Ref 1: Rat
{
    if (NULL == data || 0 == length || 3 < length || NULL == value) //-- number range expected: 0..7 and 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    int32_t number;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, &number,
            (int32_t) CELLULAR_NETWORK_RAT_GSM, (int32_t) CELLULAR_NETWORK_RAT_LTE);
    if (RETCODE_OK == ret)
    {
        *value = (CellularNetwork_Rat_T) number;
         return RETCODE_OK;
    }

    LOG_ERROR("CellularNetwork ConvertToRadioAccessTechnology value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

static Retcode_T QueryRegistrationStatus(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Retcode_T ret = RETCODE_OK;
    Retcode_T RetOptional = RETCODE_OK; //-- at cmd resp parser
    CellularNetwork_RegistrationStatus_T* p = (CellularNetwork_RegistrationStatus_T*) parameter;
    memset(p, 0, sizeof(CellularNetwork_RegistrationStatus_T));
    //-- send AT command ---
    //-- Check the network registration status.
    ret = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ATCMD_NETWORK_REGISTRATION);
    if (RETCODE_OK != ret)
    {
        return ret;
    }
    //-- handle response events ---
    //   AT+CREG? +CREG: <n>,<stat>[,<lac>,<ci>[,<Rat>]] OK
    ret = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, (uint8_t *) ATCMD_NETWORK_REGISTRATION_RESP, strlen(ATCMD_NETWORK_REGISTRATION_RESP));
    if (RETCODE_OK != ret)
    {
        return ret;
    }

    uint8_t* CmdArg = NULL;
    uint32_t LengthCmdArg = 0;
    ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
    if (RETCODE_OK != ret)
    {
        return ret;
    }

    ret = ConvertToNRegStatusN(CmdArg, LengthCmdArg, &((*p).Info)); //-- info
    if (RETCODE_OK == ret)
        (*p).IsValidInfo = true;
    AtResponseQueue_MarkBufferAsUnused();
    CmdArg = NULL;
    LengthCmdArg = 0;
    ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
    if (RETCODE_OK == ret)
    {
        ret = ConvertToNRegStatusStat(CmdArg, LengthCmdArg, &((*p).Stat)); //-- stat
        if (RETCODE_OK == ret)
            (*p).IsValidStat = true;
        AtResponseQueue_MarkBufferAsUnused();
        //-- optional parameter ---
        //-- lac
        CmdArg = NULL;
        LengthCmdArg = 0;
        RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
        if (RETCODE_OK == RetOptional)
        {
            ret = GetNRegStatusLac(CmdArg, LengthCmdArg, &((*p).Lac)); //-- lac
            if (RETCODE_OK == ret)
                (*p).IsValidLac = true;
            AtResponseQueue_MarkBufferAsUnused();
            //-- ci
            CmdArg = NULL;
            LengthCmdArg = 0;
            RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
            if (RETCODE_OK == RetOptional)
            {
                ret = GetNRegStatusCi(CmdArg, LengthCmdArg, &((*p).Ci)); //-- ci
                if (RETCODE_OK == ret)
                    (*p).IsValidCi = true;
                AtResponseQueue_MarkBufferAsUnused();
                //-- Rat
                CmdArg = NULL;
                LengthCmdArg = 0;
                RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
                if (RETCODE_OK == RetOptional)
                {
                    ret = ConvertToRadioAccessTechnology(CmdArg, LengthCmdArg, &((*p).Rat)); //-- Rat
                    if (RETCODE_OK == ret)
                        (*p).IsValidRat = true;
                    AtResponseQueue_MarkBufferAsUnused();
                }
                else
                {
                    //-- no optional params available, which is ok and no error
                }
            }
            else
            {
                //-- no optional params available, which is ok and no error
            }
        }
        else
        {
            //-- no optional params available, which is ok and no error
        } //-- end optionalparameter
    }

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ret);
}

Retcode_T CellularNetwork_QueryRegistrationStatus(CellularNetwork_RegistrationStatus_T *status)
{
    if (NULL == status)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, QueryRegistrationStatus, status, sizeof(*status));
}

//
//----------------------------------------------------------------------
//
//-- AT+CREG=[<n>]
//   p 88f, ch 7.10 Network registration status +CREG
//-- CELLULAR_NETWORK_REG_N_DEFAULT: 0 (default value and factory-programmed value): network registration URC disabled
//   CELLULAR_NETWORK_REG_N_URC:     1: network registration URC +CREG: <stat> enabled
//   CELLULAR_NETWORK_REG_N_URC_LOC  2: network registration and location information URC +CREG: <stat>[,<lac>,<ci>[,<Rat>]] enabled
//----------------------------------------------------------------------
//

static Retcode_T ConfigureRegistrationInformation(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    char cmd[CELLULAR_NETWORK_SMALL_BUFFER_SIZE];

    int count = snprintf(cmd, sizeof(cmd), ATCMD_SET_NETWORK_REGISTRATION_FMT, (unsigned int) (*(CellularNetwork_RegistrationInfo_T *) parameter)); /*lint -esym(515 516,snprintf) */
    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, cmd);

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, retcode);
}

Retcode_T CellularNetwork_ConfigureRegistrationInformation(CellularNetwork_RegistrationInfo_T setting)
{
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, ConfigureRegistrationInformation, &setting, sizeof(CellularNetwork_RegistrationInfo_T));
}

/* ---------------------------------------------------------------------
 * MODEM Network URC Handler for CREG
 * URC +CREG: <stat>[,<lac>,<ci>[,<Rat>]]
 * Sample for CELLULAR_NETWORK_REG_N_URC_LOC (AT+CREG=2)
 * ATRQ [1] CREG<\n>
 * ATRQ [2] 5<\n>
 * ATRQ [2] "1C96"<\n>
 * ATRQ [2] "BCF4E8D"<\n>
 * ATRQ [2] 6<\n>
 * dbg printf: BloxNetwork URC CREG stat:5 rat:6 lac:"1C96" ci:"BCF4E8A"
 */
Retcode_T CellularNetworkUrc_CREG(void)
{
    Retcode_T ret = RETCODE_OK;
    Retcode_T RetStat = RETCODE_OK;
    Retcode_T RetLac = RETCODE_OK;
    Retcode_T RetCi = RETCODE_OK;
    Retcode_T RetRat = RETCODE_OK;
    bool UrcFound = false;
    ret = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) ATCMD_NETWORK_REGISTRATION_RESP, strlen(ATCMD_NETWORK_REGISTRATION_RESP));
    if (RETCODE_OK == ret)
    {
        CellularNetwork_RegistrationStatus_T data;
        memset(&data, 0, sizeof(data));
        uint8_t* CmdArg = NULL;
        uint32_t LengthCmdArg = 0;
        UrcFound = true;
        ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_URC_TIMEOUT, &CmdArg, &LengthCmdArg); //-- stat
        if (RETCODE_OK == ret)
        {
            Retcode_T RetOptional = RETCODE_OK;
            RetStat = ConvertToNRegStatusStat(CmdArg, LengthCmdArg, &data.Stat);
            if (RETCODE_OK == RetStat)
            {
                data.IsValidStat = true;
                /* set device status and notify event */
                if (CELLULAR_NETWORK_REG_STAT_HOME == data.Stat
                        || CELLULAR_NETWORK_REG_STAT_ROAMING == data.Stat
                        || CELLULAR_NETWORK_REG_STAT_CSFB_NOT_PREF_HOME == data.Stat
                        || CELLULAR_NETWORK_REG_STAT_CSFB_NOT_PREF_ROAMING == data.Stat)
                {
                    CellularEngine_SetDeviceError(RETCODE_OK);
                    CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_INIT, CELLULAR_EVENT_INITIALIZED, NULL);
                }
            }

            AtResponseQueue_MarkBufferAsUnused();
            CmdArg = NULL;
            LengthCmdArg = 0;
            RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_URC_TIMEOUT, &CmdArg, &LengthCmdArg); //-- lac
            if (RETCODE_OK == RetOptional)
            {
                RetLac = GetNRegStatusLac(CmdArg, LengthCmdArg, &data.Lac);
                if (RETCODE_OK == RetLac)
                    data.IsValidLac = true;
                AtResponseQueue_MarkBufferAsUnused();
                CmdArg = NULL;
                LengthCmdArg = 0;
                RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_URC_TIMEOUT, &CmdArg, &LengthCmdArg); //-- ci
                if (RETCODE_OK == RetOptional)
                {
                    RetCi = GetNRegStatusCi(CmdArg, LengthCmdArg, &data.Ci);
                    if (RETCODE_OK == RetCi)
                        data.IsValidCi = true;
                    AtResponseQueue_MarkBufferAsUnused();
                    CmdArg = NULL;
                    LengthCmdArg = 0;
                    RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_URC_TIMEOUT, &CmdArg, &LengthCmdArg); //-- Rat
                    if (RETCODE_OK == RetOptional)
                    {
                        RetRat = ConvertToRadioAccessTechnology(CmdArg, LengthCmdArg, &data.Rat);
                        if (RETCODE_OK == RetRat)
                            data.IsValidRat = true;
                        AtResponseQueue_MarkBufferAsUnused();
                    }
                }
            }
            //-- RETCODE_OK == ret means all events received succssfully, but we have to check if
            //   all received data is in range even if optional
            if ((RETCODE_OK != RetStat) || (RETCODE_OK != RetLac) || (RETCODE_OK != RetCi) || (RETCODE_OK != RetRat))
            {
                ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            }
        }

        if (data.IsValidLac && data.IsValidCi)
        {
            LOG_DEBUG("Cellular Nw URC CREG stat:%u rat:%u lac:%u ci:%u",
                    (unsigned int) data.Stat, (unsigned int) data.Rat, (unsigned int) data.Lac, (unsigned int) data.Ci);
        }
        else
        {
            LOG_DEBUG("Cellular Network URC CREG stat:%u", (unsigned int) data.Stat);
        }
    }
    if (!UrcFound)
    {
        ret = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    return CellularUtils_ConvertAtResponseRetcodeToCellularRetcode(ret);
}

//
//----------------------------------------------------------------------
//-- AT+COPS=2 (Deregister the module from the network)
//----------------------------------------------------------------------
//

static Retcode_T DeregisterFromNetwork(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ATCMD_OPERATOR_SELECTION_DEREGISTER);

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, retcode);
}

static Retcode_T Cellular_DeregisterFromNetwork(void)
{
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, DeregisterFromNetwork, NULL, 0);
}

//
//----------------------------------------------------------------------
//-- AT+COPS=0 (enable automatic network selection by MT)
//----------------------------------------------------------------------
//

static Retcode_T EnableAutomaticSelection(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ATCMD_OPERATOR_SELECTION_AUTOMATIC);

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, retcode);
}

Retcode_T CellularNetwork_EnableAutomaticSelection(void)
{
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, EnableAutomaticSelection, NULL, 0);
}

//
//----------------------------------------------------------------------
//-- AT+COPS? (query current network operator)
//   e.g. +COPS: 0,0,"Swisscom",2
//----------------------------------------------------------------------

/**
 * @brief AT+COPS? (query current network operator)
 *   e.g. +COPS: 0,0,"Swisscom",2
 */
static Retcode_T ConvertToMode(uint8_t* data, uint32_t length, CellularNetwork_SelectionMode_T* value)
{
    if (1 != length) //-- number range expected: 0..8 without 7 and never 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    int32_t number = 0;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, &number,
            (int32_t) CELLULAR_NETWORK_SEL_MODE_DEFAULT, //-- 0
            (int32_t) CELLULAR_NETWORK_SEL_MODE_NETWORK_TA); //-- 8
    if (RETCODE_OK == ret)
    {
        if ((int32_t) CELLULAR_NETWORK_SEL_MODE_RESERVED != number)
        {
            if (((int32_t) CELLULAR_NETWORK_SEL_MODE_DEFAULT <= number) && ((int32_t) CELLULAR_NETWORK_SEL_MODE_NETWORK_TA >= number))
            {
                *value = (CellularNetwork_SelectionMode_T) number;
                return RETCODE_OK;
            }
        }
    }

    LOG_ERROR("CellularNetwork ConvertToMode value %i out of range", (int ) number);
    *value = CELLULAR_NETWORK_SEL_MODE_INVALID;
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

static Retcode_T ConvertToFormat(uint8_t* data, uint32_t length, CellularNetwork_FormatOperatorName_T* value)
{
    if (1 != length) //-- number range expected: 0..2 and never 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    int32_t number = 0;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, &number,
            (int32_t) CELLULAR_NETWORK_FORMAT_NAME_LONG_A, //-- 0
            (int32_t) CELLULAR_NETWORK_FORMAT_NAME_NUMERIC); //-- 2
    if (RETCODE_OK == ret)
    {
        if (((int32_t) CELLULAR_NETWORK_FORMAT_NAME_LONG_A <= number) && ((int32_t) CELLULAR_NETWORK_FORMAT_NAME_NUMERIC >= number))
        {
            *value = (CellularNetwork_FormatOperatorName_T) number;
            return RETCODE_OK;
        }
    }

    LOG_ERROR("CellularNetwork ConvertToFormat value %i out of range", (int ) number);
    *value = CELLULAR_NETWORK_FORMAT_NAME_INVALID;
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

static Retcode_T GetStringOper(uint8_t* data, uint32_t length, char* Dest, uint32_t MaxLength)
{
    if (length > MaxLength - 1)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    strncpy(Dest, (const char*) data, length);
    Dest[length] = 0;
    return RETCODE_OK; //-- success
}

static Retcode_T QueryCurrentOperator(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Retcode_T ret = RETCODE_OK;
    Retcode_T RetOptional = RETCODE_OK;
    CellularNetwork_CurrentOperator_T TmpOperator; //-- tmp until all succdessfully parsed
    memset(&TmpOperator, 0, sizeof(TmpOperator));
    //-- send AT command ---
    ret = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ATCMD_OPERATOR_SELECTION);
    if (RETCODE_OK != ret)
    {
        return ret;
    }

    //-- handle response events ---
    //   +COPS: <mode>[,<format>,<oper>[,<Rat>]]
    ret = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t *) ATCMD_OPERATOR_SELECTION_RESP, strlen(ATCMD_OPERATOR_SELECTION_RESP));
    if (RETCODE_OK != ret)
    {
        return ret;
    }

    uint8_t* CmdArg = NULL;
    uint32_t LengthCmdArg = 0;
    ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
    if (RETCODE_OK == ret)
    {
        ret = ConvertToMode(CmdArg, LengthCmdArg, &TmpOperator.Mode); //-- mode
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK == ret)
            TmpOperator.IsValidMode = true;
        //-- optional parameter ---
        CmdArg = NULL;
        LengthCmdArg = 0;
        RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
        if (RETCODE_OK == RetOptional)
        {
            ret = ConvertToFormat(CmdArg, LengthCmdArg, &TmpOperator.Format); //-- format
            AtResponseQueue_MarkBufferAsUnused();
            if (RETCODE_OK == ret)
                TmpOperator.IsValidFormat = true;
        }
        if (RETCODE_OK == RetOptional)
        {
            CmdArg = NULL;
            LengthCmdArg = 0;
            RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
            if (RETCODE_OK == RetOptional)
            {
                ret = GetStringOper(CmdArg, LengthCmdArg, TmpOperator.Operator, sizeof(TmpOperator.Operator)); //-- oper
                AtResponseQueue_MarkBufferAsUnused();
                if (RETCODE_OK == ret)
                    TmpOperator.IsValidOperator = true;
            }
        }
        if (RETCODE_OK == RetOptional)
        {
            CmdArg = NULL;
            LengthCmdArg = 0;
            ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
            if (RETCODE_OK == ret)
            {
                ret = ConvertToRadioAccessTechnology(CmdArg, LengthCmdArg, &TmpOperator.Rat); //-- Rat
                AtResponseQueue_MarkBufferAsUnused();
                if (RETCODE_OK == ret)
                    TmpOperator.IsValidRat = true;
            }
        }
    }

    //-- finish ---
    ret = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
    if (RETCODE_OK == ret)
    {
        //-- copy result of caller
        //   Note: each argument has a valid flag inside struct
        memcpy(parameter, &TmpOperator, sizeof(CellularNetwork_CurrentOperator_T));
    }
    return ret;
}

Retcode_T CellularNetwork_QueryCurrentOperator(CellularNetwork_CurrentOperator_T* NetworkOperator)
{
    if (NULL == NetworkOperator)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE,
            QueryCurrentOperator, (void*) NetworkOperator, sizeof(CellularNetwork_CurrentOperator_T));
}

static Retcode_T SetPsdProfile(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    char cmd[CELLULAR_NETWORK_BUFFER_SIZE];
    const Cellular_PsdProfile_T *p = (const Cellular_PsdProfile_T *) parameter;

    int count = snprintf(cmd, sizeof(cmd), ATCMD_SET_CONTEXT_PROFILE_FMT,
            (unsigned int) p->Id,
            (unsigned int) p->Protocol,
            p->Apn ? p->Apn : "",
            p->Username ? p->Username : "",
            p->Password ? p->Password : "",
            (unsigned int) p->Authentication
            );

    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, cmd);

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, retcode);
}

Retcode_T CellularNetwork_SetPsdProfile(Cellular_PsdProfile_T *profile)
{
    if (NULL == profile)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE,
            SetPsdProfile, profile, sizeof(*profile));
}

static Retcode_T GetNetworkAssignedData(CellularNetwork_AssignedParameterQuery_T *req)
{
    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ATCMD_GET_PDP_CONTEXT);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    uint8_t *arg;
    uint32_t len;
    uint32_t value;
    bool found = false;

    /* find requested profile */
    for (uint32_t count = 0; count < CELLULAR_NETWORK_PROFILE_COUNT; ++count)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, (uint8_t *) ATCMD_GET_PDP_CONTEXT_RESP, strlen(ATCMD_GET_PDP_CONTEXT_RESP));
        if (RETCODE_OK != retcode)
        {
            break;
        }
        /* parse contextid */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &len);
        if (RETCODE_OK != retcode)
        {
            break;
        }
        retcode = CellularUtils_StrtolBounds(arg, len, (int32_t *) &value, CELLULAR_NETWORK_PROFILE_ID_LOW, CELLULAR_NETWORK_PROFILE_ID_HIGH);
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK != retcode)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }
        if (value != req->ContextId)
        {
            /* ignore contextstate */
            (void) AtResponseQueue_IgnoreEvent(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT);
            /* ignore contexttype */
            (void) AtResponseQueue_IgnoreEvent(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT);
            /* ignore ipaddress */
            (void) AtResponseQueue_IgnoreEvent(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT);
            continue;
        }

        /* parse contextstate */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &len);
        if (RETCODE_OK != retcode)
        {
            break;
        }
        retcode = CellularUtils_StrtolBounds(arg, len, (int32_t *) &value, 0, 1);
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK != retcode)
        {
            break;
        }
        req->ContextState = value;

        /* parse contexttype */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &len);
        if (RETCODE_OK != retcode)
        {
            break;
        }
        retcode = CellularUtils_StrtolBounds(arg, len, (int32_t *) &value, (int32_t) CELLULAR_PSD_PROTOCOL_IPV4, (int32_t) CELLULAR_PSD_PROTOCOL_IPV6V4);
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK != retcode)
        {
            break;
        }
        req->ContextType = value;

        /* parse ipaddress */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &len);
        if (RETCODE_OK != retcode)
        {
            break;
        }
        retcode = CellularUtils_ParseIpAddress(arg + 1, len - 2, &value);
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK != retcode)
        {
            break;
        }
        req->IpAddress = value;
        found = true;
    }

    retcode = found ? RETCODE_OK : retcode;
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, retcode);
}

static Retcode_T ActivatePdpContext(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    CellularNetwork_AssignedParameterQuery_T *p = (CellularNetwork_AssignedParameterQuery_T *) parameter;

    char cmd[CELLULAR_NETWORK_BUFFER_SIZE];

    int count = snprintf(cmd, sizeof(cmd), p->ContextState ?
    ATCMD_PDP_CONTEXT_ACTIVATE_FMT : ATCMD_PDP_CONTEXT_DEACTIVATE_FMT, (unsigned int) p->ContextId);

    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, cmd);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, retcode);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    if (!p->ContextState)
    {
        CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_DISCONNECTED, CELLULAR_EVENT_DISCONNECTED, NULL);
        CellularSocket_InvalidateAll();
        return retcode;
    }

    /* request active PDP parameters */
    CellularNetwork_AssignedParameterQuery_T req;
    req.ContextId = p->ContextId;
    req.IpAddress = 0;

    retcode = GetNetworkAssignedData(&req);
    /* set device status and notify event */
    if (RETCODE_OK == retcode && CELLULAR_DEVICESTATUS_CONNECTED != CellularEngine_GetDeviceStatus())
    {
        CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_CONNECTED, CELLULAR_EVENT_CONNECTED, (void *) req.IpAddress);
    }

    return retcode;
}

Retcode_T CellularNetwork_ActivatePdpContext(void)
{
    CellularNetwork_AssignedParameterQuery_T req;
    req.ContextId = CELLULAR_NETWORK_PROFILE_ID_DEFAULT;
    req.ContextState = 1;
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, ActivatePdpContext, &req, sizeof(req));
}

Retcode_T CellularNetwork_DeactivatePdpContext(void)
{
    CellularNetwork_AssignedParameterQuery_T req;
    req.ContextId = CELLULAR_NETWORK_PROFILE_ID_DEFAULT;
    req.ContextState = 0;
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, ActivatePdpContext, &req, sizeof(req));
}

static Retcode_T SelectRadioAccessTechnology(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    if (NULL == parameter)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularNetwork_RadioAccessTechnology_T* p = (CellularNetwork_RadioAccessTechnology_T*) parameter;
    char cmd[CELLULAR_NETWORK_SMALL_BUFFER_SIZE];

    int count = snprintf(cmd, sizeof(cmd), ATCMD_SET_NETWORK_PARAMETER_FMT,
            CELLULAR_NW_PARAM_STR[CELLULAR_NW_PARAM_TYPE_NW_SCAN_MODE], (unsigned int) p->Selection);

    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, cmd);

    retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, retcode);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    count = snprintf(cmd, sizeof(cmd), ATCMD_SET_NETWORK_PARAMETER_FMT,
            CELLULAR_NW_PARAM_STR[CELLULAR_NW_PARAM_TYPE_NW_SCAN_SEQ], (unsigned int) p->Preference);

    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, cmd);

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, retcode);
}

Retcode_T CellularNetwork_SelectRadioAccessTechnology(CellularNetwork_RadioAccessTechnology_T *rat)
{
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, SelectRadioAccessTechnology, rat, sizeof(*rat));
}

//----------------------------------------------------------------------
//-- AT+CSQ (Received Signal Strength Indication (RSSI))
//   Ref 1, p 76, ch 7.2 Signal quality +CSQ

/**
 * @brief Converts AT+CSQ command arg rssi from at command responce parser event
 * @return RETCODE_OK only if success
 * @see Ref 1, p 76, ch 7.2 Signal quality +CSQ
 */
static Retcode_T ConvertToNetworkRssi(uint8_t* data, uint32_t length, CellularNetwork_Rssi_T* value)
{
    if (NULL == data || 2 < length) //-- number range expected: 0..31 and 99
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    CellularNetwork_Rssi_T number;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, (int32_t *) &number,
            (int32_t) CELLULAR_NETWORK_RSSI_LESS_113_DBM, //-- 0
            (int32_t) CELLULAR_NETWORK_RSSI_UNKNOWN); //-- 99
    if (RETCODE_OK == ret)
    {
        if (number <= CELLULAR_NETWORK_RSSI_51_DBM || CELLULAR_NETWORK_RSSI_UNKNOWN == number)
        {
            *value = number;
            return RETCODE_OK;
        }
    }

    *value = CELLULAR_NETWORK_RSSI_UNKNOWN;
    LOG_ERROR("CellularNetwork ConvertToNetworkRssi value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

/**
 * @brief Converts AT+CSQ command arg qual from at command responce parser event
 * @return RETCODE_OK only if success
 * @see  Ref 1, p 76, ch 7.2 Signal quality +CSQ
 */
static Retcode_T ConvertToNetworkQuality(uint8_t* data, uint32_t length, CellularNetwork_Quality_T* value)
{
    if (NULL == data || 2 < length) //-- number range expected: 0..7 and 99
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    CellularNetwork_Quality_T number;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, (int32_t *) &number,
            (int32_t) CELLULAR_NETWORK_QUALITY_0, //-- 0
            (int32_t) CELLULAR_NETWORK_QUALITY_UNKOWN); //-- 99
    if (RETCODE_OK == ret)
    {
        if (number <= CELLULAR_NETWORK_QUALITY_7 || CELLULAR_NETWORK_QUALITY_UNKOWN == number)
        {
            *value = number;
            return RETCODE_OK;
        }
    }

    *value = CELLULAR_NETWORK_QUALITY_UNKOWN;
    LOG_ERROR("CellularNetwork ConvertToNetworkQual value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

static Retcode_T QueryCurrentSignalQuality(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    CellularNetwork_SignalQuality_T* p = (CellularNetwork_SignalQuality_T*) parameter;
    p->IsValidRssi = false;
    p->IsValidQuality = false;

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ATCMD_SIGNAL_QUALITY_REPORT);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, (uint8_t *) ATCMD_SIGNAL_QUALITY_REPORT_RESP, strlen(ATCMD_SIGNAL_QUALITY_REPORT_RESP));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    uint8_t* arg;
    uint32_t len;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &len);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = ConvertToNetworkRssi(arg, len, &p->Rssi);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK == retcode)
    {
        p->IsValidRssi = true;
    }

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &len);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = ConvertToNetworkQuality(arg, len, &p->Quality);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK == retcode)
    {
        p->IsValidQuality = true;
    }

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, retcode);
}

Retcode_T CellularNetwork_QueryCurrentSignalQuality(CellularNetwork_SignalQuality_T* NetworkSignalQuality) //-- AT+CSQ
{
    if (NULL == NetworkSignalQuality)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE,
            QueryCurrentSignalQuality, NetworkSignalQuality, sizeof(*NetworkSignalQuality));
}

Retcode_T CellularNetwork_Init(void)
{
    bool SimCardInserted = false;
    Retcode_T retcode = Cellular_IsSimCardInserted(&SimCardInserted);

    LOG_DEBUG("modem network init");
    if (RETCODE_OK != retcode || !SimCardInserted)
    {
        LOG_ERROR("Did not find SIM card: %" PRIu32 ", %d", retcode, SimCardInserted);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_SIM_NOT_READY);
    }

    /* configure network */
    retcode = CellularNetwork_ConfigureRegistrationInformation(CELLULAR_NETWORK_REG_INFO_URC_LOC);
    if (RETCODE_OK != retcode)
    {
        LOG_ERROR("Configure CREG URC failed: %" PRIu32, retcode);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NETWORK_ERROR);
    }

    retcode = Cellular_DeregisterFromNetwork();
    if (RETCODE_OK != retcode)
    {
        LOG_ERROR("Network-deregister failed: %" PRIu32, retcode);
    }

    /* apply RAT configuration default to GSM/UMTS dual mode */
    CellularNetwork_RadioAccessTechnology_T rat;
    rat.Preference = CELLULAR_NETWORK_PREF_RAT_DEFAULT;
    rat.Selection = CELLULAR_NETWORK_SEL_RAT_DEFAULT;

    retcode = CellularNetwork_SelectRadioAccessTechnology(&rat);
    if (RETCODE_OK == retcode)
    {
        LOG_INFO("Device config RAT sel=%d pref=%d", rat.Selection, rat.Preference);
    }
    else
    {
        LOG_ERROR("SelectNetworkRAT failed: %" PRIu32, retcode);
    }

    /* connect to network */
    retcode = CellularNetwork_EnableAutomaticSelection();
    if (RETCODE_OK != retcode)
    {
        LOG_ERROR("EnableAutoNetworkSelection failed: %" PRIu32, retcode);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NETWORK_ERROR);
    }

    return retcode;
}
