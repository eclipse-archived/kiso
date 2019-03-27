/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
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

/** @ingroup BSP_BSE_BOARD_REFCONFIG Reference configuration for BSE BSP reference board */

#ifndef BCDS_FEATURE_HAL_TICK_HANDLER
#define BCDS_FEATURE_HAL_TICK_HANDLER                                           1
#endif /* ifndef BCDS_FEATURE_HAL_TICK_HANDLER */

#ifndef BCDS_FEATURE_UART
#define BCDS_FEATURE_UART                                                       1
#endif /* ifndef BCDS_FEATURE_UART */

#ifndef BCDS_FEATURE_LEUART
#define BCDS_FEATURE_LEUART                                                     0
#endif

#if (BCDS_FEATURE_UART) && !defined(BCDS_UART_COUNT)
#define BCDS_UART_COUNT                                                         4
#endif /* if (BCDS_FEATURE_UART) && !defined(BCDS_UART_COUNT) */

#ifndef BCDS_FEATURE_I2C
#define BCDS_FEATURE_I2C                                                        1
#endif /* ifndef BCDS_FEATURE_I2C */

#if (BCDS_FEATURE_I2C) && !defined(BCDS_I2C_COUNT)
#define BCDS_I2C_COUNT                                                          2
#endif /* if (BCDS_FEATURE_I2C) && !defined(BCDS_I2C_COUNT) */

#ifndef BCDS_FEATURE_SPI
#define BCDS_FEATURE_SPI                                                        1
#endif /* ifndef BCDS_FEATURE_SPI */

#if (BCDS_FEATURE_SPI) && !defined(BCDS_SPI_COUNT)
#define BCDS_SPI_COUNT                                                          1
#endif /* if (BCDS_FEATURE_SPI) && !defined(BCDS_SPI_COUNT) */

#ifndef BCDS_FEATURE_FLASH
#define BCDS_FEATURE_FLASH                                                      1
#endif /* ifndef BCDS_FEATURE_FLASH */

#ifndef BCDS_FEATURE_FLASH_INTERN
#define BCDS_FEATURE_FLASH_INTERN                                               1
#endif /* ifndef BCDS_FEATURE_FLASH_INTERN */

#ifndef BCDS_FEATURE_MCU_CRC
#define BCDS_FEATURE_MCU_CRC                                                    1
#endif /* ifndef BCDS_FEATURE_MCU_CRC */

#ifndef BCDS_FEATURE_WATCHDOG
#define BCDS_FEATURE_WATCHDOG                                                   1
#endif /* ifndef BCDS_FEATURE_WATCHDOG */

#ifndef BCDS_FEATURE_PWM
#define BCDS_FEATURE_PWM                                                        1
#endif /* ifndef BCDS_FEATURE_PWM */

#ifndef BCDS_FEATURE_RNG
#define BCDS_FEATURE_RNG                                                        1
#endif /* ifndef BCDS_FEATURE_RNG */

#ifndef BCDS_FEATURE_BSP_GSM_SIM800H
#define BCDS_FEATURE_BSP_GSM_SIM800H                                            1
#endif /* ifndef BCDS_FEATURE_BSP_GSM_SIM800H */

#ifndef BCDS_FEATURE_BSP_CAN_SN65HVD234
#define BCDS_FEATURE_BSP_CAN_SN65HVD234                                         1
#endif /* ifndef BCDS_FEATURE_BSP_CAN_SN65HVD234 */

#ifndef BCDS_FEATURE_BSP_GNSS_G7020
#define BCDS_FEATURE_BSP_GNSS_G7020                                             1
#endif /* ifndef BCDS_FEATURE_BSP_GNSS_G7020 */

#ifndef BCDS_FEATURE_BSP_MEMORY_W25Q256FV
#define BCDS_FEATURE_BSP_MEMORY_W25Q256FV                                       1
#endif /* ifndef BCDS_FEATURE_BSP_MEMORY_W25Q256FV */

#ifndef BCDS_FEATURE_BSP_BT_EM9301
#define BCDS_FEATURE_BSP_BT_EM9301                                              1
#endif /* ifndef BCDS_FEATURE_BSP_BT_EM9301 */

#ifndef BCDS_FEATURE_BSP_SENSOR_NODE
#define BCDS_FEATURE_BSP_SENSOR_NODE                                            1
#endif /* ifndef BCDS_FEATURE_BSP_SENSOR_NODE */

#ifndef BCDS_FEATURE_BSP_PWM_BUZZER
#define BCDS_FEATURE_BSP_PWM_BUZZER                                             0
#endif /* ifndef BCDS_FEATURE_BSP_PWM_BUZZER */

#ifndef BCDS_FEATURE_BSP_BUTTON
#define BCDS_FEATURE_BSP_BUTTON                                                 1
#endif /* ifndef BCDS_FEATURE_BSP_BUTTON */

#ifndef BCDS_FEATURE_BSP_CHARGER_BQ2407X
#define BCDS_FEATURE_BSP_CHARGER_BQ2407X                                        1
#endif /* ifndef BCDS_FEATURE_BSP_CHARGER_BQ2407X */

#ifndef BCDS_FEATURE_BSP_TEST_INTERFACE
#define BCDS_FEATURE_BSP_TEST_INTERFACE                                         1
#endif /* ifndef BCDS_FEATURE_BSP_TEST_INTERFACE */

#ifndef BCDS_FEATURE_BSP_LED_LM2755
#define BCDS_FEATURE_BSP_LED_LM2755                                             1
#endif /* ifndef BCDS_FEATURE_BSP_LED_LM2755 */

#ifndef BCDS_FEATURE_BSP_RNG
#define BCDS_FEATURE_BSP_RNG                                                    1
#endif /* ifndef BCDS_FEATURE_BSP_RNG */

#ifndef BCDS_FEATURE_BSP_STARTUP
#define BCDS_FEATURE_BSP_STARTUP                                                1
#endif /* ifndef BCDS_FEATURE_BSP_STARTUP */

#ifndef BCDS_FEATURE_BSP_RTOS_IN_USE
#define BCDS_FEATURE_BSP_RTOS_IN_USE                                            1
#endif /* ifndef BCDS_FEATURE_BSP_RTOS_IN_USE */

#ifndef BCDS_FEATURE_BSP_UMTS_LISAU2
#define BCDS_FEATURE_BSP_UMTS_LISAU2                                            1
#endif /* ifndef BCDS_FEATURE_BSP_UMTS_LISAU2 */



#define BCDS_FEATURE_HAL_TICK_HANDLER                                           1

#endif /*BCDS_HAL_CONFIG_H_*/
