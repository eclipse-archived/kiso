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
 * @ingroup BSP_GSM_MODULE
 * @{
 * @file
 * @brief  Implementation of GSM BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_GSM_SIM800H

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_GSM_SIM800H.h"

#if BCDS_FEATURE_BSP_GSM_SIM800H

#include "GSM_sim800H.h"
#include "BoardSettings.h"
#include "BSP_BoardType.h"
#include "BCDS_MCU_UART_Handle.h"
#include "BSP_ModuleState.h"
#include "AnalogSensor.h"


static struct MCU_UART_S BSP_UART2Handle;
static volatile enum BSP_ModuleState_E ModuleState_GSM_SIM800H = BSP_MODULE_STATE_UNINITIALIZED;
static volatile uint8_t EnableCount;


/*
 * @brief Initialize the USART2 resource
 * @details This function Initializes the USART2 resource in UART mode. It is called
 * within the Board_Initialize() function. It stores the desired configuration
 * into the resource registers and puts it into low power
 * consumption configuration until it is requested by the application.
 */
Retcode_T Board_USART2_UART_Init(void);

enum BSP_ModuleState_E GSM_SIM800H_getState(void)
{
    return ModuleState_GSM_SIM800H;
}

Retcode_T Board_USART2_UART_Init(void)
{
    Retcode_T retcode = RETCODE_OK;
    /* Enable the clock */
    __HAL_RCC_USART2_CLK_ENABLE();

    /* Configure the UART */
    BSP_UART2Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    BSP_UART2Handle.huart.Instance = GSM_SERIAL_PORT;
    BSP_UART2Handle.huart.Init.BaudRate = GSM_BAUD_RATE;
    BSP_UART2Handle.huart.Init.WordLength = GSM_DATA_BITS;
    BSP_UART2Handle.huart.Init.StopBits = GSM_STOP_BITS;
    BSP_UART2Handle.huart.Init.Parity = GSM_PARITY;
    BSP_UART2Handle.huart.Init.Mode = GSM_MODE;
    BSP_UART2Handle.huart.Init.HwFlowCtl = GSM_FLOW_CONTROL;
    BSP_UART2Handle.huart.Init.OverSampling = GSM_SAMPLING;
    BSP_UART2Handle.huart.Init.OneBitSampling = GSM_ONE_BIT_SAMPLING;
    BSP_UART2Handle.huart.AdvancedInit.AdvFeatureInit = GSM_ADVANCED_INT;
    if (HAL_OK != HAL_UART_Init(&BSP_UART2Handle.huart))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    /* Disable the UART and its clock */
    __HAL_UART_DISABLE(&BSP_UART2Handle.huart);
    __HAL_RCC_USART2_CLK_DISABLE();

    if(RETCODE_OK == retcode)
    {
        ModuleState_GSM_SIM800H = BSP_MODULE_STATE_DISCONNECTED;
    }
    return retcode;
}

Retcode_T BSP_GSM_SIM800H_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    GPIO_InitTypeDef GPIO_InitStruct;

    switch(ModuleState_GSM_SIM800H)
    {
    case BSP_MODULE_STATE_DISCONNECTED:
    case BSP_MODULE_STATE_ERROR: /*In case of error state we try to connect again to go back in a defined state, will always succeed*/

        /*
         * Enabling GPIO clocks required by the GSM module
         */
        GSM_GPIO_CLOCK_ENABLE();
        /*
         * USART2 connect
         */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_UART_TXD_PIN;
        GPIO_InitStruct.Mode = GSM_UART_TXD_MODE;
        GPIO_InitStruct.Pull = GSM_UART_TXD_PULL;
        GPIO_InitStruct.Speed = GSM_UART_TXD_SPEED;
        GPIO_InitStruct.Alternate = GSM_UART_TXD_ALTERNATE;
        HAL_GPIO_Init(GSM_UART_TXD_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_UART_RXD_PIN;
        GPIO_InitStruct.Mode = GSM_UART_RXD_MODE;
        GPIO_InitStruct.Pull = GSM_UART_RXD_PULL;
        GPIO_InitStruct.Speed = GSM_UART_RXD_SPEED;
        GPIO_InitStruct.Alternate = GSM_UART_RXD_ALTERNATE;
        HAL_GPIO_Init(GSM_UART_RXD_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_UART_RTS_PIN;
        GPIO_InitStruct.Mode = GSM_UART_RTS_MODE;
        GPIO_InitStruct.Pull = GSM_UART_RTS_PULL;
        GPIO_InitStruct.Speed = GSM_UART_RTS_SPEED;
        GPIO_InitStruct.Alternate = GSM_UART_RTS_ALTERNATE;
        HAL_GPIO_Init(GSM_UART_RTS_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_UART_CTS_PIN;
        GPIO_InitStruct.Mode = GSM_UART_CTS_MODE;
        GPIO_InitStruct.Pull = GSM_UART_CTS_PULL;
        GPIO_InitStruct.Speed = GSM_UART_CTS_SPEED;
        GPIO_InitStruct.Alternate = GSM_UART_CTS_ALTERNATE;
        HAL_GPIO_Init(GSM_UART_CTS_PORT, &GPIO_InitStruct);
        /*
         * GSM_UART_DTR
         */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_UART_DTR_PIN;
        GPIO_InitStruct.Mode = GSM_UART_DTR_MODE;
        GPIO_InitStruct.Pull = GSM_UART_DTR_PULL;
        GPIO_InitStruct.Speed = GSM_UART_DTR_SPEED;
        HAL_GPIO_Init(GSM_UART_DTR_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GSM_UART_DTR_PORT, GSM_UART_DTR_PIN, GPIO_PIN_RESET);
        /*
         * GSM_STATUS
         **/
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_STATUS_PIN;
        GPIO_InitStruct.Mode = GSM_STATUS_MODE;
        GPIO_InitStruct.Pull = GSM_STATUS_PULL;
        HAL_GPIO_Init(GSM_STATUS_PORT, &GPIO_InitStruct);
        /*
         * GSM_PWR_EN
         */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_PWR_EN_PIN;
        GPIO_InitStruct.Mode = GSM_PWR_EN_MODE;
        GPIO_InitStruct.Pull = GSM_PWR_EN_PULL;
        GPIO_InitStruct.Speed = GSM_PWR_EN_SPEED;
        HAL_GPIO_Init(GSM_PWR_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GSM_PWR_EN_PORT, GSM_PWR_EN_PIN, GPIO_PIN_RESET);
        /*
         * GSM_RESETN
         */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_RESETN_PIN;
        GPIO_InitStruct.Mode = GSM_RESETN_MODE;
        GPIO_InitStruct.Pull = GSM_RESETN_PULL;
        GPIO_InitStruct.Speed = GSM_RESETN_SPEED;
        HAL_GPIO_Init(GSM_RESETN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GSM_RESETN_PORT, GSM_RESETN_PIN, GPIO_PIN_RESET);
        /*
         * GSM_PWRKEY
         */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GSM_PWRKEY_PIN;
        GPIO_InitStruct.Mode = GSM_PWRKEY_MODE;
        GPIO_InitStruct.Pull = GSM_PWRKEY_PULL;
        GPIO_InitStruct.Speed = GSM_PWRKEY_SPEED;
        HAL_GPIO_Init(GSM_PWRKEY_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);

        EnableCount = 0;
        ModuleState_GSM_SIM800H = BSP_MODULE_STATE_CONNECTED;
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_GSM_SIM800H_Enable(void)
{
    /*todo replace the local handle with protected handle */
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    uint32_t timeout = 0;
    uint32_t start;

    switch(ModuleState_GSM_SIM800H)
    {
    case BSP_MODULE_STATE_CONNECTED:
        if (NULL == BSP_UART2Handle.IrqCallback)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
        }

        if(retcode == RETCODE_OK)
        { /* Checking battery voltage... */

            int32_t result;
            retcode = BSP_AnalogSensor_MeasureSignal( (uint32_t)BSP_VBAT, &result);
            if(retcode == RETCODE_OK)
            {
                if(GSM_VBAT_MIN > result)
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
                    break;/* Do not set module state to BSP_MODULE_STATE_ERROR */
                }
            }
        }
        if(retcode == RETCODE_OK)
        {

            /*
             * Enable UART MCU resource for GSM
             */
            GSM_UART_CLOCK_ENABLE();
            /* Enable Transmitter*/
            SET_BIT(GSM_SERIAL_PORT->CR1, USART_CR1_TE);
            /* Enable Receiver*/
            SET_BIT(GSM_SERIAL_PORT->CR1, USART_CR1_RE);
            /*
             * Clear Pending Interrupt flags
             */
            __HAL_UART_CLEAR_FLAG(&BSP_UART2Handle.huart,
                    UART_CLEAR_PEF|UART_CLEAR_FEF|UART_CLEAR_NEF|UART_CLEAR_OREF|UART_CLEAR_IDLEF|UART_CLEAR_TCF|
                    UART_CLEAR_LBDF|UART_CLEAR_CTSF|UART_CLEAR_RTOF|UART_CLEAR_EOBF|UART_CLEAR_CMF|UART_CLEAR_WUF);
            /*
             * Enable Interrupt flags
             */
            /* Enable the UART RX Not Empty Interrupt */
            /*lint -e{506} The function is provided by STM32Cube library*/
            __HAL_UART_ENABLE_IT(&BSP_UART2Handle.huart, UART_IT_RXNE);
            /* Enable the UART Parity Error Interrupt */
            /*lint -e{506} The function is provided by STM32Cube library*/
            __HAL_UART_ENABLE_IT(&BSP_UART2Handle.huart, UART_IT_PE);
            /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
            /*lint -e{506} The function is provided by STM32Cube library*/
            __HAL_UART_ENABLE_IT(&BSP_UART2Handle.huart, UART_IT_ERR); /*  */
            /*
             * Set the priority for the UART interface
             */
            HAL_NVIC_SetPriority(GSM_UART_IRQn, GSM_UART_PREEMPT_PRIORITY, GSM_UART_SUBPRIORITY);
            /*
             * Enable the interrupt requests
             */
            HAL_NVIC_EnableIRQ(GSM_UART_IRQn);

            /*
             * GSM SICOM SIM800H startup routine
             */

            /* Enable power supply to the GSM module*/
            HAL_GPIO_WritePin(GSM_RESETN_PORT, GSM_RESETN_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GSM_PWR_EN_PORT, GSM_PWR_EN_PIN, GPIO_PIN_SET);
            /* Wait for the GSM module to be powered */
            HAL_Delay(100);
            /* simulate press power key press*/
            HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_SET);

            for(start = HAL_GetTick();((GPIO_PIN_SET != HAL_GPIO_ReadPin(GSM_STATUS_PORT, GSM_STATUS_PIN)) && (timeout <= GSM_POWER_UP_TIMEOUT));)
            {
                timeout = HAL_GetTick() - start;
                if (GSM_PWRKEY_ON_DELAY <= timeout)
                {
                    HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);
                }
            }

            if (GSM_POWER_UP_TIMEOUT <= timeout)
            {
                HAL_GPIO_WritePin(GSM_PWR_EN_PORT, GSM_PWR_EN_PIN, GPIO_PIN_RESET);
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            else
            {
                HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);
            }
        }

        if(RETCODE_OK == retcode)
        {
            /* Enable the UART */
            __HAL_UART_ENABLE(&BSP_UART2Handle.huart);
            /* Check if UART enabled */
            if (HAL_OK != UART_CheckIdleState(&BSP_UART2Handle.huart))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }

        if (RETCODE_OK == retcode)
        {
            ModuleState_GSM_SIM800H = BSP_MODULE_STATE_ENABLED;
        }
        else
        {
            ModuleState_GSM_SIM800H = BSP_MODULE_STATE_ERROR;
        }
        break;
    case BSP_MODULE_STATE_ENABLED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    if(RETCODE_OK == retcode)
    {
        EnableCount++;
        assert(EnableCount);
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_GSM_SIM800H_Disable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    uint32_t timeout = 0;
    uint32_t start;

    switch(ModuleState_GSM_SIM800H)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {

            /*
             * Disable UART MCU resource for GSM
             */
            CLEAR_BIT(GSM_SERIAL_PORT->CR1, USART_CR1_TE);
            start = HAL_GetTick();
            while (FALSE == READ_BIT(GSM_SERIAL_PORT->ISR, USART_ISR_TC) && (GSM_TRANSMIT_DISABLE_TIMEOUT >= timeout))
            {
                timeout = HAL_GetTick() - start;
            }
            if ((GSM_TRANSMIT_DISABLE_TIMEOUT <= timeout) && (FALSE == READ_BIT(GSM_SERIAL_PORT->ISR, USART_ISR_TC)))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            if (RETCODE_OK == retcode)
            {
                __HAL_UART_DISABLE(&BSP_UART2Handle.huart);
                GSM_UART_CLOCK_DISABLE();
                /*
                 * Disable GSM UART Interrupt
                 */
                HAL_NVIC_DisableIRQ(GSM_UART_IRQn);
                /*
                 * Proceed to GSM module Power-down sequence
                 */
                start = HAL_GetTick();
                HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_SET);

                while ((GPIO_PIN_RESET != HAL_GPIO_ReadPin(GSM_STATUS_PORT, GSM_STATUS_PIN)) && (timeout <= GSM_POWER_DOWN_TIMEOUT))
                {
                    timeout = HAL_GetTick() - start;
                    if (GSM_PWRKEY_OFF_DELAY <= timeout)
                    {
                        HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);
                    }
                }
                if (GSM_POWER_DOWN_TIMEOUT <= timeout)
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                }
                else
                {
                    HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GSM_PWR_EN_PORT, GSM_PWR_EN_PIN, GPIO_PIN_RESET);
                }
            }
            if (RETCODE_OK == retcode)
            {
                ModuleState_GSM_SIM800H = BSP_MODULE_STATE_CONNECTED;
            }
            else
            {
                ModuleState_GSM_SIM800H = BSP_MODULE_STATE_ERROR;
            }
        }
        break;
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_GSM_SIM800H_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;

    Retcode_T retcode = RETCODE_OK;
    switch(ModuleState_GSM_SIM800H)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /**
         * Enable all GPIO ports necessary for the operation to make sure all the
         * commands have been executed
         */
        GSM_GPIO_CLOCK_ENABLE();
        /**
         * GSM_PWR_EN disconnect
         */
        HAL_GPIO_DeInit(GSM_PWR_EN_PORT, GSM_PWR_EN_PIN);
        /**
         * GSM_PWRKEY disconnect
         */
        HAL_GPIO_DeInit(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN);
        /**
         * USART2 disconnect
         */
        HAL_GPIO_DeInit(GSM_UART_TXD_PORT, GSM_UART_TXD_PIN);
        HAL_GPIO_DeInit(GSM_UART_RXD_PORT, GSM_UART_RXD_PIN);
        HAL_GPIO_DeInit(GSM_UART_RTS_PORT, GSM_UART_RTS_PIN);
        HAL_GPIO_DeInit(GSM_UART_CTS_PORT, GSM_UART_CTS_PIN);
        /**
         * GSM_STATUS disconnect
         */
        HAL_GPIO_DeInit(GSM_STATUS_PORT, GSM_STATUS_PIN);

        /**
         * GSM_RESETN disconnect
         */
        HAL_GPIO_DeInit(GSM_RESETN_PORT, GSM_RESETN_PIN);

        ModuleState_GSM_SIM800H = BSP_MODULE_STATE_DISCONNECTED;
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_GSM_SIM800H_Wakeup(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    uint32_t timeout = 0;
    uint32_t start;

    HAL_GPIO_WritePin(GSM_UART_DTR_PORT, GSM_UART_DTR_PIN, GPIO_PIN_RESET);

    start = HAL_GetTick();
    while (GSM_DTR_WAKEUP_DELAY >= timeout)
    {
        timeout = HAL_GetTick() - start;
    }
    retcode = RETCODE_OK;
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_GSM_SIM800H_Sleep(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    HAL_GPIO_WritePin(GSM_UART_DTR_PORT, GSM_UART_DTR_PIN, GPIO_PIN_SET);

    retcode = RETCODE_OK;
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_GSM_SIM800H_Restart(void)
{
    BSP_MODULE_TRY_LOCK;

    Retcode_T retcode = RETCODE_OK;
    uint32_t timeout = 0;
    uint32_t start;

    /* Stopping the GSM module... */
    start = HAL_GetTick();

    HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_SET);

    while ((GPIO_PIN_RESET != HAL_GPIO_ReadPin(GSM_STATUS_PORT, GSM_STATUS_PIN)) && (timeout <= GSM_POWER_DOWN_TIMEOUT))
    {
        timeout = HAL_GetTick() - start;
        if (GSM_PWRKEY_OFF_DELAY <= timeout)
        {
            HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);
        }
    }

    if (GSM_POWER_DOWN_TIMEOUT <= timeout)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    else
    {
        HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);
    }

    if(RETCODE_OK == retcode)
    {
        /* Waiting... */
        HAL_Delay(GSM_RESTART_DELAY);

        /* Starting the GSM module... */
        start = HAL_GetTick();

        HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_SET);

        while ((GPIO_PIN_SET != HAL_GPIO_ReadPin(GSM_STATUS_PORT, GSM_STATUS_PIN)) && (timeout <= GSM_POWER_UP_TIMEOUT))
        {
            timeout = HAL_GetTick() - start;
            if (GSM_PWRKEY_ON_DELAY <= timeout)
            {
                HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);
            }
        }

        if (GSM_POWER_UP_TIMEOUT >= timeout)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
        else
        {
            HAL_GPIO_WritePin(GSM_PWRKEY_PORT, GSM_PWRKEY_PIN, GPIO_PIN_RESET);
        }
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_GSM_SIM800H_Reset(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    uint32_t timeout = 0;
    uint32_t start;

    HAL_GPIO_WritePin(GSM_RESETN_PORT, GSM_RESETN_PIN, GPIO_PIN_RESET);
    HAL_Delay(GSM_RESET_STATUS_DELAY);
    if (GPIO_PIN_RESET != HAL_GPIO_ReadPin(GSM_STATUS_PORT, GSM_STATUS_PIN))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    if(RETCODE_OK == retcode)
    {
        HAL_Delay(GSM_RESET_DELAY);
        HAL_GPIO_WritePin(GSM_RESETN_PORT, GSM_RESETN_PIN, GPIO_PIN_SET);

        start = HAL_GetTick();
        while ((GPIO_PIN_SET != HAL_GPIO_ReadPin(GSM_STATUS_PORT, GSM_STATUS_PIN)) && (timeout <= GSM_RESET_TIMEOUT))
        {
            timeout = HAL_GetTick() - start;
        }

        if (GSM_RESET_TIMEOUT <= timeout)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    BSP_MODULE_UNLOCK;
    return retcode;

}

HWHandle_T BSP_GSM_SIM800H_GetUARTHandle(void)
{
    return (HWHandle_T) &BSP_UART2Handle;
}

void GSM_SIM800H_UARTISR(void)
{
    BSP_UART2Handle.IrqCallback((UART_T) &BSP_UART2Handle);
}

#endif /* BCDS_FEATURE_BSP_GSM_SIM800H */
/**@}*//* @ingroup BSP_GSM_MODULE */
