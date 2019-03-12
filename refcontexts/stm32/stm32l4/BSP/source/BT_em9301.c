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
 * @ingroup BSP_BT_MODULE
 * @{
 * @file
 * @brief  Implementation of Bluetooth BSP functions
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_BT_EM9301

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_BT_EM9301.h"

#if BCDS_FEATURE_BSP_BT_EM9301

#include "BoardSettings.h"
#include "BT_em9301.h"
#include "BCDS_MCU_UART_Handle.h"
#include "BSP_ModuleState.h"

enum GPIOLock_E
{
    GPIO_LOCKED = 0,
    GPIO_UNLOCKED,
};

typedef enum GPIOLock_E GPIOLock_T;

static GPIOLock_T BT_GPIOLock = GPIO_LOCKED;

DMA_HandleTypeDef BSP_UART1DMATxHandle;
DMA_HandleTypeDef BSP_UART1DMARxHandle;
static struct MCU_UART_S BSP_UART1Handle;

static volatile enum BSP_ModuleState_E ModuleState_BT_EM9301 = BSP_MODULE_STATE_UNINITIALIZED;
static volatile uint8_t EnableCount;

extern Retcode_T Board_USART1_UART_Init(void);

enum BSP_ModuleState_E BT_EM9301_getState(void)
{
    return ModuleState_BT_EM9301;
}
/*
 * @brief Initialize the USART1 resource
 * @details This function Initializes the USART1 resource in UART mode. It is called
 * within the Board_Initialize() function. It stores the desired configuration
 * into the resource registers and puts it into low power
 * consumption configuration until it is requested by the application.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_USART1_UART_Init(void)
{
    Retcode_T retcode = RETCODE_OK;
    UART_WakeUpTypeDef WakeUpSelection;

    /* Enable the clock for the DMA resource */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* Configure the DMA handler for Transmission process */
    BSP_UART1DMATxHandle.Instance = BT_DMA_TX_CHANNEL;
    BSP_UART1DMATxHandle.Init.Request = BT_DMA_TX_REQUEST;
    BSP_UART1DMATxHandle.Init.Direction = BT_DMA_TX_DIRECTION;
    BSP_UART1DMATxHandle.Init.PeriphInc = BT_DMA_TX_PINC;
    BSP_UART1DMATxHandle.Init.MemInc = BT_DMA_TX_MINC;
    BSP_UART1DMATxHandle.Init.PeriphDataAlignment = BT_DMA_TX_PDATA_ALIGN;
    BSP_UART1DMATxHandle.Init.MemDataAlignment = BT_DMA_TX_MDATA_ALIGN;
    BSP_UART1DMATxHandle.Init.Mode = BT_DMA_TX_MODE;
    BSP_UART1DMATxHandle.Init.Priority = BT_DMA_TX_PRIORITY;

    /* Configure the DMA handler for Transmission process */
    BSP_UART1DMARxHandle.Instance = BT_DMA_RX_CHANNEL;
    BSP_UART1DMARxHandle.Init.Request = BT_DMA_RX_REQUEST;
    BSP_UART1DMARxHandle.Init.Direction = BT_DMA_RX_DIRECTION;
    BSP_UART1DMARxHandle.Init.PeriphInc = BT_DMA_RX_PINC;
    BSP_UART1DMARxHandle.Init.MemInc = BT_DMA_RX_MINC;
    BSP_UART1DMARxHandle.Init.PeriphDataAlignment = BT_DMA_RX_PDATA_ALIGN;
    BSP_UART1DMARxHandle.Init.MemDataAlignment = BT_DMA_RX_MDATA_ALIGN;
    BSP_UART1DMARxHandle.Init.Mode = BT_DMA_RX_MODE;
    BSP_UART1DMARxHandle.Init.Priority = BT_DMA_RX_PRIORITY;

    if (HAL_OK != HAL_DMA_Init(&BSP_UART1DMARxHandle))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }

    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_DMA_Init(&BSP_UART1DMATxHandle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == retcode)
    {
        /* Enable the clock */
        __HAL_RCC_USART1_CLK_ENABLE();
        /* Configure the UART */
        BSP_UART1Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_DMA;

        BSP_UART1Handle.huart.Instance = BT_SERIAL_PORT;
        BSP_UART1Handle.huart.Init.BaudRate = BT_BAUD_RATE;
        BSP_UART1Handle.huart.Init.WordLength = BT_DATA_BITS;
        BSP_UART1Handle.huart.Init.StopBits = BT_STOP_BITS;
        BSP_UART1Handle.huart.Init.Parity = BT_PARITY;
        BSP_UART1Handle.huart.Init.Mode = BT_MODE;
        BSP_UART1Handle.huart.Init.HwFlowCtl = BT_FLOW_CONTROL;
        BSP_UART1Handle.huart.Init.OverSampling = BT_SAMPLING;
        BSP_UART1Handle.huart.Init.OneBitSampling = BT_ONE_BIT_SAMPLING;
        BSP_UART1Handle.huart.AdvancedInit.AdvFeatureInit = BT_ADVANCED_INT;

        /* Link DMA to peripheral driver */
        BSP_UART1Handle.huart.hdmarx = &BSP_UART1DMARxHandle;
        BSP_UART1Handle.huart.hdmatx = &BSP_UART1DMATxHandle;

        BSP_UART1DMATxHandle.Parent = &BSP_UART1Handle.huart;
        BSP_UART1DMARxHandle.Parent = &BSP_UART1Handle.huart;

        if (HAL_OK != HAL_UART_Init(&BSP_UART1Handle.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }

        WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;

        if (HAL_OK != HAL_UARTEx_StopModeWakeUpSourceConfig(&(BSP_UART1Handle.huart), WakeUpSelection))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }

        /* Disable the UART and its clock */
        __HAL_UART_DISABLE(&BSP_UART1Handle.huart);
        __HAL_RCC_USART1_CLK_DISABLE();
    }
    if (RETCODE_OK == retcode)
    {
        ModuleState_BT_EM9301 = BSP_MODULE_STATE_DISCONNECTED;
    }

    return retcode;
}

Retcode_T BSP_BT_EM9301_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;
    GPIO_InitTypeDef GPIO_InitStruct;
    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_BT_EM9301)
    {
    case BSP_MODULE_STATE_DISCONNECTED:
    case BSP_MODULE_STATE_ERROR: /*In case of error state we try to connect again to go back in a defined state, will always succeed*/
        /* Enabling GPIO clocks required by the BT module */
        BT_GPIO_CLOCK_ENABLE();

        /* BT USART2 TX */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BT_UART_TX_PIN;
        GPIO_InitStruct.Mode = BT_UART_TX_MODE;
        GPIO_InitStruct.Pull = BT_UART_TX_PULL;
        GPIO_InitStruct.Speed = BT_UART_TX_SPEED;
        GPIO_InitStruct.Alternate = BT_UART_TX_ALTERNATE;
        HAL_GPIO_Init(BT_UART_TX_PORT, &GPIO_InitStruct);

        /* BT USART2 RX */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BT_UART_RX_PIN;
        GPIO_InitStruct.Mode = BT_UART_RX_MODE;
        GPIO_InitStruct.Pull = BT_UART_RX_PULL;
        GPIO_InitStruct.Speed = BT_UART_RX_SPEED;
        GPIO_InitStruct.Alternate = BT_UART_RX_ALTERNATE;
        HAL_GPIO_Init(BT_UART_RX_PORT, &GPIO_InitStruct);

        /* BT_WAKEUP */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BT_WAKEUP_PIN;
        GPIO_InitStruct.Mode = BT_WAKEUP_MODE;
        GPIO_InitStruct.Pull = BT_WAKEUP_PULL;
        GPIO_InitStruct.Speed = BT_WAKEUP_SPEED;
        HAL_GPIO_Init(BT_WAKEUP_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(BT_WAKEUP_PORT, BT_WAKEUP_PIN, BSP_IO_PIN_LOW);

        /* BT_RST */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BT_RST_PIN;
        GPIO_InitStruct.Mode = BT_RST_MODE;
        GPIO_InitStruct.Pull = BT_RST_PULL;
        GPIO_InitStruct.Speed = BT_RST_SPEED;
        HAL_GPIO_Init(BT_RST_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(BT_RST_PORT, BT_RST_PIN, BSP_IO_PIN_LOW);

        /* @TODO 3v3 enable trigger SIGTRAP if first. */
        /* BT_3V3_EN */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BT_3V3_EN_PIN;
        GPIO_InitStruct.Mode = BT_3V3_EN_MODE;
        GPIO_InitStruct.Pull = BT_3V3_EN_PULL;
        GPIO_InitStruct.Speed = BT_3V3_EN_SPEED;
        HAL_GPIO_Init(BT_3V3_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(BT_3V3_EN_PORT, BT_3V3_EN_PIN, BSP_IO_PIN_HIGH);

        EnableCount = 0;
        ModuleState_BT_EM9301 = BSP_MODULE_STATE_CONNECTED;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_BT_EM9301_Enable(void)
{

    Retcode_T retcode = RETCODE_OK;

    BSP_MODULE_TRY_LOCK;

    assert(BSP_UART1Handle.TransferMode == BCDS_HAL_TRANSFER_MODE_DMA);
    /** @todo 2016-11-29, BCDS/ENG1: Clarify which ISR is called in DMA mode - IRQ or DMA_TX/DMA_RX? */
    assert(BSP_UART1Handle.IrqCallback);
    assert(BSP_UART1Handle.DmaTxCallback);
    assert(BSP_UART1Handle.DmaRxCallback);

    switch(ModuleState_BT_EM9301)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /** @todo 2017-03-10, BCDS/ENG1: Use proper BLE discharge time delay. */
        /* Wait until BLE chip discharges completely + 10 milliseconds */
        HAL_Delay(BT_POWER_DISCHARGE_DELAY);

        /* Enable power supply */
        HAL_GPIO_WritePin(BT_3V3_EN_PORT, BT_3V3_EN_PIN, BSP_IO_PIN_LOW);

        /* Wait for the BT module to start up */
        HAL_Delay(BT_STARTUP_DELAY);
        /* Enable the USART MCU resource */
        BT_UART_CLOCK_ENABLE();

        SET_BIT(BT_SERIAL_PORT->CR1, USART_CR1_TE);

        /* Clear Pending Interrupt flags */
        __HAL_UART_CLEAR_FLAG(&BSP_UART1Handle.huart,
                UART_CLEAR_PEF|UART_CLEAR_FEF|UART_CLEAR_NEF|UART_CLEAR_OREF|UART_CLEAR_IDLEF|UART_CLEAR_TCF|
                UART_CLEAR_LBDF|UART_CLEAR_CTSF|UART_CLEAR_RTOF|UART_CLEAR_EOBF|UART_CLEAR_CMF|UART_CLEAR_WUF);

        /* Set the priority for the UART interface */
        HAL_NVIC_SetPriority(BT_USART_IRQn, BT_USART_PREEMPT_PRIORITY, BT_USART_SUBPRIORITY);

        /* NVIC configuration for DMA transfer complete interrupt (USART1_TX) */
        HAL_NVIC_SetPriority(BT_TX_DMA_IRQ, BT_TX_DMA_PREEMPT_PRIORITY, BT_TX_SUBPRIORITY);
        HAL_NVIC_EnableIRQ(BT_TX_DMA_IRQ);

        /* NVIC configuration for DMA transfer complete interrupt (USART1_RX) */
        HAL_NVIC_SetPriority(BT_RX_DMA_IRQ, BT_RX_DMA_PREEMPT_PRIORITY, BT_RX_SUBPRIORITY);
        HAL_NVIC_EnableIRQ(BT_RX_DMA_IRQ);

        /* Enable the interrupt requests */
        HAL_NVIC_EnableIRQ(BT_USART_IRQn);

        /* Unlock BT module GPIOs */
        BT_GPIOLock = GPIO_UNLOCKED;

        /* Enable the UART */
        __HAL_UART_ENABLE(&BSP_UART1Handle.huart);

        /* Check if the USART has been enabled */
        if (HAL_OK != UART_CheckIdleState(&BSP_UART1Handle.huart))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
        if (RETCODE_OK == retcode)
        {
            ModuleState_BT_EM9301 = BSP_MODULE_STATE_ENABLED;
        }
        else
        {
            ModuleState_BT_EM9301 = BSP_MODULE_STATE_ERROR;
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

Retcode_T BSP_BT_EM9301_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t start;
    uint32_t timeout = 0;

    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_BT_EM9301)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {
            /** @todo 2016-11-29, BCDS/ENG1: Analyze what happens with DMA2 CH6 and CH7
             *        when the parent UART gets disabled
             */

            /* Disable the UART MCU resource */
            /* Disable current transmission */
            CLEAR_BIT(BT_SERIAL_PORT->CR1, USART_CR1_TE);
            start = HAL_GetTick();
            /* Wait until transmission complete */
            while (0 == READ_BIT(BT_SERIAL_PORT->ISR, USART_ISR_TC) && (BT_TRANSMIT_DISABLE_TIMEOUT >= timeout))
            {
                timeout = HAL_GetTick() - start;
            }
            /* Check if transmission timeout occurred */
            if ((BT_TRANSMIT_DISABLE_TIMEOUT <= timeout) && (0 == READ_BIT(BT_SERIAL_PORT->ISR, USART_ISR_TC)))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            else
            {
                /* Lock the GPIO functions  */
                BT_GPIOLock = GPIO_LOCKED;
                /* Disable the UART module */
                __HAL_UART_DISABLE(&BSP_UART1Handle.huart);
                /* Disable the clock for the UART Resource */
                BT_UART_CLOCK_DISABLE();
                /* Disable the Interrupt requests */
                HAL_NVIC_DisableIRQ(BT_USART_IRQn);
                /* Clear the pending interrupt */
                HAL_NVIC_ClearPendingIRQ(BT_USART_IRQn);

                /* Disable power supply */
                HAL_GPIO_WritePin(BT_3V3_EN_PORT, BT_3V3_EN_PIN, BSP_IO_PIN_HIGH);
            }
            if (RETCODE_OK == retcode)
            {
                ModuleState_BT_EM9301 = BSP_MODULE_STATE_CONNECTED;
            }
            else
            {
                ModuleState_BT_EM9301 = BSP_MODULE_STATE_ERROR;
            }
        }
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_BT_EM9301_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    switch(ModuleState_BT_EM9301)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /*
         * Enable all GPIO ports necessary for the operation to make sure all the
         * commands have been executed
         */
        BT_GPIO_CLOCK_ENABLE();
        /* USART1 disconnect */
        HAL_GPIO_DeInit(BT_UART_TX_PORT, BT_UART_TX_PIN);
        HAL_GPIO_DeInit(BT_UART_RX_PORT, BT_UART_RX_PIN);
        /* BT_WAKEUP disconnect */
        HAL_GPIO_DeInit(BT_WAKEUP_PORT, BT_WAKEUP_PIN);
        /* BT_RST disconnect */
        HAL_GPIO_DeInit(BT_RST_PORT, BT_RST_PIN);
        /* BT_PWR_EN disconnect */
        HAL_GPIO_DeInit(BT_3V3_EN_PORT, BT_3V3_EN_PIN);

        ModuleState_BT_EM9301 = BSP_MODULE_STATE_DISCONNECTED;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
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

Retcode_T BSP_BT_EM9301_SetWUHigh(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;

    if (BT_GPIOLock == GPIO_UNLOCKED)
    {
        HAL_GPIO_WritePin(BT_WAKEUP_PORT, BT_WAKEUP_PIN, BSP_IO_PIN_HIGH);
        HAL_Delay(BT_WAKEUP_DELAY);
        retcode = RETCODE_OK;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    BSP_MODULE_UNLOCK;
    return retcode;

}

Retcode_T BSP_BT_EM9301_SetWULow(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;

    if (BT_GPIOLock == GPIO_UNLOCKED)
    {
        HAL_GPIO_WritePin(BT_WAKEUP_PORT, BT_WAKEUP_PIN, BSP_IO_PIN_LOW);
        retcode = RETCODE_OK;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_BT_EM9301_Reset(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;

    /** @todo 2016-11-29, BCDS/ENG1: Clarify with HAL/BSP owner on what type of RESET needs to be implemented
     *
     * There is remote and local soft reset, power-on reset, pin reset. Requirement is unclear.
     */
    HAL_GPIO_WritePin(BT_RST_PORT, BT_RST_PIN, BSP_IO_PIN_HIGH);
    HAL_Delay(BT_RESET_PULLUP_DELAY);
    HAL_GPIO_WritePin(BT_RST_PORT, BT_RST_PIN, BSP_IO_PIN_LOW);
    HAL_Delay(BT_RESET_PROCESSING_DELAY);

    retcode = RETCODE_OK;
    BSP_MODULE_UNLOCK;

    return retcode;
}

HWHandle_T BSP_BT_EM9301_GetUARTHandle(void)
{
    return (HWHandle_T) &BSP_UART1Handle;
}

void BT_EM9301_UARTISR(void)
{
    /** @todo 2016-11-29, BCDS/ENG1: Clarify whether UART IRQ is required in DMA mode */
    BSP_UART1Handle.IrqCallback((UART_T) &BSP_UART1Handle);
}

void BT_EM9301_UARTTxIsr(void)
{
    BCDS_HAL_DMA_IRQHandler(&BSP_UART1DMATxHandle, 6UL);
}

void BT_EM9301_UARTRxIsr(void)
{
    BCDS_HAL_DMA_IRQHandler(&BSP_UART1DMARxHandle, 7UL);
}

#endif /* BCDS_FEATURE_BSP_BT_EM9301 */

/**@}*//* @ingroup BSP_BT_MODULE */
