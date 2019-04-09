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
 * @file
 *
 * @brief Implementation of compact asynchronous log record.
 *
 * @detail It supports multibyte format messages and is thread safe.
 *  There is no ISR API extension, so it should not be used from interrupts.
 *  A message with a size of LOG_BUFFER_SIZE is first allocated on the caller stack
 *  and then copied to a message queue.
 *  The message is read out by log record task and is sent over dedicated appender asynchronously,
 *  so the caller task is not blocked.
 *
 */

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID  BCDS_UTILS_MODULE_ID_LOGGING_RECORD_ASYNCHRONOUS

#include "BCDS_Logging.h"

#if BCDS_FEATURE_LOGGING

#include "LogConfig.h"

#include <stdio.h>
#include <stdarg.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "BCDS_Queue.h"

#define LOG_SYS_CLOCK_DELAY     (10)
#define LOG_APPENDER_TIMEOUT    (1000)

#define LOG_LINE_FMT            "%.10u %s %3u %.*s\t[%s:%d]\t"
#define LOG_LINE_ENDING         "\r\n"

const char *LOG_LEVEL_STR[LOG_LEVEL_COUNT] =
{   "", "F", "E", "W", "I", "D"};

static Queue_T LogQueue;
static uint8_t LogQueueBuffer[LOG_QUEUE_BUFFER_SIZE];
static TaskHandle_t LogTaskHandle = NULL;

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

static Retcode_T AsyncRecorder_Write(LogLevel_T level, uint8_t package, uint8_t module, const char *file, uint32_t line, const char *fmt, va_list args)
{
    char buffer[LOG_BUFFER_SIZE];

    BCDS_UNUSED(module);

    if ( (NULL == file) || (NULL == fmt))
    {
        return(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER));
    }

    int32_t size = snprintf(buffer, sizeof(buffer), LOG_LINE_FMT,
            (uint32_t) xTaskGetTickCount(), LOG_LEVEL_STR[level], (uint32_t) package,
            configMAX_TASK_NAME_LEN, pcTaskGetTaskName(NULL), file, (uint32_t) line); /*lint -esym(515 516,snprintf) */

    if (size > 0 && (uint32_t) size < sizeof(buffer))
    {
        size += vsnprintf(buffer + size, sizeof(buffer) - size, fmt, args);
    }

    if ((uint32_t) size > sizeof(buffer) - sizeof(LOG_LINE_ENDING))
    {
        size = sizeof(buffer) - sizeof(LOG_LINE_ENDING);
    }

    size += snprintf(buffer + size, sizeof(buffer) - size, LOG_LINE_ENDING);
    return Queue_Put(&LogQueue, buffer, size, NULL, 0);
}

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

#endif /* if BCDS_FEATURE_LOGGING */
