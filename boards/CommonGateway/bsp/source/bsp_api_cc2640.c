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

#include "BSP_BLE_CC2640.h"

#if KISO_FEATURE_BSP_BLE_CC2640

#include "BSP_CommonGateway.h"
#include "protected/gpio.h"
#include "protected/power_supply.h"
#include "stm32/stm32l4/Kiso_MCU_STM32L4_UART_Handle.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_API_BLE_CC2640

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/
/**
 * @brief   This function is to map the hardware specific UART interrupt
 *          to the interrupt callback of the UART port used in BLE cc2640
 */
void USART3_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/**
 * BSP State of the BLE module
 */
static uint8_t bspState = (uint8_t)BSP_STATE_INIT;

/**
 * Static structure which is used to keep the UART handle for Ble_CC2640
 */
static struct MCU_UART_S CC2640_UartHandle =
    {
        .TxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
        .RxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
        .Datarate = 115200,
        .huart.Instance = USART3,
        .huart.Init.BaudRate = 115200,
        .huart.Init.WordLength = UART_WORDLENGTH_8B,
        .huart.Init.StopBits = UART_STOPBITS_1,
        .huart.Init.Parity = UART_PARITY_NONE,
        .huart.Init.Mode = UART_MODE_TX_RX,
        .huart.Init.HwFlowCtl = UART_HWCONTROL_NONE,
        .huart.Init.OverSampling = UART_OVERSAMPLING_16,
        .huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE,
        .huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT,
};

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow connecting.
 */
Retcode_T BSP_BLE_CC2640_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_InitTypeDef BSP_GPIOInitStruct = {0};

        GPIO_OpenClockGate(GPIO_PORT_B, PINB_BLE_RESN);
        /* Configure PINB_BLE_RESN as output push pull */
        BSP_GPIOInitStruct.Pin = PINB_BLE_RESN;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &BSP_GPIOInitStruct);

        GPIO_OpenClockGate(GPIO_PORT_D, PIND_BLE_MTX | PIND_BLE_MRX);
        /* Configure RX TX as alternate function push pull */
        BSP_GPIOInitStruct.Pin = PIND_BLE_MTX | PIND_BLE_MRX;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_AF_PP;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        BSP_GPIOInitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &BSP_GPIOInitStruct);

        bspState = (uint8_t)BSP_STATE_CONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow enabling.
 */
Retcode_T BSP_BLE_CC2640_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* supply VCC power to the device */
        retcode = PowerSupply_EnablePower2V8Memory();
    }
    if (RETCODE_OK == retcode)
    {
        /* Enable the UART clock */
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
        /* Configure the UART resource */
        if (HAL_OK != HAL_UART_Init(&CC2640_UartHandle.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        NVIC_ClearPendingIRQ(UART4_IRQn);
        HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(UART4_IRQn);

        bspState = (uint8_t)BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 */
Retcode_T BSP_BLE_CC2640_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* Disable interrupts and deactivate UART peripheral */
        NVIC_DisableIRQ(UART4_IRQn);
        if (HAL_OK != HAL_UART_DeInit(&CC2640_UartHandle.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        __HAL_RCC_USART3_CLK_DISABLE();
        /* Power-off */
        retcode = PowerSupply_DisablePower2V8BLE();
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
Retcode_T BSP_BLE_CC2640_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;
    if (!(bspState & (uint8_t)BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        HAL_GPIO_DeInit(GPIOB, PINB_GPS_RESN);
        GPIO_CloseClockGate(GPIO_PORT_B, PINB_GPS_RESN);

        HAL_GPIO_DeInit(GPIOD, PIND_BLE_MTX | PIND_BLE_MRX);
        GPIO_CloseClockGate(GPIO_PORT_D, PIND_BLE_MTX | PIND_BLE_MRX);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t)BSP_STATE_DISCONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @return A pointer to the UART control structure
 */
HWHandle_T BSP_BLE_CC2640_GetUARTHandle(void)
{
    /* Giving out the corresponding handle */
    return (HWHandle_T)&CC2640_UartHandle;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt service routine for the UART used by the bluetooth interface.
 */
void USART3_IRQHandler(void)
{
    if (NULL != CC2640_UartHandle.IrqCallback)
    {
        CC2640_UartHandle.IrqCallback((UART_T)&CC2640_UartHandle.huart);
    }
}

#endif /* if (KISO_FEATURE_BSP_BLE_CC2640) */
