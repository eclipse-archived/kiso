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
 *
 * @brief STM32-Specific configuration of the package DRIVERS
 *
 * @details This is the configuration file for the shared package DRIVERS
 *
 */
#ifndef BCDS_DRIVERSCONFIG_H_
#define BCDS_DRIVERSCONFIG_H_

/* ************************************************************************** */
/* 1. Features and modules provided by the package DRIVERS                    */
/* ************************************************************************** */
/* Features are enabled by defining them to 1                                 */
/* 1 = Enabled, 0 = Disabled                                                  */
/*                                                                            */
/* Configuration options are set by defining to the corresponding value       */
/* ************************************************************************** */
/**
 * Component availability matrix
 *
 * | Component |  IO  | aIO | EFM32 | STM32L4|CC26xx  |STM32F7 |
 * | --------- | ---- | --- | ----- | -----  | ------ | ------ |
 * | ADXL362   | SPI  |     | (0)   | (0)    | (1)    | (0)    |
 * | ADXL372   | SPI  | I2C | (1)   | (0)    | (1)    | (0)    |
 * | H3LIS33DL | SPI  | I2C | (0)   | (0)    | (1)    | (0)    |
 * | LM2755    | I2C  |     | (0)   | (1)    | (0)    | (0)    |
 * | MAX31865  | SPI  |     | (1)   | (0)    | (0)    | (0)    |
 * | MAX44009  | I2C  |     | (1)   | (0)    | (0)    | (0)    |
 * | GNSS7     | UART |     | (0)   | (1)    | (0)    | (0)    |
 * | AT45DB    | SPI  |     | (0)   | (0)    | (1)    | (0)    |
 * | W25Q256FV | SPI  |     | (0)   | (1)    | (0)    | (0)    |
 * | NVM       | x    |     | (1)   | (0)    | (0)    | (0)    |
 * | SDCARD    | SPI  |     | (1)   | (0)    | (0)    | (0)    |
 * | PCT2075   | I2C  |     | (0)   | (0)    | (1)    | (0)    |
 * | TMP112    | I2C  |     | (0)   | (0)    | (0)    | (1)    |
 *
 * IO: Primary IO access method.
 * aIO: Alternative IO access method
 */
 /* ************************************************************************** */

#ifndef BCDS_FEATURE_ADXL362
#define BCDS_FEATURE_ADXL362                                                 (0)
#endif

#ifndef BCDS_FEATURE_ADXL372
#define BCDS_FEATURE_ADXL372                                                 (0)
#endif

#ifndef BCDS_FEATURE_GNSSUBLOX7
#define BCDS_FEATURE_GNSSUBLOX7                                              (0)
#endif

#ifndef BCDS_FEATURE_H3LIS311DL
#define BCDS_FEATURE_H3LIS311DL                                              (0)
#endif

#ifndef BCDS_FEATURE_LM2755LEDDRIVER
#define BCDS_FEATURE_LM2755LEDDRIVER                                         (0)
#endif

#ifndef BCDS_MAX31865_DRIVER
#define BCDS_MAX31865_DRIVER                                                 (0)
#endif

#ifndef BCDS_MAX44009_DRIVER
#define BCDS_MAX44009_DRIVER                                                 (0)
#endif

#ifndef BCDS_FEATURE_FLASHAT45DB
#define BCDS_FEATURE_FLASHAT45DB                                             (0)
#endif

#ifndef BCDS_FEATURE_FLASHW25Q256FV
#define BCDS_FEATURE_FLASHW25Q256FV                                          (0)
#endif

#ifndef BCDS_FEATURE_NVM
#define BCDS_FEATURE_NVM                                                     (0)
#endif

#ifndef BCDS_FEATURE_SDCARD
#define BCDS_FEATURE_SDCARD                                                  (0)
#endif

#ifndef BCDS_FEATURE_PCT2075
#define BCDS_FEATURE_PCT2075                                                 (0)
#endif

#ifndef BCDS_FEATURE_TMP112
#define BCDS_FEATURE_TMP112                                                  (1)
#endif

/* ************************************************************************** */
/* 1.1 OS Awareness                                                           */
/* ************************************************************************** */
/* Select to compile the drivers in their OS Aware version                    */
/* Recomended settings are 1 for Application and 0 for Bootloader             */
#ifndef BCDS_DRIVERS_RTOS_SUPPORT
#define BCDS_DRIVERS_RTOS_SUPPORT                                            (0)
#endif /* BCDS_DRIVERS_RTOS_SUPPORT */



/* ************************************************************************** */
/* 2. Component specific configuration                                        */
/* ************************************************************************** */

/* ************************************************************************** */
/* 2.1 ADXL362                                                                */
/* ************************************************************************** */
/* Available configuration options are:                                       */
/* - ADXL362_TEMPERATURE_ENABLE                                               */
/* ************************************************************************** */
#if (1 == BCDS_FEATURE_ADXL362)

/* ADXL372: Enable Temperature support */
#ifndef ADXL362_TEMPERATURE_ENABLE
#define ADXL362_TEMPERATURE_ENABLE                                           (1)
#endif /* ADXL362_TEMPERATURE_ENABLE */

#endif /* BCDS_FEATURE_ADXL362 */

/* ************************************************************************** */
/* 2.2 ADXL372                                                                */
/* ************************************************************************** */
/* Available configuration options are:                                       */
/* - ADXL372_IO_INTERFACE                                                     */
/* ************************************************************************** */
#if (1 == BCDS_FEATURE_ADXL372)

/* ************************************************************************** */
/* IO interface selection option. possible values are                         */
/*  - 0 for SPI interface (default)                                           */
/*  - 1 for I2C interface                                                     */
#ifndef ADXL372_IO_INTERFACE
#define ADXL372_IO_INTERFACE                                                 (0)
#endif /* ADXL372_IO_INTERFACE */

#endif

/* ************************************************************************** */
/* 2.3 H3LIS311DL                                                             */
/* ************************************************************************** */
/* Available configuration options are:                                       */
/* - H3LIS33DL_IO_INTERFACE                                                   */
/* - H3LIS_ENABLE_INTERRUPT                                                   */
/* - H3LIS_ENABLE_FILTERING                                                   */
/* ************************************************************************** */
#if (1 == BCDS_FEATURE_H3LIS311DL)

/* ************************************************************************** */
/* IO interface selection option. possible values are                         */
/*  - 0 for SPI interface (default)                                           */
/*  - 1 for I2C interface                                                     */
#ifndef H3LIS33DL_IO_INTERFACE
#define H3LIS33DL_IO_INTERFACE                                               (0)
#endif

#ifndef H3LIS_ENABLE_INTERRUPT
#define H3LIS_ENABLE_INTERRUPT                                               (1)
#endif /* H3LIS_ENABLE_INTERRUPT */

#ifndef H3LIS_ENABLE_FILTERING
#define H3LIS_ENABLE_FILTERING                                               (1)
#endif /* H3LIS_ENABLE_FILTERING */


/* ************************************************************************** */
/* Legacy definitions H3LIS_OVER_SPI and H3LIS_OVER_I2C                       */
/* are supported and overwrite the H3LIS33DL_IO_INTERFACE selection macro     */
/* ************************************************************************** */
#if defined(H3LIS_OVER_SPI)

/* Overwrite the selection macro */
#undef H3LIS33DL_IO_INTERFACE
#define H3LIS33DL_IO_INTERFACE 0

#elif defined(H3LIS_OVER_I2C)

/* Overwrite the selection macro */
#undef H3LIS33DL_IO_INTERFACE
#define H3LIS33DL_IO_INTERFACE 1

#else
/* Case where neither H3LIS_OVER_SPI or H3LIS_OVER_I2C have been defined */

#if (0 == H3LIS33DL_IO_INTERFACE)
#define H3LIS_OVER_SPI    1
#elif (1 == H3LIS33DL_IO_INTERFACE)
#define H3LIS_OVER_I2C    1
#endif

#endif /*  H3LIS33DL_IO_INTERFACE IO legacy support */

#endif /* BCDS_FEATURE_H3LIS311DL */

/* ************************************************************************** */
/* 2.4 LM2755                                                                 */
/* ************************************************************************** */
#if (1 == BCDS_FEATURE_LM2755LEDDRIVER)
/* No configuration options */
#endif

/* ************************************************************************** */
/* 2.5 MAX31865                                                               */
/* ************************************************************************** */
#if (1 == BCDS_MAX31865_DRIVER)
/* No configuration options */
#endif

/* ************************************************************************** */
/* 2.6 MAX44009                                                               */
/* ************************************************************************** */
#if (1 == BCDS_MAX44009_DRIVER)
/* No configuration options */
#endif

/* ************************************************************************** */
/* 2.7 AT45DB                                                                 */
/* ************************************************************************** */
#if (1 == BCDS_FEATURE_FLASHAT45DB)

#ifndef BCDS_FEATURE_AT45DB_RTOS_SUPPORT
#define BCDS_FEATURE_AT45DB_RTOS_SUPPORT               BCDS_DRIVERS_RTOS_SUPPORT
#endif /* BCDS_FEATURE_AT45DB_RTOS_SUPPORT */

#endif /* BCDS_FEATURE_FLASHAT45DB */

/* ************************************************************************** */
/* 2.8 W25Q256FV                                                              */
/* ************************************************************************** */
#if (1 == BCDS_FEATURE_FLASHW25Q256FV)

#ifndef BCDS_MISCDRIVERS_ENABLE_RTOS_SUPPORT
#define BCDS_MISCDRIVERS_ENABLE_RTOS_SUPPORT           BCDS_DRIVERS_RTOS_SUPPORT
#endif

#endif /* BCDS_FEATURE_FLASHW25Q256FV*/

/* ************************************************************************** */
/* 2.9 NVM                                                                    */
/* ************************************************************************** */
#if (1 == BCDS_FEATURE_NVM)
/* No configuration options */
#endif /* BCDS_FEATURE_NVM */

/* ************************************************************************** */
/* 2.10 SDCARD                                                                */
/* ************************************************************************** */
#if(1 == BCDS_FEATURE_SDCARD)
/* No configuration options */
#endif /* BCDS_FEATURE_SDCARD */

/* ************************************************************************** */
/* 2.11 PCT2075                                                               */
/* ************************************************************************** */
#if(1 == BCDS_FEATURE_PCT2075)
/* No configuration options */
#endif /* BCDS_FEATURE_PCT2075 */

#endif /* BCDS_DRIVERSCONFIG_H_ */
