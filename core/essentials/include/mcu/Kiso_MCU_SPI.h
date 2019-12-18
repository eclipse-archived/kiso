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
 * @defgroup KISO_HAL_MCU_SPI  MCU SPI
 * @ingroup KISO_HAL_MCU_IF
 * @{
 *
 * @brief Unified SPI (Serial Peripheral Interface) API
 *
 * @details
 * The SPI hardware gets initialized by the Board support package. For an SPI device, the BSP API
 * exposes a getHandle() method which provides a handle for the SPI communication interface in use. That handle is used
 * by this driver to send and receive data.
 *
 * A typical use of this API would start with initializing the SPI instance by calling
 * MCU_SPI_Initialize(mySPIHandle, mySPICallBack) where mySPIHandle is a reference to the SPI instance used by myDevice
 * and mySPICallback is a callback function in the application layer which, in case the call mode is not blocking, will
 * be called to process SPI events.
 * After this, and if myDevice is in ENABLED state, the application/middleware should be able to use the other API
 * methods MCU_SPI_Send(), MCU_SPI_Receive(), etc...
 * a possible use case would be as follows:
 * @code
 * Retcode_T retcode;
 * retcode = BSP_MyDevice_Enable();
 * if(RETCODE_OK == retcode)
 * {
 *      mySPIHandle = BSP_MyDevice_GetHandle();
 *      retcode = MCU_SPI_Initialize(mySPIHandle, mySPICallBack);
 * }
 * if(RETCODE_OK == retcode)
 * {
 *      retcode= MCU_SPI_Send();
 * }
 * if(RETCODE_OK == retcode)
 * {
 *      //do something
 * }
 * @endcode
 *
 * @note        It is possible to disable the entire SPI feature in the HAL-BSP configuration file. I.e. the SPI code
 * will only be compiled and linked if the feature <b>KISO_FEATURE_SPI</b> is set to 1 in the configuration file.
 *
 * @note        This interface only supports the SPI master mode.
 */

#ifndef KISO_MCU_SPI_H
#define KISO_MCU_SPI_H

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_SPI

/**
 * @brief       Type definition for the SPI handle which is used to identify the I2C instance to work with.
 */
typedef HWHandle_T SPI_T;

/**
 * @brief       Structure holding device attributes for an SPI device.
 *
 * @details     An SPI device is distinguished through its select and de-select functions which in most cases are acting
 *              on the Chip Select pin of the device. This structure shall be initialized by the BSP to hold reference
 *              to those functions and communicated to the upper layer by type casting to SWHandle_T and writing
 *              it to DeviceAttr member of struct MCU_DeviceHandle_S
 */
struct MCU_SPI_DeviceAttr_S
{
    Retcode_T (*MCU_SPI_SelectFuncPtr)(int32_t);   /**< Reference to the SPI device select function*/
    Retcode_T (*MCU_SPI_DeselectFuncPtr)(int32_t); /**< Reference to the SPI device deselect function */
};

/**
 * @brief       Structure representing the events that can be issued from the SPI in the callback function.
 */
struct MCU_SPI_Event_S
{
    uint32_t RxReady : 1;    /**< At least one byte has arrived.*/
    uint32_t RxError : 1;    /**< Receive error has occurred.*/
    uint32_t RxComplete : 1; /**< The expected bytes have been received.*/
    uint32_t TxError : 1;    /**< Transmit error has occurred.*/
    uint32_t TxComplete : 1; /**< All bytes passed with the last call are sent through the medium.*/
    uint32_t DataLoss : 1;   /**< Occurs upon bus error.*/
};
/**
 * @brief       Union combining event bitfield structure with a uint32_t value.
 */
union MCU_SPI_Event_U {
    struct MCU_SPI_Event_S bitfield; /**< Events structure as a bitfield */
    uint32_t registerValue;          /**< Stored value in the event structure (could be used to reset the overall bitfield structure) */
};

/**
 * @brief       Enumeration of special SPI interface error return codes
 */
enum RETCODE_HAL_SPI_E
{

    RETCODE_HAL_SPI_DRIVER_RX_NOT_READY = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_HAL_SPI_DRIVER_TX_NOT_READY,
    RETCODE_HAL_SPI_DRIVER_RX_NOT_RUN,
    RETCODE_HAL_SPI_DRIVER_RESSOURCE_NOT_AVAILABLE,
    RETCODE_SPI_INVALID_PERIPHERAL
};

/**
 * @brief       Type definition for the SPI event callback.
 *
 * @details     Upon initialization of SPI, in non blocking mode, the user should pass a reference to a function of
 *              this signature (**void function(SPI_T, struct MCU_SPI_Event_S)**), handling event notifications from
 *              the SPI driver.
 *
 * @param [in]  spi : Is the handle of the SPI instance that issued the event.
 *
 * @param [in]  event : Structure containing actual event information.
 */
typedef void (*MCU_SPI_Callback_T)(SPI_T spi, struct MCU_SPI_Event_S event);

/**
 * @brief       Initializes the SPI Interface.
 *
 * @details     This function will initialize the SPI driver and make it ready to operate, it does not influence the
 *              underlying SPI hardware it takes care of the software initialization only.
 *
 * @param [in]  spi : SPI handle.
 *
 * @param [in]  callback : A reference to a function which processes the SPI events issued from the SPI instance in
 *              non-blocking context. Passing a NULL pointer is allowed only in case the call mode is blocking.
 * @warning     The callback function is usually invoked in interrupt context. The implementation of the callback
 *              function must take this into account.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_SPI_Initialize(SPI_T spi, MCU_SPI_Callback_T callback);

/**
 * @brief       Deinitializes the SPI instance.
 *
 * @details     This function will deinitialize the SPI instance and clear, if applicable, its dynamically created
 *              context. This function does not influence the underlying SPI hardware it only takes care of the software
 *              deinitialization.
 *
 * @param [in]  spi : SPI handle.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_SPI_Deinitialize(SPI_T spi);

/**
 * @brief       Sends data via SPI
 *
 * @details      Depending on the prior initialization, the send operation could be blocking or non blocking:
 *                  - In <b>blocking</b> mode the function returns when the count of sent bytes matches the expected
 *                  count, or if an error occurred. The return value will then inform about the success or failure of
 *                  the operation.
 *                  - In case of <b>non blocking</b> the function triggers the send operation and returns the
 *                  information about the success or failure of triggering it. The send process shall continue in
 *                  the background (DMA or interrupt driven) and the application will then be informed via callback
 *                  about the success (TxComplete) or failure (TxError, DataLoss) of the send operation.
 *
 * @warning     In non-blocking mode it is not allowed to call this function a second time before completion of the
 *              first send operation i.e. the callback reported TxComplete or TxError events.
 *
 * @note        The function only supports transfers with 8 data bits or less.
 *
 * @pre         MCU_SPI_Initialize() successfully executed and the target device is in ENABLED state.
 *
 * @param[in]   spi : Is the handle of the SPI.
 *
 * @param[out]  data : Is a reference to the buffer where data to send is located. The buffer must persist until the
 *              send operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param [in]  len : Is the number of data elements to send.
 * @note        Passing 0 to len will abort the ongoing send operation.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_SPI_Send(SPI_T spi, uint8_t *data, uint32_t len);

/**
 * @brief       Receives data via SPI
 *
 * @details      Depending on the prior initialization, the receive operation could be blocking or non blocking:
 *                  - In <b>blocking</b> mode the function returns when the count of received bytes matches the expected
 *                  count, or if an error occurred. The return value will then inform about the success or failure of
 *                  the operation.
 *                  - In case of <b>non-blocking</b> the function triggers the receive operation and returns the
 *                  information about the success or failure of triggering it. The receive process shall continue in
 *                  the background (DMA or interrupt driven) and the application will then be informed via callback
 *                  about the success (RxComplete) or failure (RxError, DataLoss) of the receive operation.
 *
 * @warning     In non-blocking mode, it is not allowed to call this function a second time before completion of the
 *              first receive operation.
 *
 * @note        The function only supports transfers in 8 data bits or less.
 *
 * @pre         The SPI instance has been initialized and the target device is in ENABLED state.
 *
 * @param[in]   spi : Is the handle of the SPI.
 *
 * @param[in]   buffer : Is a reference to the buffer where to store received data. The data buffer must persist until
 *              the receive operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param [in]  len : is the number of data elements expected to be received.
 * @note        Passing 0 to len will abort the ongoing receive operation.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_SPI_Receive(SPI_T spi, uint8_t *buffer, uint32_t len);
/**
 * @brief       Transfers data via SPI (send while receive).
 *
 * @details      Depending on the prior initialization, the transfer operation could be blocking or non blocking:
 *                  - In <b>blocking</b> mode the function returns when the count of transferred bytes matches the
 *                  expected count, or if an error occurred. The return value will then inform about the success or
 *                  failure of the operation.
 *                  - In case of <b>non-blocking</b> the function triggers the transfer operation and returns the
 *                  information about the success or failure of triggering it. The transfer process shall continue in
 *                  the background (DMA or interrupt driven) and the application will then be informed via callback
 *                  about the success (RxComplete/TxComplete),  or failure (RxError/TxError, DataLoss) of the transfer
 *                  operation.
 *
 * @warning     In non-blocking mode, it is not allowed to call this function a second time before completion of the
 *              first transfer operation.
 *
 * @note        The function only supports transfers in 8 data bits or less.
 *
 * @pre         MCU_SPI_Initialize() successfully executed and the target device is in ENABLED state.
 *
 * @param[in]   spi : Is the handle of the SPI.
 *
 * @param[in]   data_out : Is a reference to buffer containing data to be sent. The data buffer must persist until
 *              the send operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param[in]   data_in : Is a reference to the buffer where to store received data. The data buffer must persist until
 *              the receive operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param [in]  numTransfer : is the number of data elements expected to be received.
 * @note        Passing 0 to numTransfer will abort the ongoing receive operation.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_SPI_Transfer(SPI_T spi, uint8_t *data_out, uint8_t *data_in, uint32_t numTransfer);

/**
 * @brief       Get the transfered data count.
 *
 * @details     The Function returns the number of currently transferred data items during MCU_SPI_Send(),
 *              MCU_SPI_Receive(), MCU_SPI_Transfer() operation.
 *
 * @param[in]   spi : SPI handle.
 *
 * @return      The count of transfered data.
 */
uint32_t MCU_SPI_GetDataCount(SPI_T spi);

#endif /* KISO_FEATURE_SPI */
#endif /* KISO_MCU_SPI_H */
/**  @} */
