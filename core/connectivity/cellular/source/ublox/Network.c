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
#include "Command.h"
#include "BCDS_Logging.h"
#include "Network.h"

#define CELLULAR_NETWORK_TIMEOUT_ENQUEUE           (UINT32_C(1000))
#define CELLULAR_NETWORK_RESPONSE_TIMEOUT_COPS     (UINT32_C(180000))  //-- msec, Ref 1, page 78, ch. 7.4 Operator selection +COPS
#define CELLULAR_NETWORK_RESPONSE_TIMEOUT_UPSDA    (UINT32_C(180000))  //-- msec, Ref 1, page 310, 18.6 Packet switched data action +UPSDA
#define CELLULAR_NETWORK_RESPONSE_TIMEOUT          (UINT32_C(20000))   //-- msec
#define CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT    (UINT32_C(10000))   //-- msec
#define CELLULAR_NETWORK_URC_TIMEOUT               (UINT32_C(120))     //-- msec

#define CELLULAR_DEFAULT_RETRY_COUNT               (UINT32_C(3))
#define CELLULAR_WAIT_FOR_SIM_READINESS_DELAY      (UINT32_C(1000))
#define CELLULAR_NETWORK_PREF_RAT_DEFAULT          CELLULAR_NETWORK_PREF_RAT_UMTS
#define CELLULAR_NETWORK_SEL_RAT_DEFAULT           CELLULAR_NETWORK_SEL_RAT_GSM_UMTS_DUALMODE

#ifndef countof
#   define countof(a) (sizeof(a) / sizeof(*(a)))
#endif

/**
 * @brief Enumeration for PDP Context action
 * @details
 * * 0: reset; it clears the specified profile resetting all the parameters to their factory-programmed values
 * * 1: store; it saves all the parameters in NVM
 * * 2: load: it reads all the parameters from NVM
 * * 3: activate; it activates a PDP context with the specified profile, using the current parameters
 * * 4: deactivate; it deactivates the PDP context associated with the specified profile
 *
 * @see  Ref 2, p 325, 18.6 Packet switched data action +UPSDA
 */
enum CellularNetwork_ActionPdpContext_E
{
    CELLULAR_NETWORK_ACTION_PDP_CONTEXT_RESET = 0,
    CELLULAR_NETWORK_ACTION_PDP_CONTEXT_STORE = 1,
    CELLULAR_NETWORK_ACTION_PDP_CONTEXT_LOAD = 2,
    CELLULAR_NETWORK_ACTION_PDP_CONTEXT_ACTIVATE = 3,
    CELLULAR_NETWORK_ACTION_PDP_CONTEXT_DEACTIVATE = 4
};
typedef enum CellularNetwork_ActionPdpContext_E CellularNetwork_ActionPdpContext_T;

/**
 * @brief Enumeration for PDP Context network-assigned or network-negotiated parameter
 * @details Index representing a network-assigned or network-negotiated parameter:
 *  0. IP address: dynamic IP address assigned during PDP context activation;
 *  1. DNS1: dynamic primary DNS address;
 *  2. DNS2: dynamic secondary DNS address;
 *  3. QoS precedence: network assigned precedence class of the QoS;
 *  4. QoS delay: network assigned delay class of the QoS;
 *  5. QoS reliability: network assigned reliability class of the QoS;
 *  6. QoS peak rate: network assigned peak rate value of the QoS;
 *  7. QoS mean rate: network assigned mean rate value of the QoS
 *  8. PSD profile status: if the profile is active the return value is 1, 0 otherwise
 *  9. 3G QoS delivery order
 *  10. 3G QoS erroneous SDU delivery
 *  11. 3G QoS extended guaranteed downlink bit rate
 *  12. 3G QoS extended maximum downlink bit rate
 *  13. 3G QoS guaranteed downlink bit rate
 *  14. 3G QoS guaranteed uplink bit rate
 *  15. 3G QoS maximum downlink bit rate
 *  16. 3G QoS maximum uplink bit rate
 *  17. 3G QoS maximum SDU size
 *  18. 3G QoS residual bit error rate
 *  19. 3G QoS SDU error ratio
 *  20. 3G QoS signalling indicator
 *  21. 3G QoS source statistics descriptor
 *  22. 3G QoS traffic class
 *  23. 3G QoS traffic priority
 *  24. 3G QoS transfer delay
 *
 * @see  Ref 1, p 312, 18.7 Packet switched network-assigned data +UPSND
 */
enum CellularNetwork_AssignedParam_E
{
    CELLULAR_NETWORK_ASSIGNED_IPADR = 0,
    CELLULAR_NETWORK_ASSIGNED_DNS1 = 1,
    CELLULAR_NETWORK_ASSIGNED_QOS_PROC = 3,
    CELLULAR_NETWORK_ASSIGNED_QOS_DELAY = 4,
    CELLULAR_NETWORK_ASSIGNED_QOS_RELIAB = 5,
    CELLULAR_NETWORK_ASSIGNED_QOS_PEAK = 6,
    CELLULAR_NETWORK_ASSIGNED_QOS_MEAN = 7,
    CELLULAR_NETWORK_ASSIGNED_PSD_PROFILE_STATUS = 8,
    CELLULAR_NETWORK_ASSIGNED_QOS_DELIVERY_ORDER = 9,
    CELLULAR_NETWORK_ASSIGNED_QOS_ERR_SDU_DELIVERY = 10,
    CELLULAR_NETWORK_ASSIGNED_QOS_EXT_GUAR_DL_BITRATE = 11,
    CELLULAR_NETWORK_ASSIGNED_QOS_EXT_MAX_DL_BITRATE = 12,
    CELLULAR_NETWORK_ASSIGNED_QOS_GUAR_DL_BITRATE = 13,
    CELLULAR_NETWORK_ASSIGNED_QOS_GUAR_UL_BITRATE = 14,
    CELLULAR_NETWORK_ASSIGNED_QOS_MAX_DL_BITRATE = 15,
    CELLULAR_NETWORK_ASSIGNED_QOS_MAX_UL_BITRATE = 16,
    CELLULAR_NETWORK_ASSIGNED_QOS_MAX_SDU_SIZE = 17,
    CELLULAR_NETWORK_ASSIGNED_QOS_RESIDUAL_BER = 18,
    CELLULAR_NETWORK_ASSIGNED_QOS_SDU_ERR_RATIO = 19,
    CELLULAR_NETWORK_ASSIGNED_QOS_SIG_IND = 20,
    CELLULAR_NETWORK_ASSIGNED_QOS_SRC_STAT_D = 21,
    CELLULAR_NETWORK_ASSIGNED_QOS_TRAFFIC_CLASS = 22,
    CELLULAR_NETWORK_ASSIGNED_QOS_TRAFFIC_PRIO = 23,
    CELLULAR_NETWORK_ASSIGNED_QOS_TRANSFE_RDELAY = 24
};
typedef enum CellularNetwork_AssignedParam_E CellularNetwork_AssignedParam_T;

struct CellularNetwork_AssignedParameterQuery_S
{
    uint32_t ProfileId;
    CellularNetwork_AssignedParam_T Query;
    union /*lint +fan */
    {
        bool IsPDPcontext;
        uint32_t Ip;
    };
};
typedef struct CellularNetwork_AssignedParameterQuery_S CellularNetwork_AssignedParameterQuery_T;

/** @brief Supported Packet switched data configuration parameters
*/
/*lint -doffsetof +doffsetof(type,member)=((size_t)&((type*)0)->member) */
static const Cellular_PsdProfileItem_T Cellular_PsdProfileItems[] =
        {
                { CELLULAR_PSD_PARAM_PROTOCOL, CELLULAR_PSD_PARAM_TYPE_U32, offsetof(Cellular_PsdProfile_T, Protocol) },
                { CELLULAR_PSD_PARAM_APN, CELLULAR_PSD_PARAM_TYPE_STRING, offsetof(Cellular_PsdProfile_T, Apn) },
                { CELLULAR_PSD_PARAM_USERNAME, CELLULAR_PSD_PARAM_TYPE_STRING, offsetof(Cellular_PsdProfile_T, Username) },
                { CELLULAR_PSD_PARAM_PASSWORD, CELLULAR_PSD_PARAM_TYPE_STRING, offsetof(Cellular_PsdProfile_T, Password) },
                { CELLULAR_PSD_PARAM_AUTH, CELLULAR_PSD_PARAM_TYPE_U32, offsetof(Cellular_PsdProfile_T, Authentication) }
        };

/**
 * @brief Enables HEX mode on the modem for socket data communication.
 * @return RETCODE_OK only if success
 *
 * @see  Ref 2, page 523, section 25.16
 */
static Retcode_T EnableHexMode(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ATCMD_ENABLE_HEX_MODE);
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, retcode);
}

static Retcode_T Cellular_EnableHexMode(void)
{
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, EnableHexMode, NULL, 0);
}

//
//----------------------------------------------------------------------
//-- AT+CREG?
//----------------------------------------------------------------------
//

/**
 * @brief Converts AT+CREG? command arg n from at command responce parser event
 * @return RETCODE_OK only if success
 *
 * @see  Ref 2, p 94, 7.10 Network registration status +CREG
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
 *
 * @see  Ref 2, p 94, 7.10 Network registration status +CREG
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
    //-- ### ERROR
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
            //-- ### ERROR
            *value = ErrorValue;
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }
    }
    else
    {
        //-- ### ERROR
        *value = ErrorValue;
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }
    return RETCODE_OK;
}

/**
 * @brief Copies AT+CREG? command arg ci from at command responce parser event
 * @return RETCODE_OK only if success
 *
 * @see  Ref 2, p 104, 7.10 Network registration status +CREG
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
 *
 * @see  Ref 2, p 104, 7.10 Network registration status +CREG
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
 *
 * @see  Ref 2, p 104, 7.10 Network registration status +CREG
 */
static Retcode_T ConvertToRadioAccessTechnology(uint8_t* data, uint32_t length, CellularNetwork_Rat_T* value) //-- Ref 1: Rat
{
    if (NULL == data || 0 == length || 3 < length || NULL == value) //-- number range expected: 0..7 and 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    int32_t number = 0;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, &number,
            (int32_t) CELLULAR_NETWORK_RAT_GSM, //-- 0
            (int32_t) CELLULAR_NETWORK_RAT_INVALID); //-- 255 (valid modem answer)
    if (RETCODE_OK == ret)
    {
        if ((((int32_t) CELLULAR_NETWORK_RAT_GSM <= number) && ((int32_t) CELLULAR_NETWORK_RAT_LTE >= number)) || ((int32_t) CELLULAR_NETWORK_RAT_INVALID == number))
        {
            *value = (CellularNetwork_Rat_T) number;
            return RETCODE_OK;
        }
    }
    //-- ### ERROR
    LOG_ERROR("CellularNetwork ConvertToRadioAccessTechnology value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

static Retcode_T QueryRegistrationStatus(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    const char AtCmd[] = "AT+CREG?\r\n";
    const char Cmd[] = "CREG";
    Retcode_T ret = RETCODE_OK;
    Retcode_T RetOptional = RETCODE_OK; //-- at cmd resp parser

    CellularNetwork_RegistrationStatus_T* p = (CellularNetwork_RegistrationStatus_T*) parameter;
    memset(p, 0, sizeof(CellularNetwork_RegistrationStatus_T));
    //-- send AT command ---
    //-- Check the network registration status.
    ret = CellularEngine_SendAtCommand((uint8_t *) AtCmd, (uint32_t) strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        //   AT+CREG? +CREG: <n>,<stat>[,<lac>,<ci>[,<Rat>]] OK
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t *) AtCmd, (uint32_t) strlen(AtCmd) - 2);
        if (RETCODE_OK == ret)
        {
            ret = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t *) Cmd, (uint32_t) strlen(Cmd));
            if (RETCODE_OK == ret)
            {
                uint8_t* CmdArg = NULL;
                uint32_t LengthCmdArg = 0;
                ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
                if (RETCODE_OK == ret)
                {
                    ret = ConvertToNRegStatusN(CmdArg, LengthCmdArg, &((*p).Info)); //-- info
                    if (RETCODE_OK == ret)
                        (*p).IsValidInfo = true;
                    AtResponseQueue_MarkBufferAsUnused();
                    CmdArg = NULL;
                    LengthCmdArg = 0;
                    ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT, &CmdArg, &LengthCmdArg);
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
                }
            }
        }
    }

    //-- finish ---
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
}

Retcode_T CellularNetwork_QueryRegistrationStatus(CellularNetwork_RegistrationStatus_T *status)
{
    if (NULL == status)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularNetwork_RegistrationStatus_T param;
    Retcode_T result = CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, QueryRegistrationStatus, &param, sizeof(CellularNetwork_RegistrationStatus_T));
    if (RETCODE_OK == result)
    {
        memcpy(status, &param, sizeof(CellularNetwork_RegistrationStatus_T));
    }
    return result;
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

    CellularNetwork_RegistrationInfo_T* p = (CellularNetwork_RegistrationInfo_T*) parameter;
    char AtCmd[CELLULAR_NETWORK_SMALL_BUFFER_SIZE];
    Retcode_T ret = RETCODE_OK;
    int count = snprintf(AtCmd, CELLULAR_NETWORK_SMALL_BUFFER_SIZE - 3, "AT+CREG=%u\r\n", (unsigned int) (*p)); /*lint -esym(515 516,snprintf) */
    if ((0 > count) || ( CELLULAR_NETWORK_SMALL_BUFFER_SIZE - 3 <= ((uint32_t) count))) //-- dynamic memory allocation is forbidden
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    //-- send AT command ---
    //-- Check the network registration status.
    ret = CellularEngine_SendAtCommand((uint8_t*) (uint8_t*) (AtCmd), (uint32_t) strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t*) (AtCmd), (uint32_t) strlen(AtCmd) - 2);
    }

    //-- finish ---
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
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
    const char CREG[] = "CREG";
    Retcode_T ret = RETCODE_OK;
    Retcode_T RetStat = RETCODE_OK;
    Retcode_T RetLac = RETCODE_OK;
    Retcode_T RetCi = RETCODE_OK;
    Retcode_T RetRat = RETCODE_OK;
    bool UrcFound = false;
    ret = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *) CREG, strlen(CREG));
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
                    (unsigned int ) data.Stat, (unsigned int ) data.Rat, (unsigned int ) data.Lac, (unsigned int ) data.Ci);
        }
        else
        {
            LOG_DEBUG("Cellular Network URC CREG stat:%u", (unsigned int ) data.Stat);
        }
    }
    if (!UrcFound)
    {
        ret = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }
    ret = CellularUtils_ConvertAtResponseRetcodeToCellularRetcode(ret);
    return ret;
}

//
//----------------------------------------------------------------------
//-- AT+COPS=2 (Deregister the module from the network)
//----------------------------------------------------------------------
//

static Retcode_T DeregisterFromNetwork(void *parameter, uint32_t ParameterLength)
{
    const char AtCmd[] = "AT+COPS=2\r\n";
    Retcode_T ret = RETCODE_OK;
    (void) parameter;
    (void) ParameterLength;
    //-- send AT command ---
    ret = CellularEngine_SendAtCommand((uint8_t *) AtCmd, (uint32_t) strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_COPS, (uint8_t *) AtCmd, (uint32_t) strlen(AtCmd) - 2);
    }

    //-- finish ---
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
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
    const char AtCmd[] = "AT+COPS=0\r\n";
    Retcode_T ret = RETCODE_OK;
    (void) parameter;
    (void) ParameterLength;
    //-- send AT command ---
    ret = CellularEngine_SendAtCommand((uint8_t *) AtCmd, (uint32_t) strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t *) AtCmd, (uint32_t) strlen(AtCmd) - 2);
    }

    //-- finish ---
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
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
            *value = (CellularNetwork_SelectionMode_T) number;
            return RETCODE_OK;
        }
    }
    //-- ### ERROR
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
        *value = (CellularNetwork_FormatOperatorName_T) number;
        return RETCODE_OK;
    }
    //-- ### ERROR
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

    const char AtCmd[] = "AT+COPS?\r\n";
    const char Cmd[] = "COPS";
    Retcode_T ret = RETCODE_OK;
    Retcode_T RetOptional = RETCODE_OK;
    CellularNetwork_CurrentOperator_T TmpOperator; //-- tmp until all succdessfully parsed
    memset(&TmpOperator, 0, sizeof(TmpOperator));
    //-- send AT command ---
    ret = CellularEngine_SendAtCommand((uint8_t*) AtCmd, strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        //   +COPS: <mode>[,<format>,<oper>[,<Rat>]]
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t *) AtCmd, (uint32_t) strlen(AtCmd) - 2);
        if (RETCODE_OK == ret)
        {
            ret = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t *) Cmd, (uint32_t) strlen(Cmd));
            if (RETCODE_OK == ret)
            {
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
            }
        } //--  ok
    } //-- send ok
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

//
//----------------------------------------------------------------------
//-- AT+UPSD= (set networks APN (depending on SIM card, may require also username and password)
//----------------------------------------------------------------------
//

static Retcode_T SetAPN(void *parameter, uint32_t ParameterLength)
{
    //-- Ref 1, p 305, ch 8.5 Packet switched data configuration +UPSD
    //   AT+UPSD=<ProfileId>,<ParamTag>,<ParamVal>
    Retcode_T ret = RETCODE_OK;

    char APN[ParameterLength + 1];
    strncpy(APN, (char*) parameter, ParameterLength);
    APN[ParameterLength] = 0;
    const uint32_t ProfileId = 0;
    const uint32_t ParamTag = 1;
    const uint32_t BufferSize = strlen(APN) + CELLULAR_NETWORK_SMALL_BUFFER_SIZE;
    char AtCmd[BufferSize];
    int count = snprintf(AtCmd, BufferSize - 3, "AT+UPSD=%u,%u,\"%s\"\r\n", (unsigned int) ProfileId, (unsigned int) ParamTag, APN);
    if ((0 > count) || (BufferSize - 1 <= ((uint32_t) count)))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    //-- send AT command ---
    ret = CellularEngine_SendAtCommand((uint8_t *) AtCmd, (uint32_t) strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t *) AtCmd, (uint32_t) strlen(AtCmd) - 2);
    }

    //-- finish ---
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
}

Retcode_T CellularNetwork_SetAPN(uint8_t* APN, uint32_t length)
{
    if (NULL == APN || 0 == length)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE,
            SetAPN, APN, length);
}

static Retcode_T SetPsdProfile(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = RETCODE_OK;
    int count;
    char cmd[CELLULAR_NETWORK_BUFFER_SIZE];
    const Cellular_PsdProfile_T *p = (const Cellular_PsdProfile_T *) parameter;
    uint32_t i;

    for (i = 0; i < countof(Cellular_PsdProfileItems); i++)
    {
        if (CELLULAR_PSD_PARAM_TYPE_U32 == Cellular_PsdProfileItems[i].Type)
        {
            count = snprintf(cmd, sizeof(cmd), "AT+UPSD=%u,%u,%u\r\n",
                    (unsigned int) p->Id,
                    Cellular_PsdProfileItems[i].Tag,
                    *(unsigned int *) (Cellular_PsdProfileItems[i].Offset + (uint32_t) p));
        }
        else if (CELLULAR_PSD_PARAM_TYPE_STRING == Cellular_PsdProfileItems[i].Type)
        {
            char *s = *(char **) (Cellular_PsdProfileItems[i].Offset + (uint32_t) p);

            if (NULL == s)
            {
                /* skip if not provided */
                continue;
            }

            count = snprintf(cmd, sizeof(cmd), "AT+UPSD=%u,%u,\"%s\"\r\n",
                    (unsigned int) p->Id,
                    Cellular_PsdProfileItems[i].Tag, s);
        }
        else
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }

        if (count <= 0 || (uint32_t) count >= sizeof(cmd))
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }

        retcode = CellularEngine_SendAtCommand((uint8_t *) cmd, strlen(cmd));

        if (RETCODE_OK != retcode)
        {
            break;
        }

        retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t *) cmd, strlen(cmd) - 2);

        if (RETCODE_OK != retcode)
        {
            break;
        }

        retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, retcode);

        if (RETCODE_OK != retcode)
        {
            break;
        }
    }

    return retcode;
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

//
//----------------------------------------------------------------------
//-- AT+UPSND= (get network assigend parameter)
//----------------------------------------------------------------------
//

/**
 * @brief AT+UPSND= (get network assigned parameter)
 */
static Retcode_T GetAssignedData(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode;
    uint8_t *arg;
    uint32_t length;
    uint32_t value;
    char cmd[CELLULAR_NETWORK_SMALL_BUFFER_SIZE];
    const char resp[] = "UPSND";

    CellularNetwork_AssignedParameterQuery_T* p = (CellularNetwork_AssignedParameterQuery_T*) parameter;

    uint32_t n = snprintf(cmd, sizeof(cmd), "AT+UPSND=%u,%u\r\n", (unsigned int) p->ProfileId, (unsigned int) p->Query);
    if (n >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    //-- send AT command ---
    retcode = CellularEngine_SendAtCommand((uint8_t*) cmd, strlen(cmd));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    //-- handle response events ---
    retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t*) cmd, strlen(cmd) - 2);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, (uint8_t*) resp, strlen(resp));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* parse ProfileId */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &length);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = CellularUtils_StrtolBounds(arg, length, (int32_t *) &value, 0, INT32_MAX);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode || value != p->ProfileId)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* parse ParamTag */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &length);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = CellularUtils_StrtolBounds(arg, length, (int32_t *) &value, 0, INT32_MAX);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode || value != (uint32_t) p->Query)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* parse DynamicParamVal */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &length);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    switch (p->Query)
    {
    case CELLULAR_NETWORK_ASSIGNED_PSD_PROFILE_STATUS:
        {
        retcode = CellularUtils_StrtolBounds(arg, length, (int32_t *) &value, 0, 1);
        if (RETCODE_OK == retcode)
        {
            p->IsPDPcontext = value;
        }
    }
        break;

    case CELLULAR_NETWORK_ASSIGNED_IPADR:
        {
        retcode = CellularUtils_ParseIpAddress(arg + 1, length - 2, &value);
        if (RETCODE_OK == retcode)
        {
            p->Ip = value;
        }
    }
        break;

    default:
        break;
    }

    AtResponseQueue_MarkBufferAsUnused();

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, retcode);
}

//
//----------------------------------------------------------------------
//-- AT+UPSDA= (activate PDP context)
//----------------------------------------------------------------------
//

/**
 * @brief AT+UPSDA= (activate PDP context)
 */
static Retcode_T ConfigurePdpContext(void *parameter, uint32_t ParameterLength)
{
    //AT+UPSDA=0,3 // activate PDP context, 3 == CELLULAR_PDP_CONTEXT_ACT_ACTIVATE
    BCDS_UNUSED(ParameterLength);
    const uint32_t ProfileId = 0;
    char AtCmd[CELLULAR_NETWORK_SMALL_BUFFER_SIZE]; //-- must be large enough for complete at command!
    Retcode_T ret = RETCODE_OK;
    CellularNetwork_ActionPdpContext_T* p = (CellularNetwork_ActionPdpContext_T*) parameter;
    int count = snprintf(AtCmd, CELLULAR_NETWORK_SMALL_BUFFER_SIZE - 3, "AT+UPSDA=%u,%u\r\n",
            (unsigned int) ProfileId,
            (unsigned int) (*p));

    if ((0 > count) || ( CELLULAR_NETWORK_SMALL_BUFFER_SIZE - 1 <= ((uint32_t) count))) //-- dynamic memory allocation is forbidden
    {
       return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    //-- send AT command ---
    ret = CellularEngine_SendAtCommand((uint8_t*) (AtCmd), (uint32_t) strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t*) (AtCmd), (uint32_t) strlen(AtCmd) - 2);
    }

    //-- finish ---
    ret = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_UPSDA, ret);
    if (RETCODE_OK != ret)
    {
        return ret;
    }

    if (CELLULAR_NETWORK_ACTION_PDP_CONTEXT_ACTIVATE != *p)
    {
        if (CELLULAR_NETWORK_ACTION_PDP_CONTEXT_DEACTIVATE == *p)
        {
            CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_DISCONNECTED, CELLULAR_EVENT_DISCONNECTED, NULL);
        }
        return ret;
    }

    /* U200 doesn't support URC UUPSDA, so request active PDP parameters here */
    CellularNetwork_AssignedParameterQuery_T req;
    req.ProfileId = 0;
    req.Query = CELLULAR_NETWORK_ASSIGNED_IPADR;

    ret = GetAssignedData(&req, sizeof(req));
    /* set device status and notify event */
    if (RETCODE_OK == ret && CELLULAR_DEVICESTATUS_CONNECTED != CellularEngine_GetDeviceStatus())
    {
        CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_CONNECTED, CELLULAR_EVENT_CONNECTED, (void *) req.Ip);
    }

    return ret;
}

static Retcode_T Cellular_ConfigurePdpContext(CellularNetwork_ActionPdpContext_T action)
{
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE,
            ConfigurePdpContext, &action, sizeof(CellularNetwork_ActionPdpContext_T));
}

Retcode_T CellularNetwork_ActivatePdpContext(void) //-- called by application
{
    return Cellular_ConfigurePdpContext(CELLULAR_NETWORK_ACTION_PDP_CONTEXT_ACTIVATE);
}

Retcode_T CellularNetwork_DeactivatePdpContext(void) //-- called by application
{
    return Cellular_ConfigurePdpContext(CELLULAR_NETWORK_ACTION_PDP_CONTEXT_DEACTIVATE);
}

Retcode_T Cellular_VerifyPdpContext(bool* success) //-- called by application
{
    if (NULL == success)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularNetwork_AssignedParameterQuery_T req;
    req.ProfileId = 0;
    req.IsPDPcontext = false;
    req.Query = CELLULAR_NETWORK_ASSIGNED_PSD_PROFILE_STATUS;

    Retcode_T retcode = CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE,
            GetAssignedData, &req, sizeof(req));

    if (RETCODE_OK == retcode)
    {
        *success = req.IsPDPcontext;
    }
    return retcode;
}

//
//----------------------------------------------------------------------
//-- AT+URAT= (select network radio access technology)
//   Ref 1, p 83, ch 7.5 Radio Access Technology (RAT) selection +URAT
//   Note: PRECONDITION: detached from network
//         Lisa-U200: Issue the AT+COPS=2 AT command to detach the module from the network (Cellular_DeregisterFromNetwork)
//----------------------------------------------------------------------
//

static Retcode_T SelectRadioAccessTechnology(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);
    Retcode_T ret = RETCODE_OK;
    if (NULL == parameter)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    CellularNetwork_RadioAccessTechnology_T* p = (CellularNetwork_RadioAccessTechnology_T*) parameter;
    //-- check if rat is applicable for LISA-U200 ---
    switch ((*p).Selection)
    {
    case CELLULAR_NETWORK_SEL_RAT_GSM_SINGLEMODE:
        case CELLULAR_NETWORK_SEL_RAT_GSM_UMTS_DUALMODE:
        case CELLULAR_NETWORK_SEL_RAT_UMTS_SINGLEMODE:
        break;
    default:
        //-- ### ERROR
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    switch ((*p).Preference)
    {
    case CELLULAR_NETWORK_PREF_RAT_GSM:
        case CELLULAR_NETWORK_PREF_RAT_UMTS:
        break;
    default:
        //-- ### ERROR
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    //-- prepare AT command ---
    const uint32_t BufferLength = CELLULAR_NETWORK_SMALL_BUFFER_SIZE + 6;
    char AtCmd[BufferLength];

    int count = snprintf(AtCmd, BufferLength - 3, "AT+URAT=%u,%u\r\n",
            (unsigned int) (*p).Selection,
            (unsigned int) (*p).Preference);
    if ((0 > count) || (BufferLength - 1 <= ((uint32_t) count)))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    //-- send AT command ---
    //-- Check the network registration status.
    ret = CellularEngine_SendAtCommand((uint8_t*) (AtCmd), (uint32_t) strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t*) (AtCmd), (uint32_t) strlen(AtCmd) - 2);
    }

    //-- finish ---
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
}

Retcode_T CellularNetwork_SelectRadioAccessTechnology(CellularNetwork_RadioAccessTechnology_T *rat)
{
    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, SelectRadioAccessTechnology, rat, sizeof(*rat));
}

//
//----------------------------------------------------------------------
//-- AT+URAT? (query network radio access technology)
//   Ref 1, p 83, ch 7.5 Radio Access Technology (RAT) selection +URAT
//----------------------------------------------------------------------
//
/**
 * @brief Parses AT+URAT? command arg number 1 from at command responce parser cmd arg event
 * @return RETCODE_OK only if successful
 * @see  Ref 2, p 88, ch 7.5 Radio Access Technology (RAT) selection +URAT
 */
static Retcode_T ConvertToRatSel(uint8_t* data, uint32_t length, CellularNetwork_SelectedRadioAccessTechnology_T* value) //-- Ref 1: SelectedRat
{
    if (NULL == data || 1 != length || NULL == value) //-- number range expected: 0..6 and never 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    int32_t number = 0;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, &number,
            (int32_t) CELLULAR_NETWORK_SEL_RAT_GSM_SINGLEMODE, //-- 0
            (int32_t) CELLULAR_NETWORK_SEL_RAT_UMTS_LTE_DUALMODE); //-- 6
    if (RETCODE_OK == ret)
    {
        *value = (CellularNetwork_SelectedRadioAccessTechnology_T) number;
        return RETCODE_OK;
    }
    //-- ### ERROR
    LOG_ERROR("CellularNetwork ConvertToRatSel value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

/**
 * @brief Parses AT+URAT? command arg number 2 from at command responce parser cmd arg event
 * @return RETCODE_OK only if successful
 * @see  Ref 2, p 88, ch 7.5 Radio Access Technology (RAT) selection +URAT
 */
static Retcode_T ConvertToRatPref(uint8_t* data, uint32_t length, CellularNetwork_PreferredRadioAccessTechnology_T* value)
{
    if (NULL == data || 1 != length || NULL == value) //-- number range expected: 0..3 without 1 and never 255
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    int32_t number = 0;
    Retcode_T ret = CellularUtils_StrtolBounds(data, length, &number,
            (int32_t) CELLULAR_NETWORK_PREF_RAT_GSM, //-- 0
            (int32_t) CELLULAR_NETWORK_PREF_RAT_LTE); //-- 3 (anyway never for LISA-U200)
    if (RETCODE_OK == ret)
    {
        if (1 != number)
        {
            *value = (CellularNetwork_PreferredRadioAccessTechnology_T) number;
            return RETCODE_OK;
        }
    }
    //-- ### ERROR
    LOG_ERROR("CellularNetwork ConvertToRatPref value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

static Retcode_T QueryRadioAccessTechnology(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    const char AtCmd[] = "AT+URAT?\r\n";
    const char Cmd[] = "URAT";
    Retcode_T ret = RETCODE_OK;
    Retcode_T RetOptional = RETCODE_OK;
    CellularNetwork_RadioAccessTechnology_T* p = (CellularNetwork_RadioAccessTechnology_T*) parameter;
    memset(p, 0, sizeof(CellularNetwork_RadioAccessTechnology_T));
    //-- send AT command ---
    //-- Check the network registration status.
    ret = CellularEngine_SendAtCommand((uint8_t*) AtCmd, strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        //  +URAT: <SelectedRat>[,<PreferredAct>]
        //  e.g.: +URAT: 1,2
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t*) AtCmd, (uint32_t) strlen(AtCmd) - 2);
        if (RETCODE_OK == ret)
        {
            ret = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, (uint8_t*) Cmd, (uint32_t) strlen(Cmd));
            if (RETCODE_OK == ret)
            {
                uint8_t* CmdArg = NULL;
                uint32_t LengthCmdArg = 0;
                ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
                if (RETCODE_OK == ret)
                {
                    ret = ConvertToRatSel(CmdArg, LengthCmdArg, &((*p).Selection));
                    if (RETCODE_OK == ret)
                        (*p).IsValidSelection = true;
                    AtResponseQueue_MarkBufferAsUnused();
                    //-- optional
                    CmdArg = NULL;
                    LengthCmdArg = 0;
                    RetOptional = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
                    if (RETCODE_OK == RetOptional)
                    {
                        ret = ConvertToRatPref(CmdArg, LengthCmdArg, &((*p).Preference));
                        if (RETCODE_OK == ret)
                            (*p).IsValidPreference = true;
                        AtResponseQueue_MarkBufferAsUnused();
                    }
                }
            }
        }
    }
    //-- finish ---
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
}

Retcode_T CellularNetwork_QueryRadioAccessTechnology(CellularNetwork_RadioAccessTechnology_T* rat)
{
    if (NULL == rat)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularNetwork_RadioAccessTechnology_T param;
    Retcode_T result = CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, QueryRadioAccessTechnology, &param, sizeof(param));
    if (RETCODE_OK == result)
    {
        *rat = param;
    }
    return result;
}

//----------------------------------------------------------------------
//-- AT+CSQ (Received Signal Strength Indication (RSSI))
//   Ref 1, p 76, ch 7.2 Signal quality +CSQ

/**
 * @brief Converts AT+CSQ command arg rssi from at command responce parser event
 * @return RETCODE_OK only if success
 * @see  Ref 2, p 80, ch 7.2 Signal quality +CSQ
 */
static Retcode_T ConvertToNetworkRssi(uint8_t* data, uint32_t length, CellularNetwork_Rssi_T* value)
{
    if (2 < length) //-- number range expected: 0..31 and 99
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
    //-- ### ERROR
    *value = CELLULAR_NETWORK_RSSI_UNKNOWN;
    LOG_ERROR("CellularNetwork ConvertToNetworkRssi value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

/**
 * @brief Converts AT+CSQ command arg qual from at command responce parser event
 * @return RETCODE_OK only if success
 * @see  Ref 2, p 80, ch 7.2 Signal quality +CSQ
 */
static Retcode_T ConvertToNetworkQuality(uint8_t* data, uint32_t length, CellularNetwork_Quality_T* value)
{
    if (2 < length) //-- number range expected: 0..7 and 99
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
    //-- ### ERROR
    *value = CELLULAR_NETWORK_QUALITY_UNKOWN;
    LOG_ERROR("CellularNetwork ConvertToNetworkQual value %i out of range", (int ) number);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED); //-- out of range, not expected
}

static Retcode_T QueryCurrentSignalQuality(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    const char AtCmd[] = "AT+CSQ\r\n";
    const char Cmd[] = "CSQ";
    Retcode_T ret = RETCODE_OK;
    CellularNetwork_SignalQuality_T* p = (CellularNetwork_SignalQuality_T*) parameter;
    memset(p, 0, sizeof(CellularNetwork_SignalQuality_T));
    (*p).IsValidRssi = false;
    (*p).IsValidQuality = false;
    //-- send AT command ---
    //-- Check the network registration status.
    ret = CellularEngine_SendAtCommand((uint8_t*) AtCmd, strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        //  +CSQ: <rssi>,<qual>
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT, (uint8_t*) AtCmd, (uint32_t) strlen(AtCmd) - 2);
        if (RETCODE_OK == ret)
        {
            ret = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, (uint8_t*) Cmd, (uint32_t) strlen(Cmd));
            if (RETCODE_OK == ret)
            {
                uint8_t* CmdArg = NULL;
                uint32_t LengthCmdArg = 0;
                ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
                if (RETCODE_OK == ret)
                {
                    ret = ConvertToNetworkRssi(CmdArg, LengthCmdArg, &((*p).Rssi));
                    if (RETCODE_OK == ret)
                        (*p).IsValidRssi = true;
                    AtResponseQueue_MarkBufferAsUnused();
                    CmdArg = NULL;
                    LengthCmdArg = 0;
                    ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &CmdArg, &LengthCmdArg);
                    if (RETCODE_OK == ret)
                    {
                        ret = ConvertToNetworkQuality(CmdArg, LengthCmdArg, &((*p).Quality));
                        if (RETCODE_OK == ret)
                            (*p).IsValidQuality = true;
                        AtResponseQueue_MarkBufferAsUnused();
                    }
                }
            }
        }
    }
    //-- finish ---
    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT, ret);
}

Retcode_T CellularNetwork_QueryCurrentSignalQuality(CellularNetwork_SignalQuality_T* NetworkSignalQuality) //-- AT+CSQ
{
    if (NULL == NetworkSignalQuality)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularNetwork_SignalQuality_T param;
    Retcode_T result = CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, QueryCurrentSignalQuality, &param, sizeof(param));
    if (RETCODE_OK == result)
    {
        *NetworkSignalQuality = param;
    }
    return result;
}

//
//----------------------------------------------------------------------
//-- AT+CGED=0 (Cell environment description)
//   Ref 1, p 100, ch 7.20 Cell environment description +CGED
//----------------------------------------------------------------------
//
/* Possible modem answer AT+CGED=0:
 * AT\r\nATZ\r\nAR\rAT<\r><\r><\n>ERROR<\r><\n>AT<\r><\r><\n>OK<\r><\n>AT+CGED?<\r><\r><\n>+CGED: 2<\r><\n><\r><\n>OK<\r
 * ><\n>AT+CGED=0<\r><\r><\n>+CGED: RAT:"UMTS",<\r><\n><\r><\n>URR:"ID"<\r><\n><\r><\n>DC:002, BP:0005, M:003, ERR: 0, R
 * C: 0, OOS:0,<\r><\n><\r><\n>BLER:-,TSIR:-,MSIR:-<\r><\n><\r><\n>HCS:1, HMD:0, LM:0,<\r><\n><\r><\n>Cell-ID:bcf4e8d, D
 * LF:10612, ULF: 9662, SC:24, RSCP LEV:46, ECN0 LEV:42, C:0, D:0, PSM: 0<\r><\n><\r><\n>Cell:G, B:- Arfcn:- RXLEV:- Bsi
 * c:- RV:-,<\r><\n><\r><\n>Cell:U, SC:24, RSCP LEV:46, ECN0 LEV:41, DLF:10564, RV:65532,<\r><\n><\r><\n>Cell:U, SC:24,
 * RSCP LEV:47, ECN0 LEV:39, DLF:10588, RV:65531,<\r><\n><\r><\n>Cell:NU, SC:24, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6
 * ,<\r><\n><\r><\n>Cell:NU, SC:8, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:32, RSCP LEV:0, E
 * CN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:100, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>C
 * ell:NU, SC:291, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:220, RSCP LEV:0, ECN0 LEV:0, DLF:
 * 10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:495, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:13,
 * RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:185, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r>
 * <\n><\r><\n>Cell:NU, SC:46, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:168, RSCP LEV:0, ECN0
 *  LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:463, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell
 *  :NU, SC:331, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:237, RSCP LEV:0, ECN0 LEV:0, DLF:10
 *  612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:21, RSCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NU, SC:228, R
 *  SCP LEV:0, ECN0 LEV:0, DLF:10612, RS:6,<\r><\n><\r><\n>Cell:NG, B:, Arfcn:9, RXLEV:0, Bsic:31, RS:1<\r><\n><\r><\n>C
 *  ell:NG, B:, Arfcn:11, RXLEV:0, Bsic:30, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:53, RXLEV:0, Bsic:32, RS:1<\r><\n><\r
 *  ><\n>Cell:NG, B:, Arfcn:54, RXLEV:0, Bsic:45, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:55, RXLEV:0, Bsic:52, RS:1<\r><
 *  \n><\r><\n>Cell:NG, B:, Arfcn:56, RXLEV:0, Bsic:50, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:57, RXLEV:0, Bsic:48, RS:
 *  1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:60, RXLEV:0, Bsic:29, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:62, RXLEV:0, Bsic:2
 *  7, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:62, RXLEV:0, Bsic:36, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:63, RXLEV:0,
 *  Bsic:25, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:65, RXLEV:0, Bsic:29, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:66, RXL
 *  EV:0, Bsic:40, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:67, RXLEV:0, Bsic:52, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:6
 *  8, RXLEV:0, Bsic:35, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:69, RXLEV:0, Bsic:59, RS:1<\r><\n><\r><\n>Cell:NG, B:, A
 *  rfcn:71, RXLEV:0, Bsic:24, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:74, RXLEV:0, Bsic:44, RS:1<\r><\n><\r><\n>Cell:NG,
 *   B:, Arfcn:78, RXLEV:0, Bsic:24, RS:1<\r><\n><\r><\n>Cell:NG, B:, Arfcn:78, RXLEV:0, Bsic:54, RS:1<\r><\n><\r><\n>RR
 *    measurement evaluation:<\r><\n><\r><\n>MeasId: 0, EventId:  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId
 *    :  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId:  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId
 *    :  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId:  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId
 *    :  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId:  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId
 *    :  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId:  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId
 *    :  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId:  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId
 *    :  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId:  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId
 *    :  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId:  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MeasId: 0, EventId
 *    :  0A, 0, 0, 0, 0, 0, 0<\r><\n><\r><\n>MM:<\r><\n><\r><\n>Process:CO, MMs:11, MMSs:80, MSC:A, T:0000 <\r><\n><\r><
 *    \n>Process:CS, MMs: 5, MMSs: 5, LUS:1, T:0004, L:0<\r><\n><\r><\n>Process:PS, MMs: 9, MMSs: 5, LUS:1, T:0008, L:0,
 *     GS:1629378508, R:0<\r><\n><\r><\n>Cell change counters:<\r><\n><\r><\n>CRT:  0, IRCR:  0<\r><\n><\r><\n>AIRCR:  0
 *     , IRHO:  0, AIRHO:  0<\r><\n><\r><\n>Equivalent PLMNs:<\r><\n><\r><\n>Mcc:262, Mnc:  2<\r><\n><\r><\n>Serving PLM
 *     N:<\r><\n><\r><\n>Mcc:262, Mnc:  2, Lac:1c96, RAC:  1 <\r><\n><\r><\n>OK<\r><\n>
 */
/* Possible modem answer AT+CGED=3:
 * AT+CGED=3<\r><\r><\n>+CGED: RAT:"UMTS",<\r><\n><\r><\n>Mcc:262, Mnc:  2, Lac:1c96, Ci:bcf4e8d, DLF:10612 , ULF:9662<\
 * r><\n><\r><\n>OK<\r><\n>
 */
/*
 * Focus: Mobile Network Code (Mnc), Location Area Code (Lac) and Cell Identity (Ci)
 */

//
//----------------------------------------------------------------------
//-- AT+CGED=<n> (Cell environment description)
//   Ref 1, p 100, ch 7.20 Cell environment description +CGED
//  <Mcc> Number Mobile Country Code, range 0 - 999 (3 digits). Other values are to be considered invalid / notavailable
//  <Mnc> Number Mobile Network Code, range 0 - 999 (1 to 3 digits). Other values are to be considered invalid / not available
//  <Lac> Number Location Area Code, range 0h-FFFFh (2 octets)
//  <Ci> Number Cell Identity, range 0h-FFFFh (2 octets)
//  <BSIC> Number Base Station Identify Code, range 0h-3Fh (6 bits)
//  <arfcn> Number Absolute Radio Frequency Channel Number, range 0 - 1023
//          The parameter value also decodes the band indicator bit (DCS or PCS) by means of the
//          most significant byte (8 means 1900 band) (i.e. if the parameter reports the value 33485, it
//          corresponds to 0x82CD, in the most significant byte there is the band indicator bit, so the
//          <arfcn> is 0x2CD (717) and belongs to 1900 band)
//----------------------------------------------------------------------
//

static Retcode_T QueryCellEnvironmentDescription(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Cellular_CellEnvironmentData_T* p = (Cellular_CellEnvironmentData_T*) parameter;

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, ATCMD_CELL_ENV_DESC);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT,
            (uint8_t *) ATCMD_CELL_ENV_DESC_RESP, (uint32_t) strlen(ATCMD_CELL_ENV_DESC_RESP));

    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    uint8_t *arg;
    uint32_t len;

    /* ignore RAT */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &len);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    AtResponseQueue_MarkBufferAsUnused();

    /* get cell environment description */
    retcode = AtResponseQueue_WaitForMiscContent(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, &arg, &len);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    unsigned int mcc, mnc, lac, ci;
    int count = sscanf((char *)arg, ATCMD_CELL_ENV_DESC_RESP_FMT, &mcc, &mnc, &lac, &ci);
    AtResponseQueue_MarkBufferAsUnused();

    if (ATCMD_CELL_ENV_DESC_ARG_COUNT != count)
    {
        LOG_ERROR("invalid response: %.*s", (unsigned int ) len, (char * )arg);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    p->Mcc = mcc;
    p->Mnc = mnc;
    p->Lac = lac;
    p->Ci = ci;

    return AtResponseQueue_WaitForNamedResponseCode(CELLULAR_NETWORK_RESPONSE_TIMEOUT_SHORT, AT_RESPONSE_CODE_OK);
}

Retcode_T CellularNetwork_QueryCellEnvironment(Cellular_CellEnvironmentData_T* data)
{
    if (NULL == data)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    return CellularRequest_Dispatch(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, QueryCellEnvironmentDescription, data, sizeof(*data));
}

Retcode_T CellularNetwork_Init(void)
{
    bool SimCardInserted = false;
    Retcode_T retcode = Cellular_IsSimCardInserted(&SimCardInserted);

    if (RETCODE_OK != retcode || !SimCardInserted)
    {
        LOG_ERROR("Did not find SIM card: %" PRIu32 ", %d", retcode, SimCardInserted);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_SIM_NOT_READY);
    }

    /* enable HEX mode */
    retcode = Cellular_EnableHexMode();
    if (RETCODE_OK != retcode)
    {
        LOG_ERROR("EnableHexMode failed: %" PRIu32, retcode);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* configure network */
    retcode = CellularNetwork_ConfigureRegistrationInformation(CELLULAR_NETWORK_REG_INFO_URC);
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

Retcode_T CellularNetworkUrc_UUPSDA(void)
{
    const char *cmd = "UUPSDA";
    Retcode_T retcode;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *) cmd, strlen(cmd));

    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    LOG_INFO("URC for UUPSDA");

    uint8_t *buffer;
    uint32_t BufferLen;

    /* get result */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    uint32_t result;
    retcode = CellularUtils_StrtolBounds(buffer, BufferLen, (int32_t*) &result, 0, (int32_t) CELLULAR_CME_ERROR_MAX);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* get ip */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    uint32_t ip;
    retcode = CellularUtils_ParseIpAddress(buffer + 1, BufferLen - 2, &ip);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* set device status and notify event */
    if ((uint32_t) CELLULAR_CME_ERROR_NONE == result && CELLULAR_DEVICESTATUS_CONNECTED != CellularEngine_GetDeviceStatus())
    {
        CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_CONNECTED, CELLULAR_EVENT_CONNECTED, (void *) ip);
    }

    /* update device error */
    CellularEngine_SetDeviceError(result);

    return retcode;
}

Retcode_T CellularNetworkUrc_UUPSDD(void)
{
    const char *cmd = "UUPSDD";
    Retcode_T retcode;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *) cmd, strlen(cmd));

    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    LOG_INFO("URC for UUPSDD");

    uint8_t *buffer;
    uint32_t BufferLen;

    /* get ProfileId */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_NETWORK_TIMEOUT_ENQUEUE, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    uint32_t result;
    /* So far only one profile is used */
    retcode = CellularUtils_StrtolBounds(buffer, BufferLen, (int32_t*) &result, 0, 0);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* set device status and notify event */
    CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_DISCONNECTED, CELLULAR_EVENT_DISCONNECTED, NULL);

    return retcode;
}
