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
 * @ingroup BSP_BUTTON
 * @{
 *  @file
 *  @brief Interrupt service routines provided for push button events.
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "BCDS_BSP_Button.h"

#if BCDS_FEATURE_BSP_BUTTON

/**
 * @brief TIM3 interrupt Service routine used to capture the TIM3 interrupt
 * which is configured to occur each time the button has been pressed for longer
 * than a specified duration to signal a long press event.
 */
void Button_TIM3ISR(void);

/**
 * @brief EXTI1 interrupt service routine, it triggers the timer or stops it
 * depending on the detected edge at the button Input.
 *
 */
void Button_EXTI1ISR(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E Button_ResetButton_getState(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E Button_ECallButton_getState(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E Button_ModuleButton_getState(void);

#endif /* BCDS_FEATURE_BSP_BUTTON */
#endif /* BUTTON_H_ */
/**@}*//* @ingroup BSP_BUTTON */
