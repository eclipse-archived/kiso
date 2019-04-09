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


#ifndef PTD_PORTDRIVER_PH_H_
#define PTD_PORTDRIVER_PH_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */
#define PTD_GPIO_LOW         				 0
#define PTD_GPIO_HIGH         				 1

#define LORA_EVEN_PIN_CNT  UINT8_C(0x3)           /**< Defines Lora + Radar Even pin interrupt count */
#define LORA_ODD_PIN_CNT   UINT8_C(0x2)           /**< Defines Lora + Radar odd pin interrupt count */

/**********LORA MODULE PIN CONFIGURATION****************/

/** Reset pin configuration for LORA module */
#define PTD_PORT_LORA_RESET            gpioPortC
#define PTD_PIN_LORA_RESET             1
#define PTD_MODE_LORA_RESET            gpioModePushPull
#define PTD_DRIVE_MODE_LORA_RESET      gpioDriveModeStandard
#define PTD_DOUT_LORA_RESET            PTD_GPIO_LOW

/** SPI MOSI pin configuration for LORA module */
#define PTD_PORT_SPI_MOSI              gpioPortC
#define PTD_PIN_SPI_MOSI               2
#define PTD_MODE_SPI_MOSI              gpioModePushPull
#define PTD_DRIVE_SPI_MOSI             gpioDriveModeStandard
#define PTD_DOUT_SPI_MOSI              PTD_GPIO_LOW

/** SPI MISO pin configuration for LORA module */
#define PTD_PORT_SPI_MISO              gpioPortC
#define PTD_PIN_SPI_MISO               3
#define PTD_MODE_SPI_MISO              gpioModePushPull
#define PTD_DRIVE_MODE_SPI_MISO        gpioDriveModeStandard
#define PTD_DOUT_SPI_MISO              PTD_GPIO_LOW

/** SPI CLK pin configuration for LORA module */
#define PTD_PORT_SPI_CLK               gpioPortC
#define PTD_PIN_SPI_CLK                4
#define PTD_MODE_SPI_CLK               gpioModePushPull
#define PTD_DRIVE_MODE_SPI_CLK         gpioDriveModeStandard
#define PTD_DOUT_SPI_CLK               PTD_GPIO_LOW

/**SPI chip select for LORA module */
#define PTD_PORT_SPI_CS                gpioPortC
#define PTD_PIN_SPI_CS                 5
#define PTD_MODE_SPI_CS                gpioModePushPull
#define PTD_DRIVE_MODE_SPI_CS          gpioDriveModeStandard
#define PTD_DOUT_SPI_CS                PTD_GPIO_HIGH

/** DATA pin for I2C */
#define PTD_PORT_I2C0_SDA2             gpioPortA
#define PTD_PIN_I2C0_SDA2              0
#define PTD_MODE_I2C0_SDA2             gpioModeWiredAnd
#define PTD_DOUT_I2C0_SDA2             PTD_GPIO_LOW

/** CLOCK pin for I2C */
#define PTD_PORT_I2C0_SCL2             gpioPortA
#define PTD_PIN_I2C0_SCL2              1
#define PTD_MODE_I2C0_SCL2             gpioModeWiredAnd
#define PTD_DOUT_I2C0_SCL2             PTD_GPIO_LOW

/**interrupt pin configuration for lora module */
#define PTD_PORT_LORA_RADIO_DIO_0      gpioPortC
#define PTD_PIN_LORA_RADIO_DIO_0       0
#define PTD_MODE_LORA_RADIO_DIO_0      gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_0      PTD_GPIO_LOW

#define PTD_PORT_LORA_RADIO_DIO_1      gpioPortB
#define PTD_PIN_LORA_RADIO_DIO_1       14
#define PTD_MODE_LORA_RADIO_DIO_1      gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_1      PTD_GPIO_LOW

#define PTD_PORT_LORA_RADIO_DIO_2      gpioPortB
#define PTD_PIN_LORA_RADIO_DIO_2       13
#define PTD_MODE_LORA_RADIO_DIO_2      gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_2      PTD_GPIO_LOW

#define PTD_PORT_LORA_RADIO_DIO_3      gpioPortB
#define PTD_PIN_LORA_RADIO_DIO_3       12
#define PTD_MODE_LORA_RADIO_DIO_3      gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_3      PTD_GPIO_LOW

#define PTD_PORT_LED_RED               gpioPortA
#define PTD_PIN_LED_RED                8
#define PTD_MODE_LED_RED               gpioModePushPull
#define PTD_DOUT_LED_RED               PTD_GPIO_HIGH

#define PTD_PORT_LED_GREEN             gpioPortA
#define PTD_PIN_LED_GREEN              9
#define PTD_MODE_LED_GREEN             gpioModePushPull
#define PTD_DOUT_LED_GREEN             PTD_GPIO_HIGH

/* Reset pin configuration for RADAR module*/
#define PTD_PORT_RADAR_ENABLE          gpioPortE
#define PTD_PIN_RADAR_ENABLE           14
#define PTD_MODE_RADAR_ENABLE          gpioModePushPull
#define PTD_DRIVE_MODE_RADAR_ENABLE    gpioDriveModeStandard
#define PTD_DOUT_RADAR_ENABLE          PTD_GPIO_LOW

/** SPI MOSI pin configuration for RADAR module */
#define PTD_PORT_RADAR_MOSI              gpioPortE
#define PTD_PIN_RADAR_MOSI               10
#define PTD_MODE_RADAR_MOSI              gpioModePushPullDrive
#define PTD_DRIVE_RADAR_MOSI             gpioDriveModeStandard
#define PTD_DOUT_RADAR_MOSI              PTD_GPIO_LOW

/** SPI MISO pin configuration for RADAR module */
#define PTD_PORT_RADAR_MISO              gpioPortE
#define PTD_PIN_RADAR_MISO               11
#define PTD_MODE_RADAR_MISO              gpioModeInputPull
#define PTD_DRIVE_MODE_RADAR_MISO        gpioDriveModeStandard
#define PTD_DOUT_RADAR_MISO              PTD_GPIO_LOW

/** SPI CLK pin configuration for RADAR module */
#define PTD_PORT_RADAR_CLK               gpioPortE
#define PTD_PIN_RADAR_CLK                12
#define PTD_MODE_RADAR_CLK               gpioModePushPullDrive
#define PTD_DRIVE_MODE_RADAR_CLK         gpioDriveModeStandard
#define PTD_DOUT_RADAR_CLK               PTD_GPIO_LOW

/**SPI chip select for RADAR module */
#define PTD_PORT_RADAR_CS                gpioPortE
#define PTD_PIN_RADAR_CS                 13
#define PTD_MODE_RADAR_CS                gpioModePushPullDrive
#define PTD_DRIVE_MODE_RADAR_CS          gpioDriveModeStandard
#define PTD_DOUT_RADAR_CS                PTD_GPIO_HIGH

/** Radar RF reference switch1 for antenna */
#define PTD_PORT_RADAR_RFREFSW_A1        gpioPortE
#define PTD_PIN_RADAR_RFREFSW_A1         8
#define PTD_MODE_RADAR_RFREFSW_A1        gpioModePushPull
#define PTD_DRIVE_MODE_RADAR_RFREFSW_A1  gpioDriveModeStandard
#define PTD_DOUT_RADAR_RFREFSW_A1        PTD_GPIO_LOW

/** Radar RF reference switch2 for antenna */
#define PTD_PORT_RADAR_RFREFSW_A2        gpioPortE
#define PTD_PIN_RADAR_RFREFSW_A2         9
#define PTD_MODE_RADAR_RFREFSW_A2        gpioModePushPull
#define PTD_DRIVE_MODE_RADAR_RFREFSW_A2  gpioDriveModeStandard
#define PTD_DOUT_RADAR_RFREFSW_A2        PTD_GPIO_HIGH

/**interrupt pin configuration for RADAR module */
#define PTD_PORT_RADAR_DATAR            gpioPortE
#define PTD_PIN_RADAR_DATAR             15
#define PTD_MODE_RADAR_DATAR            gpioModeInputPull
#define PTD_DOUT_RADAR_DATAR            PTD_GPIO_LOW

#endif /* PTD_PORTDRIVER_PH_H_ */
