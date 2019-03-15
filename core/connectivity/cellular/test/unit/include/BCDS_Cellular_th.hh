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

#ifndef BCDS_CELLULAR_TH_HH
#define BCDS_CELLULAR_TH_HH

#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "BCDS_Cellular.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, Cellular_Initialize, Cellular_CallbackFunction_T);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Uninitialize);
FAKE_VALUE_FUNC(Cellular_DeviceStatus_T, Cellular_GetDeviceStatus);
FAKE_VALUE_FUNC(Retcode_T, Cellular_QueryModem, Cellular_QueryInfo_T, void*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Control, Cellular_Control_T, void*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_NetworkInit);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Connect);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Disconnect);
FAKE_VALUE_FUNC(Retcode_T, Cellular_PowerControl, Cellular_PowerState_T);

#endif /* BCDS_CELLULAR_TH_HH */
