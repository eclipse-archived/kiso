/*****************************************************************************/
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
 *  <Brief description about the functionality of the module>
 *
 * ****************************************************************************/

/* system header files */
#include <stdint.h>
#include <string.h>

/* additional interface header files */
#include "BCDS_Utils.h"
#include "BCDS_Tests.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include "BCDS_NVM.h"
#include "BCDS_NVMConfig.h"
#include "BCDS_TestingConfig.h"
#include "BCDS_ErrorLogger.h"
#include "TestSuite_ErrorLogger.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "em_rmu.h"

/*
 * Redefine Package ID
 */

#ifdef BCDS_MODULE_ID
#undef BCDS_MODULE_ID
#endif

#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_ERRORLOGGER

#ifdef BCDS_PACKAGE_ID
#undef BCDS_PACKAGE_ID
#define BCDS_PACKAGE_ID 0
#endif

uint8_t ErrorLogger_NVMData[NVM_SECTION_UserPage_BUFFER_SIZE];

/* Function prototypes */
static Retcode_T ErrorLoggerSuiteSetup(CCMsg_T *ccmsg);

static Retcode_T TC_Common_Setup_ErrorLogger(CCMsg_T* ccmsg);

static void TC_01_Run_LoggingSingleError(CCMsg_T* ccmsg);
static void TC_02_Run_LoggingMultipleError(CCMsg_T* ccmsg);
static void TC_03_Run_ClearAllError(CCMsg_T* ccmsg);
static void TC_04_Run_GetLastError(CCMsg_T* ccmsg);
static void TC_05_Run_HasError(CCMsg_T* ccmsg);
static void TC_06_Run_CountErrors(CCMsg_T* ccmsg);
static void TC_07_Run_GetErrorAt_ParticularIndex(CCMsg_T* ccmsg);
static void TC_08_Run_GetErrorAt_DiffScenario(CCMsg_T* ccmsg);

struct NVM_S NVMUser =
{
    NVM_SECTION_UserPage,
    ErrorLogger_NVMData,
    NVM_SECTION_UserPage_BUFFER_SIZE
};

/* Local Function declarations ***************************************************** */
Retcode_T ErrorLogger_NVM_Write(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);
Retcode_T ErrorLogger_NVM_Read(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);
Retcode_T ErrorLogger_NVM_Erase(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);

ErrorLoggerConfig_T NVM_Handle =
        {
                .StorageMedium = (ErrorLogger_StorageMedium_T) STORAGE_TYPE_NVM,
                .ReadLogs = ErrorLogger_NVM_Read,
                .WriteLogs = ErrorLogger_NVM_Write,
                .EraseLogs = ErrorLogger_NVM_Erase,
                .Time_Stamp = xTaskGetTickCount,
                .position = 0xAE000,
                .length = 120
        };

/* Function implementations */
Retcode_T ErrorLogger_NVM_Read(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes)
{
	Retcode_T nvmRetCode = RETCODE_OK;
    BCDS_UNUSED(storageSelect);
    BCDS_UNUSED(StartAddr);

    NVM_Read(&NVMUser, NVM_ITEM_ID_ERRORLOGGER_START, value, numOfBytes);
    if (nvmRetCode != RETCODE_OK)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, (Retcode_T )RETCODE_INVALID_PARAM);
    }
    return nvmRetCode;
}

Retcode_T ErrorLogger_NVM_Write(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes)
{
	Retcode_T nvmRetCode = RETCODE_OK;
    BCDS_UNUSED(storageSelect);
    BCDS_UNUSED(StartAddr);

    nvmRetCode = NVM_Write(&NVMUser, NVM_ITEM_ID_ERRORLOGGER_START, value, numOfBytes);
    if (nvmRetCode != RETCODE_OK)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, (Retcode_T )RETCODE_INVALID_PARAM);
    }
    return nvmRetCode;
}

Retcode_T ErrorLogger_NVM_Erase(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes)
{
	Retcode_T nvmRetCode = RETCODE_OK;
    BCDS_UNUSED(storageSelect);
    BCDS_UNUSED(StartAddr);

    memset(value,0,NVM_ITEM_ID_ERRORLOGGER_START.length_byte);

    nvmRetCode = NVM_Write(&NVMUser, NVM_ITEM_ID_ERRORLOGGER_START, value, numOfBytes);
    if (nvmRetCode != RETCODE_OK)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, (Retcode_T )RETCODE_INVALID_PARAM);
    }
    return nvmRetCode;
}

/** The description is in the private header */
Retcode_T TestSuite_ErrorLogger_Initialize( uint8_t suite_id )
{
	Retcode_T errorReturn = RETCODE_OK;

	errorReturn = Tests_RegisterTestSuite(suite_id, &ErrorLoggerSuiteSetup, NULL);

	if(RETCODE_OK == errorReturn)
	{
		errorReturn = Tests_RegisterTestCase(suite_id, 1, NULL, &TC_01_Run_LoggingSingleError, NULL);
	}
	if(RETCODE_OK == errorReturn)
	{
		errorReturn = Tests_RegisterTestCase(suite_id, 2, &TC_Common_Setup_ErrorLogger, &TC_02_Run_LoggingMultipleError, NULL);
	}
	if(RETCODE_OK == errorReturn)
	{
		errorReturn = Tests_RegisterTestCase(suite_id, 3, NULL, &TC_03_Run_ClearAllError, NULL);
	}
	if(RETCODE_OK == errorReturn)
	{
		errorReturn = Tests_RegisterTestCase(suite_id, 4, NULL, &TC_04_Run_GetLastError, NULL);
	}
	if(RETCODE_OK == errorReturn)
	{
		errorReturn = Tests_RegisterTestCase(suite_id, 5, NULL, &TC_05_Run_HasError, NULL);
	}
	if(RETCODE_OK == errorReturn)
	{
		errorReturn = Tests_RegisterTestCase(suite_id, 6, &TC_Common_Setup_ErrorLogger, &TC_06_Run_CountErrors, NULL);
	}
	if(RETCODE_OK == errorReturn)
	{
		errorReturn = Tests_RegisterTestCase(suite_id, 7, &TC_Common_Setup_ErrorLogger, &TC_07_Run_GetErrorAt_ParticularIndex, &TC_Common_Setup_ErrorLogger);
	}
	if(RETCODE_OK == errorReturn)
	{
		errorReturn = Tests_RegisterTestCase(suite_id, 8, &TC_Common_Setup_ErrorLogger, TC_08_Run_GetErrorAt_DiffScenario, &TC_Common_Setup_ErrorLogger);
	}
    return errorReturn;
}

static Retcode_T ErrorLoggerSuiteSetup(CCMsg_T *ccmsg)
{
    Retcode_T errorReturn = RETCODE_OK;
    if (ccmsg != NULL)
    {
        /* Initialize NMV sections in user data flash */
        errorReturn = NVM_Initialize(&NVMUser);

        if (RETCODE_OK == errorReturn)
        {
        	errorReturn = ErrorLogger_Init(NVM_Handle);

   			if (RETCODE_OK == errorReturn)
   			{
   				errorReturn = ErrorLogger_ClearAllErrorLogs();
   				vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);
   			}
   			else
   			{
   				errorReturn = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
   			}
        }
    }
    else
    {
        errorReturn = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    return errorReturn;
}

/*--------------------------------------------------------------------------------------------------------------*/
/* TestCase TC 6.1.1: Logging a single error */
/*--------------------------------------------------------------------------------------------------------------*/
static void TC_01_Run_LoggingSingleError(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Retcode = RETCODE_OK;
    ErrorLogger_LogEntry_T Log[MAX_ENTRY];

    Retcode = ErrorLogger_LogError(234);

    if(RETCODE_OK == Retcode)
    {
        vTaskDelay(((portTickType)(2))/portTICK_RATE_MS);
        if(RETCODE_OK == Retcode)
        {
       		Retcode = NVM_Read(&NVMUser, NVM_ITEM_ID_ERRORLOGGER_START, Log, ERRORLOGGER_SIZE);

			if(RETCODE_OK == Retcode)
			{
				if( (1 == Log[0].SeqNo) && (234 == Log[0].ErrorCode) )
				{
					Retcode = RETCODE_OK;
				}
				else {
					Retcode = RETCODE_FAILURE;
				}
			}
       }
   }

    Tests_SendReport(Retcode, NULL);
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Test Case  TC 6.1.2: Logging the multiple errors  */
/*--------------------------------------------------------------------------------------------------------------*/
static void TC_02_Run_LoggingMultipleError(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Retcode = RETCODE_OK;
    ErrorLogger_LogEntry_T Log[MAX_ENTRY];
    uint8_t LoopVari;

    for(LoopVari = 0 ; LoopVari <= MAX_ENTRY; LoopVari++ )
    {
        Retcode = ErrorLogger_LogError(230 + LoopVari);
        vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);
        if(RETCODE_OK != Retcode)
            break;
    }


    if(RETCODE_OK == Retcode)
    {
        Retcode = NVM_Initialize(&NVMUser);

        vTaskDelay(((portTickType)(2))/portTICK_RATE_MS);

        if(RETCODE_OK == Retcode)
        {
            Retcode = NVM_Read(&NVMUser, NVM_ITEM_ID_ERRORLOGGER_START, Log, ERRORLOGGER_SIZE);

            if(RETCODE_OK == Retcode)
            {
                for(LoopVari = 0 ; LoopVari <= MAX_ENTRY; LoopVari++ )
                {
                    if( (LoopVari == (uint8_t)Log[LoopVari].SeqNo) && (234+LoopVari == (uint8_t)Log[LoopVari].ErrorCode) )
                    {
                        Retcode = RETCODE_OK;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    Tests_SendReport(Retcode, NULL);
}

/*----------------------------------------------------------------------------------------------------------------*/
/* TestCase TC 6.1.3: Clearing All Error Messages */
/*----------------------------------------------------------------------------------------------------------------*/
static void TC_03_Run_ClearAllError(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Retcode = RETCODE_OK;
    ErrorLogger_LogEntry_T Log[MAX_ENTRY];
    uint8_t LoopVari;
    int Errors = 0;

    for(LoopVari = 0 ; LoopVari < MAX_ENTRY; LoopVari++ )
    {
        Retcode = ErrorLogger_LogError(230 + LoopVari);
        vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);
        if(RETCODE_OK != Retcode)
            break;
    }

    Retcode = ErrorLogger_ClearAllErrorLogs();

    if(Retcode)
    {
        Retcode = NVM_Initialize(&NVMUser);

        vTaskDelay(((portTickType)(2))/portTICK_RATE_MS);

        if(RETCODE_OK == Retcode)
        {
            Retcode = NVM_Read(&NVMUser, NVM_ITEM_ID_ERRORLOGGER_START, Log, ERRORLOGGER_SIZE);

            if(RETCODE_OK == Retcode)
            {
                for(LoopVari = 0 ; LoopVari < MAX_ENTRY; LoopVari++ )
                {
                    if( (0 == Log[LoopVari].SeqNo) && (0 == Log[LoopVari].ErrorCode) )
                    {
                        Retcode = RETCODE_OK;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        Errors = ErrorLogger_GetTotalErrors();

        if( 0!= Errors)
        {
            Retcode = RETCODE_FAILURE;
        }
    }

    Tests_SendReport(Retcode, NULL);
}

/*----------------------------------------------------------------------------------------------------------------*/
/* TestCase TC 6.1.4: Get last Error Log */
/*----------------------------------------------------------------------------------------------------------------*/
static void TC_04_Run_GetLastError(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Retcode = RETCODE_OK;
    ErrorLogger_LogEntry_T *LogPtr = NULL, Log;
    uint8_t LoopVari;

    Retcode = ErrorLogger_LogError(234);
    vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);

    if(RETCODE_OK == Retcode)
    {
        Retcode = ErrorLogger_GetLastErrorLog(LogPtr);
        if(RETCODE(RETCODE_SEVERITY_INFO,RETCODE_INVALID_PARAM) ==  Retcode)
        {
            Retcode = RETCODE_OK;
        }
    }

    if(RETCODE_OK == Retcode)
    {
        Retcode = ErrorLogger_GetLastErrorLog(&Log);
    }

    if(RETCODE_OK ==  Retcode)
    {
        if(234 != Log.ErrorCode)
        {
            Retcode = RETCODE_FAILURE;
        }
    }

    if(RETCODE_OK ==  Retcode)
    {
        for(LoopVari = 0 ; LoopVari <= MAX_ENTRY; LoopVari++ )
        {
            Retcode = ErrorLogger_LogError(234 + LoopVari);
            vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);
            if(RETCODE_OK != Retcode)
                break;
        }
    }

    if(RETCODE_OK == Retcode)
    {
        Retcode = ErrorLogger_GetLastErrorLog(&Log);
    }


    if(RETCODE_OK ==  Retcode)
    {
        if((234 + LoopVari -1) != (uint8_t)Log.ErrorCode)
        {
            Retcode = RETCODE_FAILURE;
        }
    }

    Tests_SendReport(Retcode, NULL);
}

/*----------------------------------------------------------------------------------------------------------------*/
/* TC 6.1.5: Check where particular Error is available in log */
/*----------------------------------------------------------------------------------------------------------------*/
static void TC_05_Run_HasError(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Retcode = RETCODE_OK;
    uint8_t LoopVari;

    for(LoopVari = 0 ; LoopVari < MAX_ENTRY - 2; LoopVari++ )
    {
        Retcode = ErrorLogger_LogError(230 + LoopVari);
        vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);
        if(RETCODE_OK != Retcode)
            break;
    }

    if(RETCODE_OK == Retcode)
    {
        Retcode = ErrorLogger_HasError(230);
    }

    if(RETCODE_OK == Retcode)
    {
        Retcode = ErrorLogger_HasError(430);
        if(Retcode !=  RETCODE_OK )
        {
            Retcode = RETCODE_OK;
        }
        else
        {
            Retcode = RETCODE_FAILURE;
        }
    }

    Tests_SendReport(Retcode, NULL);
}


/*----------------------------------------------------------------------------------------------------------------*/
/*TC 6.1.6: Count number of error messages*/
/*----------------------------------------------------------------------------------------------------------------*/
static void TC_06_Run_CountErrors(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    uint16_t Errors;
    Retcode_T Retcode = RETCODE_OK;
    uint8_t LoopVari;

    Errors = ErrorLogger_GetTotalErrors();
    if(0 != Errors)
    {
        Retcode = RETCODE_FAILURE;
    }


    Errors = ErrorLogger_GetCurrentErrors();
    if(0 != Errors)
    {
        Retcode = RETCODE_FAILURE;
    }

    Retcode = ErrorLogger_LogError(234);
    vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);

    if(RETCODE_OK == Retcode)
    {
        Errors = ErrorLogger_GetTotalErrors();
        if(1 != Errors)
        {
            Retcode = RETCODE_FAILURE;
        }


        Errors = ErrorLogger_GetCurrentErrors();
        if(1 != Errors)
        {
            Retcode = RETCODE_FAILURE;
        }

    }

    for(LoopVari = 0 ; LoopVari < MAX_ENTRY; LoopVari++ )
    {
        Retcode = ErrorLogger_LogError(230 + LoopVari);
        vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);
        if(RETCODE_OK != Retcode)
            break;
    }

    if(RETCODE_OK == Retcode)
    {
        Errors = ErrorLogger_GetTotalErrors();
        if((MAX_ENTRY + 1) != Errors)
        {
            Retcode = RETCODE_FAILURE;
        }


        Errors = ErrorLogger_GetCurrentErrors();
        if(MAX_ENTRY != Errors)
        {
            Retcode = RETCODE_FAILURE;
        }

    }

    Tests_SendReport(Retcode, NULL);
}

/*----------------------------------------------------------------------------------------------------------------*/
/*TC 6.1.7: Error message at particular index*/
/*----------------------------------------------------------------------------------------------------------------*/
static void TC_07_Run_GetErrorAt_ParticularIndex(CCMsg_T* ccmsg)
{
    Retcode_T Retcode = RETCODE_OK;
    BCDS_UNUSED(ccmsg);
    ErrorLogger_LogEntry_T Log, *LogPtr = NULL;
    uint8_t LoopVari;

    if(RETCODE_OK == Retcode)
    {
        Retcode = ErrorLogger_GetLastErrorLog(LogPtr);
        if(RETCODE(RETCODE_SEVERITY_INFO,RETCODE_INVALID_PARAM) ==  Retcode)
        {
            Retcode = RETCODE_OK;
        }
    }

    Retcode = ErrorLogger_GetErrorAt(1, &Log);

    if(RETCODE(RETCODE_SEVERITY_INFO,RETCODE_OUT_OF_RESOURCES) != Retcode)
    {
        Retcode = RETCODE_FAILURE;
    }

    Retcode = ErrorLogger_GetErrorAt(1 + MAX_ENTRY, &Log);

    if(RETCODE(RETCODE_SEVERITY_INFO,RETCODE_INVALID_PARAM)!= Retcode)
    {
        Retcode = RETCODE_FAILURE;
    }

    for(LoopVari = 0 ; LoopVari <= 5; LoopVari++ )
    {
        Retcode = ErrorLogger_LogError(230 + LoopVari);
        vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);
        if(RETCODE_OK != Retcode)
            break;
    }

    if(RETCODE_OK == Retcode )
    {
        Retcode = ErrorLogger_GetErrorAt(0, &Log);
    }

    if(RETCODE_OK == Retcode )
    {
        if(230 != Log.ErrorCode )
        {
            Retcode = RETCODE_FAILURE;
        }
    }

    if(RETCODE_OK == Retcode )
    {
        Retcode_T RequiredCode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_OUT_OF_RESOURCES);
        Retcode = ErrorLogger_GetErrorAt(8, &Log);

        if( RequiredCode == Retcode)
        {
            Retcode = RETCODE_OK;
        }
        else
        {
            Retcode = RETCODE_FAILURE;
        }

    }

    Tests_SendReport(Retcode, NULL);
}

/*----------------------------------------------------------------------------------------------------------------*/
/*TC 6.1.8: Values at different scenarios*/
/*----------------------------------------------------------------------------------------------------------------*/
static void TC_08_Run_GetErrorAt_DiffScenario(CCMsg_T* ccmsg)
{
	Retcode_T Retcode = RETCODE_OK;
	uint8_t rebootCounter = UINT8_C(0);

	rebootCounter = Tests_GetRebootCounter(ccmsg);

	switch(rebootCounter)
	{
		case 0:
			if(0 != ErrorLogger_GetTotalErrors())
			{
			   	Retcode = RETCODE_FAILURE;
			}
			if(0 != ErrorLogger_GetCurrentErrors())
			{
				Retcode = RETCODE_FAILURE;
			}
			if(RETCODE_OK == Tests_SaveStateBeforeReboot(ccmsg))
			{
				ErrorLogger_LogError(10);
				vTaskDelay(1000);

				Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t )RETCODE_OUT_OF_RESOURCES));
			}
			break;

		case 1:
			ErrorLogger_Init(NVM_Handle);
			if(1 != ErrorLogger_GetTotalErrors())
			{
				Retcode = RETCODE_FAILURE;
			}
			if(1 != ErrorLogger_GetCurrentErrors())
			{
				Retcode = RETCODE_FAILURE;
			}
			break;

		default:
			Retcode = RETCODE_FAILURE;
			break;
	}
	Tests_SendReport(Retcode, NULL);
}

/*----------------------------------------------------------------------------------------------------------------*/
/*TC 6.1: This is common setup for all the test cases*/
/*----------------------------------------------------------------------------------------------------------------*/
static Retcode_T TC_Common_Setup_ErrorLogger(CCMsg_T* ccmsg)
{
	Retcode_T Retcode = RETCODE_OK;	BCDS_UNUSED(ccmsg);	ErrorLogger_ClearAllErrorLogs();
	vTaskDelay(((portTickType)(500))/portTICK_RATE_MS);	return Retcode;
}
