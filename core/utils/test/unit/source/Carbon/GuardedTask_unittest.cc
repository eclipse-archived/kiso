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
 *  @file
 *
 *  Module test specification for the GuardedTask_unittest module.
 *
 * The unit test file template follows the Four-Phase test pattern. 
 *
 * ****************************************************************************/

/* include gtest interface */
#include <gtest.h>

extern "C"
{/* start of global scope symbol and fake definitions section */

/* setup compile time configuration defines */
#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_GUARDEDTASK

#if BCDS_FEATURE_GUARDEDTASK

/* include faked interfaces */
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"

#include "FreeRTOS_th.hh"
#include "semphr_th.hh"
#include "queue_th.hh"
#include "task_th.hh"

/* include module under test */
#include "GuardedTask.c"

} /* end of global scope symbol and fake definitions section */

static bool runFunctionCalled = false;

static void dummyRunFunction(void)
{
    runFunctionCalled = true;
}

class guardedTask: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE(xTaskCreate);
        RESET_FAKE(xSemaphoreGiveFromISR);
        RESET_FAKE(xSemaphoreGive);
        RESET_FAKE(xSemaphoreTake);
        RESET_FAKE(xSemaphoreCreateBinary);
        RESET_FAKE(xSemaphoreCreateMutex);
        RESET_FAKE(vQueueDelete);
        RESET_FAKE(Retcode_RaiseError);

        runFunctionCalled = false;
    }

    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

/* specify test cases ******************************************************* */
TEST_F(guardedTask, GuardedTaskInitializeInvalidParam1)
{
    /** @testcase{ guardedTask::GuardedTaskInitializeInvalidParam1: }
     *
     * API is used to check if the initialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T* handle = NULL;
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Initialize(handle, &dummyRunFunction, (const char*) NULL, 0, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(UINT32_C(0), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskInitializeInvalidParam2)
{
    /** @testcase{ guardedTask::GuardedTaskInitializeInvalidParam2: }
     * 
     * API is used to check if the initialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Initialize(&handleBlock, &dummyRunFunction, (const char*) NULL, 0, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(UINT32_C(0), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskInitializeInvalidParam3)
{
    /** @testcase{ guardedTask::GuardedTaskInitializeInvalidParam3: }
     *
     * API is used to check if the initialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Initialize(&handleBlock, NULL, (const char*) "abc", 0, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(UINT32_C(0), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskInitializeTaskFail)
{
    /** @testcase{ guardedTask::GuardedTaskInitializeTaskFail: }
     *
     * API is used to check if the initialize function handle task creation fail correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode;
    xTaskCreate_fake.return_val = pdFAIL;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Initialize(&handleBlock, &dummyRunFunction, (const char*) "abc", 0, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retcode));
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(UINT32_C(1), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);;

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskInitializeSemaphoreBinaryFail)
{
    /** @testcase{ guardedTask::GuardedTaskInitializeSemaphoreBinaryFail: }
     *
     * API is used to check if the initialize function will handle binary-semaphore fail correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode;
    xTaskCreate_fake.return_val = pdPASS;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) NULL;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Initialize(&handleBlock, &dummyRunFunction, (const char*) "abc", 0, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retcode));
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(UINT32_C(0), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);;

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskInitializeSuccess)
{
    /** @testcase{ guardedTask::GuardedTaskInitializeSuccess: }
     *
     * API is used to check if the initialize function will execute correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode;
    xTaskCreate_fake.return_val = pdPASS;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Initialize(&handleBlock, &dummyRunFunction, (const char*) "abc", 0, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(&dummyRunFunction, handleBlock.runFunction);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskDeinitializeInvalidParam1)
{
    /** @testcase{ guardedTask::GuardedTaskDeinitializeInvalidParam1: }
     *
     * API is used to check if the deinitialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T* handle = NULL;
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Deinitialize(handle);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskDeinitializeSuccess)
{
    /** @testcase{ guardedTask::GuardedTaskDeinitializeSuccess: }
     *
     * API is used to check if the deinitialize function will execute correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode;
    handleBlock.task = (TaskHandle_t) 1;
    handleBlock.signal = (SemaphoreHandle_t) 1;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Deinitialize(&handleBlock);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), vTaskDelete_fake.call_count);
    EXPECT_EQ(UINT32_C(1), vQueueDelete_fake.call_count);
    EXPECT_EQ(NULL, handleBlock.runFunction);
    EXPECT_EQ(NULL, handleBlock.signal);
    EXPECT_EQ(NULL, handleBlock.task);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskSignalInvalidParam1)
{
    /** @testcase{ guardedTask::GuardedTaskSignalInvalidParam1: }
     *
     * API is used to check if the signal function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T* handle = NULL;
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Signal(handle);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskSignalMultipleGives)
{
    /** @testcase{ guardedTask::GuardedTaskSignalMultipleGives: }
     *
     * API is used to check if the signal function will handle multiple gives correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode1;
    Retcode_T retcode2;
    xSemaphoreGive_fake.return_val = pdPASS;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode1 = GuardedTask_Signal(&handleBlock);

    xSemaphoreGive_fake.return_val = pdFAIL;
    retcode2 = GuardedTask_Signal(&handleBlock);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode1);
    EXPECT_EQ(RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN, Retcode_GetCode(retcode2));
    EXPECT_EQ(RETCODE_SEVERITY_WARNING, Retcode_GetSeverity(retcode2));
    EXPECT_EQ(UINT32_C(2), xSemaphoreGive_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskSignalSuccess)
{
    /** @testcase{ guardedTask::GuardedTaskSignalSuccess: }
     *
     * API is used to check if the signal function will execute correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode;
    handleBlock.signal = (SemaphoreHandle_t) 1;
    xSemaphoreGive_fake.return_val = pdPASS;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_Signal(&handleBlock);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskSignalFromIsrInvalidParam1)
{
    /** @testcase{ guardedTask::GuardedTaskSignalFromIsrInvalidParam1: }
     *
     * API is used to check if the signalFromIsr function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T* handle = NULL;
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_SignalFromIsr(handle);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(UINT32_C(0), xSemaphoreGiveFromISR_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskSignalFromIsrMultipleGives)
{
    /** @testcase{ guardedTask::GuardedTaskSignalFromIsrMultipleGives: }
     *
     * API is used to check if the signalFromIsr function will handle mutliple gives correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode1;
    Retcode_T retcode2;
    xSemaphoreGiveFromISR_fake.return_val = pdPASS;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode1 = GuardedTask_SignalFromIsr(&handleBlock);

    xSemaphoreGiveFromISR_fake.return_val = pdFAIL;
    retcode2 = GuardedTask_SignalFromIsr(&handleBlock);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode1);
    EXPECT_EQ(RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN, Retcode_GetCode(retcode2));
    EXPECT_EQ(RETCODE_SEVERITY_WARNING, Retcode_GetSeverity(retcode2));
    EXPECT_EQ(UINT32_C(2), xSemaphoreGiveFromISR_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, GuardedTaskSignalFromIsrSuccess)
{
    /** @testcase{ guardedTask::GuardedTaskSignalFromIsrSuccess: }
     *
     * API is used to check if the signalFromISR function will execute correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    Retcode_T retcode;
    handleBlock.signal = (SemaphoreHandle_t) 1;
    xSemaphoreGiveFromISR_fake.return_val = pdPASS;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = GuardedTask_SignalFromIsr(&handleBlock);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGiveFromISR_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, guardedTaskExecuteInvalidParam1)
{
    /** @testcase{ guardedTask::guardedTaskExecuteInvalidParam1: }
     *
     * API is used to check if the guardedTaskExecute function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T* handle = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    GuardedTaskExecute(handle);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, guardedTaskExecuteTakeFail)
{
    /** @testcase{ guardedTask::guardedTaskExecuteTakeFail: }
     *
     * API is used to check if the guardedTaskExecute function will handle semaphore-take fail correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    handleBlock.runFunction = &dummyRunFunction;
    handleBlock.signal = (SemaphoreHandle_t) 1;
    xSemaphoreTake_fake.return_val = pdFAIL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    GuardedTaskExecute(&handleBlock);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(UINT32_C(0), runFunctionCalled);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(guardedTask, guardedTaskExecuteSuccess)
{
    /** @testcase{ guardedTask::guardedTaskExecuteSuccess: }
     *
     * API is used to check if the guardedTaskExecuteSuccess function will execute correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    GuardedTask_T handleBlock;
    handleBlock.runFunction = &dummyRunFunction;
    handleBlock.signal = (SemaphoreHandle_t) 1;
    xSemaphoreTake_fake.return_val = pdPASS;
    xSemaphoreGive_fake.return_val = pdPASS;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    GuardedTaskExecute(&handleBlock);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(true, runFunctionCalled);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}
/*****************************************************************************************/
#else
}
#endif /* BCDS_FEATURE_GUARDEDTASK */
