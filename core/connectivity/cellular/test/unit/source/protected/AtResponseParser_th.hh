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

#ifndef ATRESPONSEPARSER_TH_HH_
#define ATRESPONSEPARSER_TH_HH_

#include <gtest.h>

#include "AtResponseParser.h"

FAKE_VOID_FUNC(AtResponseParser_RegisterResponseCodeCallback, AtrpEventWithResponseCodeCallback_T)
FAKE_VOID_FUNC(AtResponseParser_RegisterErrorCallback, AtrpEventCallback_T)
FAKE_VOID_FUNC(AtResponseParser_RegisterCmdEchoCallback, AtrpEventWithDataCallback_T)
FAKE_VOID_FUNC(AtResponseParser_RegisterCmdCallback, AtrpEventWithDataCallback_T)
FAKE_VOID_FUNC(AtResponseParser_RegisterCmdArgCallback, AtrpEventWithDataCallback_T)
FAKE_VOID_FUNC(AtResponseParser_RegisterMiscCallback, AtrpEventWithDataCallback_T)
FAKE_VOID_FUNC(AtResponseParser_Reset)
FAKE_VALUE_FUNC(Retcode_T, AtResponseParser_Parse, uint8_t*, uint32_t)

#endif /* ATRESPONSEPARSER_TH_HH_ */
