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

#include "Kiso_HAL.h"

#if KISO_FEATURE_I2C

#include "stm32/stm32l4/Kiso_MCU_STM32L4_I2C_Handle.h"
#include "BSP_NucleoL4R5ZI.h"
#include "protected/i2c.h"
#include "protected/gpio.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_I2C

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void I2C1_EV_IRQHandler(void);

void I2C1_ER_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/**
 * Variable storing I2C control block for the sensors.
 */
struct MCU_I2C_S sensorsI2CStruct =
    {
        .TransferMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
        .hi2c.Instance = I2C1,
        .hi2c.Init.Timing = 0x10b0153d, // 400kHz; 200ns rise time 50ns fall time
        .hi2c.Init.OwnAddress1 = 0,
        .hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
        .hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED,
        .hi2c.Init.OwnAddress2 = 0,
        .hi2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK,
        .hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED,
        .hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED,
};

uint8_t I2C1ConnectedState = 0;
uint8_t I2C1EnabledState = 0;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 * @retval RETCODE_OK
 */
Retcode_T I2C_Connect(enum BSP_I2C_Devices id)
{
    if (0 == I2C1ConnectedState)
    {
        GPIO_InitTypeDef BSP_GPIOInitStruct = {0};

        GPIO_OpenClockGate(GPIO_PORT_B, PINB_SENS_SCL | PINB_SENS_SDA);
        BSP_GPIOInitStruct.Pin = PINB_SENS_SCL | PINB_SENS_SDA;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_AF_PP; //GPIO_MODE_AF_OD;
        BSP_GPIOInitStruct.Pull = GPIO_PULLUP;     //GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        BSP_GPIOInitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &BSP_GPIOInitStruct);
    }
    I2C1ConnectedState |= 1 << id;
    return RETCODE_OK;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success
 * @retval RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_I2C_INIT_FAILED) in case I2C initialize failed.
 */
Retcode_T I2C_Enable(enum BSP_I2C_Devices id)
{
    Retcode_T retcode = RETCODE_OK;
    if (0 == I2C1EnabledState)
    {
        /* Enable the UART clock */
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_I2C1_FORCE_RESET();
        __HAL_RCC_I2C1_RELEASE_RESET();

        /* Configure the UART resource */
        if (HAL_OK != HAL_I2C_Init(&sensorsI2CStruct.hi2c))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_I2C_INIT_FAILED);
        }
        if (HAL_OK != HAL_I2CEx_ConfigAnalogFilter(&sensorsI2CStruct.hi2c, I2C_ANALOGFILTER_ENABLE))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_I2C_INIT_FAILED);
        }
        if (HAL_OK != HAL_I2CEx_ConfigDigitalFilter(&sensorsI2CStruct.hi2c, 0))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_I2C_INIT_FAILED);
        }
        if (RETCODE_OK == retcode)
        {
            HAL_NVIC_SetPriority(I2C1_EV_IRQn, 6, 0);
            HAL_NVIC_SetPriority(I2C1_ER_IRQn, 6, 0);
            HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
            HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
        }
    }
    I2C1EnabledState |= 1 << id;
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success
 * @retval RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_I2C_DEINIT_FAILED) in case I2C deinitialize failed.
 */
Retcode_T I2C_Disable(enum BSP_I2C_Devices id)
{
    Retcode_T retcode = RETCODE_OK;
    I2C1EnabledState &= ~(1 << id);
    if (0 == I2C1EnabledState)
    {
        if (HAL_OK != HAL_I2C_DeInit(&sensorsI2CStruct.hi2c))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_I2C_DEINIT_FAILED);
        }
        if (RETCODE_OK == retcode)
        {
            HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
            __HAL_RCC_I2C1_CLK_DISABLE();
        }
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success
 */
Retcode_T I2C_Disconnect(enum BSP_I2C_Devices id)
{
    I2C1ConnectedState &= ~(1 << id);
    if (0 == I2C1ConnectedState)
    {
        HAL_GPIO_DeInit(GPIOB, PINB_SENS_SCL | PINB_SENS_SDA);
        GPIO_CloseClockGate(GPIO_PORT_B, PINB_SENS_SCL | PINB_SENS_SDA);
    }
    return RETCODE_OK;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt Service Routine handling I2C1_EV IRQ. Forwards call to MCU Layer for handling.
 */
void I2C1_EV_IRQHandler(void)
{
    if (NULL != sensorsI2CStruct.IRQCallback)
    {
        sensorsI2CStruct.IRQCallback((I2C_T)&sensorsI2CStruct);
    }
}

/**
 * Interrupt Service Routine handling I2C1_ER IRQ. Forwards call to MCU Layer for handling.
 */
void I2C1_ER_IRQHandler(void)
{
    {
        if (NULL != sensorsI2CStruct.ERRCallback)
        {
            sensorsI2CStruct.ERRCallback((I2C_T)&sensorsI2CStruct);
        }
    }
}

#endif
