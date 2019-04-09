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
 * @defgroup    HAL_MUTEX Mutex
 * @ingroup     BCDS_ESSENTIALS
 * @{
 *
 * @brief       Provides mutual exclusion in multitask environment.
 *
 * @details
 * This API to supports Mutual Exclusion to protect a code segments in HAL and BSPs.
 * By default, the mutex functions are empty because the mutex is only needed if the RTOS is active. In this case,
 * RTOS hooks should be set using the function HAL_Lock_SetHooks() to provide a proper mutex implementation.
 *
 * The original state of the mutex is unlocked.
 *
 * @note This mutex must not be used in the ISR context.
 */

#ifndef BCDS_HAL_MUTEX_H_
#define BCDS_HAL_MUTEX_H_

#include "BCDS_HAL.h"

#define HAL_MUTEX_UNINITIALIZED		NULL

/**
 * @brief Abstract type definition for mutex.
 * */
typedef void * HAL_Mutex_T;

/**
 * @brief       Type definition for the mutex hook function pointer.
 *
 * @details     To use the API the user should pass to HAL_Mutex_SetHooks() references to functions of this signature
 *              (**Retcode_T function(Hal_Mutex_T *mutex)**), handling the lock/unlock operations of a mutex object
 *
 * @param[in]   *mutex : reference to the mutex object to handle.
 */
typedef Retcode_T (*HAL_Mutex_Hook_T)(HAL_Mutex_T *mutex);

/**
 * @brief       Sets RTOS hooks
 *
 * @details     If an RTOS is used, then hooks should be set to provide proper mutex functions based on RTOS mechanisms
 *              such as mutexSemaphores.
 *
 * @param[in]   lockHook: the hook for the lock function
 *
 * @param[in]   unlockHook: the hook for the unlock function
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when any one of the param has been NULL
 */

Retcode_T HAL_Mutex_SetHooks(HAL_Mutex_Hook_T lockHook, HAL_Mutex_Hook_T unlockHook);

/**
 * @brief       Locks the HAL mutex
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when HAL_Mutex_SetHooks() is not initialized.
 */
Retcode_T HAL_Mutex_Lock(HAL_Mutex_T *mutex);

/**
 * @brief       Unlocks the HAL mutex
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when HAL_Mutex_SetHooks() is not initialized.
 */
Retcode_T HAL_Mutex_Unlock(HAL_Mutex_T *mutex);

#endif /* BCDS_HAL_MUTEX_H_ */
/** @} */
