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
 
#ifndef BCDS_LOGGING_TH_HH_
#define BCDS_LOGGING_TH_HH_

/* ban or wrap the real interface header */
#include "BCDS_Logging.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, Logging_Init, const LogRecorder_T*,
        const LogAppender_T*);
DECLARE_FAKE_VALUE_FUNC7_VARARG(Retcode_T, Logging_Log, LogLevel_T, uint8_t, uint8_t, const char*,
        uint32_t, const char*, ...);
DEFINE_FAKE_VALUE_FUNC7_VARARG(Retcode_T, Logging_Log, LogLevel_T, uint8_t, uint8_t, const char*,
        uint32_t, const char*, ...);
FAKE_VALUE_FUNC(LogFilterId_T, LogFilter_Add, LogLevel_T, uint8_t, uint8_t);
FAKE_VALUE_FUNC(Retcode_T, LogFilter_Delete, LogFilterId_T);
FAKE_VALUE_FUNC(Retcode_T, LogFilter_Configure, LogFilterId_T, LogLevel_T,
        uint8_t, uint8_t);
FAKE_VALUE_FUNC(bool, LogFilter_Apply, LogLevel_T, uint8_t, uint8_t);

#endif /* BCDS_LOGGING_TH_HH_ */
