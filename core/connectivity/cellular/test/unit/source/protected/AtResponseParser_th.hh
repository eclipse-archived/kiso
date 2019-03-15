/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

#ifndef ATRESPONSEPARSER_TH_HH_
#define ATRESPONSEPARSER_TH_HH_

/* Include the needed headers */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "Common_th.hh"
#include "CellularAtResponseParser.h"

/* Mock-ups for the provided interfaces */
FAKE_VOID_FUNC(AtResponseParser_RegisterResponseCodeCallback, AtrpEventWithResponseCodeCallback_T);
FAKE_VOID_FUNC(AtResponseParser_RegisterErrorCallback, AtrpEventCallback_T);
FAKE_VOID_FUNC(AtResponseParser_RegisterCmdEchoCallback, AtrpEventWithDataCallback_T);
FAKE_VOID_FUNC(AtResponseParser_RegisterCmdCallback, AtrpEventWithDataCallback_T);
FAKE_VOID_FUNC(AtResponseParser_RegisterCmdArgCallback, AtrpEventWithDataCallback_T);
FAKE_VOID_FUNC(AtResponseParser_RegisterMiscCallback, AtrpEventWithDataCallback_T);
FAKE_VOID_FUNC(AtResponseParser_Reset);
FAKE_VALUE_FUNC(Retcode_T, AtResponseParser_Parse, uint8_t*, uint32_t);

#endif /* ATRESPONSEPARSER_TH_HH_ */
