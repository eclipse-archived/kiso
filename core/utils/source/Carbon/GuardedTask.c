/*****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  <Brief description about the functionality of the module>
 *
 * ****************************************************************************/

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_GUARDEDTASK

/* own header files */
#include "BCDS_GuardedTask.h"

#if BCDS_FEATURE_GUARDEDTASK

/* additional interface header files */
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*lint +rw(_to_semi) -d__asm=_to_semi */

/* prototypes */
static void GuardedTaskExecute(GuardedTask_T* context);

static void GuardedTaskRunFunction(void* param);

/* local variable */

/* global variables */

/* local functions */
static void GuardedTaskExecute(GuardedTask_T* context)
{
    if ((NULL != context) && (NULL != context->runFunction) && (NULL != context->signal))
    {
        /* Wait for the "run"-semaphore to be signaled. */
        int8_t osRetcode = xSemaphoreTake(context->signal, portMAX_DELAY);
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

/* global functions */

/* The description of the function is available in header file */
Retcode_T GuardedTask_Deinitialize(GuardedTask_T* handle)
{
    Retcode_T retcode;

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
            if (pdPASS != xTaskCreate(GuardedTaskRunFunction, taskName, taskStackSize, handle, taskPriority, &handle->task))
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

/* The description of the function is available in header file */
Retcode_T GuardedTask_Signal(GuardedTask_T* handle)
{
    Retcode_T retcode;

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

/* The description of the function is available in header file */
Retcode_T GuardedTask_SignalFromIsr(GuardedTask_T* handle)
{
    Retcode_T retcode;

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

/* Deprecated function. See header file for more information */
Retcode_T GuardedTask_signalFromIsr(GuardedTask_T* handle)
{
	return(GuardedTask_SignalFromIsr(handle));
}

#endif /* if BCDS_FEATURE_GUARDEDTASK */
