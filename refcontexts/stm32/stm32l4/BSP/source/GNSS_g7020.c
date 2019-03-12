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
 * @ingroup BSP_GNSS_MODULE
 * @{
 * @file
 * @brief  Implementation of GNSS BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_GNSS_G7020

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_GNSS_G7020.h"

#if BCDS_FEATURE_BSP_GNSS_G7020

#include "GNSS_g7020.h"
#include "LDO_tlv70233.h"
#include "BoardSettings.h"
#include "BSP_BoardType.h"
#include "BCDS_MCU_UART_Handle.h"
#include "BSP_ModuleState.h"

static struct MCU_UART_S BSP_LPUART1Handle;/**< used by the test interface */
static volatile enum BSP_ModuleState_E ModuleState_GNSS_g7020 = BSP_MODULE_STATE_UNINITIALIZED;
static volatile uint8_t EnableCount;

enum BSP_ModuleState_E GNSS_G7020_getState(void)
{
    return ModuleState_GNSS_g7020;
}

/*
 * @brief Initialize the Low Power UART resource
 * @details This function Initializes the LPUART1 resource in UART mode. It is
 * called within the Board_Initialize() function. It stores the desired
 * configuration into the resource registers and puts it into low power
 * consumption configuration until it is requested by the application.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_LPUART1_UART_Init(void);

Retcode_T Board_LPUART1_UART_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* Enable the cock */
    __HAL_RCC_LPUART1_CLK_ENABLE();

    /* Configure the UART */
    BSP_LPUART1Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    BSP_LPUART1Handle.huart.Instance = GNSS_SERIAL_PORT;
    BSP_LPUART1Handle.huart.Init.BaudRate = GNSS_BAUD_RATE;
    BSP_LPUART1Handle.huart.Init.WordLength = GNSS_DATA_BITS;
    BSP_LPUART1Handle.huart.Init.StopBits = GNSS_STOP_BITS;
    BSP_LPUART1Handle.huart.Init.Parity = GNSS_PARITY;
    BSP_LPUART1Handle.huart.Init.Mode = GNSS_MODE;
    BSP_LPUART1Handle.huart.Init.HwFlowCtl = GNSS_FLOW_CONTROL;
    BSP_LPUART1Handle.huart.Init.OverSampling = GNSS_SAMPLING;
    BSP_LPUART1Handle.huart.Init.OneBitSampling = GNSS_ONE_BIT_SAMPLING;
    BSP_LPUART1Handle.huart.AdvancedInit.AdvFeatureInit = GNSS_ADVANCED_INT;

    if (HAL_OK != HAL_UART_Init(&BSP_LPUART1Handle.huart))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }

    /* Disable the UART and its clock */
    __HAL_UART_DISABLE(&BSP_LPUART1Handle.huart);
    __HAL_RCC_LPUART1_CLK_DISABLE();

    if(RETCODE_OK == retcode)
    {
        ModuleState_GNSS_g7020 = BSP_MODULE_STATE_DISCONNECTED;
    }
    return retcode;
}

Retcode_T BSP_GNSS_G7020_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    GPIO_InitTypeDef GPIO_InitStruct;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_GNSS_g7020)
    {
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH: */
    case BSP_MODULE_STATE_DISCONNECTED:
        /* Enabling GPIO clocks required by the GNSS module */
        GNSS_GPIO_CLOCK_ENABLE();

        /* GNSS LPUART1 TX */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GNSS_UART_TXD_PIN;
        GPIO_InitStruct.Mode = GNSS_UART_TXD_MODE;
        GPIO_InitStruct.Pull = GNSS_UART_TXD_PULL;
        GPIO_InitStruct.Speed = GNSS_UART_TXD_SPEED;
        GPIO_InitStruct.Alternate = GNSS_UART_TXD_ALTERNATE;
        HAL_GPIO_Init(GNSS_UART_TXD_PORT, &GPIO_InitStruct);

        /* GNSS LPUART1 RX */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GNSS_UART_RXD_PIN;
        GPIO_InitStruct.Mode = GNSS_UART_RXD_MODE;
        GPIO_InitStruct.Pull = GNSS_UART_RXD_PULL;
        GPIO_InitStruct.Speed = GNSS_UART_RXD_SPEED;
        GPIO_InitStruct.Alternate = GNSS_UART_RXD_ALTERNATE;
        HAL_GPIO_Init(GNSS_UART_RXD_PORT, &GPIO_InitStruct);

        /* GNSS_CLK32 */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GNSS_CLK32_PIN;
        GPIO_InitStruct.Mode = GNSS_CLK32_MODE;
        GPIO_InitStruct.Pull = GNSS_CLK32_PULL;
        HAL_GPIO_Init(GNSS_CLK32_PORT, &GPIO_InitStruct);

        /* GNSS_EN */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GNSS_EN_PIN;
        GPIO_InitStruct.Mode = GNSS_EN_MODE;
        GPIO_InitStruct.Pull = GNSS_EN_PULL;
        GPIO_InitStruct.Speed = GNSS_EN_SPEED;
        HAL_GPIO_Init(GNSS_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GNSS_EN_PORT, GNSS_EN_PIN, BSP_IO_PIN_HIGH);

        /* GNSS_RESETN */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = GNSS_RESETN_PIN;
        GPIO_InitStruct.Mode = GNSS_RESETN_MODE;
        GPIO_InitStruct.Pull = GNSS_RESETN_PULL;
        GPIO_InitStruct.Speed = GNSS_RESETN_SPEED;
        HAL_GPIO_Init(GNSS_RESETN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GNSS_RESETN_PORT, GNSS_RESETN_PIN, BSP_IO_PIN_HIGH);
        EnableCount = 0;
        ModuleState_GNSS_g7020 = BSP_MODULE_STATE_CONNECTED;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH: */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_GNSS_G7020_Enable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    assert(BSP_LPUART1Handle.TransferMode == BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    assert(BSP_LPUART1Handle.IrqCallback);

    switch(ModuleState_GNSS_g7020)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /* Enable 3v3 power rail if not already enabled */
        retcode = BSP_LDO_TLV70233_Enable();
        if (retcode == RETCODE_OK)
        {
            /* enable 32.768 kHz clock output on LSCO */
            HAL_RCCEx_EnableLSCO(RCC_LSCOSOURCE_LSE);

            /* Enable the peripheral */
            GNSS_UART_CLOCK_ENABLE();

            SET_BIT(GNSS_SERIAL_PORT->CR1, USART_CR1_TE);
            SET_BIT(GNSS_SERIAL_PORT->CR1, USART_CR1_RE);

            /* Clear Pending Interrupt flags */
            __HAL_UART_CLEAR_FLAG(&BSP_LPUART1Handle.huart,
                    UART_CLEAR_PEF|UART_CLEAR_FEF|UART_CLEAR_NEF|UART_CLEAR_OREF|UART_CLEAR_IDLEF|UART_CLEAR_TCF|
                    UART_CLEAR_LBDF|UART_CLEAR_CTSF|UART_CLEAR_RTOF|UART_CLEAR_EOBF|UART_CLEAR_CMF|UART_CLEAR_WUF);

            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_DISABLE_IT(&BSP_LPUART1Handle.huart, UART_IT_TXE);

            /* Enable Interrupt flags */
            /* Enable the UART RX Not Empty Interrupt */
            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_ENABLE_IT(&BSP_LPUART1Handle.huart, UART_IT_RXNE);
            /* Enable the UART Parity Error Interrupt */
            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_ENABLE_IT(&BSP_LPUART1Handle.huart, UART_IT_PE);
            /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_ENABLE_IT(&BSP_LPUART1Handle.huart, UART_IT_ERR);

            /* Enable the interrupt requests */
            HAL_NVIC_SetPriority(GNSS_UART_IRQn, GNSS_PREEMPT_PRIORITY, GNSS_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(GNSS_UART_IRQn);

            __HAL_UART_ENABLE(&BSP_LPUART1Handle.huart);

            /* Check if the UART has been enabled */
            if (HAL_OK != UART_CheckIdleState(&BSP_LPUART1Handle.huart))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }

            /* Activate the GNSS (after everything is ready for communication) */
            HAL_GPIO_WritePin(GNSS_EN_PORT, GNSS_EN_PIN, BSP_IO_PIN_LOW);
            HAL_Delay(GNSS_POWER_ON_DELAY);

            if(RETCODE_OK == retcode)
            {
                ModuleState_GNSS_g7020 = BSP_MODULE_STATE_ENABLED;
            }
            else
            {
                ModuleState_GNSS_g7020 = BSP_MODULE_STATE_ERROR;
            }
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

Retcode_T BSP_GNSS_G7020_Disable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    uint32_t start = 0;
    uint32_t timeout = 0;

    switch(ModuleState_GNSS_g7020)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {
            /* Disable the UART MCU resource */
            /* Disable current transmission */
            CLEAR_BIT(GNSS_SERIAL_PORT->CR1, USART_CR1_TE);
            CLEAR_BIT(GNSS_SERIAL_PORT->CR1, USART_CR1_RE);
            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_DISABLE_IT(&BSP_LPUART1Handle.huart, UART_IT_RXNE);
            /*lint -e{506} The function is provided by STM32Cube library */
            __HAL_UART_DISABLE_IT(&BSP_LPUART1Handle.huart, UART_IT_TXE);
            start = HAL_GetTick();
            /* Wait until transmission complete */
            while (0 == READ_BIT(GNSS_SERIAL_PORT->ISR, USART_ISR_TC) && (GNSS_TRANSMIT_DISABLE_TIMEOUT >= timeout))
            {
                timeout = HAL_GetTick() - start;
            }
            /* Check if transmission timeout occured */
            if ((GNSS_TRANSMIT_DISABLE_TIMEOUT <= timeout) && (0 == READ_BIT(GNSS_SERIAL_PORT->ISR, USART_ISR_TC)))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            else
            {
                /* Disable the UART module */
                __HAL_UART_DISABLE(&BSP_LPUART1Handle.huart);
                /* Disable the clock for the UART Resource */
                GNSS_UART_CLOCK_DISABLE();
                /* Disable the Interrupt requests */
                HAL_NVIC_DisableIRQ(GNSS_UART_IRQn);
                /* Clear the pending interrupt */
                HAL_NVIC_ClearPendingIRQ(GNSS_UART_IRQn);
                /* Disable 3v3 Power rail if not used by other peripheral */
                HAL_GPIO_WritePin(GNSS_EN_PORT, GNSS_EN_PIN, BSP_IO_PIN_HIGH);
                /* Disable LSCO output */
                HAL_RCCEx_DisableLSCO();
            }
            if(RETCODE_OK == retcode)
            {
                ModuleState_GNSS_g7020 = BSP_MODULE_STATE_CONNECTED;
                /* Disable the power rail if not used */
                retcode = BSP_LDO_TLV70233_Disable();
            }
            if(RETCODE_OK != retcode)
            {
                ModuleState_GNSS_g7020 = BSP_MODULE_STATE_ERROR;
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

Retcode_T BSP_GNSS_G7020_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_GNSS_g7020)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /*
         * Enable all GPIO ports necessary for the operation to make sure all the
         * commands have been executed
         */
        GNSS_GPIO_CLOCK_ENABLE();

        /* GNSS_PWR_EN disconnect */
        HAL_GPIO_DeInit(GNSS_CLK32_PORT, GNSS_CLK32_PIN);
        /* LPUART1 disconnect */
        HAL_GPIO_DeInit(GNSS_UART_TXD_PORT, GNSS_UART_TXD_PIN);
        HAL_GPIO_DeInit(GNSS_UART_RXD_PORT, GNSS_UART_RXD_PIN);

        /* GNSS_EN disconnect */
        HAL_GPIO_DeInit(GNSS_EN_PORT, GNSS_EN_PIN);

        /* GNSS_RST disconnect */
        HAL_GPIO_DeInit(GNSS_RESETN_PORT, GNSS_RESETN_PIN);
        ModuleState_GNSS_g7020 = BSP_MODULE_STATE_DISCONNECTED;
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

Retcode_T BSP_GNSS_G7020_Reset(void)
{
    BSP_MODULE_TRY_LOCK;
    HAL_GPIO_WritePin(GNSS_RESETN_PORT, GNSS_RESETN_PIN, BSP_IO_PIN_HIGH);
    HAL_Delay(GNSS_RESET_PULL_DELAY);
    HAL_GPIO_WritePin(GNSS_RESETN_PORT, GNSS_RESETN_PIN, BSP_IO_PIN_LOW);
    HAL_Delay(GNSS_RESET_PROCESSING_DELAY);
    BSP_MODULE_UNLOCK;

    return RETCODE_OK;
}

HWHandle_T BSP_GNSS_G7020_GetUARTHandle(void)
{
    return (HWHandle_T) &BSP_LPUART1Handle;
}

void GNSS_G7020_UARTISR(void)
{
    BSP_LPUART1Handle.IrqCallback((UART_T) &BSP_LPUART1Handle);
}

#endif /* BCDS_FEATURE_BSP_GNSS_7020 */
/**@}*//* @ingroup BSP_GNSS_MODULE */
