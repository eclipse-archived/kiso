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
 * @ingroup BSP_XDK110_SDCard
 * @{
 */
/**
 * @file
 * @brief  Implementation of SD_CARD BSP functions
 */

/* This is used to define the correct module ID for generation of module
 * error codes
 */

#include "BCDS_BSP.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_SD_CARD

#include "BCDS_HAL.h"
#if BCDS_FEATURE_BSP_SD_CARD

#include "BSP_BoardType.h"
#include "BSP_BoardSettings.h"
#include "BSP_BoardShared.h"
#include "BCDS_MCU_SPI_Handle.h"
#include "BSP_SDCard.h"
#include "BCDS_BSP_SD_Card.h"

#if BCDS_FEATURE_DMA
#include "BSP_DMA.h"
#include "BCDS_MCU_DMA_Handle.h"

static SPI_DMA_Callback_T SDCard_SPI_TxDmaCallback = NULL;
static SPI_DMA_Callback_T SDCard_SPI_RxDmaCallback = NULL;

struct MCU_DMA_Channel_S BSP_SD_CARD_SPI_TX_Channel;
struct MCU_DMA_Channel_S BSP_SD_CARD_SPI_RX_Channel;
#endif

/*
 * enumeration of custom return codes
 */
enum
{
    RETCODE_BSP_SD_CARD_SERIAL_NOT_INITIALIZED = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_BSP_SD_CARD_DISABLED,
    RETCODE_BSP_SD_CARD_ENABLE_NOT_COMPLETED,
    RETCODE_BSP_SD_CARD_DETECT_LINE_BUSY,
    RETCODE_BSP_SD_CARD_NOT_INSERTED,
    RETCODE_BSP_SD_CARD_CANNOT_BE_ENABLED,
    RETCODE_BSP_SD_CARD_SPI_ERROR,
};

struct MCU_SPI_Handle_S BSP_SDCard_SPI_Handle;

static SPI_IRQ_Callback_T SDCard_SPI_RxISRCallback;

static SPI_IRQ_Callback_T SDCard_SPI_TxISRCallback;

static BSP_SDCardAutoDetectCallback_T SDCard_DetectEvent;

static void SDCard_Detectinterrupt(void);
static void SDCardEnable(void);
static void SDCardDisable(void);

static bool SDCardDetectInterruptEnaled;
static bool SDCardEnabled = false;
/*
 * Refer to interface header for description
 */
Retcode_T BSP_SDCard_Connect(void)
{
    GPIO_PinModeSet(SD_CARD_CS_PORT, SD_CARD_CS_PIN, SD_CARD_CS_MODE, 1);
    GPIO_PinModeSet(SD_CARD_LS_PORT, SD_CARD_LS_PIN, SD_CARD_LS_MODE, 1);
    GPIO_PinModeSet(SD_DETECT_PORT, SD_DETECT_PIN, SD_DETECT_MODE, 1);
    GPIO_PinModeSet(SD_CARD_SPI1_MISO_PORT, SD_CARD_SPI1_MISO_PIN, SD_CARD_SPI1_MISO_MODE, 0);
    GPIO_PinModeSet(SD_CARD_SPI1_MOSI_PORT, SD_CARD_SPI1_MOSI_PIN, SD_CARD_SPI1_MOSI_MODE, 0);
    GPIO_PinModeSet(SD_CARD_SPI1_SCK_PORT, SD_CARD_SPI1_SCK_PIN, SD_CARD_SPI1_SCK_MODE, 0);

    CMU_ClockEnable(SD_CARD_SPI1_CLOCK, true);
    USART1_PORT_BASE->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_CLKPEN | USART_ROUTE_LOCATION_LOC1;
    CMU_ClockEnable(SD_CARD_SPI1_CLOCK, false);

#if BCDS_FEATURE_DMA
    BSP_SD_CARD_SPI_TX_Channel.ChannelId = SDCARD_SPI_TX_DMA_CHANNEL;
    BSP_SD_CARD_SPI_TX_Channel.Config.highPri = false;
    BSP_SD_CARD_SPI_TX_Channel.Config.enableInt = true;
    BSP_SD_CARD_SPI_TX_Channel.Config.select = DMAREQ_USART1_TXBL;
    BSP_SD_CARD_SPI_TX_Channel.CallBack.cbFunc = (DMA_FuncPtr_TypeDef) SDCard_SPI_TxDMAComplete;
    BSP_SD_CARD_SPI_TX_Channel.CallBack.userPtr = &BSP_SDCard_SPI_Handle;

    BSP_SD_CARD_SPI_RX_Channel.ChannelId = SDCARD_SPI_RX_DMA_CHANNEL;
    BSP_SD_CARD_SPI_RX_Channel.Config.highPri = false;
    BSP_SD_CARD_SPI_RX_Channel.Config.enableInt = true;
    BSP_SD_CARD_SPI_RX_Channel.Config.select = DMAREQ_USART1_RXDATAV;
    BSP_SD_CARD_SPI_RX_Channel.CallBack.cbFunc = (DMA_FuncPtr_TypeDef) SDCard_SPI_RxDMAComplete;
    BSP_SD_CARD_SPI_RX_Channel.CallBack.userPtr = &BSP_SDCard_SPI_Handle;

    BSP_SDCard_SPI_Handle.Link1 = (void*)&BSP_SD_CARD_SPI_TX_Channel;
    BSP_SDCard_SPI_Handle.Link2 = (void*)&BSP_SD_CARD_SPI_RX_Channel;

    BSP_SDCard_SPI_Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_DMA;

#else
    BSP_SDCard_SPI_Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;
#endif
    BSP_SDCard_SPI_Handle.TxDefaultData = 0xff;
    BSP_SDCard_SPI_Handle.Instance = USART1_PORT_BASE;
    return RETCODE_OK;
}

/*
 * Refer to prototype for description
 */
Retcode_T BSP_SDCard_Enable(BSP_SDCardAutoDetectCallback_T autoDetectCallback)
{
    /* Dont enable if the callback has not been registered */
    if (NULL == BSP_SDCard_SPI_Handle.IrqRxCallback || NULL == BSP_SDCard_SPI_Handle.IrqTxCallback)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SD_CARD_SERIAL_NOT_INITIALIZED);
    }
    /*lint -e506 wrong config results in runtime error reported*/
    if ((BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT) && (NULL == autoDetectCallback))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    /*lint -e506 wrong config results in runtime error reported*/
    if ((0 == BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT) && (BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT_FORCE))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if ((BSP_IO_PIN_LOW == BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT) && (BSP_IO_PIN_HIGH == GPIO_PinInGet(SD_DETECT_PORT, SD_DETECT_PIN)))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SD_CARD_NOT_INSERTED);
    }
    if (false == SDCardEnabled)
    {
        if (BSP_IO_PIN_LOW == GPIO_PinInGet(SD_DETECT_PORT, SD_DETECT_PIN))
        {
            SDCardEnable();
        }
    }
    /*lint -e506 wrong config results in runtime error reported*/
    if ((BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT))
    {
        /*lint -e506 wrong config results in runtime error reported*/
        if ((BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT_FORCE) || (NULL == BoardCommon_GPIOInterruptGetCallback(SD_DETECT_PIN)))
        {
            GPIO_IntDisable(0x01 << SD_DETECT_PIN);
            GPIO_IntClear(0x01 << SD_DETECT_PIN);
            SDCard_DetectEvent = autoDetectCallback;
            NVIC_DisableIRQ(GPIO_EVEN_IRQn);
            BoardCommon_GPIOInterruptRegisterCallback(SD_DETECT_PIN, SDCard_Detectinterrupt);
            GPIO_IntConfig(SD_DETECT_PORT, SD_DETECT_PIN, true, true, true);
            NVIC_EnableIRQ(GPIO_EVEN_IRQn);
            SDCardDetectInterruptEnaled = true;
			/* Triggered the callback at the initialization time to updated the sdcard status because there won't be any signal change detected if SDcard is already inserted */
            SDCard_Detectinterrupt();
        }
        else
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SD_CARD_DETECT_LINE_BUSY);
        }
    }
    return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_SDCard_Disable(void)
{
    if (SDCardDetectInterruptEnaled)
    {
        GPIO_IntDisable(0x01 << SD_DETECT_PIN);
        GPIO_IntClear(0x01 << SD_DETECT_PIN);
        SDCard_DetectEvent = NULL;
        BoardCommon_GPIOInterruptUnregisterCallback(SD_DETECT_PIN);
        SDCardDetectInterruptEnaled = false;
    }
    SDCardDisable();
    return RETCODE_OK;

}
/*
 * Refer to interface header for description
 */
Retcode_T BSP_SDCard_Disconnect(void)
{
    GPIO_PinModeSet(SD_CARD_CS_PORT, SD_CARD_CS_PIN, gpioModeDisabled, 1);
    GPIO_PinModeSet(SD_CARD_LS_PORT, SD_CARD_CS_PIN, gpioModeDisabled, 1);
    GPIO_PinModeSet(SD_DETECT_PORT, SD_DETECT_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(SD_CARD_SPI1_MISO_PORT, SD_CARD_SPI1_MISO_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(SD_CARD_SPI1_MOSI_PORT, SD_CARD_SPI1_MOSI_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(SD_CARD_SPI1_SCK_PORT, SD_CARD_SPI1_SCK_PIN, gpioModeDisabled, 0);

    CMU_ClockEnable(SD_CARD_SPI1_CLOCK, true);
    USART1_PORT_BASE->ROUTE = _USART_ROUTE_RESETVALUE;
    CMU_ClockEnable(SD_CARD_SPI1_CLOCK, false);
    return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_SDCard_SetCSHigh(void)
{
    GPIO_PinOutSet(SD_CARD_CS_PORT, SD_CARD_CS_PIN);
    return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_SDCard_SetCSLow(void)
{
    GPIO_PinOutClear(SD_CARD_CS_PORT, SD_CARD_CS_PIN);
    return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
HWHandle_T BSP_SDCard_GetSPIHandle(void)
{
    return (HWHandle_T) &BSP_SDCard_SPI_Handle;
}

/*
 * Refer to interface header for description
 */
void SDCard_SPIRxISR(void)
{
    if (NULL != SDCard_SPI_RxISRCallback)
    {
        /* All UART interrupt processing is to be done at the MCU level */
        SDCard_SPI_RxISRCallback((SPI_T) &BSP_SDCard_SPI_Handle);
    }
}

/*
 * Refer to interface header for description
 */
void SDCard_SPITxISR(void)
{
    if (NULL != SDCard_SPI_TxISRCallback)
    {
        /* All UART interrupt processing is to be done at the MCU level */
        SDCard_SPI_TxISRCallback((SPI_T) &BSP_SDCard_SPI_Handle);
    }
}

#if BCDS_FEATURE_DMA
/*
 * Refer to interface header for description
 */
void SDCard_SPI_RxDMAComplete(uint32_t channel, bool primary, void * user)
{
	BCDS_UNUSED(channel);
	BCDS_UNUSED(primary);
	BCDS_UNUSED(user);

	if (NULL != SDCard_SPI_RxDmaCallback)
	{
		/* All UART interrupt processing is to be done at the MCU level */
		SDCard_SPI_RxDmaCallback((SPI_T) &BSP_SDCard_SPI_Handle);
	}
}


/*
 * Refer to interface header for description
 */
void SDCard_SPI_TxDMAComplete(uint32_t channel, bool primary, void * user)
{
	BCDS_UNUSED(channel);
	BCDS_UNUSED(primary);
	BCDS_UNUSED(user);

	if (NULL != SDCard_SPI_TxDmaCallback)
	{
		/* All UART interrupt processing is to be done at the MCU level */
		SDCard_SPI_TxDmaCallback((SPI_T) &BSP_SDCard_SPI_Handle);
	}
}

#endif

void SDCard_Detectinterrupt(void)
{

    if (SDCardDetectInterruptEnaled)
    {
        if (BSP_IO_PIN_LOW == GPIO_PinInGet(SD_DETECT_PORT, SD_DETECT_PIN))
        {
            SDCard_DetectEvent((uint32_t)BSP_XDK_SDCARD_INSERTED);
            if (false == SDCardEnabled)
            {
                SDCardEnable();
                SDCardEnabled = true;
            }
        }
        else
        {
            SDCard_DetectEvent((uint32_t)BSP_XDK_SDCARD_EJECTED);
            SDCardDisable();
        }
    }
}

void SDCardEnable(void)
{
    BoardCommon_EnablePowerSupply3V3(SD_CARD);
    BoardCommon_WakeupPowerSupply2V5(SD_CARD);
    /* Enable level shifter */
    GPIO_PinOutClear(SD_CARD_LS_PORT, SD_CARD_LS_PIN);
    /* Select card */
    GPIO_PinOutClear(SD_CARD_CS_PORT, SD_CARD_CS_PIN);
    /*enable the clock for the usart/spi interface*/
    CMU_ClockEnable(SD_CARD_SPI1_CLOCK, true);
    USART1_PORT_BASE->CMD = USART_CMD_CLEARTX;
    USART1_PORT_BASE->CMD = USART_CMD_CLEARRX;
    /* Set interrupt priorities */
    NVIC_SetPriority(SD_CARD_RX_IRQN, SD_CARD_RX_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SD_CARD_TX_IRQN, SD_CARD_TX_INTERRUPT_PRIORITY);
    /* clear pending interrupts */
    NVIC_ClearPendingIRQ(SD_CARD_RX_IRQN);
    NVIC_ClearPendingIRQ(SD_CARD_TX_IRQN);
    /* Assign the callback */
    SDCard_SPI_RxISRCallback = BSP_SDCard_SPI_Handle.IrqRxCallback;
    SDCard_SPI_TxISRCallback = BSP_SDCard_SPI_Handle.IrqTxCallback;

#if BCDS_FEATURE_DMA
    SDCard_SPI_RxDmaCallback = BSP_SDCard_SPI_Handle.DmaRxCallback;
	SDCard_SPI_TxDmaCallback = BSP_SDCard_SPI_Handle.DmaTxCallback;
#endif

    USART1_PORT_BASE->IEN |= USART_IEN_RXDATAV;
    USART1_PORT_BASE->IFC = _USART_IFC_MASK;
    NVIC_EnableIRQ(SD_CARD_RX_IRQN);
    NVIC_EnableIRQ(SD_CARD_TX_IRQN);
    USART1_PORT_BASE->CMD = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;
    SDCardEnabled = true;

}

void SDCardDisable(void)
{
    USART1_PORT_BASE->CMD = USART_CMD_MASTERDIS | USART_CMD_TXDIS | USART_CMD_RXDIS;
    USART1_PORT_BASE->IEN &= ~(USART_IEN_RXDATAV);
    CMU_ClockEnable(SD_CARD_SPI1_CLOCK, false);

    NVIC_DisableIRQ(SD_CARD_RX_IRQN);
    NVIC_DisableIRQ(SD_CARD_TX_IRQN);

    SDCard_SPI_RxISRCallback = NULL;
    SDCard_SPI_TxISRCallback = NULL;

    GPIO_PinOutSet(SD_CARD_CS_PORT, SD_CARD_CS_PIN);
    GPIO_PinOutSet(SD_CARD_LS_PORT, SD_CARD_LS_PIN);

    BoardCommon_DisablePowerSupply3V3(SD_CARD);
    BoardCommon_SnoozePowerSupply2V5(SD_CARD);
    SDCardEnabled = false;

}
#endif /* BCDS_FEATURE_BSP_SD_CARD */

/**@}*/
