/******************************************************************************/
/**
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 *
 *  @file
 *
 *  Module test specification for the CmdProcessor module.
 *
 * The unit test file template follows the Four-Phase test pattern. ...
 *
 * ****************************************************************************/

/* include gtest interface */
#include <gtest.h>

extern "C"
{/* start of global scope symbol and fake definitions section */

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_CMDPROCESSOR

#if BCDS_FEATURE_CMDPROCESSOR

/* include faked interfaces */
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"

#include "task_th.hh"
#include "queue_th.hh"

/* include module under test */
#include "CmdProcessor.c"

} /* end of global scope symbol and fake definitions section */

/* setup compile time configuration defines */
#define TASK_PRIORITY                   UINT32_C(1)     /**< Task Priority should be less then timer task priority */
#define STACK_SIZE                      UINT32_C(256)   /**< stack size of the task */
#define QUEUE_SIZE                      UINT32_C(3)     /**< size of the queue.Holds command processor command structure variables */
#define INIT_VAL                        UINT32_C(2)     /**< Used as an initial value to variables */
#define OS_ERROR                        UINT32_C(1)     /**< Used as a return value to an error */
#define QUEUE_CREATED                   UINT32_C(1)     /**< Used as a return value for successful queue creation */

static CmdProcessor_Cmd_T cmd;

/* Fake function created to pass as a argument to the  cmd_Enqueue function*/
void dump_fn(void *, uint32_t)
{

}

signed long myXQueueReceive(QueueHandle_t xQueue, void * pvBuffer, TickType_t xTicksToWait)
{
    BCDS_UNUSED(xQueue);

    EXPECT_EQ(portMAX_DELAY, xTicksToWait);

    *((CmdProcessor_Cmd_T*)pvBuffer) = cmd;

    return pdPASS;
}

class cmdProcessor: public testing::Test
{
protected:

    virtual void SetUp()
    {
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(xQueueCreate);
        RESET_FAKE(xQueueSend);
        RESET_FAKE(xQueueSendFromISR);
        RESET_FAKE(vTaskSuspend);
        RESET_FAKE(vTaskResume);
        RESET_FAKE(xQueueReceive);
        RESET_FAKE(Retcode_RaiseError);

        memset(&cmd, 0, sizeof(CmdProcessor_Cmd_T));

        FFF_RESET_HISTORY()
        ;
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

/* specify test cases ******************************************************* */

/**
 *  Module is used to execute the function from the queue and it is provided by the apis.
 *  It contains the overall test cases to test the queue creation and task creation by the defined apis in C module
 */
typedef cmdProcessor cmdProcessor_deathtest;

TEST_F(cmdProcessor, cmdProcessorIntializationQueueFail)
{
    /** @testcase{ cmdProcessor::cmdProcessorIntializationQueueFail: }
     * if the queue is not created then it returns an error.on successful creation of queue  it creates a task.
     * API is used to check the queue creation.
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    uint32_t taskPriority = TASK_PRIORITY;
    uint32_t taskStackDepth = STACK_SIZE;
    uint32_t queueSize = QUEUE_SIZE;
    Retcode_T retVal = RETCODE_OK;
    xQueueCreate_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_Initialize(&cmdProcessor, (char*) "abc", taskPriority, taskStackDepth, queueSize);

    /* VERIFY : Compare the expected with actual */
    EXPECT_NE(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xQueueCreate_fake.call_count);
    EXPECT_EQ(NULL, xQueueCreate_fake.return_val);
}

TEST_F(cmdProcessor, cmdProcessorIntializationTaskFail)
{
    /** @testcase{ cmdProcessor::cmdProcessorIntializationTaskFail: }
     * API is used to check the task creation failure part in the initialization function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    uint32_t taskPriority = TASK_PRIORITY;
    uint32_t taskStackDepth = STACK_SIZE;
    uint32_t queueSize = QUEUE_SIZE;
    Retcode_T retVal = RETCODE_OK;
    xQueueCreate_fake.return_val = (void *) QUEUE_CREATED;
    xTaskCreate_fake.return_val = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_Initialize(&cmdProcessor, (char *) "abc", taskPriority, taskStackDepth, queueSize);

    /* VERIFY : Compare the expected with actual */
    EXPECT_NE(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(1), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xQueueCreate_fake.call_count);
}

TEST_F(cmdProcessor, cmdProcessorIntializationTaskSuccess)
{
    /** @testcase{ cmdProcessor::cmdProcessorIntializationTaskSuccess: }
     * API is used to check the task and queue creation in the initialization function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    uint32_t taskPriority = TASK_PRIORITY;
    uint32_t taskStackDepth = STACK_SIZE;
    uint32_t queueSize = QUEUE_SIZE;
    Retcode_T retVal = RETCODE_FAILURE;
    xTaskCreate_fake.return_val = pdTRUE;
    xQueueCreate_fake.return_val = (void *) QUEUE_CREATED;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_Initialize(&cmdProcessor, (char *) "abc", taskPriority, taskStackDepth, queueSize);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(1), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xQueueCreate_fake.call_count);
}

TEST_F(cmdProcessor, cmdProcessorIntializationNullParam)
{
    /** @testcase{ cmdProcessor::cmdProcessorIntializationNullParam: }
     * API is used to check the parameter checking in the initialization function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    uint32_t taskPriority = TASK_PRIORITY;
    uint32_t taskStackDepth = STACK_SIZE;
    uint32_t queueSize = QUEUE_SIZE;
    Retcode_T retVal = RETCODE_FAILURE;
    xTaskCreate_fake.return_val = pdTRUE;
    xQueueCreate_fake.return_val = (void *) QUEUE_CREATED;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_Initialize(NULL, (char *) "abc", taskPriority, taskStackDepth, queueSize);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retVal);
    EXPECT_EQ(UINT32_C(0), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xQueueCreate_fake.call_count);
}

TEST_F(cmdProcessor, cmdProcessorTaskNameExceedsLength)
{
    /** @testcase{ cmdProcessor::cmdProcessorTaskNameExceedsLength: }
     * API is used to check task name length
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    uint32_t taskPriority = TASK_PRIORITY;
    uint32_t taskStackDepth = STACK_SIZE;
    uint32_t queueSize = QUEUE_SIZE;
    char taskName[] = "abcdefghijklmnopqrstuvwxya123456789";
    char truncatedTaskName[] = "abcdefghijklmnopqrstuvwxya12345";
    Retcode_T retVal = RETCODE_FAILURE;
    xTaskCreate_fake.return_val = pdTRUE;
    xQueueCreate_fake.return_val = (void *) QUEUE_CREATED;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_Initialize(&cmdProcessor, taskName, taskPriority, taskStackDepth, queueSize);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);

    // xTaskCreate( pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask )

    ASSERT_EQ(INT32_C(0), strcmp((char * )xTaskCreate_fake.arg1_val, truncatedTaskName));
    EXPECT_EQ(UINT32_C(1), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xQueueCreate_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorEnqueueFromISRFail)
{
    /** @testcase{ cmdProcessor::CmdProcessorEnqueueFromISRFail: }
     * API is used to check the Command processor enqueue from ISR fail because of full queue condition
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    uint32_t param2 = INIT_VAL;
    Retcode_T retVal = RETCODE_OK;
    CmdProcessor_Func_T fun = dump_fn;
    xQueueSendFromISR_fake.return_val = errQUEUE_FULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_EnqueueFromIsr(&cmdProcessor, fun, NULL, param2);

    /* VERIFY : Compare the expected with actual */
    EXPECT_NE(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(1), xQueueSendFromISR_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorEnqueueFromISRNullParam)
{
    /** @testcase{ cmdProcessor::CmdProcessorEnqueueFromISRFail: }
     * API is used to check the Command processor enqueue from ISR fail because of parameter NULL
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    uint32_t param2 = INIT_VAL;
    Retcode_T retVal = RETCODE_OK;
    CmdProcessor_Func_T fun = dump_fn;
    xQueueSendFromISR_fake.return_val = errQUEUE_FULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_EnqueueFromIsr(NULL, fun, NULL, param2);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retVal);
    EXPECT_EQ(UINT32_C(0), xQueueSendFromISR_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorEnqueueFromISRSuccess)
{
    /** @testcase{ cmdProcessor::CmdProcessorEnqueueFromISRSuccess: }
     * API is used to check the Command processor enqueue from ISR success condition
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    uint32_t param2 = INIT_VAL;
    Retcode_T retVal = RETCODE_OK;
    CmdProcessor_Func_T fun = dump_fn;
    xQueueSendFromISR_fake.return_val = pdPASS;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_EnqueueFromIsr(&cmdProcessor, fun, NULL, param2);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_LT(UINT32_C(0), xQueueSendFromISR_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorEnqueueError)
{
    /** @testcase{ cmdProcessor::CmdProcessorEnqueueError: }
     * API is used to check the Command processor enqueue fail condition
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    uint32_t param2 = INIT_VAL;
    Retcode_T retVal = RETCODE_OK;
    CmdProcessor_Func_T fun = dump_fn;
    xQueueSend_fake.return_val = errQUEUE_FULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_Enqueue(&cmdProcessor, fun, NULL, param2);

    /* VERIFY : Compare the expected with actual */
    EXPECT_NE(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(1), xQueueSend_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorEnqueueNullParam)
{
    /** @testcase{ cmdProcessor::CmdProcessorEnqueueNullParam: }
     * API is used to check the Command processor enqueue parameter checking
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    uint32_t param2 = INIT_VAL;
    Retcode_T retVal = RETCODE_OK;
    CmdProcessor_Func_T fun = dump_fn;
    xQueueSend_fake.return_val = errQUEUE_FULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_Enqueue(NULL, fun, NULL, param2);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retVal);
    EXPECT_EQ(UINT32_C(0), xQueueSend_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorEnqueueSuccess)
{
    /** @testcase{ cmdProcessor::CmdProcessorEnqueueSuccess: }
     * API is used to check the Command processor enqueue success condition
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    uint32_t param2 = INIT_VAL;
    Retcode_T retVal = RETCODE_OK;
    CmdProcessor_Func_T fun = dump_fn;
    xQueueSend_fake.return_val = pdPASS;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CmdProcessor_Enqueue(&cmdProcessor, fun, NULL, param2);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(1), xQueueSend_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorSuspendNullParam)
{
    /** @testcase{ cmdProcessor::CmdProcessorResume: }
     * API used to check param checking in the Command processor suspend function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    CmdProcessor_Suspend(NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(0), vTaskResume_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorSuspend)
{
    /** @testcase{ cmdProcessor::CmdProcessorSuspend: }
     * API is used to check the Command processor suspend function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    CmdProcessor_Suspend(&cmdProcessor);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), vTaskSuspend_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorResumeNullParam)
{
    /** @testcase{ cmdProcessor::CmdProcessorResume: }
     * API used to check param checking in the Command processor resume function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    CmdProcessor_Resume(NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(0), vTaskResume_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorResume)
{
    /** @testcase{ cmdProcessor::CmdProcessorResume: }
     * API used to check the Command processor resume function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    CmdProcessor_Resume(&cmdProcessor);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), vTaskResume_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorDequeueSuccess)
{
    /** @testcase{ cmdProcessor::CmdProcessorDequeueSuccess: }
     * API used to check the Command processor dequeue function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    xQueueReceive_fake.custom_fake = &myXQueueReceive;
    cmd.func = &dump_fn;
    cmd.param1 = NULL;
    cmd.param2 = UINT32_C(0);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    Dequeue(&cmdProcessor);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), xQueueReceive_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorDequeueFail)
{
    /** @testcase{ cmdProcessor::CmdProcessorDequeueSuccess: }
     * API used to check the Command processor dequeue function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    xQueueReceive_fake.custom_fake = &myXQueueReceive;
    cmd.func = NULL;
    cmd.param1 = NULL;
    cmd.param2 = UINT32_C(0);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    Dequeue(&cmdProcessor);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), xQueueReceive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorDequeueReceiveFail)
{
    /** @testcase{ cmdProcessor::CmdProcessorDequeueReceiveFail: }
     * API used to check the Command processor dequeue function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor;
    xQueueReceive_fake.return_val = pdFAIL;
    cmd.func = NULL;
    cmd.param1 = NULL;
    cmd.param2 = UINT32_C(0);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    Dequeue(&cmdProcessor);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), xQueueReceive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
}

TEST_F(cmdProcessor, CmdProcessorDequeueCmdPrcsrFail)
{
    /** @testcase{ cmdProcessor::CmdProcessorDequeueCmdPrcsrFail: }
     * API used to check the Command processor dequeue function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    cmd.func = NULL;
    cmd.param1 = NULL;
    cmd.param2 = UINT32_C(0);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    Dequeue(NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
}


TEST_F(cmdProcessor, CmdProcessorDequeueCmdPrcsrQueueFail)
{
    /** @testcase{ cmdProcessor::CmdProcessorDequeueCmdPrcsrQueueFail: }
     * API used to check the Command processor dequeue function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    CmdProcessor_T cmdProcessor = {NULL,NULL, 0};
    cmd.func = NULL;
    cmd.param1 = NULL;
    cmd.param2 = UINT32_C(0);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    Dequeue(&cmdProcessor);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
}
TEST_F(cmdProcessor,DeprecatedInitialize)
{
    Retcode_T retval = RETCODE_OK;
    retval = CmdProcessor_initialize(NULL,NULL,1,1,1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_INVALID_PARAM), retval);
}
TEST_F(cmdProcessor,DeprecatedEnqueue)
{
    Retcode_T retval = RETCODE_OK;
    retval = CmdProcessor_enqueue(NULL,NULL,NULL,1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_INVALID_PARAM), retval);
}
TEST_F(cmdProcessor,DeprecatedEnqueueFromIsr)
{
    Retcode_T retval = RETCODE_OK;
    retval = CmdProcessor_enqueueFromIsr(NULL,NULL,NULL,1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_INVALID_PARAM), retval);
}
TEST_F(cmdProcessor,DeprecatedSuspend)
{
    CmdProcessor_suspend(NULL);
}
TEST_F(cmdProcessor,DeprecatedResume)
{
    CmdProcessor_resume(NULL);
}

#else
}
#endif /* if BCDS_FEATURE_CMDPROCESSOR */
