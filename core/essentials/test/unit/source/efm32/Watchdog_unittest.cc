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

/* include faked interfaces */
#include "BCDS_HAL_th.hh"
#include "em_wdog_th.hh"
#include "em_rmu_th.hh"


#if BCDS_FEATURE_WATCHDOG

/* include module under test */
#include "Watchdog.c"

/* end of global scope symbol and fake definitions section */

class EFM32_WATCHDOG_TEST: public testing::Test
{
protected:

    /* Watch-dog handle with init configuration */
    MCU_Watchdog_Init_T wdgInitStruct;

    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        RESET_FAKE (WDOG_Init);
        RESET_FAKE (WDOG_Enable);
        RESET_FAKE (WDOG_Feed);
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

TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_Init_Configure)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_Init_Configure: }
     * Successful MCU_Watchdog_Init() test case with application watchdog configuration
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WDOG_Init_fake.call_count = 0;

    wdgInitStruct.WdgCallback = NULL;
    wdgInitStruct.ResetMode = MCU_WATCHDOG_RESET_ON;
    wdgInitStruct.RunOnCpuHalt = false;
    wdgInitStruct.Timeout = 6002;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Init((WdgHandle_T)&wdgInitStruct);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), WDOG_Init_fake.call_count);
}

TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_Init_NULL)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_Init_NULL: }
     * Successful MCU_Watchdog_Init() test case with NULL parameter
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WDOG_Init_fake.call_count = 0;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Init(NULL);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(0), WDOG_Init_fake.call_count);
}

TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_Init_InvalidTimeout)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_Init_InvalidTimeout: }
     * Successful MCU_Watchdog_Init() test case with invalid timeout parameter
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WDOG_Init_fake.call_count = 0;

    wdgInitStruct.WdgCallback = NULL;
    wdgInitStruct.ResetMode = MCU_WATCHDOG_RESET_ON;
    wdgInitStruct.RunOnCpuHalt = false;
    wdgInitStruct.Timeout = 256001;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Init((WdgHandle_T)&wdgInitStruct);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(0), WDOG_Init_fake.call_count);
}

TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_Init_ResetOff)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_Init_ResetOff: }
     * Successful MCU_Watchdog_Init() test case with invalid timeout parameter
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WDOG_Init_fake.call_count = 0;

    wdgInitStruct.WdgCallback = NULL;
    wdgInitStruct.ResetMode = MCU_WATCHDOG_RESET_OFF;
    wdgInitStruct.RunOnCpuHalt = false;
    wdgInitStruct.Timeout = 256001;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Init((WdgHandle_T)&wdgInitStruct);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(0), WDOG_Init_fake.call_count);
}
/* ***************************************************************************************** */
/* MCU_Watchdog_Enable()                                                                       */
/* ***************************************************************************************** */
TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_Enable)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_Enable: }
     * Successful MCU_Watchdog_Enable() test case after MCU_Watchdog_Init
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WDOG_Enable_fake.call_count = 0;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Enable();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), WDOG_Enable_fake.call_count);
}

TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_Enable_WithoutInit)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_Enable_WithoutInit: }
     * Successful MCU_Watchdog_Enable() test case before MCU_Watchdog_Init
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WDOG_Enable_fake.call_count = 0;
    wdgInitFlag = false;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Enable();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(0), WDOG_Enable_fake.call_count);
}

/* ***************************************************************************************** */
/* MCU_Watchdog_Feed()                                                                       */
/* ***************************************************************************************** */
TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_Feed)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_Feed: }
     * Successful MCU_Watchdog_Feed() test case after MCU_Watchdog_Enable
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    WDOG_Feed_fake.call_count = 0;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_Watchdog_Feed();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), WDOG_Feed_fake.call_count);
}

/* ***************************************************************************************** */
/* MCU_Watchdog_IsResetCausedByWatchdog()                                                                       */
/* ***************************************************************************************** */
TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_IsResetCausedByWatchdog)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_IsResetCausedByWatchdog: }
     * MCU_Watchdog_IsResetCausedByWatchdog gives status of watchdog reset
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    RMU_ResetCauseGet_fake.return_val = RMU_RSTCAUSE_WDOGRST;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    bool wdgResetFlag = MCU_Watchdog_IsResetCausedByWatchdog();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_TRUE(wdgResetFlag);
}

TEST_F(EFM32_WATCHDOG_TEST, MCU_Watchdog_IsResetCausedByPowerOnReset)
{
    /** @testcase{ EFM32_WATCHDOG_TEST::MCU_Watchdog_IsResetCausedByWatchdog: }
     * MCU_Watchdog_IsResetCausedByWatchdog gives status of watchdog reset
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    RMU_ResetCauseGet_fake.return_val = RMU_RSTCAUSE_PORST;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    bool wdgResetFlag = MCU_Watchdog_IsResetCausedByWatchdog();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_FALSE(wdgResetFlag);
}

#endif