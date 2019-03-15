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
