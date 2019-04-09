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
 * @defgroup BSP_XDK110_BOARD BOARD
 * @ingroup BCDS_HAL_BSP_IMP
 * @{
 * @file
 * @brief Board settings header file
 * @details This file holds static configurations which are meant to be
 * assigned to a peripheral at a certain point in time for the Initialize,
 * Connect, Enable, Disable, Disconnect functions.
 * It is prefered to have only macro definitions in this file so that it
 * contains compile time necessary information only.
 */

#ifndef BoardSettings_H
#define BoardSettings_H
#include "BCDS_HAL.h"

#include "em_chip.h"
#include "em_gpio.h"
#include "em_usb.h"
#include "em_msc.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "em_i2c.h"
#include "em_rtc.h"
#include "em_burtc.h"
#include "em_timer.h"
#include "em_leuart.h"
#include "em_rmu.h"
#include "em_wdog.h"
#include "em_adc.h"
#include "em_aes.h"
#include "em_assert.h"
#include "em_dbg.h"
#include "em_int.h"


#if BCDS_FEATURE_BSP_LORA_SX1272
#ifndef BCDS_BOARD_APLM
#define BCDS_BOARD_APLM	1
#endif
#ifndef BCDS_BOARD_XDK110
#define BCDS_BOARD_XDK110 0
#endif
#else
#ifndef BCDS_BOARD_APLM
#define BCDS_BOARD_APLM	0
#endif
#ifndef BCDS_BOARD_XDK110
#define BCDS_BOARD_XDK110 1
#endif
#endif

#if (1 < (BCDS_BOARD_APLM + BCDS_BOARD_XDK110))
#error "multiple boards defined"
#endif

#ifndef BCDS_FEATURE_TRACEPORT
#define BCDS_FEATURE_TRACEPORT 0
#endif

#if BCDS_BOARD_APLM
#define EFM32_HFXO_FREQ                                                         UINT32_C(28000000)/**< value of the high speed external oscillator */
#else
#define EFM32_HFXO_FREQ                                                         UINT32_C(48000000)/**< value of the high speed external oscillator */
#endif

#define HSE_VALUE                                                               EFM32_HFXO_FREQ
#define BSP_GPIO_INT_EVEN_MASK                                                  0x00005555
#define BSP_GPIO_INT_ODD_MASK                                                   0x0000AAAA
#define BSP_ENABLE                                                              true
#define BSP_DISABLE                                                             false
#define BSP_IO_PIN_LOW                                                          UINT32_C(0)
#define BSP_IO_PIN_HIGH                                                         UINT32_C(1)

/* DMA Controller Interrupt Priority */

#define DMA_INTERRUPT_PRIO                                                      (5)

#define ADC0_CH4_PIN                                                            (4)
#define ADC0_CH4_PORT                                                           (gpioPortD)

#define ADC0_CH5_PIN                                                            (5)
#define ADC0_CH5_PORT                                                           (gpioPortD)

#define ADC0_CH6_PIN                                                            (6)
#define ADC0_CH6_PORT                                                           (gpioPortD)

#define ADC0_CH7_PIN                                                            (7)
#define ADC0_CH7_PORT                                                           (gpioPortD)

#define AKU_OUT_PIN                                                             (4)
#define AKU_OUT_PORT                                                            (gpioPortD)
#define AKU_OUT_MODE                                                            (gpioModePushPullDrive)

////////////////////////////////////////////////////////////////////////////////SENSOR NODE
#define SENSOR_NODE_I2C_BUS                                                     I2C0
#define SENSOR_NODE_I2C_CLOCK                                                   (cmuClock_I2C0)
#define SENSOR_NODE_I2C_IRQn                                                    (I2C0_IRQn)
#define SENSOR_NODE_INTERRUPT_PRIORITY                                          UINT32_C(5)
#define SENSOR_NODE_I2C_PRIORITY                                                UINT32_C(5)

#define VDD_BMA280_PIN                                                          (7)
#define VDD_BMA280_PORT                                                         (gpioPortE)
#define VDD_BMA280_MODE                                                         (gpioModePushPull)

#define VDD_BME280_PIN                                                          (8)
#define VDD_BME280_PORT                                                         (gpioPortE)
#define VDD_BME280_MODE                                                         (gpioModePushPullDrive)

#define VDD_BMG160_PIN                                                          (13)
#define VDD_BMG160_PORT                                                         (gpioPortD)
#define VDD_BMG160_MODE                                                         (gpioModePushPullDrive)

#define VDD_BMI160_PIN                                                          (3)
#define VDD_BMI160_PORT                                                         (gpioPortE)
#define VDD_BMI160_MODE                                                         (gpioModePushPullDrive)

#define VDD_BMM150_PIN                                                          (5)
#define VDD_BMM150_PORT                                                         (gpioPortE)
#define VDD_BMM150_MODE                                                         (gpioModePushPull)

#define VDD_MAX44009_PIN                                                        (6)
#define VDD_MAX44009_PORT                                                       (gpioPortE)
#define VDD_MAX44009_MODE                                                       (gpioModePushPullDrive)

#define BMA280_INT1_PIN                                                         (8)
#define BMA280_INT1_PORT                                                        (gpioPortF)
#define BMA280_INT1_MODE                                                        (gpioModeInputPullFilter)
#define BMA280_INT1_EDGE_RISING                                                 false
#define BMA280_INT1_EDGE_FALLING                                                true

#define BMA280_INT2_PIN                                                         (9)
#define BMA280_INT2_PORT                                                        (gpioPortF)
#define BMA280_INT2_MODE                                                        (gpioModeInputPullFilter)
#define BMA280_INT2_EDGE_RISING                                                 false
#define BMA280_INT2_EDGE_FALLING                                                true

#define BMG160_INT1_PIN                                                         (6)
#define BMG160_INT1_PORT                                                        (gpioPortC)
#define BMG160_INT1_MODE                                                        (gpioModeInputPullFilter)
#define BMG160_INT1_EDGE_RISING                                                 false
#define BMG160_INT1_EDGE_FALLING                                                true

#define BMG160_INT2_PIN                                                         (7)
#define BMG160_INT2_PORT                                                        (gpioPortA)
#define BMG160_INT2_MODE                                                        (gpioModeInputPullFilter)
#define BMG160_INT2_EDGE_RISING                                                 false
#define BMG160_INT2_EDGE_FALLING                                                true

#define BMI160_INT1_PIN                                                         (13)
#define BMI160_INT1_PORT                                                        (gpioPortA)
#define BMI160_INT1_MODE                                                        (gpioModeInputPullFilter)
#define BMI160_INT1_EDGE_RISING                                                 false
#define BMI160_INT1_EDGE_FALLING                                                true

#define BMI160_INT2_PIN                                                         (14)
#define BMI160_INT2_PORT                                                        (gpioPortA)
#define BMI160_INT2_MODE                                                        (gpioModeInputPullFilter)
#define BMI160_INT2_EDGE_RISING                                                 false
#define BMI160_INT2_EDGE_FALLING                                                true

#define BMM150_DRDY_PIN                                                         (15)
#define BMM150_DRDY_PORT                                                        (gpioPortE)
#define BMM150_DRDY_MODE                                                        (gpioModeInputPullFilter)
#define BMM150_DRDY_EDGE_RISING                                                 true
#define BMM150_DRDY_EDGE_FALLING                                                false

#define BMM150_INT_PIN                                                          (12)
#define BMM150_INT_PORT                                                         (gpioPortD)
#define BMM150_INT_MODE                                                         (gpioModeInputPullFilter)
#define BMM150_INT_EDGE_RISING                                                  false
#define BMM150_INT_EDGE_FALLING                                                 true

#define MAX44009_INTN_PIN                                                       (4)
#define MAX44009_INTN_PORT                                                      (gpioPortE)
#define MAX44009_INTN_MODE                                                      (gpioModeInputPullFilter)
#define MAX44009_INTN_EDGE_RISING                                               false
#define MAX44009_INTN_EDGE_FALLING                                              true

#define I2C0_SCL_PIN                                                            (15)
#define I2C0_SCL_PORT                                                           (gpioPortD)
#define I2C0_SCL_MODE                                                           (gpioModeWiredAnd)

#define I2C0_SDA_PIN                                                            (14)
#define I2C0_SDA_PORT                                                           (gpioPortD)
#define I2C0_SDA_MODE                                                           (gpioModeWiredAnd)
////////////////////////////////////////////////////////////////////////////////BT MODULE
#define BT_STARTUP_DELAY_MS                                                     UINT32_C(4)
#define BT_WAKEUP_DELAY_MS                                                      UINT32_C(250)
#define BT_RESET_PROCESSING_DELAY_MS                                            UINT32_C(5)
#define BT_RESET_PULLUP_DELAY_MS                                                UINT32_C(2)
#define BT_TRANSMIT_DISABLE_TIMEOUT_MS                                          UINT32_C(2)

#define BT_UART0_CLOCK                                                          (cmuClock_UART0)
#define BT_UART_RX_IRQn                                                         UART0_RX_IRQn
#define BT_UART_RX_PRIORITY                                                     UINT32_C(5)
#define BT_UART_TX_IRQn                                                         UART0_TX_IRQn
#define BT_UART_TX_PRIORITY                                                     UINT32_C(5)

#define BT_SERIAL_PORT                                                          UART0

#define BT_IRQ_PIN                                                              (11)
#define BT_IRQ_PORT                                                             (gpioPortD)
#define BT_IRQ_MODE                                                             (gpioModeDisabled)

#define BT_RST_PIN                                                              (9)
#define BT_RST_PORT                                                             (gpioPortA)
#define BT_RST_MODE                                                             (gpioModeWiredOr)

#define BT_UART0_RX_PIN                                                         (1)
#define BT_UART0_RX_PORT                                                        (gpioPortE)
#define BT_UART0_RX_MODE                                                        (gpioModeInput)
#define BT_UART0_RX_DMA_CHANNEL                                                 (3)

#define BT_UART0_TX_PIN                                                         (0)
#define BT_UART0_TX_PORT                                                        (gpioPortE)
#define BT_UART0_TX_MODE                                                        (gpioModePushPull)
#define BT_UART0_TX_DMA_CHANNEL                                                 (2)

#define BT_WAKEUP_PIN                                                           (10)
#define BT_WAKEUP_PORT                                                          (gpioPortD)
#define BT_WAKEUP_MODE                                                          (gpioModeWiredOr)
////////////////////////////////////////////////////////////////////////////////CHARGING MODULE

#define CHARGE_ON_UPPER_LIMIT                                                   4650 /* 4200 + 225(H/W signal) mvolt = 4425 + (Error Limit)225 mvolt  = 4650 */
#define CHARGE_ON_LOWER_LIMIT                                                   4300

#define CHARGE_CRITICAL_UPPER_LIMIT                                             3500
#define CHARGE_CRITICAL_LOWER_LIMIT                                             3300

#define CHARGE_LOW_UPPER_LIMIT                                                  3650
#define CHARGE_LOW_LOWER_LIMIT                                                  3500

#define CHARGE_NORMAL_UPPER_LIMIT                                               3925
#define CHARGE_NORMAL_LOWER_LIMIT                                               3650

#define CHARGE_HIGH_UPPER_LIMIT                                                 4300
#define CHARGE_HIGH_LOWER_LIMIT                                                 3925

#define VOUT_SENSE_ADC_PIN                                                      (7)
#define VOUT_SENSE_ADC_PORT                                                     (gpioPortD)

#define CHARGING_SELECT_PIN                                                     (3)
#define CHARGING_SELECT_PORT                                                    (gpioPortD)

#define DBG_SWO_PIN                                                             (2)
#define DBG_SWO_PORT                                                            (gpioPortF)

#define ETM_TCLK_PIN                                                            (6)
#define ETM_TCLK_PORT                                                           (gpioPortA)

#define ETM_TD0_PIN                                                             (2)
#define ETM_TD0_PORT                                                            (gpioPortA)

#define ETM_TD1_PIN                                                             (3)
#define ETM_TD1_PORT                                                            (gpioPortA)

#define ETM_TD2_PIN                                                             (4)
#define ETM_TD2_PORT                                                            (gpioPortA)

#define ETM_TD3_PIN                                                             (5)
#define ETM_TD3_PORT                                                            (gpioPortA)

#define EXT_CS_PIN                                                              (8)
#define EXT_CS_PORT                                                             (gpioPortD)

#define GPIO_EXT1_PIN                                                           (1)
#define GPIO_EXT1_PORT                                                          (gpioPortA)

#define GPIO_EXT2_PIN                                                           (2)
#define GPIO_EXT2_PORT                                                          (gpioPortE)

#define GPIO_SWO_PIN                                                            (2)
#define GPIO_SWO_PORT                                                           (gpioPortF)

#define HFXO_N_PIN                                                              (14)
#define HFXO_N_PORT                                                             (gpioPortB)

#define HFXO_P_PIN                                                              (13)
#define HFXO_P_PORT                                                             (gpioPortB)

#define HFXTAL_N_PIN                                                            (14)
#define HFXTAL_N_PORT                                                           (gpioPortB)

#define HFXTAL_P_PIN                                                            (13)
#define HFXTAL_P_PORT                                                           (gpioPortB)

#define I2C1_SCL_PIN                                                            (12)
#define I2C1_SCL_PORT                                                           (gpioPortB)

#define I2C1_SDA_PIN                                                            (11)
#define I2C1_SDA_PORT                                                           (gpioPortB)

#define KEY1_PIN                                                                (9)
#define KEY1_PORT                                                               (gpioPortE)
#define KEY1_MODE                                                               (gpioModeInputPullFilter)
#define KEY1_PULL                                                               (1)
#define KEY1_EDGE_RISING                                                        true
#define KEY1_EDGE_FALLING                                                       true

#define KEY2_PIN                                                                (14)
#define KEY2_PORT                                                               (gpioPortE)
#define KEY2_MODE                                                               (gpioModeInputPullFilter)
#define KEY2_PULL                                                               (1)
#define KEY2_EDGE_RISING                                                        true
#define KEY2_EDGE_FALLING                                                       true

#define LED_ORANGE_PWM_PIN                                                      (1)
#define LED_ORANGE_PWM_PORT                                                     (gpioPortB)

#define LED_RED_PWM_PIN                                                         (12)
#define LED_RED_PWM_PORT                                                        (gpioPortA)

#define LED_YELLOW_PWM_PIN                                                      (0)
#define LED_YELLOW_PWM_PORT                                                     (gpioPortB)

#define LFXO_N_PIN                                                              (8)
#define LFXO_N_PORT                                                             (gpioPortB)

#define LFXO_P_PIN                                                              (7)
#define LFXO_P_PORT                                                             (gpioPortB)

#define LFXTAL_N_PIN                                                            (8)
#define LFXTAL_N_PORT                                                           (gpioPortB)

#define LFXTAL_P_PIN                                                            (7)
#define LFXTAL_P_PORT                                                           (gpioPortB)

#define PWR_2V5_SNOOZE_PIN                                                      (5)
#define PWR_2V5_SNOOZE_PORT                                                     (gpioPortF)
#define PWR_2V5_SNOOZE_MODE                                                     (gpioModeWiredOr)

#define PWR_3V3_EN_PIN                                                          (11)
#define PWR_3V3_EN_PORT                                                         (gpioPortC)
#define PWR_3V3_EN_MODE                                                         (gpioModePushPull)

//////////////////////////////////////////////////////////////////////////////// SD_CARD
#define USART1_PORT_BASE                                                     	USART1
#define USART1_PERCLK_FREQUENCY                                            		HSE_VALUE
#define USART1_BAUDRATE                                                    		UINT32_C(4000000)

#define SD_CARD_SPI1_CLOCK                                                       (cmuClock_USART1)

#define SDCARD_SPI_TX_DMA_CHANNEL                                               (0)
#define SDCARD_SPI_RX_DMA_CHANNEL                                               (1)

#define SD_CARD_RX_IRQN                                                         USART1_RX_IRQn
#define SD_CARD_RX_INTERRUPT_PRIORITY                                           UINT32_C(5)

#define SD_CARD_TX_IRQN                                                         USART1_TX_IRQn
#define SD_CARD_TX_INTERRUPT_PRIORITY                                           UINT32_C(6)

#define SD_CARD_CS_PIN                                                          (15)
#define SD_CARD_CS_PORT                                                         (gpioPortB)
#define SD_CARD_CS_MODE                                                         (gpioModeWiredAnd)

#define SD_CARD_LS_PIN                                                          (7)
#define SD_CARD_LS_PORT                                                         (gpioPortC)
#define SD_CARD_LS_MODE                                                         (gpioModeWiredAnd)


#define SD_DETECT_PIN                                                           (12)
#define SD_DETECT_PORT                                                          (gpioPortF)
#define SD_DETECT_MODE                                                          (gpioModeInputPullFilter)
#define SD_DETECT_EDGE_RISING                                                   true
#define SD_DETECT_EDGE_FALLING                                                  true

#define SD_CARD_SPI1_MISO_PIN                                                   (1)
#define SD_CARD_SPI1_MISO_PORT                                                  (gpioPortD)
#define SD_CARD_SPI1_MISO_MODE                                                  (gpioModeInput)

#define SD_CARD_SPI1_MOSI_PIN                                                   (0)
#define SD_CARD_SPI1_MOSI_PORT                                                  (gpioPortD)
#define SD_CARD_SPI1_MOSI_MODE                                                  (gpioModePushPull)

#define SD_CARD_SPI1_SCK_PIN                                                    (2)
#define SD_CARD_SPI1_SCK_PORT                                                   (gpioPortD)
#define SD_CARD_SPI1_SCK_MODE                                                   (gpioModePushPull)

////////////////////////////////////////////////////////////////////////////////
#define SPI2_MISO_PIN                                                           (4)
#define SPI2_MISO_PORT                                                          (gpioPortB)

#define SPI2_MOSI_PIN                                                           (3)
#define SPI2_MOSI_PORT                                                          (gpioPortB)

#define SPI2_SCK_PIN                                                            (5)
#define SPI2_SCK_PORT                                                           (gpioPortB)
#define TIM0_CC0_PIN                                                            (0)
#define TIM0_CC0_PORT                                                           (gpioPortA)

#define TIM0_CC1_PIN                                                            (0)
#define TIM0_CC1_PORT                                                           (gpioPortC)

#define TIM0_CC2_PIN                                                            (1)
#define TIM0_CC2_PORT                                                           (gpioPortC)

#define TIM0_CDTI0_PIN                                                          (2)
#define TIM0_CDTI0_PORT                                                         (gpioPortC)

#define TIM0_CDTI1_PIN                                                          (3)
#define TIM0_CDTI1_PORT                                                         (gpioPortC)

#define TIM0_CDTI2_PIN                                                          (4)
#define TIM0_CDTI2_PORT                                                         (gpioPortC)

#define TIM2_CC0_PIN                                                            (8)
#define TIM2_CC0_PORT                                                           (gpioPortC)

#define TIM2_CC1_PIN                                                            (9)
#define TIM2_CC1_PORT                                                           (gpioPortC)

#define TIM2_CC2_PIN                                                            (10)
#define TIM2_CC2_PORT                                                           (gpioPortC)

#define UART0_RX_PIN                                                            (1)
#define UART0_RX_PORT                                                           (gpioPortE)

#define UART0_TX_PIN                                                            (0)
#define UART0_TX_PORT                                                           (gpioPortE)

#define UART1_CTS_PIN                                                           (6)
#define UART1_CTS_PORT                                                          (gpioPortF)

#define UART1_RTS_PIN                                                           (2)
#define UART1_RTS_PORT                                                          (gpioPortB)

#define UART1_RX_PIN                                                            (10)
#define UART1_RX_PORT                                                           (gpioPortB)

#define UART1_TX_PIN                                                            (9)
#define UART1_TX_PORT                                                           (gpioPortB)

#define UNUSED6B_PIN                                                            (6)
#define UNUSED13E_PIN                                                           (13)

#define UNUSED7F_PIN                                                            (7)
#define UNUSED6B_PORT                                                           (gpioPortB)

#define UNUSED13E_PORT                                                          (gpioPortE)
#define UNUSED7F_PORT                                                           (gpioPortF)

#define USART0_CLK_PIN                                                          (12)
#define USART0_CLK_PORT                                                         (gpioPortE)

#define USART0_RX_PIN                                                           (11)
#define USART0_RX_PORT                                                          (gpioPortE)

#define USART0_TX_PIN                                                           (10)
#define USART0_TX_PORT                                                          (gpioPortE)

#define USART1_CLK_PIN                                                          (2)
#define USART1_CLK_PORT                                                         (gpioPortD)

#define USART1_RX_PIN                                                           (1)
#define USART1_RX_PORT                                                          (gpioPortD)

#define USART1_TX_PIN                                                           (0)
#define USART1_TX_PORT                                                          (gpioPortD)

#define USB_DM_PORT                                                             (gpioPortF)
#define USB_DM_PIN                                                              (10)

#define USB_DP_PORT                                                             (gpioPortF)
#define USB_DP_PIN                                                              (11)

#define VDD_AKU_PIN                 (9)
#define VDD_AKU_PORT                (gpioPortD)

//////////////////////////////////////////////////////////////////////////////// WIFI MODULE
#define USART0_PORT_BASE                                                        USART0
#define USART0_PERCLK_FREQUENCY 	                                            HSE_VALUE
#define USART0_BAUDRATE                                                       UINT32_C(10000000)
#define WIFI_SPI0_CLOCK                                                         (cmuClock_USART0)

#define WIFI_SUPPLY_SETTING_DELAY_MS                                            UINT32_C(3)// typical value from vendor datasheet
#define WIFI_PWRON_HW_WAKEUP_DELAY_MS                                           UINT32_C(25)// typical value from vendor datasheet
#define WIFI_INIT_DELAY_MS                                                      UINT32_C(1350)// typical value from vendor datasheet
#define WIFI_MIN_HIB_DELAY_MS                                                   UINT32_C(10)// typical value from vendor datasheet
#define WIFI_HIB_WAKEUP_DELAY_MS                                                UINT32_C(50)// typical value from vendor datasheet
#define WIFI_MIN_RESET_DELAY_MS                                                 UINT32_C(5)// typical value from vendor datasheet
#define WIFI_POWER_OFF_DELAY_MS                                                 UINT32_C(20)// typical value from vendor datasheet

#define WIFI_RX_IRQN                                                            USART0_RX_IRQn
#define WIFI_RX_INTERRUPT_PRIORITY                                              UINT32_C(5)

#define WIFI_TX_INTERRUPT_PRIORITY                                              UINT32_C(6)
#define WIFI_TX_IRQN                                                            USART0_TX_IRQn

#define WIFI_SPI_TX_DMA_CHANNEL                                               (3)
#define WIFI_SPI_RX_DMA_CHANNEL                                               (2)

#define VDD_WIFI_EN_PIN                                                         (8)
#define VDD_WIFI_EN_PORT                                                        (gpioPortA)
#define VDD_WIFI_EN_MODE                                                        (gpioModePushPull)

#define WIFI_CSN_PIN                                                            (5)
#define WIFI_CSN_PORT                                                           (gpioPortC)
#define WIFI_CSN_MODE                                                           (gpioModePushPull)

#define WIFI_INT_PIN                                                            (10)
#define WIFI_INT_PORT                                                           (gpioPortA)
#define WIFI_INT_MODE                                                           (gpioModeInput)
#define WIFI_INT_EDGE_RISING                                                    true
#define WIFI_INT_EDGE_FALLING                                                   false

#define WIFI_NHIB_PIN                                                           (11)
#define WIFI_NHIB_PORT                                                          (gpioPortA)
#define WIFI_NHIB_MODE                                                          (gpioModePushPull)

#define WIFI_NRESET_PIN                                                         (15)
#define WIFI_NRESET_PORT                                                        (gpioPortA)
#define WIFI_NRESET_MODE                                                        (gpioModeWiredAnd)

#define WIFI_SPI0_MISO_PIN                                                      (11)
#define WIFI_SPI0_MISO_PORT                                                     (gpioPortE)
#define WIFI_SPI0_MISO_MODE                                                     (gpioModeInput)

#define WIFI_SPI0_MOSI_PIN                                                      (10)
#define WIFI_SPI0_MOSI_PORT                                                     (gpioPortE)
#define WIFI_SPI0_MOSI_MODE                                                     (gpioModePushPull)

#define WIFI_SPI0_SCK_PIN                                                       (12)
#define WIFI_SPI0_SCK_PORT                                                      (gpioPortE)
#define WIFI_SPI0_SCK_MODE                                                      (gpioModePushPull)


#define LORA_TX_IRQN                                                            USART2_TX_IRQn
#define LORA_TX_INTERRUPT_PRIORITY                                              UINT32_C(5)

#define LORA_RX_IRQN                                                            USART2_RX_IRQn
#define LORA_RX_INTERRUPT_PRIORITY                                              UINT32_C(5)

#define USART2_PORT_BASE                                                        USART2
#define USART2_PERCLK_FREQUENCY                                               	0
#define USART2_BAUDRATE                                                       UINT32_C(7000000)
#define LORA_SPI_CLOCK                                                         (cmuClock_USART2)
#define LORA_ROUTE_LOCATION														0

#define BSP_LoRa_RADIO_DIO_0_PORT												gpioPortC
#define BSP_LoRa_RADIO_DIO_0_PIN												0
#define BSP_LoRa_RADIO_DIO_0_MODE												gpioModeInputPull
#define BSP_LoRa_RADIO_DIO_0_DOUT												0

#define BSP_LoRa_RADIO_DIO_1_PORT												gpioPortB
#define BSP_LoRa_RADIO_DIO_1_PIN												14
#define BSP_LoRa_RADIO_DIO_1_MODE												gpioModeInputPull
#define BSP_LoRa_RADIO_DIO_1_DOUT												0

#define BSP_LoRa_RADIO_DIO_2_PORT												gpioPortB
#define BSP_LoRa_RADIO_DIO_2_PIN												13
#define BSP_LoRa_RADIO_DIO_2_MODE												gpioModeInputPull
#define BSP_LoRa_RADIO_DIO_2_DOUT												0

#define BSP_LoRa_RADIO_DIO_3_PORT												gpioPortB
#define BSP_LoRa_RADIO_DIO_3_PIN												12
#define BSP_LoRa_RADIO_DIO_3_MODE												gpioModeInputPull
#define BSP_LoRa_RADIO_DIO_3_DOUT												0

#define BSP_LoRa_RADIO_MISO_PORT												gpioPortC
#define BSP_LoRa_RADIO_MISO_PIN													3
#define BSP_LoRa_RADIO_MISO_MODE												gpioModeInputPull
#define BSP_LoRa_RADIO_MISO_DOUT												0

#define BSP_LoRa_RADIO_MOSI_PORT												gpioPortC
#define BSP_LoRa_RADIO_MOSI_PIN													2
#define BSP_LoRa_RADIO_MOSI_MODE												gpioModePushPullDrive
#define BSP_LoRa_RADIO_MOSI_DOUT												0

#define BSP_LoRa_RADIO_CLK_PORT													gpioPortC
#define BSP_LoRa_RADIO_CLK_PIN													4
#define BSP_LoRa_RADIO_CLK_MODE													gpioModePushPullDrive
#define BSP_LoRa_RADIO_CLK_DOUT													0

#define BSP_LoRa_RADIO_CS_PORT													gpioPortC
#define BSP_LoRa_RADIO_CS_PIN													5
#define BSP_LoRa_RADIO_CS_MODE													gpioModePushPullDrive
#define BSP_LoRa_RADIO_CS_DOUT													1

#define BSP_SERIAL_LEUART_PORT                                                   (LEUART1)
#define BSP_SERIAL_LEUART_CLOCK												     (cmuClock_LEUART1)
#define BSP_LEUART_CLOCK_SELECT            										 (cmuSelect_LFXO)
#define BSP_LEUART_CLOCKDIV_VALUE          										 (UINT16_C(cmuClkDiv_1))

#define BSP_LEUART_RX_PORT                                                       (gpioPortC)
#define BSP_LEUART_RX_PIN                                                        (UINT16_C(7))
#define BSP_LEUART_RX_MODE                                                       (gpioModeInput)
#define BSP_LEUART_RX_DOUT                                                       (UINT8_C(1))
#define BSP_LEUART_RX_DRIVE_MODE           									     (gpioDriveModeStandard)

#define BSP_LEUART_TX_PORT                                                       (gpioPortC)
#define BSP_LEUART_TX_PIN                                                        (UINT16_C(6))
#define BSP_LEUART_TX_MODE                                                       (gpioModePushPull)
#define BSP_LEUART_TX_DOUT													   	 (UINT8_C(1))
#define BSP_LEUART_TX_DRIVE_MODE           									 	 (gpioDriveModeStandard)

#endif /* BoardSettings_H */

/** @}*/
