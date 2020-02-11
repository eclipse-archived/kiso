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
 * @brief       Serial Communication Channel Interface
 * @details     This header provides APIs to control uart communication line if this one is used as a communication 
 *              channel between the Test executor and the test controller.
 */

#ifndef SERIALCCHANNEL_H_
#define SERIALCCHANNEL_H_

/*###################### INCLUDED HEADERS ############################################################################*/

#include "Kiso_Retcode.h"

/*###################### MACROS DEFINITION ###########################################################################*/

/*###################### TYPE DEFINITIONS ############################################################################*/

/*###################### EXPORTED FUNCTIONS PROTOTYPES ###############################################################*/

/**
 * @brief           Initializes the serial interface
 *
 * @return           RETCODE_OK if Initialized successfully error code otherwise.
 */
Retcode_T Serial_Initialize(void);

/**
 * @brief           Deinitializes the serial interface
 *
 * @return          RETCODE_OK if Deinitialized successfully error code otherwise.
 */
Retcode_T Serial_Deinitialize(void);

/**
 * @brief           Receives data from the serial interface
 *
 * @param[in]       data: A pointer to a data buffer.
 * @param[in]       len: Number of bytes to be received.
 *
 * @return          RETCODE_OK if Receive successful error code otherwise.
 */
Retcode_T Serial_Receive(void *data, uint32_t len);

/**
 * @brief           Sends data to the serial interface
 *
 * @param[in]       data: A pointer to a data buffer.
 * @param[in]       len: Number of bytes to be sent.
 *
 * @return          RETCODE_OK if Send successful error code otherwise
 */
Retcode_T Serial_Send(void *data, uint32_t len);

/*###################### GLOBAL VARIABLES  ###########################################################################*/

extern volatile uint32_t serialReceivedCnt;

#endif /* SERIALCCHANNEL_H_ */
