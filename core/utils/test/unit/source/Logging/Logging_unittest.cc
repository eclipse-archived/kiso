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

FFF_DEFINITION_BLOCK_START

/* Start of global scope symbol and fake definitions section */
extern "C"
{
/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID  KISO_UTILS_MODULE_ID_LOGGING

#if KISO_FEATURE_LOGGING

#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "portmacro_th.hh"
#include "Kiso_Queue_th.hh"
#include "semphr_th.hh"
#include "LogConfig.h"
#define Queue_Purge(x) KISO_UNUSED(x);break;

#undef KISO_MODULE_ID
#include "../../../../source/Logging/freertos/Logging_AsyncRecorder.c"
#undef KISO_MODULE_ID
#include "Filter.c"
#undef KISO_MODULE_ID
#include "Logging.c"

static Retcode_T AppenderInitFake = RETCODE_OK;

static Retcode_T LogAppenderInitFake(void *wakeup)
{
    KISO_UNUSED(wakeup);
    return AppenderInitFake;
}

static Retcode_T WriteResult = RETCODE_OK;

static Retcode_T LogAppenderWriteFake(const char *message, uint32_t length)
{
    KISO_UNUSED(message);
    KISO_UNUSED(length);
    if (WriteResult != RETCODE_OK)
    {
        WriteResult = RETCODE_OK;
        return RETCODE_FAILURE;
    }
    return WriteResult;
}
Retcode_T RecordWriteFake(LogLevel_T level, uint8_t package, uint8_t module, const char *file, uint32_t line, const char *fmt, va_list args)
{
    KISO_UNUSED(level);
    KISO_UNUSED(package);
    KISO_UNUSED(file);
    KISO_UNUSED(line);
    KISO_UNUSED(fmt);
    KISO_UNUSED(args);
    KISO_UNUSED(module);
    return RETCODE_OK;
}
static SemaphoreHandle_t FakeSemaphoreHandle = NULL;

static SemaphoreHandle_t xSemaphoreCreateBinaryFake(void)
{
    return FakeSemaphoreHandle;
}

static Retcode_T QueueGetResult = RETCODE_FAILURE;

static Retcode_T Queue_GetFake(Queue_T *arg1, void **arg2, uint32_t*arg3, uint32_t arg4)
{
    KISO_UNUSED(arg1);
    KISO_UNUSED(arg2);
    KISO_UNUSED(arg3);
    KISO_UNUSED(arg4);
    if (RETCODE_FAILURE == QueueGetResult)
    {
        QueueGetResult = RETCODE_OK;
        return RETCODE_FAILURE;
    }
    else
    {
        *arg3 = 1UL; /* Random Number */
    }
    return QueueGetResult;
}

static BaseType_t xTaskPassed = pdFAIL;

static BaseType_t xTaskCreateFake(TaskHookFunction_t TaskFun, const char * arg1,
        unsigned short arg2, void * arg3, UBaseType_t arg4, TaskHandle_t* arg5)
{
    KISO_UNUSED(TaskFun);
    KISO_UNUSED(arg1);
    KISO_UNUSED(arg2);
    KISO_UNUSED(arg3);
    KISO_UNUSED(arg4);
    KISO_UNUSED(arg5);

    return xTaskPassed;
}

 /* End of global scope symbol and fake definitions section */
}

FFF_DEFINITION_BLOCK_END

static Retcode_T LogAppenderInit(void *init)
{
    KISO_UNUSED(init);
    return RETCODE_OK;
}

static Retcode_T LogAppenderWrite(const char *message, uint32_t length)
{
    Retcode_T rc = RETCODE_OK;

    if (NULL == message)
    {
        rc = RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_NULL_POINTER);
    }
    else if (0UL == length)
    {
        rc = RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_INVALID_PARAM);
    }
    else
    {
        for (uint32_t i = 0; i < length; i++)
        {
            (void) (message[i]);
        }
    }

    return rc;
}

static const LogAppender_T LogAppender =
        {
                .Init = LogAppenderInit,
                .Write = LogAppenderWrite
        };

const LogAppender_T *Logging_LogAppender = &LogAppender;


class Logging: public testing::Test
{
protected:

    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        RESET_FAKE(xSemaphoreCreateBinary);
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(Queue_Get);
        RESET_FAKE(Queue_Create);

        xSemaphoreCreateBinary_fake.custom_fake = xSemaphoreCreateBinaryFake;
        xTaskCreate_fake.custom_fake = xTaskCreateFake;
        Queue_Get_fake.custom_fake = Queue_GetFake;
        QueueGetResult = RETCODE_FAILURE;
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* Nothing to do if clean up is not required */
    }
};

TEST_F(Logging, Logging_LogFail)
{
    /** @testcase{ Logging, Logging_InitFail }
     *
     * Verifying that a Retcode error code is returned when trying to log something without first
     * initialising the recorder.
     */

    LogLevel_T level = LOG_LEVEL_NONE;
    uint8_t package = 0;
    uint8_t module = 0;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "string";
    const char *arg = "error";

    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(Logging_Log(level, package, module, file, line, fmt, arg)));
}

TEST_F(Logging, Logging_InitFail)
{
    /** @testcase{ Logging, Logging_InitFail }
     * 
     * Verifying that a Retcode error code is returned when the logging functionality is
     * initialised with invalid parameters in the struct of the required recorder or appender.
     */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Logging_Init((LogRecorder_T*) NULL, (LogAppender_T*) NULL)));

    LogAppender_T appender =
    {   .Init = NULL, .Write = NULL};
    LogRecorder_T recorder =
    { .Init = NULL, .Deinit = NULL, .Write = NULL, .Wakeup = NULL, .Appender = appender };

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Logging_Init(&recorder, NULL)));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Logging_Init(Logging_AsyncRecorder, NULL)));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Logging_Init(Logging_AsyncRecorder, &appender)));
    appender.Init = &LogAppenderInitFake;
    appender.Write = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Logging_Init(Logging_AsyncRecorder, &appender)));
    appender.Write = &LogAppenderWriteFake;

    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(Logging_Init(Logging_AsyncRecorder, &appender)));
    FakeSemaphoreHandle = (SemaphoreHandle_t) 1;
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(Logging_Init(Logging_AsyncRecorder, &appender)));
    xTaskPassed = pdPASS;
    AppenderInitFake = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(Logging_Init(Logging_AsyncRecorder, &appender)));
    Queue_Create_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(Logging_Init(Logging_AsyncRecorder, &appender)));
    recorder.Init = Logging_AsyncRecorder->Init;
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Logging_Init(&recorder, &appender)));
    recorder.Deinit = Logging_AsyncRecorder->Deinit;
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Logging_Init(&recorder, &appender)));
    AppenderInitFake = RETCODE_OK;

}

TEST_F(Logging, Logging_InitPass)
{
    /** @testcase{ Logging, Logging_InitPass }
     *
     * Verifying that intialisation passes when valid recorder and (in this case albeit faked)
     * valid appender passed to the initialisation function.
     */

    LogAppender_T appender =
    {   .Init = &LogAppenderInitFake, .Write = &LogAppenderWriteFake};

    FakeSemaphoreHandle = (SemaphoreHandle_t) 1;
    xTaskPassed = pdPASS;

    EXPECT_EQ(RETCODE_OK, Logging_Init(Logging_AsyncRecorder, &appender));
}

TEST_F(Logging, Logging_LogPass)
{
    /** @testcase{ Logging, Logging_LogPass }
     *
     * Verifying that logging is successful when the recorder has been properly initialised.
     * Furthermore tests that even log level none can be logged as no filter with higher log
     * level has been added.
     */

    LogLevel_T level = LOG_LEVEL_NONE;
    uint8_t package = 0;
    uint8_t module = 0;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "string";
    const char *arg = "error";

    EXPECT_EQ(RETCODE_OK, Logging_Log(level, package, module, file, line, fmt, arg));
}

TEST_F(Logging, Logging_FilterPass)
{
    /** @testcase{ Logging, Logging_FilterPass }
     *
     * Verifying that a filter can be added and configured that allows the logging of errors
     * with a lower or equal level. Also tests that filters can be removed and others added to
     * allow the logging of errors with a lower level.
     */

    LogLevel_T level = LOG_LEVEL_WARNING;
    uint8_t package = 0;
    uint8_t module = KISO_UTILS_MODULE_ID_LOGGING;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "string";
    const char *arg = "error";

    EXPECT_EQ(1U, LogFilter_Add(LOG_LEVEL_WARNING, 0, KISO_UTILS_MODULE_ID_LOGGING));
    EXPECT_EQ(RETCODE_OK, Logging_Log(level, package, module, file, line, fmt, arg));
    EXPECT_EQ(RETCODE_OK, LogFilter_Configure(1U, LOG_LEVEL_ERROR, package, module));
    EXPECT_EQ(RETCODE_OK, LogFilter_Delete(0U));
    EXPECT_EQ(0U, LogFilter_Add(LOG_LEVEL_INFO, 2, module));
    EXPECT_EQ(RETCODE_OK, Logging_Log(LOG_LEVEL_ERROR, 2, module, file, line, fmt, arg));
}

TEST_F(Logging, Logging_FilterFail)
{

    /** @testcase{ Logging, Logging_FilterFail }
     *
     * Verifying that a filter blocks error that do not come from the same package or module
     * as the filter has been configured for. Also tests that a filter blocks errors if they
     * have a higher level than the filter level.
     */

    LogLevel_T level = LOG_LEVEL_INFO;
    uint8_t package = 0;
    uint8_t module = KISO_UTILS_MODULE_ID_LOGGING;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "string";
    const char *arg = "error";

    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(Logging_Log(level, 1, module, file, line, fmt, arg)));
    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(Logging_Log(level, 2,
            KISO_UTILS_MODULE_ID_LOGGING_FILTER, file, line, fmt, arg)));
    EXPECT_EQ(RETCODE_OK, LogFilter_Delete(0U));
    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(Logging_Log(level, package, module, file, line, fmt, arg)));

    /* Currently no check is done to ensure that the the filter you are deleting or configuring
     * has been added, hence this check is only to ensure that the Filter Id is contained in the
     * array of filter items.
     */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(LogFilter_Configure(255U, LOG_LEVEL_ERROR, package, module)));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(LogFilter_Delete(255U)));

}

TEST_F(Logging, Logging_RecorderTask)
{
    /** @testcase{ Logging, Logging_RecorderTask }
     *
     * Simple test to just verify that the faked functions causes the expected number of runs
     * of the loop in the AsyncRecorder_Task.
     */

    WriteResult = RETCODE_FAILURE;
    QueueGetResult = RETCODE_OK;

    LogAppender_T Appender =
    {   .Init = LogAppenderInitFake, .Write = LogAppenderWriteFake};
    LogRecorder_T Recorder =
    {   .Init = NULL, .Deinit = NULL, .Write = NULL, .Wakeup = Logging_AsyncRecorder->Wakeup,
            .Appender = *Logging_LogAppender};

    AsyncRecorder_Task(&Recorder);
    Recorder.Appender = Appender;
    AsyncRecorder_Task(&Recorder);
    EXPECT_EQ(0U, Queue_Get_fake.call_count);
}

TEST_F(Logging, Logging_UARTefm32Appender)
{
    /** @testcase{ Logging, Logging_UARTefm32Appender }
     *
     * Test the functionality of the UARTAppender for the efm32 board. Verifies that it can be
     * used to initialise the recorder and log errors.
     */
    LogLevel_T level = LOG_LEVEL_WARNING;
    uint8_t package = 0;
    uint8_t module = KISO_UTILS_MODULE_ID_LOGGING;
    const char *file = "log.txt";
    uint32_t line = 32;
    const char *fmt = "string";
    const char *arg = "error";

    EXPECT_EQ(RETCODE_OK, LogFilter_Delete(1U));
    EXPECT_EQ(RETCODE_OK, Logging_Init(Logging_AsyncRecorder, Logging_LogAppender));
    EXPECT_EQ(0U, xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(RETCODE_OK, Logging_Log(level, package, module, file, line, fmt, arg));
}
TEST_F(Logging, Logging_AsyncRecorder_Task)
{
    void *wakeupStatus;
    uint8_t status = 0;
    wakeupStatus = &status;
    LogAppender_T Appender =
        {   .Init = LogAppenderInitFake, .Write = LogAppenderWriteFake};
    LogRecorder_T recorder =
        { .Init = Logging_AsyncRecorder->Init, .Deinit = Logging_AsyncRecorder->Deinit, .Write = RecordWriteFake, .Wakeup = wakeupStatus, .Appender = Appender };
    AsyncRecorder_Task(&recorder);
    EXPECT_EQ(3U, Queue_Get_fake.call_count);
}

TEST_F(Logging, Logging_AsyncRecorder_TaskFail)
{
    void *wakeupStatus;
    uint8_t status = 0;
    wakeupStatus = &status;
    LogAppender_T Appender =
        {   .Init = LogAppenderInitFake, .Write = NULL};
    LogRecorder_T recorder =
        { .Init = Logging_AsyncRecorder->Init, .Deinit = Logging_AsyncRecorder->Deinit, .Write = NULL, .Wakeup = wakeupStatus, .Appender = Appender };
    RESET_FAKE(Retcode_RaiseError);
    AsyncRecorder_Task(&recorder);
    EXPECT_EQ(1U, Retcode_RaiseError_fake.call_count);

    AsyncRecorder_Task(NULL);
    EXPECT_EQ(2U, Retcode_RaiseError_fake.call_count);
}

TEST_F(Logging, NULLTestinInitDeinit)
{
    Retcode_T retcode= RETCODE_OK;
    retcode = Logging_AsyncRecorder->Init(NULL);
    retcode = Retcode_GetCode(retcode);
    EXPECT_EQ(RETCODE_NULL_POINTER, retcode);

    retcode = Logging_AsyncRecorder->Deinit(NULL);
    retcode = Retcode_GetCode(retcode);
    EXPECT_EQ(RETCODE_NULL_POINTER, retcode);

    LogAppender_T Appender =
        {   .Init = LogAppenderInitFake, .Write = NULL};
    LogRecorder_T recorder =
        { .Init = Logging_AsyncRecorder->Init, .Deinit = Logging_AsyncRecorder->Deinit, .Write = NULL, .Wakeup = NULL, .Appender = Appender };
    retcode = Logging_AsyncRecorder->Deinit(&recorder);
    retcode = Retcode_GetCode(retcode);
    EXPECT_EQ(RETCODE_NULL_POINTER, retcode);
}
#else
}
FFF_DEFINITION_BLOCK_END

#endif /* if KISO_FEATURE_LOGGING */
