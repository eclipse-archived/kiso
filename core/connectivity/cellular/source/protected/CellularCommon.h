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
 * @brief Internal header that defines return codes for cellular module.
 *
 */

#ifndef CELLULAR_COMMON_H
#define CELLULAR_COMMON_H

#include "BCDS_Retcode.h"

/**
 * @brief Return codes issued by cellular module functions
 */
typedef enum
{
	RETCODE_CELLULAR_NOT_RESPONSIVE = RETCODE_FIRST_CUSTOM_CODE,//!< RETCODE_CELLULAR_NOT_RESPONSIVE
	RETCODE_CELLULAR_RESPONDED_ERROR,                           //!< RETCODE_CELLULAR_RESPONDED_ERROR
	RETCODE_CELLULAR_RESPONDED_ABORTED,                         //!< RETCODE_CELLULAR_RESPONDED_ABORTED
	RETCODE_CELLULAR_RESPONDED_OTHER,                           //!< RETCODE_CELLULAR_RESPONDED_OTHER
	RETCODE_CELLULAR_RESPONSE_UNEXPECTED,                       //!< RETCODE_CELLULAR_RESPONSE_UNEXPECTED
	RETCODE_CELLULAR_IN_UNKNOWN_STATE,                          //!< RETCODE_CELLULAR_IN_UNKNOWN_STATE
	RETCODE_CELLULAR_URC_NOT_PRESENT,                           //!< RETCODE_CELLULAR_URC_NOT_PRESENT
	RETCODE_CELLULAR_NETWORK_ERROR,                             //!< RETCODE_CELLULAR_NETWORK_ERROR
	RETCODE_CELLULAR_SHUTDOWN_FAILURE,                          //!< RETCODE_CELLULAR_SHUTDOWN_FAILURE
	RETCODE_CELLULAR_DRIVER_BUSY,                               //!< RETCODE_CELLULAR_DRIVER_BUSY
	RETCODE_CELLULAR_SOCKET_INVALID_ID,                         //!< RETCODE_CELLULAR_SOCKET_INVALID_ID
	RETCODE_CELLULAR_SOCKET_BIND_FAILED,                        //!< RETCODE_CELLULAR_SOCKET_BIND_FAILED
	RETCODE_CELLULAR_DRIVER_ERROR_CMD_ARG,                      //!< RETCODE_CELLULAR_DRIVER_ERROR_CMD_ARG
	RETCODE_CELLULAR_SIM_NOT_READY,                             //!< RETCODE_CELLULAR_SIM_NOT_READY

	RETCODE_CELLULAR_LAST_CUSTOM_CODE                           //!< RETCODE_CELLULAR_LAST_CUSTOM_CODE
} CellularResponseCode_T;

/**
 * @brief Notifies socket data bytes available for reading
 */
void CellularSocket_NotifyDataReceived(void);

#endif /* CELLULAR_COMMON_H */
