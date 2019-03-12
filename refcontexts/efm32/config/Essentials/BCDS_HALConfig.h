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

/** Enables the tick handler provided by the HAL */
#define BCDS_FEATURE_HAL_TICK_HANDLER                                           1

#ifndef BCDS_FEATURE_UART
#define BCDS_FEATURE_UART                                                       1
#endif

#ifndef BCDS_FEATURE_LEUART
#define BCDS_FEATURE_LEUART                                                     0
#endif

#ifndef BCDS_LEUART_CC
#define BCDS_LEUART_CC															0
#endif

#ifndef BCDS_FEATURE_DMA
#define BCDS_FEATURE_DMA                                                        0
#endif

#ifndef BCDS_FEATURE_EFM32_BURTC
#define BCDS_FEATURE_EFM32_BURTC                                                0
#endif

#ifndef BCDS_FEATURE_I2C
#define BCDS_FEATURE_I2C                                                        1
#endif

#ifndef BCDS_FEATURE_BSP_EXTENSIONPORT
#define BCDS_FEATURE_BSP_EXTENSIONPORT     									    1
#endif

#define BCDS_FEATURE_SPI                                                        1
#define BCDS_FEATURE_FLASH                                                      1
#define BCDS_FEATURE_WATCHDOG                                                   1
#define BCDS_FEATURE_PWM                                                        1
#define BCDS_FEATURE_TIMER                                                      1
#define BCDS_FEATURE_SLEEP                                                      1
#define BCDS_UART_COUNT                                                         5
#define BCDS_LEUART_COUNT                                                       1
#define BCDS_I2C_COUNT                                                          2
#define BCDS_SPI_COUNT                                                          2
#define BCDS_FEATURE_GPIO														1
#define BCDS_FEATURE_EFM32_RTC													1
/** Define the BSP features which are supported by this package */
#ifndef BCDS_FEATURE_BSP_PWM_LED
#define BCDS_FEATURE_BSP_PWM_LED                                                0
#endif

#ifndef BCDS_FEATURE_BSP_LED
#define BCDS_FEATURE_BSP_LED                                                    1
#endif

#ifndef BCDS_FEATURE_BSP_LED_EXT
#define BCDS_FEATURE_BSP_LED_EXT                                                1
#endif

#ifndef BCDS_FEATURE_BSP_GSM_SIM800H
#define BCDS_FEATURE_BSP_GSM_SIM800H                                            0
#endif /* ifndef BCDS_FEATURE_BSP_GSM_SIM800H */

#ifndef BCDS_FEATURE_BSP_CAN_SN65HVD234
#define BCDS_FEATURE_BSP_CAN_SN65HVD234                                         0
#endif

#ifndef BCDS_FEATURE_BSP_GNSS_G7020
#define BCDS_FEATURE_BSP_GNSS_G7020                                             0
#endif

#ifndef BCDS_FEATURE_BSP_MEMORY_W25Q256FV
#define BCDS_FEATURE_BSP_MEMORY_W25Q256FV                                       0
#endif

#ifndef BCDS_FEATURE_BSP_BT_EM9301
#define BCDS_FEATURE_BSP_BT_EM9301                                              1
#endif

#ifndef BCDS_FEATURE_BSP_SENSOR_NODE
#define BCDS_FEATURE_BSP_SENSOR_NODE                                            1
#endif

#ifndef BCDS_FEATURE_BSP_PWM_BUZZER
#define BCDS_FEATURE_BSP_PWM_BUZZER                                             0
#endif

#ifndef BCDS_FEATURE_BSP_BUTTON
#define BCDS_FEATURE_BSP_BUTTON                                                 1
#endif

#ifndef BCDS_FEATURE_BSP_BUTTON_EXT
#define BCDS_FEATURE_BSP_BUTTON_EXT                                             1
#endif

#ifndef BCDS_FEATURE_BSP_CHARGER_BQ2407X
#define BCDS_FEATURE_BSP_CHARGER_BQ2407X                                        1
#endif

#define BCDS_FEATURE_BSP_TEST_INTERFACE                                         1

#ifndef BCDS_FEATURE_BSP_WIFI_CC3100MOD
#define BCDS_FEATURE_BSP_WIFI_CC3100MOD                                         1
#endif

#ifndef BCDS_FEATURE_BSP_SD_CARD
#define BCDS_FEATURE_BSP_SD_CARD                                                1
#endif

#ifndef BCDS_FEATURE_BSP_MAX31865
#define BCDS_FEATURE_BSP_MAX31865           									1
#endif

#ifndef BCDS_FEATURE_BSP_EXTENSION_GPIO
#define BCDS_FEATURE_BSP_EXTENSION_GPIO      									1
#endif

#ifndef BCDS_FEATURE_BSP_MIC_AKU340
#define BCDS_FEATURE_BSP_MIC_AKU340                                             1
#endif

#ifndef BCDS_FEATURE_BSP_UMTS_LISAU2
#define BCDS_FEATURE_BSP_UMTS_LISAU2                                            1
#endif

#ifndef BCDS_FEATURE_BSP_UMTS_QUECTELUC20
#define BCDS_FEATURE_BSP_UMTS_QUECTELUC20                                       0
#endif

#ifndef BCDS_FEATURE_BSP_CHARGER_BQ2405X
#define BCDS_FEATURE_BSP_CHARGER_BQ2405X                                        1
#endif

#ifndef BCDS_FEATURE_BSP_GPS_NEO7N
#define BCDS_FEATURE_BSP_GPS_NEO7N                                              0
#endif

#ifndef BCDS_FEATURE_BSP_USB
#define BCDS_FEATURE_BSP_USB                                                    1
#endif

#ifndef BCDS_FEATURE_BSP_IRSENSOR_NODE
#define BCDS_FEATURE_BSP_IRSENSOR_NODE                                          1
#endif

#ifndef BCDS_FEATURE_BSP_LORA_RN2XX3
#define BCDS_FEATURE_BSP_LORA_RN2XX3                                            0
#endif

#ifndef BCDS_FEATURE_BSP_LORA_SX1272
#define BCDS_FEATURE_BSP_LORA_SX1272                                            0
#endif
/** Define settings for the activated features */

#if BCDS_FEATURE_BSP_UMTS_LISAU2
#define BCDS_FEATURE_BSP_UMTS_LISAU2_UART                                       1

#define BCDS_FEATURE_BSP_UMTS_LISAU2_SPI                                        0

#if !(BCDS_FEATURE_BSP_UMTS_LISAU2_UART != BCDS_FEATURE_BSP_UMTS_LISAU2_SPI)
#error "UMTS/HSPA cellular module LISAU2 can communicate only through one interface either UART or SPI"
#endif
#endif

#if BCDS_FEATURE_BSP_SENSOR_NODE

#define BSP_XDK_SENSOR_BMA280_ENABLE_INTERRUPT                                  1

#define BSP_XDK_SENSOR_BMA280_FORCE_INTERRUPT                                   0

#define BSP_XDK_SENSOR_BMG160_ENABLE_INTERRUPT                                  1

#define BSP_XDK_SENSOR_BMM150_ENABLE_INTERRUPT                                  1

#define BSP_XDK_SENSOR_BMM150_FORCE_INTERRUPT                                   0

#define BSP_XDK_SENSOR_MAX44009_ENABLE_INTERRUPT                                1

#define BSP_XDK_SENSOR_BMI160_ENABLE_INTERRUPT                                  0

#define BSP_XDK_SENSOR_BMI160_FORCE_INTERRUPT                                   0

#endif /*BCDS_FEATURE_BSP_SENSOR_NODE*/

#if BCDS_FEATURE_BSP_BUTTON

#define BSP_XDK_KEY1_INTERRUPT                                                  1
#define BSP_XDK_KEY1_INTERRUPT_FORCE                                            0
#define BSP_XDK_KEY2_INTERRUPT                                                  1
#define BSP_XDK_KEY2_INTERRUPT_FORCE                                            0

#endif /* BCDS_FEATURE_BSP_BUTTON */

#if BCDS_FEATURE_BSP_BUTTON_EXT

#define BSP_XDK_EXT_BUTTON_COUNT												1

#define BSP_XDK_EXT_BUTTON_A1													1
#define BSP_XDK_EXT_BUTTON_A1_INTERRUPT											1
#define BSP_XDK_EXT_BUTTON_A1_INTERRUPT_FORCED									1

#define BSP_XDK_EXT_BUTTON_A2													0
#define BSP_XDK_EXT_BUTTON_A2_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_A2_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A3													0
#define BSP_XDK_EXT_BUTTON_A3_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_A3_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A4													0
#define BSP_XDK_EXT_BUTTON_A4_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_A4_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A5													0
#define BSP_XDK_EXT_BUTTON_A5_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_A5_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A6													0
#define BSP_XDK_EXT_BUTTON_A6_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_A6_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A7													0
#define BSP_XDK_EXT_BUTTON_A7_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_A7_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A8													0
#define BSP_XDK_EXT_BUTTON_A8_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_A8_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A9													0
#define BSP_XDK_EXT_BUTTON_A9_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_A9_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A10													0
#define BSP_XDK_EXT_BUTTON_A10_INTERRUPT										0
#define BSP_XDK_EXT_BUTTON_A10_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A11													0
#define BSP_XDK_EXT_BUTTON_A11_INTERRUPT										0
#define BSP_XDK_EXT_BUTTON_A11_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A12													0
#define BSP_XDK_EXT_BUTTON_A12_INTERRUPT										0
#define BSP_XDK_EXT_BUTTON_A12_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_A13													0
#define BSP_XDK_EXT_BUTTON_A13_INTERRUPT										0
#define BSP_XDK_EXT_BUTTON_A13_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_B1													0
#define BSP_XDK_EXT_BUTTON_B1_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_B1_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_B2													0
#define BSP_XDK_EXT_BUTTON_B2_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_B2_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_B3													0
#define BSP_XDK_EXT_BUTTON_B3_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_B3_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_B4													0
#define BSP_XDK_EXT_BUTTON_B4_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_B4_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_B5													0
#define BSP_XDK_EXT_BUTTON_B5_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_B5_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_B6													0
#define BSP_XDK_EXT_BUTTON_B6_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_B6_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_B7													0
#define BSP_XDK_EXT_BUTTON_B7_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_B7_INTERRUPT_FORCED									0

#define BSP_XDK_EXT_BUTTON_B8													0
#define BSP_XDK_EXT_BUTTON_B8_INTERRUPT											0
#define BSP_XDK_EXT_BUTTON_B8_INTERRUPT_FORCED									0

#endif /* BCDS_FEATURE_BSP_BUTTON_EXT */

#if BCDS_FEATURE_BSP_LED_EXT

#define BSP_XDK_EXT_LED_COUNT                                                1

#define BSP_XDK_EXT_LED_A1                                                   0
#define BSP_XDK_EXT_LED_A2                                                   1
#define BSP_XDK_EXT_LED_A3                                                   0
#define BSP_XDK_EXT_LED_A4                                                   0
#define BSP_XDK_EXT_LED_A5                                                   0
#define BSP_XDK_EXT_LED_A6                                                   0
#define BSP_XDK_EXT_LED_A7                                                   0
#define BSP_XDK_EXT_LED_A8                                                   0
#define BSP_XDK_EXT_LED_A9                                                   0
#define BSP_XDK_EXT_LED_A10                                                  0
#define BSP_XDK_EXT_LED_A11                                                  0
#define BSP_XDK_EXT_LED_A12                                                  0
#define BSP_XDK_EXT_LED_A13                                                  0
#define BSP_XDK_EXT_LED_B1                                                   0
#define BSP_XDK_EXT_LED_B2                                                   0
#define BSP_XDK_EXT_LED_B3                                                   0
#define BSP_XDK_EXT_LED_B4                                                   0
#define BSP_XDK_EXT_LED_B5                                                   0
#define BSP_XDK_EXT_LED_B6                                                   0
#define BSP_XDK_EXT_LED_B7                                                   0
#define BSP_XDK_EXT_LED_B8                                                   0

#endif /* BCDS_FEATURE_BSP_LED_EXT */

#if BCDS_FEATURE_BSP_SD_CARD
#define BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT                                    1

#define BCDS_FEATURE_BSP_SD_CARD_AUTO_DETECT_FORCE                              0
#endif /*BCDS_FEATURE_BSP_SD_CARD*/

#endif /*BCDS_HAL_CONFIG_H_*/
