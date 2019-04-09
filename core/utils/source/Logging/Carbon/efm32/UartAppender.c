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
 * @file
 *
 * @brief Implements log appender over USART DMA for efm32 target.
 *
 */

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID  BCDS_UTILS_MODULE_ID_LOGGING_APPENDER_UART

#include "BCDS_Logging.h"

#if BCDS_FEATURE_LOGGING

#include "LogConfig.h"

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"
/*lint -save -e830 -e534 */
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_dma.h"
/*lint -restore */

static DMA_DESCRIPTOR_TypeDef DmaControlBlock[LOG_DMA_CHAN + 1] __attribute__ ((aligned(256)));

static DMA_CB_TypeDef dmaCallBack;

static DMA_Init_TypeDef dmaInit =
{
    .hprot = 0,
    .controlBlock = DmaControlBlock
};

static DMA_CfgChannel_TypeDef txChanCfg =
{
    .highPri = false,
    .enableInt = true,
    .select = DMAREQ_USART2_TXBL,
    .cb = &dmaCallBack
};

static DMA_CfgDescr_TypeDef txDescrCfg =
{
    .dstInc = dmaDataIncNone,
    .srcInc = dmaDataInc1,
    .size = dmaDataSize1,
    .arbRate = dmaArbitrate1,
    .hprot = 0
};

static void UartAppender_Cb(unsigned int channel, bool primary, void *user)
{
    BCDS_UNUSED(primary);

    if (LOG_DMA_CHAN == channel && NULL != user)
    {
        BaseType_t taskWoken = pdFALSE;
        (void) xSemaphoreGiveFromISR((SemaphoreHandle_t) user, &taskWoken);
        portYIELD_FROM_ISR(taskWoken);
    }
}

static Retcode_T UartAppender_Init(void *wakeup)
{
    /* initialize the usart */
    GPIO_PinModeSet(LOG_GPIO_TX_PRT, LOG_GPIO_TX_PIN, LOG_GPIO_TX_EN, 1);
    CMU_ClockEnable(LOG_SER_CMU_CLOCK, true);

    USART_InitAsync_TypeDef data =
    {
        usartEnable,
        SystemHFXOClockGet(), /*lint !e446 */
        LOG_SER_BAUDRATE,
        usartOVS16,
        LOG_SER_DATABITS,
        LOG_SER_PARITY,
        LOG_SER_STOPBITS,
#if defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_TINY_FAMILY)
        false,
        false,
        usartPrsRxCh0
#endif
    };

    USART_Reset(LOG_SER_PORT);
    USART_InitAsync(LOG_SER_PORT, &data);

    LOG_SER_PORT->CTRL |= USART_CTRL_TXBIL_DEFAULT;
    USART_Enable(LOG_SER_PORT, (USART_Enable_TypeDef)(USART_CMD_TXEN));
    LOG_SER_PORT->ROUTE = USART_ROUTE_TXPEN | LOG_SER_ROUTE_LOCATION;

    /* initialize dma */
    dmaCallBack.cbFunc = UartAppender_Cb;
    dmaCallBack.userPtr = wakeup;
    dmaCallBack.primary = 0;

    DMA_Init(&dmaInit);
    DMA_CfgChannel(LOG_DMA_CHAN, &txChanCfg);
    DMA_CfgDescr(LOG_DMA_CHAN, true, &txDescrCfg);

    DmaControlBlock[LOG_DMA_CHAN].DSTEND = &LOG_SER_PORT->TXDATA;
    return RETCODE_OK;
}

static Retcode_T UartAppender_Write(const char *message, uint32_t length)
{
    DMA_ActivateBasic(LOG_DMA_CHAN, true, false, NULL, (char *) message, length - 1);
    return RETCODE_OK;
}

static const LogAppender_T UartAppender =
{
    .Init = UartAppender_Init,
    .Write = UartAppender_Write
};

const LogAppender_T *Logging_UARTAppender = &UartAppender;

#endif /* if BCDS_FEATURE_LOGGING */
