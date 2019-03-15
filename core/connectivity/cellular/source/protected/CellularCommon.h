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
