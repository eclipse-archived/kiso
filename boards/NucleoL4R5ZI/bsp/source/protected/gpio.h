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
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief Provides pin mapping and internally exposed GPIO handling functions
 *
 * @details 
 */
#ifndef PROTECTED_GPIO_H_
#define PROTECTED_GPIO_H_

#include "Kiso_HAL.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"

/*---------------------- EXPORTED MACROS ----------------------------------------------------------------------------*/

/* ************************************************************************** */
/* LED SETTINGS ************************************************************* */
/* ************************************************************************** */
#define PINB_LED_R                      GPIO_PIN_14
#define PINB_LED_B                      GPIO_PIN_7
#define PINC_LED_G                      GPIO_PIN_7

#define PORTB_PIN_LED_ALL               (PINB_LED_R | PINB_LED_B)
#define PORTC_PIN_LED_ALL               (PINC_LED_G)

/* ************************************************************************** */
/* LPUART SETTINGS ************************************************************/
/* ************************************************************************** */
#define PIND_USART3_TX                  GPIO_PIN_8
#define PIND_USART3_RX                  GPIO_PIN_9


/* ************************************************************************** */
/* I2C SETTINGS ************************************************************* */
/* ************************************************************************** */
#define PINB_SENS_SCL                   GPIO_PIN_8
#define PINB_SENS_SDA                   GPIO_PIN_9


/* ************************************************************************** */
/* SPI SETTINGS ***************************************************************/
/* ************************************************************************** */

#define SPIx                             SPI1

/* Definition for SPIx clock resources */
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_CLK_DISABLE()               __HAL_RCC_SPI1_CLK_DISABLE()
#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

/* Definition for SPIx's NVIC */
#define SPIx_IRQn                        SPI1_IRQn
#define SPIx_IRQHandler                  SPI1_IRQHandler

/* Definition for SPIx Pins */
/* SCK GPIO pin configuration*/
#define SPIx_SCK_PIN                     GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_SCK_PORT_SELECT             GPIO_PORT_A
#define SPIx_SCK_AF                      GPIO_AF5_SPI1
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()

/* MISO pin configuration*/
#define SPIx_MISO_PIN                    GPIO_PIN_6

/* MOSI pin configuration*/
#define SPIx_MOSI_PIN                    GPIO_PIN_7

/* CS pin configuration*/
#define SPIx_CS_PIN                    GPIO_PIN_14
#define SPIx_CS_GPIO_PORT              GPIOD
#define SPIx_CS_PORT_SELECT            GPIO_PORT_D
#define SPIx_CS_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()


/*---------------------- EXPORTED TYPES ------------------------------------------------------------------------------*/
/**
 * GPIO ports available in the MCU
 */
enum GPIO_Ports_E
{
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_G,
    GPIO_PORT_H
};

/*---------------------- EXPORTED FUNCTIONS -------------------------------------------------------------------------*/

/**
 * Opens GPIO clock gate and saves its state
 */
void GPIO_OpenClockGate(enum GPIO_Ports_E port, uint16_t pinNames);

/**
 * Closes GPIO clock gate and saves its state
 */
void GPIO_CloseClockGate(enum GPIO_Ports_E port, uint16_t pinNames);

/**
 * Returns true if GPIO clock gate is open and false if closed.
 */
bool GPIO_GetClockGate(enum GPIO_Ports_E port);

#endif /* PROTECTED_GPIO_H_ */
