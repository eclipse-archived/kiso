/**********************************************************************************************************************
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
 **********************************************************************************************************************/

/**
 * @file
 *
 * @brief
 * Handles MCU STM32 SPI driver encapsulation
 */
/*---------------------- INCLUDED HEADERS ---------------------------------------------------------------------------*/
#include "Kiso_MCU_SPI.h"

#if KISO_FEATURE_SPI
#include "Kiso_MCU_STM32F7_SPI_Handle.h"
/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_SPI

#define SPI_DEFAULT_TIMEOUT_MS UINT32_C(1000)
#define SPI_MIN_TIMEOUT_MS UINT32_C(1)
#define SPI_8BIT_DATA_SIZE UINT32_C(8)
#define SPI_SAFETY_FACTOR UINT32_C(2)
#define SPI_SECOND_TO_MILLI UINT32_C(1000)

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

/*
 * The STM32Cube Library provides a HAL driver for SPI I/O operations.
 * However depending on the configured transfer mode (Polling, Interrupt or DMA)
 * the developer has to call different functions (see stm32l4xx_hal_spi.h).
 * Only the function signatures for IRQ and DMA transfer are the same. The
 * polling functions are introducing an additional parameter for a timeout.
 *
 * The plan is to provide function pointers on these functions (at least for
 * IRQ and DMA mode) and to set them according to the configuration of the SPI
 * interface upon initialization.
 */

void SPI_IRQHandler(SPI_T spi);
void SPI_DMATxHandler(SPI_T spi);
void SPI_DMARxHandler(SPI_T spi);

Retcode_T SPI_SendPollMode(struct MCU_SPI_S *spi);
Retcode_T SPI_ReceivePollMode(struct MCU_SPI_S *spi);
Retcode_T SPI_TransferPollMode(struct MCU_SPI_S *spi);

Retcode_T SPI_SendIntMode(struct MCU_SPI_S *spi);
Retcode_T SPI_ReceiveIntMode(struct MCU_SPI_S *spi);
Retcode_T SPI_TransferIntMode(struct MCU_SPI_S *spi);

Retcode_T SPI_SendDmaMode(struct MCU_SPI_S *spi);
Retcode_T SPI_ReceiveDmaMode(struct MCU_SPI_S *spi);
Retcode_T SPI_TransferDmaMode(struct MCU_SPI_S *spi);

Retcode_T SPI_CancelTransaction(struct MCU_SPI_S *spi);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/** See description in the interface declaration */
Retcode_T MCU_SPI_Initialize(SPI_T spi, MCU_SPI_Callback_T callback)
{
    Retcode_T retcode = RETCODE_OK;

    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;
    if (!pSPI)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (!IS_SPI_ALL_INSTANCE(pSPI->hspi.Instance))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SPI_INVALID_PERIPHERAL);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if ((SPI_STATE_INIT != pSPI->State))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        switch (pSPI->TransferMode)
        {
        case KISO_HAL_TRANSFER_MODE_POLLING:
            pSPI->IRQCallback = NULL;
            pSPI->DmaTxCallback = NULL;
            pSPI->DmaRxCallback = NULL;
            pSPI->AppCallback = NULL;

            pSPI->TxFunPtr = SPI_SendPollMode;
            pSPI->RxFunPtr = SPI_ReceivePollMode;
            pSPI->TransferFunPtr = SPI_TransferPollMode;
            pSPI->CancelFunPtr = SPI_CancelTransaction;
            pSPI->State = SPI_STATE_READY;
            break;

        case KISO_HAL_TRANSFER_MODE_INTERRUPT:
            if (NULL != callback)
            {
                pSPI->IRQCallback = SPI_IRQHandler;
                pSPI->DmaTxCallback = NULL;
                pSPI->DmaRxCallback = NULL;
                pSPI->AppCallback = callback;

                pSPI->TxFunPtr = SPI_SendIntMode;
                pSPI->RxFunPtr = SPI_ReceiveIntMode;
                pSPI->TransferFunPtr = SPI_TransferIntMode;
                pSPI->CancelFunPtr = SPI_CancelTransaction;
                pSPI->State = SPI_STATE_READY;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            }
            break;

        case KISO_HAL_TRANSFER_MODE_DMA:
            if (NULL != callback)
            {
                pSPI->IRQCallback = SPI_IRQHandler;
                pSPI->DmaTxCallback = SPI_DMATxHandler;
                pSPI->DmaRxCallback = SPI_DMARxHandler;
                pSPI->AppCallback = callback;

                pSPI->TxFunPtr = SPI_SendDmaMode;
                pSPI->RxFunPtr = SPI_ReceiveDmaMode;
                pSPI->TransferFunPtr = SPI_TransferDmaMode;
                pSPI->CancelFunPtr = SPI_CancelTransaction;
                pSPI->State = SPI_STATE_READY;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            }
            break;

        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
            break;
        }
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_SPI_Deinitialize(SPI_T spi)
{
    Retcode_T retcode = RETCODE_OK;

    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;
    if (!pSPI)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if ((SPI_STATE_READY != pSPI->State))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        pSPI->State = SPI_STATE_INIT;
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_SPI_Send(SPI_T spi, uint8_t *data, uint32_t len)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;

    if (NULL == spi || NULL == data)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (UINT16_MAX < len)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (len != 0)
        {
            if (SPI_STATE_READY == pSPI->State)
            {
                pSPI->Transaction.pTxDataBuffer = data;
                /** \todo: Check for len > 0xFFFF because of the downcast */
                pSPI->Transaction.Size = (uint16_t)len;
                retcode = pSPI->TxFunPtr(pSPI);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (SPI_STATE_TX == pSPI->State)
            {
                /* Application wants to cancel ongoing sending which is signaled by length == 0 */
                retcode = pSPI->CancelFunPtr(pSPI);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_SPI_Receive(SPI_T spi, uint8_t *buffer, uint32_t len)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;

    if (NULL == spi || NULL == buffer)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (UINT16_MAX < len)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (len != 0)
        {
            if (SPI_STATE_READY == pSPI->State)
            {
                /*clear the receive buffer*/
                for (uint32_t i = 0; i < len; i++)
                {
                    /* The STM32 HAL Library relies on the receive buffer in order to trigger the clock generation */
                    buffer[i] = pSPI->DefaultData;
                }
                pSPI->Transaction.pRxDataBuffer = buffer;
                /** \todo: Check for len > 0xFFFF because of the downcast */
                pSPI->Transaction.Size = (uint16_t)len;
                retcode = pSPI->RxFunPtr(pSPI);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (SPI_STATE_RX == pSPI->State)
            {
                /* Application wants to cancel ongoing sending which is signaled by length == 0 */
                retcode = pSPI->CancelFunPtr(pSPI);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_SPI_Transfer(SPI_T spi, uint8_t *data_out, uint8_t *data_in, uint32_t numTransfer)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;

    if (NULL == spi || NULL == data_out || NULL == data_in)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (UINT16_MAX < numTransfer)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (numTransfer != 0)
        {
            if (SPI_STATE_READY == pSPI->State)
            {
                pSPI->Transaction.pTxDataBuffer = data_out;
                pSPI->Transaction.pRxDataBuffer = data_in;
                /** \todo: Check for len > 0xFFFF because of the downcast */
                pSPI->Transaction.Size = (uint16_t)numTransfer;
                retcode = pSPI->TransferFunPtr(pSPI);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (SPI_STATE_TX_RX == pSPI->State)
            {
                /* Application wants to cancel ongoing sending which is signaled by length == 0 */
                retcode = pSPI->CancelFunPtr(pSPI);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
    }
    return retcode;
}

/** See description in the interface declaration */
uint32_t MCU_SPI_GetDataCount(SPI_T spi)
{
    uint32_t dataCount = 0;
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;
    if (pSPI)
    {
        switch (pSPI->State)
        {
        case SPI_STATE_TX:
            dataCount = pSPI->hspi.TxXferCount;
            break;
        case SPI_STATE_RX:
            dataCount = pSPI->hspi.RxXferCount;
            break;
        case SPI_STATE_TX_RX:
            dataCount = pSPI->hspi.RxXferCount;
            break;
        default:
            dataCount = 0;
            break;
        }
    }
    return dataCount;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * @brief       Sends data in polling mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */
Retcode_T SPI_SendPollMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t timeout_ms = SPI_DEFAULT_TIMEOUT_MS;
    if (spi->DataRate)
    {
        timeout_ms = SPI_MIN_TIMEOUT_MS + ((spi->Transaction.Size * SPI_8BIT_DATA_SIZE * SPI_SAFETY_FACTOR) / (1 + spi->DataRate / SPI_SECOND_TO_MILLI));
    }

    spi->State = SPI_STATE_TX;
    if (HAL_OK != HAL_SPI_Transmit(&spi->hspi, spi->Transaction.pTxDataBuffer, spi->Transaction.Size, timeout_ms))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    spi->State = SPI_STATE_READY;
    return retcode;
}

/**
 * @brief       Receives data in polling mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */
Retcode_T SPI_ReceivePollMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;

    uint32_t timeout_ms = SPI_DEFAULT_TIMEOUT_MS;
    if (spi->DataRate)
    {
        timeout_ms = SPI_MIN_TIMEOUT_MS + ((spi->Transaction.Size * SPI_8BIT_DATA_SIZE * SPI_SAFETY_FACTOR) / (1 + spi->DataRate / SPI_SECOND_TO_MILLI));
    }
    spi->State = SPI_STATE_RX;
    if (HAL_OK != HAL_SPI_Receive(&spi->hspi, spi->Transaction.pRxDataBuffer, spi->Transaction.Size, timeout_ms))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    spi->State = SPI_STATE_READY;
    return retcode;
}

/**
 * @brief       Sends and receives data in polling mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */
Retcode_T SPI_TransferPollMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t timeout_ms = SPI_DEFAULT_TIMEOUT_MS;

    if (spi->DataRate)
    {
        /* Calculate the timeout value as the double of the required time to process the transaction */
        timeout_ms = SPI_MIN_TIMEOUT_MS + ((spi->Transaction.Size * SPI_8BIT_DATA_SIZE * SPI_SAFETY_FACTOR) / (1 + spi->DataRate / SPI_SECOND_TO_MILLI));
    }

    spi->State = SPI_STATE_TX_RX;
    if (HAL_OK != HAL_SPI_TransmitReceive(&spi->hspi, spi->Transaction.pTxDataBuffer, spi->Transaction.pRxDataBuffer, spi->Transaction.Size, timeout_ms))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    spi->State = SPI_STATE_READY;
    return retcode;
}

/**
 * @brief       Sends data in interrupt mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */
Retcode_T SPI_SendIntMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;

    spi->State = SPI_STATE_TX;
    if (HAL_OK != HAL_SPI_Transmit_IT(&spi->hspi, spi->Transaction.pTxDataBuffer, spi->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        spi->State = SPI_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Receives data in interrupt mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */
Retcode_T SPI_ReceiveIntMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;

    spi->State = SPI_STATE_RX;
    if (HAL_OK != HAL_SPI_Receive_IT(&spi->hspi, spi->Transaction.pRxDataBuffer, spi->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        spi->State = SPI_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Sends and receives data in interrupt mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */
Retcode_T SPI_TransferIntMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;

    spi->State = SPI_STATE_TX_RX;
    if (HAL_OK != HAL_SPI_TransmitReceive_IT(&spi->hspi, spi->Transaction.pTxDataBuffer, spi->Transaction.pRxDataBuffer, spi->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        spi->State = SPI_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Sends data in DMA mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */
Retcode_T SPI_SendDmaMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;

    spi->State = SPI_STATE_TX;
    if (HAL_OK != HAL_SPI_Transmit_DMA(&spi->hspi, spi->Transaction.pTxDataBuffer, spi->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        spi->State = SPI_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Receives data in DMA mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */

Retcode_T SPI_ReceiveDmaMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;

    spi->State = SPI_STATE_RX;
    if (HAL_OK != HAL_SPI_Receive_DMA(&spi->hspi, spi->Transaction.pRxDataBuffer, spi->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        spi->State = SPI_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Sends and receives data in DMA mode
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */
Retcode_T SPI_TransferDmaMode(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;

    spi->State = SPI_STATE_TX_RX;
    if (HAL_OK != HAL_SPI_TransmitReceive_DMA(&spi->hspi, spi->Transaction.pTxDataBuffer, spi->Transaction.pRxDataBuffer, spi->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        spi->State = SPI_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Cancels an ongoing transaction. Valid for all modes.
 * @param       spi reference to the SPI control block (structure)
 * @return      RETCODE_OK in case of success
 *              RETCODE_FAILURE in case of STM32 HAL Library failure.
 */

Retcode_T SPI_CancelTransaction(struct MCU_SPI_S *spi)
{
    Retcode_T retcode = RETCODE_OK;
    if (HAL_OK != HAL_SPI_Abort(&spi->hspi))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    else
    {
        spi->State = SPI_STATE_READY;
    }
    return retcode;
}

/** SPI IRQ callback handler which is called back
 * by the BSP SPI IRQ handler
 * @param spi is the BSP handle for the interface
 */
void SPI_IRQHandler(SPI_T spi)
{
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;
    if (pSPI)
    {
        HAL_SPI_IRQHandler(&pSPI->hspi);
    }
}

/**
 * @brief      IRQ handler for DMA RX called by BSP
 * @param      dma: Handle to MCU uart (MCU_SPI_handle_S)
 * @retval     None
 */
void SPI_DMARxHandler(SPI_T spi)
{
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;
    if (pSPI)
    {
        HAL_DMA_IRQHandler(pSPI->hspi.hdmarx);
    }
}

/**
 * @brief      IRQ handler for DMA TX called by BSP
 * @param      dma: Handle to MCU uart (MCU_SPI_handle_S)
 * @retval     None
 */
void SPI_DMATxHandler(SPI_T spi)
{
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)spi;
    if (pSPI)
    {
        HAL_DMA_IRQHandler(pSPI->hspi.hdmatx);
    }
}

/**
 * This section contains callback functions that override the STM32 HAL
 * default callback implementations.
 */

/**
 * @brief Callback function for SPI error events
 * @details This function overrides the weak definition provided by the STM32
 * HAL and is called upon every error interrupt request occurring on a
 * particular SPI interface.
 * @param hspi is the STM32 HAL handle of the interface that reports the error
 */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)hspi;

    if (pSPI)
    {
        /* Has a valid handle, now process the event */
        struct MCU_SPI_Event_S Events = {0, 0, 0, 0, 0, 0};

        uint32_t HalError = HAL_SPI_GetError(hspi);
        switch (HalError)
        {
        case HAL_SPI_ERROR_MODF: /*!< MODF error                        */
            Events.DataLoss = 1;
            break;
        case HAL_SPI_ERROR_CRC: /*!< CRC error                         */
            Events.RxError = 1;
            Events.DataLoss = 1;
            break;
        case HAL_SPI_ERROR_OVR: /*!< OVR error                         */
            Events.RxError = 1;
            Events.DataLoss = 1;
            break;
        case HAL_SPI_ERROR_FRE: /*!< FRE error                         */
            Events.RxError = 1;
            Events.DataLoss = 1;
            break;
        case HAL_SPI_ERROR_DMA: /*!< DMA transfer error                */
            Events.RxError = 1;
            Events.TxError = 1;
            Events.DataLoss = 1;
            break;
        case HAL_SPI_ERROR_FLAG: /*!< Error on BSY/TXE/FTLVL/FRLVL Flag */
            Events.RxError = 1;
            Events.TxError = 1;
            Events.DataLoss = 1;
            break;
        default:
            break;
        }

        if (NULL != pSPI->AppCallback)
        {
            pSPI->AppCallback((SPI_T)pSPI, Events);
        }
        pSPI->State = SPI_STATE_READY;
    }
}

/**
 * @brief Callback function for SPI TX complete events.
 * @details This function overrides the weak definition provided by the STM32
 * HAL and is called when a Tx transfer is completed.
 * @param hspi is the STM32 HAL handle of the interface that reports the event
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)hspi;
    if (pSPI)
    {
        if (pSPI->State == SPI_STATE_TX)
        {
            struct MCU_SPI_Event_S Events = {0, 0, 0, 0, 0, 0};
            Events.TxComplete = 1;

            pSPI->AppCallback((SPI_T)pSPI, Events);
            pSPI->State = SPI_STATE_READY;
        }
    }
}

/**
 * @brief Callback function for SPI RX complete events.
 * @details This function overrides the weak definition provided by the STM32
 * HAL and is called when a Rx transfer is completed.
 * @param hspi is the STM32 HAL handle of the interface that reports the event
 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)hspi;
    if (pSPI)
    {
        if (pSPI->State == SPI_STATE_RX)
        {
            /* Has a valid handle, now process the event */
            struct MCU_SPI_Event_S Events = {0, 0, 0, 0, 0, 0};
            Events.RxComplete = 1;

            pSPI->AppCallback((SPI_T)pSPI, Events);
            pSPI->State = SPI_STATE_READY;
        }
    }
}

/**
 * @brief Callback function for SPI RX/TX complete events.
 * @details This function overrides the weak definition provided by the STM32
 * HAL and is called when a Rx/Tx transfer is completed.
 * @param hspi is the STM32 HAL handle of the interface that reports the event
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    struct MCU_SPI_S *pSPI = (struct MCU_SPI_S *)hspi;
    if (pSPI)
    {
        if (pSPI->State == SPI_STATE_TX_RX)
        {
            /* Has a valid handle, now process the event */
            struct MCU_SPI_Event_S Events = {0, 0, 0, 0, 0, 0};
            Events.RxComplete = 1;
            Events.TxComplete = 1;

            pSPI->AppCallback((SPI_T)pSPI, Events);
            pSPI->State = SPI_STATE_READY;
        }
    }
}

#endif //-- KISO_FEATURE_SPI
