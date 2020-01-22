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
 * @brief Provides microsecond time stamping based on internal timer.
 *
 * @details
 */
#ifndef PROTECTED_TIME_H_
#define PROTECTED_TIME_H_

#include "Kiso_HAL.h"

/**
 * Returns the microseconds elapsed since last system reset.
 */
uint64_t Time_GetTimeStamp(void);

/**
 * This function initializes the microseconds timer used to run the system clock.
 * LPTIM1 is used as microseconds timer and is clocked at 1MHz rate using HSI16(16MHz) prescaled
 * by 16.
 */
Retcode_T Time_StartTime(uint64_t initialValue);

/**
 * This function adds an offset to the timeUs value.,
 */
void Time_AdjustTime(uint64_t timeOffset);

#endif /* PROTECTED_TIME_H_ */
