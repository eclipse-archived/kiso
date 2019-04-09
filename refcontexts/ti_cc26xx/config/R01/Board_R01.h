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
 * @brief Board configuration settings for IROLL R01 hardware sample.
 */

#ifndef BOARD_IROLL_R01_H_
#define BOARD_IROLL_R01_H_

/* public interface declaration */
#include <ti/drivers/PIN.h>
#include "ioc.h"

/* public type and macro definitions */

/* Board GPIO initialization table */
extern PIN_Config BoardGpioInitTable[];

#define DEFINE_BOARD_PIN_STATE(pin, settings) (PIN_ID(pin)|(settings))

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>                <pin mapping>
 */

/* Temperature Sensor */
#define BOARD_I2C0_SDA0             			IOID_25
#define BOARD_I2C0_SCL0             			IOID_24
#define BOARD_PWR_Tempsensor					IOID_19
#define Board_I2C0_SDA1             			PIN_UNASSIGNED
#define Board_I2C0_SCL1             			PIN_UNASSIGNED

/* Flash */
#define BOARD_CS_FLASH                         	IOID_0
#define BOARD_PWR_FLASH                         IOID_1
#define BOARD_SPI0_MOSI                         IOID_26
#define BOARD_SPI0_MISO                       	IOID_27
#define BOARD_SPI0_CLK                       	IOID_18
#define BOARD_SPI_FLASH_CSN                     IOID_0
#define BOARD_SPI_FLASH_CS						BOARD_CS_FLASH
#define BOARD_FLASH_CS_ON       				0
#define BOARD_FLASH_CS_OFF      				1
#define BOARD_SPI_FLASH_CS          			BOARD_CS_FLASH

/* Acceleration Sensor */
#define BOARD_SPI1_MOSI	                        IOID_12
#define BOARD_SPI1_MISO                      	IOID_13
#define BOARD_SPI1_CLK                        	IOID_10
#define BOARD_INT1_H3LIS                       	IOID_11
#define BOARD_INT2_H3LIS                      	IOID_14
#define BOARD_PWR_H3LIS                        	IOID_5
#define BOARD_CS_H3LIS                         	IOID_4

/* LEDs */
#define BOARD_LEDB								IOID_6
#define BOARD_LEDG								IOID_7

/* Test points */
#define BOARD_JTAG_TDO                       	IOID_16
#define BOARD_JTAG_TDI                   		IOID_17

/* UART */
#define BOARD_UART_RX 	                        IOID_2
#define BOARD_UART_TX                           IOID_3

/** ============================================================================
 *  Instance identifiers
 *  ==========================================================================*/
/* Generic I2C instance identifiers */
#define Board_I2C0                   		CC2650_I2C0

/* Generic UART instance identifiers */
#define BOARD_UART                          CC2650_UART0

/* Generic SPI instance identifiers */
#define BOARD_SPI0                          CC2650_SPI0
#define BOARD_SPI1                          CC2650_SPI1


/* Generic Watchdog instance identifiers */
#define BOARD_WATCHDOG                      CC2650_WATCHDOG0

#define HW_BTLE_SLAVESCA_PPM                UINT16_C(204)   /**< SCA (Sleep Clock Accuracy) value in ppm (parts per million) */

/** ============================================================================
 *  Number of peripherals and their names
 *  ==========================================================================*/

/*!
 *  @def    CC2650_I2CName
 *  @brief  Enum of I2C names on the CC2650 development board
 */
typedef enum CC2650STK_I2CName {
    CC2650_I2C0 = 0,
    CC2650_I2CCOUNT
} CC2650STK_I2CName;

/**
 *  @def    CC2650_UARTName
 *  @brief  Enum of UARTs on the CC2650 development board
 */
enum CC2650_UARTName_E
{
    CC2650_UART0 = 0,
    CC2650_UARTCOUNT
};
typedef enum CC2650_UARTName_E CC2650_UARTName_T;

/**
 *  @def    CC2650_SPI
 *  @brief  Enum of SPI's on the CC2650 development board
 */
enum CC2650_SPIName_E
{
    CC2650_SPI0 = 0,
    CC2650_SPI1,
    CC2650_SPICOUNT
};
typedef enum CC2650_SPIName_E CC2650_SPIName_T;

/**
 *  @def    CC2650_UdmaName
 *  @brief  Enum of DMA buffers
 */
enum CC2650_UdmaName_E
{
    CC2650_UDMA0 = 0,
    CC2650_UDMACOUNT
};
typedef enum CC2650_UdmaName_E CC2650_UdmaName_T;


/**
 *  @def    CC2650_WATCHDOGName
 *  @brief  Enum of WATCHDOG on the CC2650 development board
 */
enum CC2650_WatchdogName_E
{
    CC2650_WATCHDOG0 = 0,
    CC2650_WATCHDOGCOUNT
};
typedef enum CC2650_WatchdogName_E CC2650_WatchdogName_T;

/**
 *  @def    CC2650_TRNGName
 *  @brief  Enum of TRNG names on the board
 */
enum CC2650_TRNGName_E {
    CC2650_TRNG0 = 0,
    CC2650_TRNGCOUNT
};
typedef enum CC2650_TRNGName_E CC2650_TRNGName_T;

/*!
 *  @def    CC2640R2_CryptoName
 *  @brief  Enum of Crypto names
 */
enum CC2640R2_CryptoName_E {
    CC2640R2_CRYPTO0 = 0,
    CC2640R2_CRYPTOCOUNT
};
typedef enum CC2640R2_CryptoName_E CC2640R2_CryptoName_T;

/* public function prototype declarations */

#endif /* BOARD_IROLL_R01_H_ */
