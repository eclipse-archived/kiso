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

/* Enables the tick handler provided by the HAL */
#define BCDS_FEATURE_HAL_TICK_HANDLER                                           0

#define BCDS_FEATURE_UART                                                       1
#define BCDS_FEATURE_LEUART                                                     0
#define BCDS_FEATURE_I2C                                                        1
#define BCDS_FEATURE_SPI                                                        1
#define BCDS_FEATURE_FLASH                                                      1
#define BCDS_FEATURE_WATCHDOG                                                   0
#define BCDS_FEATURE_PWM                                                        0
#define BCDS_FEATURE_SLEEP                                                      1

#if (BCDS_FEATURE_UART)
/* Defines the default count of possible UART handles */
#define BCDS_UART_COUNT                                                         (UINT32_C(5))
#endif /* if (BCDS_FEATURE_UART) */

#if (BCDS_FEATURE_LEUART)
/* Defines the default count of possible LEUART handles */
#define BCDS_LEUART_COUNT                                                       (UINT32_C(1))
#endif /* if (BCDS_FEATURE_LEUART) */

#if (BCDS_FEATURE_I2C)
/* Defines the default count of possible I2C handles */
#define BCDS_I2C_COUNT                                                          (UINT32_C(2))
#endif /* if (BCDS_FEATURE_I2C) */

#if (BCDS_FEATURE_SPI)
/* Defines the default count of possible SPI handles */
#define BCDS_SPI_COUNT                                                          (UINT32_C(2))
#endif /* if (BCDS_FEATURE_SPI) */

/* Defines the BSp features which are supported by the iRoll board BSP */
#define BCDS_HAL_BSP_MODULE_ID_SENSOR_NODE                                      0
#define BCDS_HAL_BSP_MODULE_ID_TEST_INTERFACE                                   0
#define BCDS_FEATURE_BSP_ACCEL_H3LIS                                  			1

/* Define the BSP features which are supported by the XDK board BSP */
#define BCDS_FEATURE_BSP_PWM_LED                                                0
#define BCDS_FEATURE_BSP_LED                                                    0
#define BCDS_FEATURE_BSP_LED_LM2755                                             0
#define BCDS_FEATURE_BSP_STARTUP                                                0
#define BCDS_FEATURE_BSP_GSM_SIM800H                                            0
#define BCDS_FEATURE_BSP_CAN_SN65HVD234                                         0
#define BCDS_FEATURE_BSP_GNSS_G7020                                             0
#define BCDS_FEATURE_BSP_MEMORY_W25Q256FV                                       0
#define BCDS_FEATURE_BSP_MEMORY_W25X40CL                                        1
#define BCDS_FEATURE_BSP_BT_EM9301                                              0
#define BCDS_FEATURE_BSP_SENSOR_NODE                                            1
#define BCDS_FEATURE_BSP_PWM_BUZZER                                             0
#define BCDS_FEATURE_BSP_BUTTON                                                 0
#define BCDS_FEATURE_BSP_CHARGER_BQ2407X                                        0
#define BCDS_FEATURE_BSP_TEST_INTERFACE                                         0
#define BCDS_FEATURE_BSP_WIFI_CC3100MOD                                         0
#define BCDS_FEATURE_BSP_MIC_AKU340                                             0
#define BCDS_FEATURE_BSP_UMTS_LISAU2                                            0
#define BCDS_FEATURE_BSP_CHARGER_BQ2405X                                        0
#define BCDS_FEATURE_BSP_GPS_NEO7N                                              0
#define BCDS_FEATURE_BSP_ACCEL_ADXL362                                          1
#define BCDS_FEATURE_BSP_MEMORY_AT45DB                                          1

/* Define settings for the activated features */
#if (BCDS_FEATURE_BSP_UMTS_LISAU2)
#define BCDS_FEATURE_BSP_UMTS_LISAU2_UART                                       1
#define BCDS_FEATURE_BSP_UMTS_LISAU2_SPI                                        0
#if !(BCDS_FEATURE_BSP_UMTS_LISAU2_UART != BCDS_FEATURE_BSP_UMTS_LISAU2_SPI)
#error "UMTS/HSPA cellular module LISAU2 can communicate only through one interface either UART or SPI"
#endif /* if !(BCDS_FEATURE_BSP_UMTS_LISAU2_UART != BCDS_FEATURE_BSP_UMTS_LISAU2_SPI) */
#endif /* if BCDS_FEATURE_BSP_UMTS_LISAU2 */

#if BCDS_FEATURE_BSP_SENSOR_NODE
#define BSP_XDK_SENSOR_BMA280_ENABLE_INTERRUPT                                  1
#define BSP_XDK_SENSOR_BMA280_FORCE_INTERRUPT                                   0
#define BSP_XDK_SENSOR_BMG160_ENABLE_INTERRUPT                                  1
#define BSP_XDK_SENSOR_BMM150_ENABLE_INTERRUPT                                  1
#define BSP_XDK_SENSOR_BMM150_FORCE_INTERRUPT                                   0
#define BSP_XDK_SENSOR_MAX44009_ENABLE_INTERRUPT                                0
#define BSP_XDK_SENSOR_BMI160_ENABLE_INTERRUPT                                  0
#define BSP_XDK_SENSOR_BMI160_FORCE_INTERRUPT                                   0
#endif /* BCDS_FEATURE_BSP_SENSOR_NODE */

#if BCDS_FEATURE_BSP_BUTTON
#define BSP_XDK_KEY1_INTERRUPT                                                  1
#define BSP_XDK_KEY1_INTERRUPT_FORCE                                            0
#define BSP_XDK_KEY2_INTERRUPT                                                  1
#define BSP_XDK_KEY2_INTERRUPT_FORCE                                            0
#endif /* BCDS_FEATURE_BSP_BUTTON */

#if BCDS_FEATURE_BSP_SD_CARD
#define BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT                                    1
#define BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT_FORCE                              0
#endif /*BCDS_FEATURE_BSP_SD_CARD*/

#endif /*BCDS_HAL_CONFIG_H_*/
