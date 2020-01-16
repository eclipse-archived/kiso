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
 * @brief Accelerometer Sensor BMA280 Example
 *
 * @details Demo application demonstrates how to use BMA280 sensor from KISO.
 *
 **/

/* own header files */
#include "AppInfo.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID APP_MODULE_ID_ACCEL_SENSOR

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "BSP_NucleoL4R5ZI.h"
#include "Kiso_BSP_LED.h"
#include "Kiso_CmdProcessor.h"
#include "Kiso_Assert.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "Kiso_Logging.h"
#include "bma2x2.h"
#include "accel_sensor.h"
#include "Kiso_BSP_BMA280.h"
#include "Kiso_MCU_SPI.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

static s8 bma280_Write(u8 addr, u8 reg, u8 *data, u8 len);
static s8 bma280_Read(u8 addr, u8 reg, u8 *data, u8 len);

static void spicallback(SPI_T spi, struct MCU_SPI_Event_S event);

/*---------------------- VARIABLES DECLARATIONS ---------------------------------------------------------------------*/

struct bma2x2_t bma280Struct = {
    .bus_read = bma280_Read,
    .bus_write = bma280_Write,
};

HWHandle_T spihandle;

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

static s8 bma280_Write( u8 addr, u8 reg, u8 *data, u8 len)
{
	KISO_UNUSED(addr);
	KISO_UNUSED(reg);

    Retcode_T retcode = MCU_SPI_Send(spihandle, data, len);
    if (RETCODE_OK == retcode)
    {
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

static s8 bma280_Read( u8 addr, u8 reg, u8 *data, u8 len)
{
	KISO_UNUSED(addr);
	KISO_UNUSED(reg);

	Retcode_T retcode = MCU_SPI_Receive(spihandle, data, len);
    if (RETCODE_OK == retcode)
    {
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

static void spicallback(SPI_T spi, struct MCU_SPI_Event_S event)
{
	KISO_UNUSED(spi);
	KISO_UNUSED(event);
}

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

Retcode_T AccelSensor_init(void)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = BSP_BMA280_Connect(SPI_DEVICE_BMA280_ID);
    if(RETCODE_OK == retcode)
    {
    	retcode = BSP_BMA280_Enable(SPI_DEVICE_BMA280_ID);
    }
    if(RETCODE_OK == retcode)
	{
    	spihandle = BSP_BMA280_GetHandle(SPI_DEVICE_BMA280_ID);
        retcode = MCU_SPI_Initialize(spihandle, spicallback);
	}
    if(RETCODE_OK == retcode)
   	{
		if (SUCCESS != bma2x2_init(&bma280Struct))
		{
			retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_APP_BMA280_INIT_FAILED);
		}
   	}

    return retcode;
}

/**@} */

/** ************************************************************************* */

