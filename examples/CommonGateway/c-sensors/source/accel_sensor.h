/**********************************************************************************************************************
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
**********************************************************************************************************************/

/**
 * @file
 * @defgroup    NON_GROUPED
 * @ingroup     NON_GROUPED
 * @{
 *
 * @brief       Put brief description here.
 *
 * @details
 * Put detailed description here.
 */

#ifndef ACCEL_SENSOR_H_
#define ACCEL_SENSOR_H_

/*---------------------- INCLUDED HEADERS ---------------------------------------------------------------------------*/
#include "Kiso_I2CTransceiver.h"

/*---------------------- MACRO DEFINITIONS --------------------------------------------------------------------------*/

/*---------------------- TYPE DEFINITIONS ---------------------------------------------------------------------------*/

/*---------------------- FUNCTION DECLARATIONS ----------------------------------------------------------------------*/

Retcode_T Accelerometer_Init(I2cTranceiverHandlePtr_T i2cTransceiverRef);

/*---------------------- GLOBAL VARIABLES DECLARATION ---------------------------------------------------------------*/


#endif /* ACCEL_SENSOR_H_ */
/**@}*/
