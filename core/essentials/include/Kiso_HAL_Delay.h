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
 * @defgroup    HAL_DELAY Delay
 * @ingroup     KISO_HAL
 * @{
 *
 * @brief       Provide functions for different delay times.
 *
 * @details
 * By default the delay functions is implemented with a active blocking delay.
 * Using the function HAL_Delay_SetMsHook(), an RTOS Delay for millisecond
 * implementation can be hooked in if needed to replace the HAL implementation by a non blocking implementation.
 */

#ifndef KISO_HAL_DELAY_H_
#define KISO_HAL_DELAY_H_

#include "Kiso_HAL.h"

/**
 * @brief       Function data type for to handle different OS Functions by setting a new Function
 */
typedef Retcode_T (*HAL_Delay_MsHook_T)(uint32_t timeInMs);

/**
 * @brief       Set delay ms Mode for use in NoOS or RTOS Context
 *
 * @param[in]   hook : Pointer to new Delay ms Function
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when any one of the param has been NULL
 */
Retcode_T HAL_Delay_SetMsHook(HAL_Delay_MsHook_T hook);

/**
 * @brief       Delay/wait in us
 *
 * @details     This is just a wrapper for the BSP_Board_DelayUs() function.
 *
 * @param[in]   timeInUs : value in us to active wait (in noOS and in RTOS Mode )
 *
 * @retval      RETCODE_OK when successful
 */
Retcode_T HAL_Delay_WaitUs(uint32_t timeInUs);

/**
 * @brief       Delay/wait in ms
 *
 * @details     By default this function is wrapper for the BSP_Board_Delay() function. It is implemented as a busy
 *              wait. Using the function HAL_Delay_SetMsHook(), an RTOS delay function can be hooked-in if needed to
 *              replace the busy wait with a proper RTOS delay mechanism.
 *
 * @param[in]   timeInMs : value in ms to wait (active wait in noOS and task delay in RTOS Mode )
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when HAL_Delay_SetMsHook() is not initialized.
 */
Retcode_T HAL_Delay_WaitMs(uint32_t timeInMs);

#endif /* KISO_HAL_DELAY_H_ */
/** @} */
