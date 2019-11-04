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
 *      Module test specification for the Logging_SyncRecorder_unittest.cc module.
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
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_LOGGING_RECORD_SYNCHRONOUS

/* Check if the logging feature is activated */
#if KISO_FEATURE_LOGGING
#include "LogConfig.h"

/* Check if the sync recorder is activated */
#if KISO_SYNC_RECORDER

#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "portmacro_th.hh"

#include "Logging_SyncRecorder.c"

    /* End of global scope symbol and fake definitions section */
}

/* Define pseudo fake snprintf */
static bool SetCustomerSnprintf = false;
static int32_t SetCountdownFake = 0; /* After x calls, call the fake snprintf */

static int SnprintfCustomReturn = 0;

int snprintf(char *s, size_t n, const char *format, ...)
{
    if (SetCustomerSnprintf == true && SetCountdownFake == 0)
    {
        return SnprintfCustomReturn;
    }
    else
    {
        SetCountdownFake -= 1;
        int ret;
        va_list list;
        va_start(list, format);
        ret = vsnprintf(s, n, format, list);
        va_end(list);
        return ret;
    }
}

/* Define a fake appender */
static Retcode_T AppenderFakeRetcode = RETCODE_OK;
static const char *AppenderFakeReceivedBuffer = NULL;
static uint32_t AppenderFakeReceivedBufferSize = 0;

static Retcode_T LogAppenderInitFake(void *wakeup)
{
    KISO_UNUSED(wakeup);
    return AppenderFakeRetcode;
}

static Retcode_T LogAppenderWriteFake(const char *message, uint32_t length)
{
    AppenderFakeReceivedBuffer = message;
    AppenderFakeReceivedBufferSize = length;

    return AppenderFakeRetcode;
}

static const LogAppender_T LogAppender =
    {
        .Init = LogAppenderInitFake,
        .Write = LogAppenderWriteFake};
const LogAppender_T *Logging_LogAppender = &LogAppender;

/* Create Sync recorder to be tested -> needed because upper logging instance generates an instance of the compact */
static LogRecorder_T LogRecordSyncCompactTestInstance =
    {
        .Init = SyncRecorder_Init,
        .Deinit = SyncRecorder_Deinit,
        .Write = SyncRecorder_Write,
        .Wakeup = NULL,
        .Appender = LogAppender};

/* The tests */
class Logging_SyncRecorder : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* Nothing to do if clean up is not required */
    }
};

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderInitialization_Success)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = LogRecordSyncCompact.Init(&LogRecordSyncCompactTestInstance);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderInitialization_NullPointerGiven)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = LogRecordSyncCompact.Init(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderDeInitialization_Success)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = LogRecordSyncCompact.Deinit(&LogRecordSyncCompactTestInstance);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderDeinitialization_NullPointerGiven)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = LogRecordSyncCompact.Deinit(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

/* For the next tests, we need to call the Logging_SyncRecorder write in the va context */
Retcode_T callWrite(void *self, LogLevel_T level, uint8_t package, uint8_t module, const char *file, uint32_t line, const char *fmt, ...)
{
    Retcode_T retcode = RETCODE_OK;

    va_list args;
    va_start(args, fmt);
    retcode = LogRecordSyncCompact.Write(self, level, package, module, file, line, fmt, args);
    va_end(args);

    return retcode;
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_SuccessWithContentCheck)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t package = 1;
    uint8_t module = 2;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "string %s";
    char arg[] = "error";

    AppenderFakeRetcode = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 10;

    retcode = callWrite(&LogRecordSyncCompactTestInstance, LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE_OK, retcode);

    /* Expected output */
    char expectedOutput[] = "10 E 1 (null)\t[log.txt:32]\tstring error\r\n";
    EXPECT_STREQ(expectedOutput, AppenderFakeReceivedBuffer);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_NullPointerFail)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = callWrite(NULL, LOG_LEVEL_ERROR, 0, 0, 0, 0, NULL, NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_FileNullPointerFail)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t package = 1;
    uint8_t module = 2;
    uint32_t line = 32;
    const char *fmt = "string %s";
    char arg[] = "error";

    AppenderFakeRetcode = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 10;

    retcode = callWrite(&LogRecordSyncCompactTestInstance, LOG_LEVEL_ERROR, package, module, NULL, line, fmt, arg);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_VaListNullPointerFail)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t package = 1;
    uint8_t module = 2;
    const char *file = "log.txt";
    uint32_t line = 32;
    char arg[] = "error";

    AppenderFakeRetcode = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 10;

    retcode = callWrite(&LogRecordSyncCompactTestInstance, LOG_LEVEL_ERROR, package, module, file, line, NULL, arg);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_InputMetaDataEmpty)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t package = 1;
    uint8_t module = 2;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "";
    char arg[] = "";

    AppenderFakeRetcode = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 10;

    SetCustomerSnprintf = true;
    SetCountdownFake = 0;
    SnprintfCustomReturn = 0;

    retcode = callWrite(&LogRecordSyncCompactTestInstance, LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(0), AppenderFakeReceivedBufferSize);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_InputMetaDataOverflow)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t package = 1;
    uint8_t module = 2;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "";
    char arg[] = "";

    AppenderFakeRetcode = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 10;

    SetCustomerSnprintf = true;
    SetCountdownFake = 0;
    SnprintfCustomReturn = LOG_BUFFER_SIZE;

    retcode = callWrite(&LogRecordSyncCompactTestInstance, LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(LOG_BUFFER_SIZE, (uint16_t)(AppenderFakeReceivedBufferSize));
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_NoPayload)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t package = 1;
    uint8_t module = 2;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "";
    char arg[] = "";

    AppenderFakeRetcode = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 10;

    SetCustomerSnprintf = true;
    SetCountdownFake = 1;
    SnprintfCustomReturn = 0;

    retcode = callWrite(&LogRecordSyncCompactTestInstance, LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE_OK, retcode);

    /* Expected output */
    printf("%s", AppenderFakeReceivedBuffer);
    char expectedOutput[] = "10 E 1 (null)\t[log.txt:32]\t";
    EXPECT_STREQ(expectedOutput, AppenderFakeReceivedBuffer);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_PayloadToBig)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t package = 1;
    uint8_t module = 2;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "";
    char arg[] = "";

    AppenderFakeRetcode = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 10;

    SetCustomerSnprintf = true;
    SetCountdownFake = 1;
    SnprintfCustomReturn = LOG_BUFFER_SIZE;

    retcode = callWrite(&LogRecordSyncCompactTestInstance, LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE_OK, retcode);
    char expectedOutput[] = "10 E 1 (null)\t[log.txt:32]\t";
    EXPECT_STREQ(expectedOutput, AppenderFakeReceivedBuffer);
}

TEST_F(Logging_SyncRecorder, Logging_SyncRecorderWrite_AppenderFail)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t package = 1;
    uint8_t module = 2;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "string %s";
    char arg[] = "error";

    AppenderFakeRetcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
    xTaskGetTickCount_fake.return_val = 10;

    retcode = callWrite(&LogRecordSyncCompactTestInstance, LOG_LEVEL_ERROR, package, module, file, line, fmt, arg);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

#else
}
#endif /* if KISO_SYNC_RECORDER */
#else
}
#endif /* if KISO_FEATURE_LOGGING */
