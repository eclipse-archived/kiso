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
 * @brief Declares the I2C handle used by BSP and MCU for STM32 targets
 *
 */

#ifndef BCDS_MCU_STM32F7_I2C_HANDLE_H_
#define BCDS_MCU_STM32F7_I2C_HANDLE_H_

#include "BCDS_MCU_I2C.h"

#if BCDS_FEATURE_I2C

#include "stm32f7xx.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_i2c.h"

/**
 * @brief This data type represents a function pointer which is used between
 * BSP and MCU I2C as a callback whenever an IRQ event/error is to be notified
 * from the BSP to the MCU I2C driver.
 *
 * @param [in] i2c : Handle of the I2C whose IRQ event should be handled.
 */
typedef void (*MCU_BSPI2C_IRQ_Callback_T)(I2C_T i2c);

/**
 * @brief Declares a function pointer for master receive and transmit functions
 * which has the same signature like:
 *  - HAL_I2C_Master_Transmit_IT
 *  - HAL_I2C_Master_Receive_IT
 *  - HAL_I2C_Master_Transmit_DMA
 *  - HAL_I2C_Master_Receive_DMA
 */
typedef HAL_StatusTypeDef (*STM_HAL_RxTxFunPtr)(I2C_HandleTypeDef *hi2c,
        uint16_t DevAddress,
        uint8_t *pData,
        uint16_t Size);


/**
 * @brief   Structure which is used as I2C handle.
 *
 * @detail  The handle is usually created by the BSP and fetched by the
 * application layer to use the I2C interface.
 */
struct MCU_I2C_S
{
    /** Context struct of stm32 I2C driver*/
    I2C_HandleTypeDef hi2c;
    /** Function pointer of the MCU I2C EV IRQ callback handler which is used
     * by the BSP to notify MCU about IRQ events. Will be set by MCU I2C upon
     * initialization. */
    MCU_BSPI2C_IRQ_Callback_T IRQCallback;
    /** Function pointer of the MCU I2C Error IRQ callback handler which is
     * used by the BSP to notify MCU about IRQ error events. Will be set by MCU
     * I2C upon initialization. */
    MCU_BSPI2C_IRQ_Callback_T ERRCallback;
    /** Function pointer of the MCU I2C DMA Rx callback handler which is used
     * by the BSP to notify MCU about DMA Rx events. Will be set by MCU I2C
     * upon initialization and if transfer mode is set to DMA.*/
    MCU_BSPI2C_IRQ_Callback_T DMARxCallback;
    /** Function pointer of the MCU I2C DMA Tx callback handler which is used
     * by the BSP to notify MCU about DMA Tx events. Will be set by MCU I2C
     * upon initialization and if transfer mode is set to DMA.*/
    MCU_BSPI2C_IRQ_Callback_T DMATxCallback;
    /** Index of the MCU I2C interface. Set by MCU I2C upon initialization. */
    uint32_t Index;
    /** Identifies the transfer mode that is currently configured for this I2C
     * instance. This value will be set by the BSP upon configuration of the
     * I2C interface.*/
    enum BCDS_HAL_TransferMode_E TransferMode;

    /** Callback function pointer that has been passed with a call to the
     * initialize function. It is used to callback the application layer when
     * needed.*/
    MCU_I2C_Callback_T AppLayerCallback;
    /** Function pointer to the vendor library Tx function. Will be set
     *  upon initialization. */
    HAL_StatusTypeDef (*TxFunPtr)(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
            uint8_t *pData, uint16_t Size);

    /** Function pointer to the vendor library Rx function. Will be set upon
     *  initialization.*/
    HAL_StatusTypeDef (*RxFunPtr)(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
            uint8_t *pData, uint16_t Size);
};

#endif /* BCDS_FEATURE_I2C && BCDS_I2C_COUNT */

#endif /* BCDS_MCU_STM32F7_I2C_HANDLE_H_ */
