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
 *      Module test specification for the Logging_unittest.cc module.
 *
 * @detail
 *      The unit test file template follows the Four-Phase test pattern.
 *
 * @file
 **/

/* Include gtest interface */
#include <gtest.h>

/* Start of global scope symbol and fake definitions section */
extern "C"
{
/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_LOGGING

#if KISO_FEATURE_LOGGING

#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "LogConfig.h"

#include "Logging.c"
    /* End of global scope symbol and fake definitions section */
}

/* Fake one Filter API */
FAKE_VALUE_FUNC(bool, LogFilter_Apply, LogLevel_T, uint8_t, uint8_t);

/* Define a fake recorder */
FAKE_VALUE_FUNC(Retcode_T, RecorderInitFake, void *);
FAKE_VALUE_FUNC(Retcode_T, RecorderDeinitFake, void *);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsizeof-array-argument"
FAKE_VALUE_FUNC(Retcode_T, RecorderWriteFake, void *, LogLevel_T, uint8_t, uint8_t, const char *, uint32_t, const char *, va_list);
#pragma GCC diagnostic pop

static const LogRecorder_T Recorder =
    {
        .Init = RecorderInitFake,
        .Deinit = RecorderDeinitFake,
        .Write = RecorderWriteFake,
        .Wakeup = NULL,
        .Appender = {.Init = NULL, .Write = NULL}};
const LogRecorder_T *Fake_Recorder = &Recorder;

/* Define a fake appender */
FAKE_VALUE_FUNC(Retcode_T, AppenderInitFake, void *);
FAKE_VALUE_FUNC(Retcode_T, AppenderWriteFake, const char *, uint32_t);

static const LogAppender_T Appender =
    {
        .Init = AppenderInitFake,
        .Write = AppenderWriteFake};
const LogAppender_T *Fake_Appender = &Appender;

class Logging : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE(LogFilter_Apply);
        RESET_FAKE(RecorderInitFake);
        RESET_FAKE(RecorderDeinitFake);
        RESET_FAKE(RecorderWriteFake);
        RESET_FAKE(AppenderInitFake);
        RESET_FAKE(AppenderWriteFake);
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        /* Clean logging internal instance. It cannot be cleaned after any initialization */
        Logging_Recorder.Init = NULL;
        Logging_Recorder.Deinit = NULL;
        Logging_Recorder.Write = NULL;
        Logging_Recorder.Wakeup = NULL;
        Logging_Recorder.Appender.Init = NULL;
        Logging_Recorder.Appender.Write = NULL;
    }
};

TEST_F(Logging, Logging_Initialization_Success)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Logging_Init(Fake_Recorder, Fake_Appender);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), RecorderInitFake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), AppenderInitFake_fake.call_count);
}

TEST_F(Logging, Logging_Initialization_InputFail)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Logging_Init(NULL, NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(Logging, Logging_Initialization_RecorderInitFail)
{
    Retcode_T retcode = RETCODE_OK;
    RecorderInitFake_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);

    retcode = Logging_Init(Fake_Recorder, Fake_Appender);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

TEST_F(Logging, Logging_Initialization_AppenderInitFail)
{
    Retcode_T retcode = RETCODE_OK;
    AppenderInitFake_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);

    retcode = Logging_Init(Fake_Recorder, Fake_Appender);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
    EXPECT_EQ(UINT32_C(1), RecorderDeinitFake_fake.call_count);
}

TEST_F(Logging, Logging_Initialization_AppenderInitNullPointerCoverage)
{
    Retcode_T retcode = RETCODE_OK;
    AppenderInitFake_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);

    const LogRecorder_T RecorderNull =
        {
            .Init = NULL,
            .Deinit = NULL,
            .Write = NULL,
            .Wakeup = NULL,
            .Appender = {.Init = NULL, .Write = NULL}};
    const LogRecorder_T *Fake_RecorderNull = &RecorderNull;
    retcode = Logging_Init(Fake_RecorderNull, Fake_Appender);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    /* Next test */
    const LogRecorder_T RecorderNull2 =
        {
            .Init = RecorderInitFake,
            .Deinit = NULL,
            .Write = NULL,
            .Wakeup = NULL,
            .Appender = {.Init = NULL, .Write = NULL}};
    const LogRecorder_T *Fake_RecorderNull2 = &RecorderNull2;
    retcode = Logging_Init(Fake_RecorderNull2, Fake_Appender);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    /* Next test */
    const LogRecorder_T RecorderNull3 =
        {
            .Init = RecorderInitFake,
            .Deinit = RecorderDeinitFake,
            .Write = NULL,
            .Wakeup = NULL,
            .Appender = {.Init = NULL, .Write = NULL}};
    const LogRecorder_T *Fake_RecorderNull3 = &RecorderNull3;
    retcode = Logging_Init(Fake_RecorderNull3, Fake_Appender);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    /* Next test */
    retcode = Logging_Init(Fake_Recorder, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    /* Next test */
    const LogAppender_T AppenderNull =
        {
            .Init = NULL,
            .Write = NULL};
    const LogAppender_T *Fake_AppenderNull = &AppenderNull;
    retcode = Logging_Init(Fake_Recorder, Fake_AppenderNull);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    /* Next test */
    const LogAppender_T AppenderNull1 =
        {
            .Init = AppenderInitFake,
            .Write = NULL};
    const LogAppender_T *Fake_AppenderNull1 = &AppenderNull1;
    retcode = Logging_Init(Fake_Recorder, Fake_AppenderNull1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

/* The following declarations are only here to give an example
 * of input for the next tests.
 */
uint8_t package = 1;
uint8_t module = 2;
const char *file = "log.txt";
uint32_t line = 32;
const char *fmt = "string %s";
char arg[] = "error";

TEST_F(Logging, Logging_Log_Success)
{
    Retcode_T retcode = RETCODE_OK;
    LogFilter_Apply_fake.return_val = true;

    retcode = Logging_Init(Fake_Recorder, Fake_Appender);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = Logging_Log(LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), RecorderWriteFake_fake.call_count);
}

TEST_F(Logging, Logging_Log_NotInit)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Logging_Log(LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_UNINITIALIZED), retcode);
}

TEST_F(Logging, Logging_Log_FilterFail)
{
    Retcode_T retcode = RETCODE_OK;
    LogFilter_Apply_fake.return_val = false;

    retcode = Logging_Init(Fake_Recorder, Fake_Appender);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = Logging_Log(LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_NOT_SUPPORTED), retcode);
}

TEST_F(Logging, Logging_Log_WriteFail)
{
    Retcode_T retcode = RETCODE_OK;
    LogFilter_Apply_fake.return_val = true;
    RecorderWriteFake_fake.return_val = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_INCONSITENT_STATE);

    retcode = Logging_Init(Fake_Recorder, Fake_Appender);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = Logging_Log(LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_INCONSITENT_STATE), retcode);
    EXPECT_EQ(UINT32_C(1), RecorderWriteFake_fake.call_count);
}

#else
}

#endif /* if KISO_FEATURE_LOGGING */
