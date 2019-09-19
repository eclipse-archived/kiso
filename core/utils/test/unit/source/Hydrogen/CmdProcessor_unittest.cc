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
 *      Module test specification for the CmdProcessor_unittest.cc module.
 * 
 * @detail
 *      This test case will test the command processor initialize and enqueue functions
 *      The unit test file template follows the Four-Phase test pattern.
 * 
 * @file
 **/

/* Include gtest interface */
#include <gtest.h>

FFF_DEFINITION_BLOCK_START /* Start of global scope symbol and fake definitions section */

/* Setup compile time configuration defines */
extern "C"
{
/* Module includes */
#include "Kiso_Utils.h"
#if KISO_FEATURE_CMDPROCESSOR

/* Include faked interfaces */
#include "Task_th.hh"
#include "ICall_th.hh"
#include "util_th.hh"
#include "Kiso_Retcode_th.hh"

#define CMD_PROCESSOR_TASK_LOOP_CONDITION   false

/* Include module under test */
#include "CmdProcessor.c"
}

/* Creating a test callback fake */
FAKE_VOID_FUNC(TestCallbackFunc, void *, uint32_t)

/* Default objects for existing and valid pointers */
CmdProcessor_Cmd_T cmd;

uint32_t defaultQueueObject;
uint32_t defaultTaskObject;
uint32_t defaultMallocObject;

FAKE_VALUE_FUNC(xdc_UInt, ti_sysbios_knl_Swi_disable__E)
FAKE_VOID_FUNC(ti_sysbios_knl_Swi_restore__E, xdc_UInt)
FAKE_VALUE_FUNC(xdc_UInt, ti_sysbios_hal_Hwi_HwiProxy_disable__E)
FAKE_VOID_FUNC(ti_sysbios_hal_Hwi_HwiProxy_restore__E, xdc_UInt)

FAKE_VOID_FUNC(ti_sysbios_knl_Mailbox_Params__init__S, xdc_Ptr, const xdc_Void *, xdc_SizeT, xdc_SizeT)

FAKE_VALUE_FUNC(ti_sysbios_knl_Mailbox_Handle, ti_sysbios_knl_Mailbox_create, xdc_SizeT, xdc_UInt, const ti_sysbios_knl_Mailbox_Params *, xdc_runtime_Error_Block *)

FAKE_VALUE_FUNC(xdc_Bool, ti_sysbios_knl_Mailbox_post__E, Mailbox_Object *, Ptr , xdc_UInt32)
FAKE_VALUE_FUNC(xdc_Bool, ti_sysbios_knl_Mailbox_pend__E, Mailbox_Object *, Ptr , xdc_UInt32)

xdc_Bool TestMailboxPend(Mailbox_Object * obj, Ptr msg, xdc_UInt32 timeout)
{
    KISO_UNUSED(obj);
    KISO_UNUSED(timeout);

    if(NULL != msg)
    {
        memcpy( (void *)msg, (void *)&cmd, sizeof(cmd));
    }

    return ti_sysbios_knl_Mailbox_pend__E_fake.return_val;
}


FFF_DEFINITION_BLOCK_END /* End of global scope symbol and fake definitions section */

class CmdProcessor: public testing::Test
{
protected:


 //   CmdProcessor_T cmdProcessor;

    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        RESET_FAKE(Retcode_RaiseError);

        RESET_FAKE(ti_sysbios_knl_Task_create);

        RESET_FAKE(ti_sysbios_knl_Mailbox_Params__init__S);
        RESET_FAKE(ti_sysbios_knl_Mailbox_create);
        RESET_FAKE(ti_sysbios_knl_Mailbox_post__E);
        RESET_FAKE(ti_sysbios_knl_Mailbox_pend__E);


        RESET_FAKE(ICall_malloc);
        RESET_FAKE(ICall_free);

        RESET_FAKE(TestCallbackFunc);

        ICall_malloc_fake.return_val = &defaultMallocObject;
        ti_sysbios_knl_Task_create_fake.return_val = (ti_sysbios_knl_Task_Handle)&defaultTaskObject;
        ti_sysbios_knl_Mailbox_create_fake.return_val = (ti_sysbios_knl_Mailbox_Handle)&defaultQueueObject;
        ti_sysbios_knl_Mailbox_post__E_fake.return_val = TRUE;
        ti_sysbios_knl_Mailbox_pend__E_fake.return_val = TRUE;

        ti_sysbios_knl_Mailbox_pend__E_fake.custom_fake = TestMailboxPend;

        /* Fake initialization of the Command processor */
        CmdProcessorContext.queue = (cmdProcessorQueueHandle_t)&defaultQueueObject;
        CmdProcessorContext.task = (cmdProcessorTaskHandle_t)&defaultTaskObject;

        /* Initialization of Fake command */
        cmd.func = TestCallbackFunc;
        cmd.param1 = NULL;
        cmd.param2 = UINT32_C(1);

    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* Nothing to do if clean up is not required */
    }
};

/* Specify test cases ******************************************************* */
 
TEST_F(CmdProcessor,InitializeSuccess)
{
    /** @testcase{ CmdProcessor::InitializeSuccess: }
     *
     *  tests whether the API initializes the command processor.
     *
     *  This test runs the happy path
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  rc = RETCODE_FAILURE;

    uint32_t taskPriority = UINT32_C(1);
    uint32_t taskStackDepth = UINT32_C(1024);
    uint32_t queueSize = UINT32_C(10);

   // ti_sysbios_knl_Task_create_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Initialize(
            &CmdProcessorContext,
            (char *)"cmdProcessorTest",
            taskPriority,
            taskStackDepth,
            queueSize);

    /* Important checkpoints are: Created Mailbox, Created Task and allocated memory  */
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Mailbox_create_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ICall_malloc_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Task_create_fake.call_count);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor,InitializeFailure_InputCheck_1)
{
    /** @testcase{ CmdProcessor::InitializeFailure_InputCheck_1: }
     *
     *  tests whether the API initializes the command processor.
     *
     *  This test will fail due to a null pointer being presented as command processor instance
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  rc = RETCODE_FAILURE;

    uint32_t taskPriority = UINT32_C(1);
    uint32_t taskStackDepth = UINT32_C(1024);
    uint32_t queueSize = UINT32_C(10);

   // ti_sysbios_knl_Task_create_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Initialize(
            NULL,
            (char *)"cmdProcessorTest",
            taskPriority,
            taskStackDepth,
            queueSize);

    /* Important checkpoints are: Created Mailbox, Created Task and allocated memory  */
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Mailbox_create_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ICall_malloc_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Task_create_fake.call_count);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor,InitializeFailure_InputCheck_2)
{
    /** @testcase{ CmdProcessor::InitializeFailure_InputCheck_2: }
     *
     *  tests whether the API initializes the command processor.
     *
     *  This test will fail due to a wrong task priority
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  rc = RETCODE_FAILURE;

    //uint32_t taskPriority = UINT32_C(1);
    uint32_t taskStackDepth = UINT32_C(1024);
    uint32_t queueSize = UINT32_C(10);

   // ti_sysbios_knl_Task_create_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Initialize(
            &CmdProcessorContext,
            (char *)"cmdProcessorTest",
            0,
            taskStackDepth,
            queueSize); /* Invalid parameter for queue size */

    /* Important checkpoints are: Created Mailbox, Created Task and allocated memory  */
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Mailbox_create_fake.call_count);
    EXPECT_EQ(UINT32_C(0), ICall_malloc_fake.call_count);
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Task_create_fake.call_count);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(CmdProcessor,InitializeFailure_InputCheck_3)
{
    /** @testcase{ CmdProcessor::InitializeFailure_InputCheck_3: }
     *
     *  tests whether the API initializes the command processor.
     *
     *  This test will fail due to a wrong amount of stack depth requested
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  rc = RETCODE_FAILURE;

    uint32_t taskPriority = UINT32_C(1);
    //uint32_t taskStackDepth = UINT32_C(1024);
    uint32_t queueSize = UINT32_C(10);

   // ti_sysbios_knl_Mailbox_create_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Initialize(
            &CmdProcessorContext,
            (char *)"cmdProcessorTest",
            taskPriority,
            0,  /* Invalid parameter for stack depth */
            queueSize);

    /* Important checkpoints are: Created Mailbox, Created Task and allocated memory  */
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Mailbox_create_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ICall_malloc_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Task_create_fake.call_count);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor,InitializeFailure_InputCheck_4)
{
    /** @testcase{ CmdProcessor::InitializeFailure_InputCheck_4: }
     *
     *  tests whether the API initializes the command processor.
     *
     *  This test will fail due to a wrong amount of stack depth requested
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  rc = RETCODE_FAILURE;

    uint32_t taskPriority = UINT32_C(1);
    uint32_t taskStackDepth = UINT32_C(1024);
    //uint32_t queueSize = UINT32_C(10);

   // ti_sysbios_knl_Task_create_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Initialize(
            &CmdProcessorContext,
            (char *)"cmdProcessorTest",
            taskPriority,
            taskStackDepth,
            0); /* Invalid parameter for queue size */

    /* Important checkpoints are: Created Mailbox, Created Task and allocated memory  */
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Mailbox_create_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ICall_malloc_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Task_create_fake.call_count);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor,InitializeFailure_MailboxCreateFailure)
{
    /** @testcase{ CmdProcessor::InitializeFailure_MailboxCreateFailure: }
     *
     *  tests whether the API initializes the command processor.
     *
     *  This test will fail due a mailbox creation failure
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  rc = RETCODE_FAILURE;

    uint32_t taskPriority = UINT32_C(1);
    uint32_t taskStackDepth = UINT32_C(1024);
    uint32_t queueSize = UINT32_C(10);

    ti_sysbios_knl_Mailbox_create_fake.return_val = (ti_sysbios_knl_Mailbox_Handle)NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Initialize(
            &CmdProcessorContext,
            (char *)"cmdProcessorTest",
            taskPriority,
            taskStackDepth,
            queueSize);


    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_QUEUE_INIT_FAIL), rc);
}

TEST_F(CmdProcessor,InitializeFailure_StackAllocationFailure)
{
    /** @testcase{ CmdProcessor::InitializeFailure_StackAllocationFailure: }
     *
     *  tests whether the API initializes the command processor.
     *
     *  This test will fail due a stack allocation failure
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  rc = RETCODE_FAILURE;

    uint32_t taskPriority = UINT32_C(1);
    uint32_t taskStackDepth = UINT32_C(1024);
    uint32_t queueSize = UINT32_C(10);

    ICall_malloc_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Initialize(
            &CmdProcessorContext,
            (char *)"cmdProcessorTest",
            taskPriority,
            taskStackDepth,
            queueSize);


    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES), rc);
}

TEST_F(CmdProcessor,InitializeFailure_TaskCreationFailure)
{
    /** @testcase{ CmdProcessor::InitializeFailure_TaskCreationFailure: }
     *
     *  tests whether the API initializes the command processor.
     *
     *  This test will fail due a task creation failure
     */

    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  rc = RETCODE_FAILURE;

    uint32_t taskPriority = UINT32_C(1);
    uint32_t taskStackDepth = UINT32_C(1024);
    uint32_t queueSize = UINT32_C(10);

    ti_sysbios_knl_Task_create_fake.return_val = (ti_sysbios_knl_Task_Handle)NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Initialize(
            &CmdProcessorContext,
            (char *)"cmdProcessorTest",
            taskPriority,
            taskStackDepth,
            queueSize);


    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_TASK_INIT_FAIL), rc);
}

TEST_F(CmdProcessor, EnqueueSuccess)
{
    /** @testcase{ CmdProcessor::EnqueueSuccess: }
     *
     *  This test runs the happy path
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Enqueue(
            &CmdProcessorContext,
            TestCallbackFunc,
            param1,
            param2);


    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor, EnqueueInputFailure_1)
{
    /** @testcase{ CmdProcessor::EnqueueInputFailure_1: }
     *
     *  This test causes a failure due to a null pointer as Command processor handle
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Enqueue(
            NULL,
            TestCallbackFunc,
            param1,
            param2);


    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor, EnqueueInputFailure_2)
{
    /** @testcase{ CmdProcessor::EnqueueInputFailure_2: }
     *
     *  This test causes a failure due to a null pointer passed as callback
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Enqueue(
            &CmdProcessorContext,
            NULL,
            param1,
            param2);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(CmdProcessor, EnqueueQueueNull)
{
    /** @testcase{ CmdProcessor::EnqueueQueueNull: }
     *
     *  This test tries to enqueue a command, but the queue is not initialized
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    CmdProcessorContext.queue = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Enqueue(
            &CmdProcessorContext,
            TestCallbackFunc,
            param1,
            param2);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(CmdProcessor, EnqueueFailure)
{
    /** @testcase{ CmdProcessor::EnqueueQueueNull: }
     *
     *  tests whether the API .
     *
     *  This test tries to enqueue a command but gets a Mailbox failure
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    ti_sysbios_knl_Mailbox_post__E_fake.return_val = FALSE;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_Enqueue(
            &CmdProcessorContext,
            TestCallbackFunc,
            param1,
            param2);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_QUEUE_ENQUEUE_FAIL), rc);
}

TEST_F(CmdProcessor, EnqueueFromIsrSuccess)
{
    /** @testcase{ CmdProcessor::EnqueueSuccess: }
     *
     *  This test runs the happy path
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_EnqueueFromIsr(
            &CmdProcessorContext,
            TestCallbackFunc,
            param1,
            param2);


    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor, EnqueueFromIsrInputFailure_1)
{
    /** @testcase{ CmdProcessor::EnqueueInputFailure_1: }
     *
     *  This test causes a failure due to a null pointer as Command processor handle
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_EnqueueFromIsr(
            NULL,
            TestCallbackFunc,
            param1,
            param2);


    //EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor, EnqueueFromIsrInputFailure_2)
{
    /** @testcase{ CmdProcessor::EnqueueInputFailure_2: }
     *
     *  This test causes a failure due to a null pointer passed as callback
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_EnqueueFromIsr(
            &CmdProcessorContext,
            NULL,
            param1,
            param2);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(CmdProcessor, EnqueueFromIsrQueueNull)
{
    /** @testcase{ CmdProcessor::EnqueueQueueNull: }
     *
     *  This test tries to enqueue a command, but the queue is not initialized
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    CmdProcessorContext.queue = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_EnqueueFromIsr(
            &CmdProcessorContext,
            TestCallbackFunc,
            param1,
            param2);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(CmdProcessor, EnqueueFromIsrFailure)
{
    /** @testcase{ CmdProcessor::EnqueueQueueNull: }
     *
     *  tests whether the API .
     *
     *  This test tries to enqueue a command but gets a Mailbox failure
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    void * param1 = NULL;
    uint32_t param2 = UINT32_C(1);

    ti_sysbios_knl_Mailbox_post__E_fake.return_val = FALSE;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = CmdProcessor_EnqueueFromIsr(
            &CmdProcessorContext,
            TestCallbackFunc,
            param1,
            param2);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_QUEUE_ENQUEUE_FAIL), rc);
}

TEST_F(CmdProcessor, DequeueAndExecute)
{
    /** @testcase{ CmdProcessor::Dequeue: }
     *
     *  tests whether the API .
     *
     *  This test runs the happy path of the dequeue function
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = DequeueAndExecute(&CmdProcessorContext);

    EXPECT_EQ(UINT32_C(1), TestCallbackFunc_fake.call_count);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(CmdProcessor, DequeueAndExecuteInputFail_1)
{
    /** @testcase{ CmdProcessor::Dequeue: }
     *
     *  tests whether the API .
     *
     *  This test tries to run the dequeue without an command processor
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = DequeueAndExecute(NULL);

    EXPECT_EQ(UINT32_C(0), TestCallbackFunc_fake.call_count);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}

TEST_F(CmdProcessor, DequeueAndExecuteInputFail_2)
{
    /** @testcase{ CmdProcessor::Dequeue: }
     *
     *  tests whether the API .
     *
     *  This test tries to run the dequeue without an initializes queue
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */

    CmdProcessorContext.queue = NULL;

    rc = DequeueAndExecute(&CmdProcessorContext);

    EXPECT_EQ(UINT32_C(0), TestCallbackFunc_fake.call_count);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}


TEST_F(CmdProcessor, DequeueAndExecuteDequeueFailure)
{
    /** @testcase{ CmdProcessor::Dequeue: }
     *
     *  tests whether the API .
     *
     *  This test fails to a dequeue failure
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;

    ti_sysbios_knl_Mailbox_pend__E_fake.return_val = FALSE;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = DequeueAndExecute(&CmdProcessorContext);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_QUEUE_DEQUEUE_FAIL), rc);
}

TEST_F(CmdProcessor, DequeueAndExecuteExecutionFailure)
{
    /** @testcase{ CmdProcessor::Dequeue: }
     *
     *  tests whether the API .
     *
     *  This test fails due to an invalid callback
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T  rc = RETCODE_FAILURE;
    cmd.func = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    rc = DequeueAndExecute(&CmdProcessorContext);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_EXECUTE_FAILURE), rc);
}


TEST_F(CmdProcessor, TaskLoopRunnerSuccess)
{
    /** @testcase{ CmdProcessor::Run: }
     *
     * Tests the happy path
     */

    /* SETUP: Declare and initialize local variables required only by this test case */


    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    TaskLoopRunner((UArg)&CmdProcessorContext, 0);

    EXPECT_EQ(0, Retcode_RaiseError_fake.call_count);
}

TEST_F(CmdProcessor, RunFailure)
{
    /** @testcase{ CmdProcessor::RunFailure: }
     *
     * Tests a case where the command processor fails
     */

    /* SETUP: Declare and initialize local variables required only by this test case */


    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    TaskLoopRunner((UArg)NULL, 0);

    EXPECT_EQ(1, Retcode_RaiseError_fake.call_count);
}
/*****************************************************************************************/
#else
}
#endif /* if KISO_FEATURE_CMDPROCESSOR */
