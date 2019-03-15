/*----------------------------------------------------------------------------*/
/**
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 *
 * @file
 * 
 */
/*----------------------------------------------------------------------------*/

#ifndef BCDS_CELLULARSMS_TH_HH
#define BCDS_CELLULARSMS_TH_HH

#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"

#include "BCDS_Cellular_Sms.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CellularSms_Initialize, Sms_CallbackFunction_T);
FAKE_VALUE_FUNC(Retcode_T, CellularSms_Deinitialize);
FAKE_VALUE_FUNC(Retcode_T, CellularSms_SendText, uint8_t*, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSms_SetServiceCenter, uint8_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularSms_GetServiceCenter, uint8_t*, uint8_t*);

#endif /* BCDS_CELLULARSMS_TH_HH */
