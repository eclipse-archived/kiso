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
#include <stdio.h>
#include <inttypes.h>

#include "sensors_common.h"
#include "env_sensor.h"

#include "BSP_CommonGateway.h"
#include "BCDS_BSP_BME280.h"
#include "BCDS_BSP_Board.h"
#include "BCDS_I2CTransceiver.h"

#include "BCDS_CmdProcessor.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "bme280.h"
/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID CGW_APP_MODULE_ENV_SENSOR

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

int8_t bme280_Write(uint8_t address, uint8_t reg, uint8_t * data, uint16_t len);
int8_t bme280_Read(uint8_t address, uint8_t reg, uint8_t * data, uint16_t len);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

struct bme280_dev bme280Struct = {
	.read = bme280_Read,
	.write = bme280_Write,
	.delay_ms = BSP_Board_Delay,
	.intf = BME280_I2C_INTF,
	.dev_id = COMMONGATEWAY_BME280_I2CADDRESS,
};

static I2cTranceiverHandlePtr_T envSensorTransceiver = NULL;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/
Retcode_T Environmental_Init(I2cTranceiverHandlePtr_T i2cTransceiverRef)
{
    Retcode_T retcode;
    retcode = BSP_BME280_Connect(COMMONGATEWAY_BME280_ID);
    if (RETCODE_OK == retcode)
    {
        BSP_BME280_Enable(COMMONGATEWAY_BME280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        envSensorTransceiver = i2cTransceiverRef;
        bme280Struct.settings = (struct bme280_settings) {
        	.osr_p = BME280_OVERSAMPLING_4X,
        	.osr_h = BME280_OVERSAMPLING_2X,
			.standby_time = BME280_STANDBY_TIME_125_MS,
        };

        if (BME280_OK != bme280_init(&bme280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_APP_BME280_INIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (BME280_OK != bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_APP_BME280_INIT_FAILED);
        }
        BSP_Board_Delay(100);
    }
    return retcode;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
int8_t bme280_Write(uint8_t address, uint8_t reg, uint8_t * data, uint16_t len)
{
    Retcode_T retcode = I2CTransceiver_Write(envSensorTransceiver, address, reg, data, len);
    if (RETCODE_OK == retcode)
    {
        return BME280_OK;
    }
    else
    {
        return BME280_E_COMM_FAIL;
    }
}

int8_t bme280_Read(uint8_t address, uint8_t reg, uint8_t * data, uint16_t len)
{
    Retcode_T retcode = I2CTransceiver_Read(envSensorTransceiver, address, reg, data, len);
    if (RETCODE_OK == retcode)
    {
        return BME280_OK;
    }
    else
    {
        return BME280_E_COMM_FAIL;
    }
}
