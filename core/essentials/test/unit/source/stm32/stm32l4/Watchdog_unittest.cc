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

/* include gtest interface */
#include <gtest.h>

extern "C"
{/* start of global scope symbol and fake definitions section */

#include "BCDS_Basics.h"

#if BCDS_FEATURE_WATCHDOG
/* include faked interfaces */
#include "BCDS_HAL_th.hh"
#include "stm32l4xx_th.hh"
#include "stm32l4xx_hal_iwdg_th.hh"
#include "stm32l4xx_hal_rcc_th.hh"

/* include module under test */
#include "Watchdog.c"

}

class STM32_WATCHDOG_TEST: public testing::Test
{
protected:

    /* Watch-dog handle created at board initialization. */
    IWDG_HandleTypeDef testWatchogHandle;

    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        RESET_FAKE (HAL_IWDG_Init);
        RESET_FAKE (HAL_IWDG_Refresh);
        RESET_FAKE (__HAL_RCC_GET_FLAG);
        RESET_FAKE (__HAL_RCC_CLEAR_RESET_FLAGS);
    }

    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

/* specify test cases ******************************************************* */

/* ***************************************************************************************** */
/* MCU_Watchdog_Init() 																	     */
/* ***************************************************************************************** */

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_Init_success)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_Init_success: }
     * Successful MCU_Watchdog_Init() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WdgHandle_ptr = NULL;
    WdgHandle_T wdgHandle = &testWatchogHandle;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Init(wdgHandle);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(&testWatchogHandle, WdgHandle_ptr);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    WdgHandle_ptr = NULL;
}

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_Init_failure_invalid_param)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_Init_failure_invalid_param: }
     * MCU_Watchdog_Init() failing test case.
     */

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Init(NULL);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_FATAL, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

/* ***************************************************************************************** */
/* MCU_Watchdog_Enable() 																	 */
/* ***************************************************************************************** */

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_Enable_success)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_Enable_success: }
     * Successful MCU_Watchdog_Enable() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WdgHandle_ptr = &testWatchogHandle;
    HAL_IWDG_Init_fake.return_val = HAL_OK;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Enable();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), HAL_IWDG_Init_fake.call_count);
    EXPECT_EQ(WdgHandle_ptr, HAL_IWDG_Init_fake.arg0_val);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_IWDG_Init);
    WdgHandle_ptr = NULL;
}

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_Enable_failure_uninitializedState)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_Enable_failure_uninitializedState: }
     * MCU_Watchdog_Enable() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WdgHandle_ptr = NULL;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Enable();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_FATAL, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(retcode));
}

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_Enable_errorPropagationTest)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_Enable_errorPropagationTest: }
     * Error propagation test when the APIs called from within MCU_Watchdog_Enable return an error
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WdgHandle_ptr = &testWatchogHandle;
    HAL_IWDG_Init_fake.return_val = HAL_ERROR;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Enable();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_FATAL, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_IWDG_Init);
    WdgHandle_ptr = NULL;
}

/* ***************************************************************************************** */
/* MCU_Watchdog_Feed() 																	     */
/* ***************************************************************************************** */

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_Feed_success)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_Feed_success: }
     * Successful MCU_Watchdog_Feed() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WdgHandle_ptr = &testWatchogHandle;
    HAL_IWDG_Refresh_fake.return_val = HAL_OK;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Feed();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), HAL_IWDG_Refresh_fake.call_count);
    EXPECT_EQ(WdgHandle_ptr, HAL_IWDG_Refresh_fake.arg0_val);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_IWDG_Refresh);
    WdgHandle_ptr = NULL;
}

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_Feed_failure_uninitializedState)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_Feed_failure_uninitializedState: }
     * MCU_Watchdog_Feed() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WdgHandle_ptr = NULL;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Feed();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_FATAL, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(retcode));
}

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_Feed_errorPropagationTest)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_Feed_errorPropagationTest: }
     * Error propagation test when the APIs called from within MCU_Watchdog_Feed return an error
     */

    WdgHandle_ptr = &testWatchogHandle;
    HAL_IWDG_Refresh_fake.return_val = HAL_ERROR;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Feed();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_FATAL, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_IWDG_Refresh);
    WdgHandle_ptr = NULL;
}

/* ***************************************************************************************** */
/* MCU_Watchdog_IsResetCausedByWatchdog() 											         */
/* ***************************************************************************************** */

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_IsResetCausedByWatchdog_success_dueToReset)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_IsResetCausedByWatchdog_success_dueToReset: }
     * Successful MCU_Watchdog_IsResetCausedByWatchdog() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    __HAL_RCC_GET_FLAG_fake.return_val = RESET;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    bool retcode = MCU_Watchdog_IsResetCausedByWatchdog();
    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_FALSE(retcode);
    EXPECT_EQ(UINT32_C(1), __HAL_RCC_GET_FLAG_fake.call_count);
    EXPECT_EQ(UINT32_C(0), __HAL_RCC_CLEAR_RESET_FLAGS_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (__HAL_RCC_GET_FLAG);
}

TEST_F(STM32_WATCHDOG_TEST, MCU_Watchdog_IsResetCausedByWatchdog_success_notDueToReset)
{
    /** @testcase{ STM32_WATCHDOG_TEST::MCU_Watchdog_IsResetCausedByWatchdog_success_notDueToReset: }
     * Successful MCU_Watchdog_IsResetCausedByWatchdog() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    __HAL_RCC_GET_FLAG_fake.return_val = ~RESET;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    bool retcode = MCU_Watchdog_IsResetCausedByWatchdog();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_TRUE(retcode);
    EXPECT_EQ(UINT32_C(1), __HAL_RCC_GET_FLAG_fake.call_count);
    EXPECT_EQ(UINT32_C(1), __HAL_RCC_CLEAR_RESET_FLAGS_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (__HAL_RCC_GET_FLAG);
    RESET_FAKE (__HAL_RCC_CLEAR_RESET_FLAGS);
}
#else
}

#endif /* if BCDS_FEATURE_WATCHDOG */
