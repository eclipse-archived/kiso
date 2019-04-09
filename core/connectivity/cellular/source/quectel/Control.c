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
 * @brief Control functions for the modem
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_CONTROL

#include "CellularModule.h"
#include "CellularBsp.h"
#include <stdio.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "CellularResponseQueue.h"
#include "CellularAtResponseParser.h"
#include "CellularCommon.h"
#include "CellularEngine.h"
#include "CellularUtils.h"
#include "CellularBsp.h"
#include "Command.h"
#include "CellularUrc.h"
#include "BCDS_Logging.h"
#include "Control.h"

#define CELLULAR_CONTROL_ENQUEUE_TIMEOUT                   (UINT32_C(1000))
#define CELLULAR_CONTROL_DEQUEUE_TIMEOUT                   (UINT32_C(100))
#define CELLULAR_CONTROL_STARTUP_AT_RESPONSE_TIMEOUT       (UINT32_C(1000))
#define CELLULAR_CONTROL_STARTUP_TIMEOUT                   (UINT32_C(20000))
#define CELLULAR_CONTROL_SHUTDOWN_TIMEOUT                  (UINT32_C(20000))
#define CELLULAR_CONTROL_SHUTDOWN_DELAY                    (UINT32_C(1000))
#define CELLULAR_RESPONSE_TIMEOUT                          (UINT32_C(5000))
#define CELLULAR_CONTROL_CMD_BUFFER_SIZE                   (UINT32_C(32))

extern CellularInitStatus_T CellularInitStatus;

static Retcode_T ProbeCellularForResponsiveness(void)
{
    Retcode_T retcode = CellularEngine_SendAtCommand((uint8_t *) ATCMD_AT, strlen(ATCMD_AT));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    uint8_t *arg;
    uint32_t len;
    uint32_t t0 = xTaskGetTickCount();
    uint32_t dt;

    while ((dt = xTaskGetTickCount() - t0) < CELLULAR_CONTROL_STARTUP_AT_RESPONSE_TIMEOUT)
    {
        /* wait for any response */
        AtResponseQueueEntry_T *entry;
        if (RETCODE_OK != AtResponseQueue_GetEvent(CELLULAR_CONTROL_STARTUP_AT_RESPONSE_TIMEOUT - dt, &entry))
        {
            continue;
        }
        /* consume any urc */
        (void) CellularUrc_HandleResponses();
        /* check the command response */
        if (RETCODE_OK == AtResponseQueue_WaitForMiscContent(0, &arg, &len))
        {
            if (0 == memcmp(arg, ATCMD_AT_RESP, strlen(ATCMD_AT_RESP)))
            {
                AtResponseQueue_MarkBufferAsUnused();
                return AtResponseQueue_WaitForNamedResponseCode(CELLULAR_CONTROL_DEQUEUE_TIMEOUT, AT_RESPONSE_CODE_OK);
            }
            AtResponseQueue_MarkBufferAsUnused();
        }
    }
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
}

static Retcode_T StartupCellularCommon(void)
{
    AtResponseQueue_Reset();
    CellularEngine_SetFlukeCharFilterEnabled(true);
    memset(&CellularInitStatus, 0, sizeof(CellularInitStatus));

    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
    uint32_t t0 = xTaskGetTickCount();
    uint32_t dt;

    while ((dt = xTaskGetTickCount() - t0) < CELLULAR_CONTROL_STARTUP_TIMEOUT)
    {
        /* wait for any response */
        AtResponseQueueEntry_T *entry;
        if (RETCODE_OK != AtResponseQueue_GetEvent(CELLULAR_CONTROL_STARTUP_TIMEOUT - dt, &entry))
        {
            continue;
        }

        /* handle URC responses */
        if (RETCODE_OK != CellularEngine_HandleEvents())
        {
            continue;
        }

        /* check modem init status */
        if (CellularInitStatus.MeAllFuncAvailable && CellularInitStatus.SimCardPinReady
                && CellularInitStatus.SmsInitDone && CellularInitStatus.PhonebookInitDone)
        {
            CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_INIT, CELLULAR_EVENT_INITIALIZING, NULL);
            retcode = RETCODE_OK;
            break;
        }
    }

    CellularEngine_SetFlukeCharFilterEnabled(false);
    return retcode;
}

static Retcode_T ResetCellular(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    LOG_DEBUG("modem reset");
    Retcode_T retcode = CellularBsp_Reset();
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    return StartupCellularCommon();
}

static Retcode_T StartupCellular(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    LOG_DEBUG("modem startup");
    Retcode_T retcode = CellularBsp_Startup();
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = StartupCellularCommon();
    if (RETCODE_OK != retcode)
    {
        /* try to recover the module, i.e. apply modem default configuration and reset */
        (void) CellularEngine_SendAtCommand((uint8_t *) ATCMD_SET_DEFAULT_CONFIGURATION, strlen(ATCMD_SET_DEFAULT_CONFIGURATION));
        (void) AtResponseQueue_WaitForNamedResponseCode(CELLULAR_RESPONSE_TIMEOUT, AT_RESPONSE_CODE_OK);
        return ResetCellular(NULL, 0);
    }

    return retcode;
}

static Retcode_T ShutdownCellular(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    LOG_DEBUG("modem shutdown");
    Retcode_T retcode = CellularBsp_Shutdown();
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    AtResponseQueue_Reset();
    CellularEngine_SetFlukeCharFilterEnabled(true);
    memset(&CellularInitStatus, 0, sizeof(CellularInitStatus));

    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
    uint32_t t0 = xTaskGetTickCount();
    uint32_t dt;

    while ((dt = xTaskGetTickCount() - t0) < CELLULAR_CONTROL_SHUTDOWN_TIMEOUT)
    {
        /* wait for any response */
        AtResponseQueueEntry_T *entry;
        if (RETCODE_OK != AtResponseQueue_GetEvent(CELLULAR_CONTROL_SHUTDOWN_TIMEOUT - dt, &entry))
        {
            continue;
        }

        /* handle URC responses */
        if (RETCODE_OK != CellularEngine_HandleEvents())
        {
            continue;
        }

        /* check modem init status */
        if (CellularInitStatus.MePoweredDown)
        {
            CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_DISCONNECTED, CELLULAR_EVENT_DISCONNECTED, NULL);
            retcode = RETCODE_OK;
            vTaskDelay(CELLULAR_CONTROL_SHUTDOWN_DELAY);
            break;
        }
    }

    CellularEngine_SetFlukeCharFilterEnabled(false);
    return retcode;
}

static Retcode_T GetPowerStatus(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);
    Cellular_PowerStatus_T* PowerStatus = (Cellular_PowerStatus_T*) parameter;

    if (NULL == PowerStatus)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (RETCODE_OK == ProbeCellularForResponsiveness())
    {
        *PowerStatus = CELLULAR_POWER_STATUS_ON;
    }
    else
    {
        *PowerStatus = CELLULAR_POWER_STATUS_OFF;
    }

    return RETCODE_OK;
}

/* Put function implementations here */
Retcode_T Cellular_Startup(void)
{
    return CellularRequest_Dispatch(CELLULAR_CONTROL_ENQUEUE_TIMEOUT, StartupCellular, NULL, 0);
}

Retcode_T Cellular_Shutdown(void)
{
    return CellularRequest_Dispatch(CELLULAR_CONTROL_ENQUEUE_TIMEOUT, ShutdownCellular, NULL, 0);
}

Retcode_T Cellular_Reset(void)
{
    return CellularRequest_Dispatch(CELLULAR_CONTROL_ENQUEUE_TIMEOUT, ResetCellular, NULL, 0);
}

Retcode_T Cellular_GetPowerStatus(Cellular_PowerStatus_T* PowerStatus)
{
    return CellularRequest_Dispatch(CELLULAR_CONTROL_ENQUEUE_TIMEOUT, GetPowerStatus, PowerStatus, sizeof(PowerStatus));
}

//----------------------------------------------------------------------
/**
 * @brief   Get ICCID (Integrated Circuit Card ID) of the SIM-card
 * @param[out]  buffer with size of at least 20 bytes
 * @return  RETCODE_OK only if success
 * @see      Ref 1, p36f, ch 4.12 Card identification +CCID
 */

typedef struct
{
    uint8_t *Iccid;
    uint32_t *Length;
    uint32_t MaxLength;
} Cellular_IccidQuery_T;

static Retcode_T GetIccid(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Cellular_IccidQuery_T* p = (Cellular_IccidQuery_T*) parameter;

    if (NULL == p || NULL == p->Iccid || NULL == p->Length || 0 == p->MaxLength)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_RESPONSE_TIMEOUT, ATCMD_CARD_IDENTIFICATION);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    uint8_t* arg;
    uint32_t len;

    retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_RESPONSE_TIMEOUT,
            (uint8_t *) ATCMD_CARD_IDENTIFICATION_RESP, strlen(ATCMD_CARD_IDENTIFICATION_RESP));
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_RESPONSE_TIMEOUT, &arg, &len);
        if (RETCODE_OK == retcode)
        {
            if (len < (p->MaxLength - 1))
            {
                memcpy(p->Iccid, arg, len);
                p->Iccid[len] = '\0';
                *(p->Length) = len;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_DRIVER_ERROR_CMD_ARG);
            }
            AtResponseQueue_MarkBufferAsUnused();
            /* reset device error */
            CellularEngine_SetDeviceError(RETCODE_OK);
            return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_RESPONSE_TIMEOUT, retcode);
        }
    }
    else if ((RETCODE_OK == AtResponseQueue_WaitForNamedCmd(CELLULAR_RESPONSE_TIMEOUT,
            (uint8_t *) ATCMD_CME_ERROR_RESP, strlen(ATCMD_CME_ERROR_RESP)))
            && (RETCODE_OK == AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_RESPONSE_TIMEOUT, &arg, &len)))
    {
        int32_t error;
        if (RETCODE_OK == CellularUtils_StrtolBounds(arg, len, &error, (int) CELLULAR_CME_ERROR_NONE, (int) CELLULAR_CME_ERROR_MAX))
        {
            /* set device error */
            CellularEngine_SetDeviceError((Retcode_T) error);
        }
        AtResponseQueue_MarkBufferAsUnused();
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_SIM_NOT_READY);
    }

    return retcode;
}

Retcode_T Cellular_GetIccid(uint8_t *iccid, uint32_t MaxLength, uint32_t* length) //-- MaxLength expected to be at least CELLULAR_BUFFER_SIZE_ICCID
{
    if (NULL == iccid || 0 == MaxLength || NULL == length)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Cellular_IccidQuery_T param;
    param.Iccid = iccid;
    param.Length = length;
    param.MaxLength = MaxLength;

    return CellularRequest_Dispatch(CELLULAR_CONTROL_ENQUEUE_TIMEOUT, GetIccid, &param, sizeof(param));
}

Retcode_T Cellular_EnableEcho(bool enable)
{
    Retcode_T retcode;
    char cmd[CELLULAR_CONTROL_CMD_BUFFER_SIZE];
    (void) snprintf(cmd, sizeof(cmd), ATCMD_SET_CMD_ECHO_FMT, !!enable);

    retcode = CellularEngine_SendAtCommand((uint8_t *) cmd, strlen(cmd));
    if (RETCODE_OK == retcode && !enable)
    {
        /* Ignore command echo */
        (void) AtResponseQueue_IgnoreEvent(CELLULAR_RESPONSE_TIMEOUT);
    }

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_RESPONSE_TIMEOUT, retcode);
}

Retcode_T Cellular_IsSimCardInserted(bool *SimCardInserted)
{
    if (NULL == SimCardInserted)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    uint8_t IccidBuffer[CELLULAR_BUFFER_SIZE_ICCID];
    uint32_t IccidLength;
    Retcode_T retcode = Cellular_GetIccid(IccidBuffer, CELLULAR_BUFFER_SIZE_ICCID, &IccidLength);

    if (RETCODE_OK == retcode)
    {
        *SimCardInserted = true;
    }
    else if (RETCODE_CELLULAR_SIM_NOT_READY == (CellularResponseCode_T) Retcode_GetCode(retcode))
    {
        *SimCardInserted = false;
        retcode = RETCODE_OK;
    }

    return retcode;
}
