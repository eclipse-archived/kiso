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
 * @brief Add a brief description here.
 *
 * @details Put here the documentation of this header file. Explain the interface exposed
 * by this header, e.g. what is the purpose of use, how to use it, etc.
 */

#ifndef CHARGER_BQ2407X_H_
#define CHARGER_BQ2407X_H_

#include "BCDS_BSP_CHARGER_BQ2407X.h"

#if BCDS_FEATURE_BSP_CHARGER_BQ2407X

/* Include all headers which are needed by this file. */
enum BSP_ModuleState_E Charger_bq2407x_getState(void);

/* Put the type and macro definitions here */

/* Put the function declarations here */

/**
 * @brief EXTI9_5 interrupt service routine, it is used to detect the error state
 * of the charger which is a ~2Hz square signal
 *
 */
void Charging_Stat_EXTI7ISR(void);

/**
 * @brief Timer interrupt service routine
 *
 */
void ChargerTIM_ISR(void);

#endif /* BCDS_FEATURE_BSP_CHARGER_BQ2407X */

#endif /* CHARGER_BQ2407X_H_ */

