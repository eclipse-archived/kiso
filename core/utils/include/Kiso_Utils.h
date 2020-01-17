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
 * @file
 * @defgroup UTILS Utils
 * @ingroup KISO_CORE 
 
 * @{
 *
 * @brief
 *     The Kiso utilities package provides commonly used modules in application development
 *      like Logging (@ref LOGGING), Task Monitoring (@ref TASKMONITOR), CRC checking (@ref CRC), etc.
 *      These modules can be individually enabled or disabled per application via
 *      Kiso_UtilsConfig.cmake in the config directory of the application.
 *
 * @details
 
 
 */

#ifndef KISO_UTILS_H_
#define KISO_UTILS_H_

/* interface dependency checks */
#include "Kiso_UtilsConfig.h"
#include "Kiso_Retcode.h"

/**
 * @brief
 *      Enumeration to represent the return codes of Utils module.
 */
enum Utils_Retcode_E
{
    RETCODE_XPROTOCOL_INTEGRITY_FAILED = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL,
    RETCODE_XPROTOCOL_DATA_BUFFER_TOO_SMALL,
    RETCODE_XPROTOCOL_DATA_EXCEEDES_MAX_PAYLOAD_LENGTH,
    RETCODE_XPROTOCOL_FRAME_NOT_COMPLETE_YET,
    RETCODE_XPROTOCOL_START_DELIMITER_MISSING,
    RETCODE_XPROTOCOL_END_DELIMITER_MISSING,
    RETCODE_CMDLINE_DEBUGGER_COMMAND_NOT_FOUND,
    RETCODE_CMDPROCESSOR_TASK_INIT_FAIL,     /**< Failed to initialize Command Processor Task */
    RETCODE_CMDPROCESSOR_QUEUE_INIT_FAIL,    /**< Failed to initialize CmdProcessor Queue */
    RETCODE_CMDPROCESSOR_QUEUE_ENQUEUE_FAIL, /**< Failed to enqueue element, probably because of a timeout */
    RETCODE_CMDPROCESSOR_QUEUE_DEQUEUE_FAIL, /**< Failed to dequeue element, probably because of a timeout */
    RETCODE_CMDPROCESSOR_EXECUTE_FAILURE,    /**< Command to be executed was a NULL pointer */
    RETCODE_CMDPROCESSOR_QUEUE_ERROR,
    RETCODE_GUARDEDTASK_SEMAPHORE_ALREADY_GIVEN,
    RETCODE_GUARDEDTASK_SEMAPHORE_ERROR,
    RETCODE_TASKMONITOR_BUFFER_FULL_ERROR,
    RETCODE_SLEEPCONTROL_NOSLEEP,
    RETCODE_I2CTRANSCEIVER_TRANSFER_ERROR,
    RETCODE_MAX_ERROR,
};

/**
 * @brief
 *      KISO_MODULE_ID for each Utils C module
 *
 * @details
 *      Usage:
 *
 * @code{.c}
 *      #undef KISO_MODULE_ID
 *      #define KISO_MODULE_ID KISO_UTILS_MODULE_ID_xxx
 * @endcode
 *
 */
enum KISO_UTILS_ModuleID_E
{
    KISO_UTILS_MODULE_ID_CRC = 1,
    KISO_UTILS_MODULE_ID_RINGBUFFER,
    KISO_UTILS_MODULE_ID_XPROTOCOL,
    KISO_UTILS_MODULE_ID_LOGGING,
    KISO_UTILS_MODULE_ID_LOGGING_FILTER,
    KISO_UTILS_MODULE_ID_LOGGING_RECORD_ASYNCHRONOUS,
    KISO_UTILS_MODULE_ID_LOGGING_RECORD_SYNCHRONOUS,
    KISO_UTILS_MODULE_ID_LOGGING_APPENDER_UART,
    KISO_UTILS_MODULE_ID_CMD_LINE_DEBUGGER,
    KISO_UTILS_MODULE_ID_UART_TRANSCEIVER,
    KISO_UTILS_MODULE_ID_I2C_TRANSCEIVER,
    KISO_UTILS_MODULE_ID_ERRORLOGGER,
    KISO_UTILS_MODULE_ID_CMDPROCESSOR,
    KISO_UTILS_MODULE_ID_GUARDEDTASK,
    KISO_UTILS_MODULE_ID_TASKMONITOR,
    KISO_UTILS_MODULE_ID_EVENTHUB,
    KISO_UTILS_MODULE_ID_SLEEPCONTROL,
    KISO_UTILS_MODULE_ID_PIPEANDFILTER,
};

#endif /* KISO_UTILS_H_ */
/**@} */
