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

/* own header files */
#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_GUARDEDTASK

#include "BCDS_GuardedTask.h"

#if BCDS_FEATURE_GUARDEDTASK

/* BCDS Basics header files */
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"

/* Additional interface header files */
#include <ICall.h>
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* variables */
static Semaphore_Handle SemHandle; /**< Guarded task Sem handle*/
static Task_Handle TaskHandle; /**< Guarded task handle*/
static GuardedTask_Function_T RunFunction; /**< Guarded task Run function */

static Semaphore_Params semParams; /**< Guarded task semaphore parameters declaration*/
static char *guardedTaskStack = NULL; /**< Guarded task stack */

/* prototypes */
static void GuardedTaskRunFunction(UArg pvParameters1, UArg pvPatameters2);

/* local functions */

static void GuardedTaskRunFunction(UArg pvParameters1,UArg pvPatameters2)
{
    BCDS_UNUSED(pvParameters1);
    BCDS_UNUSED(pvPatameters2);

    while(true)
    {
        if ((NULL != RunFunction) && (NULL != SemHandle))
        {
            /* Wait for the "run"-semaphore to be signaled. */
            if(Semaphore_pend(SemHandle, BIOS_WAIT_FOREVER))
            {
                RunFunction();
            }
        }
        else
        {
            Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t ) RETCODE_NULL_POINTER));
            return;
        }
    }
}

/* global functions */

/* The description of the function is available in header file */
Retcode_T GuardedTask_Deinitialize(GuardedTask_T* handle)
{
    BCDS_UNUSED(handle);
    Retcode_T retcode = RETCODE_OK;

    if ((NULL != TaskHandle)&&(NULL != SemHandle))
    {
        Task_delete(&TaskHandle);
        ICall_free(guardedTaskStack);
        Semaphore_delete(&SemHandle);
        RunFunction = NULL;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_NULL_POINTER);
    }

    return retcode;
}

Retcode_T GuardedTask_Initialize(GuardedTask_T* handle, GuardedTask_Function_T taskRunFunction, const char* taskName, uint32_t taskPriority, uint32_t taskStackSize)
{
    BCDS_UNUSED(handle);
    BCDS_UNUSED(taskName);
    Retcode_T retcode = RETCODE_OK;

    if ((NULL != taskRunFunction) && (UINT32_C(0) != taskStackSize))
    {
        RunFunction = taskRunFunction;

        /* Create semaphore */
        SemHandle = Semaphore_create(0, &semParams, NULL);
        if (NULL == SemHandle)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_OUT_OF_RESOURCES);
        }

        guardedTaskStack = (char *) ICall_malloc(taskStackSize);

        /* Task Construct */
        Task_Params taskParams;
        Task_Params_init(&taskParams);
        taskParams.arg0 = NULL;
        taskParams.arg1 = NULL;
        taskParams.stack = guardedTaskStack;
        taskParams.stackSize = taskStackSize;
        taskParams.priority = taskPriority;

        TaskHandle = Task_create(GuardedTaskRunFunction, &taskParams, NULL);
        if (NULL == TaskHandle)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_OUT_OF_RESOURCES);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/* The description of the function is available in header file */
Retcode_T GuardedTask_Signal(GuardedTask_T* handle)
{
    BCDS_UNUSED(handle);
    Retcode_T retcode = RETCODE_OK;

    if (NULL != SemHandle)
    {
        /* semaphore post */
        Semaphore_post(SemHandle);
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/* Deprecated function. See header file for more information */
Retcode_T GuardedTask_deinitialize(GuardedTask_T* handle)
{
	return(GuardedTask_Deinitialize(handle));
}

/* Deprecated function. See header file for more information */
Retcode_T GuardedTask_initialize(GuardedTask_T* handle, GuardedTask_Function_T taskRunFunction, const char* taskName, uint32_t taskPriority, uint32_t taskStackSize)
{
	return(GuardedTask_Initialize(handle, taskRunFunction, taskName, taskPriority, taskStackSize));
}

/* Deprecated function. See header file for more information */
Retcode_T GuardedTask_signal(GuardedTask_T* handle)
{
	return(GuardedTask_Signal(handle));
}

#endif /* if BCDS_FEATURE_GUARDEDTASK */
