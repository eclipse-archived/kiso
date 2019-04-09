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
 * @ingroup BSP_XDK110_LoRa_SX1272_MODULE
 * @{
 */
/**
 * @file
 * @brief  Implementation of LoRa BSP and HAL functions which supports SX1272 LoRa chip
 */

/* This is used to define the correct module ID for generation of module
 * error codes
 */
#include "BCDS_BSP.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_LORA_SX1272

#include "BCDS_HAL.h"

#if BCDS_FEATURE_BSP_LORA_SX1272
#include "BCDS_BSP_LoRa_SX1272.h"
#include "BSP_LoRa_SX1272.h"
#include "BSP_BoardSettings.h"
#include "BCDS_MCU_SPI_Handle.h"
#include "BCDS_MCU_GPIO_Handle.h"
#include "BSP_BoardShared.h"
#include "BCDS_MCU_Timer.h"
#include "BCDS_MCU_Timer_Handle.h"

struct MCU_SPI_Handle_S BSP_LoRa_SX1272_SPI_Handle;
static SPI_IRQ_Callback_T LoRa_SX1272_SPI_RxISRCallback;
static SPI_IRQ_Callback_T LoRa_SX1272_SPI_TxISRCallback;

/*
 * Refer to interface header for description
 */
Retcode_T BSP_LoRa_SX1272_Connect(void)
{
    /* pin setting for chip seSD cardcardcard */
    GPIO_PinModeSet(BSP_LoRa_RADIO_CS_PORT, BSP_LoRa_RADIO_CS_PIN, BSP_LoRa_RADIO_CS_MODE, BSP_LoRa_RADIO_CS_DOUT);

    /* MOSI configuration */
    GPIO_PinModeSet(BSP_LoRa_RADIO_MOSI_PORT, BSP_LoRa_RADIO_MOSI_PIN, BSP_LoRa_RADIO_MOSI_MODE, BSP_LoRa_RADIO_MOSI_DOUT);

    /* MISO configuration */
    GPIO_PinModeSet(BSP_LoRa_RADIO_MISO_PORT, BSP_LoRa_RADIO_MISO_PIN, BSP_LoRa_RADIO_MISO_MODE, BSP_LoRa_RADIO_MISO_DOUT);

    /* Clock configuration */
    GPIO_PinModeSet(BSP_LoRa_RADIO_CLK_PORT, BSP_LoRa_RADIO_CLK_PIN, BSP_LoRa_RADIO_CLK_MODE, BSP_LoRa_RADIO_CLK_DOUT);

    MCU_GPIO_Handle_T pinConfig;
    pinConfig.Port = BSP_LoRa_RADIO_CS_PORT;
    pinConfig.Pin = BSP_LoRa_RADIO_CS_PIN;

    MCU_GPIO_WritePin((GPIO_T)&pinConfig, MCU_GPIO_PIN_STATE_HIGH);

    /* Set the routing for the WIFI SPI pins TX,RX and clock */
    /* Chip select (CS) pin will be controlled by software */
    CMU_ClockEnable(LORA_SPI_CLOCK, true);

    USART2_PORT_BASE->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_CLKPEN | LORA_ROUTE_LOCATION;

    CMU_ClockEnable(LORA_SPI_CLOCK, false);

    BSP_LoRa_SX1272_SPI_Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;
    BSP_LoRa_SX1272_SPI_Handle.TxDefaultData = 0xff;
    BSP_LoRa_SX1272_SPI_Handle.Instance = USART2_PORT_BASE;

    /* pin setting for External interrupt of Lora DIO*/
    GPIO_PinModeSet( BSP_LoRa_RADIO_DIO_0_PORT, BSP_LoRa_RADIO_DIO_0_PIN, BSP_LoRa_RADIO_DIO_0_MODE, BSP_LoRa_RADIO_DIO_0_DOUT );

    /* pin setting for External interrupt of Lora DIO*/
    GPIO_PinModeSet( BSP_LoRa_RADIO_DIO_1_PORT, BSP_LoRa_RADIO_DIO_1_PIN, BSP_LoRa_RADIO_DIO_1_MODE, BSP_LoRa_RADIO_DIO_1_DOUT );

    /* pin setting for External interrupt of Lora DIO*/
    GPIO_PinModeSet( BSP_LoRa_RADIO_DIO_2_PORT, BSP_LoRa_RADIO_DIO_2_PIN, BSP_LoRa_RADIO_DIO_2_MODE, BSP_LoRa_RADIO_DIO_2_DOUT );
	
	
    return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_LoRa_SX1272_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    /*enable the clock for the USART interface*/
    CMU_ClockEnable(LORA_SPI_CLOCK, true);
    USART2_PORT_BASE->CMD = USART_CMD_CLEARTX | USART_CMD_CLEARRX;
	
    /* Set interrupt priorities */
	NVIC_SetPriority(GPIO_ODD_IRQn, 5);
    NVIC_SetPriority(GPIO_EVEN_IRQn, 5);
    NVIC_SetPriority(LORA_TX_IRQN, LORA_TX_INTERRUPT_PRIORITY);
    NVIC_SetPriority(LORA_RX_IRQN, LORA_RX_INTERRUPT_PRIORITY);
	
    /* clear pending interrupts */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_ClearPendingIRQ(LORA_TX_IRQN);
    NVIC_ClearPendingIRQ(LORA_RX_IRQN);
    /* Assign the callback */

    LoRa_SX1272_SPI_RxISRCallback = BSP_LoRa_SX1272_SPI_Handle.IrqRxCallback;
    LoRa_SX1272_SPI_TxISRCallback = BSP_LoRa_SX1272_SPI_Handle.IrqTxCallback;

    NVIC_EnableIRQ(LORA_TX_IRQN);
    NVIC_EnableIRQ(LORA_RX_IRQN);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);

    USART2_PORT_BASE->CMD = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;

    return retcode;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_LoRa_SX1272_Disable(void)
{
	return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_LoRa_SX1272_EnableInterruptPin(enum BSP_LoRa_SX1272_IO_E pinName, BSP_LoRa_SX1272_InterruptCallback_T  callback)
{
	uint8_t risingedge = true;
	uint8_t fallingedge = false;
	if(pinName == BSP_LoRa_SX1272_IO0)
	{
		GPIO_IntConfig(BSP_LoRa_RADIO_DIO_0_PORT ,BSP_LoRa_RADIO_DIO_0_PIN ,risingedge, fallingedge,true);
		BoardCommon_GPIOInterruptRegisterCallback(BSP_LoRa_RADIO_DIO_0_PIN, callback);
	}
	else if(pinName == BSP_LoRa_SX1272_IO1)
	{
		GPIO_IntConfig(BSP_LoRa_RADIO_DIO_1_PORT ,BSP_LoRa_RADIO_DIO_1_PIN ,risingedge, fallingedge,true);
		BoardCommon_GPIOInterruptRegisterCallback(BSP_LoRa_RADIO_DIO_1_PIN, callback);
	}
	else if(pinName == BSP_LoRa_SX1272_IO2)
	{
		GPIO_IntConfig(BSP_LoRa_RADIO_DIO_2_PORT ,BSP_LoRa_RADIO_DIO_2_PIN ,risingedge, fallingedge,true);
		BoardCommon_GPIOInterruptRegisterCallback(BSP_LoRa_RADIO_DIO_2_PIN, callback);
	}
	else if(pinName == BSP_LoRa_SX1272_IO3)
	{
		GPIO_IntConfig(BSP_LoRa_RADIO_DIO_3_PORT ,BSP_LoRa_RADIO_DIO_3_PIN ,risingedge, fallingedge,true);
		BoardCommon_GPIOInterruptRegisterCallback(BSP_LoRa_RADIO_DIO_3_PIN, callback);
	}
	return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
HWHandle_T BSP_LoRa_SX1272_GetSPIHandle()
{
	return &BSP_LoRa_SX1272_SPI_Handle;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_LoRa_SX1272_Disconnect(void)
{
	return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
void LoRa_SX1272_SPIRxISR(void)
{
	if(NULL != BSP_LoRa_SX1272_SPI_Handle.IrqRxCallback)
	{
		/* All UART interrupt processing is to be done at the MCU level */
		BSP_LoRa_SX1272_SPI_Handle.IrqRxCallback((SPI_T) &BSP_LoRa_SX1272_SPI_Handle);
	}
}

/*
 * Refer to interface header for description
 */
void LoRa_SX1272_SPITxISR(void)
{
	if(NULL != BSP_LoRa_SX1272_SPI_Handle.IrqTxCallback)
	{
		/* All UART interrupt processing is to be done at the MCU level */
		BSP_LoRa_SX1272_SPI_Handle.IrqTxCallback((SPI_T) &BSP_LoRa_SX1272_SPI_Handle);
	}
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_LoRa_SX1272_SetCSLow(void)
{
	MCU_GPIO_Handle_T pinConfig;
    pinConfig.Port = BSP_LoRa_RADIO_CS_PORT;
    pinConfig.Pin = BSP_LoRa_RADIO_CS_PIN;
	
	MCU_GPIO_WritePin((GPIO_T)&pinConfig, MCU_GPIO_PIN_STATE_LOW);
	return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_LoRa_SX1272_SetCSHigh(void)
{
	MCU_GPIO_Handle_T pinConfig;
    pinConfig.Port = BSP_LoRa_RADIO_CS_PORT;
    pinConfig.Pin = BSP_LoRa_RADIO_CS_PIN;
	
	MCU_GPIO_WritePin((GPIO_T)&pinConfig, MCU_GPIO_PIN_STATE_HIGH);
	return RETCODE_OK;
}

#endif
