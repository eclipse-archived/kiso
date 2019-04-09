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
 * @ingroup UTILS
 *
 * @defgroup SLEEPCONTROL Sleep Control
 * @{
 *
 * @brief Sleep Control
 *
 * @details
 *
 * @file
 **/

#ifndef BCDS_SLEEP_CONTROL_H
#define BCDS_SLEEP_CONTROL_H

#include "BCDS_Utils.h"

#if BCDS_FEATURE_SLEEPCONTROL

/* Uses HAL MCU Sleep Interface */
#include "BCDS_MCU_Sleep.h"

/* Uses the HAL MCU Timer Interface*/
#include "BCDS_MCU_Timer.h"

/* Dependency Check */
#if (BCDS_FEATURE_SLEEP && BCDS_FEATURE_TIMER)

/**
 * A data type for sleep hook functions
 *
 * The SleepControls allows to register a pre-sleep and a post-sleep
 * hook function. These will be invoked respectively before and after
 * entering the sleep mode
 *
 * @param[in] mode: sleep mode entered or exited
 */
typedef Retcode_T (*SleepControl_Hook_T)(uint32_t mode);

/**
 * @brief Initializes the SleepControl module
 *
 * @param[in] preSleepHook : The hook function used to set the system into a given low-power state
 * @param[in] postSleepHook : The hook function to be used in order to restore the system from sleep
 * @param[in] wakeupTimer : The timer used for timekeeping during the tickless mode
 *
 * @note The timer has to have a compare-match interrupt for channel 0.
 *
 * @retval #RETCODE_OK if initialization was successful
 * @retval #RETCODE_NULL_POINTER if wakeupTimer pointer is NULL
 * @return errors from #MCU_Timer_Initialize and #MCU_Sleep_Initialize
 */
Retcode_T SleepControl_Initialize(SleepControl_Hook_T preSleepHook, SleepControl_Hook_T postSleepHook, Timer_T wakeupTimer);

/**
 * Perform the OS tickless sleep for given amount of OS ticks
 *
 * @param[in] sleepTimeInTicks : amount of time requested to sleep
 *
 * @retval RETCODE_OK if run was successful
 * @retval RETCODE_SLEEPCONTROL_NOSLEEP
 * @return errors from #MCU_Timer_GetClockFrequency, #MCU_Timer_SetCompareValue and #MCU_Timer_Start
 *
 */
Retcode_T SleepControl_SleepTickless(uint32_t sleepTimeInTicks);

#else
#error "BCDS_FEATURE_SLEEPCONTROL depends on BCDS_FEATURE_SLEEP and BCDS_FEATURE_TIMER"
#endif /* Dependency Check end */

#endif /* if BCDS_FEATURE_SLEEPCONTROL */

#endif /* BCDS_SLEEP_CONTROL_H */
/**@} */
