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
 * @defgroup    KISO_HAL_MCU_GPIO  MCU GPIO
 * @ingroup     KISO_HAL_MCU_IF
 * @{
 *
 * @brief       Unified GPIO API
 *
 * @details     Interface for accessing GPIOs, i.e. initialize and deinitialize, read, write, toggle.
 */

#ifndef KISO_MCU_GPIO_H_
#define KISO_MCU_GPIO_H_

#include "Kiso_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_GPIO

/**
 * @brief       Typedef for the GPIO handle which is used to identify the GPIO HW context to use.
 */
typedef HWHandle_T GPIO_T;

/**
 * @brief       Enumerates Pin States
 */
enum MCU_GPIO_PinState_E
{
    MCU_GPIO_PIN_STATE_LOW,
    MCU_GPIO_PIN_STATE_HIGH,
    MCU_GPIO_PIN_STATE_TRISTATE,
    MCU_GPIO_PIN_STATE_OUT_OF_RANGE
};
typedef enum MCU_GPIO_PinState_E MCU_GPIO_PinState_T;

/**
 * @brief       Initializes the GPIO context.
 *
 * @param[in]   gpio : the GPIO pin context to be initialized
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_GPIO_Initialize(GPIO_T gpio);

/**
 * @brief       Deinitializes the GPIO context.
 */
Retcode_T MCU_GPIO_DeInitialize(GPIO_T gpio);

/**
 * @brief       Modify the output level of a GPIO pin from the GPIO perspective.
 *
 * @param[in]   gpio : the GPIO pin context (optional can be null, when not used).
 *
 * @param[in]   gpioLevel: the GPIO pin output level to be set.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_GPIO_WritePin(GPIO_T gpio, MCU_GPIO_PinState_T gpioLevel );

/**
 * @brief       Read the output level of a GPIO pin from the GPIO perspective.
 *
 * @param[in]   gpio : the GPIO context (optional can be null, when not used).
 *
 * @param[out]  gpioLevel: the GPIO's pin current input level being read.
 *
 * @return      RETCODE_OK in case of success error code otherwise.
 */
Retcode_T MCU_GPIO_ReadPin(GPIO_T gpio, MCU_GPIO_PinState_T *gpioLevel );

/**
 * @brief       Toggle the output level of a GPIO pin.
 *
 * @param[in]   gpio : the GPIO context (optional can be null, when not used).
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_GPIO_TogglePin(GPIO_T gpio);

#endif /* KISO_FEATURE_GPIO */
#endif /* KISO_MCU_GPIO_H_ */
/**  @} */

