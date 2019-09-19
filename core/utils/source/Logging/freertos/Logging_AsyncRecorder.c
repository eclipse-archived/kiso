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
 *      Implementation of compact asynchronous log record.
 *
 * @details
 *      It supports multibyte format messages and is thread safe.
 *      There is no ISR API extension, so it should not be used from interrupts.
 *      A message with a size of LOG_BUFFER_SIZE is first allocated on the caller stack
 *      and then copied to a message queue.
 *      The message is read out by log record task and is sent over dedicated appender asynchronously,
 *      so the caller task is not blocked.
 * 
 * @file
 **/

/* Include utils to have access to the defined module and error IDs */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID  KISO_UTILS_MODULE_ID_LOGGING_RECORD_ASYNCHRONOUS

/* Include the Logging header, which include the configuration that enable and define macros for this module */
#include "Kiso_Logging.h"

/* Enable/Disable macro for the feature */
#if KISO_FEATURE_LOGGING && KISO_ASYNC_RECORDER

/* Include needed headers */
#include <stdio.h>
#include <stdarg.h>
#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"
#include "Kiso_Queue.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

/* Message structure definition via macros*/
#define LOG_LINE_FMT "%" PRIu32 " %s %" PRIu32 " %.*s\t[%s:%" PRIu32 "]\t"
#define LOG_LINE_ENDING         "\r\n"

const char *LOG_LEVEL_STR[LOG_LEVEL_COUNT] =
{   "", "F", "E", "W", "I", "D"};

static Queue_T LogQueue; /**< Guarded queue used to forward the messages to the task that will send them */
static uint8_t LogQueueBuffer[LOG_QUEUE_BUFFER_SIZE]; /**< Buffer that will gather a suit of messages */
static TaskHandle_t LogTaskHandle = NULL; /**< FreeRTOS task handle */

/**
 * @brief
 * 		The task will be triggered via the guarded-queue and will run between 2 application run cycles.
 */
static void AsyncRecorder_Task(void *param)
{
    char *msg;
    uint32_t len;
    LogRecorder_T *recorder = (LogRecorder_T *) param;

    if ((NULL == recorder) || (NULL == recorder->Wakeup) || (NULL == recorder->Appender.Write))
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER));
        return;
    }

    vTaskDelay(LOG_SYS_CLOCK_DELAY / portTICK_PERIOD_MS);

    while (1)
    {
        if (RETCODE_OK == Queue_Get(&LogQueue, (void **) &msg, &len, portMAX_DELAY))
        {
            if (RETCODE_OK == recorder->Appender.Write(msg, len))
            {
                (void) xSemaphoreTake((SemaphoreHandle_t) recorder->Wakeup, LOG_APPENDER_TIMEOUT / portTICK_PERIOD_MS);
                (void) Queue_Purge(&LogQueue);
            }
        }
    }
}

/**
 * @brief
 * 		Initialize the recorder (queue, task, semaphore creations)
 */
static Retcode_T AsyncRecorder_Init(void *self)
{
    LogRecorder_T *recorder = (LogRecorder_T *) self;
    if (NULL == recorder)
    {
        return(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER));
    }

    Retcode_T retcode = Queue_Create(&LogQueue, LogQueueBuffer, sizeof(LogQueueBuffer));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    recorder->Wakeup = xSemaphoreCreateBinary();
    if (NULL == recorder->Wakeup)
    {
        (void) Queue_Delete(&LogQueue);
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    }

    BaseType_t status = xTaskCreate(AsyncRecorder_Task, "LOG", LOG_TASK_STACK_SIZE, recorder, LOG_TASK_PRIORITY, &LogTaskHandle);
    if (pdPASS != status)
    {
        (void) Queue_Delete(&LogQueue);
        vSemaphoreDelete(recorder->Wakeup);
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    }

    return RETCODE_OK;
}

/**
 * @brief
 * 		Deinitialize the recorder (queue, task, semaphore destruction)
 */
static Retcode_T AsyncRecorder_Deinit(void *self)
{
    LogRecorder_T *recorder = (LogRecorder_T *) self;
    if ((NULL == recorder) || (NULL == recorder->Wakeup))
    {
        return(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER));
    }

    Retcode_T retcode = Queue_Delete(&LogQueue);
    vSemaphoreDelete(recorder->Wakeup);
    recorder->Wakeup = NULL;
    vTaskDelete(LogTaskHandle);

    return retcode;
}

/**
 * @brief
 * 		Function that will be called when a log API is called (LOG_XXX(...))
 */
static Retcode_T AsyncRecorder_Write(void *self, LogLevel_T level, uint8_t package, uint8_t module, const char *file, uint32_t line, const char *fmt, va_list args)
{
    char buffer[LOG_BUFFER_SIZE];
    KISO_UNUSED(module);
    KISO_UNUSED(self);

    /* Check NULL pointers to avoid overflows or wrong addressing */
    if ( (NULL == file) || (NULL == fmt))
    {
        return(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER));
    }

    /* Add first into the buffer different information relative to what is logged such as tick, file name, line, log level, ... */
    int32_t size = snprintf(buffer, sizeof(buffer), LOG_LINE_FMT,
            (uint32_t) xTaskGetTickCount(), LOG_LEVEL_STR[level], (uint32_t) package,
            configMAX_TASK_NAME_LEN, pcTaskGetTaskName(NULL), file, (uint32_t) line);
    /* Parse and add the message sent via the log function */
    if (size > 0 && (uint32_t) size < sizeof(buffer))
    {
        size += vsnprintf(buffer + size, sizeof(buffer) - size, fmt, args);
    }

    /* Delete data to make an end of line fit */
    if ((uint32_t) size > sizeof(buffer) - sizeof(LOG_LINE_ENDING))
    {
        size = sizeof(buffer) - sizeof(LOG_LINE_ENDING);
    }

    /* Add the end of line */
    size += snprintf(buffer + size, sizeof(buffer) - size, LOG_LINE_ENDING);

    /* Put into the queue the generated log message */
    return Queue_Put(&LogQueue, buffer, size, NULL, 0);
}

/**
 * @brief Create singleton
 */
static const LogRecorder_T LogRecordAsyncCompact =
{
    .Init = AsyncRecorder_Init,
    .Deinit = AsyncRecorder_Deinit,
    .Write = AsyncRecorder_Write,
    .Wakeup = NULL,
    .Appender =
    {   .Init = NULL, .Write = NULL}
};
const LogRecorder_T* Logging_AsyncRecorder = &LogRecordAsyncCompact;

#endif /* if KISO_FEATURE_LOGGING && KISO_ASYNC_RECORDER */
