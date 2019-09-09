/**********************************************************************************************************************
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
 **********************************************************************************************************************/

/*---------------------- INCLUDED HEADERS ---------------------------------------------------------------------------*/
#include "AppInfo.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID CGW_APP_MODULE_ENV_SENSOR
#include "sensors_common.h"

#include "BSP_CommonGateway.h"
#include "accel_sensor.h"
#include "env_sensor.h"
#include "Kiso_BSP_BME280.h"
#include "Kiso_BSP_BMA280.h"
#include "Kiso_BSP_LED.h"
#include "Kiso_Logging.h"

#include "Kiso_I2CTransceiver.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID CGW_APP_MODULE_ENV_SENSOR

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void I2CCallback(I2C_T i2c, struct MCU_I2C_Event_S event);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static I2cTranceiverHandle_T i2cTransceiverStruct;
static I2C_T i2cHandle;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
    Retcode_T returnValue = RETCODE_OK;

    returnValue = Logging_Init(Logging_SyncRecorder, Logging_UARTAppender);
    if (RETCODE_OK == returnValue)
    {
        LOG_DEBUG(" Logging was started successfully");
    }
    if (RETCODE_OK == returnValue)
    {
        i2cHandle = BSP_BMA280_GetHandle(0);
        if (NULL == i2cHandle)
        {
            i2cHandle = BSP_BME280_GetHandle(0);
        }
        if (NULL == i2cHandle)
        {
            returnValue = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_I2C_HANDLE_DOES_NOT_EXIST);
        }
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = I2CTransceiver_Init(&i2cTransceiverStruct, i2cHandle);
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = MCU_I2C_Initialize(i2cHandle, I2CCallback);
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = Accelerometer_Init(&i2cTransceiverStruct);
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = Environmental_Init(&i2cTransceiverStruct);
    }
    if (RETCODE_OK == returnValue)
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_GREEN_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }
    else
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_RED_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }
}
/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

void I2CCallback(I2C_T i2c, struct MCU_I2C_Event_S event)
{
    I2CTransceiver_LoopCallback(&i2cTransceiverStruct, event);
}
