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
 * @defgroup    KISO_C_EXAMPLE_BLINKY Blinky LED Example
 * @{
 *
 * @brief Blinky LED example
 *
 * @details Demo application demonstrates how to use the LED API from KISO.
 * @file
 **/

#ifndef BLINKY_LED_H_
#define BLINKY_LED_H_

/* Include all headers which are needed by this file. */
#include "Kiso_Basics.h"

/* Put the type and macro definitions here */

/* Put the function declarations here */
/**
 * @brief This is a template function where the user can write his custom application.
 *
 * @param[in] CmdProcessorHandle Handle of the main commandprocessor
 *
 * @param[in] param2  Currently not used will be used in future
 *
 */
void appInitSystem(void *CmdProcessorHandle, uint32_t param2);

/**
 * @brief Toggles an LED on/off and enqueues itself in the commandprocessor after delay
 *
 * @param[in] param1 Handle of the main commandprocessor
 *
 * @param[in] param2 Unused
 *
 */
void blink_led(void *param1, uint32_t param2);

#endif /* BLINKY_LED_H_ */

/**
 * @}
 */
