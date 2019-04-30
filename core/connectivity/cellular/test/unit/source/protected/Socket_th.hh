/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/
 
#ifndef CELLULAR_SOCKET_TH_HH
#define CELLULAR_SOCKET_TH_HH

/* Include the needed headers */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "Socket.h"

/* Mock-ups for the provided interfaces */
#if quectel == BCDS_CELLULAR_PLATFORM
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Open, CellularSocket_OpenRequest_T *);
#elif ublox == BCDS_CELLULAR_PLATFORM
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Open, CellularUdpSocket_T *, CellularSocket_Type_T, CellularUdpSocket_Callback_T);
#endif
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Close, CellularUdpSocket_T);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Send, CellularSocket_Handle_T, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_SendTo, CellularUdpSocket_T, uint32_t, uint16_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Receive, CellularSocket_Handle_T, uint8_t*, uint32_t, uint32_t*, uint32_t*, uint16_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_ReceiveFrom, CellularUdpSocket_T, uint32_t *, uint16_t *, uint8_t **, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_GetPeer, CellularSocket_Handle_T, uint32_t*, uint16_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_GetTcpStatus, CellularSocket_Handle_T, CellularSocket_TcpStatus_T*);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_GetStatus, CellularSocket_Handle_T, CellularSocket_Status_T*);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Connect, CellularSocket_Handle_T, uint32_t, uint16_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSocket_Disconnect, CellularSocket_Handle_T);
FAKE_VOID_FUNC(CellularSocket_InvalidateAll);
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUrc_UUSOCL);
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUrc_UUSORD);
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUrc_UUSORF);
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUrc_QIURC);

#endif /* CELLULAR_SOCKET_TH_HH */
