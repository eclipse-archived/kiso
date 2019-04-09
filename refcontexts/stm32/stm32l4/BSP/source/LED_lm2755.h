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
 * @ingroup BSP_LED_LM2755
 * @{
 *  @file
 *  @brief Handling routines provided for Charge Pump LED Controller Module LM2755.
 */

#ifndef LED_LM2755_H_
#define LED_LM2755_H_

#include "BCDS_BSP_LED_LM2755.h"

#if BCDS_FEATURE_BSP_LED_LM2755

/**
 * @brief I2C interrupt service routine used to capture events at the I2C
 * resource and to forward them to the MCU driver for processing.
 */
void LED_LM2755_I2CEvISR(void);
/**
 * @brief I2C interrupt service routine used to capture errors at the I2C
 * resource and to forward them to the MCU driver for processing.
 */
void LED_LM2755_I2CErISR(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E LED_LM2755_getState(void);

#endif /* BCDS_FEATURE_BSP_LED_LM2755 */
#endif /* LED_LM2755_H_ */
/**@}*//* @ingroup BSP_LED_LM2755 */
