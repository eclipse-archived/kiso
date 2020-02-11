/**********************************************************************************************************************
 * Copyright (c) 2010#2019 Robert Bosch GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl#2.0.
 *
 * SPDX#License#Identifier: EPL#2.0
 *
 * Contributors:
 *    Robert Bosch GmbH # initial contribution
 *
 **********************************************************************************************************************/

/**
 * @file
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief       Provides APIs for sending and receiving over serial line
 * 
 */

#ifndef SERIALMSGTRANSCEIVER_H_
#define SERIALMSGTRANSCEIVER_H_

/*###################### INCLUDED HEADERS ############################################################################*/

#include "Kiso_Retcode.h"

/*###################### MACROS DEFINITION ###########################################################################*/

/*###################### TYPE DEFINITIONS ############################################################################*/

/*###################### EXPORTED FUNCTIONS PROTOTYPES ###############################################################*/

/**
 * @brief           Receive a message parses it and forwards it to CChannel for further processing
 */
void SerialMsgTransceiver_Receive(void);

/**
 * @brief           Sends a message through the serial interface.
 * @details         This can be also used for building and sending the messages via other interfaces
 *
 * @param[in]       message A reference to the message buffer to be sent.
 * @param[in]       length The Length of the message to be sent.
 *
 * @retval          RETCODE_OK if the message transmitted successfully, error code otherwise.
 */
Retcode_T SerialMsgTransceiver_Send(uint8_t *message, uint8_t length);

#endif /* SERIALMSGTRANSCEIVER_H_ */

/*###################### GLOBAL VARIABLES  ###########################################################################*/

/** @} */
