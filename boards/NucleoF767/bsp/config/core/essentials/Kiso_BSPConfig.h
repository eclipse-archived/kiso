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
 * @defgroup    HALCONFIG   NucleoF767 HAL Configuration
 * @ingroup     KISO_NUCLEOF767
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
 *  #define KISO_FEATURE_BSP_LED     1   // Enable BSP feature LED
 *  #define KISO_FEATURE_BSP_BUTTON  0   // Disable BSP feature Button
 * ```
 * If a define is missing in this header the feature will be disabled by default.
 */

#ifndef KISO_BSPCONFIG_H_
#define KISO_BSPCONFIG_H_

// clang-format off

/* BSP Features */
#ifndef KISO_FEATURE_BSP_LED
#define KISO_FEATURE_BSP_LED 1
#endif

#ifndef KISO_FEATURE_BSP_BUTTON
#define KISO_FEATURE_BSP_BUTTON 1
#endif

#ifndef KISO_FEATURE_BSP_GENERIC_UART
#define KISO_FEATURE_BSP_GENERIC_UART 1
#endif

#ifndef KISO_FEATURE_BSP_CELLULAR_SARAR4N4
#define KISO_FEATURE_BSP_CELLULAR_SARAR4N4 0
#endif

#ifndef KISO_FEATURE_BSP_GNSS_MAXM8
#define KISO_FEATURE_BSP_GNSS_MAXM8 0
#endif

#ifndef KISO_FEATURE_BSP_BMA280
#define KISO_FEATURE_BSP_BMA280 0
#endif

#ifndef KISO_FEATURE_BSP_BME280
#define KISO_FEATURE_BSP_BME280 0
#endif

#ifndef KISO_FEATURE_BSP_BLE_CC2640
#define KISO_FEATURE_BSP_BLE_CC2640 0
#endif

#ifndef KISO_FEATURE_BSP_MEMORY_W25
#define KISO_FEATURE_BSP_MEMORY_W25 0
#endif

#ifndef KISO_FEATURE_BSP_TEST_INTERFACE
#define KISO_FEATURE_BSP_TEST_INTERFACE 1
#endif

// clang-format on

#endif /* KISO_BSPCONFIG_H_ */
/**@} */
