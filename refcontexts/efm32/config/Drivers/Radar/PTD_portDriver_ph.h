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
#define PTD_GPIO_LOW                         0
#define PTD_GPIO_HIGH                        1

#define LORA_EVEN_PIN_CNT  UINT8_C(0x3)           /**< Defines Lora Even pin interrupt count */
#define LORA_ODD_PIN_CNT   UINT8_C(0x3)           /**< Defines Lora odd pin interrupt count */

/**********LORA MODULE PIN CONFIGURATION****************/

/** Reset pin configuration for LORA module */
#define PTD_PORT_LORA_RESET                  gpioPortC
#define PTD_PIN_LORA_RESET                   1
#define PTD_MODE_LORA_RESET                  gpioModePushPull
#define PTD_DOUT_LORA_RESET                  PTD_GPIO_LOW

/** SPI MOSI pin configuration for LORA module */
#define PTD_PORT_SPI_MOSI                    gpioPortC
#define PTD_PIN_SPI_MOSI                     2
#define PTD_MODE_SPI_MOSI                    gpioModePushPullDrive
#define PTD_DOUT_SPI_MOSI                    PTD_GPIO_LOW

/** SPI MISO pin configuration for LORA module */
#define PTD_PORT_SPI_MISO                    gpioPortC
#define PTD_PIN_SPI_MISO                     3
#define PTD_MODE_SPI_MISO                    gpioModeInputPull
#define PTD_DOUT_SPI_MISO                    PTD_GPIO_LOW

/** SPI CLK pin configuration for LORA module */
#define PTD_PORT_SPI_CLK                     gpioPortC
#define PTD_PIN_SPI_CLK                      4
#define PTD_MODE_SPI_CLK                     gpioModePushPullDrive
#define PTD_DOUT_SPI_CLK                     PTD_GPIO_LOW

/**SPI chip select for LORA module */
#define PTD_PORT_SPI_CS                      gpioPortC
#define PTD_PIN_SPI_CS                       5
#define PTD_MODE_SPI_CS                      gpioModePushPullDrive
#define PTD_DOUT_SPI_CS                      PTD_GPIO_HIGH

/** DATA pin for I2C */
#define PTD_PORT_I2C0_SDA2                   gpioPortA
#define PTD_PIN_I2C0_SDA2                    0
#define PTD_MODE_I2C0_SDA2                   gpioModeWiredAnd
#define PTD_DOUT_I2C0_SDA2                   PTD_GPIO_HIGH

/** CLOCK pin for I2C */
#define PTD_PORT_I2C0_SCL2                   gpioPortA
#define PTD_PIN_I2C0_SCL2                    1
#define PTD_MODE_I2C0_SCL2                   gpioModeWiredAnd
#define PTD_DOUT_I2C0_SCL2                   PTD_GPIO_HIGH

/**interrupt pin configuration for lora module */
#define PTD_PORT_LORA_RADIO_DIO_0            gpioPortC
#define PTD_PIN_LORA_RADIO_DIO_0             0
#define PTD_MODE_LORA_RADIO_DIO_0            gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_0            PTD_GPIO_LOW

#define PTD_PORT_LORA_RADIO_DIO_1            gpioPortB
#define PTD_PIN_LORA_RADIO_DIO_1             14
#define PTD_MODE_LORA_RADIO_DIO_1            gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_1            PTD_GPIO_LOW

#define PTD_PORT_LORA_RADIO_DIO_2            gpioPortB
#define PTD_PIN_LORA_RADIO_DIO_2             13
#define PTD_MODE_LORA_RADIO_DIO_2            gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_2            PTD_GPIO_LOW

#define PTD_PORT_LORA_RADIO_DIO_3            gpioPortB
#define PTD_PIN_LORA_RADIO_DIO_3             12
#define PTD_MODE_LORA_RADIO_DIO_3            gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_3            PTD_GPIO_LOW

#define PTD_PORT_LORA_RADIO_DIO_4            gpioPortA
#define PTD_PIN_LORA_RADIO_DIO_4             8
#define PTD_MODE_LORA_RADIO_DIO_4            gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_4            PTD_GPIO_LOW

#define PTD_PORT_LORA_RADIO_DIO_5            gpioPortA
#define PTD_PIN_LORA_RADIO_DIO_5             9
#define PTD_MODE_LORA_RADIO_DIO_5            gpioModeInputPull
#define PTD_DOUT_LORA_RADIO_DIO_5            PTD_GPIO_LOW

#define PTD_PORT_LORA_BOM_DET_1              gpioPortC
#define PTD_PIN_LORA_BOM_DET_1               11
#define PTD_MODE_LORA_BOM_DET_1              gpioModeInput
#define PTD_DOUT_LORA_BOM_DET1               PTD_GPIO_LOW

#define PTD_PORT_LORA_BOM_DET_2              gpioPortC
#define PTD_PIN_LORA_BOM_DET_2               12
#define PTD_MODE_LORA_BOM_DET_2              gpioModeInputPull
#define PTD_DOUT_LORA_BOM_DET_2              PTD_GPIO_LOW

#define PTD_PORT_LORA_BOM_DET_3              gpioPortC
#define PTD_PIN_LORA_BOM_DET_3               13
#define PTD_MODE_LORA_BOM_DET_3              gpioModeInputPull
#define PTD_DOUT_LORA_BOM_DET_3              PTD_GPIO_LOW

/** Reset pin configuration for RADAR module */
#define PTD_PORT_RADAR_ENABLE               gpioPortE
#define PTD_PIN_RADAR_ENABLE                14
#define PTD_MODE_ENABLE_RADAR_ENABLE        gpioModePushPull
#define PTD_MODE_DISABLE_RADAR_ENABLE       gpioModePushPull
#define PTD_DOUT_ENABLE_RADAR_ENABLE        PTD_GPIO_HIGH
#define PTD_DOUT_DISABLE_RADAR_ENABLE       PTD_GPIO_LOW

/** SPI MOSI pin configuration for RADAR module */
#define PTD_PORT_RADAR_MOSI                 gpioPortE
#define PTD_PIN_RADAR_MOSI                  10
#define PTD_MODE_ENABLE_RADAR_MOSI          gpioModePushPull
#define PTD_MODE_DISABLE_RADAR_MOSI         gpioModeDisabled
#define PTD_DOUT_ENABLE_RADAR_MOSI          PTD_GPIO_LOW
#define PTD_DOUT_DISABLE_RADAR_MOSI         PTD_GPIO_LOW

/** SPI MISO pin configuration for RADAR module */
#define PTD_PORT_RADAR_MISO                 gpioPortE
#define PTD_PIN_RADAR_MISO                  11
#define PTD_MODE_ENABLE_RADAR_MISO          gpioModeInputPull
#define PTD_MODE_DISABLE_RADAR_MISO         gpioModeDisabled
#define PTD_DOUT_ENABLE_RADAR_MISO          PTD_GPIO_LOW
#define PTD_DOUT_DISABLE_RADAR_MISO         PTD_GPIO_LOW

/** SPI CLK pin configuration for RADAR module */
#define PTD_PORT_RADAR_CLK                  gpioPortE
#define PTD_PIN_RADAR_CLK                   12
#define PTD_MODE_ENABLE_RADAR_CLK           gpioModePushPull
#define PTD_MODE_DISABLE_RADAR_CLK          gpioModeDisabled
#define PTD_DOUT_ENABLE_RADAR_CLK           PTD_GPIO_LOW
#define PTD_DOUT_DISABLE_RADAR_CLK          PTD_GPIO_LOW

/**SPI chip select for RADAR module */
#define PTD_PORT_RADAR_CS                   gpioPortE
#define PTD_PIN_RADAR_CS                    13
#define PTD_MODE_ENABLE_RADAR_CS            gpioModePushPull
#define PTD_MODE_DISABLE_RADAR_CS           gpioModePushPull
#define PTD_DOUT_ENABLE_RADAR_CS            PTD_GPIO_HIGH
#define PTD_DOUT_DISABLE_RADAR_CS           PTD_GPIO_LOW

/** Radar RF reference switch1 for antenna */
#define PTD_PORT_RADAR_RFREFSW_A1            gpioPortE
#define PTD_PIN_RADAR_RFREFSW_A1             8
#define PTD_MODE_ENABLE_RADAR_RFREFSW_A1     gpioModePushPull
#define PTD_MODE_DISABLE_RADAR_RFREFSW_A1    gpioModeDisabled
#define PTD_DOUT_ENABLE_RADAR_RFREFSW_A1     PTD_GPIO_LOW
#define PTD_DOUT_DISABLE_RADAR_RFREFSW_A1    PTD_GPIO_LOW

/** Radar RF reference switch2 for antenna */
#define PTD_PORT_RADAR_RFREFSW_A2            gpioPortE
#define PTD_PIN_RADAR_RFREFSW_A2             9
#define PTD_MODE_ENABLE_RADAR_RFREFSW_A2     gpioModePushPull
#define PTD_MODE_DISABLE_RADAR_RFREFSW_A2    gpioModeDisabled
#define PTD_DOUT_ENABLE_RADAR_RFREFSW_A2     PTD_GPIO_HIGH
#define PTD_DOUT_DISABLE_RADAR_RFREFSW_A2    PTD_GPIO_LOW

/** Interrupt pin configuration for RADAR module */
#define PTD_PORT_RADAR_DATAR                 gpioPortE
#define PTD_PIN_RADAR_DATAR                  15
#define PTD_MODE_ENABLE_RADAR_DATAR          gpioModeInput
#define PTD_MODE_DISABLE_RADAR_DATAR         gpioModeDisabled
#define PTD_DOUT_ENABLE_RADAR_DATAR          PTD_GPIO_LOW
#define PTD_DOUT_DISABLE_RADAR_DATAR         PTD_GPIO_LOW

/** Pin configuration for memory */
#define PTD_PORT_MEM_W_N                     gpioPortC
#define PTD_PIN_MEM_W_N                      8
#define PTD_MODE_MEM_W_N                     gpioModePushPull
#define PTD_DOUT_MEM_W_N                     PTD_GPIO_LOW

/** Pin configuration for memory */
#define PTD_PORT_MEM_HOLD_N                  gpioPortC
#define PTD_PIN_MEM_HOLD_N                   9
#define PTD_MODE_MEM_HOLD_N                  gpioModePushPull
#define PTD_DOUT_MEM_HOLD_N                  PTD_GPIO_LOW

/** Pin configuration for memory enable */
#define PTD_PORT_MEM_EN                      gpioPortC
#define PTD_PIN_MEM_EN                       10
#define PTD_MODE_MEM_EN                      gpioModeWiredAnd
#define PTD_DOUT_MEM_EN                      PTD_GPIO_LOW

/** Pin configuration for memory sleep state */
#define PTD_PORT_MEM_DISABLE                      gpioPortC
#define PTD_PIN_MEM_DISABLE                       10
#define PTD_MODE_MEM_DISABLE                      gpioModeDisabled
#define PTD_DOUT_MEM_DISABLE                      PTD_GPIO_HIGH

/** SPI MOSI pin configuration for memory module */
#define PTD_PORT_MEM_MOSI                    gpioPortD
#define PTD_PIN_MEM_MOSI                     0
#define PTD_MODE_MEM_MOSI                    gpioModePushPull
#define PTD_DOUT_MEM_MOSI                    PTD_GPIO_LOW

/** SPI MISO pin configuration for memory module */
#define PTD_PORT_MEM_MISO                   gpioPortD
#define PTD_PIN_MEM_MISO                    1
#define PTD_MODE_MEM_MISO                   gpioModeInputPull
#define PTD_DOUT_MEM_MISO                   PTD_GPIO_HIGH

/** SPI CLK pin configuration for Memory module */
#define PTD_PORT_MEM_SCK                    gpioPortD
#define PTD_PIN_MEM_SCK                     2
#define PTD_MODE_MEM_SCK                    gpioModePushPull
#define PTD_DOUT_MEM_SCK                    PTD_GPIO_LOW

/**SPI chip select for Memory module */
#define PTD_PORT_MEM_CSN                    gpioPortD
#define PTD_PIN_MEM_CSN                     3
#define PTD_MODE_MEM_CSN                    gpioModePushPull
#define PTD_DOUT_LORA_MEM_CSN               PTD_GPIO_LOW

/** VDD pin configuration for BMM */
#define PTD_PORT_BMM_VDD                     gpioPortF
#define PTD_PIN_BMM_VDD                      3
#define PTD_MODE_BMM_VDD                     gpioModePushPullDrive
#define PTD_DOUT_BMM_VDD                     PTD_GPIO_HIGH

/** Interrupt pin for BMM */
#define PTD_PORT_BMM_INT                     gpioPortF
#define PTD_PIN_BMM_INT                      4
#define PTD_MODE_BMM_INT                     gpioModeInputPull
#define PTD_DOUT_BMM_INT                     PTD_GPIO_LOW

/** Data ready pin for BMM */
#define PTD_PORT_BMM_DRDY                    gpioPortF
#define PTD_PIN_BMM_DRDY                     5
#define PTD_MODE_BMM_DRDY                    gpioModeInputPull
#define PTD_DOUT_BMM_DRDY                    PTD_GPIO_LOW

/** Power monitor enable port */
#define PTD_PORT_VBAT_SENSE_EN				 gpioPortD
#define PTD_PIN_VBAT_SENSE_EN				 5
#define PTD_MODE_ENABLE_VBAT_SENSE_EN		 gpioModePushPull
#define PTD_MODE_DISABLE_VBAT_SENSE_EN		 gpioModeDisabled
#define PTD_DOUT_ENABLE_VBAT_SENSE_EN	     PTD_GPIO_HIGH
#define PTD_DOUT_DISABLE_VBAT_SENSE_EN		 PTD_GPIO_LOW

/** Drive Mode Setting for all  the pins of Port A */
#define PTD_PORT_PORT_A                      gpioPortA
#define PTD_DRIVE_MODE_PORT_A                gpioDriveModeStandard

/** Drive Mode Setting for all  the pins of Port B */
#define PTD_PORT_PORT_B                      gpioPortB
#define PTD_DRIVE_MODE_PORT_B                gpioDriveModeStandard

/** Drive Mode Setting for all  the pins of Port C */
#define PTD_PORT_PORT_C                      gpioPortC
#define PTD_DRIVE_MODE_PORT_C                gpioDriveModeStandard

/** Drive Mode Setting for all  the pins of Port D */
#define PTD_PORT_PORT_D                      gpioPortD
#define PTD_DRIVE_MODE_PORT_D                gpioDriveModeStandard

/** Drive Mode Setting for all  the pins of Port E */
#define PTD_PORT_PORT_E                      gpioPortE
#define PTD_DRIVE_MODE_PORT_E_IDLE           gpioDriveModeStandard
#define PTD_DRIVE_MODE_PORT_E_ACTIVE         gpioDriveModeLowest

/** Drive Mode Setting for all  the pins of Port F */
#define PTD_PORT_PORT_F                      gpioPortF
#define PTD_DRIVE_MODE_PORT_F                gpioDriveModeHigh

#endif /* PTD_PORTDRIVER_PH_H_ */
