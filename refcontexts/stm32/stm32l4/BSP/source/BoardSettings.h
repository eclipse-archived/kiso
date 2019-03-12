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
 * @ingroup BSP_BSE_BOARD
 * @{
 * @file
 * @brief Board settings header file.
 * @details This file holds static configurations which are meant to be
 * assigned to a peripheral at a certain point in time for the Initialize,
 * Connect, Enable, Disable, Disconnect functions.
 * It is preferred to have only macro definitions in this file so that it
 * contains compile time necessary information only.
 */

#ifndef BOARDSETTINGS_H_
#define BOARDSETTINGS_H_

#if !defined(USE_HAL_DRIVER) || !defined(STM32L486xx)
#error "USE_HAL_DRIVER and BCDS_DEVICE_PACKAGE_TYPE must be defined to include stm32l486xx.h, stm32l4xx_hal_conf.h and stm32l4xx_hal.h via stm32l4xx.h"
#endif

#include "stm32l4xx.h"

/* Board Specific Settings */
/*
 * 3V3 Domain
 */
#define BSP_IO_PIN_LOW                              GPIO_PIN_RESET
#define BSP_IO_PIN_HIGH                             GPIO_PIN_SET

#ifndef BSP_FEATURE_ANALOG_SENSOR
#define BSP_FEATURE_ANALOG_SENSOR                                               1
#endif /* ifndef BSP_FEATURE_ANALOG_SENSOR */

#ifndef BSP_FEATURE_LDO_TLV70233
#define BSP_FEATURE_LDO_TLV70233                                                1
#endif /* ifndef BSP_FEATURE_LDO_TLV70233 */

/**
 * @ingroup BSP_BSE_BOARD
 * @{
 */
#define BOARD_GPIO_CLOCK_ENABLE()             do {\
                                                    __HAL_RCC_GPIOA_CLK_ENABLE(); \
                                                    __HAL_RCC_GPIOB_CLK_ENABLE(); \
                                                    __HAL_RCC_GPIOC_CLK_ENABLE(); \
                                                    __HAL_RCC_GPIOD_CLK_ENABLE(); \
                                                    __HAL_RCC_GPIOE_CLK_ENABLE(); \
                                                    __HAL_RCC_GPIOF_CLK_ENABLE(); \
                                                    __HAL_RCC_GPIOG_CLK_ENABLE(); \
                                                    __HAL_RCC_GPIOH_CLK_ENABLE(); \
                                                } while(0)

#define HARDWARE_VARIANT_ID_MAX                     UINT8_C(8)

#define R5010_PORT                                  GPIOE
#define R5010_PIN                                   GPIO_PIN_13
#define R5010_MODE                                  GPIO_MODE_INPUT
#define R5010_PULL                                  GPIO_PULLUP
#define R5010_SPEED                                 GPIO_SPEED_LOW

#define R5011_PORT                                  GPIOE
#define R5011_PIN                                   GPIO_PIN_11
#define R5011_MODE                                  GPIO_MODE_INPUT
#define R5011_PULL                                  GPIO_PULLUP
#define R5011_SPEED                                 GPIO_SPEED_LOW

#define R5012_PORT                                  GPIOE
#define R5012_PIN                                   GPIO_PIN_9
#define R5012_MODE                                  GPIO_MODE_INPUT
#define R5012_PULL                                  GPIO_PULLUP
#define R5012_SPEED                                 GPIO_SPEED_LOW

#define PWR3V3_EN_PORT                              GPIOE
#define PWR3V3_EN_PIN                               GPIO_PIN_7
#define PWR3V3_EN_MODE                              GPIO_MODE_OUTPUT_PP
#define PWR3V3_EN_PULL                              GPIO_NOPULL
#define PWR3V3_EN_SPEED                             GPIO_SPEED_LOW
/*
 * External Power Supply Voltage sensing
 */
#define VEXT_SENSE_ADC_PORT                         GPIOA
#define VEXT_SENSE_ADC_PIN                          GPIO_PIN_3
#define VEXT_SENSE_ADC_MODE                         GPIO_MODE_ANALOG_ADC_CONTROL
#define VEXT_SENSE_ADC_PULL                         GPIO_NOPULL

#define VEXT_SENSE_ADC_CHANNEL                      ADC_CHANNEL_8

#define VEXT_DCDC_PWR_GOOD_PORT                     GPIOA
#define VEXT_DCDC_PWR_GOOD_PIN                      GPIO_PIN_0
/** @warning: HAL_GPIO_Init() function in the stm32cube library configures the
 *  external interrupt line and overwrites the previous settings in the
 *  SYSCFG_EXTICRx. be careful when working with event or interrupt modes
 */
#define VEXT_DCDC_PWR_GOOD_MODE_STOP                GPIO_MODE_EVT_RISING_FALLING
#define VEXT_DCDC_PWR_GOOD_MODE                     GPIO_MODE_INPUT
#define VEXT_DCDC_PWR_GOOD_PULL                     GPIO_NOPULL
#define VEXT_DCDC_PWR_GOOD_SPEED                    GPIO_SPEED_LOW

/**@}*//* @ingroup BSP_BSE_BOARD */

/**
 * @ingroup BSP_BSE_CHARGING_MODULE
 * @{
 */
#define CHARGER_GPIO_CLOCK_ENABLE()             do {\
                                                    __HAL_RCC_GPIOA_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOB_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOC_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOD_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOG_CLK_ENABLE();\
                                                } while(0)

/*
 * ADC DMA interrupt priority
 */
#define CHARGER_DMA_IRQn                            DMA1_Channel1_IRQn
#define CHARGER_DMA_PREEMPT_PRIORITY                UINT32_C(15)
#define CHARGER_DMA_SUBPRIORITY                     UINT32_C(0)

/*
 * CHARGING_STAT pin external interrupt priority
 */
#define CHARGING_STAT_EXTI_IRQN                     EXTI9_5_IRQn
#define CHARGING_STAT_EXTI_PREEMPT_PRIORITY         UINT32_C(7)
#define CHARGING_STAT_EXTI_SUBPRIORITY              UINT32_C(0)

/*
 * CHARGING_STAT signal timer configuration
 */
#define CHARGING_TIMER_CLOCK_ENABLE()           do {\
                                                    __HAL_RCC_TIM4_CLK_ENABLE();\
                                                } while(0)

#define CHARGING_TIMER_CLOCK_DISABLE()          do {\
                                                    __HAL_RCC_TIM4_CLK_DISABLE();\
                                                } while(0)

/* The Charger signals an error with a roughly 2Hz square signal; if there is no either
 * rising or falling edge on the CHARGING_STAT pin for longer than
 * CHARGING_ERROR_TIMEOUT_TIME_MS time it means there is no error.
 */
#define CHARGING_ERROR_TIMEOUT_TIME_MS          UINT32_C(500)

#define CHARGING_TIMER                          TIM4

#define CHARGING_TIMER_PRESCALER                UINT32_C(47999)
#define CHARGING_TIMER_COUNTER_MODE             TIM_COUNTERMODE_UP;
#define CHARGING_TIMER_CLK_DIV                  TIM_CLOCKDIVISION_DIV1;

/*
 * CHARGING_STAT signal timer priority
 */
#define CHARGING_TIMER_IRQN                     TIM4_IRQn
#define CHARGING_TIMER_PREEMPT_PRIORITY         UINT32_C(6)
#define CHARGING_TIMER_SUBPRIORITY              UINT32_C(0)

/*
 * Battery sensing
 */
/* Battery voltage channel control */
#define VBAT_SENSE_ADC_EN_PORT                      GPIOB
#define VBAT_SENSE_ADC_EN_PIN                       GPIO_PIN_0
#define VBAT_SENSE_ADC_EN_MODE                      GPIO_MODE_OUTPUT_PP
#define VBAT_SENSE_ADC_EN_PULL                      GPIO_NOPULL
#define VBAT_SENSE_ADC_EN_SPEED                     GPIO_SPEED_LOW

/* Battery current channel */
#define AD_IBAT_PORT                                GPIOC
#define AD_IBAT_PIN                                 GPIO_PIN_3
#define AD_IBAT_MODE                                GPIO_MODE_ANALOG_ADC_CONTROL
#define AD_IBAT_PULL                                GPIO_NOPULL
#define AD_IBAT_CHANNEL                             ADC_CHANNEL_4

/* Battery voltage channel */
#define VBAT_SENSE_ADC_PORT                         GPIOC
#define VBAT_SENSE_ADC_PIN                          GPIO_PIN_1
#define VBAT_SENSE_ADC_MODE                         GPIO_MODE_ANALOG_ADC_CONTROL
#define VBAT_SENSE_ADC_PULL                         GPIO_NOPULL
#define VBAT_SENSE_ADC_CHANNEL                      ADC_CHANNEL_2

#define GSM_VBAT_MIN                                INT32_C(3400)
/*
 * Battery Charger
 */
#define CHARGING_EN_PORT                            GPIOG
#define CHARGING_EN_PIN                             GPIO_PIN_10
#define CHARGING_EN_MODE                            GPIO_MODE_OUTPUT_OD
#define CHARGING_EN_PULL                            GPIO_NOPULL
#define CHARGING_EN_SPEED                           GPIO_SPEED_LOW

#define CHARGING_IN_EN1_PORT                        GPIOG
#define CHARGING_IN_EN1_PIN                         GPIO_PIN_11
#define CHARGING_IN_EN1_MODE                        GPIO_MODE_OUTPUT_PP
#define CHARGING_IN_EN1_PULL                        GPIO_NOPULL
#define CHARGING_IN_EN1_SPEED                       GPIO_SPEED_LOW

#define CHARGING_IN_EN2_PORT                        GPIOG
#define CHARGING_IN_EN2_PIN                         GPIO_PIN_6
#define CHARGING_IN_EN2_MODE                        GPIO_MODE_OUTPUT_PP
#define CHARGING_IN_EN2_PULL                        GPIO_NOPULL
#define CHARGING_IN_EN2_SPEED                       GPIO_SPEED_LOW

/** @todo 2016-11-17, BCDS/ENG1: add signal as (redundant) wake-up source and change interfaces to event driven behavior */
#define CHARGING_PWR_GOOD_PORT                      GPIOC
#define CHARGING_PWR_GOOD_PIN                       GPIO_PIN_5
#define CHARGING_PWR_GOOD_MODE                      GPIO_MODE_INPUT
#define CHARGING_PWR_GOOD_PULL                      GPIO_PULLUP

/** @todo 2016-11-17, BCDS/ENG1: change interfaces to event driven behaviour to allow error notification from charger chip */
#define CHARGING_STAT_PORT                          GPIOD
#define CHARGING_STAT_PIN                           GPIO_PIN_7
#define CHARGING_STAT_MODE                          GPIO_MODE_IT_RISING_FALLING
#define CHARGING_STAT_PULL                          GPIO_PULLUP
/**@}*//* @ingroup BSP_BSE_CHARGING_MODULE */

/**
 * @ingroup BSP_BSE_MEMORY_MODULE
 * @{
 */
#define MEM_DMA_TX_CHANNEL                          DMA1_Channel3
#define MEM_DMA_TX_REQUEST                          DMA_REQUEST_1
#define MEM_DMA_TX_DIRECTION                        DMA_MEMORY_TO_PERIPH
#define MEM_DMA_TX_PINC                             DMA_PINC_DISABLE
#define MEM_DMA_TX_MINC                             DMA_MINC_ENABLE
#define MEM_DMA_TX_PDATA_ALIGN                      DMA_PDATAALIGN_BYTE
#define MEM_DMA_TX_MDATA_ALIGN                      DMA_MDATAALIGN_BYTE
#define MEM_DMA_TX_MODE                             DMA_NORMAL
#define MEM_DMA_TX_PRIORITY                         DMA_PRIORITY_LOW

#define MEM_DMA_RX_CHANNEL                          DMA1_Channel2
#define MEM_DMA_RX_REQUEST                          DMA_REQUEST_1
#define MEM_DMA_RX_DIRECTION                        DMA_PERIPH_TO_MEMORY
#define MEM_DMA_RX_PINC                             DMA_PINC_DISABLE
#define MEM_DMA_RX_MINC                             DMA_MINC_ENABLE
#define MEM_DMA_RX_PDATA_ALIGN                      DMA_PDATAALIGN_BYTE
#define MEM_DMA_RX_MDATA_ALIGN                      DMA_MDATAALIGN_BYTE
#define MEM_DMA_RX_MODE                             DMA_NORMAL
#define MEM_DMA_RX_PRIORITY                         DMA_PRIORITY_HIGH

#define MEM_SPI_INSTANCE                            SPI1
#define MEM_SPI_BR_PRESCALER                        SPI_BAUDRATEPRESCALER_4
#define MEM_SPI_DIRECTION                           SPI_DIRECTION_2LINES
#define MEM_SPI_CLOCK_PHASE                         SPI_PHASE_1EDGE
#define MEM_SPI_POLARITY                            SPI_POLARITY_LOW
#define MEM_SPI_DATASIZE                            SPI_DATASIZE_8BIT
#define MEM_SPI_FIRST_BIT                           SPI_FIRSTBIT_MSB
#define MEM_SPI_TI_MODE                             SPI_TIMODE_DISABLE
#define MEM_SPI_CRC                                 SPI_CRCCALCULATION_DISABLE
#define MEM_SPI_CRC_POL                             7
#define MEM_SPI_CRC_LENGTH                          SPI_CRC_LENGTH_8BIT
#define MEM_SPI_NSS                                 SPI_NSS_SOFT
#define MEM_SPI_NSS_MODE                            SPI_NSS_PULSE_DISABLE
#define MEM_SPI_MODE                                SPI_MODE_MASTER

#define MEM_GPIO_CLOCK_ENABLE()                     __HAL_RCC_GPIOG_CLK_ENABLE()
#define MEM_DELAY_BEFORE_WRITE_MS                   UINT32_C(6)
#define MEM_SPI_TRANSMIT_END_TIMEOUT_MS             UINT32_C(5)
#define MEM_SPI_RECEIVE_END_TIMEOUT_MS              UINT32_C(5)
#define MEM_DISABLE_DELAY_MS                        UINT32_C(1)

/* Interrupts*/
#define MEM_TX_DMA_IRQ                              DMA1_Channel3_IRQn
#define MEM_TX_DMA_PREEMPT_PRIORITY                 UINT32_C(7)
#define MEM_TX_SUBPRIORITY                          UINT32_C(0)

#define MEM_RX_DMA_IRQ                              DMA1_Channel2_IRQn
#define MEM_RX_DMA_PREEMPT_PRIORITY                 UINT32_C(6)
#define MEM_RX_SUBPRIORITY                          UINT32_C(0)

#define MEM_CS_N_PORT                               GPIOG
#define MEM_CS_N_PIN                                GPIO_PIN_5
/** @todo 2017-02-03, BCDS/ENG1: Based on the STM32L4x6 datasheet and reference
 *  manual the OSPEED and output mode configuration settings for MEM_CS_N shall
 *  be GPO/OD/OS0, but with those settings the external peripheral does not work
 *  at the desired bus speed of 12 MHz.
 *  The STM32CubeMX code generator sets the NSS pin under hardware control to
 *  AF5/PP/OS3 and MOSI/SCK pins to AF5/PP/OS0 which is rather odd.
 *  Further analysis will be required to figure out what is causing the issues.
 *   */
#define MEM_CS_N_MODE                               GPIO_MODE_OUTPUT_PP
#define MEM_CS_N_PULL                               GPIO_NOPULL
#define MEM_CS_N_SPEED                              GPIO_SPEED_FREQ_MEDIUM

#define MEM_HOLD_N_PORT                             GPIOG
#define MEM_HOLD_N_PIN                              GPIO_PIN_1
#define MEM_HOLD_N_MODE                             GPIO_MODE_OUTPUT_PP
#define MEM_HOLD_N_PULL                             GPIO_NOPULL
#define MEM_HOLD_N_SPEED                            GPIO_SPEED_FREQ_MEDIUM

#define MEM_MISO_PORT                               GPIOG
#define MEM_MISO_PIN                                GPIO_PIN_3
#define MEM_MISO_MODE                               GPIO_MODE_AF_PP
#define MEM_MISO_PULL                               GPIO_NOPULL
#define MEM_MISO_SPEED                              GPIO_SPEED_FREQ_MEDIUM
#define MEM_MISO_ALTERNATE                          GPIO_AF5_SPI1

#define MEM_MOSI_PORT                               GPIOG
#define MEM_MOSI_PIN                                GPIO_PIN_4
#define MEM_MOSI_MODE                               GPIO_MODE_AF_PP
#define MEM_MOSI_PULL                               GPIO_NOPULL
#define MEM_MOSI_SPEED                              GPIO_SPEED_FREQ_MEDIUM
#define MEM_MOSI_ALTERNATE                          GPIO_AF5_SPI1

#define MEM_SCK_PORT                                GPIOG
#define MEM_SCK_PIN                                 GPIO_PIN_2
#define MEM_SCK_MODE                                GPIO_MODE_AF_PP
#define MEM_SCK_PULL                                GPIO_NOPULL
#define MEM_SCK_SPEED                               GPIO_SPEED_FREQ_MEDIUM
#define MEM_SCK_ALTERNATE                           GPIO_AF5_SPI1

#define MEM_W_N_PORT                                GPIOG
#define MEM_W_N_PIN                                 GPIO_PIN_9
#define MEM_W_N_MODE                                GPIO_MODE_OUTPUT_PP
#define MEM_W_N_PULL                                GPIO_NOPULL
#define MEM_W_N_SPEED                               GPIO_SPEED_FREQ_MEDIUM
/**@}*//* @ingroup BSP_BSE_MEMORY_MODULE */

/**
 * @ingroup BSP_BSE_BT_MODULE
 * @{
 */
/* GPIO ports clock enable commands necessary for BT Module*/
/* Refer here to all the necessary GPIO clock enabling commands for BT module */
#define BT_GPIO_CLOCK_ENABLE()                  do {\
                                                    __HAL_RCC_GPIOA_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOE_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOB_CLK_ENABLE();\
                                                } while(0)
#define BT_UART_CLOCK_ENABLE()                  __HAL_RCC_USART1_CLK_ENABLE()
#define BT_UART_CLOCK_DISABLE()                 __HAL_RCC_USART1_CLK_DISABLE()

#define BT_UART_CLOCK_SELECT                    RCC_PERIPHCLK_USART1
#define BT_UART_CLOCK_SOURCE_ACTIVE             RCC_USART1CLKSOURCE_SYSCLK
#define BT_UART_CLOCK_SOURCE_STOP1              RCC_USART1CLKSOURCE_HSI
#define BT_SERIAL_PORT                          USART1
#define BT_BAUD_RATE                            UINT32_C(115200)
#define BT_DATA_BITS                            UART_WORDLENGTH_8B
#define BT_STOP_BITS                            UART_STOPBITS_1
#define BT_PARITY                               UART_PARITY_NONE
#define BT_MODE                                 UART_MODE_TX_RX
#define BT_FLOW_CONTROL                         UART_HWCONTROL_NONE
#define BT_SAMPLING                             UART_OVERSAMPLING_16
#define BT_ONE_BIT_SAMPLING                     UART_ONEBIT_SAMPLING_DISABLED
#define BT_ADVANCED_INT                         UART_ADVFEATURE_NO_INIT

#define BT_STARTUP_DELAY                        UINT32_C(4)
#define BT_WAKEUP_DELAY                         UINT32_C(3)
#define BT_RESET_PROCESSING_DELAY               UINT32_C(5)
#define BT_RESET_PULLUP_DELAY                   UINT32_C(2)
#define BT_TRANSMIT_DISABLE_TIMEOUT             UINT32_C(2)
#define BT_POWER_DISCHARGE_DELAY                UINT32_C(250)

#define BT_USART_IRQn                           USART1_IRQn
#define BT_USART_PREEMPT_PRIORITY               UINT32_C(14)
#define BT_USART_SUBPRIORITY                    UINT32_C(0)

/*---DMA Settings for the BT UART---------------------------------------------*/

#define BT_DMA_TX_CHANNEL                          DMA2_Channel6
#define BT_DMA_TX_REQUEST                          DMA_REQUEST_2
#define BT_DMA_TX_DIRECTION                        DMA_MEMORY_TO_PERIPH
#define BT_DMA_TX_PINC                             DMA_PINC_DISABLE
#define BT_DMA_TX_MINC                             DMA_MINC_ENABLE
#define BT_DMA_TX_PDATA_ALIGN                      DMA_PDATAALIGN_BYTE
#define BT_DMA_TX_MDATA_ALIGN                      DMA_MDATAALIGN_BYTE
#define BT_DMA_TX_MODE                             DMA_NORMAL
#define BT_DMA_TX_PRIORITY                         DMA_PRIORITY_LOW

#define BT_DMA_RX_CHANNEL                          DMA2_Channel7
#define BT_DMA_RX_REQUEST                          DMA_REQUEST_2
#define BT_DMA_RX_DIRECTION                        DMA_PERIPH_TO_MEMORY
#define BT_DMA_RX_PINC                             DMA_PINC_DISABLE
#define BT_DMA_RX_MINC                             DMA_MINC_ENABLE
#define BT_DMA_RX_PDATA_ALIGN                      DMA_PDATAALIGN_BYTE
#define BT_DMA_RX_MDATA_ALIGN                      DMA_MDATAALIGN_BYTE
#define BT_DMA_RX_MODE                             DMA_CIRCULAR
#define BT_DMA_RX_PRIORITY                         DMA_PRIORITY_HIGH

/* Interrupts*/
#define BT_TX_DMA_IRQ                              DMA2_Channel6_IRQn
#define BT_TX_DMA_PREEMPT_PRIORITY                 UINT32_C(14)
#define BT_TX_SUBPRIORITY                          UINT32_C(0)

#define BT_RX_DMA_IRQ                              DMA2_Channel7_IRQn
#define BT_RX_DMA_PREEMPT_PRIORITY                 UINT32_C(14)
#define BT_RX_SUBPRIORITY                          UINT32_C(0)

/*---GPIO Pins for the BT module----------------------------------------------*/
#define BT_RST_PORT                             GPIOE
#define BT_RST_PIN                              GPIO_PIN_15
#define BT_RST_MODE                             GPIO_MODE_OUTPUT_PP
#define BT_RST_PULL                             GPIO_NOPULL
#define BT_RST_SPEED                            GPIO_SPEED_LOW

#define BT_UART_RX_PORT                         GPIOB
#define BT_UART_RX_PIN                          GPIO_PIN_7
#define BT_UART_RX_MODE                         GPIO_MODE_AF_PP
#define BT_UART_RX_PULL                         GPIO_NOPULL
#define BT_UART_RX_SPEED                        GPIO_SPEED_LOW
#define BT_UART_RX_ALTERNATE                    GPIO_AF7_USART1

#define BT_UART_TX_PORT                         GPIOB
#define BT_UART_TX_PIN                          GPIO_PIN_6
#define BT_UART_TX_MODE                         GPIO_MODE_AF_PP
#define BT_UART_TX_PULL                         GPIO_NOPULL
#define BT_UART_TX_SPEED                        GPIO_SPEED_LOW
#define BT_UART_TX_ALTERNATE                    GPIO_AF7_USART1

#define BT_WAKEUP_PORT                          GPIOB
#define BT_WAKEUP_PIN                           GPIO_PIN_12
#define BT_WAKEUP_MODE                          GPIO_MODE_OUTPUT_PP
#define BT_WAKEUP_PULL                          GPIO_NOPULL
#define BT_WAKEUP_SPEED                         GPIO_SPEED_LOW

#define BT_3V3_EN_PORT                          GPIOA
#define BT_3V3_EN_PIN                           GPIO_PIN_12
#define BT_3V3_EN_MODE                          GPIO_MODE_OUTPUT_OD
#define BT_3V3_EN_PULL                          GPIO_NOPULL
#define BT_3V3_EN_SPEED                         GPIO_SPEED_LOW
/**@}*//* @ingroup BSP_BSE_BT_MODULE */

/**
 * @ingroup BSP_BSE_CAN_MODULE
 * @{
 */
/* GPIO ports clock enable commands necessary for CAN Module*/
#define CAN_GPIO_CLOCK_ENABLE()                 do {\
                                                    __HAL_RCC_GPIOE_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOD_CLK_ENABLE();\
                                                } while(0)
/* Delays and timeouts */
#define CAN_POWER_ON_DELAY                      UINT32_C(5)
#define CAN_ENABLE_DELAY                        UINT32_C(1)

#define CAN1_RX1_PREEMPT_PRIORITY                 UINT32_C(5)
#define CAN1_RX1_SUBPRIORITY                      UINT32_C(0)

#define CAN1_TX_PREEMPT_PRIORITY                 UINT32_C(5)
#define CAN1_TX_SUBPRIORITY                      UINT32_C(0)

#define CAN1_SCE_PREEMPT_PRIORITY                UINT32_C(5)
#define CAN1_SCE_SUBPRIORITY                     UINT32_C(0)

/*---GPIO Pins for CAN transceiver--------------------------------------------*/
#define CAN_EN_PORT                             GPIOE
#define CAN_EN_PIN                              GPIO_PIN_1
#define CAN_EN_MODE                             GPIO_MODE_OUTPUT_PP
#define CAN_EN_PULL                             GPIO_NOPULL
#define CAN_EN_SPEED                            GPIO_SPEED_LOW

#define CAN_RX_PORT                             GPIOD
#define CAN_RX_PIN                              GPIO_PIN_0
#define CAN_RX_MODE                             GPIO_MODE_AF_PP
#define CAN_RX_PULL                             GPIO_NOPULL
#define CAN_RX_SPEED                            GPIO_SPEED_LOW
#define CAN_RX_ALTERNATE                        GPIO_AF9_CAN1

#define CAN_TX_PORT                             GPIOD
#define CAN_TX_PIN                              GPIO_PIN_1
#define CAN_TX_MODE                             GPIO_MODE_AF_PP
#define CAN_TX_PULL                             GPIO_NOPULL
#define CAN_TX_SPEED                            GPIO_SPEED_LOW
#define CAN_TX_ALTERNATE                        GPIO_AF9_CAN1
/**@}*//* @ingroup BSP_BSE_CAN_MODULE */

/**
 * @ingroup BSP_BSE_GNSS_MODULE
 * @{
 */
/*---CLock definition for the UART interface used by the GNSS module----------*/
#define GNSS_UART_CLOCK_SELECT                  RCC_PERIPHCLK_LPUART1
#define GNSS_UART_CLOCK_SOURCE                  RCC_LPUART1CLKSOURCE_LSE
/*---Interrupt priority for the GNSS UART interface ---------------------------*/
#define GNSS_UART_IRQn                          LPUART1_IRQn
#define GNSS_PREEMPT_PRIORITY                   UINT32_C(14)
#define GNSS_SUBPRIORITY                        UINT32_C(0)
/*---GPIO ports clock enable commands necessary for GNSS Module --------------*/
#define GNSS_GPIO_CLOCK_ENABLE()                do {\
                                                    __HAL_RCC_GPIOA_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOB_CLK_ENABLE();\
                                                } while(0)
#define GNSS_UART_CLOCK_ENABLE()                __HAL_RCC_LPUART1_CLK_ENABLE()
#define GNSS_UART_CLOCK_DISABLE()               __HAL_RCC_LPUART1_CLK_DISABLE()

/*---Serial interface parameter for the GNSS module---------------------------*/
#define GNSS_SERIAL_PORT                        LPUART1
#define GNSS_BAUD_RATE                          UINT32_C(9600)
#define GNSS_DATA_BITS                          UART_WORDLENGTH_8B
#define GNSS_STOP_BITS                          UART_STOPBITS_1
#define GNSS_MODE                               UART_MODE_TX_RX
#define GNSS_PARITY                             UART_PARITY_NONE
#define GNSS_FLOW_CONTROL                       UART_HWCONTROL_NONE
#define GNSS_SAMPLING                           UART_OVERSAMPLING_16
#define GNSS_ONE_BIT_SAMPLING                   UART_ONEBIT_SAMPLING_DISABLED
#define GNSS_ADVANCED_INT                       UART_ADVFEATURE_NO_INIT

/*---GNSS Power Delay/Timeout-------------------------------------------------*/
#define GNSS_RESET_PULL_DELAY                   UINT32_C(50)
#define GNSS_POWER_ON_DELAY                     UINT32_C(100)
#define GNSS_RESET_PROCESSING_DELAY             UINT32_C(200)
#define GNSS_TRANSMIT_DISABLE_TIMEOUT           UINT32_C(5)
/*---GPIO PIN definitions for the GNSS module----------------------------------*/
#define GNSS_CLK32_PORT                         GPIOA
#define GNSS_CLK32_PIN                          GPIO_PIN_2
#define GNSS_CLK32_MODE                         GPIO_MODE_ANALOG
#define GNSS_CLK32_PULL                         GPIO_NOPULL
#define GNSS_CLK32_SPEED                        GPIO_SPEED_LOW

#define GNSS_EN_PORT                            GPIOB
#define GNSS_EN_PIN                             GPIO_PIN_14
#define GNSS_EN_MODE                            GPIO_MODE_OUTPUT_OD
#define GNSS_EN_PULL                            GPIO_NOPULL
#define GNSS_EN_SPEED                           GPIO_SPEED_LOW

#define GNSS_RESETN_PORT                        GPIOB
#define GNSS_RESETN_PIN                         GPIO_PIN_15
#define GNSS_RESETN_MODE                        GPIO_MODE_OUTPUT_OD
#define GNSS_RESETN_PULL                        GPIO_NOPULL
#define GNSS_RESETN_SPEED                       GPIO_SPEED_LOW

#define GNSS_UART_RXD_PORT                      GPIOB
#define GNSS_UART_RXD_PIN                       GPIO_PIN_10
#define GNSS_UART_RXD_MODE                      GPIO_MODE_AF_PP
#define GNSS_UART_RXD_PULL                      GPIO_NOPULL
#define GNSS_UART_RXD_SPEED                     GPIO_SPEED_LOW
#define GNSS_UART_RXD_ALTERNATE                 GPIO_AF8_LPUART1

#define GNSS_UART_TXD_PORT                      GPIOB
#define GNSS_UART_TXD_PIN                       GPIO_PIN_11
#define GNSS_UART_TXD_MODE                      GPIO_MODE_AF_PP
#define GNSS_UART_TXD_PULL                      GPIO_NOPULL
#define GNSS_UART_TXD_SPEED                     GPIO_SPEED_LOW
#define GNSS_UART_TXD_ALTERNATE                 GPIO_AF8_LPUART1
/**@}*//* @ingroup BSP_BSE_GNSS_MODULE */

/**
 * @ingroup BSP_BSE_GSM_MODULE
 * @{
 */
/*---CLock definition for the UART interface used by the GSM module-----------*/
#define GSM_UART_CLOCK_SELECT                   RCC_PERIPHCLK_USART2
#define GSM_UART_CLOCK_SOURCE                   RCC_USART2CLKSOURCE_SYSCLK
/*---Interrupt priority for the GSM UART interface----------------------------*/
#define GSM_UART_IRQn                           USART2_IRQn
#define GSM_UART_PREEMPT_PRIORITY               UINT32_C(14)
#define GSM_UART_SUBPRIORITY                    UINT32_C(0)
/*---GPIO ports clock enable commands necessary for GSM Module----------------*/
#define GSM_GPIO_CLOCK_ENABLE()                 do {\
                                                    __HAL_RCC_GPIOF_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOC_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOA_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOD_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOE_CLK_ENABLE();\
                                                } while(0)
#define GSM_UART_CLOCK_ENABLE()                 __HAL_RCC_USART2_CLK_ENABLE()
#define GSM_UART_CLOCK_DISABLE()                __HAL_RCC_USART2_CLK_DISABLE()

/*---Serial interface parameter for the GSM module----------------------------*/
#define GSM_SERIAL_PORT                         USART2
#define GSM_BAUD_RATE                           UINT32_C(115200)
#define GSM_DATA_BITS                           UART_WORDLENGTH_8B
#define GSM_STOP_BITS                           UART_STOPBITS_1
#define GSM_MODE                                UART_MODE_TX_RX
#define GSM_PARITY                              UART_PARITY_NONE
#define GSM_FLOW_CONTROL                        UART_HWCONTROL_NONE
#define GSM_SAMPLING                            UART_OVERSAMPLING_16
#define GSM_ONE_BIT_SAMPLING                    UART_ONEBIT_SAMPLING_DISABLED
#define GSM_ADVANCED_INT                        UART_ADVFEATURE_NO_INIT

/*---GSM Power Delay/Timeout--------------------------------------------------*/
#define GSM_POWER_UP_TIMEOUT                    UINT32_C(4000)
#define GSM_POWER_DOWN_TIMEOUT                  UINT32_C(3000)
#define GSM_PWRKEY_ON_DELAY                     UINT32_C(1500)
#define GSM_PWRKEY_OFF_DELAY                    UINT32_C(1500)
#define GSM_DTR_WAKEUP_DELAY                    UINT32_C(60)
#define GSM_RESTART_DELAY                       UINT32_C(900)
#define GSM_RESET_DELAY                         UINT32_C(150)
/* According to datasheet, 400us (1ms) delay should be enough, but that does not
 * always work (fails about 1/5). Doubling the delay fixes the problem (works 100/100). */
#define GSM_RESET_STATUS_DELAY                  UINT32_C(2)
#define GSM_RESET_TIMEOUT                       UINT32_C(3000)
#define GSM_TRANSMIT_DISABLE_TIMEOUT            UINT32_C(2)

/*---GPIO PIN definitions for the GSM module----------------------------------*/
#define GSM_STATUS_PORT                         GPIOF
#define GSM_STATUS_PIN                          GPIO_PIN_2
#define GSM_STATUS_MODE                         GPIO_MODE_INPUT
#define GSM_STATUS_PULL                         GPIO_PULLDOWN

#define GSM_PWR_EN_PORT                         GPIOC
#define GSM_PWR_EN_PIN                          GPIO_PIN_9
#define GSM_PWR_EN_MODE                         GPIO_MODE_OUTPUT_PP
#define GSM_PWR_EN_PULL                         GPIO_NOPULL
#define GSM_PWR_EN_SPEED                        GPIO_SPEED_LOW

#define GSM_PWRKEY_PORT                         GPIOC
#define GSM_PWRKEY_PIN                          GPIO_PIN_10
#define GSM_PWRKEY_MODE                         GPIO_MODE_OUTPUT_PP
#define GSM_PWRKEY_PULL                         GPIO_NOPULL
#define GSM_PWRKEY_SPEED                        GPIO_SPEED_LOW

#define GSM_RESETN_PORT                         GPIOA
#define GSM_RESETN_PIN                          GPIO_PIN_10
#define GSM_RESETN_MODE                         GPIO_MODE_OUTPUT_OD
#define GSM_RESETN_PULL                         GPIO_NOPULL
#define GSM_RESETN_SPEED                        GPIO_SPEED_LOW

#define GSM_UART_DTR_PORT                       GPIOE
#define GSM_UART_DTR_PIN                        GPIO_PIN_0
#define GSM_UART_DTR_MODE                       GPIO_MODE_OUTPUT_PP
#define GSM_UART_DTR_PULL                       GPIO_NOPULL
#define GSM_UART_DTR_SPEED                      GPIO_SPEED_LOW

#define GSM_UART_RTS_PORT                       GPIOD
#define GSM_UART_RTS_PIN                        GPIO_PIN_4
#define GSM_UART_RTS_MODE                       GPIO_MODE_AF_PP
#define GSM_UART_RTS_PULL                       GPIO_NOPULL
#define GSM_UART_RTS_SPEED                      GPIO_SPEED_LOW
#define GSM_UART_RTS_ALTERNATE                  GPIO_AF7_USART2

#define GSM_UART_CTS_PORT                       GPIOD
#define GSM_UART_CTS_PIN                        GPIO_PIN_3
#define GSM_UART_CTS_MODE                       GPIO_MODE_AF_PP
#define GSM_UART_CTS_PULL                       GPIO_NOPULL
#define GSM_UART_CTS_SPEED                      GPIO_SPEED_LOW
#define GSM_UART_CTS_ALTERNATE                  GPIO_AF7_USART2

#define GSM_UART_RXD_PORT                       GPIOD
#define GSM_UART_RXD_PIN                        GPIO_PIN_6
#define GSM_UART_RXD_MODE                       GPIO_MODE_AF_PP
#define GSM_UART_RXD_PULL                       GPIO_NOPULL
#define GSM_UART_RXD_SPEED                      GPIO_SPEED_LOW
#define GSM_UART_RXD_ALTERNATE                  GPIO_AF7_USART2

#define GSM_UART_TXD_PORT                       GPIOD
#define GSM_UART_TXD_PIN                        GPIO_PIN_5
#define GSM_UART_TXD_MODE                       GPIO_MODE_AF_PP
#define GSM_UART_TXD_PULL                       GPIO_NOPULL
#define GSM_UART_TXD_SPEED                      GPIO_SPEED_LOW
#define GSM_UART_TXD_ALTERNATE                  GPIO_AF7_USART2
/**@}*//* @ingroup BSP_BSE_GSM_MODULE */

/**
 * @ingroup BSP_BSE_SENSOR_NODE
 * @{
 */
#define SENSOR_NODE_GPIO_CLOCK_ENABLE()         do {\
                                                    __HAL_RCC_GPIOD_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOE_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOF_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOG_CLK_ENABLE();\
                                                } while(0)
#define SENSOR_NODE_I2C_CLOCK_ENABLE()          __HAL_RCC_I2C1_CLK_ENABLE()
#define SENSOR_NODE_I2C_CLOCK_DISABLE()         __HAL_RCC_I2C1_CLK_DISABLE()

#define SENSOR_NODE_I2C_EVENT_PRIO              UINT32_C(8)
/**< Interrupt Priority for I2C events */
#define SENSOR_NODE_I2C_ERROR_PRIO              UINT32_C(7)
/**< Interrupt Priority for I2C errors */
#define EXTI_LINE1510_INTERRUPT_PRIO            UINT32_C(6)
/**< Interrupt Priority for Lines 10 to 15 shared by gyroscope accelerometer and
 *  geomagnetic sensors */
#define EXTI_LINE3_INTERRUPT_PRIO               UINT32_C(7)
/**< Interrupt priority for line 3 used by geomagnetic sensor*/
#define BMI160_INT1_PORT                        GPIOD
#define BMI160_INT1_PIN                         GPIO_PIN_10
#define BMI160_INT1_MODE                        GPIO_MODE_IT_FALLING
#define BMI160_INT1_MODE_STOP                   GPIO_MODE_EVT_RISING
/**< @warning: HAL_GPIO_Init() function in the stm32cube library configures the
 *  external interrupt line and overwrites the previous settings in the
 *  SYSCFG_EXTICRx. be careful when working with event or interrupt modes*/
#define BMI160_INT1_PULL                        GPIO_PULLUP
#define BMI160_INT1_SPEED                       GPIO_SPEED_LOW

/*---Geomagnetic Sensor-------------------------------------------------------*/
#define BMM_DRDY_PORT                           GPIOF
#define BMM_DRDY_PIN                            GPIO_PIN_15
#define BMM_DRDY_MODE                           GPIO_MODE_IT_RISING
/**< @warning: HAL_GPIO_Init() function in the stm32cube library configures the
 *  external interrupt line and overwrites the previous settings in the
 *  SYSCFG_EXTICRx. be careful when working with event or interrupt modes*/
#define BMM_DRDY_PULL                           GPIO_PULLDOWN
#define BMM_DRDY_SPEED                          GPIO_SPEED_LOW

#define BMM_INT_PORT                            GPIOF
#define BMM_INT_PIN                             GPIO_PIN_3
#define BMM_INT_MODE                            GPIO_MODE_IT_RISING
/**< @warning: HAL_GPIO_Init() function in the stm32cube library configures the
 *  external interrupt line and overwrites the previous settings in the
 *  SYSCFG_EXTICRx. be careful when working with event or interrupt modes*/
#define BMM_INT_PULL                            GPIO_PULLDOWN
#define BMM_INT_SPEED                           GPIO_SPEED_LOW

/* Gyroscope Sensor */
#define BMI160_EN_PORT                          GPIOE
#define BMI160_EN_PIN                           GPIO_PIN_12
#define BMI160_EN_MODE                          GPIO_MODE_OUTPUT_OD
#define BMI160_EN_PULL                          GPIO_NOPULL
#define BMI160_EN_SPEED                         GPIO_SPEED_LOW

#define BMI160_INT2_PORT                        GPIOF
#define BMI160_INT2_PIN                         GPIO_PIN_14
#define BMI160_INT2_MODE                        GPIO_MODE_IT_FALLING
/**< @warning: HAL_GPIO_Init() function in the stm32cube library configures the
 *  external interrupt line and overwrites the previous settings in the
 *  SYSCFG_EXTICRx. be careful when working with event or interrupt modes*/
#define BMI160_INT2_PULL                        GPIO_PULLUP
#define BMI160_INT2_SPEED                       GPIO_SPEED_LOW

/*Sensors I2C Bus */
#define I2C1_DIGITAL_FILTER_ENABLE              I2C_ANALOGFILTER_ENABLE
#define I2C1_TIMING                             UINT32_C(0x2010091A)

#define I2C_CLK_PORT                            GPIOG
#define I2C_CLK_PIN                             GPIO_PIN_14
#define I2C_CLK_MODE                            GPIO_MODE_AF_OD
#define I2C_CLK_PULL                            GPIO_NOPULL
#define I2C_CLK_SPEED                           GPIO_SPEED_HIGH
#define I2C_CLK_ALTERNATE                       GPIO_AF4_I2C1

#define I2C_DATA_PORT                           GPIOG
#define I2C_DATA_PIN                            GPIO_PIN_13
#define I2C_DATA_MODE                           GPIO_MODE_AF_OD
#define I2C_DATA_PULL                           GPIO_NOPULL
#define I2C_DATA_SPEED                          GPIO_SPEED_HIGH
#define I2C_DATA_ALTERNATE                      GPIO_AF4_I2C1
/**@}*//* @ingroup BSP_BSE_SENSOR_NODE */

/**
 * @ingroup BSP_BSE_BUTTON
 * @{
 */
/*---Buttons Configuration----------------------------------------------------*/
/* GPIO ports clock enable commands necessary for Button Module*/
#define BUTTON_GPIO_CLOCK_ENABLE()              do {\
                                                    __HAL_RCC_GPIOB_CLK_ENABLE();\
                                                } while(0)

#define BUTTON_DEBOUNCE_TIME_MS                 UINT32_C(200)
#define BUTTON_LONG_PRESS_MS                    (UINT32_C(3000) - BUTTON_DEBOUNCE_TIME_MS)

#define BUTTON_TIMER                            TIM3

#define BUTTON_TIMER_PRESCALER                  UINT32_C(47999)
#define BUTTON_TIMER_PERIOD                     BUTTON_LONG_PRESS_MS
#define BUTTON_TIMER_COUNTER_MODE               TIM_COUNTERMODE_UP;
#define BUTTON_TIMER_CLK_DIV                    TIM_CLOCKDIVISION_DIV1;
/*---Interrupt priority for the Buttons ---------------------------*/
#define BUTTON_EXTI_IRQN                        EXTI1_IRQn
#define BUTTON_EXTI_PREEMPT_PRIORITY            UINT32_C(6)
#define BUTTON_EXTI_SUBPRIORITY                 UINT32_C(0)

#define BUTTON_TIMER_IRQN                       TIM3_IRQn
#define BUTTON_TIMER_PREEMPT_PRIORITY           UINT32_C(7)
#define BUTTON_TIMER_SUBPRIORITY                UINT32_C(0)

#define SYS_RESET_BUTTON_PORT                   GPIOB
#define SYS_RESET_BUTTON_PIN                    GPIO_PIN_4
#define SYS_RESET_BUTTON_MODE                   GPIO_MODE_INPUT
#define SYS_RESET_BUTTON_PULL                   GPIO_NOPULL
#define SYS_RESET_BUTTON_SPEED                  GPIO_SPEED_LOW

#define SYS_ECALL_BUTTON_PORT                   GPIOB
#define SYS_ECALL_BUTTON_PIN                    GPIO_PIN_1
#define SYS_ECALL_BUTTON_MODE                   GPIO_MODE_IT_RISING_FALLING
#define SYS_ECALL_BUTTON_PULL                   GPIO_PULLUP
#define SYS_ECALL_BUTTON_SPEED                  GPIO_SPEED_LOW
/**@}*//* @ingroup BSP_BSE_BUTTON */

/**
 * @ingroup BSP_BSE_PWM_BUZZER
 * @{
 */
/*
 * PWM
 */
#define TIM1_DEFAULT_PRESCALER                  UINT32_C(4)
#define PWM_DEFAULT_PERIOD                      UINT32_C(1000)
#define PWM_DEFAULT_FREQUENCY                   UINT32_C(20000)  /* 20 kHz */
#define PWM_FREQUENCY_MAX                       UINT32_C(22000)
#define PWM_FREQUENCY_MIN                       UINT32_C(0001)

/* Buzzer */
#define BUZZER_PWM_DEFAULT_VOLUME               UINT32_C(333)
#define BUZZER_PWM_TIMER                        TIM1
#define BUZZER_PWM_TIMER_CHANNEL                TIM_CHANNEL_4

#define BUZZER_PWM_CHANNEL_SETCOMPARE           LL_TIM_OC_SetCompareCH4
#define BUZZER_PWM_CHANNEL_GETCOMPARE           LL_TIM_OC_GetCompareCH4

#define BUZZER_PWM_PORT                         GPIOE
#define BUZZER_PWM_PIN                          GPIO_PIN_14
#define BUZZER_PWM_MODE                         GPIO_MODE_AF_PP
#define BUZZER_PWM_PULL                         GPIO_NOPULL
#define BUZZER_PWM_SPEED                        GPIO_SPEED_LOW
#define BUZZER_PWM_ALTERNATE                    GPIO_AF1_TIM1

#define VBUZZER_EN_PORT                         GPIOD
#define VBUZZER_EN_PIN                          GPIO_PIN_13
#define VBUZZER_EN_MODE                         GPIO_MODE_OUTPUT_PP
#define VBUZZER_EN_PULL                         GPIO_NOPULL
#define VBUZZER_EN_SPEED                        GPIO_SPEED_LOW
/**@}*//* @ingroup BSP_BSE_PWM_BUZZER */

/**
 * @ingroup BSP_BSE_TEST_IF
 * @{
 */
/*
 * Test Console
 */
/*---Serial interface parameter for the test interface---------------------------*/
#define TEST_UART_CLOCK_DISABLE()               __HAL_RCC_USART3_CLK_DISABLE()
#define TEST_UART_CLOCK_ENABLE()                __HAL_RCC_USART3_CLK_ENABLE()
#define TEST_GPIO_CLOCK_ENABLE()                __HAL_RCC_GPIOD_CLK_ENABLE()

#define TEST_SERIAL_PORT                        USART3
#define TEST_BAUD_RATE                          UINT32_C(115200)
#define TEST_DATA_BITS                          UART_WORDLENGTH_8B
#define TEST_STOP_BITS                          UART_STOPBITS_1
#define TEST_MODE                               UART_MODE_TX_RX
#define TEST_PARITY                             UART_PARITY_NONE
#define TEST_FLOW_CONTROL                       UART_HWCONTROL_NONE
#define TEST_SAMPLING                           UART_OVERSAMPLING_16
#define TEST_ONE_BIT_SAMPLING                   UART_ONEBIT_SAMPLING_DISABLED
#define TEST_ADVANCED_INT                       UART_ADVFEATURE_NO_INIT

#define TEST_UART_IRQn                          USART3_IRQn
#define TEST_PREEMPT_PRIORITY                   UINT32_C(15)
#define TEST_SUBPRIORITY                        UINT32_C(0)

#define TEST_UART_CLOCK_SELECT                  RCC_PERIPHCLK_USART3
#define TEST_UART_CLOCK_SOURCE                  RCC_USART3CLKSOURCE_PCLK1

#define TEST_TRANSMIT_DISABLE_TIMEOUT           UINT32_C(1)

/* Test Console GPIO Pins */
#define TEST_RX_PORT                            GPIOD
#define TEST_RX_PIN                             GPIO_PIN_9
#define TEST_RX_MODE                            GPIO_MODE_AF_OD
#define TEST_RX_PULL                            GPIO_PULLUP
#define TEST_RX_SPEED                           GPIO_SPEED_LOW
#define TEST_RX_ALTERNATE                       GPIO_AF7_USART3

#define TEST_TX_PORT                            GPIOD
#define TEST_TX_PIN                             GPIO_PIN_8
#define TEST_TX_MODE                            GPIO_MODE_AF_PP
#define TEST_TX_PULL                            GPIO_NOPULL
#define TEST_TX_SPEED                           GPIO_SPEED_LOW
#define TEST_TX_ALTERNATE                       GPIO_AF7_USART3
/**@}*//* @ingroup BSP_BSE_TEST_IF */

/**
 * @ingroup BSP_BSE_LED_LM2755
 * @{
 */
#define LED_LM2755_GPIO_CLOCK_ENABLE()         do {\
                                                    __HAL_RCC_GPIOD_CLK_ENABLE();\
                                                    __HAL_RCC_GPIOF_CLK_ENABLE();\
                                                } while(0)
#define LED_LM2755_I2C_CLOCK_ENABLE()          __HAL_RCC_I2C2_CLK_ENABLE()
#define LED_LM2755_I2C_CLOCK_DISABLE()         __HAL_RCC_I2C2_CLK_DISABLE()

#define LED_LM2755_I2C_EVENT_PRIO              UINT32_C(8)
/**< Interrupt Priority for I2C events */
#define LED_LM2755_I2C_ERROR_PRIO              UINT32_C(7)
/**< Interrupt Priority for I2C errors */

#define LED_CHRGPUMP_EN_PORT                    GPIOD
#define LED_CHRGPUMP_EN_PIN                     GPIO_PIN_14
#define LED_CHRGPUMP_EN_MODE                    GPIO_MODE_OUTPUT_PP
#define LED_CHRGPUMP_EN_PULL                    GPIO_NOPULL
#define LED_CHRGPUMP_EN_SPEED                   GPIO_SPEED_LOW

/* I2C2 Bus */
#define I2C2_DIGITAL_FILTER_ENABLE              I2C_ANALOGFILTER_ENABLE
#define I2C2_TIMING                             UINT32_C(0x2010091A)

#define I2C2_CLK_PORT                           GPIOF
#define I2C2_CLK_PIN                            GPIO_PIN_1
#define I2C2_CLK_MODE                           GPIO_MODE_AF_OD
#define I2C2_CLK_PULL                           GPIO_NOPULL
#define I2C2_CLK_SPEED                          GPIO_SPEED_LOW
#define I2C2_CLK_ALTERNATE                      GPIO_AF4_I2C2

#define I2C2_DATA_PORT                          GPIOF
#define I2C2_DATA_PIN                           GPIO_PIN_0
#define I2C2_DATA_MODE                          GPIO_MODE_AF_OD
#define I2C2_DATA_PULL                          GPIO_NOPULL
#define I2C2_DATA_SPEED                         GPIO_SPEED_LOW
#define I2C2_DATA_ALTERNATE                     GPIO_AF4_I2C2
/**@}*//* BSP_BSE_LED_LM2755 */

/**
 * @ingroup BSP_BSE_BOARD
 * @{
 */
/*
 * Oscillators
 */
/*
 * HSE Oscillator:
 */
#define OSC_GPIO_CLOCK_ENABLE()                 do {\
                                                    __HAL_RCC_GPIOH_CLK_ENABLE();\
                                                } while(0)

#define OSC_PORT                                GPIOH
#define OSC_IN_PIN                              GPIO_PIN_0
#define OSC_OUT_PIN                             GPIO_PIN_1
#define OSC_MODE                                GPIO_MODE_ANALOG
/**@}*//* @ingroup BSP_BSE_BOARD */

#endif /* BOARDSETTINGS_H_ */
/**@}*//* @ingroup BSP_BSE_BOARD */
