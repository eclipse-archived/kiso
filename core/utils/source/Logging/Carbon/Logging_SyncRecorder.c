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
 * @brief Implementation of compact synchronous log record.
 *
 */

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID  BCDS_UTILS_MODULE_ID_LOGGING_RECORD_SYNCHRONOUS

#include "BCDS_Logging.h"

#if BCDS_FEATURE_LOGGING

#include "LogConfig.h"

#include <stdio.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"

#define LOG_LINE_FMT            "%.10u %s %3u %.*s\t[%s:%d]\t"
#define LOG_LINE_ENDING         "\r\n"

const char *LOG_LEVEL_STRING[LOG_LEVEL_COUNT] =
{   "", "F", "E", "W", "I", "D"};


static Retcode_T SyncRecorder_Init(void *self)
{
    LogRecorder_T *recorder = (LogRecorder_T *) self;
    if (NULL == recorder)
    {
        return(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER));
    }
    return RETCODE_OK;
}

static Retcode_T SyncRecorder_Deinit(void *self)
{
    LogRecorder_T *recorder = (LogRecorder_T *) self;
    if ((NULL == recorder) || (NULL == recorder->Wakeup))
    {
        return(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER));
    }
    return RETCODE_OK;
}

static Retcode_T SyncRecorder_Write(void *self, LogLevel_T level, uint8_t package, uint8_t module, const char *file, uint32_t line, const char *fmt, va_list args)
{
    char buffer[LOG_BUFFER_SIZE] = {0};
    int32_t size = 0;
    BCDS_UNUSED(module);

    /* Check NULL pointers to avoid overflows or wrong addressing */
    if ( (NULL == file) || (NULL == fmt) || (NULL == self))
    {
        return(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER));
    }

    /* Cast the pointer to the recorder to be able to access the appender */
    LogRecorder_T *recorder = (LogRecorder_T *) self;

    /* Add first into the buffer different information relative to what is logged such as tick, file name, line, log level, ... */
    size = snprintf(buffer, sizeof(buffer), LOG_LINE_FMT,
            (uint32_t) xTaskGetTickCount(), LOG_LEVEL_STRING[level], (uint32_t) package,
            configMAX_TASK_NAME_LEN, pcTaskGetTaskName(NULL), file, (uint32_t) line); /*lint -esym(515 516,snprintf) */

    /* Parse and add the message sent via the log function */
    if (size > 0 && (uint32_t) size < sizeof(buffer))
    {
        size += vsnprintf(buffer + size, sizeof(buffer) - size, fmt, args);
    }

    /* Add the end of line */
    if (size > 0 && (uint32_t) size < sizeof(buffer))
    {
    	size += snprintf(buffer + size, sizeof(buffer) - size, LOG_LINE_ENDING);
    }

    /* Write to the appender the generated log message */
    return recorder->Appender.Write(buffer, size);
}

static const LogRecorder_T LogRecordSyncCompact =
{
    .Init = SyncRecorder_Init,
    .Deinit = SyncRecorder_Deinit,
    .Write = SyncRecorder_Write,
    .Wakeup = NULL,
    .Appender =
    {   .Init = NULL, .Write = NULL}
};

const LogRecorder_T* Logging_SyncRecorder = &LogRecordSyncCompact;

#endif /* if BCDS_FEATURE_LOGGING */
