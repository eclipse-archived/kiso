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
#define KISO_MODULE_ID KISO_APP_MODULE_ACCEL_SENSOR

#include "accel_sensor.h"
#include "BSP_CommonGateway.h"
#include "Kiso_Retcode.h"
#include "Kiso_BSP_BMA280.h"
#include "Kiso_BSP_Board.h"
#include "Kiso_I2CTransceiver.h"
#include "FreeRTOS.h"
#include "timers.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/
static void Delay(BMA2x2_MDELAY_DATA_TYPE delay);
s8 bma280_Write(u8 address, u8 reg, u8 *data, u8 len);
s8 bma280_Read(u8 address, u8 reg, u8 *data, u8 len);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

struct bma2x2_t bma280Struct = {
    .bus_read = bma280_Read,
    .bus_write = bma280_Write,
    .delay_msec = Delay,
    .dev_addr = COMMONGATEWAY_BMA280_I2CADDRESS,
};

static I2cTranceiverHandlePtr_T accelSensorTransceiver = NULL;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/
Retcode_T SensorAccelerometer_Init(I2cTranceiverHandlePtr_T i2cTransceiverRef)
{
    Retcode_T retcode;
    retcode = BSP_BMA280_Connect(COMMONGATEWAY_BMA280_ID);
    if (RETCODE_OK == retcode)
    {
        BSP_BMA280_Enable(COMMONGATEWAY_BMA280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        accelSensorTransceiver = i2cTransceiverRef;
        if (SUCCESS != bma2x2_init(&bma280Struct))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (SUCCESS != bma2x2_set_bw(BMA2x2_BW_500HZ))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
        BSP_Board_Delay(100);
    }

    return retcode;
}

Retcode_T SensorAccelerometer_Read(struct bma2x2_accel_data *data)
{
    if (0 != bma2x2_read_accel_xyz(data))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    return RETCODE_OK;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
static void Delay(BMA2x2_MDELAY_DATA_TYPE delay)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING)
    {
        BSP_Board_Delay(delay);
    }
    else
    {
        vTaskDelay(delay);
    }
}

s8 bma280_Write(u8 address, u8 reg, u8 *data, u8 len)
{
    Retcode_T retcode = I2CTransceiver_Write(accelSensorTransceiver, address, reg, data, len);
    if (RETCODE_OK == retcode)
    {
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

s8 bma280_Read(u8 address, u8 reg, u8 *data, u8 len)
{
    Retcode_T retcode = I2CTransceiver_Read(accelSensorTransceiver, address, reg, data, len);
    if (RETCODE_OK == retcode)
    {
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}
