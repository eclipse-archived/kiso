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


#ifndef BCDS_TESTSUITE_H_
#define BCDS_TESTSUITE_H_

/* public interface declaration */
#include "BCDS_Cellular.h"
#include "BCDS_CellularUdpSocket.h"
#include "BCDS_CellularSms.h"

/* public type and macro definitions */

/* local function prototype declarations */

/* public function prototype declarations */

/**
 * @brief  Gives the status of the Cellular events
 *
 * @param[in] event - Cellular event
 *
 * @param[in] info - Information pertaining to the Cellular event.
 */

void ParseEventCallback(Cellular_CallbackEvent_T event, void *info);

/**
 * @brief  Gives the status of the SMS events
 *
 * @param[in] event - SMS event
 *
 * @param[in] Info - Information pertaining to the SMS event.
 */
void SmsEventCallback(Sms_Event_T Event, void* Info);

/**
 * @brief  Gives the status of the Cellular events
 *
 * @param[in] event - Cellular event
 *
 * @param[in] info
 */

void ParseSocketCallback(CellularUdpSocket_T socket, CellularUdpEvent_T event);
/**
 * @brief  Gives the status of the Cellular events
 *
 * @param[in] event - Cellular event
 *
 * @param[in] info
 */

void RobustnessParseSocketCallback(CellularUdpSocket_T socket, CellularUdpEvent_T event);
/**
 * @brief  Gives the status of the Cellular events
 *
 * @param[in] event - Cellular event
 *
 * @param[in] info
 */

void RobustnessParseEventCallback(Cellular_CallbackEvent_T event, void *info);

/* local module global variable declarations */

/* local inline function definitions */

#endif /* BCDS_TESTSUITE_H_ */

