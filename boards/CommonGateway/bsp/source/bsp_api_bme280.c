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

#include "Kiso_BSP_BME280.h"

#if KISO_FEATURE_BSP_BME280

#include "Kiso_HAL_Delay.h"
#include "BSP_CommonGateway.h"
#include "protected/power_supply.h"
#include "protected/gpio.h"
#include "protected/i2c.h"
#include "stm32/stm32l4/Kiso_MCU_STM32L4_I2C_Handle.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_API_BME280

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/
/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static uint8_t bspState = (uint8_t)BSP_STATE_INIT; /**< BSP State of the sensor */

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow connecting.
 */
Retcode_T BSP_BME280_Connect(int32_t deviceId)
{
    KISO_UNUSED(deviceId);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = I2C_Connect(I2C_DEVICE_BME280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t)BSP_STATE_CONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow enabling.
 */
Retcode_T BSP_BME280_Enable(int32_t deviceId)
{
    KISO_UNUSED(deviceId);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_EnablePower2V8Sensor(POWER_SUPPLY_SENSOR_BME280);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = I2C_Enable(I2C_DEVICE_BME280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t)BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 */
Retcode_T BSP_BME280_Disable(int32_t deviceId)
{
    KISO_UNUSED(deviceId);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = I2C_Disable(I2C_DEVICE_BME280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_DisablePower2V8Sensor(POWER_SUPPLY_SENSOR_BME280);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t)BSP_STATE_DISABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disconnecting.
 */
Retcode_T BSP_BME280_Disconnect(int32_t deviceId)
{
    KISO_UNUSED(deviceId);
    Retcode_T retcode = RETCODE_OK;
    if (!(bspState & (uint8_t)BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = I2C_Disconnect(I2C_DEVICE_BME280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t)BSP_STATE_DISCONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @return A pointer to the I2C control structure
 */
HWHandle_T BSP_BME280_GetHandle(int32_t deviceId)
{
    KISO_UNUSED(deviceId);
    return (HWHandle_T)&sensorsI2CStruct;
}

/**
 * See API interface for function documentation
 * @retval RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED) function not implemented by BSP.
 */
Retcode_T BSP_BME280_Control(uint32_t command, void *arg)
{
    KISO_UNUSED(command);
    KISO_UNUSED(arg);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

#endif /* KISO_FEATURE_BSP_BME280 */
