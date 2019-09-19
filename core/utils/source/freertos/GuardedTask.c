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
 *
 * @brief
 *      Guarded Task Interface Implementation
 *
 * @details
 *      This source file implements following features:
 *      - GuardedTask_Deinitialize()
 *      - GuardedTask_Initialize()
 *      - GuardedTask_Signal()
 *      - GuardedTask_SignalFromIsr()
 * 
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_GUARDEDTASK

/* Include Kiso_GuardedTask interface header */
#include "Kiso_GuardedTask.h"

#if KISO_FEATURE_GUARDEDTASK

/* KISO basics header files */
#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Prototypes */
static void GuardedTaskExecute(GuardedTask_T* context);

static void GuardedTaskRunFunction(void* param);

/* Local variable */

/* Global variables */

/* Local functions */
static void GuardedTaskExecute(GuardedTask_T* context)
{
    if ((NULL != context) && (NULL != context->runFunction) && (NULL != context->signal))
    {
        /* Wait for the "run"-semaphore to be signaled. */
        BaseType_t osRetcode = xSemaphoreTake(context->signal, portMAX_DELAY);
        if (pdPASS != osRetcode)
        {
            Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t) RETCODE_GUARDEDTASK_SEMAPHORE_ERROR));
            return;
        }

        context->runFunction();
    }
    else
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t) RETCODE_INVALID_PARAM));
        return;
    }
}

static void GuardedTaskRunFunction(void* param)
{
    for (;;)
    {
        GuardedTask_T* context = (GuardedTask_T*) param;

        GuardedTaskExecute(context);
    }
}

/*  The description of the function is available in Kiso_GuardedTask.h */
Retcode_T GuardedTask_Deinitialize(GuardedTask_T* handle)
{
    Retcode_T retcode = RETCODE_OK;

    if (NULL != handle)
    {
        if (NULL != handle->task)
        {
            vTaskDelete(handle->task);
            handle->task = NULL;
        }

        if (NULL != handle->signal)
        {
            vSemaphoreDelete(handle->signal);
            handle->signal = NULL;
        }

        handle->runFunction = NULL;

        retcode = RETCODE_OK;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t) RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_GuardedTask.h */
Retcode_T GuardedTask_Initialize(GuardedTask_T* handle, GuardedTask_Function_T taskRunFunction, const char* taskName, uint32_t taskPriority, uint32_t taskStackSize)
{
    Retcode_T retcode = RETCODE_OK;

    if ((NULL != handle) && (NULL != taskRunFunction) && (NULL != taskName))
    {
        handle->runFunction = taskRunFunction;

        handle->signal = xSemaphoreCreateBinary();
        if (handle->signal == NULL)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t) RETCODE_OUT_OF_RESOURCES);
        }

        if (RETCODE_OK == retcode)
        {
            if (pdPASS != xTaskCreate(GuardedTaskRunFunction, taskName, (uint16_t)taskStackSize, handle, taskPriority, &handle->task))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t) RETCODE_OUT_OF_RESOURCES);
            }
        }

        if (RETCODE_OK != retcode)
        {
            if (NULL != handle->signal)
            {
                vSemaphoreDelete(handle->signal);
                handle->signal = NULL;
            }
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t) RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_GuardedTask.h */
Retcode_T GuardedTask_Signal(GuardedTask_T* handle)
{
    Retcode_T retcode = RETCODE_OK;

    if ((NULL != handle) && (NULL != handle->signal))
    {
        if (pdPASS == xSemaphoreGive(handle->signal))
        {
            retcode = RETCODE_OK;
        }
        else
        {
            // The osRetcode will be pdFAIL if the internal queue interaction failed.
            // This could be caused by the queue being full, which is non fatal for a binary semaphore.
            // That's why we set the severity to "WARNING" only.
            retcode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t) RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t) RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_GuardedTask.h */
Retcode_T GuardedTask_SignalFromIsr(GuardedTask_T* handle)
{
    Retcode_T retcode = RETCODE_OK;

    if ((NULL != handle) && (NULL != handle->signal))
    {
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        if (pdPASS == xSemaphoreGiveFromISR(handle->signal, &higherPriorityTaskWoken))
        {
            portYIELD_FROM_ISR(higherPriorityTaskWoken);

            retcode = RETCODE_OK;
        }
        else
        {
            // The osRetcode will be pdFAIL if the internal queue interaction failed.
            // This could be caused by the queue being full, which is non fatal for a binary semaphore.
            // That's why we set the severity to "WARNING" only.
            retcode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t) RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t) RETCODE_INVALID_PARAM);
    }

    return retcode;
}

#endif /* if KISO_FEATURE_GUARDEDTASK */
