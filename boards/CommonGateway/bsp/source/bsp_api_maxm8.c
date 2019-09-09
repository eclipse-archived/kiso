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

#include "Kiso_BSP_GNSS_MAXM8.h"

#if KISO_FEATURE_BSP_GNSS_MAXM8

#include "stm32/stm32l4/Kiso_MCU_STM32L4_UART_Handle.h"
#include "Kiso_HAL_Delay.h"
#include "BSP_CommonGateway.h"
#include "protected/power_supply.h"
#include "protected/gpio.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_API_MAXM8

#define MAXM8_DELAY_1_MS                     UINT32_C(1)
#define MAXM8_MAX_TIME_TO_OFF_MS             UINT32_C(1000)
#define MAXM8_MAX_TIME_TO_ACTIVE_MS          4000

#define MAXM8_UART_INT_PRIORITY              UINT32_C(10)
#define MAXM8_UART_SUBPRIORITY               UINT32_C(0)

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void UART4_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static uint8_t bspState = (uint8_t) BSP_STATE_INIT; /**< BSP State of the cellular module */

/**
 * Static structure storing the UART handle for GNSS MAXM8.
 */
static struct MCU_UART_S MaxM8_UARTStruct =
        {
                .TxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
                .RxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
                .Datarate = 9600,
                .huart.Instance = UART4,
                .huart.Init.BaudRate = 9600,
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
Retcode_T BSP_GNSS_MAXM8_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_InitTypeDef BSP_GPIOInitStruct = { 0 };

        GPIO_OpenClockGate(GPIO_PORT_B, PINB_POW_GPS_BAK | PINB_GPS_RESN);
        /* Configure PIN_GPS_RESN PIN_POW_GPS_BAK as output push pull */
        BSP_GPIOInitStruct.Pin = PINB_POW_GPS_BAK | PINB_GPS_RESN;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &BSP_GPIOInitStruct);

        GPIO_OpenClockGate(GPIO_PORT_A, PINA_GPS_MTX | PINA_GPS_MRX);
        /* Configure RX TX as alternate function push pull */
        BSP_GPIOInitStruct.Pin = PINA_GPS_MTX | PINA_GPS_MRX;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_AF_PP;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        BSP_GPIOInitStruct.Alternate = GPIO_AF8_UART4;
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
Retcode_T BSP_GNSS_MAXM8_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* Put RESETN and V_BCKP pin to high state before powering the GPS module */
        HAL_GPIO_WritePin(GPIOB, PINB_POW_GPS_BAK, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, PINB_GPS_RESN, GPIO_PIN_SET);
        /* supply VCC power to the device */
        retcode = PowerSupply_EnablePower2V8GPS();
    }
    if (RETCODE_OK == retcode)
    {
        /* Enable the UART clock */
        __HAL_RCC_UART4_CLK_ENABLE()
        ;
        __HAL_RCC_UART4_FORCE_RESET();
        __HAL_RCC_UART4_RELEASE_RESET();

        /* Configure the UART resource */
        if (HAL_OK != HAL_UART_Init(&MaxM8_UARTStruct.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_UART_INIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        HAL_NVIC_SetPriority(UART4_IRQn, MAXM8_UART_INT_PRIORITY, MAXM8_UART_SUBPRIORITY);
        HAL_NVIC_EnableIRQ(UART4_IRQn);

        bspState = (uint8_t) BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 */
Retcode_T BSP_GNSS_MAXM8_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* Disable interrupts and deactivate UART peripheral */
        HAL_NVIC_DisableIRQ(UART4_IRQn);
        if (HAL_OK != HAL_UART_DeInit(&MaxM8_UARTStruct.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_UART_DEINIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        __HAL_RCC_UART4_CLK_DISABLE();
        /* Power-off */
        retcode = PowerSupply_DisablePower2V8GPS();
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
Retcode_T BSP_GNSS_MAXM8_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;
    if (!(bspState & (uint8_t) BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        HAL_GPIO_DeInit(GPIOB, PINB_POW_GPS_BAK | PINB_GPS_RESN);
        GPIO_CloseClockGate(GPIO_PORT_B, PINB_POW_GPS_BAK | PINB_GPS_RESN);

        HAL_GPIO_DeInit(GPIOA, PINA_GPS_MTX | PINA_GPS_MRX);
        GPIO_CloseClockGate(GPIO_PORT_A, PINA_GPS_MTX | PINA_GPS_MRX);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t) BSP_STATE_DISCONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @return A pointer to the SARA R4N4 UART control structure
 */
HWHandle_T BSP_GNSS_MAXM8_GetUARTHandle(void)
{
    return (HWHandle_T) &MaxM8_UARTStruct;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_NOT_SUPPORTED Reset procedure using RESN signal has not been implemented because of high risks.
 */
Retcode_T BSP_GNSS_MAXM8_Reset(void)
{
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * See API interface for function documentation
 * @retval RETCODE_NOT_SUPPORTED Reset procedure using RESN signal has not been implemented because of high risks.
 */
Retcode_T BSP_GNSS_MAXM8_Control(uint32_t command, void* arg)
{
    KISO_UNUSED(command);
    KISO_UNUSED(arg);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt Service Routine handling UART4 IRQ. Forwards call to MCU Layer for handling.
 */
void UART4_IRQHandler(void)
{
    if (MaxM8_UARTStruct.IrqCallback)
    {
        MaxM8_UARTStruct.IrqCallback((UART_T) &MaxM8_UARTStruct);
    }
}
#endif /* KISO_FEATURE_BSP_GNSS_MAXM8 */
