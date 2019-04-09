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
 * Defines MCU UART handle
 *
 * @details
 * MCU functions do always have at least an integer parameter of type UART_T.
 * This integer is casted by MCU to a pointer to MCU_UART_handle_T.
 */

#ifndef BCDS_MCU_UART_HANDLE_H_
#define BCDS_MCU_UART_HANDLE_H_

#if BCDS_FEATURE_UART

#include "BCDS_MCU_UART.h"


/* TI UART abstraction layer.  BLE SDK 3.0 C:\ti\simplelink_cc2640r2_sdk_1_00_00_22\source\ti\drivers */
#include "drivers/UART.h"

enum MCU_UARTState_E
{
    MCU_UART_STATE_INIT,
    MCU_UART_STATE_READY,
    MCU_UART_STATE_ABORTING,
    MCU_UART_STATE_RUNNING
};

/**
 * @brief   Structure which is used as UART handle.
 *
 * @detail  The handle is usually created by the BSP and fetched by the
 * application layer to use the UART interface.
 */
struct MCU_UART_Handle_S;

struct MCU_UART_Handle_S
{
    UART_Handle handle; /**< Handle expected by TI UART driver */
    enum BCDS_HAL_TransferMode_E transferMode; /**< transfer mode used */
    enum BCDS_HAL_CallMode_E volatile callMode; /**< call mode used */

    enum MCU_UARTState_E volatile rxState; /**< state of the rx line */
    enum MCU_UARTState_E volatile txState; /**< state of the tx line */

    void (*txCallback)(UART_T uart); /**< function that will be called upon transfer completion */
    void (*rxCallback)(UART_T uart); /**< function that will be called upon transfer completion */

    MCU_UART_Callback_T appCallback; /**< function registered by the application to receive the UART transfer events( transfer_complete or transfer_error )*/

    Retcode_T (*rxFunc)(struct MCU_UART_Handle_S* uart, uint8_t * data, uint32_t length); /**< function performing UART receiving depending on the transfer mode */
    Retcode_T (*txFunc)(struct MCU_UART_Handle_S* uart, uint8_t * data, uint32_t length); /**< function performing UART transmitting depending on the transfer mode */
    Retcode_T (*abortTxFunc)(struct MCU_UART_Handle_S* uart);/**< function performing UART abort transmitting depending on the transfer mode */
    Retcode_T (*abortRxFunc)(struct MCU_UART_Handle_S* uart);/**< function performing UART abort receiving depending on the transfer mode */

    uint8_t* rxBuffer; /**< pointer to receive buffer passed from the API user*/
    uint32_t volatile rxCount;/** counter of received bytes*/
    uint32_t  rxSize;/** size of rx buffer bytes*/

    uint32_t volatile txCount;/** counter of transmitted bytes*/
    uint32_t  txSize;/** size of tx buffer */
};

#endif /* BCDS_FEATURE_UART && BCDS_UART_COUNT */
#endif /* BCDS_MCU_UART_HANDLE_H_ */

