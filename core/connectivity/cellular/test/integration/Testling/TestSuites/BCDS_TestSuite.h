/*****************************************************************************/
/*
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *
 *  The interface header exports the following features:
 *  -PowerControlTestSuite_Init()
 *  -NetworkTestSuite_Init()
 *  -SocketTestSuite_Init()
 *
 * ****************************************************************************/

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

