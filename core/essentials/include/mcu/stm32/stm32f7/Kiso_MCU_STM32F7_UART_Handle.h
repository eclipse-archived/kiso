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
 * @brief       Structure holding the current transaction parameters.
 *
 * @param       *pTransmitBuffer Reference to the transmit buffer for the current send transaction.
 * @param       TransmitSize Transmit size.
 * @param       *pReceiveBuffer Reference to the receive buffer for the current receiver transaction.
 * @param       ReceivetSize Receive size.
 */
struct MCU_UART_Transcation_S
{
    uint8_t *pTransmitBuffer;
    uint32_t TransmitSize;
    uint8_t *pReceiveBuffer;
    uint32_t ReceivetSize;
};

/* forward structure declaration */
struct MCU_UART_S;
/**
 * @brief       Structure holding the UART control block.
 *
 * @param       huart STM32L4 library UART Handle.
 * @param       TxMode Transmitter mode (set by BSP).
 * @param       RxMode Receiver mode (set by BSP).
 * @param       Datarate Data rate in bauds (set by the BSP).
 * @param       TxState State of the transmitter (internal use only).
 * @param       RxState State of the receiver (internal use only).
 * @param       IrqCallback Reference to the IRQ handler function (internal use only).
 * @param       DmaRxCallback Reference to the  Rx DMA handler function (internal use only).
 * @param       DmaTxCallback Reference to the  Rx DMA handler function (internal use only).
 * @param       AppCallback Reference to the applications event handler function  (internal use only).
 * @param       SendFunc Reference to the sending function depending on the transmitter mode (internal use only).
 * @param       ReceiveFunc Reference to the receiving function depending on the receiver mode (internal use only).
 * @param       AbortSendFunc Reference to the canceling function depending on the transmitter mode (internal use only).
 * @param       AbortReceiveFunc Reference to the canceling function depending on the receiver mode (internal use only).
 * @param       Transaction Current transaction parameters.
 */
struct MCU_UART_S
{
    UART_HandleTypeDef huart;
    enum KISO_HAL_TransferMode_E TxMode;
    enum KISO_HAL_TransferMode_E RxMode;
    uint32_t Datarate;
    MCU_UART_State_T TxState;
    MCU_UART_State_T RxState;
    void (*IrqCallback)(UART_T uart);
    void (*DmaRxCallback)(UART_T uart);
    void (*DmaTxCallback)(UART_T uart);
    MCU_UART_Callback_T AppCallback;
    Retcode_T (*SendFunc)(struct MCU_UART_S *uart);
    Retcode_T (*ReceiveFunc)(struct MCU_UART_S *uart);
    void (*AbortSendFunc)(struct MCU_UART_S *uart);
    void (*AbortReceiveFunc)(struct MCU_UART_S *uart);
    struct MCU_UART_Transcation_S Transaction;
};

#endif /* KISO_FEATURE_UART */
#endif /* KISO_MCU_STM32L4_UART_HANDLE_H_ */
