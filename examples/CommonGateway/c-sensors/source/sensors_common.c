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
#define KISO_MODULE_ID KISO_APP_MODULE_SENSORS_COMMON

#include "sensors_common.h"
#include "accel_sensor.h"
#include "env_sensor.h"
#include "BSP_CommonGateway.h"
#include "Kiso_Retcode.h"
#include "Kiso_BSP_BME280.h"
#include "Kiso_BSP_BMA280.h"
#include "Kiso_BSP_LED.h"
#include "Kiso_Logging.h"
#include "Kiso_CmdProcessor.h"
#include "Kiso_I2CTransceiver.h"
#include "Kiso_BSP_Board.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/
void appPeriodicTest(void *CmdProcessorHandle, uint32_t param2);
void I2CCallback(I2C_T i2c, struct MCU_I2C_Event_S event);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static I2cTranceiverHandle_T i2cTransceiverStruct;
static I2C_T i2cHandle;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

void appInitSystem(void *CmdProcessorHandle, uint32_t param2)
{
    (void)CmdProcessorHandle;
    (void)param2;
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
            returnValue = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
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
        returnValue = SensorAccelerometer_Init(&i2cTransceiverStruct);
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = SensorEnvironment_Init(&i2cTransceiverStruct);
    }
    if (RETCODE_OK == returnValue)
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_GREEN_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }
    else
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_RED_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = CmdProcessor_Enqueue((CmdProcessor_T *)CmdProcessorHandle, appPeriodicTest, CmdProcessorHandle, UINT32_C(0));
    }
}
/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
void appPeriodicTest(void *CmdProcessorHandle, uint32_t param2)
{
    (void)param2;
    struct bme280_data sensorEnvironmentData = {0};
    struct bma2x2_accel_data sensorAccelerometerData = {0};

    Retcode_T returnValue = RETCODE_OK;

    if (RETCODE_OK == returnValue)
    {
        returnValue = SensorEnvironment_Read(&sensorEnvironmentData);
        if (RETCODE_OK == returnValue)
        {
#ifdef BME280_FLOAT_ENABLE
#error BME280_FLOAT_ENABLE not supported for this example
#else
            /* Finally we need to apply static temperature offset correction.
             * As no two sensors can be the same, this coefficient will be
             * different from sensor to sensor.
             * */
            const int32_t DEFAULT_TEMPERATURE_OFFSET = -210;
            int32_t temperature = sensorEnvironmentData.temperature + DEFAULT_TEMPERATURE_OFFSET;

            //  Unit is in 1024 * % relative humidity
            uint32_t humidity = (sensorEnvironmentData.humidity * 1000) / 1024;
#ifdef BME280_64BIT_ENABLE
            // If macro "BME280_64BIT_ENABLE" is enabled, which it is by default, the unit is 100 * Pascal
            uint32_t pressure = sensorEnvironmentData.pressure;
#else
            // If this macro is disabled, Then the unit is in Pascal
            uint32_t pressure = sensorEnvironmentData.pressure * 100;
#endif
            // clang-format off
            LOG_DEBUG("Temperature: %ld.%02lu;\tPresure: %lu.%03luhPa;\tHumidity: %lu.%03lu%%RH",
                    temperature / 100,
                    temperature % 100,
                    pressure / 10000,
                    (pressure % 10000) / 10,
                    humidity / 1000,
                    humidity % 1000);
            // clang-format on
#endif
        }
        else
        {
            LOG_DEBUG("Env sensor read failed!\r\n");
        }
    }

    if (RETCODE_OK == returnValue)
    {
        returnValue = SensorAccelerometer_Read(&sensorAccelerometerData);
        if (RETCODE_OK == returnValue)
        {
            // clang-format off
            LOG_DEBUG("X: %ld;\tY: %ld;\tZ: %ld",
                    sensorAccelerometerData.x,
                    sensorAccelerometerData.y,
                    sensorAccelerometerData.z);
            // clang-format on
        }
        else
        {
            LOG_DEBUG("Accel sensor read failed!\r\n");
        }
    }

    if (RETCODE_OK == returnValue)
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_GREEN_ID, COMMONGATEWAY_LED_COMMAND_TOGGLE);
    }
    else
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_GREEN_ID, COMMONGATEWAY_LED_COMMAND_OFF);
        BSP_LED_Switch(COMMONGATEWAY_LED_RED_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }

    if (RETCODE_OK == returnValue)
    {
        BSP_Board_Delay(5);
        returnValue = CmdProcessor_Enqueue((CmdProcessor_T *)CmdProcessorHandle, appPeriodicTest, CmdProcessorHandle, UINT32_C(0));
    }
}

void I2CCallback(I2C_T i2c, struct MCU_I2C_Event_S event)
{
    (void)i2c;
    I2CTransceiver_LoopCallback(&i2cTransceiverStruct, event);
}
