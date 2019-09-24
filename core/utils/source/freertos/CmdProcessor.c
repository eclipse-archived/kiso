
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
 *      Command Processor Interface Implementation
 *
 * @details
 *      This source file implements following features:
 *      - CmdProcessor_Initialize()
 *      - CmdProcessor_Enqueue()
 *      - CmdProcessor_EnqueueFromIsr()
 *      - CmdProcessor_Suspend()
 *      - CmdProcessor_Resume()
 *
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_CMDPROCESSOR

/* Include Kiso_CmdProcessor interface header */
#include "Kiso_CmdProcessor.h"

#if KISO_FEATURE_CMDPROCESSOR

/* KISO basics header files */
#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "portmacro.h"
#include "projdefs.h"

/* Structure saving the data of the function handles enqueued */
struct CmdProcessor_Cmd_S
{
    CmdProcessor_Func_T func;
    void *param1;
    uint32_t param2;
};

/*The data type for command processor elements */
typedef struct CmdProcessor_Cmd_S CmdProcessor_Cmd_T;

/* This function  dequeues the queue and executes the function */
static void Run(void *pvParameters);

static void Dequeue(CmdProcessor_T *cmdProcessor);

/* The description of the function is available in Kiso_CmdProcessor.h*/
Retcode_T CmdProcessor_Initialize(
    CmdProcessor_T *cmdProcessor, const char *name,
    uint32_t taskPriority, uint32_t taskStackDepth, uint32_t queueSize)
{
    uint32_t nameLen;
    Retcode_T retcode = RETCODE_OK;

    if ((NULL != cmdProcessor) && (NULL != name))
    {
        nameLen = strlen(name);
        memset(cmdProcessor->name, 0, CMDPROCESSOR_MAX_NAME_LEN);
        nameLen = (nameLen >= CMDPROCESSOR_MAX_NAME_LEN - 1) ? (CMDPROCESSOR_MAX_NAME_LEN - 1) : nameLen;
        memcpy(cmdProcessor->name, name, nameLen);

        cmdProcessor->queue = (cmdProcessorQueueHandle_t)xQueueCreate(queueSize, sizeof(CmdProcessor_Cmd_T));

        if (NULL == cmdProcessor->queue)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
        }
        else
        {
            if (pdPASS != xTaskCreate(Run, (const char *)cmdProcessor->name, (uint16_t)taskStackDepth,
                                      (void *)cmdProcessor, taskPriority, &cmdProcessor->task))

            {
                /* The task was not created as there was insufficient heap memory remaining.*/
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
            }
            else
            {
                retcode = RETCODE_OK;
            }
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_CmdProcessor.h */
Retcode_T CmdProcessor_Enqueue(CmdProcessor_T *cmdProcessor, CmdProcessor_Func_T func, void *param1, uint32_t param2)
{
    BaseType_t rc;
    Retcode_T retcode;

    /* Null checking */
    assert(func);

    CmdProcessor_Cmd_T cmd;
    cmd.func = func;
    cmd.param1 = param1;
    cmd.param2 = param2;

    /* On success, CMDPROCESSOR_OK is returned.CMDPROCESSOR_QUEUE_FULL if the queue is full */
    if ((NULL != cmdProcessor) && (NULL != cmdProcessor->queue))
    {
        rc = xQueueSend(cmdProcessor->queue, &cmd, 0);

        if (pdPASS == rc)
        {
            retcode = RETCODE_OK;
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_CmdProcessor.h */
Retcode_T CmdProcessor_EnqueueFromIsr(CmdProcessor_T *cmdProcessor, CmdProcessor_Func_T func, void *param1, uint32_t param2)
{
    int32_t rc = pdPASS;
    Retcode_T retcode = RETCODE_OK;
    uint32_t higherPriorityTaskWoken = pdFALSE;

    /* Null checking */
    assert(func != NULL);

    CmdProcessor_Cmd_T cmd;
    cmd.func = func;
    cmd.param1 = param1;
    cmd.param2 = param2;

    /* On success, CMDPROCESSOR_OK is returned.CMDPROCESSOR_QUEUE_FULL if the queue is full */
    if ((NULL != cmdProcessor) && (NULL != cmdProcessor->queue))
    {
        rc = xQueueSendFromISR(cmdProcessor->queue, &cmd, (BaseType_t *)&higherPriorityTaskWoken);
        if (pdPASS == rc)
        {
            retcode = RETCODE_OK;
            portYIELD_FROM_ISR(higherPriorityTaskWoken);
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_CmdProcessor.h */
void CmdProcessor_Suspend(CmdProcessor_T *cmdProcessor)
{
    if ((NULL != cmdProcessor) && (NULL != cmdProcessor->task))
    {
        vTaskSuspend((TaskHandle_t)cmdProcessor->task);
    }
    else
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_INVALID_PARAM));
    }
}

/*  The description of the function is available in Kiso_CmdProcessor.h */
void CmdProcessor_Resume(CmdProcessor_T *cmdProcessor)
{
    if ((NULL != cmdProcessor) && (NULL != cmdProcessor->task))
    {
        vTaskResume((TaskHandle_t)cmdProcessor->task);
    }
    else
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_INVALID_PARAM));
    }
}

/* This is the task function. It dequeues the queue and executes the function read from queue on successful queue receive */
static void Run(void *pvParameters)
{
    CmdProcessor_T *cmdProcessor = (CmdProcessor_T *)pvParameters;

    while (true)
    {
        Dequeue(cmdProcessor);
    }
    Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t)RETCODE_FAILURE));
}

/*  The description of the function is available in Kiso_CmdProcessor.h */
static void Dequeue(CmdProcessor_T *cmdProcessor)
{
    if ((NULL != cmdProcessor) && (NULL != cmdProcessor->queue))
    {
        CmdProcessor_Cmd_T cmd;
        cmdProcessorQueueHandle_t cmdQueue;
        BaseType_t rc;

        cmdQueue = cmdProcessor->queue;
        rc = xQueueReceive(cmdQueue, &cmd, portMAX_DELAY);
        if (rc == pdPASS)
        {
            if (NULL != cmd.func)
            {
                cmd.func(cmd.param1, cmd.param2);
            }
            else
            {
                Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t)RETCODE_INVALID_PARAM));
            }
        }
        else
        {
            Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t)RETCODE_CMDPROCESSOR_QUEUE_ERROR));
        }
    }
    else
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t)RETCODE_INVALID_PARAM));
    }
}

#endif /* if KISO_FEATURE_CMDPROCESSOR */
