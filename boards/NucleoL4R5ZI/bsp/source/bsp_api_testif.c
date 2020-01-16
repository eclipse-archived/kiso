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

#include "Kiso_BSP_TestInterface.h"

#if KISO_FEATURE_BSP_TEST_INTERFACE

#include "stm32/stm32l4/Kiso_MCU_STM32L4_UART_Handle.h"
#include "Kiso_HAL_Delay.h"
#include "BSP_NucleoL4R5ZI.h"
#include "protected/gpio.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_API_TEST_IF

#define TESTIF_UART_INT_PRIORITY              UINT32_C(10)
#define TESTIF_UART_SUBPRIORITY               UINT32_C(1)

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

Retcode_T BSP_TestInterface_Control(uint32_t command, void* arg);
void USART3_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static uint8_t bspState = (uint8_t) BSP_STATE_INIT; /**< BSP State of the cellular module */

/**
 * Static structure storing the UART handle for Test Interface
 */
static struct MCU_UART_S testIf_UARTStruct =
{
	.TxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
	.RxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
	.Datarate = 115200U,
	.huart.Instance = USART3,
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
Retcode_T BSP_TestInterface_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* IOSV bit MUST be set to access GPIO port G[2:15] */
		__HAL_RCC_PWR_CLK_ENABLE();
		__HAL_RCC_LPTIM1_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct = { 0 };

		/* UART RX/TX GPIO pin configuration  */
        GPIO_OpenClockGate(GPIO_PORT_D, PIND_USART3_TX | PIND_USART3_RX);

		GPIO_InitStruct.Pin       = PIND_USART3_TX | PIND_USART3_RX;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        bspState = (uint8_t) BSP_STATE_CONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow enabling.
 */
Retcode_T BSP_TestInterface_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();
		__GPIOD_CLK_ENABLE();
        /* Configure the UART resource */
        if (HAL_OK != HAL_UART_Init(&testIf_UARTStruct.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_UART_INIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
    	NVIC_ClearPendingIRQ(USART3_IRQn);
		HAL_NVIC_SetPriority(USART3_IRQn, TESTIF_UART_INT_PRIORITY, TESTIF_UART_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(USART3_IRQn);

        bspState = (uint8_t) BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 */
Retcode_T BSP_TestInterface_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* Disable interrupts and deactivate UART peripheral */
        HAL_NVIC_DisableIRQ(USART3_IRQn);
        /* Clear the pending interrupt  */
        HAL_NVIC_ClearPendingIRQ(USART3_IRQn);

        if (HAL_OK != HAL_UART_DeInit(&testIf_UARTStruct.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_UART_DEINIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
    	__USART3_CLK_DISABLE();
        bspState = (uint8_t) BSP_STATE_DISABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disconnecting.
 */
Retcode_T BSP_TestInterface_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;
    if (!(bspState & (uint8_t) BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        HAL_GPIO_DeInit(GPIOD, PIND_USART3_TX | PIND_USART3_RX);
        GPIO_CloseClockGate(GPIO_PORT_D, PIND_USART3_TX | PIND_USART3_RX);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t) BSP_STATE_DISCONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @return A pointer to the UART control structure
 */
HWHandle_T BSP_TestInterface_GetUARTHandle(void)
{
    return (HWHandle_T) &testIf_UARTStruct;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_NOT_SUPPORTED.
 */
Retcode_T BSP_TestInterface_Control(uint32_t command, void* arg)
{
    KISO_UNUSED(command);
    KISO_UNUSED(arg);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt Service Routine handling USART3 IRQ. Forwards call to MCU Layer for handling.
 */
void USART3_IRQHandler(void)
{
    if (testIf_UARTStruct.IrqCallback)
    {
        testIf_UARTStruct.IrqCallback((UART_T) &testIf_UARTStruct);
    }
}
#endif /* KISO_FEATURE_BSP_TEST_INTERFACE */
