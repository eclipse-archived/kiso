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

#ifndef CELLULAR_CONTROL_TH_HH
#define CELLULAR_CONTROL_TH_HH

/* Include all headers which are needed by this file. */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "Control.h"

/* Put the function declarations here */
FAKE_VALUE_FUNC(Retcode_T, Cellular_Startup);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Shutdown);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Reset);
FAKE_VALUE_FUNC(Retcode_T, Cellular_GetPowerStatus, Cellular_PowerStatus_T *);
FAKE_VALUE_FUNC(Retcode_T, Cellular_IsSimCardInserted, bool *);
FAKE_VALUE_FUNC(Retcode_T, Cellular_GetIccid, uint8_t *, uint32_t, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_EnableEcho, bool);

#endif /* CELLULAR_CONTROL_TH_HH */
