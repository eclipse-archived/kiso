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

#ifndef LOG_TH_HH
#define LOG_TH_HH

/* Include all headers which are needed by this file. */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"

#undef BCDS_LOGGING

#include "BCDS_Logging.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, Log_Init);
FAKE_VOID_FUNC(Log_InitHardware, void *);
FAKE_VOID_FUNC(Log_SendMessage, const char *, uint32_t);

#endif /* LOG_TH_HH */
