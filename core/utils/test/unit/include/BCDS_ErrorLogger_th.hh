/****************************************************************************/
/**
 *  Copyright (C) Bosch Connected Devices and Solutions.
 *  All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  Mockup implementation for the BCDS_ErrorLogger module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_ERRORLOGGER_HH_
#define BCDS_ERRORLOGGER_HH_

/* include system header files */

/* wrap the real interface header */
#include "BCDS_ErrorLogger.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_Init , ErrorLoggerConfig_T );
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_LogError,Retcode_T);
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_GetLastErrorLog,ErrorLogger_LogEntry_T *);
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_HasError,Retcode_T);
FAKE_VALUE_FUNC(uint16_t,ErrorLogger_GetTotalErrors);
FAKE_VALUE_FUNC(uint8_t,ErrorLogger_GetCurrentErrors);
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_GetErrorAt,uint8_t,ErrorLogger_LogEntry_T *);
FAKE_VALUE_FUNC(Retcode_T,ErrorLogger_ClearAllErrorLogs);

#endif /* BCDS_ERRORLOGGER_HH_*/
