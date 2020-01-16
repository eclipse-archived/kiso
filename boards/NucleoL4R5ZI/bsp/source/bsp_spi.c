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

#include "Kiso_HAL.h"

#if KISO_FEATURE_SPI

#include "stm32/stm32l4/Kiso_MCU_STM32L4_SPI_Handle.h"
#include "BSP_NucleoL4R5ZI.h"
#include "protected/spi.h"
#include "protected/gpio.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_SPI

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void SPIx_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/******************************* SPI ********************************/

/**
 * Variable storing SPI control block for the sensors.
 */
struct MCU_SPI_S nucleoSpiStruct =
{
	.TransferMode = KISO_HAL_TRANSFER_MODE_INTERRUPT,
	/* SPI Config */
	.hspi.Instance = SPIx,
	/* SPI baudrate is set to 9 MHz maximum (APB1/SPI_BaudRatePrescaler = 72/8 = 9 MHz)
			  to verify these constraints:
				 - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
				   Since the provided driver doesn't use read capability from LCD, only constraint
				   on write baudrate is considered.
				 - SD card SPI interface max baudrate is 25MHz for write/read
				 - PCLK2 max frequency is 72 MHz
	 */
	.hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2,
	.hspi.Init.Direction         = SPI_DIRECTION_2LINES,
	.hspi.Init.CLKPhase          = SPI_PHASE_1EDGE,
	.hspi.Init.CLKPolarity       = SPI_POLARITY_LOW,
	.hspi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE,
	.hspi.Init.CRCPolynomial     = 7,
	.hspi.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE,
	.hspi.Init.DataSize          = SPI_DATASIZE_8BIT,
	.hspi.Init.FirstBit          = SPI_FIRSTBIT_MSB,
	.hspi.Init.NSS               = SPI_NSS_SOFT,
	.hspi.Init.NSSPMode          = SPI_NSS_PULSE_ENABLE,
	.hspi.Init.TIMode            = SPI_TIMODE_DISABLE,
	.hspi.Init.Mode              = SPI_MODE_MASTER,
};

uint8_t spiConnectedState = 0;
uint8_t spiEnabledState = 0;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 * @retval RETCODE_OK
 */
Retcode_T SPI_Connect(enum BSP_SPI_Devices id)
{
    GPIO_InitTypeDef  GPIO_InitStruct = { 0 };

    /*** Configure the GPIOs ***/
    if (0 == spiConnectedState)
    {
        /*##### -1- Enable peripherals and GPIO Clocks #####*/
        /* Enable GPIO TX/RX clock */
        SPIx_SCK_GPIO_CLK_ENABLE();
        SPIx_CS_GPIO_CLK_ENABLE();

        /* Enable SPI clock */
        SPIx_CLK_ENABLE();
        SPIx_FORCE_RESET();
        SPIx_RELEASE_RESET();

        /*##### -2- Configure peripheral GPIO #####*/
        /* SPI GPIO pin configuration  */
        GPIO_OpenClockGate(SPIx_SCK_PORT_SELECT, SPIx_SCK_PIN | SPIx_MISO_PIN | SPIx_MOSI_PIN);
        GPIO_InitStruct.Pin       = SPIx_SCK_PIN | SPIx_MISO_PIN | SPIx_MOSI_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = SPIx_SCK_AF;
        HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

        /* SPI CS GPIO pin configuration  */
        GPIO_OpenClockGate(SPIx_CS_PORT_SELECT, SPIx_CS_PIN);
        GPIO_InitStruct.Pin = SPIx_CS_PIN;
        HAL_GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct);

        /*##### -3- Configure the NVIC for SPI #####*/
        /* NVIC for SPI */
        HAL_NVIC_SetPriority(SPIx_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(SPIx_IRQn);
    }
    spiConnectedState |= 1 << id;

    return RETCODE_OK;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success
 * @retval RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SPI_INIT_FAILED) in case SPI initialize failed.
 */
Retcode_T SPI_Enable(enum BSP_SPI_Devices id)
{
    Retcode_T retcode = RETCODE_OK;

    if (0 == spiEnabledState)
    {
        /* Configure the SPI resource */
        if (HAL_OK != HAL_SPI_Init(&nucleoSpiStruct.hspi))
        {
             retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SPI_INIT_FAILED);
        }
    }
    spiEnabledState |= 1 << id;

    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success
 * @retval RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SPI_DEINIT_FAILED) in case SPI deinitialize failed.
 */
Retcode_T SPI_Disable(enum BSP_SPI_Devices id)
{
    Retcode_T retcode = RETCODE_OK;

    spiEnabledState &= ~(1 << id);
    if (0 == spiEnabledState)
    {
        if (HAL_OK != HAL_SPI_DeInit(&nucleoSpiStruct.hspi))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SPI_DEINIT_FAILED);
        }
        if (RETCODE_OK == retcode)
        {
            __HAL_RCC_SPI1_CLK_DISABLE();
        }
    }

    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success
 */
Retcode_T SPI_Disconnect(enum BSP_SPI_Devices id)
{
    spiConnectedState &= ~(1 << id);
    if (0 == spiConnectedState)
    {
        /*#####-1- Reset peripherals #####*/
        SPIx_FORCE_RESET();
        SPIx_RELEASE_RESET();

        /*#####-2- Disable peripherals and GPIO Clocks #####*/
        /* Deconfigure SPI */
        HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN | SPIx_MISO_PIN | SPIx_MOSI_PIN);
        GPIO_CloseClockGate(SPIx_SCK_PORT_SELECT, SPIx_SCK_PIN | SPIx_MISO_PIN | SPIx_MOSI_PIN);

        HAL_GPIO_DeInit(SPIx_CS_GPIO_PORT, SPIx_CS_PIN);
        GPIO_CloseClockGate(SPIx_CS_PORT_SELECT, SPIx_CS_PIN);

        /*#####-3- Disable the NVIC for SPI #####*/
        HAL_NVIC_DisableIRQ(SPIx_IRQn);
    }

    return RETCODE_OK;
}

/**
 * See API interface for function documentation
 * @return A pointer to the SPI control structure
 */
HWHandle_T BSP_TestInterface_GetSPIHandle(void)
{
    return (HWHandle_T) &nucleoSpiStruct;
}


/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt Service Routine handling SPI_EV IRQ. Forwards call to MCU Layer for handling.
 */
void SPIx_IRQHandler(void)
{
    if (NULL != nucleoSpiStruct.IRQCallback)
    {
        nucleoSpiStruct.IRQCallback((SPI_T) &nucleoSpiStruct);
    }
}

#endif
