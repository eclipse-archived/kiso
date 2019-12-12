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
#define KISO_FEATURE_CMDLINEDEBUGGER 1
#define KISO_FEATURE_CMDPROCESSOR    1
#define KISO_FEATURE_CRC             1
#define KISO_FEATURE_EVENTHUB        1
#define KISO_FEATURE_GUARDEDTASK     1
#define KISO_FEATURE_ERRORLOGGER     1
#define KISO_FEATURE_LOGGING         1
#define KISO_FEATURE_RINGBUFFER      1
#define KISO_FEATURE_SLEEPCONTROL    0
#define KISO_FEATURE_TASKMONITOR     1
#define KISO_TASKMONITOR_MAX_TASKS   10
#define KISO_FEATURE_UARTTRANSCEIVER 1
#define KISO_FEATURE_I2CTRANSCEIVER  1
#define KISO_FEATURE_XPROTOCOL       1
#define KISO_FEATURE_PIPEANDFILTER   1
// clang-format on

#endif /* KISO_UTILSCONFIG_H_ */
