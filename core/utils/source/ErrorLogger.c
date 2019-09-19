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
 *      Error Logger Interface Implementation
 *
 * @details
 *      This source file implements following features:
 *      - ErrorLogger_Init()
 *      - ErrorLogger_LogError()
 *      - ErrorLogger_GetLastErrorLog()
 *      - ErrorLogger_HasError()
 *      - ErrorLogger_GetTotalErrors()
 *      - ErrorLogger_GetCurrentErrors()
 *      - ErrorLogger_GetErrorAt()
 *      - ErrorLogger_ClearAllErrorLogs()
 * 
 * @file
 **/

/*
 * Redefine Package ID
 */

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_ERRORLOGGER

#if KISO_FEATURE_ERRORLOGGER

#ifdef KISO_PACKAGE_ID
#undef KISO_PACKAGE_ID
#define KISO_PACKAGE_ID 0
#endif

/* Include Kiso_ErrorLogger interface header */
#include "Kiso_ErrorLogger.h"

/** @brief Actual consecutive sequence number
 * @details The sequence number is stored with the error entry in the log.
 * The first error sequence number is 1 and is incremented by 1 with each new
 * entry that is made in the log.
 */
static uint16_t ErrorSeqNo = 0;

/**
 * Defines the next index onto which an error can be written
 */
static uint32_t NextIndexToWriteOn = 0;

/** @brief Buffer into which the error logger data is stored when fetched from user
 * data section
 * @details The error log is organized as a ring buffer with a fixed size and
 * is able to hold a fixed number of error logger entries. When the end of the
 * buffer is reached then writing starts over at the first entry.
 */
static uint8_t DataFromUserPage[ERRORLOGGER_SIZE] = { 0 };

/** Pointer on an array of MAXENTRIES log entries */
static ErrorLogger_LogEntry_T (*pErrorEntries)[MAXENTRIES];

ErrorLoggerConfig_T ErrorLoggerHandle;

/*  The description of the function is available in Kiso_ErrorLogger.h */
Retcode_T ErrorLogger_Init(ErrorLoggerConfig_T storageAgentHandle)
{
    Retcode_T nvmRetCode = RETCODE_OK;
    ErrorLoggerHandle = storageAgentHandle;

    /* A pointer on an array of log entries */
    pErrorEntries = (ErrorLogger_LogEntry_T (*)[MAXENTRIES]) DataFromUserPage; /* Point on first entry */

    /* Need to read out the entire error log in order to process it */
    nvmRetCode = ErrorLoggerHandle.ReadLogs(ErrorLoggerHandle.StorageMedium, DataFromUserPage, ErrorLoggerHandle.position, ErrorLoggerHandle.length);
    if (RETCODE_OK == nvmRetCode)
    {
        uint16_t highestSeq = 0;

        /* Then loop through the entries and search the entry with containing
         * the highest sequence number to determine the next write index
         */
        for (uint32_t i = 0; i < MAXENTRIES; i++)
        {
            if ((*pErrorEntries)[i].SeqNo >= highestSeq)
            {
                highestSeq = (*pErrorEntries)[i].SeqNo;
            }
        }
        ErrorSeqNo = highestSeq;
        /* Determine the next index to write on. The sequence number is
         * consecutively incremented with each new entry so that the first
         * entry starts with a sequence number of 1.
         */
        NextIndexToWriteOn = ErrorSeqNo % MAXENTRIES;
        return RETCODE_OK;
    }
    
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
}

/*  The description of the function is available in Kiso_ErrorLogger.h */
Retcode_T ErrorLogger_LogError(Retcode_T Error)
{
    Retcode_T nvmRet = RETCODE_OK;

    /* If error code is RETCODE_OK implies no error. So don't log */
    if (RETCODE_OK == Error)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, (Retcode_T )RETCODE_INVALID_PARAM);
    }
    /* else do nothing */

    ErrorSeqNo++; /* first entry ever has sequence number 1 */
    /* Fill next error entry with data */
    (*pErrorEntries)[NextIndexToWriteOn].TimeStamp = ErrorLoggerHandle.Time_Stamp();
    (*pErrorEntries)[NextIndexToWriteOn].ErrorCode = Error;
    (*pErrorEntries)[NextIndexToWriteOn].SeqNo = ErrorSeqNo;
    (*pErrorEntries)[NextIndexToWriteOn].Reserved = 0x0;

    nvmRet = ErrorLoggerHandle.WriteLogs(ErrorLoggerHandle.StorageMedium, (void *) pErrorEntries,ErrorLoggerHandle.position, ErrorLoggerHandle.length);
    if (RETCODE_OK == nvmRet)
    {
        /* Recalculate write index */
        NextIndexToWriteOn = ErrorSeqNo % MAXENTRIES;
        return RETCODE_OK;
    }
  	/* Userpage write is not success. So error is not updated to Userpage. */
    ErrorSeqNo -= 1;
  
    return nvmRet;
}

/*  The description of the function is available in Kiso_ErrorLogger.h */
Retcode_T ErrorLogger_GetLastErrorLog(ErrorLogger_LogEntry_T *LogEntry)
{
    if (LogEntry)
    {
        /* NextIndexToWriteOn should also be the last error logged */
        if (ErrorSeqNo)
        {
            uint32_t LastIndexWritten;
            if (NextIndexToWriteOn > 0)
            {
                LastIndexWritten = NextIndexToWriteOn - 1;
            }
            else
            {
                LastIndexWritten = (MAXENTRIES - 1);
            }
            *LogEntry = (*pErrorEntries)[LastIndexWritten];
            return RETCODE_OK;
        }
        /*No Error is occured */
        return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_OUT_OF_RESOURCES);
    }
    return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_INVALID_PARAM);
}

/*  The description of the function is available in Kiso_ErrorLogger.h */
Retcode_T ErrorLogger_HasError(Retcode_T Error)
{
    /* Loop through the logged errors and search the error code passed */
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T )RETCODE_FAILURE);
    
    if (ErrorSeqNo)
    {
        for (uint32_t i = 0; i < MAXENTRIES; i++)
        {
            if ((*pErrorEntries)[i].ErrorCode == Error)
            {
                retcode = RETCODE_OK;
                break;  /* LEAVES THE LOOP */
            }
        }
    }
    return retcode;
}

/*  The description of the function is available in Kiso_ErrorLogger.h */
uint16_t ErrorLogger_GetTotalErrors(void)
{
    return ErrorSeqNo;
}

/*  The description of the function is available in Kiso_ErrorLogger.h */
uint8_t ErrorLogger_GetCurrentErrors(void)
{
    uint8_t ErrorCount;
    if (ErrorSeqNo >= MAXENTRIES)
    {
        ErrorCount = MAXENTRIES;
    }
    else
    {
        ErrorCount = ErrorSeqNo;
    }
    return ErrorCount;
}

/*  The description of the function is available in Kiso_ErrorLogger.h */
Retcode_T ErrorLogger_GetErrorAt(uint8_t index, ErrorLogger_LogEntry_T *LogEntry)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T )RETCODE_INVALID_PARAM);
    if (LogEntry && (index < MAXENTRIES))
    {
        if ((*pErrorEntries)[index].SeqNo)
        {
            *LogEntry = (*pErrorEntries)[index];
            retcode = RETCODE_OK;
        }
        else
        {
            /* Error at that index */
            retcode = RETCODE(RETCODE_SEVERITY_INFO, (Retcode_T )RETCODE_OUT_OF_RESOURCES);
        }
    }
    return retcode;
}

/*  The description of the function is available in Kiso_ErrorLogger.h */
Retcode_T ErrorLogger_ClearAllErrorLogs(void)
{
    Retcode_T retcode = RETCODE_OK;
    /* Clear log */
    memset(DataFromUserPage, 0, ErrorLoggerHandle.length);

    /* Write entire error cleared log to flash */
    retcode = ErrorLoggerHandle.EraseLogs(ErrorLoggerHandle.StorageMedium, (void *) pErrorEntries, ErrorLoggerHandle.position, ErrorLoggerHandle.length);                                            //write activity here
    
    if (RETCODE_OK == retcode)
    {
        /* Re-initialize stuff as all errors are cleared now*/
        retcode = ErrorLogger_Init(ErrorLoggerHandle);
    }

    return retcode;
}

#endif /* if KISO_FEATURE_ERRORLOGGER */
