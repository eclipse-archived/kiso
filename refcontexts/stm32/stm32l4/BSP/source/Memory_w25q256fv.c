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
 * @ingroup BSP_MEMORY_MODULE
 * @{
 * @file
 * @brief  Implementation of SPI External memory Winbond(R) W25Q256FV BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_MEM_W25Q256FV

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_Memory_W25Q256FV.h"

#if BCDS_FEATURE_BSP_MEMORY_W25Q256FV

#include "Memory_w25q256fv.h"
#include "BoardSettings.h"
#include "LDO_tlv70233.h"
#include "BSP_BoardType.h"
#include "BCDS_MCU_SPI_Handle.h"
#include "BSP_ModuleState.h"

/* Static Functions and variable declaration */

enum GPIOLock_E
{
    GPIO_LOCKED = 0,
    GPIO_UNLOCKED,
};

typedef enum GPIOLock_E GPIOLock_T;

static GPIOLock_T Mem_GPIOLock = GPIO_LOCKED;
static DMA_HandleTypeDef BSP_SPI1DMATxHandle;
static DMA_HandleTypeDef BSP_SPI1DMARxHandle;
static struct MCU_SPI_S BSP_SPI1Handle;
static volatile enum BSP_ModuleState_E ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_UNINITIALIZED;
static volatile uint8_t EnableCount;

enum BSP_ModuleState_E Mem_W25Q256FV_getState(void)
{
    return ModuleState_Mem_W25Q256FV;
}

/*
 * @brief Initializes the SPI interface
 * @details This function Initializes the SPI resource. It is called within the
 * Board_Initialize() function. It stores the desired configuration into the
 * resource registers and puts it into low power consumption configuration until
 *  it is requested by the application.
 * @return RETCODE_OK in case of success error code otherwise.

 */
Retcode_T Board_SPI1_Init(void);

Retcode_T Board_SPI1_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* Enable the clock for the DMA resource */
    __HAL_RCC_DMA1_CLK_ENABLE();

    BSP_SPI1Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_DMA;

    /* Configure the DMA handler for Transmission process */
    BSP_SPI1DMATxHandle.Instance = MEM_DMA_TX_CHANNEL;
    BSP_SPI1DMATxHandle.Init.Request = MEM_DMA_TX_REQUEST;
    BSP_SPI1DMATxHandle.Init.Direction = MEM_DMA_TX_DIRECTION;
    BSP_SPI1DMATxHandle.Init.PeriphInc = MEM_DMA_TX_PINC;
    BSP_SPI1DMATxHandle.Init.MemInc = MEM_DMA_TX_MINC;
    BSP_SPI1DMATxHandle.Init.PeriphDataAlignment = MEM_DMA_TX_PDATA_ALIGN;
    BSP_SPI1DMATxHandle.Init.MemDataAlignment = MEM_DMA_TX_MDATA_ALIGN;
    BSP_SPI1DMATxHandle.Init.Mode = MEM_DMA_TX_MODE;
    BSP_SPI1DMATxHandle.Init.Priority = MEM_DMA_TX_PRIORITY;

    if (HAL_OK != HAL_DMA_Init(&BSP_SPI1DMATxHandle))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    else
    {
        /* Associate the initialized DMA handle to the the SPI handle */
        __HAL_LINKDMA(&(BSP_SPI1Handle.hspi), hdmatx, BSP_SPI1DMATxHandle);
    }
    if (RETCODE_OK == retcode)
    {
        /* Configure the DMA handler for Transmission process */
        BSP_SPI1DMARxHandle.Instance = MEM_DMA_RX_CHANNEL;

        BSP_SPI1DMARxHandle.Init.Request = MEM_DMA_RX_REQUEST;
        BSP_SPI1DMARxHandle.Init.Direction = MEM_DMA_RX_DIRECTION;
        BSP_SPI1DMARxHandle.Init.PeriphInc = MEM_DMA_RX_PINC;
        BSP_SPI1DMARxHandle.Init.MemInc = MEM_DMA_RX_MINC;
        BSP_SPI1DMARxHandle.Init.PeriphDataAlignment = MEM_DMA_RX_PDATA_ALIGN;
        BSP_SPI1DMARxHandle.Init.MemDataAlignment = MEM_DMA_RX_MDATA_ALIGN;
        BSP_SPI1DMARxHandle.Init.Mode = MEM_DMA_RX_MODE;
        BSP_SPI1DMARxHandle.Init.Priority = MEM_DMA_RX_PRIORITY;

        if (HAL_OK != HAL_DMA_Init(&BSP_SPI1DMARxHandle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
        else
        {
            /* Associate the initialized DMA handle to the the SPI handle */
            __HAL_LINKDMA(&(BSP_SPI1Handle.hspi), hdmarx, BSP_SPI1DMARxHandle);
        }
    }
    if (RETCODE_OK == retcode)
    {
        __HAL_RCC_SPI1_CLK_ENABLE();

        BSP_SPI1Handle.hspi.Instance = MEM_SPI_INSTANCE;
        BSP_SPI1Handle.hspi.Init.BaudRatePrescaler = MEM_SPI_BR_PRESCALER;
        BSP_SPI1Handle.hspi.Init.Direction = MEM_SPI_DIRECTION;
        BSP_SPI1Handle.hspi.Init.CLKPhase = MEM_SPI_CLOCK_PHASE;
        BSP_SPI1Handle.hspi.Init.CLKPolarity = MEM_SPI_POLARITY;
        BSP_SPI1Handle.hspi.Init.DataSize = MEM_SPI_DATASIZE;
        BSP_SPI1Handle.hspi.Init.FirstBit = MEM_SPI_FIRST_BIT;
        BSP_SPI1Handle.hspi.Init.TIMode = MEM_SPI_TI_MODE;
        BSP_SPI1Handle.hspi.Init.CRCCalculation = MEM_SPI_CRC;
        BSP_SPI1Handle.hspi.Init.CRCPolynomial = MEM_SPI_CRC_POL;
        BSP_SPI1Handle.hspi.Init.CRCLength = MEM_SPI_CRC_LENGTH;
        BSP_SPI1Handle.hspi.Init.NSS = MEM_SPI_NSS;
        BSP_SPI1Handle.hspi.Init.NSSPMode = MEM_SPI_NSS_MODE;
        BSP_SPI1Handle.hspi.Init.Mode = MEM_SPI_MODE;
        if (HAL_OK != HAL_SPI_Init(&(BSP_SPI1Handle.hspi)))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    __HAL_SPI_DISABLE(&(BSP_SPI1Handle.hspi));
    __HAL_RCC_SPI1_CLK_DISABLE();

    if (RETCODE_OK == retcode)
    {
        ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_DISCONNECTED;
    }
    return retcode;
}

Retcode_T BSP_Mem_W25Q256FV_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    GPIO_InitTypeDef GPIO_InitStruct;

    switch(ModuleState_Mem_W25Q256FV)
    {
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH: */
    case BSP_MODULE_STATE_DISCONNECTED:
        /* Enabling GPIO clocks required by the memory module */
        MEM_GPIO_CLOCK_ENABLE();

        /* MEM_CS_N pin is configured in _Enable() */

        /* Configure HOLD pin */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = MEM_HOLD_N_PIN;
        GPIO_InitStruct.Mode = MEM_HOLD_N_MODE;
        GPIO_InitStruct.Pull = MEM_HOLD_N_PULL;
        GPIO_InitStruct.Speed = MEM_HOLD_N_SPEED;
        HAL_GPIO_Init(MEM_HOLD_N_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(MEM_HOLD_N_PORT, MEM_HOLD_N_PIN, GPIO_PIN_RESET);

        /* Configure write protection pin  pin */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = MEM_W_N_PIN;
        GPIO_InitStruct.Mode = MEM_W_N_MODE;
        GPIO_InitStruct.Pull = MEM_W_N_PULL;
        GPIO_InitStruct.Speed = MEM_W_N_SPEED;
        HAL_GPIO_Init(MEM_W_N_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(MEM_W_N_PORT, MEM_W_N_PIN, GPIO_PIN_RESET);

        /* Configure serial input pin */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = MEM_MOSI_PIN;
        GPIO_InitStruct.Mode = MEM_MOSI_MODE;
        GPIO_InitStruct.Pull = MEM_MOSI_PULL;
        GPIO_InitStruct.Speed = MEM_MOSI_SPEED;
        GPIO_InitStruct.Alternate = MEM_MOSI_ALTERNATE;
        HAL_GPIO_Init(MEM_MOSI_PORT, &GPIO_InitStruct);

        /* Configure serial output pin */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = MEM_MISO_PIN;
        GPIO_InitStruct.Mode = MEM_MISO_MODE;
        GPIO_InitStruct.Pull = MEM_MISO_PULL;
        GPIO_InitStruct.Speed = MEM_MISO_SPEED;
        GPIO_InitStruct.Alternate = MEM_MISO_ALTERNATE;
        HAL_GPIO_Init(MEM_MISO_PORT, &GPIO_InitStruct);

        /* Configure serial clock pin */
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = MEM_SCK_PIN;
        GPIO_InitStruct.Mode = MEM_SCK_MODE;
        GPIO_InitStruct.Pull = MEM_SCK_PULL;
        GPIO_InitStruct.Speed = MEM_SCK_SPEED;
        GPIO_InitStruct.Alternate = MEM_SCK_ALTERNATE;
        HAL_GPIO_Init(MEM_SCK_PORT, &GPIO_InitStruct);

        EnableCount = 0;
        retcode = RETCODE_OK;
        ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_CONNECTED;
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

Retcode_T BSP_Mem_W25Q256FV_Enable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode= RETCODE_OK;
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Dont enable if the callback has not been registered */
    assert(NULL != BSP_SPI1Handle.DmaRxCallback);
    assert(NULL != BSP_SPI1Handle.DmaTxCallback);

    switch(ModuleState_Mem_W25Q256FV)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = BSP_LDO_TLV70233_Enable();
        if (RETCODE_OK == retcode)
        {
            /* Enabling GPIO clocks required by the memory module */
            MEM_GPIO_CLOCK_ENABLE();

            /* Configure CS pin */
            memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
            GPIO_InitStruct.Pin = MEM_CS_N_PIN;
            GPIO_InitStruct.Mode = MEM_CS_N_MODE;
            GPIO_InitStruct.Pull = MEM_CS_N_PULL;
            GPIO_InitStruct.Speed = MEM_CS_N_SPEED;
            HAL_GPIO_Init(MEM_CS_N_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(MEM_CS_N_PORT,MEM_CS_N_PIN,GPIO_PIN_SET);

            /* Enable the clocks */
            __HAL_RCC_SPI1_CLK_ENABLE();
            /* Enable the SPI resource */
            __HAL_SPI_ENABLE(&(BSP_SPI1Handle.hspi));

            HAL_Delay(MEM_DELAY_BEFORE_WRITE_MS);

            /* NVIC configuration for DMA transfer complete interrupt (SPI1_TX) */
            HAL_NVIC_SetPriority(MEM_TX_DMA_IRQ, MEM_TX_DMA_PREEMPT_PRIORITY, MEM_TX_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(MEM_TX_DMA_IRQ);

            /* NVIC configuration for DMA transfer complete interrupt (SPI1_RX) */
            HAL_NVIC_SetPriority(MEM_RX_DMA_IRQ, MEM_RX_DMA_PREEMPT_PRIORITY,MEM_RX_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(MEM_RX_DMA_IRQ);

            Mem_GPIOLock = GPIO_UNLOCKED;

            ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_ENABLED;

        }
        else
        {
            ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_ERROR;
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

Retcode_T BSP_Mem_W25Q256FV_Disable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    uint32_t timeout = 0;
    uint32_t start = 0;


    switch(ModuleState_Mem_W25Q256FV)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {
            start = HAL_GetTick();
            while ((__HAL_SPI_GET_FLAG(&(BSP_SPI1Handle.hspi),SPI_FLAG_FTLVL))&&(timeout < MEM_SPI_TRANSMIT_END_TIMEOUT_MS)&&(__HAL_SPI_GET_FLAG(&(BSP_SPI1Handle.hspi),SPI_FLAG_BSY)))
            {
                timeout = HAL_GetTick() - start;
            }
            if (timeout <= MEM_SPI_TRANSMIT_END_TIMEOUT_MS)
            {
                __HAL_SPI_DISABLE(&(BSP_SPI1Handle.hspi));
                start = HAL_GetTick();
                while ((__HAL_SPI_GET_FLAG(&(BSP_SPI1Handle.hspi),SPI_FLAG_FRLVL))&&(timeout < MEM_SPI_RECEIVE_END_TIMEOUT_MS))
                {
                    timeout = HAL_GetTick() - start;
                }
                if (timeout <= MEM_SPI_RECEIVE_END_TIMEOUT_MS)
                {
                    HAL_NVIC_DisableIRQ(DMA1_Channel3_IRQn);
                    /* */
                    HAL_NVIC_ClearPendingIRQ(DMA1_Channel3_IRQn);
                    /*
                     * Disable the clocks
                     */
                    __HAL_RCC_SPI1_CLK_DISABLE();
                }
                else
                {
                    ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_ERROR;
                    retcode= RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE);
                }
            }
            else
            {
                ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_ERROR;
                retcode= RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE);
            }

            if (RETCODE_OK == retcode)
            {
                HAL_GPIO_WritePin(MEM_CS_N_PORT,MEM_CS_N_PIN,GPIO_PIN_SET);
                HAL_Delay(MEM_DISABLE_DELAY_MS);
                Mem_GPIOLock = GPIO_LOCKED;

                HAL_GPIO_WritePin(MEM_HOLD_N_PORT, MEM_HOLD_N_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MEM_W_N_PORT, MEM_W_N_PIN, GPIO_PIN_RESET);
                /* deinit of the CS PIn to avoid power consumption if set to low and cross power of the component through the Pull-up R4601 if 3v3 is kept on*/
                HAL_GPIO_DeInit(MEM_CS_N_PORT, MEM_CS_N_PIN);

                ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_CONNECTED;

                retcode = BSP_LDO_TLV70233_Disable();
            }

            if (RETCODE_OK != retcode)
            {
                ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_ERROR;
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

Retcode_T BSP_Mem_W25Q256FV_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;

    switch(ModuleState_Mem_W25Q256FV)
    {
    case BSP_MODULE_STATE_CONNECTED:
        HAL_GPIO_DeInit(MEM_HOLD_N_PORT, MEM_HOLD_N_PIN);

        HAL_GPIO_DeInit(MEM_W_N_PORT, MEM_W_N_PIN);

        HAL_GPIO_DeInit(MEM_MOSI_PORT, MEM_MOSI_PIN);

        HAL_GPIO_DeInit(MEM_MISO_PORT, MEM_MISO_PIN);

        HAL_GPIO_DeInit(MEM_SCK_PORT, MEM_SCK_PIN);

        ModuleState_Mem_W25Q256FV = BSP_MODULE_STATE_DISCONNECTED;
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

HWHandle_T BSP_Mem_W25Q256FV_GetSPIHandle(void)
{
    return (HWHandle_T) &BSP_SPI1Handle;
}

Retcode_T BSP_Mem_W25Q256FV_SetCSHigh(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    if (Mem_GPIOLock == GPIO_UNLOCKED)
    {
        HAL_GPIO_WritePin(MEM_CS_N_PORT,MEM_CS_N_PIN,GPIO_PIN_SET);
        retcode = RETCODE_OK;
    }
    else
    {
        retcode= RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Mem_W25Q256FV_SetCSLow(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    if (Mem_GPIOLock == GPIO_UNLOCKED)
    {
        HAL_GPIO_WritePin(MEM_CS_N_PORT,MEM_CS_N_PIN,GPIO_PIN_RESET);
        retcode = RETCODE_OK;
    }
    else
    {
        retcode= RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Mem_W25Q256FV_SetHoldHigh(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    if (Mem_GPIOLock == GPIO_UNLOCKED)
    {
        HAL_GPIO_WritePin(MEM_HOLD_N_PORT,MEM_HOLD_N_PIN,GPIO_PIN_SET);
        retcode = RETCODE_OK;
    }
    else
    {
        retcode= RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Mem_W25Q256FV_SetHoldLow(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    if (Mem_GPIOLock == GPIO_UNLOCKED)
    {
        HAL_GPIO_WritePin(MEM_HOLD_N_PORT,MEM_HOLD_N_PIN,GPIO_PIN_RESET);
        retcode = RETCODE_OK;
    }
    else
    {
        retcode= RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Mem_W25Q256FV_SetWPHigh(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    if (Mem_GPIOLock == GPIO_UNLOCKED)
    {
        HAL_GPIO_WritePin(MEM_W_N_PORT,MEM_W_N_PIN,GPIO_PIN_SET);
        retcode = RETCODE_OK;
    }
    else
    {
        retcode= RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Mem_W25Q256FV_SetWPLow(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode;
    if (Mem_GPIOLock == GPIO_UNLOCKED)
    {
        HAL_GPIO_WritePin(MEM_W_N_PORT,MEM_W_N_PIN,GPIO_PIN_RESET);
        retcode = RETCODE_OK;
    }
    else
    {
        retcode= RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

void Mem_W25Q256FV_SPI_DMARxISR(void)
{
    BSP_SPI1Handle.DmaRxCallback((SPI_T) &BSP_SPI1Handle);
}

void Mem_W25Q256FV_SPI_DMATxISR(void)
{
    BSP_SPI1Handle.DmaTxCallback((SPI_T) &BSP_SPI1Handle);
}

#endif /* BCDS_FEATURE_BSP_MEMORY_W25Q256FV */
/**@}*//* @ingroup BSP_MEMORY_MODULE */
