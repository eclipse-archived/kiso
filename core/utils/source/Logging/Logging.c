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
 *      Logging Interface Implementation.
 *
 * @details
 *      Implements log engine interface.
 *      This source file implements following features:
 *      - Logging_Init()
 *      - Logging_Log()
 * 
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID  KISO_UTILS_MODULE_ID_LOGGING

/* Include Kiso_Logging interface header */
#include "Kiso_Logging.h"

#if KISO_FEATURE_LOGGING

/* KISO basics header files */
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

/* System header file */
#include <stdio.h>

static LogRecorder_T Logging_Recorder =
{   .Init = NULL, .Deinit = NULL, .Write = NULL, .Wakeup = NULL, .Appender =
    {   .Init = NULL, .Write = NULL}};

/*  The description of the function is available in Kiso_Logging.h */
Retcode_T Logging_Init(const LogRecorder_T *recorder, const LogAppender_T *appender)
{
    if (NULL == recorder || NULL == recorder->Init || NULL == recorder->Deinit
            || NULL == recorder->Write || NULL == appender || NULL == appender->Init
            || NULL == appender->Write)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Logging_Recorder.Init = recorder->Init;
    Logging_Recorder.Deinit = recorder->Deinit;
    Logging_Recorder.Write = recorder->Write;
    Logging_Recorder.Appender.Init = appender->Init;
    Logging_Recorder.Appender.Write = appender->Write;

    Retcode_T retcode = Logging_Recorder.Init(&Logging_Recorder);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = Logging_Recorder.Appender.Init(Logging_Recorder.Wakeup);
    if (RETCODE_OK != retcode)
    {
        (void) Logging_Recorder.Deinit(&Logging_Recorder);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_Logging.h */
Retcode_T Logging_Log(LogLevel_T level, uint8_t package, uint8_t module, const char *file,
        uint32_t line, const char *fmt, ...)
{
    if (NULL == Logging_Recorder.Write)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_UNINITIALIZED);
    }

    if (!LogFilter_Apply(level, package, module))
    {
        return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_NOT_SUPPORTED);
    }

    assert(NULL != file && NULL != fmt);

    va_list args;
    va_start(args, fmt);
    Retcode_T retcode = Logging_Recorder.Write(&Logging_Recorder, level, package, module, file, line, fmt, args);
    va_end(args);

    return retcode;
}

#endif /* if KISO_FEATURE_LOGGING */
