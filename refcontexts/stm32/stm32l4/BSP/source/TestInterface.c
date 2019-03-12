/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @ingroup BSP_TEST_IF
 * @{
 * @file
 * @brief  Implementation of test interface BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_TEST_INTERFACE

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_TestInterface.h"

#if BCDS_FEATURE_BSP_TEST_INTERFACE

#include "BoardSettings.h"
#include "TestInterface.h"
#include "BCDS_MCU_UART_Handle.h"
#include "BSP_ModuleState.h"

struct MCU_UART_S BSP_UART3Handle;

static volatile enum BSP_ModuleState_E ModuleState_TestInterface = BSP_MODULE_STATE_UNINITIALIZED;
static volatile uint8_t EnableCount;

/*
 * @brief Initialize the USART3 resource
 * @details This function Initializes the USART3 resource in UART mode. It is
 * called within the Board_Initialize() function. It stores the desired configuration
 * into the resource registers and puts it into low power
 * consumption configuration until it is requested by the application.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_USART3_UART_Init(void);

enum BSP_ModuleState_E TestInterface_getState(void)
{
    return ModuleState_TestInterface;
}

Retcode_T Board_USART3_UART_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* Enable the clock */
    __HAL_RCC_USART3_CLK_ENABLE();
    /* Configure the UART */
    BSP_UART3Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    BSP_UART3Handle.huart.Instance = TEST_SERIAL_PORT;
    BSP_UART3Handle.huart.Init.BaudRate = TEST_BAUD_RATE;
    BSP_UART3Handle.huart.Init.WordLength = TEST_DATA_BITS;
    BSP_UART3Handle.huart.Init.StopBits = TEST_STOP_BITS;
    BSP_UART3Handle.huart.Init.Parity = TEST_PARITY;
    BSP_UART3Handle.huart.Init.Mode = TEST_MODE;
    BSP_UART3Handle.huart.Init.HwFlowCtl = TEST_FLOW_CONTROL;
    BSP_UART3Handle.huart.Init.OverSampling = TEST_SAMPLING;
    BSP_UART3Handle.huart.Init.OneBitSampling = TEST_ONE_BIT_SAMPLING;
    BSP_UART3Handle.huart.AdvancedInit.AdvFeatureInit = TEST_ADVANCED_INT;
    if (HAL_OK != HAL_UART_Init(&BSP_UART3Handle.huart))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    /* Disable the UART and its clock */
    __HAL_USART_DISABLE(&BSP_UART3Handle.huart);
    __HAL_RCC_USART3_CLK_DISABLE();
    if(RETCODE_OK == retcode)
    {
        ModuleState_TestInterface = BSP_MODULE_STATE_DISCONNECTED;
    }

    return retcode;
}

Retcode_T BSP_TestInterface_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    GPIO_InitTypeDef GPIO_InitStruct;

    switch(ModuleState_TestInterface)
    {
    case BSP_MODULE_STATE_DISCONNECTED:
    case BSP_MODULE_STATE_ERROR: /*In case of error state we try to connect again to go back in a defined state, will always succeed*/

        /* Enabling GPIO clocks required by the TEST module */
        TEST_GPIO_CLOCK_ENABLE();

        /*Test interface TX */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = TEST_TX_PIN;
        GPIO_InitStruct.Mode = TEST_TX_MODE;
        GPIO_InitStruct.Pull = TEST_TX_PULL;
        GPIO_InitStruct.Speed = TEST_TX_SPEED;
        GPIO_InitStruct.Alternate = TEST_TX_ALTERNATE;
        HAL_GPIO_Init(TEST_TX_PORT, &GPIO_InitStruct);

        /*Test Interface RX */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = TEST_RX_PIN;
        GPIO_InitStruct.Mode = TEST_RX_MODE;
        GPIO_InitStruct.Pull = TEST_RX_PULL;
        GPIO_InitStruct.Speed = TEST_RX_SPEED;
        GPIO_InitStruct.Alternate = TEST_RX_ALTERNATE;
        HAL_GPIO_Init(TEST_RX_PORT, &GPIO_InitStruct);
        EnableCount = 0;
        ModuleState_TestInterface = BSP_MODULE_STATE_CONNECTED;
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

Retcode_T BSP_TestInterface_Enable(void)
{
    BSP_MODULE_TRY_LOCK;

    Retcode_T retcode = RETCODE_OK;
    /* Dont enable if the callback has not been registered */
    if ( NULL == BSP_UART3Handle.IrqCallback)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    if(RETCODE_OK == retcode)
    {
        switch(ModuleState_TestInterface)
        {
        case BSP_MODULE_STATE_CONNECTED:
            /*
             * Enable the peripheral
             */
            TEST_UART_CLOCK_ENABLE();

            SET_BIT(TEST_SERIAL_PORT->CR1, USART_CR1_TE);
            /*
             * Clear Pending Interrupt flags
             */
            __HAL_UART_CLEAR_FLAG(&BSP_UART3Handle.huart,
                    UART_CLEAR_PEF|UART_CLEAR_FEF|UART_CLEAR_NEF|UART_CLEAR_OREF|UART_CLEAR_IDLEF|UART_CLEAR_TCF|
                    UART_CLEAR_LBDF|UART_CLEAR_CTSF|UART_CLEAR_RTOF|UART_CLEAR_EOBF|UART_CLEAR_CMF|UART_CLEAR_WUF);
            /*
             * Enable Interrupt flags
             */
            /* Enable the UART RX Not Empty Interrupt */
            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_ENABLE_IT(&BSP_UART3Handle.huart, UART_IT_RXNE);
            /* Enable the UART Parity Error Interrupt */
            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_ENABLE_IT(&BSP_UART3Handle.huart, UART_IT_PE);
            /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_ENABLE_IT(&BSP_UART3Handle.huart, UART_IT_ERR); /*  */

            /* Enable the interrupt requests */
            HAL_NVIC_SetPriority(TEST_UART_IRQn, TEST_PREEMPT_PRIORITY, TEST_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(TEST_UART_IRQn);

            __HAL_UART_ENABLE(&BSP_UART3Handle.huart);
            /*
             * Check if the UART has been enabled
             */
            if (HAL_OK != UART_CheckIdleState(&BSP_UART3Handle.huart))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            if (RETCODE_OK == retcode)
            {
                ModuleState_TestInterface = BSP_MODULE_STATE_ENABLED;
            }
            else
            {
                ModuleState_TestInterface = BSP_MODULE_STATE_ERROR;
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
    }

    if(RETCODE_OK == retcode)
    {
        EnableCount++;
        assert(EnableCount);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_TestInterface_Disable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    uint32_t start = 0;
    uint32_t timeout = 0;

    switch(ModuleState_TestInterface)
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
             * Disable the UART MCU resource
             */
            /* Disable current transmission */
            CLEAR_BIT(TEST_SERIAL_PORT->CR1, USART_CR1_TE);
            start = HAL_GetTick();
            /* Wait until transmission complete*/
            while (0 == READ_BIT(TEST_SERIAL_PORT->ISR, USART_ISR_TC) && (TEST_TRANSMIT_DISABLE_TIMEOUT >= timeout))
            {
                timeout = HAL_GetTick() - start;
            }
            /* Check if transmission timeout occured */
            if ((TEST_TRANSMIT_DISABLE_TIMEOUT <= timeout) && (0 == READ_BIT(TEST_SERIAL_PORT->ISR, USART_ISR_TC)))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            else
            {
                /* Disable the UART module */
                __HAL_UART_DISABLE(&BSP_UART3Handle.huart);
                /* Disable the clock for the UART Resource */
                TEST_UART_CLOCK_DISABLE();
                /*
                 * Disable the Interrupt requests
                 */
                HAL_NVIC_DisableIRQ(TEST_UART_IRQn);
                /*
                 * Clear the pending interrupt
                 */
                HAL_NVIC_ClearPendingIRQ(TEST_UART_IRQn);
            }
            if (RETCODE_OK == retcode)
            {
                ModuleState_TestInterface = BSP_MODULE_STATE_CONNECTED;
            }
            else
            {
                ModuleState_TestInterface = BSP_MODULE_STATE_ERROR;
            }
        }
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_TestInterface_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    switch(ModuleState_TestInterface)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /*
         * Enable all GPIO ports necessary for the operation to make sure all the
         * commands have been executed
         */
        TEST_GPIO_CLOCK_ENABLE();

        HAL_GPIO_DeInit(TEST_TX_PORT, TEST_TX_PIN);
        HAL_GPIO_DeInit(TEST_RX_PORT, TEST_RX_PIN);

        ModuleState_TestInterface = BSP_MODULE_STATE_DISCONNECTED;
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

HWHandle_T BSP_TestInterface_GetUARTHandle(void)
{
    return (HWHandle_T) &BSP_UART3Handle;
}

void TestInterface_UARTISR(void)
{
    BSP_UART3Handle.IrqCallback((UART_T) &BSP_UART3Handle);
}

#endif /* BCDS_FEATURE_BSP_TEST_INTERFACE */
/**@}*//* @ingroup BSP_TEST_IF */
