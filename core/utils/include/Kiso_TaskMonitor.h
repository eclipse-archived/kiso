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
 * @defgroup TASKMONITOR TaskMonitor
 * @{
 *
 * @brief
 *      Monitor the system tasks in order to detect deadlocks or significant mismatch with the expected scheduling
 *
 * @details
 *
 * @file
 */
#ifndef KISO_TASKMONITOR_H_
#define KISO_TASKMONITOR_H_

#include "Kiso_Utils.h"

#if KISO_FEATURE_TASKMONITOR

/* Interface dependency checks */
#include "Kiso_Retcode.h"


/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "task.h"


/**
 * @brief
 *      Initialize the Task monitor.
 * @details
 *      All variables are initialed to default value
 *
 * @retval #RETCODE_OK
 *      Task Monitor is initialized successfully
 * 
 */
Retcode_T TaskMonitor_Initialize(void);

/**
 * @brief
 *      Register the task for monitoring
 *
 *
 * @param[in] task
 *      Task handler for given task
 *
 * @param[in] upperLimit
 *      In Seconds.Maximum allowed time out to trigger the task execution from freertos kernel.
 *
 * @retval #RETCODE_OK
 *      Task is registered successfully
 * @retval #RETCODE_TASKMONITOR_BUFFER_FULL_ERROR
 *      Task is not registered.
 * @retval #RETCODE_INVALID_PARAM
 *      Task handler is NULL or upperLimit is zero
 * 
 */

Retcode_T TaskMonitor_Register(TaskHandle_t task, uint32_t upperLimit);

/**
 * @brief
 *      Update the time stamp while exiting from task execution. 
 *      This function is mapped with traceTASK_SWITCHED_OUT() to call from Freertos kernel 
 *
 * @note
 *      This function is called at every task exit. Hence execution time should be very minimal.
 *		Usage #define traceTASK_SWITCHED_OUT()	TaskMonitor_Update(&pxCurrentTCB->pxTaskTag, xTickCount)
 *
 *		Data types is added as �unsigned int� to avoid compilation warning in FreeRTOSConfig.h file
 *
 * @param[out] taskTag
 * 	    Current Tasks's task tag pointer to update the tickcount
 *
 * @param[in] tickCount 
 * 	    Count of ticks since vTaskStartScheduler was called
 *
 */
void TaskMonitor_Update (unsigned int * taskTag, unsigned int tickCount);

/**
 * @brief
 *      Check the all registered task execution time with the corresponding upper-limit
 *
 * @note
 *      This function should be called from free rtos idle hook.
 *
 * @retval True
 *      Anyone of the task execution fail
 * @retval False
 *      All task are executed successfully
 * 
 * @details
 *      Usage
 * 
 * @code{.c}
 *      void vApplicationIdleHook(void)
 *	    {
 *		    bool rc;
 *
 *		    rc = TaskMonitor_Check();
 *		    if(rc)
 *		    {
 *			    //Handle Error
 *		    }
 *	    }
 * @endcode
 *
 */
bool TaskMonitor_Check(void);

#endif /* if KISO_FEATURE_TASKMONITOR */

#endif /* KISO_TASKMONITOR_H_ */
/**@} */
