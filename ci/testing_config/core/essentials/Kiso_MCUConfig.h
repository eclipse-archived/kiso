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
 * @defgroup    MCUCONFIG   MCU HAL Configuration
 * @ingroup     KISO_ESSENTIALS
 * @{
 * @brief Configures which hardware capabilities the board makes available.
 *
 * @details
 * ```
 * This header file is included by the Kiso_HAL.h from the HAL (Hardware
 * Abstraction Layer) module. It is used to allow a per project configuration of
 * the features provided by the HAL.
 *
 * Features are enabled or disabled by defining a particular feature to either
 * 1 or 0 e.g.
 * ```c
 *  #define KISO_FEATURE_I2C    1   // Enable HAL feature I2C
 *  define KISO_FEATURE_SPI    0   // Disable HAL feature SPI
 * ```
 * If a define is missing in this header the feature will be disabled by default.
 */

#ifndef KISO_MCUCONFIG_H_
#define KISO_MCUCONFIG_H_

// clang-format off

/* MCU Features */
#define KISO_FEATURE_UART     1
#define KISO_FEATURE_I2C      1
#define KISO_FEATURE_SPI      1
#define KISO_FEATURE_FLASH    1
/* \todo: Enable watchdog feature once unit-tests are compilable */
#define KISO_FEATURE_WATCHDOG 0
#define KISO_FEATURE_PWM      1
#define KISO_FEATURE_SLEEP    1
#define KISO_FEATURE_RNG      1
#define KISO_FEATURE_MCU_CRC  1

// clang-format on

#endif /* KISO_MCUCONFIG_H_ */
/**@} */
