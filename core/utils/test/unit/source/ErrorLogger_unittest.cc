/******************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  Module test specification for the ErrorLogger module.
 *
 *  The unit test file template follows the Four-Phase test pattern. ...
 *
 * ****************************************************************************/

/* include gtest interface */
#include <gtest.h>

extern "C"
{

/* setup compile time configuration defines */
#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_ERRORLOGGER

#if BCDS_FEATURE_ERRORLOGGER

/* include faked interfaces */
#include "BCDS_Retcode_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"

#include "ErrorLogger.c"
}

/* Retcode function locally defined */
Retcode_T Retcode_compose(uint32_t package,
        Retcode_Severity_T severity, uint32_t code)
{
    uint32_t p = (package & 0x000000FF) << 24;
    uint32_t s = ((uint32_t) severity & 0x000000FF) << 16;
    uint32_t c = (code & 0x0000FFFF);
    Retcode_T retcode = (code == UINT32_C(0)) ?(Retcode_T) UINT32_C(0) : (Retcode_T) (p | s | c);

    return (retcode);
}

/* Local Function declarations ***************************************************** */
Retcode_T ErrorLogger_Test_Write(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);
Retcode_T ErrorLogger_Test_Read(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);
Retcode_T ErrorLogger_Test_Erase(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);

/* global variables ********************************************************* */



static uint8_t testFlag = 0; //dummy variable used for masking the return type of function.
Retcode_T ErrorLogger_Test_Read(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes)
{
    BCDS_UNUSED(storageSelect);
    BCDS_UNUSED(StartAddr);
    BCDS_UNUSED(numOfBytes);
    BCDS_UNUSED(value);
    Retcode_T retVal = RETCODE_OK;
    if(testFlag == 1)
	{
		return RETCODE_FAILURE;
	}
    return retVal;
}

Retcode_T ErrorLogger_Test_Write(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes)
{
    BCDS_UNUSED(storageSelect);
    BCDS_UNUSED(StartAddr);
	BCDS_UNUSED(numOfBytes);
	BCDS_UNUSED(value);
    Retcode_T retVal = RETCODE_OK;
    if(testFlag ==1)
    {
    	return RETCODE_FAILURE;
    }
    return retVal;
}

Retcode_T ErrorLogger_Test_Erase(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes)
{
	BCDS_UNUSED(storageSelect);
	BCDS_UNUSED(StartAddr);
	BCDS_UNUSED(numOfBytes);
	BCDS_UNUSED(value);
    Retcode_T retVal = RETCODE_OK;
    if(testFlag == 1)
	{
		return RETCODE_FAILURE;
	}
    return retVal;
}

ErrorLoggerConfig_T Log_Handle =
	{
                .StorageMedium = (ErrorLogger_StorageMedium_T) STORAGE_TYPE_NVM,
                .ReadLogs = ErrorLogger_Test_Read,
                .WriteLogs= ErrorLogger_Test_Write,
                .EraseLogs = ErrorLogger_Test_Erase,
                .Time_Stamp = xTaskGetTickCount,
                .position = 0xAE000,
                .length = 120
    };


class BCDS_ErrorLogger: public testing::Test
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
        /* nothing to do if clean up is not required */
    }
};

/**
 *  @testcase test case to check the ErrorLogger Init function when nvm fails.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_Init_Fail)
{
    /* Declare and initialize local variable required only for this test case  */
   Retcode_T Retcode;
   testFlag = 1;

   /* Exercise: call relevant production code Interface with appropriate test inputs  */
   Retcode = ErrorLogger_Init(Log_Handle);

   /* Verify : Compare the expected with actual    */
   EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE),Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger Init function.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_Init_NoLogs)
{
    /* Declare and initialize local variable required only for this test case  */
   Retcode_T Retcode;

   /* Exercise: call relevant production code Interface with appropriate test inputs  */
   Retcode = ErrorLogger_Init(Log_Handle);

   /* Verify : Compare the expected with actual    */
   EXPECT_EQ(UINT32_C(0),ErrorSeqNo);
   EXPECT_EQ(UINT32_C(0),NextIndexToWriteOn);
   EXPECT_EQ(RETCODE_SUCCESS,Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger Init function with half logs filled.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_Init_HalfLogsFilled)
{
    /* Declare and initialize local variable required only for this test case  */
   Retcode_T Retcode;

   for(int i = 0; i < (int)MAXENTRIES/2; i++ )
   {
      ErrorLogger_LogError((Retcode_T) 500+i);
   }

   /* Exercise: call relevant production code Interface with appropriate test inputs  */
   Retcode = ErrorLogger_Init(Log_Handle);

   /* Verify : Compare the expected with actual    */
   EXPECT_EQ(MAXENTRIES/2,ErrorSeqNo);
   EXPECT_EQ((uint32_t)MAXENTRIES/2,NextIndexToWriteOn);
   EXPECT_EQ(RETCODE_SUCCESS,Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger Init function with one and half logs filled.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_Init_OneAndHalfTimesLogsFilled)
{
    /* Declare and initialize local variable required only for this test case  */
   Retcode_T Retcode;

   for(int i = 0; i < (int)(MAXENTRIES+ MAXENTRIES/2); i++ )
   {
      ErrorLogger_LogError((Retcode_T) 500+i);
   }

   /* Exercise: call relevant production code Interface with appropriate test inputs  */
   Retcode = ErrorLogger_Init(Log_Handle);

   /* Verify : Compare the expected with actual    */
   EXPECT_EQ((MAXENTRIES+ MAXENTRIES/2),ErrorSeqNo);
   EXPECT_EQ((uint32_t)MAXENTRIES/2,NextIndexToWriteOn);
   EXPECT_EQ(RETCODE_SUCCESS,Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger_LogError function when error is RETCODE_OK.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_LogError_RETCODEOK)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;

    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Retcode = ErrorLogger_LogError( RETCODE_OK);

    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(Retcode,RETCODE(RETCODE_SEVERITY_WARNING, (Retcode_T )RETCODE_INVALID_PARAM));
}

/**
 *  @testcase test case to check the ErrorLogger_LogError function when one error occurs.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_LogError_ONEError)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;

    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Retcode = ErrorLogger_LogError( (Retcode_T) 250);

    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(RETCODE_OK,Retcode);
    EXPECT_EQ(UINT32_C(1),ErrorSeqNo);
    EXPECT_EQ(UINT32_C(1),NextIndexToWriteOn);
}

/**
 *  @testcase test case to check the ErrorLogger_LogError function when flush fails.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_LogError_FlushFail)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;
    testFlag = 1;
    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Retcode = ErrorLogger_LogError( (Retcode_T) 250);
    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(RETCODE_FAILURE,Retcode);
    EXPECT_EQ(UINT32_C(0),ErrorSeqNo);
    EXPECT_EQ(UINT32_C(0),NextIndexToWriteOn);
}

/**
 *  @testcase test case to check the ErrorLogger_GetLastErrorLog function when null pointer used.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_GetLastErrorLog_NULLPTR)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;
    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Retcode = ErrorLogger_GetLastErrorLog(NULL);
    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(Retcode, RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T )RETCODE_INVALID_PARAM));
}

/**
 *  @testcase test case to check the ErrorLogger_GetLastErrorLog function when no error is available.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_GetLastErrorLog_NoError)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;
    ErrorLogger_LogEntry_T LogEntry;

    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Retcode = ErrorLogger_GetLastErrorLog(&LogEntry);
    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T )RETCODE_OUT_OF_RESOURCES),Retcode);
}

/**
 *  @testcase test case to check the ErrorLogger_GetLastErrorLog function when one error is available.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_GetLastErrorLog_OneError)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;
    ErrorLogger_LogEntry_T LogEntry;
    ErrorLogger_LogError((Retcode_T) 250);
    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Retcode = ErrorLogger_GetLastErrorLog(&LogEntry);

    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(Retcode, RETCODE_OK);
    EXPECT_EQ((unsigned int)250, LogEntry.ErrorCode);
}

/**
 *  @testcase test case to check the ErrorLogger_GetLastErrorLog function when multiple errors are available.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_GetLastErrorLog_MultiError)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;
    ErrorLogger_LogEntry_T LogEntry;
    for(int i = 0; i<(int)(2*MAXENTRIES);i++)
    {
        /* Exercise: call relevant production code Interface with appropriate test inputs  */
       ErrorLogger_LogError((Retcode_T) 200+i);
       Retcode = ErrorLogger_GetLastErrorLog(&LogEntry);
       /* Verify : Compare the expected with actual    */
       EXPECT_EQ(Retcode, RETCODE_OK);
       EXPECT_EQ((unsigned int)200+i, LogEntry.ErrorCode);
    }
}

/**
 *  @testcase test case to check the ErrorLogger_HasError function when no errors available.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_HasError_WithoutErrors)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T retcode;
    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    retcode = ErrorLogger_HasError(332);
    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T )RETCODE_FAILURE),retcode );

}

/**
 *  @testcase test case to check the ErrorLogger_HasError function when multiple errors available.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_HasError_WithAvailableErrors)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T retcode;
    for(int i = 0; i<(int)(2*MAXENTRIES);i++)
    {
        /* Exercise: call relevant production code Interface with appropriate test inputs  */
        ErrorLogger_LogError((Retcode_T) 200+i);
        retcode = ErrorLogger_HasError((Retcode_T) 200+i);
        /* Verify : Compare the expected with actual    */
        EXPECT_EQ(retcode, (Retcode_T)RETCODE_SUCCESS );
    }
}

/**
 *  @testcase test case to check the ErrorLogger_GetTotalErrors function.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_GetTotalErrors)
{
    EXPECT_EQ(ErrorSeqNo,ErrorLogger_GetTotalErrors());
}

/**
 *  @testcase test case to check the ErrorLogger_GetCurrentErrors function when multiple errors available.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_GetCurrentErrors_LessMaxEntries)
{
    /* Declare and initialize local variable required only for this test case  */
    uint8_t Count;
    for(int i = 0; i <(int)( MAXENTRIES/2); i++ )
    {
       (void)ErrorLogger_LogError((Retcode_T) 500+i);
    }
    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Count = ErrorLogger_GetCurrentErrors();
    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(MAXENTRIES/2,Count);
}

/**
 *  @testcase test case to check the ErrorLogger_GetCurrentErrors function when multiple errors available.
 *
 */
TEST_F(BCDS_ErrorLogger, ErrorLogger_GetCurrentErrors_OneAndHalfMaxEntries)
{
    /* Declare and initialize local variable required only for this test case  */
    uint8_t Count;
    for(int i = 0; i <(int)( MAXENTRIES+MAXENTRIES/2); i++ )
    {
       (void)ErrorLogger_LogError((Retcode_T) 500+i);
    }
    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Count = ErrorLogger_GetCurrentErrors();
    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(Count,(uint8_t)MAXENTRIES);
}

/**
 *  @testcase test case to check the ErrorLogger_GetErrorAt function when multiple errors available.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_GetErrorAt)
{
    /* Declare and initialize local variable required only for this test case  */
    ErrorLogger_LogEntry_T LogEntry;
    Retcode_T Retcode;

    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    for(int i = 0; i < (int)MAXENTRIES; i++ )
    {
       ErrorLogger_LogError((Retcode_T) 500+i);
    }

    for(int i = 0; i<(int) MAXENTRIES;i++)
    {
        Retcode = ErrorLogger_GetErrorAt(i,&LogEntry);

    /* Verify : Compare the expected with actual    */
        EXPECT_EQ(Retcode,RETCODE_SUCCESS);
        EXPECT_EQ(LogEntry.ErrorCode,(Retcode_T) 500+i);
    }
    Retcode = ErrorLogger_GetErrorAt(MAXENTRIES+2, &LogEntry);

        EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T )RETCODE_INVALID_PARAM),Retcode);

}

/**
 *  @testcase test case to check the ErrorLogger_GetErrorAt function when no entry is found.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_GetErrorAtFail)
{
    /* Declare and initialize local variable required only for this test case  */
	Retcode_T Retcode;

	/* Exercise: call relevant production code Interface with appropriate test inputs  */
	Retcode = ErrorLogger_GetErrorAt(MAXENTRIES,NULL);

	/* Verify : Compare the expected with actual    */
	EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T )RETCODE_INVALID_PARAM),Retcode);
}
/**
 *  @testcase test case to check the ErrorLogger_ClearAllErrorLogs function.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_ClearAllErrorLogs)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;
    for(int i = 0; i <= (int)MAXENTRIES; i++ )
    {
       ErrorLogger_LogError((Retcode_T) 500+i);
    }

    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Retcode = ErrorLogger_ClearAllErrorLogs();
    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(Retcode,RETCODE_SUCCESS);
    EXPECT_EQ(NextIndexToWriteOn,(uint32_t)0);
    EXPECT_EQ(ErrorSeqNo,(uint16_t)0);
}

/**
 *  @testcase test case to check the ErrorLogger_ClearAllErrorLogs function when erase operation fails.
 *
 */
TEST_F(BCDS_ErrorLogger,ErrorLogger_ClearAllErrorLogsFail)
{
    /* Declare and initialize local variable required only for this test case  */
    Retcode_T Retcode;
    testFlag = 1;
    for(int i = 0; i <= (int)MAXENTRIES; i++ )
    {
       ErrorLogger_LogError((Retcode_T) 500+i);
    }

    /* Exercise: call relevant production code Interface with appropriate test inputs  */
    Retcode = ErrorLogger_ClearAllErrorLogs();
    /* Verify : Compare the expected with actual    */
    EXPECT_EQ(Retcode,RETCODE_FAILURE);
    EXPECT_EQ(NextIndexToWriteOn,(uint32_t)0);
    EXPECT_EQ(ErrorSeqNo,(uint16_t)0);
}
#else
}
#endif /* if BCDS_FEATURE_ERRORLOGGER */
