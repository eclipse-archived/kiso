/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) 2015-2018 Bosch Connected Devices and Solutions GmbH. 
 * Copyright (C) 2019 Robert Bosch GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @file
 * @defgroup    BCDS_HAL_MCU_I2C MCU I2C
 * @ingroup     BCDS_HAL_MCU_IF
 * @{
 *
 * @brief       Unified I2C Master Interface
 *
 * @details
 * The I2C hardware gets initialized by the Board support package. For an I2C device, the BSP API
 * exposes a getHandle() method which provides a handle for the I2C communication interface in use. That handle is used
 * by this driver to send and receive data.
 *
 * A typical use of this API would start with initializing the I2C instance by calling
 * MCU_I2C_Initialize(myI2CHandle, myI2CCallBack) where myI2CHandle is a reference to the I2C instance used by myDevice
 * and myI2CCallback is a callback function in the application layer which, in case the call mode is not blocking, will
 * be called to process I2C events.
 * After this, and if myDevice is in ENABLED state, the application/middleware should be able to use the other API
 * methods MCU_I2C_Send(), MCU_I2C_Receive(), etc...
 * a possible use case would be as follows:
 * @code
 * Retcode_T retcode;
 * retcode = BSP_MyDevice_Enable();
 * if(RETCODE_OK == retcode)
 * {
 *      myI2CHandle = BSP_MyDevice_GetHandle();
 *      retcode = MCU_I2C_Initialize(myI2CHandle, myI2CCallBack);
 * }
 * if(RETCODE_OK == retcode)
 * {
 *      retcode= MCU_I2C_ReadRegister();
 * }
 * if(RETCODE_OK == retcode)
 * {
 *      //do something
 * }
 * @endcode
 *
 * @note        It is possible to disable the entire I2C feature in the HAL-BSP configuration file. I.e. the I2C code
 * will only be compiled and linked if the feature <b>BCDS_FEATURE_I2C</b> is set to 1 in the configuration file.
 *
 * @note        This interface only supports the I2C master mode.
 *
 * @note        This I2C interface only supports 7 bit addresses for slave addressing.
 */
#ifndef BCDS_MCU_I2C_H
#define BCDS_MCU_I2C_H

#include "BCDS_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_I2C

/**
 * @brief       Type definition for the I2C handle which is used to identify the I2C instance to work with.
 */
typedef HWHandle_T I2C_T;

/**
 * @brief       Bitfield structure to represent the events that can be issued from the I2C in the callback function.
 */
struct MCU_I2C_Event_S
{
    uint32_t RxReady        :1; /**< At least one byte has arrived after the call to MCU_UART_Receive(). */
    uint32_t TransferError  :1; /**< A Transfer error has occurred. */
    uint32_t RxComplete     :1; /**< The expected number of bytes have been received. */
    uint32_t TxComplete     :1; /**< All bytes passed with the last call to MCU_UART_Send() are sent. */
};

/**
 * @brief       Union of MCU_I2C_Event_S and a scalar uint32_t for optimized access.
 */
union MCU_I2C_Event_U
{
    uint32_t value; /**< Stored value in the event structure (could be used to reset the overall bitfield structure).*/
    struct MCU_I2C_Event_S bits;/**< Events structure as a bitfield.*/
};

/**
 * @brief       Type definition for the I2C event callback.
 *
 * @details     Upon initialization of I2C, in non-blocking mode, the user should pass a reference to a function of
 *              this signature (**void function(I2C_T, struct MCU_I2C_Event_S)**), handling event notifications from
 *              the I2C driver.
 *
 * @param [in]  i2c : Is the handle of the I2C instance that issued the event.
 *
 * @param [in]  event : Structure containing actual event information.
 */
typedef void (*MCU_I2C_Callback_T)(I2C_T i2c, struct MCU_I2C_Event_S event);

/**
 * @brief       Initializes the I2C Interface.
 *
 * @details     This function will initialize the I2C driver and make it ready to operate, it does not influence the
 *              underlying I2C hardware it takes care of the software initialization only.
 *
 * @note        All other I2C API calls require a call to MCU_I2C_Initialize() first else they shall report an error.
 *
 * @param [in]  i2c : I2C handle, a reference to the I2C object instance to be initialized.
 *
 * @param [in]  callback :a reference to a function which processes the I2C events,in non-blocking context, which come
 *              from the i2c instance to be initialized. Passing a NULL pointer is allowed only in case the call mode
 *              is blocking.
 * @warning     The callback function is usually invoked in interrupt context. The implementation of the callback
 *              function must take this into account.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_I2C_Initialize(I2C_T i2c, MCU_I2C_Callback_T callback);

/**
 * @brief       Deinitializes the I2C instance.
 *
 * @details     This function will deinitialize the I2C instance and clear, if applicable, its dynamically created
 *              context. This function does not influence the underlying i2c hardware it only takes care of the software
 *              deinitialization.
 *
 * @param [in]  i2c : I2C handle.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_I2C_Deinitialize(I2C_T i2c);

/**
 * @brief       Sends data via I2C.
 *
 * @details      Depending on the prior initialization, the send operation could be blocking or non blocking:
 *                  - In <b>blocking</b> mode the function returns when the count of sent bytes matches the expected
 *                  count, or if an error occurred. The return value will then inform about the success or failure of
 *                  the operation.
 *                  - In the <b>non-blocking</b> case, this function just triggers the sending process. Data
 *                  transmission will happen after the return from this function. The return value informs only about
 *                  success or failure of triggering the transmission and the application will be informed about success
 *                  or failure of the data transmission via callback.
 *
 * @warning     In non-blocking mode it is not allowed to call this function a second time before completion of the
 *              first send operation i.e. the callback reported TxComplete or TxError events.
 *
 * @pre         MCU_I2C_Initialize() successfully executed and the targeted device is in ENABLED state.
 *
 * @param [in]  i2c : The handle of the I2C.
 *
 * @param [in]  slaveAddress : The I2C address of the slave to which to send the data.
 *
 * @param [in]  data : Is a reference to the buffer containing the data to be send. The the data buffer must persist
 *              until the send operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param [in]  len : Is the number of data elements to send. Passing 0 will abort any pending non-blocking transmission.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_I2C_Send(I2C_T i2c, uint16_t slaveAddress, uint8_t * data, uint32_t len);

/**
 * @brief       Receives data via I2C.
 *
 * @details     Depending on the prior initialization, the receive operation could be blocking non-blocking:
 *                  - In <b>blocking</b> mode the function returns when the count of received bytes matches the expected
 *                  count, or if an error occurred. The return value will then inform about the success or failure of
 *                  the operation.
 *                  - In the <b>non-blocking</b> case, this function just triggers the receive process. Data
 *                  transmission will happen after the return from this function. The return value informs only about
 *                  success or failure of triggering the transmission and the application will be informed about success
 *                  or failure of the data transmission via callback.
 *
 * @pre         MCU_I2C_Initialize() successfully executed and the targeted device is in ENABLED state.
 *
 * @param [in]  i2c : Is the handle of the I2C.
 *
 * @param [in]  slaveAddr : The I2C address of the slave from which to read the data.
 *
 * @param [out] buffer : Is a reference to the buffer where to store received data. The the data buffer must persist
 *              until the receive operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param [in]  len : Is the number of data elements expected to be received.
 * @note        Passing 0 to len will abort the ongoing receive operation.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_I2C_Receive(I2C_T i2c, uint16_t slaveAddr, uint8_t * buffer, uint32_t len);

/**
 * @brief       Reads from a device's register.
 *
 * @details     This function, if supported by the intended device itself, will read from a register specified by
 *              **registerAddr** a specified number of bytes **rxlen** and store them in the receive buffer
 *              **rxBuffer**.
 *
 * @pre         The MCU_I2C_Initialize() successfully executed and the targeted device is in ENABLED state.
 *
 * @param [in]  i2c : Is the handle of the I2C.
 *
 * @param [in]  slaveAddr : Is the I2C address of the connected device from which to read the register.
 *
 * @param [in]  registerAddr : Is the register address of the device to read.
 *
 * @param [out] rxBuffer : Is a reference to the buffer where to store received data. The the data buffer must persist
 *              until the receive operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param [in]  rxLen : Is the length the number of data elements expected to be received.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_I2C_ReadRegister(I2C_T i2c, uint16_t slaveAddr, uint8_t registerAddr, uint8_t * rxBuffer, uint32_t rxLen);

/**
 * @brief       Writes to a device's register.
 *
 * @details     This function, if supported by the intended device itself, will write to a register specified by
 *              **registerAddr** a specified number of bytes <B>txlen</B> and store them in the receive buffer
 *              **txdata**.
 *
 * @pre         MCU_I2C_Initialize() successfully executed and the targeted device is in ENABLED state.
 *
 * @param [in]  i2c : Is the handle of the I2C.
 *
 * @param [in]  slaveAddr : Is the I2C address of the connected device to which to write the register.
 *
 * @param [in]  registerAddr : Is the register address of the device to read.
 *
 * @param [in]  txdata : Is a reference to the buffer containing the data to be send. The the data buffer must persist
 *              until the send operation has finished either by return (in blocking context) or by callback
 *              (in non blocking context).
 *
 * @param [in]  txLen : Is number of data elements to be send.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_I2C_WriteRegister(I2C_T i2c, uint16_t slaveAddr,uint8_t registerAddr, uint8_t * txdata, uint32_t txLen);

#endif /* BCDS_FEATURE_I2C */
#endif /* BCDS_MCU_I2C_H */
/**  @} */
