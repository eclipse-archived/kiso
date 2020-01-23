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
 *      Module test specification for the Logging_UartAppender_unittest.cc module.
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
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_LOGGING_APPENDER_UART

/* Check if the logging feature is activated */
#if KISO_FEATURE_LOGGING
#include "LogConfig.h"

/* Check if the uart appender is activated */
#if KISO_UART_APPENDER

#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "Kiso_MCU_UART_th.hh"
#include "Kiso_BSP_TestInterface_th.hh"
#include "Kiso_UARTTransceiver_th.hh"

#include "Logging_UartAppender.c"
    /* End of global scope symbol and fake definitions section */
}

/* The tests */
class Logging_UartAppenderTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        RESET_FAKE(UARTTransceiver_Initialize);
        RESET_FAKE(UARTTransceiver_Start);
        RESET_FAKE(BSP_TestInterface_Connect);
        RESET_FAKE(BSP_TestInterface_Enable);
        RESET_FAKE(MCU_UART_Initialize);
        RESET_FAKE(UARTTransceiver_WriteData);
        RESET_FAKE(UARTTransceiver_LoopCallback);
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* Nothing to do if clean up is not required */
    }
};

TEST_F(Logging_UartAppenderTest, Logging_UartAppenderInitialization_Success)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = UartAppender.Init(NULL);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(Logging_UartAppenderTest, Logging_UartAppenderInitialization_UARTTransceiverInitFail)
{
    Retcode_T retcode = RETCODE_OK;
    UARTTransceiver_Initialize_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);

    retcode = UartAppender.Init(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(Logging_UartAppenderTest, Logging_UartAppenderInitialization_UARTTransceiverStartFail)
{
    Retcode_T retcode = RETCODE_OK;
    UARTTransceiver_Start_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);

    retcode = UartAppender.Init(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

TEST_F(Logging_UartAppenderTest, Logging_UartAppenderInitialization_BSPTestInterfaceConnectFail)
{
    Retcode_T retcode = RETCODE_OK;
    BSP_TestInterface_Connect_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);

    retcode = UartAppender.Init(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

TEST_F(Logging_UartAppenderTest, Logging_UartAppenderInitialization_BSPTestInterfaceEnableFail)
{
    Retcode_T retcode = RETCODE_OK;
    BSP_TestInterface_Enable_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);

    retcode = UartAppender.Init(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

TEST_F(Logging_UartAppenderTest, Logging_UartAppenderInitialization_MCUUARTInitializeFail)
{
    Retcode_T retcode = RETCODE_OK;
    MCU_UART_Initialize_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);

    retcode = UartAppender.Init(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

TEST_F(Logging_UartAppenderTest, Logging_UartAppenderWrite_SuccessOrFailure)
{
    (void)UartAppender.Write(NULL, 0); // Nothing really to test. There is a direct call to UARTTransceiver_WriteData.

    EXPECT_EQ(UINT32_C(1), UARTTransceiver_WriteData_fake.call_count);
}

/* Starting here, testing the static functions and doing coverage*/
TEST_F(Logging_UartAppenderTest, UartCallback_SuccessOrFailure)
{
    struct MCU_UART_Event_S event;

    UartCallback(NULL, event); // Nothing really to test. There is a direct call to UARTTransceiver_LoopCallback.

    EXPECT_EQ(UINT32_C(1), UARTTransceiver_LoopCallback_fake.call_count);
}

TEST_F(Logging_UartAppenderTest, LogCheckEndFrameFunc_CheckFalse)
{
    bool retcode = true;
    retcode = LogCheckEndFrameFunc(0); // Nothing really to test. Return false...

    EXPECT_FALSE(retcode);
}

#else
}
#endif /* if KISO_UART_APPENDER */
#else
}
#endif /* if KISO_FEATURE_LOGGING */
