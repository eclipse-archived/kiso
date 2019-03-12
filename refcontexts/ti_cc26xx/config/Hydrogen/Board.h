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
 * @brief Board configuration settings
 */
#ifndef __BOARD_H__
#define __BOARD_H__

/* public interface declaration */
#include "ioc.h"

/* public type and macro definitions */
#define HW_BTLE_SLAVESCA_PPM                UINT16_C(204)   /**< SCA (Sleep Clock Accuracy) value in ppm (parts per million) */

#define DEFINE_BOARD_PIN_STATE(pin, settings) (PIN_ID(pin)|(settings))

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>                <pin mapping>
 */
/* LEDs */
#define BOARD_LED_ON                        UINT8_C(1)                               /* LEDs on CC2650 are active high */
#define BOARD_LED_OFF                       UINT8_C(0)

#define BOARD_LED1                          IOID_25
#define BOARD_LED3                          IOID_27
#define BOARD_LED2                          IOID_7
#define BOARD_LED4                          IOID_6

#define BOARD_DCDC_ENBYPASS                 IOID_5
#define BOARD_DCDC_ENABLE                   UINT8_C(1)
#define BOARD_DCDC_BYPASS                   UINT8_C(0)

/* Button Board */
#define BOARD_KEY_SELECT                    IOID_11
#define BOARD_KEY_UP                        IOID_19
#define BOARD_KEY_TDL_REF                   BOARD_KEY_UP
#define BOARD_KEY_DOWN                      IOID_12
#define BOARD_KEY_LEFT                      IOID_15
#define BOARD_KEY_RIGHT                     IOID_18

/* LCD  Board */
#define BOARD_3V3_EN                        IOID_13
#define BOARD_LCD_MODE                      IOID_4
#define BOARD_LCD_RST                       IOID_5
#define BOARD_LCD_CSN                       IOID_14

/* UART Board */
#define BOARD_UART_RX                       IOID_2
#define BOARD_UART_TX                       IOID_3
#define BOARD_UART_CTS                      IOID_0
#define BOARD_UART_RTS                      IOID_21

/* SPI Board */
#define BOARD_SPI0_MISO                     IOID_8
#define BOARD_SPI0_MOSI                     IOID_9
#define BOARD_SPI0_CLK                      IOID_10
#define BOARD_SPI1_MISO                     IOID_24
#define BOARD_SPI1_MOSI                     IOID_23
#define BOARD_SPI1_CLK                      IOID_30

/* Sensor Chip Select */
#define BOARD_ADXL_INT1                     IOID_28
#define BOARD_SPI_ADXL_CSN                  IOID_11
#define BOARD_SPI_BME280_CSN                IOID_26
#define BOARD_SPI_FLASH_CSN                 IOID_29

#define BOARD_FLASH_RST                     IOID_15

#define BOARD_SENSOR_MODE                   IOID_UNUSED
#define BOARD_SENSOR_RST                    IOID_UNUSED
#define BOARD_SENSOR_CSN                    IOID_UNUSED

/** ============================================================================
 *  Instance identifiers
 *  ==========================================================================*/

/* Generic UART instance identifiers */
#define BOARD_UART                          CC2650_UART0

/* Generic SPI instance identifiers */
#define BOARD_SPI0                          CC2650_SPI0
#define BOARD_SPI1                          CC2650_SPI1

/* PWM Instances */
#define BOARD_PWM0                          CC2650_PWM0
#define BOARD_PWM0_BASE                     GPT0_BASE
#define BOARD_PWM0_TIMER                    TIMER_A
#define BOARD_PWM0_EVENT                    IOC_PORT_MCU_PORT_EVENT0

#define BOARD_PWM1                          CC2650_PWM1
#define BOARD_PWM1_BASE                     GPT0_BASE
#define BOARD_PWM1_TIMER                    TIMER_B
#define BOARD_PWM1_EVENT                    IOC_PORT_MCU_PORT_EVENT1

/* Generic Watchdog instance identifiers */
#define BOARD_WATCHDOG                      CC2650_WATCHDOG0

/** ============================================================================
 *  Number of peripherals and their names
 *  ==========================================================================*/

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
 *  @def    CC2650_PWMName
 *  @brief  Enum of PWM buffers
 */
enum CC2650_PWMName_E
{
    CC2650_PWM0 = 0,
    CC2650_PWM1,
    CC2650_PWMCOUNT
};
typedef enum CC2650_PWMName_E CC2650_PWMName_T;

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

#endif /* __BOARD_H__ */
