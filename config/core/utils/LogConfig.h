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
 *
 * @brief Configuration header for the Logging module.
 *
 */

#ifndef LOG_CONFIG_H
#define LOG_CONFIG_H

/* Enable logging extension macro */
#define KISO_LOGGING 1

/* Enable/Disable recorders */
#define KISO_SYNC_RECORDER 1
#define KISO_ASYNC_RECORDER 0

/* Enable /Disable appenders */
#define KISO_UART_APPENDER 1

/* Filter configuration */
/* Number of allowed filters >=1 */
#define LOG_FILTER_ITEM_COUNT (UINT8_C(1))
/* Default log level for package and module */
#define LOG_LEVEL_PACKAGE_DEFAULT (LOG_LEVEL_DEBUG)
#define LOG_LEVEL_MODULE_DEFAULT (LOG_LEVEL_DEBUG)

/* Configuration for the asynchronous-recorder (if activated) */
#if KISO_ASYNC_RECORDER == 1
#define LOG_BUFFER_SIZE (UINT16_C(256))
#define LOG_QUEUE_BUFFER_SIZE (UINT16_C(2048))
#define LOG_TASK_STACK_SIZE (UINT16_C(256))
#define LOG_TASK_PRIORITY (UINT8_C(4))

#define LOG_SYS_CLOCK_DELAY (10)
#define LOG_APPENDER_TIMEOUT (50)
#endif

/* Configuration for the synchronous-recorder (if activated) */
#if KISO_SYNC_RECORDER == 1
#define LOG_BUFFER_SIZE (UINT16_C(256))
#define LOG_APPENDER_TIMEOUT UINT32_C(1000)

#endif

#endif /* LOG_CONFIG_H */
