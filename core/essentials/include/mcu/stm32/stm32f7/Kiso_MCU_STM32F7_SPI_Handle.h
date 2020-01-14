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
 * @brief       
 * Defines MCU SPI Control Block
 *
 * @details
 * MCU functions do always have at least an integer parameter of type SPI_T.
 * This integer is casted by MCU to a pointer to MCU_SPI_handle_T.
 * The data pointed to by SPI_TypeDef* is managed in BSP.
 */

#ifndef KISO_MCU_STM32F7_SPI_HANDLE_H_
#define KISO_MCU_STM32F7_SPI_HANDLE_H_

#include "Kiso_MCU_SPI.h"

#if KISO_FEATURE_SPI

#include "stm32f7xx.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_spi.h"

/**
 * @brief       Enumeration of driver state machine states.
 */
typedef enum MCU_SPI_State_E
{
    SPI_STATE_INIT,
    SPI_STATE_READY,
    SPI_STATE_TX,
    SPI_STATE_RX,
    SPI_STATE_TX_RX,
} MCU_SPI_State_T;

/**
 * @brief        Structure defining an SPI transaction.
 */
struct MCU_SPI_Transaction_S
{
    uint8_t *pRxDataBuffer; /**< Reference to the receive buffer used for the transaction*/
    uint8_t *pTxDataBuffer; /**< Reference to the transmit buffer used for the transcation */
    uint16_t Size;          /**< Size in bytes of the transaction */
};

/**
 * @brief       SPI Control Block.
 * @detail      This structure is usually hosted by the BSP, it is used internally by the SPI driver in order to control
 *              the  peripherals operation.
 */
struct MCU_SPI_S;
struct MCU_SPI_S
{
    SPI_HandleTypeDef hspi;                              /**< STM32 HAL library SPI handle structure  */
    enum KISO_HAL_TransferMode_E TransferMode;           /**< Identifies the transfer mode that is currently configured for
     this SPI instance. This value will be set by the BSP upon configuration of the SPI interface.*/
    uint8_t DefaultData;                                 /**<default data to be sent on the MOSI line in order to trigger the clock generation*/
    MCU_SPI_State_T State;                               /**< driver state */
    uint32_t DataRate;                                   /**< Data rate in bits per second as configured by the BSP */
    void (*IRQCallback)(SPI_T spi);                      /**< Reference to the SPI IRQ handler function */
    void (*DmaRxCallback)(SPI_T spi);                    /**< Reference to the Rx DMA IRQ handler in case DMA mode is used mode */
    void (*DmaTxCallback)(SPI_T spi);                    /**< Reference to the Tx DMA IRQ handler in case DMA mode is used mode*/
    MCU_SPI_Callback_T AppCallback;                      /**< Reference to the application event handler which will be called if an event has
    to be signaled */
    Retcode_T (*TxFunPtr)(struct MCU_SPI_S *pSPI);       /**< Reference to the Tx function. Will be set upon initialization. */
    Retcode_T (*RxFunPtr)(struct MCU_SPI_S *pSPI);       /**< Reference to the Rx function. Will be set upon initialization. */
    Retcode_T (*TransferFunPtr)(struct MCU_SPI_S *pSPI); /**< Reference to the Transfer function. Will be set upon initialization. */
    Retcode_T (*CancelFunPtr)(struct MCU_SPI_S *pSPI);   /**< Reference to the Cancel function. Will be set upon initialization. */
    struct MCU_SPI_Transaction_S Transaction;            /**< Current transaction parameters*/
};

#endif /* KISO_FEATURE_SPI */
#endif /* KISO_MCU_STM32F7_SPI_HANDLE_H_ */
