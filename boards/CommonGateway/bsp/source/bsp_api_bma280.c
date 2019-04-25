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

#include "BCDS_BSP_BMA280.h"

#if BCDS_FEATURE_BSP_BMA280

#include "BCDS_HAL_Delay.h"
#include "BSP_CommonGateway.h"
#include "protected/power_supply.h"
#include "protected/gpio.h"
#include "protected/i2c.h"
#include "stm32/stm32l4/BCDS_MCU_STM32L4_I2C_Handle.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID MODULE_BSP_API_BMA280

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/
/*---------------------- VARIABLES DECLARATIONS ---------------------------------------------------------------------*/

static uint8_t bspState = (uint8_t) BSP_STATE_INIT; /**< BSP State of the sensor */

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow connecting.
 */
Retcode_T BSP_BMA280_Connect(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = I2C_Connect(I2C_DEVICE_BMA280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_InitTypeDef BSP_GPIOInitStruct = { 0 };

        GPIO_OpenClockGate(GPIO_PORT_A, PINA_BMA_INT);
        BSP_GPIOInitStruct.Pin = PINA_BMA_INT;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_INPUT;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &BSP_GPIOInitStruct);

        bspState = (uint8_t) BSP_STATE_CONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow enabling.
 */
Retcode_T BSP_BMA280_Enable(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_EnablePower2V8Sensor(POWER_SUPPLY_SENSOR_BMA280);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = I2C_Enable(I2C_DEVICE_BMA280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t) BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 */
Retcode_T BSP_BMA280_Disable(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = I2C_Disable(I2C_DEVICE_BMA280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_DisablePower2V8Sensor(POWER_SUPPLY_SENSOR_BMA280);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t) BSP_STATE_DISABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disconnecting.
 */
Retcode_T BSP_BMA280_Disconnect(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    Retcode_T retcode = RETCODE_OK;
    if (!(bspState & (uint8_t) BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = I2C_Disconnect(I2C_DEVICE_BMA280_ID);
    }
    if (RETCODE_OK == retcode)
    {
        HAL_GPIO_DeInit(GPIOA, PINA_BMA_INT);
        GPIO_CloseClockGate(GPIO_PORT_A, PINA_BMA_INT);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t) BSP_STATE_DISCONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @return A pointer to the I2C control structure
 */
HWHandle_T BSP_BMA280_GetHandle(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    return (HWHandle_T) &sensorsI2CStruct;
}

/**
 * See API interface for function documentation
 * @retval RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED) function not implemented by BSP.
 */
Retcode_T BSP_BMA280_Control(uint32_t command, void* arg)
{
    BCDS_UNUSED(command);
    BCDS_UNUSED(arg);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * See API interface for function documentation
 * @retval RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED) function not implemented by BSP.
 */
Retcode_T BSP_BMA280_Int1Enable(int32_t deviceId, BSP_BMA280_InterruptCallback_T callback)
{
    BCDS_UNUSED(deviceId);
    BCDS_UNUSED(callback);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * See API interface for function documentation
 * @retval RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED) function not implemented by BSP.
 */
Retcode_T BSP_BMA280_Int1Disable(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * See API interface for function documentation
 * @retval RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED) function not implemented by BSP.
 */
Retcode_T BSP_BMA280_Int2Enable(int32_t deviceId, BSP_BMA280_InterruptCallback_T callback)
{
    BCDS_UNUSED(deviceId);
    BCDS_UNUSED(callback);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * See API interface for function documentation
 * @retval RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED) function not implemented by BSP.
 */
Retcode_T BSP_BMA280_Int2Disable(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * See API interface for function documentation
 * @retval RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED) function not implemented by BSP.
 */
Retcode_T BSP_BMA280_SetCSHigh(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * See API interface for function documentation
 * @retval RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED) function not implemented by BSP.
 */
Retcode_T BSP_BMA280_SetCSLow(int32_t deviceId)
{
    BCDS_UNUSED(deviceId);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

#endif /* BCDS_FEATURE_BSP_BMA280 */
