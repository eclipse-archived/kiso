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

#include "Kiso_BSP_Memory_W25.h"

#if KISO_FEATURE_BSP_MEMORY_W25

#include "BSP_CommonGateway.h"
#include "protected/gpio.h"
#include "protected/power_supply.h"
#include "stm32/stm32l4/Kiso_MCU_STM32L4_SPI_Handle.h"
#include "Kiso_HAL_Delay.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_API_MEMORY_W25

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void DMA1_Channel2_IRQHandler(void);

void DMA1_Channel3_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/**
 * BSP State of the BLE module
 */
static uint8_t bspState = (uint8_t)BSP_STATE_INIT;

/**
 * Static structure storing SPI handle for serial flash memory.
 */
static struct MCU_SPI_S W25_SpiHandle =
    {
        .TransferMode = KISO_HAL_TRANSFER_MODE_DMA,
        .hspi.Instance = SPI1,
        .hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4,
        .hspi.Init.Direction = SPI_DIRECTION_2LINES,
        .hspi.Init.CLKPhase = SPI_PHASE_1EDGE,
        .hspi.Init.CLKPolarity = SPI_POLARITY_LOW,
        .hspi.Init.DataSize = SPI_DATASIZE_8BIT,
        .hspi.Init.FirstBit = SPI_FIRSTBIT_MSB,
        .hspi.Init.TIMode = SPI_TIMODE_DISABLE,
        .hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
        .hspi.Init.CRCPolynomial = 7,
        .hspi.Init.CRCLength = SPI_CRC_LENGTH_8BIT,
        .hspi.Init.NSS = SPI_NSS_SOFT,
        .hspi.Init.NSSPMode = SPI_NSS_PULSE_DISABLE,
        .hspi.Init.Mode = SPI_MODE_MASTER,
};

static DMA_HandleTypeDef W25_DMATxHandle =
    {
        .Instance = DMA1_Channel3,
        .Init.Request = DMA_REQUEST_1,
        .Init.Direction = DMA_MEMORY_TO_PERIPH,
        .Init.PeriphInc = DMA_PINC_DISABLE,
        .Init.MemInc = DMA_MINC_ENABLE,
        .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
        .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
        .Init.Mode = DMA_NORMAL,
        .Init.Priority = DMA_PRIORITY_LOW,
};

static DMA_HandleTypeDef W25_DMARxHandle =
    {
        .Instance = DMA1_Channel2,
        .Init.Request = DMA_REQUEST_1,
        .Init.Direction = DMA_PERIPH_TO_MEMORY,
        .Init.PeriphInc = DMA_PINC_DISABLE,
        .Init.MemInc = DMA_MINC_ENABLE,
        .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
        .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
        .Init.Mode = DMA_NORMAL,
        .Init.Priority = DMA_PRIORITY_HIGH,
};

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow connecting.
 */
Retcode_T BSP_Memory_W25_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_InitTypeDef BSP_GPIOInitStruct = {0};

        GPIO_OpenClockGate(GPIO_PORT_E, PINE_MEM_WP | PINE_MEM_HOLD | PINE_MEM_CS | PINE_MEM_SCK | PINE_MEM_MISO | PINE_MEM_MOSI);
        /* Configure output push pull pins */
        BSP_GPIOInitStruct.Pin = PINE_MEM_WP | PINE_MEM_HOLD | PINE_MEM_CS;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        BSP_GPIOInitStruct.Pull = GPIO_PULLUP;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(GPIOB, &BSP_GPIOInitStruct);

        /* Configure output push pull pins */
        BSP_GPIOInitStruct.Pin = PINE_MEM_WP | PINE_MEM_HOLD | PINE_MEM_CS;
        BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
        BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        BSP_GPIOInitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &BSP_GPIOInitStruct);

        bspState = (uint8_t)BSP_STATE_CONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow enabling.
 */
Retcode_T BSP_Memory_W25_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        /* The schematic contains a bug where CS is not pulled-up to VCC */
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_CS, GPIO_PIN_SET);
        retcode = PowerSupply_EnablePower2V8Memory();
    }
    if (RETCODE_OK == retcode)
    {
        /* Enable the clock for the DMA resource */
        __HAL_RCC_DMA1_CLK_ENABLE();
        /* Configure the DMA handler for Transmission process */
        if (HAL_OK != HAL_DMA_Init(&W25_DMATxHandle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        /* Associate the initialized DMA handle to the the SPI handle */
        __HAL_LINKDMA(&(W25_SpiHandle.hspi), hdmatx, W25_DMATxHandle);

        /* Configure the DMA handler for Transmission process */
        if (HAL_OK != HAL_DMA_Init(&W25_DMARxHandle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        /* Associate the initialized DMA handle to the the SPI handle */
        __HAL_LINKDMA(&(W25_SpiHandle.hspi), hdmarx, W25_DMARxHandle);
        /* Configure SPI resource */
        __HAL_RCC_SPI1_CLK_ENABLE();
        if (HAL_OK != HAL_SPI_Init(&(W25_SpiHandle.hspi)))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        /* NVIC configuration for DMA transfer complete interrupt (SPI1_TX) */
        HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 7, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

        /* NVIC configuration for DMA transfer complete interrupt (SPI1_RX) */
        HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 6, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

        bspState = (uint8_t)BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 */
Retcode_T BSP_Memory_W25_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        HAL_SPI_StateTypeDef HalSpiState = W25_SpiHandle.hspi.State;
        if (HalSpiState == HAL_SPI_STATE_BUSY || HalSpiState == HAL_SPI_STATE_BUSY_TX || HalSpiState == HAL_SPI_STATE_BUSY_RX || HalSpiState == HAL_SPI_STATE_BUSY_TX_RX)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        __HAL_SPI_DISABLE(&(W25_SpiHandle.hspi));
        if (HAL_SPI_DeInit(&(W25_SpiHandle.hspi)) != HAL_OK)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SPI_DEINIT_FAILED);
        }
    }
    if (RETCODE_OK == retcode)
    {

        HAL_NVIC_DisableIRQ(DMA1_Channel3_IRQn);
        HAL_NVIC_ClearPendingIRQ(DMA1_Channel3_IRQn);
        HAL_NVIC_DisableIRQ(DMA1_Channel2_IRQn);
        HAL_NVIC_ClearPendingIRQ(DMA1_Channel2_IRQn);
        __HAL_RCC_SPI1_CLK_DISABLE();
        __HAL_RCC_DMA1_CLK_DISABLE();
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_CS, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_HOLD, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_WP, GPIO_PIN_RESET);
        retcode = PowerSupply_DisablePower2V8Memory();
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t)BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disconnecting.
 */
Retcode_T BSP_Memory_W25_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        HAL_GPIO_DeInit(GPIOB, PINE_MEM_WP | PINE_MEM_HOLD | PINE_MEM_CS | PINE_MEM_SCK | PINE_MEM_MISO | PINE_MEM_MOSI);
        GPIO_CloseClockGate(GPIO_PORT_E, PINE_MEM_WP | PINE_MEM_HOLD | PINE_MEM_CS | PINE_MEM_SCK | PINE_MEM_MISO | PINE_MEM_MOSI);

        bspState = (uint8_t)BSP_STATE_DISCONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @return A pointer to the I2C control structure
 */
HWHandle_T BSP_Memory_W25_GetHandle(void)
{
    return (HWHandle_T)&W25_SpiHandle;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the state prevents operation.
 */
Retcode_T BSP_Memory_W25_SetCSHigh(void)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    if (bspState == BSP_STATE_CONNECTED || bspState == BSP_STATE_ENABLED)
    {
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_CS, GPIO_PIN_SET);
        retcode = RETCODE_OK;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the state prevents operation.
 */
Retcode_T BSP_Memory_W25_SetCSLow(void)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    if (bspState == BSP_STATE_CONNECTED || bspState == BSP_STATE_ENABLED)
    {
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_CS, GPIO_PIN_RESET);
        retcode = RETCODE_OK;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the state prevents operation.
 */
Retcode_T BSP_Memory_W25_SetHoldHigh(void)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    if (bspState == BSP_STATE_CONNECTED || bspState == BSP_STATE_ENABLED)
    {
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_HOLD, GPIO_PIN_SET);
        retcode = RETCODE_OK;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the state prevents operation.
 */
Retcode_T BSP_Memory_W25_SetHoldLow(void)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    if (bspState == BSP_STATE_CONNECTED || bspState == BSP_STATE_ENABLED)
    {
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_HOLD, GPIO_PIN_RESET);
        retcode = RETCODE_OK;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the state prevents operation.
 */
Retcode_T BSP_Memory_W25_SetWPHigh(void)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    if (bspState == BSP_STATE_CONNECTED || bspState == BSP_STATE_ENABLED)
    {
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_WP, GPIO_PIN_SET);
        retcode = RETCODE_OK;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the state prevents operation.
 */
Retcode_T BSP_Memory_W25_SetWPLow(void)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    if (bspState == BSP_STATE_CONNECTED || bspState == BSP_STATE_ENABLED)
    {
        HAL_GPIO_WritePin(GPIOE, PINE_MEM_WP, GPIO_PIN_RESET);
        retcode = RETCODE_OK;
    }
    return retcode;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt Service Routine handling DMA1 Channel2 IRQ. Forwards call to MCU Layer for handling.
 */
void DMA1_Channel2_IRQHandler(void)
{
    if (W25_SpiHandle.DmaRxCallback)
    {
        W25_SpiHandle.DmaRxCallback((SPI_T)&W25_SpiHandle);
    }
}

/**
 * Interrupt Service Routine handling DMA1 Channel3 IRQ. Forwards call to MCU Layer for handling.
 */
void DMA1_Channel3_IRQHandler(void)
{
    if (W25_SpiHandle.DmaTxCallback)
    {
        W25_SpiHandle.DmaTxCallback((SPI_T)&W25_SpiHandle);
    }
}

#endif /* KISO_FEATURE_BSP_MEMORY_W25 */
