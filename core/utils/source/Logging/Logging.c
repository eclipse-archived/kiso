/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/
/**
 * @file
 *
 * @brief Implements log engine interface.
 *
 */

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID  BCDS_UTILS_MODULE_ID_LOGGING

#include "BCDS_Logging.h"

#if BCDS_FEATURE_LOGGING

#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include <stdio.h>

static LogRecorder_T Logging_Recorder =
{   .Init = NULL, .Deinit = NULL, .Write = NULL, .Wakeup = NULL, .Appender =
    {   .Init = NULL, .Write = NULL}};

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

Retcode_T Logging_Log(LogLevel_T level, uint8_t package, uint8_t module, const char *file,
        uint32_t line, const char *fmt, ...)
{
    if (NULL == Logging_Recorder.Wakeup)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_UNINITIALIZED);
    }

    if (!LogFilter_Apply(level, package, module))
    {
        return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_NOT_SUPPORTED);
    }

    assert(NULL != file && NULL != fmt);

    va_list args;
    va_start(args, fmt); /*lint !e530 !e10 !e40 */
    Retcode_T retcode = Logging_Recorder.Write(level, package, module, file, line, fmt, args);
    va_end(args);

    return retcode;
}

#endif /* if BCDS_FEATURE_LOGGING */
