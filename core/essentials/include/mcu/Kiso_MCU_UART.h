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
 * @defgroup KISO_HAL_MCU_UART  MCU UART
 * @ingroup KISO_HAL_MCU_IF
 * @{
 *
 * @brief Unified UART (Universal Asynchronous Receiver Transmitter) Interface
 *
 * @details
 * A typical use of this API would start with initializing the UART instance. This will initializes the UART
 * with default settings.
 *
 * @code
 * retcode = MCU_UART_Initialize(uart, callback);
 * @endcode
 *
 * Now, the UART is configured and active such that the communication can start. The application should use the
 * function MCU_UART_Receive() to receive a certain number bytes. If receiving is non-blocking, the call triggers the
 * receiving process and returns. The callback is invoked with an RxComplete event when the expected number of bytes has
 * been received.
 *
 * For sending, the application uses the function MCU_UART_Send(). In a non-blocking mode a callback will happen to
 * notify the application about the success or failure of the sending operation.
 *
 * The callback may be also called for other events such as errors.
 *
 * @warning The callback function is usually invoked in the ISR context. The implementation of the callback function
 * must take this into account.

 * @note It is possible to disable the entire UART feature in the HAL BSP configuration file. I.e. the UART code will
 * only be compiled and linked if the feature <b>KISO_FEATURE_UART</b> is set to 1 in the configuration file.
 *
 * @note The driver may work in an interrupt- or DMA-based mode. This depends on the implementation of the driver as
 * well as on the way how the UART hardware is initialized and configured in the BSP.
 */

#ifndef KISO_MCU_UART_H_
#define KISO_MCU_UART_H_

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_UART

/**
 * @brief       Data type for the UART handler type which is used to identify the UART component to work with.
 */
typedef HWHandle_T UART_T;

/**
 * @brief       Structure to represent the events received from the UART. It is a bit field where every bit represents
 *              a particular event.
 */
struct MCU_UART_Event_S
{
    uint32_t RxError        :1; /**<  Receiver error has occurred.*/
    uint32_t RxComplete     :1; /**< The expected bytes have been received.*/
    uint32_t RxAborted      :1; /**< The receive operation has been aborted for some reason (e.g error ).*/
    uint32_t TxError        :1; /**< Transmitter error has occurred.*/
    uint32_t TxComplete     :1; /**< All bytes have been sent.*/
    uint32_t Cts            :1; /**< CTS (Clear To Send) line state has changed. */
    uint32_t Dsr            :1; /**< DSR (Data Set Ready) line state has changed. */
    uint32_t Dcd            :1; /**< DCD (Data Carrier Detect) line state has changed. */
    uint32_t Ri             :1; /**< RI (Ring Indicator) line state has changed. */
    uint32_t Unused         :24;

};

/**
 * @brief       Union grouping MCU_UART_Event_S structure to an uint32_t value
 */
union MCU_UART_Event_U
{
    struct MCU_UART_Event_S bitfield; /**< Events structure as a bitfield */
    uint32_t registerValue; /**< Stored value in the event structure (could be used to reset the overall bitfield structure) */
};

/**
 * @brief       Enumeration of special Uart interface error return codes
 */
enum RETCODE_MCU_UART_E
{
    RETCODE_MCU_UART_FAILURE = RETCODE_FIRST_CUSTOM_CODE ,
};


/**
 * @brief       Type definition for the UART event callback.
 *
 * @details     Upon initialization of UART, in non blocking mode, the user should pass a reference to a function of
 *              this signature (**void function(UART_T, struct MCU_UART_Event_S)**) handling event notifications from
 *              the UART driver.
 *
 * @param [in]  uart : Is the handle of the UART instance that issued the event.
 *
 * @param [in]  event : Structure containing actual event information.
 */
typedef void (*MCU_UART_Callback_T)(UART_T uart, struct MCU_UART_Event_S event);

/**
 * @brief       Initializes the UART Interface.
 *
 * @details     This function will initialize the UART driver and make it ready to operate, it does not influence the
 *              underlying UART hardware it takes care of the software initialization only.
 *
 * @note        All other UART API calls require a call to MCU_UART_Initialize() first else they shall report an error.
 *
 * @param [in]  uart : UART handle, a reference to the UART object instance to be initialized.
 *
 * @param [in]  callback :a reference to a function which processes the UART events, in non-blocking mode, which come
 *              from the UART instance to be initialized. Passing a NULL pointer is allowed only in case the call mode
 *              is blocking.
 * @warning     The callback function is usually invoked in interrupt context. The implementation of the callback
 *              function must take this into account.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback);

/**
 * @brief       Deinitializes the UART instance.
 *
 * @details     This function will deinitialize the UART instance and clear, if applicable, its dynamically allocated
 *              context. This function does not influence the underlying UART hardware it only takes care of the software
 *              deinitialization.
 *
 * @param [in]  uart : UART handle.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_UART_Deinitialize(UART_T uart);

/**
 * @brief       Sends data via UART
 *
 * @details     Depending on the prior initialization, the send operation could be blocking or non blocking:
 *                  - In <b>blocking</b> mode the function returns when the count of sent bytes matches the expected
 *                  count, or if an error occurred. The return value will then inform about the success or failure of
 *                  the operation.
 *                  - In case of <b>non blocking</b> the function triggers the send operation and returns the
 *                  information about the success or failure of triggering it. The send process shall continue in
 *                  the background (DMA or interrupt driven) and the application will then be informed via callback
 *                  about the success (TxComplete) or failure (TxError) of the send operation.
 *
 * @warning     In non-blocking mode it is not allowed to call this function a second time before completion of the
 *              first send operation i.e. the callback reported TxComplete or TxError events.
 *
 * @pre         MCU_UART_Initialize() successfully executed and the targeted device is in ENABLED state.
 *
 * @param[in]   uart : Is the handle of the UART.
 *
 * @param[in]   data : Is a reference to the buffer where data to send is located. The buffer must persist until the
 *              send operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param [in]  len : Is the number of data elements to send.
 * @note        Passing 0 to len will abort the ongoing send operation.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_UART_Send(UART_T uart, uint8_t * data, uint32_t len);

/**
 * @brief       Receiving data from the UART.
 *
 * @details     Depending on the prior initialization, the receive operation could be blocking or non blocking:
 *                  - In <b>blocking</b> mode the function starts the receive process and blocks until the count of
 *                  received bytes matches the expected count, or if an error occurred. The return value will then
 *                  inform about the success or failure of the operation.
 *                  - In non-blocking call mode the callback is invoked with an RxComplete event once the expected
 *                  number of bytes has been received.The received bytes are stored in the provided buffer. Unlike I2C
 *                  and SPI, UART is designed to receive streams. Hence, one call of MCU_UART_Receive(uart, buffer, n)
 *                  in non-blocking mode  will result in continuously receiving n bytes into the given buffer and
 *                  invoking the callback. The upper-layer does not need to call MCU_UART_Receive() again to receive
 *                  the next n bytes. In order to stop receiving, the upper-layer needs to call MCU_UART_Receive() with
 *                  the size of 0.
 *
 * @warning     In non-blocking mode it is not allowed to call this function a second time before completion of the
 *              first send operation i.e. the callback reported RxComplete or RxError events.
 *
 * @pre         MCU_UART_Initialize() successfully executed and the targeted device is in ENABLED state.
 *
 * @param[in]   uart : UART handle.
 *
 * @param[in]   buffer : Pointer to the buffer in which the received data is to be stored.
 *
 * @param[in]   size : The length of the data to be received.
 * @note        Passing 0 to len will abort the ongoing send operation.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_UART_Receive(UART_T uart, uint8_t* buffer, uint32_t size);

/**
 * @brief       Number of bytes available in the receiving buffer.
 *
 * @details     The application may use this function to check the progress of an ongoing receiving process.
 *
 * @param[in]   uart  : UART handle.
 * @param[out]  count : The number of available bytes.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_UART_GetRxCount(UART_T uart, uint32_t* count);

#endif /* KISO_FEATURE_UART */
#endif /* KISO_MCU_UART_H_ */
/**  @} */
