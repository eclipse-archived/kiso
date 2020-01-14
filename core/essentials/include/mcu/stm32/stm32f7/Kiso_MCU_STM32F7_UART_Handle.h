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
 * @brief   Defines MCU UART handle for STM32F7 MCU variant
 */

#ifndef KISO_MCU_STM32F7_UART_HANDLE_H_
#define KISO_MCU_STM32F7_UART_HANDLE_H_

#include "Kiso_MCU_UART.h"
#if KISO_FEATURE_UART
#include "stm32f7xx.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_uart.h"

/**
 * Enumeration of driver state machine states.
 */
typedef enum MCU_UART_State_E
{
    UART_STATE_INIT,
    UART_STATE_READY,
    UART_STATE_TX,
    UART_STATE_RX,
} MCU_UART_State_T;

/**
 * Structure holding the current transaction parameters.
 */
struct MCU_UART_Transcation_S
{
    const uint8_t *pTransmitBuffer; /**< *pTransmitBuffer Reference to the transmit buffer for the current send transaction. */
    uint16_t TransmitSize;          /**< TransmitSize Transmit size. */
    uint8_t *pReceiveBuffer;        /**< *pReceiveBuffer Reference to the receive buffer for the current receiver transaction. */
    uint16_t ReceivetSize;          /**< ReceivetSize Receive size. */
};

/* forward structure declaration */
struct MCU_UART_S;
/**
 * Structure holding the UART control block.
 */
struct MCU_UART_S
{
    UART_HandleTypeDef huart;                          /**< huart STM32f7 library UART Handle. */
    enum KISO_HAL_TransferMode_E TxMode;               /**< TxMode Transmitter mode (set by BSP) */
    enum KISO_HAL_TransferMode_E RxMode;               /**< RxMode Receiver mode (set by BSP). */
    uint32_t Datarate;                                 /**< Datarate Data rate in bauds (set by the BSP). */
    MCU_UART_State_T TxState;                          /**< TxState State of the transmitter (internal use only). */
    MCU_UART_State_T RxState;                          /**< RxState State of the receiver (internal use only). */
    void (*IrqCallback)(UART_T uart);                  /**< IrqCallback Reference to the IRQ handler function (internal use only). */
    void (*DmaRxCallback)(UART_T uart);                /**< DmaRxCallback Reference to the  Rx DMA handler function (internal use only). */
    void (*DmaTxCallback)(UART_T uart);                /**< DmaTxCallback Reference to the  Rx DMA handler function (internal use only). */
    MCU_UART_Callback_T AppCallback;                   /**< AppCallback Reference to the applications event handler function  (internal use only). */
    Retcode_T (*SendFunc)(struct MCU_UART_S *uart);    /**< SendFunc Reference to the sending function depending on the transmitter mode (internal use only).*/
    Retcode_T (*ReceiveFunc)(struct MCU_UART_S *uart); /**< ReceiveFunc Reference to the receiving function depending on the receiver mode (internal use only). */
    void (*AbortSendFunc)(struct MCU_UART_S *uart);    /**< AbortSendFunc Reference to the canceling function depending on the transmitter mode (internal use only).*/
    void (*AbortReceiveFunc)(struct MCU_UART_S *uart); /**< AbortReceiveFunc Reference to the canceling function depending on the receiver mode (internal use only). */
    struct MCU_UART_Transcation_S Transaction;         /**< Transaction Current transaction parameters.*/
};

#endif /* KISO_FEATURE_UART */
#endif /* KISO_MCU_STM32F7_UART_HANDLE_H_ */
