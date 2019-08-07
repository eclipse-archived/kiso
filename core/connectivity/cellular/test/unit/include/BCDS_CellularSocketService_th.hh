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

#ifndef BCDS_CELLULARSOCKETSERVICE_TH_HH_
#define BCDS_CELLULARSOCKETSERVICE_TH_HH_

#include "BCDS_CellularSocketService.h"

#include "gtest.h"

FAKE_VALUE_FUNC(Retcode_T, CellularSocket_CreateAndBind, CellularSocket_Handle_T*, const Cellular_DataContext_T*, uint16_t, CellularSocket_Protocol_T, CellularSocket_NotifySocketClosed_T, CellularSocket_NotifyDataReady_T);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Connect, CellularSocket_Handle_T, const Cellular_IpAddress_T*, uint16_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Listen, CellularSocket_Handle_T, uint16_t, CellularSocket_NotifyConnectionAccepted_T);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Send, CellularSocket_Handle_T, const uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_SendTo, CellularSocket_Handle_T, const uint8_t*, uint32_t, const Cellular_IpAddress_T*, uint16_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Receive, CellularSocket_Handle_T, uint8_t*, uint32_t, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_ReceiveFrom, CellularSocket_Handle_T, uint8_t*, uint32_t, uint32_t*, Cellular_IpAddress_T*, uint16_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_QueryBytesAvailable, CellularSocket_Handle_T, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Close,CellularSocket_Handle_T);

#endif /* BCDS_CELLULARSOCKETSERVICE_TH_HH_ */
