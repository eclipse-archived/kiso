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
 * @breif
 *      Module test specification for the ErrorLogger_unittest.cc module.
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
/* Setup compile time configuration defines */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_ERRORLOGGER

#if KISO_FEATURE_ERRORLOGGER

/* Include faked interfaces */
#include "Kiso_Retcode_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"

/* Include module under test */
#include "ErrorLogger.c"

    /* End of global scope symbol and fake definitions section */
}

/* Retcode function locally defined */
Retcode_T Retcode_compose(uint32_t package,
                          Retcode_Severity_T severity, uint32_t code)
{
    uint32_t p = (package & 0x000000FF) << 24;
    uint32_t s = ((uint32_t)severity & 0x000000FF) << 16;
    uint32_t c = (code & 0x0000FFFF);
    Retcode_T retcode = (code == UINT32_C(0)) ? (Retcode_T)UINT32_C(0) : (Retcode_T)(p | s | c);

    return (retcode);
}

/* Local Function declarations ***************************************************** */
Retcode_T ErrorLogger_Test_Write(ErrorLogger_StorageMedium_T storageSelect, void *value, uint32_t StartAddr, uint32_t numOfBytes);
Retcode_T ErrorLogger_Test_Read(ErrorLogger_StorageMedium_T storageSelect, void *value, uint32_t StartAddr, uint32_t numOfBytes);
Retcode_T ErrorLogger_Test_Erase(ErrorLogger_StorageMedium_T storageSelect, void *value, uint32_t StartAddr, uint32_t numOfBytes);

/* Global variables ********************************************************* */

static uint8_t testFlag = 0; //dummy variable used for masking the return type of function.

Retcode_T ErrorLogger_Test_Read(ErrorLogger_StorageMedium_T storageSelect, void *value, uint32_t StartAddr, uint32_t numOfBytes)
{
    KISO_UNUSED(storageSelect);
    KISO_UNUSED(StartAddr);
    KISO_UNUSED(numOfBytes);
    KISO_UNUSED(value);
    Retcode_T retVal = RETCODE_OK;
    if (testFlag == 1)
    {
        return RETCODE_FAILURE;
    }
    return retVal;
}

Retcode_T ErrorLogger_Test_Write(ErrorLogger_StorageMedium_T storageSelect, void *value, uint32_t StartAddr, uint32_t numOfBytes)
{
    KISO_UNUSED(storageSelect);
    KISO_UNUSED(StartAddr);
    KISO_UNUSED(numOfBytes);
    KISO_UNUSED(value);
    Retcode_T retVal = RETCODE_OK;
    if (testFlag == 1)
    {
        return RETCODE_FAILURE;
    }
    return retVal;
}

Retcode_T ErrorLogger_Test_Erase(ErrorLogger_StorageMedium_T storageSelect, void *value, uint32_t StartAddr, uint32_t numOfBytes)
{
    KISO_UNUSED(storageSelect);
    KISO_UNUSED(StartAddr);
    KISO_UNUSED(numOfBytes);
    KISO_UNUSED(value);
    Retcode_T retVal = RETCODE_OK;
    if (testFlag == 1)
    {
        return RETCODE_FAILURE;
    }
    return retVal;
}

ErrorLoggerConfig_T Log_Handle =
    {
        .StorageMedium = (ErrorLogger_StorageMedium_T)STORAGE_TYPE_NVM,
        .ReadLogs = ErrorLogger_Test_Read,
        .WriteLogs = ErrorLogger_Test_Write,
        .EraseLogs = ErrorLogger_Test_Erase,
        .Time_Stamp = xTaskGetTickCount,
        .position = 0xAE000,
        .length = 120};

class KISO_ErrorLogger : public testing::Test
{
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        /*Reset the Fake function Structure*/
        ErrorSeqNo = 0;
        NextIndexToWriteOn = 0;
        testFlag = 0;
        FFF_RESET_HISTORY();
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        /* Nothing to do if clean up is not required */
    }
};

/* Specify test cases ******************************************************* */

/**
 *  @testcase test case to check the ErrorLogger Init function when nvm fails.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_Init_Fail)
{
    Retcode_T Retcode;
    testFlag = 1;

    Retcode = ErrorLogger_Init(Log_Handle);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE), Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger Init function.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_Init_NoLogs)
{
    Retcode_T Retcode;

    Retcode = ErrorLogger_Init(Log_Handle);

    EXPECT_EQ(UINT32_C(0), ErrorSeqNo);
    EXPECT_EQ(UINT32_C(0), NextIndexToWriteOn);
    EXPECT_EQ(RETCODE_SUCCESS, Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger Init function with half logs filled.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_Init_HalfLogsFilled)
{
    Retcode_T Retcode;

    for (int i = 0; i < (int)ERRORLOGGER_MAXENTRIES / 2; i++)
    {
        ErrorLogger_LogError((Retcode_T)500 + i);
    }

    Retcode = ErrorLogger_Init(Log_Handle);

    EXPECT_EQ(ERRORLOGGER_MAXENTRIES / 2, ErrorSeqNo);
    EXPECT_EQ((uint32_t)ERRORLOGGER_MAXENTRIES / 2, NextIndexToWriteOn);
    EXPECT_EQ(RETCODE_SUCCESS, Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger Init function with one and half logs filled.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_Init_OneAndHalfTimesLogsFilled)
{
    Retcode_T Retcode;

    for (int i = 0; i < (int)(ERRORLOGGER_MAXENTRIES + ERRORLOGGER_MAXENTRIES / 2); i++)
    {
        ErrorLogger_LogError((Retcode_T)500 + i);
    }

    Retcode = ErrorLogger_Init(Log_Handle);

    EXPECT_EQ((ERRORLOGGER_MAXENTRIES + ERRORLOGGER_MAXENTRIES / 2), ErrorSeqNo);
    EXPECT_EQ((uint32_t)ERRORLOGGER_MAXENTRIES / 2, NextIndexToWriteOn);
    EXPECT_EQ(RETCODE_SUCCESS, Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger_LogError function when error is RETCODE_OK.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_LogError_RETCODEOK)
{
    Retcode_T Retcode;
    Retcode = ErrorLogger_LogError(RETCODE_OK);
    EXPECT_EQ(Retcode, RETCODE(RETCODE_SEVERITY_WARNING, (Retcode_T)RETCODE_INVALID_PARAM));
}

/**
 *  @testcase test case to check the ErrorLogger_LogError function when one error occurs.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_LogError_ONEError)
{
    Retcode_T Retcode;
    Retcode = ErrorLogger_LogError((Retcode_T)250);

    EXPECT_EQ(RETCODE_OK, Retcode);
    EXPECT_EQ(UINT32_C(1), ErrorSeqNo);
    EXPECT_EQ(UINT32_C(1), NextIndexToWriteOn);
}

/**
 *  @testcase test case to check the ErrorLogger_LogError function when flush fails.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_LogError_FlushFail)
{
    Retcode_T Retcode;
    testFlag = 1;

    Retcode = ErrorLogger_LogError((Retcode_T)250);

    EXPECT_EQ(RETCODE_FAILURE, Retcode);
    EXPECT_EQ(UINT32_C(0), ErrorSeqNo);
    EXPECT_EQ(UINT32_C(0), NextIndexToWriteOn);
}

/**
 *  @testcase test case to check the ErrorLogger_GetLastErrorLog function when null pointer used.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetLastErrorLog_NULLPTR)
{
    Retcode_T Retcode;
    Retcode = ErrorLogger_GetLastErrorLog(NULL);
    EXPECT_EQ(Retcode, RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T)RETCODE_INVALID_PARAM));
}

/**
 *  @testcase test case to check the ErrorLogger_GetLastErrorLog function when no error is available.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetLastErrorLog_NoError)
{
    Retcode_T Retcode;
    ErrorLogger_LogEntry_T LogEntry;

    Retcode = ErrorLogger_GetLastErrorLog(&LogEntry);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T)RETCODE_OUT_OF_RESOURCES), Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger_GetLastErrorLog function when one error is available.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetLastErrorLog_OneError)
{
    Retcode_T Retcode;
    ErrorLogger_LogEntry_T LogEntry;
    ErrorLogger_LogError((Retcode_T)250);

    Retcode = ErrorLogger_GetLastErrorLog(&LogEntry);

    EXPECT_EQ(Retcode, RETCODE_OK);
    EXPECT_EQ((unsigned int)250, LogEntry.ErrorCode);
}

/**
 *  @testcase test case to check the ErrorLogger_GetLastErrorLog function when multiple errors are available.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetLastErrorLog_MultiError)
{
    Retcode_T Retcode;
    ErrorLogger_LogEntry_T LogEntry;
    for (int i = 0; i < (int)(2 * ERRORLOGGER_MAXENTRIES); i++)
    {
        ErrorLogger_LogError((Retcode_T)200 + i);
        Retcode = ErrorLogger_GetLastErrorLog(&LogEntry);
        EXPECT_EQ(Retcode, RETCODE_OK);
        EXPECT_EQ((unsigned int)200 + i, LogEntry.ErrorCode);
    }
}

/**
 *  @testcase test case to check the ErrorLogger_HasError function when no errors available.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_HasError_WithoutErrors)
{
    Retcode_T retcode;
    retcode = ErrorLogger_HasError(332);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T)RETCODE_FAILURE), retcode);
}

/**
 *  @testcase test case to check the ErrorLogger_HasError function when multiple errors available.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_HasError_WithAvailableErrors)
{
    Retcode_T retcode;
    for (int i = 0; i < (int)(2 * ERRORLOGGER_MAXENTRIES); i++)
    {
        ErrorLogger_LogError((Retcode_T)200 + i);
        retcode = ErrorLogger_HasError((Retcode_T)200 + i);
        EXPECT_EQ(retcode, (Retcode_T)RETCODE_SUCCESS);
    }
}

/**
 *  @testcase test case to check the ErrorLogger_GetTotalErrors function.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetTotalErrors)
{
    EXPECT_EQ(ErrorSeqNo, ErrorLogger_GetTotalErrors());
}

/**
 *  @testcase test case to check the ErrorLogger_GetCurrentErrors function when multiple errors available.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetCurrentErrors_LessMaxEntries)
{
    uint8_t Count;
    for (int i = 0; i < (int)(ERRORLOGGER_MAXENTRIES / 2); i++)
    {
        (void)ErrorLogger_LogError((Retcode_T)500 + i);
    }
    Count = ErrorLogger_GetCurrentErrors();
    EXPECT_EQ(ERRORLOGGER_MAXENTRIES / 2, Count);
}

/**
 *  @testcase test case to check the ErrorLogger_GetCurrentErrors function when multiple errors available.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetCurrentErrors_OneAndHalfMaxEntries)
{
    uint8_t Count;
    for (int i = 0; i < (int)(ERRORLOGGER_MAXENTRIES + ERRORLOGGER_MAXENTRIES / 2); i++)
    {
        (void)ErrorLogger_LogError((Retcode_T)500 + i);
    }
    Count = ErrorLogger_GetCurrentErrors();
    EXPECT_EQ(Count, (uint8_t)ERRORLOGGER_MAXENTRIES);
}

/**
 *  @testcase test case to check the ErrorLogger_GetErrorAt function when multiple errors available.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetErrorAt)
{
    ErrorLogger_LogEntry_T LogEntry;
    Retcode_T Retcode;

    for (int i = 0; i < (int)ERRORLOGGER_MAXENTRIES; i++)
    {
        ErrorLogger_LogError((Retcode_T)500 + i);
    }

    for (int i = 0; i < (int)ERRORLOGGER_MAXENTRIES; i++)
    {
        Retcode = ErrorLogger_GetErrorAt(i, &LogEntry);

        EXPECT_EQ(Retcode, RETCODE_SUCCESS);
        EXPECT_EQ(LogEntry.ErrorCode, (Retcode_T)500 + i);
    }
    Retcode = ErrorLogger_GetErrorAt(ERRORLOGGER_MAXENTRIES + 2, &LogEntry);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T)RETCODE_INVALID_PARAM), Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger_GetErrorAt function when no entry is found.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_GetErrorAtFail)
{
    Retcode_T Retcode;
    Retcode = ErrorLogger_GetErrorAt(ERRORLOGGER_MAXENTRIES, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T)RETCODE_INVALID_PARAM), Retcode);
}
/**
 *  @testcase test case to check the ErrorLogger_ClearAllErrorLogs function.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_ClearAllErrorLogs)
{
    Retcode_T Retcode;
    for (int i = 0; i <= (int)ERRORLOGGER_MAXENTRIES; i++)
    {
        ErrorLogger_LogError((Retcode_T)500 + i);
    }

    Retcode = ErrorLogger_ClearAllErrorLogs();

    EXPECT_EQ(Retcode, RETCODE_SUCCESS);
    EXPECT_EQ(NextIndexToWriteOn, (uint32_t)0);
    EXPECT_EQ(ErrorSeqNo, (uint16_t)0);
}

/**
 *  @testcase test case to check the ErrorLogger_ClearAllErrorLogs function when erase operation fails.
 *
 */
TEST_F(KISO_ErrorLogger, ErrorLogger_ClearAllErrorLogsFail)
{
    Retcode_T Retcode;
    testFlag = 1;
    for (int i = 0; i <= (int)ERRORLOGGER_MAXENTRIES; i++)
    {
        ErrorLogger_LogError((Retcode_T)500 + i);
    }

    Retcode = ErrorLogger_ClearAllErrorLogs();
    EXPECT_EQ(Retcode, RETCODE_FAILURE);
    EXPECT_EQ(NextIndexToWriteOn, (uint32_t)0);
    EXPECT_EQ(ErrorSeqNo, (uint16_t)0);
}
#else
}
#endif /* if KISO_FEATURE_ERRORLOGGER */
