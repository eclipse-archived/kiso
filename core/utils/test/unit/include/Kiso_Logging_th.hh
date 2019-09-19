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
 * @defgroup LOGGING Logging
 * @{
 *
 * @brief
 *      Mockup implementation for the Kiso_Logging_th.hh module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_LOGGING_TH_HH_
#define KISO_LOGGING_TH_HH_

/* Wrap Kiso_Logging interface header */
#include "Kiso_Logging.h"

/* Include gtest header file */
#include "gtest.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, Logging_Init, const LogRecorder_T*,
        const LogAppender_T*);
DECLARE_FAKE_VALUE_FUNC7_VARARG(Retcode_T, Logging_Log, LogLevel_T, uint8_t, uint8_t, const char*,
        uint32_t, const char*, ...);
DEFINE_FAKE_VALUE_FUNC7_VARARG(Retcode_T, Logging_Log, LogLevel_T, uint8_t, uint8_t, const char*,
        uint32_t, const char*, ...);
FAKE_VALUE_FUNC(LogFilterId_T, LogFilter_Add, LogLevel_T, uint8_t, uint8_t)
FAKE_VALUE_FUNC(Retcode_T, LogFilter_Delete, LogFilterId_T)
FAKE_VALUE_FUNC(Retcode_T, LogFilter_Configure, LogFilterId_T, LogLevel_T,
        uint8_t, uint8_t);
FAKE_VALUE_FUNC(bool, LogFilter_Apply, LogLevel_T, uint8_t, uint8_t)

#endif /* KISO_LOGGING_TH_HH_ */
