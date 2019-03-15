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

#ifndef CELLULAR_NETWORK_TH_HH
#define CELLULAR_NETWORK_TH_HH

#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "Network.h"

/* Mock-ups for the provided interfaces */
FAKE_VOID_FUNC(CellularNetwork_RegisterUrcCregCallback, CellularNetwork_UrcCregCallback_T);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_QueryCellEnvironment, Cellular_CellEnvironmentData_T*);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_QueryRegistrationStatus, CellularNetwork_RegistrationStatus_T*);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_ConfigureRegistrationInformation, CellularNetwork_RegistrationInfo_T);
FAKE_VALUE_FUNC(Retcode_T, Cellular_DeregisterFromNetwork);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_EnableAutomaticSelection);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_QueryCurrentOperator, CellularNetwork_CurrentOperator_T*);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_SetAPN, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_ActivatePdpContext);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_DeactivatePdpContext);
FAKE_VALUE_FUNC(Retcode_T, Cellular_VerifyPdpContext, bool*);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_SelectRadioAccessTechnology, CellularNetwork_RadioAccessTechnology_T*);
FAKE_VALUE_FUNC(Retcode_T, CellularNetwork_QueryRadioAccessTechnology, CellularNetwork_RadioAccessTechnology_T*);
FAKE_VALUE_FUNC(Retcode_T, CellularNetworkUrc_CREG);
FAKE_VALUE_FUNC(Retcode_T, CellularNetworkUrc_UUPSDA);
FAKE_VALUE_FUNC(Retcode_T, CellularNetworkUrc_UUPSDD);

#endif /* CELLULAR_NETWORK_TH_HH */
