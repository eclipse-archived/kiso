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
 * @defgroup CMDPROCESSOR CmdProcessor
 * @{
 *
 * @brief
 *      Handles the execution of enqueued commands
 *
 * @details
 *      Command processor module has a task, to executes the next
 *      function from the queue. This function is enqueued by the application
 *      using the command enqueue function
 *
 * @code
 *
 *      // function to load in queue
 *      void AppFunc(void * , uint32_t param)
 *      {
 *          // do some functionality
 *      }
 *
 *      int main(void)
 *      {
 *          // Intialize the command processor
 *          CmdProcessor_Initialize(&cmdprocessor,"<taskname>",priority, stack_size,queue_size);
 *          //create a timer task
 *          OS_timerCreate(timer, 10,pdTRUE ,NULL, vTimerCallback);
 *      }
 *
 *      // Timer call back function to load application function in queue
 *      void vTimerCallback( OS_timerHandle_tp pxTimer )
 *      {
 *          CmdProcessor_enqueue(&CmdProcessor,&AppFunc, NULL, NULL);
 *      }
 *
 * @endcode
 *
 * @file
 **/

/* header definition ******************************************************** */
#ifndef KISO_CMDPROCESSOR_H_
#define KISO_CMDPROCESSOR_H_

#include "Kiso_Utils.h"

#if KISO_FEATURE_CMDPROCESSOR

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/* public type and macro definitions */
#define CMDPROCESSOR_MAX_NAME_LEN        UINT32_C(32)   /**< Maximum length of command processor task name including 0 termination char */

/**
 * @brief
 *      This data type represents a function pointer for the functions which can be handled by
 *      the command processor.
 *
 * @note
 *      The return value of the command processor function is void. Exceptions must be handled within the function itself.
 *
 * @param [in]  *param1
 *      A generic pointer to any context data structure which will be passed to the function when it is invoked by the command processor.
 * @param [in]   param2
 *      A generic 32 bit value  which will be passed to the function when it is invoked by the command processor..
 * 
 */
typedef void (*CmdProcessor_Func_T)(void *param1, uint32_t param2);

typedef void * cmdProcessorTaskHandle_t;

typedef void * cmdProcessorQueueHandle_t;


/* Structure contains task handle and queue handle as it members to access the queue and task easily.*/
struct _CmdProcessor_S
{
	cmdProcessorTaskHandle_t task;
    cmdProcessorQueueHandle_t queue;
    int8_t name[CMDPROCESSOR_MAX_NAME_LEN];
};

/**
 * The Command Processor handle data type. The structure  itself is hidden in the
 * source file.
 */
typedef struct _CmdProcessor_S CmdProcessor_T;

/** enums to represent the return status of a command processor */
enum CmdProcessor_Retcode_E
{
    CMDPROCESSOR_RETCODE_QUEUE_FULL,
    CMDPROCESSOR_RETCODE_QUEUE_ERROR,
};

/**
 * @brief
 *      This function initializes the command processor instance. It creates a queue which
 *      will carry the command functions and a task which
 *      will process the queue.
 *
 * @details
 * 
 * @code{.c}
 *      CmdProcessor_T *cmdProcessor = CmdProcessor_getSingleton();
 *      CmdProcessor_Initialize(&cmdProcessor, "AppCmdProcessor", ...);
 *@endcode
 *
 * @note
 *      Please take care while assigning the priority to command processor task, it should be less than scheduler timer task priority.
 *      Command processor does not support the delegation from ISR because it falls under the application tasks and application task should be
 *      operated at least priority compare to  ISR and RTOS(timer service).
 *
 * @param[in]   cmdProcessor
 *      Contains the queue and task handles
 * @param[in]   name
 *      Represents the task name
 * @param[in]   taskPriority
 *      Represents the task priority
 * @param[in]   taskStackDepth
 *      Represents the stack Size for the task
 * @param[in]   queueSize
 *      Represents the queue Size
 *
 * @retval      #RETCODE_OK
 *      When the queue is created successfully
 * @retval      #RETCODE_NULL_POINTER
 *      When the task or queue is not created
 * 
 */
Retcode_T CmdProcessor_Initialize(CmdProcessor_T *cmdProcessor, const char* name, uint32_t taskPriority, uint32_t taskStackDepth, uint32_t queueSize);

/**
 *  @brief
 *      This routine is used to hand-over a function to the command processor for execution. The function is added
 *      to the queue which is processed in a FIFO manner.
 *
 *  @warning
 *      This routine should be called after cmdprocessor_intitialize function.
 *      Hand-over Function prototype should be same as cmdprocess_fun_t
 *
 *  @param[in]  cmdProcessor
 *      Contains the queue handle
 *  @param[in]  func
 *      Represents the function
 *  @param[in]  param1
 *      A generic pointer to an arbitrary context data structure which will be passed to the function when it is invoked by the command processor.
 *  @param[in]  param2
 *      Second argument of the function
 *
 *  @retval     #RETCODE_OK
 *      When the function is pushed successfully into the queue
 *  @retval     #RETCODE_INVALID_PARAM
 *      When cmdProcessor or function pointers is NULL
 *  @retval     #RETCODE_FAILURE
 *      When the queue is full
 * 
 */
Retcode_T CmdProcessor_Enqueue(CmdProcessor_T *cmdProcessor, CmdProcessor_Func_T func, void *param1, uint32_t param2);

/**
 *  @brief
 *      This routine is used to hand-over a function to the command processor from ISR context. The function is added
 *      to the queue which is processed in a FIFO manner.
 *
 *  @warning
 *      This routine should be called after cmdprocessor_intitialize function.
 *      Hand-over Function prototype should be same as cmdprocess_fun_t
 *
 *  @param[in]  cmdProcessor
 *      Contains the queue handle
 *  @param[in]  func
 *      Represents the function
 *  @param[in]  param1
 *      A generic pointer to an arbitrary context data structure which will be passed to the function when it is invoked by the command processor.
 *  @param[in]  param2
 *      Second argument of the function
 *
 *  @retval     #RETCODE_OK
 *      When the function is pushed successfully into the queue
 *  @retval     #RETCODE_INVALID_PARAM
 *      When cmdProcessor or function pointers is NULL
 *  @retval     #RETCODE_FAILURE
 *      When the queue is full
 * 
 */
Retcode_T CmdProcessor_EnqueueFromIsr(CmdProcessor_T *cmdProcessor, CmdProcessor_Func_T func, void *param1, uint32_t param2);


/**
 * @brief
 *      The API suspends the command processor service.
 *      It is used to stop the execution of the command processor task functionality.
 *
 * @param[in]   cmdProcessor
 *      Holds the command processor task handle
 *
 */
void CmdProcessor_Suspend(CmdProcessor_T *cmdProcessor);

/**
 * @brief       The API resumes the command processor service.
 *              It is used to execute the suspended command processor task functionality.
 *
 * @param[in]   cmdProcessor    - Holds the command processor task handle
 *
 */
void CmdProcessor_Resume(CmdProcessor_T *cmdProcessor);

#endif /* if KISO_FEATURE_CMDPROCESSOR */

#endif /* KISO_CMDPROCESSOR_H_ */
/**@} */
