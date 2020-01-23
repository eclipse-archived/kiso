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
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"

/*---------------------- EXPORTED MACROS ----------------------------------------------------------------------------*/

#define PINB_LED_R GPIO_PIN_14
#define PINB_LED_G GPIO_PIN_0
#define PINB_LED_B GPIO_PIN_7
#define PINC_USR_BUTTON GPIO_PIN_13

/*---------------------- EXPORTED TYPES ------------------------------------------------------------------------------*/
/**
 * GPIO ports available in the MCU
 */
enum GPIO_Ports_E
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_G,
    GPIO_PORT_H,
    GPIO_PORT_I,
    GPIO_PORT_J,
    GPIO_PORT_K,
    GPIO_PORT_LAST,
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
