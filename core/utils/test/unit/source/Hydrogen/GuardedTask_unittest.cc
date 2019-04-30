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
 *  Module test specification for the CmdProcessor_cc_unittest module.
 * this test case will test the command processor initialize and enqueue functions
 * The unit test file template follows the Four-Phase test pattern. 
 *
 * ****************************************************************************/

/* include gtest interface */
#include <gtest.h>

FFF_DEFINITION_BLOCK_START /* start of global scope symbol and fake definitions section */


/* setup compile time configuration defines */
extern "C"
{
#include "BCDS_Utils.h"
#if BCDS_FEATURE_GUARDEDTASK

/* include faked interfaces */
#include "Task_th.hh"
#include "Semaphore_th.hh"
#include "ICall_th.hh"
#include "util_th.hh"
#include "BCDS_Retcode_th.hh"

/* include module under test */
#undef while
#define while(N)

#include "GuardedTask.c"

}

void fun(void)
{
    uint8_t temp = 5;
    BCDS_UNUSED(temp);
}

ti_sysbios_knl_Task_Handle ti_sysbios_knl_Task_create_customfake(ti_sysbios_knl_Task_FuncPtr fxn, const ti_sysbios_knl_Task_Params *__paramsPtr, xdc_runtime_Error_Block *__eb )
{
    BCDS_UNUSED(__paramsPtr);
    BCDS_UNUSED(__eb);
    fxn((UArg)0,(UArg)0);

    return (ti_sysbios_knl_Task_Handle)(0xAA);
}

void ti_sysbios_knl_Semaphore_post__E_customfake(ti_sysbios_knl_Semaphore_Handle a)
{
    BCDS_UNUSED(a);
    GuardedTaskRunFunction(0,0);
}

Retcode_T Retcode_compose_customFake(uint32_t package, Retcode_Severity_T severity, uint32_t code)
{
    uint32_t p = (package & 0x000000FF) << 24;
    uint32_t s = ((uint32_t) severity & 0x000000FF) << 16;
    uint32_t c = (code & 0x0000FFFF);
    Retcode_T retcode = (code == UINT32_C(0)) ? (Retcode_T) UINT32_C(0) : (Retcode_T) (p | s | c);

    return (retcode);
}

FFF_DEFINITION_BLOCK_END /* end of global scope symbol and fake definitions section */

class GuardedTask: public testing::Test
{
protected:

    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
        RESET_FAKE(ti_sysbios_knl_Semaphore_create);
        RESET_FAKE(ti_sysbios_knl_Task_create);
        RESET_FAKE(ti_sysbios_knl_Semaphore_delete);
        RESET_FAKE(ti_sysbios_knl_Task_delete);
        RESET_FAKE(ICall_malloc);
        RESET_FAKE(ICall_free);
        RESET_FAKE(ti_sysbios_knl_Semaphore_pend__E);

    }

    virtual void TearDown()
    {

        ; /* nothing to do if clean up is not required */
    }
};
 
TEST_F(GuardedTask,GuardedTaskinitializeFail)
{
    /** @testcase{ GuardedTask::GuardedTaskinitialize: }
     *
     *  tests whether the API initialize the GuardedTaskinitialize.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    uint32_t taskPriority =UINT32_C(1);
    uint32_t taskStackDepth=UINT32_C(0);
    ti_sysbios_knl_Task_create_fake.return_val = NULL;
    ti_sysbios_knl_Semaphore_create_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Initialize( NULL, NULL,(char *)"GuardedTest", taskPriority, taskStackDepth);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Task_create_fake.call_count);
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Semaphore_create_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_INVALID_PARAM), retval);
}

TEST_F(GuardedTask,GuardedTaskinitializeFAIL1)
{
    /** @testcase{ GuardedTask::GuardedTaskinitialize: }
     *
     *  tests whether the API initialize the GuardedTaskinitialize.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    uint32_t taskPriority =UINT32_C(1);
    uint32_t taskStackDepth=UINT32_C(644);
    ti_sysbios_knl_Task_create_fake.return_val = NULL;
    ti_sysbios_knl_Semaphore_create_fake.return_val = NULL;


    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Initialize( NULL, fun, (char *)"GuardedTest", taskPriority, taskStackDepth);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Task_create_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Semaphore_create_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_OUT_OF_RESOURCES), retval);
}

TEST_F(GuardedTask,GuardedTaskinitialize)
{
    /** @testcase{ GuardedTask::GuardedTaskinitialize: }
     *
     *  tests whether the API initialize the GuardedTaskinitialize.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    uint32_t taskPriority =UINT32_C(1);
    uint32_t taskStackDepth=UINT32_C(644);
    uint32_t handle = UINT32_C(1);
    ti_sysbios_knl_Semaphore_pend__E_fake.return_val = TRUE;
    ti_sysbios_knl_Task_create_fake.custom_fake = ti_sysbios_knl_Task_create_customfake;
    ti_sysbios_knl_Semaphore_create_fake.return_val = (ti_sysbios_knl_Semaphore_Handle)handle;


    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Initialize( NULL, fun, (char *)"GuardedTest", taskPriority, taskStackDepth);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Task_create_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Semaphore_create_fake.call_count);
    EXPECT_EQ(RETCODE_OK, retval);
}
/*****************************************************************************************/

TEST_F(GuardedTask,GuardedTaskDeinitializeFail)
{
    /** @testcase{ GuardedTask::GuardedTaskDeinitialize: }
     *
     *  tests whether the API initialize the GuardedTaskDeinitialize.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    TaskHandle = NULL;
    SemHandle = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Deinitialize( NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Task_delete_fake.call_count);
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Semaphore_delete_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_NULL_POINTER), retval);
}

TEST_F(GuardedTask,GuardedTaskDeinitializeFail1)
{
    /** @testcase{ GuardedTask::GuardedTaskDeinitialize: }
     *
     *  tests whether the API initialize the GuardedTaskDeinitialize.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    uint32_t handle = UINT32_C(1);
    TaskHandle = NULL;
    SemHandle = (ti_sysbios_knl_Semaphore_Handle)handle;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Deinitialize( NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Task_delete_fake.call_count);
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Semaphore_delete_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_NULL_POINTER), retval);
}
TEST_F(GuardedTask,GuardedTaskDeinitializeFail2)
{
    /** @testcase{ GuardedTask::GuardedTaskDeinitialize: }
     *
     *  tests whether the API initialize the GuardedTaskDeinitialize.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    uint32_t handle = UINT32_C(1);
    TaskHandle = (ti_sysbios_knl_Task_Handle)handle;
    SemHandle = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Deinitialize( NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Task_delete_fake.call_count);
    EXPECT_EQ(UINT32_C(0), ti_sysbios_knl_Semaphore_delete_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_NULL_POINTER), retval);
}

TEST_F(GuardedTask,GuardedTaskDeinitialize)
{
    /** @testcase{ GuardedTask::GuardedTaskDeinitialize: }
     *
     *  tests whether the API initialize the GuardedTaskDeinitialize.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    uint32_t handle = UINT32_C(1);
    TaskHandle = (ti_sysbios_knl_Task_Handle)handle;
    SemHandle = (ti_sysbios_knl_Semaphore_Handle)handle;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Deinitialize( NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Task_delete_fake.call_count);
    EXPECT_EQ(UINT32_C(1), ti_sysbios_knl_Semaphore_delete_fake.call_count);
    EXPECT_EQ(RETCODE_OK, retval);
}

/*****************************************************************************************/
TEST_F(GuardedTask,GuardedTaskSignalFAIL)
{
    /** @testcase{ GuardedTask::GuardedTaskSignal: }
     *
     *  tests whether the API initialize the GuardedTaskSignal.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    RunFunction = NULL;
    SemHandle = NULL;
    ti_sysbios_knl_Semaphore_post__E_fake.custom_fake = ti_sysbios_knl_Semaphore_post__E_customfake;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Signal( NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_INVALID_PARAM), retval);
}

TEST_F(GuardedTask,GuardedTaskSignal)
{
    /** @testcase{ GuardedTask::GuardedTaskSignal: }
     *
     *  tests whether the API initialize the GuardedTaskSignal.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    Retcode_T  retval = RETCODE_FAILURE;
    uint32_t handle = UINT32_C(1);
    RunFunction = NULL;
    SemHandle = (ti_sysbios_knl_Semaphore_Handle)handle;
    ti_sysbios_knl_Semaphore_post__E_fake.custom_fake = ti_sysbios_knl_Semaphore_post__E_customfake;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retval = GuardedTask_Signal( NULL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retval);
}
TEST_F(GuardedTask,DeprecatedDeinit)
{
    Retcode_T  retval = RETCODE_FAILURE;
    TaskHandle = NULL;
    retval = GuardedTask_deinitialize(NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_NULL_POINTER), retval);
}
TEST_F(GuardedTask,DeprecatedInit)
{
    Retcode_T  retval = RETCODE_FAILURE;
    retval = GuardedTask_initialize(NULL,NULL,NULL,1,1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_INVALID_PARAM), retval);
}
TEST_F(GuardedTask,DeprecatedSignal)
{
    Retcode_T  retval = RETCODE_FAILURE;
    SemHandle = NULL;
    retval = GuardedTask_signal(NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t ) RETCODE_INVALID_PARAM), retval);

    RESET_FAKE(Retcode_RaiseError);
    UArg pvParameters1 = NULL;
    GuardedTaskRunFunction(pvParameters1,pvParameters1);
    SemHandle = NULL;
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
}
/*****************************************************************************************/
#else
}
#endif /* if BCDS_FEATURE_GUARDEDTASK */
