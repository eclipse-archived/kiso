/*----------------------------------------------------------------------------*/
/**
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 *
 * @file
 * 
 */
/*----------------------------------------------------------------------------*/

#ifndef BCDS_CELLULARUDPSOCKET_TH_HH
#define BCDS_CELLULARUDPSOCKET_TH_HH

#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"

#include "BCDS_Cellular_Ip.h"

/* Mock-ups for deprecated interfaces */
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUdp_Open, CellularUdpSocket_T*, CellularUdpSocket_Callback_T);
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUdp_Listen, CellularUdpSocket_T, uint16_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUdp_Receive, CellularUdpSocket_T, uint32_t*, uint16_t*, uint8_t**, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUdp_Close, CellularUdpSocket_T);
FAKE_VALUE_FUNC(Retcode_T, CellularSocketUdp_Send, CellularUdpSocket_T, const uint32_t, uint16_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, Cellular_ResolveName, uint8_t*, uint32_t, uint32_t*);

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CellularUdpSocket_Open, CellularUdpSocket_T*, CellularUdpSocket_Callback_T);
FAKE_VALUE_FUNC(Retcode_T, CellularUdpSocket_Listen, CellularUdpSocket_T, uint16_t);
FAKE_VALUE_FUNC(Retcode_T, CellularUdpSocket_Receive, CellularUdpSocket_T, uint32_t*, uint16_t*, uint8_t**, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularUdpSocket_Close, CellularUdpSocket_T);
FAKE_VALUE_FUNC(Retcode_T, CellularUdpSocket_Send, CellularUdpSocket_T, const uint32_t, uint16_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularUdpSocket_ResolveIp, uint8_t*, uint32_t, uint32_t*);

#endif /* BCDS_CELLULARUDPSOCKET_TH_HH */
