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
 * @defgroup HALCONFIG    Sensgate HAL Configuration file
 * @ingroup  BSP_SENSGATE_IF
 * @{
 */
/**
 * @brief Contains a default configuration to enable or disable HAL components.
 * 
 * @details 
 * This header file is included by the BCDS_HAL.h from the HAL (Hardware Abstraction Layer) module. It is used to allow
 * a per project configuration of the features provided by the HAL component.
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

/* MCU Features */
#define BCDS_FEATURE_UART                                                       1
#define BCDS_FEATURE_I2C                                                        1
#define BCDS_FEATURE_SPI                                                        1
#define BCDS_FEATURE_FLASH                                                      0
#define BCDS_FEATURE_WATCHDOG                                                   0
#define BCDS_FEATURE_PWM                                                        0
#define BCDS_FEATURE_SLEEP                                                      0

/* BSP Features */
#define BCDS_FEATURE_BSP_LED                                                    1
#define BCDS_FEATURE_BSP_BUTTON                                                 1
#define BCDS_FEATURE_BSP_CELLULAR_SARAR4N4                                      1
#define BCDS_FEATURE_BSP_GNSS_MAXM8                                             1
#define BCDS_FEATURE_BSP_BMA280                                                 1
#define BCDS_FEATURE_BSP_BME280                                                 1
#define BCDS_FEATURE_BSP_BLE_CC2640                                             1
#define BCDS_FEATURE_BSP_MEMORY_W25                                             1

#endif /*BCDS_HAL_CONFIG_H_*/
