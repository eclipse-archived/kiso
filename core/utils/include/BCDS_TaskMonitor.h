/*----------------------------------------------------------------------------*/
/*
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 */
/*----------------------------------------------------------------------------*/

/**
 * @ingroup UTILS
 *
 * @defgroup TASKMONITOR TaskMonitor
 * @{
 *
 * @brief Monitor the system tasks in order to detect deadlocks or significant mismatch with the expected scheduling
 *
 * @details
 *
 * @file
 */
#ifndef BCDS_TASKMONITOR_H_
#define BCDS_TASKMONITOR_H_

#include "BCDS_Utils.h"

#if BCDS_FEATURE_TASKMONITOR

/* interface dependency checks */
#include "BCDS_Retcode.h"
#include "FreeRTOS.h"
#include "task.h"


/**
 * @brief   Initialize the Task monitor.
 * @details All variables are initialed to default value
 *
 * @retval #RETCODE_OK : Task Monitor is initialized successfully
 */
Retcode_T TaskMonitor_Initialize(void);

/**
 * @brief   Register the task for monitoring
 *
 *
 * @param[in] task
 * 	Task handler for given task
 *
 * @param[in] upperLimit in Seconds
 * Maximum allowed time out to trigger the task execution from freertos kernel.
 *
 * @retval #RETCODE_OK : Task is registered successfully
 * @retval #RETCODE_TASKMONITOR_BUFFER_FULL_ERROR : Task is not registered.
 * @retval #RETCODE_INVALID_PARAM : task handler is NULL or upperLimit is zero
 */

Retcode_T TaskMonitor_Register(TaskHandle_t task, uint32_t upperLimit);

/**
 * @brief   Update the time stamp while exiting from task execution. 
 * This function is mapped with traceTASK_SWITCHED_OUT() to call from Freertos kernel 
 *
 * @note    This function is called at every task exit. Hence execution time should be very minimal.
 *			Usage #define traceTASK_SWITCHED_OUT()	TaskMonitor_Update(&pxCurrentTCB->pxTaskTag, xTickCount)
 *
 *			Data types is added as �unsigned int� to avoid compilation warning in FreeRTOSConfig.h file
 *
 * @param[out] taskTag
 * 	Current Tasks's task tag pointer to update the tickcount
 *
 * @param[in] tickCount 
 * 	count of ticks since vTaskStartScheduler was called
 *
 */
void TaskMonitor_Update (unsigned int * taskTag, unsigned int tickCount);

/**
 * @brief   Check the all registered task execution time with the corresponding upper-limit
 *
 * @note    This function should be called from free rtos idle hook.
 *
 * @retval True : Anyone of the task execution fail
 * @retval False : All task are executed successfully
 * 
 * @details usage
 * @code{.c}
 * void vApplicationIdleHook(void)
 *	{
 *		bool rc;
 *
 *		rc = TaskMonitor_Check();
 *		if(rc)
 *		{
 *			//Handle Error
 *		}
 *	}
 *	@endcode
 *
 */
bool TaskMonitor_Check(void);

#endif /* if BCDS_FEATURE_TASKMONITOR */

#endif /* BCDS_TASKMONITOR_H_ */
/**@} */
