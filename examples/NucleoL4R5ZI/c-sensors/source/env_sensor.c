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
 * @ingroup
 *
 * @defgroup
 * @{
 *
 * @brief Environmental Sensor BME280 Example
 *
 * @details Demo application demonstrates how to use BME280 sensor from KISO.
 *
 **/

/* own header files */
#include "AppInfo.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID APP_MODULE_ID_ENV_SENSOR

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "BSP_NucleoL4R5ZI.h"
#include "Kiso_BSP_LED.h"
#include "Kiso_BSP_BOARD.h"
#include "env_sensor.h"
#include "Kiso_CmdProcessor.h"
#include "Kiso_I2CTransceiver.h"
#include "Kiso_Assert.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "Kiso_Logging.h"
#include "bme280.h"
#include "env_sensor.h"
#include "Kiso_BSP_BME280.h"
#include "Kiso_MCU_I2C.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

int8_t bme280_Write(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
int8_t bme280_Read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
void I2CCallback(I2C_T i2c, struct MCU_I2C_Event_S event);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

struct bme280_dev bme280Struct = {
    .read = bme280_Read,
    .write = bme280_Write,
	.delay_ms = BSP_Board_Delay,
    .intf = BME280_I2C_INTF,
    .dev_id = NUCLEOL4R5ZI_BME280_I2CADDRESS,
};

static I2cTranceiverHandle_T i2cTransceiverStruct;
static I2C_T i2cHandle;

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
void I2CCallback(I2C_T i2c, struct MCU_I2C_Event_S event)
{
    I2CTransceiver_LoopCallback(i2c, event);
}

int8_t bme280_Write(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
    Retcode_T retcode = I2CTransceiver_Write(&i2cTransceiverStruct, address, reg, data, len);
    if (RETCODE_OK == retcode)
    {
        return BME280_OK;
    }
    else
    {
        return BME280_E_COMM_FAIL;
    }
}

int8_t bme280_Read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
    Retcode_T retcode = I2CTransceiver_Read(&i2cTransceiverStruct, address, reg, data, len);
    if (RETCODE_OK == retcode)
    {
        return BME280_OK;
    }
    else
    {
        return BME280_E_COMM_FAIL;
    }
}

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

Retcode_T EnvSensor_init(void)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = BSP_BME280_Connect(I2C_DEVICE_BME280_ID);
    if (RETCODE_OK == retcode)
    {
        BSP_BME280_Enable(I2C_DEVICE_BME280_ID);
    }
    if (RETCODE_OK == retcode)
	{
    	i2cHandle = BSP_BME280_GetHandle(I2C_DEVICE_BME280_ID);

		if (NULL == i2cHandle)
		{
			retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_I2C_HANDLE_DOES_NOT_EXIST);
		}
	}
    if (RETCODE_OK == retcode)
    {
        if (RETCODE_OK == retcode)
    	{
    		retcode = MCU_I2C_Initialize(i2cHandle, I2CCallback);
    	}
        if (RETCODE_OK == retcode)
        {
        	retcode = I2CTransceiver_Init(&i2cTransceiverStruct, i2cHandle);
        }
	}
    if (RETCODE_OK == retcode)
    {
        bme280Struct.settings = (struct bme280_settings){
            .osr_p = BME280_OVERSAMPLING_4X,
            .osr_h = BME280_OVERSAMPLING_2X,
            .standby_time = BME280_STANDBY_TIME_125_MS,
        };

        if (BME280_OK != bme280_init(&bme280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_BME280_INIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (BME280_OK != bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_BME280_INIT_FAILED);
        }
        BSP_Board_Delay(100);
    }

    return retcode;
}

/**@} */

/** ************************************************************************* */

