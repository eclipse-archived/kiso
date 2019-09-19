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
 * @ingroup UTILS
 *
 * @defgroup I2CTRANSCEIVER I2CTransceiver
 * @{
 *
 * @brief
 *      Advanced API functions for sending and receiving via I2C
 *
 * @file
 */
#ifndef KISO_I2CTRANSCEIVER_H_
#define KISO_I2CTRANSCEIVER_H_

#include "Kiso_Utils.h"

#if KISO_FEATURE_I2CTRANSCEIVER
/* Include KISO header files */
#include "Kiso_Retcode.h"
#include "Kiso_HAL.h"
#if KISO_FEATURE_I2C
#include "Kiso_MCU_I2C.h"

/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "semphr.h"

/** Struct holding the I2C related configuration */
struct I2cTranceiverHandle_S
{
    bool InitializationStatus;
    /* I2C Handle used by the I2C driver in Essentials */
    I2C_T I2CHandle;
    /*semaphore used to synchronize the read/write process*/
    xSemaphoreHandle I2CBusSync;
    /*mutex used to synchronize the access of read/write process*/
    xSemaphoreHandle I2CMutexLock;
    /* status of I2C transfer*/
    int8_t I2cTransferStatusFlag;

};
typedef struct I2cTranceiverHandle_S I2cTranceiverHandle_T,*I2cTranceiverHandlePtr_T;

/**
 * @brief
 *      Initializes the transceiver for the use with the passed I2C handle.
 *
 * @param [in] i2cTransceiver
 *      A pointer to be initialized for I2C transceiver context structure to be initialized.
 *
 * @param [in] i2cHandle
 *      The handle of the I2C to be used by the transceiver. The handle
 *      must be initialized before passing it here.
 *
 * @retval #RETCODE_OK
 *      If successfully initialized.
 * @retval #RETCODE_NULL_POINTER
 *      If any of the parameter is NULL.
 * @retval #RETCODE_SEMAPHORE_ERROR
 *      If the semaphores are not created (see #I2cTranceiverHandle_T.I2CBusSync
 *      and #I2cTranceiverHandle_T.I2CMutexLock).
 * @retval #RETCODE_DOPPLE_INITIALIZATION
 *      If you are trying to initialize the transceiver again.
 * 
 */
Retcode_T I2CTransceiver_Init(I2cTranceiverHandlePtr_T i2cTransceiver, I2C_T i2cHandle);

/**
 * @brief
 *      Function to read a register of a device connected to I2C
 *
 * @param[in]   i2cTransceiver
 *      A pointer to the transceiver.
 *
 * @param [in]  i2cAddr
 *      I2C address of the connected device from read has to happen.
 *
 * @param [in]  regAddr
 *      Register address of the device to read.
 *
 * @param [out] buffer
 *      Buffer in which the values will be stored.
 *
 * @param [in]  bytesToRead
 *      Number of bytes to be read.
 *
 * @retval #RETCODE_OK
 *      If successfully read the register.
 * @retval #RETCODE_UNINITIALIZED
 *      If called without initializing.
 * @retval #RETCODE_NULL_POINTER
 *      If any of the parameter is NULL.
 * @retval #RETCODE_SEMAPHORE_ERROR
 *      If semaphore could not be taken within given time.
 * @retval #RETCODE_FAILURE
 *      If I2C transfer is not successful.
 * 
 */
Retcode_T I2CTransceiver_Read(I2cTranceiverHandlePtr_T i2cTransceiver,uint8_t i2cAddr,uint8_t regAddr,uint8_t *buffer,uint8_t bytesToRead );

/**
 * @brief
 *      Function to write a register of a device connected to I2C
 *
 * @param[in]   i2cTransceiver
 *      A pointer to the transceiver.
 *
 * @param [in]  i2cAddr
 *      I2C address of the connected device to which write has to happen.
 *
 * @param [in]  regAddr
 *      Register address of the device to write.
 *
 * @param [in] buffer
 *      Buffer in which the values to be written are stored.
 *
 * @param [in]  bytesToWrite
 *      Number of bytes to be written.
 *
 * @retval #RETCODE_OK
 *      If successfully written to the register.
 * @retval #RETCODE_UNINITIALIZED
 *      If called without initializing.
 * @retval #RETCODE_NULL_POINTER
 *      If any of the parameter is NULL.
 * @retval #RETCODE_SEMAPHORE_ERROR
 *      If semaphore could not be taken within given time.
 * @retval #RETCODE_FAILURE
 *      If I2C transfer is not successful.
 * 
 */

Retcode_T I2CTransceiver_Write(I2cTranceiverHandlePtr_T i2cTransceiver,uint8_t i2cAddr,uint8_t regAddr,uint8_t *buffer,uint8_t bytesToWrite);

/**
 * @brief
 *      Function to loop the I2C callback.
 *
 * @details
 *      It is necessary for the I2CTransceiver to loop the I2Ccallback.
 *      This means, the middle-layer driver which uses I2C and I2CTransceiver
 *      must initialize I2C with a callback function which invokes
 *      this function in order to loop the callback from I2C to the
 *      transceiver.
 *
 * @param[in] transceiver
 *      A pointer to the transceiver.
 *
 * @param[in] event
 *      The event which is notified by the callback
 * 
 */
void I2CTransceiver_LoopCallback(I2cTranceiverHandlePtr_T transceiver,struct MCU_I2C_Event_S event);

/**
 * @brief
 *      De-initializes the transceiver.
 *
 * @details
 *      The transceiver must not be used after calling this function.
 *
 * @param[in] transceiver
 *      A pointer to the transceiver context structure to be de-initialized
 *
 * @retval #RETCODE_OK
 *      If successfully de-initialized
 * @retval #RETCODE_INVALID_PARAM
 *      If the transceiver pointer parameter is NULL
 * 
 */
Retcode_T I2CTransceiver_Deinit(I2cTranceiverHandlePtr_T transceiver);
#endif /* KISO_FEATURE_I2C */

#endif /* KISO_FEATURE_I2CTRANSCEIVER */

#endif /* KISO_I2CTRANSCEIVER_H_ */

/**@} */
