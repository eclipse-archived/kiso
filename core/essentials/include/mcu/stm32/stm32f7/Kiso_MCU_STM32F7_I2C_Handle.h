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
 * @brief       
 * Declares the I2C handle used by BSP and MCU for STM32FF7 targets
 *
 */

#ifndef KISO_MCU_STM32F7_I2C_HANDLE_H_
#define KISO_MCU_STM32F7_I2C_HANDLE_H_

#include "Kiso_MCU_I2C.h"

#if KISO_FEATURE_I2C

#include "stm32f7xx.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_i2c.h"

/**
 * @brief       Enumerates the driver state machine states.
 */
typedef enum MCU_I2C_State_E
{
    I2C_STATE_INIT,
    I2C_STATE_READY,
    I2C_STATE_TX,
    I2C_STATE_RX,
} MCU_I2C_State_T;

/**
 * @brief       Structure used internally for i2c transactions.
 */
struct MCU_I2C_Transaction_S
{
    uint16_t DevAddress;  /**< Slave address of the I2C device target of the transaction */
    uint8_t *pDataBuffer; /**< Reference to the data buffer used for the transaction (Read/Write) */
    uint16_t Size;        /**< Count of bytes to be transmitted or received */
    bool PrependRegAddr;  /**< If set to true the driver will send the provided RegisterAddr first and then continues the transaction (send or receive) */
    uint8_t RegisterAddr; /**< The register address to be written */
};

/**
 * @brief       Structure which is used as I2C handle.
 *
 * @details     The handle is usually created by the BSP and fetched by the application layer to use the I2C interface.
 */
struct MCU_I2C_S; /* forward declaration of the structure */
struct MCU_I2C_S
{
    I2C_HandleTypeDef hi2c;                                   /**< Context struct of stm32 I2C driver*/
    enum KISO_HAL_TransferMode_E TransferMode;                /**< The transfer mode configured for this I2C instance. This value will be set by the BSP upon configuration of the I2C interface.*/
    uint32_t DataRate;                                        /**< Data rate in bits per second as configured by the BSP */
    MCU_I2C_State_T State;                                    /**< Current State of the I2C transceiver */
    void (*IRQCallback)(I2C_T i2c);                           /**< Reference to the I2C_EV IRQ handler. Will be set by MCU I2C upon initialization.*/
    void (*ERRCallback)(I2C_T i2c);                           /**< Reference to the I2C_ER IRQ handler. Will be set by MCU I2C upon initialization.*/
    void (*DMARxCallback)(I2C_T i2c);                         /**< Reference to the DMA Rx IRQ handler. Will be set by MCU I2C upon initialization.*/
    void (*DMATxCallback)(I2C_T i2c);                         /**< Reference to the DMA Tx IRQ handler. Will be set by MCU I2C upon initialization.*/
    MCU_I2C_Callback_T AppLayerCallback;                      /**< Reference to the application callback which will be executed for event notification to the upper layers. */
    Retcode_T (*SendFunPtr)(struct MCU_I2C_S *pi2c);          /**< Reference to the send function. Will be set upon initialization depending on the mode in use. */
    Retcode_T (*ReceiveFunPtr)(struct MCU_I2C_S *pi2c);       /**< Reference to the receive function. Will be set upon initialization depending on the mode in use. */
    Retcode_T (*ReadRegisterFunPtr)(struct MCU_I2C_S *pi2c);  /**< Reference to the read register function. Will be set upon initialization depending on the mode in use. */
    Retcode_T (*WriteRegisterFunPtr)(struct MCU_I2C_S *pi2c); /**< Reference to the write register function. Will be set upon initialization depending on the mode in use. */
    Retcode_T (*CancelFunPtr)(struct MCU_I2C_S *pi2c);        /**< Reference to the cancel function. Will be set upon initialization.*/
    struct MCU_I2C_Transaction_S Transaction;                 /**< Current transaction parameters */
};

#endif /* KISO_FEATURE_I2C */
#endif /* KISO_MCU_STM32F7_I2C_HANDLE_H_ */
