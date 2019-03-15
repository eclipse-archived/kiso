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
