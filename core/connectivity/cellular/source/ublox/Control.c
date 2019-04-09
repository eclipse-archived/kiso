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
#include "BCDS_Logging.h"
#include "Control.h"

#define CELLULAR_CONTROL_ENQUEUE_TIMEOUT                   (UINT32_C(1000))
#define CELLULAR_CONTROL_STARTUP_AT_RESPONSE_TIMEOUT       (UINT32_C(1000))
#define CELLULAR_CONTROL_STARTUP_RETRY_PERIOD              (UINT32_C(3000))
#define CELLULAR_CONTROL_STARTUP_DELAY                     (UINT32_C(5000))
#define CELLULAR_CONTROL_STARTUP_MAX_RETRY_COUNT           (UINT32_C(3))

#define CELLULAR_CONTROL_STARTUP_PROBE_COMMAND             ("AT\r\n")
#define CELLULAR_CONTROL_STARTUP_PROBE_COMMAND_LENGTH      (strlen(CELLULAR_CONTROL_STARTUP_PROBE_COMMAND))

#define CELLULAR_CONTROL_SHUTDOWN_WAIT_TIME                (UINT32_C(3000))
#define CELLULAR_CONTROL_SHUTDOWN_MAX_RETRY_COUNT          (UINT32_C(3))

#define CELLULAR_CONTROL_RESET_RETRY_PERIOD                (UINT32_C(1000))
#define CELLULAR_CONTROL_RESET_MAX_RETRY_COUNT             (UINT32_C(4))

#define CELLULAR_CONTROL_GET_STATUS_RETRY_PERIOD           (UINT32_C(500))
#define CELLULAR_CONTROL_GET_STATUS_MAX_RETRY_COUNT        (UINT32_C(2))

#define CELLULAR_ICCID_OF_UNREADY_SIM                      ("00000000000000000000")

#define CELLULAR_RESPONSE_TIMEOUT                          (UINT32_C(5000))

#define CELLULAR_CONTROL_CMD_BUFFER_SIZE                   (UINT32_C(32))

static Retcode_T ResetCellular(void* parameter, uint32_t ParameterLength);

static Retcode_T ProbeCellularForResponsiveness(void)
{
    Retcode_T retcode;
    AtResponseQueueEntry_T *e;

    retcode = CellularEngine_SendAtCommand((uint8_t*) CELLULAR_CONTROL_STARTUP_PROBE_COMMAND, CELLULAR_CONTROL_STARTUP_PROBE_COMMAND_LENGTH);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
    while (AtResponseQueue_GetEvent(CELLULAR_CONTROL_STARTUP_AT_RESPONSE_TIMEOUT, &e) == RETCODE_OK)
    {
        if (AT_EVENT_TYPE_MISC == e->Type && e->BufferLength > 0
                && 0 == memcmp(CELLULAR_CONTROL_STARTUP_PROBE_COMMAND, e->Buffer, e->BufferLength))
        {
            retcode = RETCODE_OK;
        }
        AtResponseQueue_MarkBufferAsUnused();
    }

    return retcode;
}

static Retcode_T StartupCellular(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode;

    /* Set device status */
    CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_INIT, CELLULAR_EVENT_INITIALIZING, NULL);

    AtResponseQueue_Reset();

    /* During startup, we ignore any fluke characters (as they may occur and destroy our parser) */
    CellularEngine_SetFlukeCharFilterEnabled(true);

    retcode = CellularBsp_Startup();
    if (RETCODE_OK == retcode)
    {
        //-- without this wait time each 30rd get ICCID failed
        vTaskDelay(CELLULAR_CONTROL_STARTUP_DELAY / portTICK_PERIOD_MS);

        /* Cellular has now received its POWER_ON command and should start booting.
         * We will probe the modem several times until we receive a valid answer ... or timeout. */
        LOG_INFO("Cellular_Startup: hardware-POWER_ON command sent!");

        Retcode_T CellularIsResponsive = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        for (uint32_t i = 0; RETCODE_OK != CellularIsResponsive && i < CELLULAR_CONTROL_STARTUP_MAX_RETRY_COUNT; i++)
        {
            CellularIsResponsive = ProbeCellularForResponsiveness();

            if (RETCODE_OK != CellularIsResponsive)
                vTaskDelay(CELLULAR_CONTROL_STARTUP_RETRY_PERIOD / portTICK_PERIOD_MS);
        }

        if (RETCODE_OK == CellularIsResponsive)
        {
            /* Everything fine, modem is responsive! Exit gracefully */
            LOG_INFO("Cellular_Startup: modem powered up!");
            retcode = RETCODE_OK;
        }
        else
        {
            /* OK, modem is still not responding ... try resetting */
            LOG_INFO("Cellular_Startup: modem not responding, trying reset...");

            (void) ResetCellular(NULL, 0);
        }
    }
    else
    {
        /* Something's wrong with the BSP?! */
        LOG_ERROR("Cellular_Startup: unexpected error from BSP!");
    }

    CellularEngine_SetFlukeCharFilterEnabled(false);

    return retcode;
}

static Retcode_T ShutdownCellular(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = CellularBsp_Shutdown();
    if (RETCODE_OK != retcode)
    {
        LOG_ERROR("Cellular_Shutdown: hardware-POWER_OFF failed!");
    }

    return retcode;
}

static Retcode_T ResetCellular(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(parameter);
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = RETCODE_OK;

    /* During reset, we ignore any fluke characters (as they may occur and destroy our parser) */
    CellularEngine_SetFlukeCharFilterEnabled(true);

    if (RETCODE_OK == retcode)
    {
        retcode = CellularBsp_Reset();
    }

    if (RETCODE_OK == retcode)
    {
        /* Cellular has now received its RESET command and should start booting.
         * We will probe the modem several times until we receive a valid answer ... or timeout. */
        LOG_INFO("Cellular_Reset: hardware-RESET command sent!");
        vTaskDelay(CELLULAR_CONTROL_STARTUP_DELAY / portTICK_PERIOD_MS);

        Retcode_T CellularIsResponsive = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
        for (uint32_t i = 0; RETCODE_OK != CellularIsResponsive && i < CELLULAR_CONTROL_RESET_MAX_RETRY_COUNT; i++)
        {
            CellularIsResponsive = ProbeCellularForResponsiveness();

            if (RETCODE_OK != CellularIsResponsive)
                vTaskDelay(CELLULAR_CONTROL_RESET_RETRY_PERIOD / portTICK_PERIOD_MS);
        }

        if (RETCODE_OK == CellularIsResponsive)
        {
            /* Everything fine, modem is responsive! Exit gracefully */
            LOG_INFO("Cellular_Reset: modem powered up!");
            retcode = RETCODE_OK;
        }
        else
        {
            /* OK, modem is still not responding ... we give up! */
            LOG_ERROR("Cellular_Reset: modem not responding!");
            retcode = CellularIsResponsive;
        }
    }
    else
    {
        /* Something's wrong with the BSP?! */
        LOG_ERROR("Cellular_Reset: unexpected error from BSP!");
    }

    CellularEngine_SetFlukeCharFilterEnabled(false);

    /* set device status and notify event */
    if (RETCODE_OK == retcode)
    {
        CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_IDLE, CELLULAR_EVENT_INITIALIZING, NULL);
    }

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

    *PowerStatus = CELLULAR_POWER_STATUS_UNKNOWN;

    Retcode_T CellularIsResponsive = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
    for (uint32_t i = 0; RETCODE_OK != CellularIsResponsive && i < CELLULAR_CONTROL_GET_STATUS_MAX_RETRY_COUNT; i++)
    {
        CellularIsResponsive = ProbeCellularForResponsiveness();

        if (RETCODE_OK != CellularIsResponsive)
            vTaskDelay(CELLULAR_CONTROL_GET_STATUS_RETRY_PERIOD / portTICK_PERIOD_MS);
    }

    if (RETCODE_OK == CellularIsResponsive)
    {
        LOG_INFO("Cellular_GetPowerStatus: modem powered up!");
        *PowerStatus = CELLULAR_POWER_STATUS_ON;
    }
    else
    {
        LOG_INFO("Cellular_GetPowerStatus: modem not responding!");
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
        if (0 == strncmp((const char *) IccidBuffer, CELLULAR_ICCID_OF_UNREADY_SIM, IccidLength))
        {
            retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_SIM_NOT_READY);
        }
    }
    else if (Retcode_GetCode(retcode) == (uint32_t) RETCODE_CELLULAR_RESPONDED_ERROR)
    {
        retcode = RETCODE_OK;
        *SimCardInserted = false;
    }

    return retcode;
}

//----------------------------------------------------------------------
/**
 * @brief   Get ICCID (Integrated Circuit Card ID) of the SIM-card
 * @param[out]  buffer with size of at least 20 bytes
 * @retval  RETCODE_OK only if success
 */

struct Cellular_IccidQuery_S
{
    char Iccid[CELLULAR_BUFFER_SIZE_ICCID];
};
typedef struct Cellular_IccidQuery_S Cellular_IccidQuery_T;

static Retcode_T GetIccid(void *parameter, uint32_t ParameterLength)
{
    const char AtCmd[] = "AT+CCID?\r\n"; //-- to be sent
    const char Cmd[] = "CCID"; //-- arg begin
    Retcode_T ret = RETCODE_OK;

    if (NULL == parameter || sizeof(Cellular_IccidQuery_T) != ParameterLength)
    {
        //-- ### ERROR
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Cellular_IccidQuery_T* p = (Cellular_IccidQuery_T*) parameter;

    ret = CellularEngine_SendAtCommand((uint8_t*) AtCmd, (uint32_t) strlen(AtCmd));
    if (RETCODE_OK == ret)
    {
        //-- handle response events ---
        //   AT+CCID? +CCID: 8939107900010087330 OK
        ret = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_RESPONSE_TIMEOUT, (uint8_t*) AtCmd, (uint32_t) strlen(AtCmd) - 2);
        if (RETCODE_OK == ret)
        {

            ret = AtResponseQueue_WaitForNamedCmd(CELLULAR_RESPONSE_TIMEOUT, (uint8_t*) Cmd, (uint32_t) strlen(Cmd));
            if (RETCODE_OK == ret)
            {

                uint8_t* CmdArg = NULL;
                uint32_t LengthCmdArg = 0;
                ret = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_RESPONSE_TIMEOUT, &CmdArg, &LengthCmdArg);
                if (RETCODE_OK == ret)
                {
                    if (CmdArg && (0 < LengthCmdArg) && (CELLULAR_BUFFER_SIZE_ICCID > LengthCmdArg))
                    {
                        memset((*p).Iccid, 0, sizeof((*p).Iccid));
                        strncpy((*p).Iccid, (const char*) CmdArg, LengthCmdArg);
                        (*p).Iccid[LengthCmdArg] = 0;
                    }
                    else
                    {
                        //-- ### ERROR
                        //-- failed to extract data
                        ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_DRIVER_ERROR_CMD_ARG);
                    }
                    AtResponseQueue_MarkBufferAsUnused();
                }
            }
        }

        ret = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_RESPONSE_TIMEOUT, ret);
    }

    return ret;
}

Retcode_T Cellular_GetIccid(uint8_t *iccid, uint32_t MaxLength, uint32_t* length) //-- MaxLength expected to be at least CELLULAR_BUFFER_SIZE_ICCID
{
    if (NULL == iccid || NULL == length)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    Cellular_IccidQuery_T param;
    Retcode_T result = CellularRequest_Dispatch(CELLULAR_CONTROL_ENQUEUE_TIMEOUT, GetIccid, &param, sizeof(param));
    if (RETCODE_OK == result)
    {
        uint32_t len = (uint32_t) strlen(param.Iccid);
        if (MaxLength < len + 1)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        else
        {
            memcpy(iccid, param.Iccid, len);
            iccid[len] = 0; //-- if caller should use buffer as string we explicit zero terminate for reason of saftey
            *length = len;
        }
    }
    return result;
}

Retcode_T Cellular_EnableEcho(bool enable)
{
    Retcode_T retcode;
    char cmd[CELLULAR_CONTROL_CMD_BUFFER_SIZE];
    (void) snprintf(cmd, sizeof(cmd), "ATE%d\r\n", !!enable);

    retcode = CellularEngine_SendAtCommand((uint8_t *) cmd, strlen(cmd));
    if (RETCODE_OK == retcode && !enable)
    {
        /* Ignore command echo */
        (void) AtResponseQueue_IgnoreEvent(CELLULAR_RESPONSE_TIMEOUT);
    }

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_RESPONSE_TIMEOUT, retcode);
}

//----------------------------------------------------------------------
