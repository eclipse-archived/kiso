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
 * @brief
 *      Guarded Task creates a task which waits for a given signal before executing an attached function once.
 *
 * @details
 *      Guarded task is wrapper of function, which will be executed only
 *      when it is needed, or 'signaled'. When/if guarded function ends,
 *      it can be executed, or signaled, again.
 *      This API provides easy mechanism to differ event handling and
 *      execute them in user context (for example, differing interrupt processing).
 *
 * @note
 *      1. When actually function will be executed depends on OS scheduler.
 *      2. If GuardedTask_Deinitialize() is called immediately after
 *             GuardedTask_Signal(), there are chances that function will
 *             not be executed because OS scheduler still didn't decide to
 *             run it.
 *      3. If function is suspended by OS scheduler in order to execute another
 *             OS task and GuardedTask_Deinitialize() is called within it,
 *             execution of function will not be resumed. 
 *      Responsibility to handle such situations is transfered to user code.
 *
 * @code{.c}
 * #include "Kiso_GuardedTask.h"
 *
 * // Guarded task function prototype
 * void GuardedTask_Function(void);
 *
 * // Guarded task function implementation
 * void GuardedTask_Function(void)
 * {
 *     // Do something useful
 * }
 *
 * int main(void)
 * {
 *     GuardedTask_T taskHandle = {0};
 *
 *     // Initialize the corresponding guarded task
 *     Retcode_T retcode = GuardedTask_Initialize(&taskHandle, GuardedTask_Function, "OurLovelyTask", 4, 100);
 *
 *     if(RETCODE_OK == retcode)
 *     {
 *         // Order execution of task function.
 *         retcode = GuardedTask_Signal(&taskHandle);
 *         // Or, if called from within ISR handler:
 *         // retcode = GuardedTask_SignalFromIsr(&taskHandle);
 *     }
 *
 *     if(RETCODE_OK == retcode)
 *     {
 *         retcode = GuardedTask_Deinitialize(&taskHandle);
 *     }
 * }
 * @endcode
 *
 * @file
 **/

#ifndef KISO_GUARDED_TASK_H_
#define KISO_GUARDED_TASK_H_

/* public interface declaration */
#include "Kiso_Utils.h"

#if KISO_FEATURE_GUARDEDTASK

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/* public type and macro definitions */
typedef void (*GuardedTask_Function_T)(void);

typedef void *GuardedTask_TaskHandle_T;

typedef void *GuardedTask_SemaphoreHandle_T;

struct GuardedTask_S
{
    GuardedTask_TaskHandle_T task;
    GuardedTask_SemaphoreHandle_T signal;
    GuardedTask_Function_T runFunction;
};

typedef struct GuardedTask_S GuardedTask_T;

/* public function prototype declarations */

/** @brief
 *      This function initializes a given GuardedTask handle.
 *
 * @param[in] handle
 *      A pointer to an empty GuardedTask structure
 * @param[in] taskRunFunction
 *      A function pointer to the user-code run function that should be called by the task
 * @param[in] taskName
 *      A cstring representing the name of the task
 * @param[in] taskPriority
 *      The task priority
 * @param[in] taskStackSize
 *      The task stack size
 *
 * @retval  #RETCODE_OK
 *      Guarded Task has been initialized successfully
 * @retval  #RETCODE_INVALID_PARAM
 *      One of the parameters was invalid
 * @retval  #RETCODE_OUT_OF_RESOURCES
 *      Unable to allocate resources for task or semaphore
 * 
 */
Retcode_T GuardedTask_Initialize(GuardedTask_T *handle, GuardedTask_Function_T taskRunFunction, const char *taskName, uint32_t taskPriority, uint32_t taskStackSize);

/**  @brief
 *      This function deinitializes a given GuardTask handle and sets all attributes to NULL.
 *
 *  @param[in] handle
 *      A pointer to an initialized GuardTask structure.
 *
 *  @retval #RETCODE_OK
 *      Guarded Task has been deinitialized successfully
 *  @retval #RETCODE_INVALID_PARAM
 *      handle pointer is NULL
 * 
 */
Retcode_T GuardedTask_Deinitialize(GuardedTask_T *handle);

/**  @brief
 *      This function signals the GuardTask-task to execute its user run function
 *
 *  @param[in] handle
 *      A pointer to an initialized GuardTask structure
 *
 *  @note
 *      This function may ONLY be called from NON-ISR context!
 *
 *  @retval #RETCODE_OK
 *      Task has been signaled properly
 *  @retval #RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN
 *      Indicates that the semaphore was already in a given state.
 *      This implicitly means that the encapsulated task is still in execution
 *  @retval #RETCODE_INVALID_PARAM
 *      Given parameter was invalid
 * 
 */
Retcode_T GuardedTask_Signal(GuardedTask_T *handle);

/**  @brief
 *      This function signals the GuardTask-task to execute its user run function
 *
 *  @param[in] handle
 *      A pointer to an initialized GuardTask structure
 *
 *  @note
 *      This function may ONLY be called from ISR context!
 *
 *
 *  @retval #RETCODE_OK
 *      Task has been signaled properly
 *  @retval #RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN
 *      Indicates that the semaphore was already in a given state.
 *      This implicitly means that the encapsulated task is still in execution
 *  @retval #RETCODE_INVALID_PARAM
 *      Given parameter was invalid
 * 
 */
Retcode_T GuardedTask_SignalFromIsr(GuardedTask_T *handle);

#endif /* if KISO_FEATURE_GUARDEDTASK */

#endif /* KISO_GUARDED_TASK_H_ */
/**@} */
