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
 * @brief Defines MCU Timer handle
 *
 *
 * @details
 *
 */
#ifndef BCDS_MCU_TIMER_HANDLE_H_
#define BCDS_MCU_TIMER_HANDLE_H_

/* Include all headers which are needed by this file. */
#if defined (stm32f7)
    #include "stm32f7/BCDS_MCU_STM32F7_Timer_Handle.h"
#else
    #error "The STM32 family is not defined or not supported."
#endif
/* Put the type and macro definitions here */

/* Put the function declarations here */

#endif /* BCDS_MCU_TIMER_HANDLE_H_ */
