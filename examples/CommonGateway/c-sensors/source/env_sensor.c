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
#include "Kiso_BSP_BME280.h"
#include "Kiso_BSP_Board.h"
#include "Kiso_I2CTransceiver.h"

#include "Kiso_CmdProcessor.h"
#include "Kiso_Assert.h"
#include "FreeRTOS.h"
#include "timers.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID CGW_APP_MODULE_ENV_SENSOR

// clang-format off
/* Status register (0xF3) */
#define BME280_REG_STATUS_ADDR          (UINT8_C(0xF3))
/* Mask to clear unused bits */
#define BME280_STATUS_MSK               (UINT8_C(0x09))
/* Status register bit 3 (conversion is running) */
#define BME280_STATUS_MEASURING         (UINT8_C(0x08))
/* Status register bit 0 (NVM data being copied to image registers) */
#define BME280_STATUS_IM_UPDATE         (UINT8_C(0x01))
// clang-format on

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

int8_t bme280_Write(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
int8_t bme280_Read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);

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
Retcode_T SensorEnvironment_Init(I2cTranceiverHandlePtr_T i2cTransceiverRef)
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

        if (BME280_OK != bme280_init(&bme280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_BME280_INIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        // clang-format off
        bme280Struct.settings = (struct bme280_settings)
        {
            .filter = BME280_FILTER_COEFF_OFF,
            .standby_time = BME280_STANDBY_TIME_0_5_MS,
            .osr_t = BME280_OVERSAMPLING_1X,
            .osr_p = BME280_OVERSAMPLING_1X,
            .osr_h = BME280_OVERSAMPLING_1X,
        };
        uint8_t desired_settings =  BME280_OSR_PRESS_SEL  |   // To set pressure oversampling.
                                    BME280_OSR_TEMP_SEL   |   // To set temperature oversampling.
                                    BME280_OSR_HUM_SEL    |   // To set humidity oversampling.
                                    BME280_FILTER_SEL     |   // To set filter setting.
                                    BME280_STANDBY_SEL;       // To set standby duration setting.
        // clang-format on
        if (BME280_OK != bme280_set_sensor_settings(desired_settings, &bme280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_BME280_INIT_FAILED);
        }
        BSP_Board_Delay(100);
    }
    if (RETCODE_OK == retcode)
    {
        if (BME280_OK != bme280_set_sensor_mode(BME280_SLEEP_MODE, &bme280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_BME280_INIT_FAILED);
        }
        BSP_Board_Delay(100);
    }

    return retcode;
}

Retcode_T SensorEnvironment_Read(struct bme280_data *data)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t reg_data = 0;

    if (BME280_OK != bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280Struct))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_BME280_INIT_FAILED);
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait until sensor exit sleep mode and start conversion cycle */
        do
        {
            if (BME280_OK == bme280_get_regs(BME280_REG_STATUS_ADDR, &reg_data, sizeof(reg_data), &bme280Struct))
            {
                reg_data &= BME280_STATUS_MSK;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        } while ((RETCODE_OK == retcode) && !(reg_data & BME280_STATUS_IM_UPDATE));
    }

    if (RETCODE_OK == retcode)
    {
        /* Wait for sensor to complete conversion cycle.
         * Otherwise we will get values from previous measurement.
         *  */
        do
        {
            if (BME280_OK == bme280_get_regs(BME280_REG_STATUS_ADDR, &reg_data, sizeof(reg_data), &bme280Struct))
            {
                reg_data &= BME280_STATUS_MSK;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        } while ((RETCODE_OK == retcode) && ((reg_data & BME280_STATUS_MEASURING) || (reg_data & BME280_STATUS_IM_UPDATE)));
    }

    if (RETCODE_OK == retcode)
    {
        /* bme280_get_sensor_data() is intended to return
         * compensated data using internal correction factors
         * */
        if (BME280_OK != bme280_get_sensor_data(BME280_ALL, data, &bme280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    return retcode;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
int8_t bme280_Write(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
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

int8_t bme280_Read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
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
