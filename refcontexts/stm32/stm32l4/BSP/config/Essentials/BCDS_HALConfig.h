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
 * @brief Contains a default configuration to enable or disable HAL components.
 * @details This header file is usually included by the BCDS_HAL.h from the
 * HAL (Hardware Abstraction Layer) module. It is used to allow a per project
 * configuration of the features provided by the HAL component.
 *
 * Features are enabled or disabled by defining a particular feature to either
 * 1 or 0 e.g.
 * @code
 *   #define BCDS_FEATURE_I2C    1   // Enable HAL feature I2C
 *   #define BCDS_FEATURE_SPI    0   // Disable HAL feature SPI
 * If no defines are made then all HAL features will be enabled.
 */
#ifndef BCDS_HAL_CONFIG_H_
#define BCDS_HAL_CONFIG_H_

#include "BCDS_Essentials_Version.h"

#if (BCDS_ESSENTIALS_VERSION_MAJOR != 1) || (BCDS_ESSENTIALS_VERSION_MINOR != 1) || (BCDS_ESSENTIALS_VERSION_PATCH != 0)
#error "The BSP is only compatible with Essentials v1.1.0"
#endif /* Essentials package compatibility test */

/* Define the HAL/MCU features which are supported by this package */
#define BCDS_FEATURE_FLASH                                                      1
#define BCDS_FEATURE_GPIO                                                       1
#define BCDS_FEATURE_I2C                                                        1
#define BCDS_I2C_COUNT                                                          2
#define BCDS_FEATURE_LEUART                                                     0
#define BCDS_LEUART_COUNT                                                       0
#define BCDS_FEATURE_PWM                                                        1
#define BCDS_FEATURE_SLEEP                                                      1
#define BCDS_FEATURE_SPI                                                        1
#define BCDS_SPI_COUNT                                                          1
#define BCDS_FEATURE_TIMER                                                      1
#define BCDS_FEATURE_UART                                                       1
#define BCDS_UART_COUNT                                                         4
#define BCDS_FEATURE_WATCHDOG                                                   1
#define BCDS_FEATURE_RNG                                                        1

/* Define the HAL/BSP features which are supported by this package */
#define BCDS_FEATURE_BSP_BT_EM9301                                              1
#define BCDS_FEATURE_BSP_BUTTON                                                 1
#define BCDS_FEATURE_BSP_CAN_SN65HVD234                                         1
#define BCDS_FEATURE_BSP_CHARGER_BQ2405X                                        0
#define BCDS_FEATURE_BSP_CHARGER_BQ2407X                                        1
#define BCDS_FEATURE_BSP_GNSS_G7020                                             1
#define BCDS_FEATURE_BSP_GPS_NEO7N                                              0
#define BCDS_FEATURE_BSP_GSM_SIM800H                                            1
#define BCDS_FEATURE_BSP_LED                                                    0
#define BCDS_FEATURE_BSP_LED_LM2755                                             1
#define BCDS_FEATURE_BSP_LORA_RN2XX3                                            0
#define BCDS_FEATURE_BSP_MEMORY_S25FL116K                                       0
#define BCDS_FEATURE_BSP_MEMORY_W25Q256FV                                       1
#define BCDS_FEATURE_BSP_MIC_AKU340                                             0
#define BCDS_FEATURE_BSP_PWM_BUZZER                                             1
#define BCDS_FEATURE_BSP_PWM_LED                                                0
#define BCDS_FEATURE_BSP_SDCARD                                                 0
#define BCDS_FEATURE_BSP_SENSOR_NODE                                            1
#define BCDS_FEATURE_BSP_TEST_INTERFACE                                         1
#define BCDS_FEATURE_BSP_UMTS_LISAU2                                            0
#define BCDS_FEATURE_BSP_WIFI_CC3100MOD                                         0
#define BCDS_FEATURE_BSP_RNG                                                    1

/* Define the BSP features which are supported by this package */
#define BCDS_FEATURE_BSP_RTOS_IN_USE                                            0
#define BCDS_FEATURE_BSP_VECTOR_CAN                                             1
#define BCDS_FEATURE_BSP_CUSTOM_CAN                                             0
#define BCDS_FEATURE_BSP_STARTUP                                                1

#define BCDS_FEATURE_HAL_TICK_HANDLER                                           1

#endif /*BCDS_HAL_CONFIG_H_*/
