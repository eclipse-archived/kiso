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
 * @defgroup GUARDEDTASK GuardedTask
 * @{
 *
 * @brief Guarded Task interface
 *
 * @details
 *
 * @file
 **/

#ifndef BCDS_GUARDED_TASK_H_
#define BCDS_GUARDED_TASK_H_

/* public interface declaration */
#include "BCDS_Utils.h"

#if BCDS_FEATURE_GUARDEDTASK

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

/* public type and macro definitions */
typedef void (*GuardedTask_Function_T)(void);

typedef void * GuardedTask_TaskHandle_T;

typedef void * GuardedTask_SemaphoreHandle_T;

struct GuardedTask_S
{
    GuardedTask_TaskHandle_T task;
    GuardedTask_SemaphoreHandle_T signal;
    GuardedTask_Function_T runFunction;
};

typedef struct GuardedTask_S GuardedTask_T;

/* public function prototype declarations */

/** @brief   This function initializes a given GuardedTask handle.
 *
 * @param[in] handle            -   A pointer to an empty GuardedTask structure
 * @param[in] taskRunFunction   -   A function pointer to the user-code run function that should be called by the task
 * @param[in] taskName          -   A cstring representing the name of the task
 * @param[in] taskPriority      -   The task priority
 * @param[in] taskStackSize     -   The task stack size
 *
 * @retval  #RETCODE_OK                  -   Guarded Task has been initialized successfully
 * @retval  #RETCODE_INVALID_PARAM       -   One of the parameters was invalid
 * @retval  #RETCODE_OUT_OF_RESOURCES    -   Unable to allocate resources for task or semaphore
 */
Retcode_T GuardedTask_Initialize(GuardedTask_T* handle, GuardedTask_Function_T taskRunFunction, const char* taskName, uint32_t taskPriority, uint32_t taskStackSize);

/**  @brief  This function deinitializes a given GuardTask handle and sets all attributes to NULL.
 *
 *  @param[in] handle   -   A pointer to an initialized GuardTask structure.
 *
 *  @retval #RETCODE_OK              -   Guarded Task has been deinitialized successfully
 *  @retval #RETCODE_INVALID_PARAM   -   handle pointer is NULL
 */
Retcode_T GuardedTask_Deinitialize(GuardedTask_T* handle);

/**  @brief  This function signals the GuardTask-task to execute its user run function
 *
 *  @param[in] handle   -   A pointer to an initialized GuardTask structure
 *
 *  @note This function may ONLY be called from NON-ISR context!
 *
 *  @retval #RETCODE_OK                                  -   Task has been signaled properly
 *  @retval #RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN -   Indicates that the semaphore was already in a given state.
 *                                                           This implicitly means that the encapsulated task is still in execution
 *  @retval #RETCODE_INVALID_PARAM                       -   Given parameter was invalid
 */
Retcode_T GuardedTask_Signal(GuardedTask_T* handle);

/**  @brief  This function signals the GuardTask-task to execute its user run function
 *
 *  @param[in] handle   -   A pointer to an initialized GuardTask structure
 *
 *  @note This function may ONLY be called from ISR context!
 *
 *
 *  @retval #RETCODE_OK                                  -   Task has been signaled properly
 *  @retval #RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN -   Indicates that the semaphore was already in a given state.
 *                                                          This implicitly means that the encapsulated task is still in execution
 *  @retval #RETCODE_INVALID_PARAM                       -   Given parameter was invalid
 */
Retcode_T GuardedTask_SignalFromIsr(GuardedTask_T* handle);

#endif /* if BCDS_FEATURE_GUARDEDTASK */

#endif /* BCDS_GUARDED_TASK_H_ */
/**@} */
