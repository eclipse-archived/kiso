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

/**
 * @file
 *
 * @brief Provision to send text message to the given mobile number by the user.
 *
 * @details A typical usage for sending the text message as shown below.
 * 1. Initialization
 * 2. Send text message (Send the text message only after GSM registration)
 * 3. De-Initialize.
 *
 * This module provides a callback for the SMS event
 **/

 
#ifndef QCTEL_SMS_H
#define QCTEL_SMS_H

#include "BCDS_Basics.h"


/**
 * @brief API is as a callback for SMS event
 *
 * @param[in] - Type  			- Event type
 * @param[in] - Payload_Ptr  	- Event data received
 *
 */
void SmsEventNotifyCB(uint8_t Type, uint8_t* Payload_Ptr);

#endif /* QCTEL_SMS_H */
