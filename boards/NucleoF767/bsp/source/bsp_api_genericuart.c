/********************************************************************************
* Copyright (c) 2010-2020 Robert Bosch GmbH
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

#include "Kiso_BSP_GenericUart.h"

#if KISO_FEATURE_BSP_GENERIC_UART

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "stm32/stm32f7/Kiso_MCU_STM32F7_UART_Handle.h"
#include "Kiso_HAL_Delay.h"
#include "BSP_NucleoF767.h"
#include "protected/gpio.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_API_GENERICUART

#define UART_INT_PRIORITY UINT32_C(10)
#define UART_SUBPRIORITY UINT32_C(1)

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void USART2_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static uint8_t bspState = (uint8_t)BSP_STATE_INIT; /**< BSP State of the cellular module */

/**
 * Static structure storing the UART handle for Test Interface
 */
static struct MCU_UART_S uartCtrlStruct =
    {
        .TxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
        .RxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
        .Datarate = 115200U,
        .huart.Instance = USART2,
        .huart.Init.BaudRate = 115200U,
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
Retcode_T BSP_GenericUart_Connect(uint32_t id)
{
    KISO_UNUSED(id);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* IOSV bit MUST be set to access GPIO port G[2:15] */
        __HAL_RCC_PWR_CLK_ENABLE();
        __HAL_RCC_LPTIM1_CLK_ENABLE();

        GPIO_InitTypeDef GPIO_InitStruct = {0};

        /* UART RX/TX GPIO pin configuration  */
        GPIO_OpenClockGate(GPIO_PORT_D, PIND_USART2_TX | PIND_USART2_RX);

        GPIO_InitStruct.Pin = PIND_USART2_TX | PIND_USART2_RX;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        bspState = (uint8_t)BSP_STATE_CONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow enabling.
 */
Retcode_T BSP_GenericUart_Enable(uint32_t id)
{
    KISO_UNUSED(id);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
        __GPIOD_CLK_ENABLE();
        /* Configure the UART resource */
        if (HAL_OK != HAL_UART_Init(&uartCtrlStruct.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_UART_INIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        NVIC_ClearPendingIRQ(USART2_IRQn);
        HAL_NVIC_SetPriority(USART2_IRQn, UART_INT_PRIORITY, UART_SUBPRIORITY);
        HAL_NVIC_EnableIRQ(USART2_IRQn);

        bspState = (uint8_t)BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 */
Retcode_T BSP_GenericUart_Disable(uint32_t id)
{
    KISO_UNUSED(id);
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* Disable interrupts and deactivate UART peripheral */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
        /* Clear the pending interrupt  */
        HAL_NVIC_ClearPendingIRQ(USART2_IRQn);

        if (HAL_OK != HAL_UART_DeInit(&uartCtrlStruct.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_UART_DEINIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        __USART2_CLK_DISABLE();
        bspState = (uint8_t)BSP_STATE_DISABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disconnecting.
 */
Retcode_T BSP_GenericUart_Disconnect(uint32_t id)
{
    KISO_UNUSED(id);
    Retcode_T retcode = RETCODE_OK;
    if (!(bspState & (uint8_t)BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        HAL_GPIO_DeInit(GPIOD, PIND_USART2_TX | PIND_USART2_RX);
        GPIO_CloseClockGate(GPIO_PORT_D, PIND_USART2_TX | PIND_USART2_RX);
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
HWHandle_T BSP_GenericUart_GetHandle(uint32_t id)
{
    KISO_UNUSED(id);
    return (HWHandle_T)&uartCtrlStruct;
}

/**
 * This function is not in use.
 */
Retcode_T BSP_GenericUart_UserControl(uint32_t control, void *param)
{
    KISO_UNUSED(control);
    KISO_UNUSED(param);

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt Service Routine handling USART2 IRQ. Forwards call to MCU Layer for handling.
 */
void USART2_IRQHandler(void)
{
    if (uartCtrlStruct.IrqCallback)
    {
        uartCtrlStruct.IrqCallback((UART_T)&uartCtrlStruct);
    }
}
#endif /* KISO_FEATURE_BSP_TEST_INTERFACE */
