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
 * @brief Utils config header.
 *
 * @details
 * Provides configuration interface for the Utils components.
 */

#ifndef KISO_UTILSCONFIG_H_
#define KISO_UTILSCONFIG_H_

// clang-format off

#ifndef KISO_FEATURE_CMDLINEDEBUGGER
/** @brief Enable (1) or disable (0) the CmdLineDebugger feature. */
#define KISO_FEATURE_CMDLINEDEBUGGER 1
#endif

#ifndef KISO_FEATURE_CMDPROCESSOR
/** @brief Enable (1) or disable (0) the CmdProcessor feature. */
#define KISO_FEATURE_CMDPROCESSOR 1
#endif

#ifndef KISO_FEATURE_CRC
/** @brief Enable (1) or disable (0) the Crc feature. */
#define KISO_FEATURE_CRC 1
#endif

#ifndef KISO_FEATURE_EVENTHUB
/** @brief Enable (1) or disable (0) the EventHub feature. */
#define KISO_FEATURE_EVENTHUB 1
#endif

#ifndef KISO_FEATURE_GUARDEDTASK
/** @brief Enable (1) or disable (0) the GuardedTask feature. */
#define KISO_FEATURE_GUARDEDTASK 1
#endif

#ifndef KISO_FEATURE_ERRORLOGGER
/** @brief Enable (1) or disable (0) the ErrorLogger feature. */
#define KISO_FEATURE_ERRORLOGGER 1
#endif

#ifndef KISO_FEATURE_LOGGING
/** @brief Enable (1) or disable (0) the Logging feature. */
#define KISO_FEATURE_LOGGING 1
#endif

#ifndef KISO_FEATURE_RINGBUFFER
/** @brief Enable (1) or disable (0) the RingBuffer feature. */
#define KISO_FEATURE_RINGBUFFER 1
#endif

#ifndef KISO_FEATURE_SLEEPCONTROL
/** @brief Enable (1) or disable (0) the SleepControl feature. */
#define KISO_FEATURE_SLEEPCONTROL 0
#endif

#ifndef KISO_FEATURE_TASKMONITOR
/** @brief Enable (1) or disable (0) the TaskMonitor feature. */
#define KISO_FEATURE_TASKMONITOR 1
#endif

#if KISO_FEATURE_TASKMONITOR
    #ifndef KISO_TASKMONITOR_MAX_TASKS
    /** @brief Maximum number of TaskMonitor tickets to reserve for the system. */
    #define KISO_TASKMONITOR_MAX_TASKS 10
    #endif
#endif /* if KISO_FEATURE_TASKMONITOR */

#ifndef KISO_FEATURE_UARTTRANSCEIVER
/** @brief Enable (1) or disable (0) the UartTransceiver feature. */
#define KISO_FEATURE_UARTTRANSCEIVER 1
#endif

#ifndef KISO_FEATURE_I2CTRANSCEIVER
/** @brief Enable (1) or disable (0) the I2CTransceiver feature. */
#define KISO_FEATURE_I2CTRANSCEIVER 1
#endif

#ifndef KISO_FEATURE_XPROTOCOL
/** @brief Enable (1) or disable (0) the XProtocol feature. */
#define KISO_FEATURE_XPROTOCOL 1
#endif

#ifndef KISO_FEATURE_PIPEANDFILTER
/** @brief Enable (1) or disable (0) the pipe & filter pattern feature. */
#define KISO_FEATURE_PIPEANDFILTER 1
#endif

// clang-format on

#endif /* KISO_UTILSCONFIG_H_ */
