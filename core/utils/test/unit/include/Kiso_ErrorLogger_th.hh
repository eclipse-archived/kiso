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
 *      Mockup implementation for the Kiso_ErrorLogger_th.hh module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_ERRORLOGGER_HH_
#define KISO_ERRORLOGGER_HH_

/* Include gtest header file */
#include "gtest.h"

/* Wrap Kiso_ErrorLogger interface header */
#include "Kiso_ErrorLogger.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_Init , ErrorLoggerConfig_T )
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_LogError,Retcode_T)
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_GetLastErrorLog,ErrorLogger_LogEntry_T *)
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_HasError,Retcode_T)
FAKE_VALUE_FUNC(uint16_t,ErrorLogger_GetTotalErrors)
FAKE_VALUE_FUNC(uint8_t,ErrorLogger_GetCurrentErrors)
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_GetErrorAt,uint8_t,ErrorLogger_LogEntry_T *)
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_ClearAllErrorLogs)

#endif /* KISO_ERRORLOGGER_HH_*/
