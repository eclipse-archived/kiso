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

#ifndef SOCKETSERVICE_TH_HH_
#define SOCKETSERVICE_TH_HH_

#include "SocketService.h"

#include "gtest.h"

FAKE_VOID_FUNC(SocketService_NotifySocketDataReceived, uint32_t, uint32_t);
FAKE_VOID_FUNC(SocketService_NotifySocketAccepted, uint32_t, const AT_UBlox_Address_T*, uint16_t, uint32_t, const AT_UBlox_Address_T*, uint16_t);
FAKE_VOID_FUNC(SocketService_NotifySocketClosed, uint32_t);

#endif /* SOCKETSERVICE_TH_HH_ */
