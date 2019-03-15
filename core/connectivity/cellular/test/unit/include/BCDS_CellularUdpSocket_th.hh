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
