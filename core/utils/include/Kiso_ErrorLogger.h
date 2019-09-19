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
 * @ingroup UTILS
 *
 * @defgroup ERRORLOGGER ErrorLogger
 * @{
 *
 * @brief
 *      This file contains public interface for ErrorLogger module
 *
 * @details
 *      This file exposes the public interface of the ErrorLogger Module. Mostly call
 *      ErrorLogger_LogError() with Retcode_T to log the error to a persistent memory such as
 *      Userpage, SD card etc. Each time the ErrorLogger_LogError() is called, error will be logged
 *      with time stamp following the ring buffer model.
 *
 * @file
 **/

#ifndef KISO_ERRORLOGGER_H_
#define KISO_ERRORLOGGER_H_

/* KISO interface header files */
#include "Kiso_Utils.h"

#if KISO_FEATURE_ERRORLOGGER

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/**
 * @brief
 *      Struct for storing the details of Logs
 */
struct ErrorLogger_LogEntry_S
{
    uint32_t TimeStamp;      /** It records the timestamp (uptime) of error occurance */
    uint32_t ErrorCode;      /** It holds the Retcode_T which says from where the error comes */
    uint16_t SeqNo;          /** It holds error occurance number */
    uint16_t Reserved;       /** For furture use */
}__attribute__((packed));

typedef struct ErrorLogger_LogEntry_S ErrorLogger_LogEntry_T;

/** @Warning This item needs to be updated whenever the log entry structure changes!! */
#define SIZEOF_LOGENTRY     (UINT8_C(12))

#define ERRORLOGGER_SIZE    (UINT8_C(120))

#define MAXENTRIES (ERRORLOGGER_SIZE/SIZEOF_LOGENTRY)

/**
 * @brief
 *      Enum for different storage medium
 */
enum ErrorLogger_StorageMedium_E
{
	STORAGE_TYPE_SDCARD = 0,
	STORAGE_TYPE_EXT_FLASH,
	STORAGE_TYPE_NVM,
	STORAGE_TYPE_OTHERS // Reserved for future usage of any other medium
};

typedef enum ErrorLogger_StorageMedium_E ErrorLogger_StorageMedium_T;

/**
 * @brief
 *      This function reads the data from the specified partition of the storage medium. 
 *
 * @note
 *      Before calling this function ensure ErrorLogger_Init has been done.
 *      Also the caller of this function has to ensure the valid position & no of bytes to read from the medium.
 *
 * @param[in] partitio
 *      Type of the storage medium to read.
 * @param[in] value
 *      Pointer to destination value to hold the data. This pointer should have enough room
 *      to place no of bytes that is mentioned in argument numOfBytes.
 * @param[in] StartAddr
 *      Offset from the start of the partition.
 * @param[in] numOfBytes
 *      Number of bytes to read from the storage medium.
 *
 * @retval  #RETCODE_OK
 *      If the read was successful, else an error code is returned.
 *
 */
typedef Retcode_T (*ErrorLogger_ReadFunc_T)(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);

/**
 * @brief
 *      This function writes the data from the specified partition of the storage medium. 
 *
 * @note
 *      Before calling this function ensure ErrorLogger_Init has been done.
 *      Also the caller of this function has to ensure the valid position & no of bytes to write from the medium.
 *
 * @param[in] partition
 *      Type of the storage medium to write.
 * @param[in] value
 *      Pointer to destination value to hold the data. This pointer should have enough room to place
 *      no of bytes that is mentioned in argument numOfBytes.
 * @param[in] StartAddr
 *      Offset from the start of the partition.
 * @param[in] numOfBytes
 *      Number of bytes to write from the storage medium.
 *
 * @retval  #RETCODE_OK
 *      If the write was successful, else an error code is returned.
 *
 */
typedef Retcode_T (*ErrorLogger_WriteFunc_T)(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);

/**
 * @brief
 *      This function writes the data from the specified partition of the storage medium. 
 *
 * @note
 *      Before calling this function ensure ErrorLogger_Init has been done.
 *      Also the caller of this function has to ensure the valid position & no of bytes to erase from the medium.
 *
 * @param[in] partition
 *      Type of the storage medium to erase.
 * @param[in] value
 *      Pointer to destination value to hold the data. This pointer should have enough room to place
 *      no of bytes that is mentioned in argument numOfBytes.
 * @param[in] StartAddr
 *      Offset from the start of the partition.
 * @param[in] numOfBytes
 *      Number of bytes to erase from the storage medium.
 *
 * @retval  #RETCODE_OK
 *      When the erase was successful else an error code is returned.
 *
 */
typedef Retcode_T (*ErrorLogger_EraseFunc_T)(ErrorLogger_StorageMedium_T storageSelect, void* value, uint32_t StartAddr, uint32_t numOfBytes);

/**
 * @brief
 *      This function is used to provide the time stamp and unit will be
 *      based on application perspective.
 *
 * @return
 *      Current time stamp.
 */
typedef uint32_t (*ErrorLogger_GetTime) (void);

/** Structure contains function pointers as it members to access the storage medium easily.*/
struct ErrorLoggerConfig_S
{
	ErrorLogger_StorageMedium_T StorageMedium;
    ErrorLogger_ReadFunc_T ReadLogs;
    ErrorLogger_WriteFunc_T WriteLogs;
    ErrorLogger_EraseFunc_T EraseLogs;
    ErrorLogger_GetTime Time_Stamp;
    uint32_t position;
    uint32_t length;
};

typedef struct ErrorLoggerConfig_S ErrorLoggerConfig_T;

/**
 * @brief
 *      API for initialize the ErrorLogger module
 *
 * @details
 *      The current configuration of error logger is that it can store up to 120 bytes where
 *      one error log entry is 12 bytes. Thus 10 logs could be stored. This can be modified
 *      based on the application requirements.
 *
 * @param[in] storageAgentHandle
 *      Data structure handling the information required for error logging
 *
 * @retval #RETCODE_OK
 *      When the ErrorLogger module is successfully initialized
 * @retval #RETCODE_FAILURE
 *      When the read operation of the storage agent failed
 * 
 */
Retcode_T ErrorLogger_Init(ErrorLoggerConfig_T storageAgentHandle);

/**
 * @brief
 *      API for logging error to the storage medium
 * 
 * @wawrning
 *      This API is not thread-safe and the priority has to be handled in the application.
 *
 * @details
 *      It logs the errors to the storage medium along with timestamp. It also counts and
 *      numbers all the errors.
 *
 * @param[in] Error
 *      Error code to be logged.
 *
 * @retval #RETCODE_OK
 *      When error is logged successfully
 * @retval #RETCODE_INVALID_PARAM
 *      When trying to log a RETCODE_OK value for error (which is not an error...)
 * @return
 *      When writing failed, the actual error code is the one returned by the #WriteLogs function
 *      (see #ErrorLoggerConfig_T)
 * 
 */
Retcode_T ErrorLogger_LogError(Retcode_T Error);

/**
 * @brief
 *      API to get the most recent error happened in the system.
 *
 * @details
 *      Using this API, one can get the most recent error in the system.
 *
 * @note 
 *      If there is an error, LogEntry is filled with most recent error, else it is left untouched
 *
 * @param[out] LogEntry
 *      Pointer to be filled with last logged error
 *
 * @retval #RETCODE_OK
 *      When the last error is read successfully
 * @retval #RETCODE_INVALID_PARAM
 *      When the LogEntry pointer is NULL
 * @retval #RETCODE_OUT_OF_RESOURCES
 *      When no error was logged yet
 * 
 */
Retcode_T ErrorLogger_GetLastErrorLog(ErrorLogger_LogEntry_T *LogEntry);

/**
 * @brief
 *      API to query if particular error has happened in the system.
 *
 * @param[in] Error
 *      The Error code to be searched in the logs
 *
 * @retval RETCODE_OK
 *      When the queried error is present
 * @retval RETCODE_FAILURE
 *      When the queried error is not present
 * 
 */
Retcode_T ErrorLogger_HasError(Retcode_T Error);

/**
 * @brief
 *      API to query total number of errors occurred in the system.
 *
 * @note
 *      The total number of errors logged can be bigger than the maximum number of entries
 *      as the errors are logged in a cyclical manner
 *
 * @return
 *      Numbers of errors in the system.
 * 
 */
uint16_t ErrorLogger_GetTotalErrors(void);

/**
 * @brief
 *      API to query numbers of errors available in the storage medium.
 *
 * @return
 *      Numbers of errors in the storage medium.
 * 
 */
uint8_t ErrorLogger_GetCurrentErrors(void);

/**
 * @brief
 *      API to get the error at particular index
 *
 * @param[in] index
 *      Index of the error to be queried
 * @param[out] LogEntry
 *      If there is an error at this index, it will be filled with that error.
 *
 * @retval #RETCODE_OK
 *      When there is an error at this index and it is successfully read
 * @retval #RETCODE_OUT_OF_RESOURCES
 *      When the index is bigger than number of error that can be stored
 * @retval #RETCODE_INVALID_PARAM
 *      When there is no error at this index
 *
 */
Retcode_T ErrorLogger_GetErrorAt(uint8_t index, ErrorLogger_LogEntry_T *LogEntry);

/**
 * @brief
 *      API to clear all the error in the user page
 *
 * @retval #RETCODE_OK
 *      When all the errors have been erased successfully and the error logger is
 *      reinitialized successfully
 * @return
 *      Errors from either the erase operation (see #ErrorLoggerConfig_T.EraseLogs) or the error logger
 *      initialization (see #ErrorLogger_Init)
 * 
 */
Retcode_T ErrorLogger_ClearAllErrorLogs(void);

#endif /* if KISO_FEATURE_ERRORLOGGER */
#endif /* KISO_ERRORLOGGER_H_ */
/**@} */
