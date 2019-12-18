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

#include "Kiso_BSP_Cellular_SaraR4N4.h"

#if KISO_FEATURE_BSP_CELLULAR_SARAR4N4

#include "Kiso_HAL_Delay.h"
#include "stm32/stm32l4/Kiso_MCU_STM32L4_UART_Handle.h"

#include "BSP_CommonGateway.h"
#include "protected/power_supply.h"
#include "protected/gpio.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_API_SARAR4N4

#define SARAR4N4_DELAY_1_MS UINT32_C(1)
#define SARAR4N4_LVL_SHIFT_VCCA_SET_DELAY_MS SARAR4N4_DELAY_1_MS
#define SARAR4N4_LVL_SHIFT_OE_SET_DELAY_MS SARAR4N4_DELAY_1_MS
#define SARAR4N4_LVL_SHIFT_OE_RESET_DELAY_MS SARAR4N4_DELAY_1_MS
#define SARAR4N4_PWRON_VALID_TIME_MS UINT32_C(2000) //2 seconds
#define SARAR4N4_MAX_TIME_TO_OFF_MS UINT32_C(20000) //20 seconds
#define SARAR4N4_MAX_TIME_TO_ACTIVE_MS 4000

#define SARAR4N4_UART_INT_PRIORITY UINT32_C(5)
#define SARAR4N4_UART_SUBPRIORITY UINT32_C(0)

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void USART2_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/*
 * BSP State of the cellular module
 */
static uint8_t bspState = (uint8_t)BSP_STATE_INIT;

/*
 * Static structure which is used to keep the UART handle for CellularSaraR4N4.
 */
static struct MCU_UART_S SaraR4N4_UARTStruct =
    {
        .TxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
        .RxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
        .Datarate = 115200,
        .huart.Instance = USART2,
        .huart.Init.BaudRate = 115200,
        .huart.Init.WordLength = UART_WORDLENGTH_8B,
        .huart.Init.StopBits = UART_STOPBITS_1,
        .huart.Init.Parity = UART_PARITY_NONE,
        .huart.Init.Mode = UART_MODE_TX_RX,
        .huart.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS,
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
Retcode_T BSP_Cellular_SaraR4N4_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_InitTypeDef BSP_GPIOInitStruct = {0};

        GPIO_OpenClockGate(GPIO_PORT_B, PINB_GSM_PON);

        BSP_GPIOInitStruct.Pin = PINB_GSM_PON;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &BSP_GPIOInitStruct);

        GPIO_OpenClockGate(GPIO_PORT_E, PINE_EN_LVL | PINE_POW_LVL | PINE_GSM_RESN | PINE_GSM_GPIO1);

        BSP_GPIOInitStruct.Pin = PINE_EN_LVL | PINE_POW_LVL;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOE, &BSP_GPIOInitStruct);

        BSP_GPIOInitStruct.Pin = PINE_GSM_RESN;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOE, &BSP_GPIOInitStruct);

        BSP_GPIOInitStruct.Pin = PINE_GSM_GPIO1;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_INPUT;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOE, &BSP_GPIOInitStruct);

        /*Configure GPIOD pins : CTS, RTS and RX as open drain alternate function and */
        GPIO_OpenClockGate(GPIO_PORT_D, PIND_GSM_MCTS | PIND_GSM_MRTS | PIND_GSM_MTX | PIND_GSM_MRX);
        BSP_GPIOInitStruct.Pin = PIND_GSM_MCTS | PIND_GSM_MRTS | PIND_GSM_MTX | PIND_GSM_MRX;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_AF_PP;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        BSP_GPIOInitStruct.Alternate = GPIO_AF7_USART2;
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
Retcode_T BSP_Cellular_SaraR4N4_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* Put level shifter outputs into Hi-Z state before powering up the GSM module */
        HAL_GPIO_WritePin(GPIOE, PINE_EN_LVL, GPIO_PIN_SET);
        /* supply VCC power to the device */
        retcode = PowerSupply_EnablePower3V9GSM();
    }
    if (RETCODE_OK == retcode)
    {
        /* Supply VCCA reference to level shifter, Note that VCCB is supplied directly from V_INT output */
        HAL_GPIO_WritePin(GPIOE, PINE_POW_LVL, GPIO_PIN_SET);
        retcode = HAL_Delay_WaitMs(SARAR4N4_LVL_SHIFT_VCCA_SET_DELAY_MS);
    }
    if (RETCODE_OK == retcode)
    {
        /* Set PON and RESN pins to High before enabling the level shifter */
        HAL_GPIO_WritePin(GPIOB, PINB_GSM_PON, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, PINE_GSM_RESN, GPIO_PIN_SET);
        /* Enable level shifting */
        HAL_GPIO_WritePin(GPIOE, PINE_EN_LVL, GPIO_PIN_RESET);
        retcode = HAL_Delay_WaitMs(SARAR4N4_LVL_SHIFT_OE_RESET_DELAY_MS);
    }
    if (RETCODE_OK == retcode)
    {
        /* Send Power on signal*/
        HAL_GPIO_WritePin(GPIOB, PINB_GSM_PON, GPIO_PIN_RESET);
        retcode = HAL_Delay_WaitMs(SARAR4N4_PWRON_VALID_TIME_MS);
        HAL_GPIO_WritePin(GPIOB, PINB_GSM_PON, GPIO_PIN_SET);
    }
    if (RETCODE_OK == retcode)
    {
        /* wait for the module to transit to active mode */
        for (uint32_t i = 0; i >= SARAR4N4_MAX_TIME_TO_ACTIVE_MS; i++)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT);
            if (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOD, PIND_GSM_MRX))
            {
                retcode = RETCODE_OK;
                break;
            }
            HAL_Delay_WaitMs(SARAR4N4_DELAY_1_MS);
        }
    }
    if (RETCODE_OK == retcode)
    {
        /* enable the uart clock */
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();

        /* Configure the UART resource */
        if (HAL_OK != HAL_UART_Init(&SaraR4N4_UARTStruct.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        HAL_NVIC_SetPriority(USART2_IRQn, SARAR4N4_UART_INT_PRIORITY, SARAR4N4_UART_SUBPRIORITY);
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
Retcode_T BSP_Cellular_SaraR4N4_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* Send Power off signal*/
        HAL_GPIO_WritePin(GPIOB, PINB_GSM_PON, GPIO_PIN_RESET);
        retcode = HAL_Delay_WaitMs(SARAR4N4_PWRON_VALID_TIME_MS);
    }
    if (RETCODE_OK == retcode)
    {
        HAL_GPIO_WritePin(GPIOB, PINB_GSM_PON, GPIO_PIN_SET);
        retcode = HAL_Delay_WaitMs(SARAR4N4_MAX_TIME_TO_OFF_MS);
    }
    if (RETCODE_OK == retcode)
    {
        /* Disable interrupts and deactivate UART peripheral */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
        if (HAL_OK != HAL_UART_DeInit(&SaraR4N4_UARTStruct.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        __HAL_RCC_USART2_CLK_DISABLE();
        /* Disconnect GPIOs using level shifter */
        HAL_GPIO_WritePin(GPIOE, PINE_EN_LVL, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, PINE_POW_LVL, GPIO_PIN_RESET);
        /* Power OFF the module */
        retcode = PowerSupply_DisablePower3V9GSM();
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
Retcode_T BSP_Cellular_SaraR4N4_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;
    if (!(bspState & (uint8_t)BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* since 1V8 level is not present if the module is disabled, All GPIOs except the  GSM Power Supply could be
         * disconnected.
         */
        HAL_GPIO_DeInit(GPIOB, PINB_GSM_PON);
        GPIO_CloseClockGate(GPIO_PORT_B, PINB_GSM_PON);

        HAL_GPIO_DeInit(GPIOD, PIND_GSM_MCTS | PIND_GSM_MRTS | PIND_GSM_MTX | PIND_GSM_MRX);
        GPIO_CloseClockGate(GPIO_PORT_D, PIND_GSM_MCTS | PIND_GSM_MRTS | PIND_GSM_MTX | PIND_GSM_MRX);

        HAL_GPIO_DeInit(GPIOE, PINE_EN_LVL | PINE_POW_LVL | PINE_GSM_RESN | PINE_GSM_GPIO1);
        GPIO_CloseClockGate(GPIO_PORT_E, PINE_EN_LVL | PINE_POW_LVL | PINE_GSM_RESN | PINE_GSM_GPIO1);

        bspState = (uint8_t)BSP_STATE_DISCONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @return A pointer to the SARA R4N4 UART control structure
 */
HWHandle_T BSP_Cellular_SaraR4N4_GetUARTHandle(void)
{
    return (HWHandle_T)&SaraR4N4_UARTStruct;
}

/**
 * See API interface for function documentation
 * @retval NULL SPI is not used as communication interface.
 */
HWHandle_T BSP_Cellular_SaraR4N4_GetSPIHandle(void)
{
    return (HWHandle_T)NULL;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_NOT_SUPPORTED Reset procedure using RESN signal has not been implemented because of high risks.
 */
Retcode_T BSP_Cellular_SaraR4N4_Reset(void)
{
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T BSP_Cellular_SaraR4N4_Control(uint32_t command, void *arg)
{
    KISO_UNUSED(command);
    KISO_UNUSED(arg);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}
/**
 * See API interface for function documentation
 * @retval 0xFFFFFFFF CTS signal is contolled by uart hardware and therefore not readable
 */
uint32_t BSP_Cellular_SaraR4N4_GetCTS(void)
{
    /* invalid command CTS is controlled by UART hardware */
    return UINT32_C(0xFFFFFFFF);
}

/**
 * See API interface for function documentation
 * @retval RETCODE_NOT_SUPPORTED RTS signal is contolled by uart hardware and therefore not writable
 */
Retcode_T BSP_Cellular_SaraR4N4_SetRTSLow(void)
{
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * See API interface for function documentation
 * @retval RETCODE_NOT_SUPPORTED RTS signal is contolled by uart hardware and therefore not writable
 */
Retcode_T BSP_Cellular_SaraR4N4_SetRTSHigh(void)
{
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt Service Routine handling USART2 IRQ. Forwards call to MCU Layer for handling.
 */
void USART2_IRQHandler(void)
{
    if (SaraR4N4_UARTStruct.IrqCallback)
    {
        SaraR4N4_UARTStruct.IrqCallback((UART_T)&SaraR4N4_UARTStruct);
    }
}

#endif /* KISO_FEATURE_BSP_CELLULAR_SARAR4N4 */
