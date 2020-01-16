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
 * @ingroup     KISO_C_EXAMPLE_SENSORS
 * @defgroup    ENVIRONMEMTAL_SENSOR Environmental sensor
 * @{
 *
 * @brief       Environmental sensor code for the sensors example.
 *
 * @details
 *              Demonstrates usage of Environmental sensor BME280 in Kiso for STM32-Nucleo-L4R5ZI board.
 * @file
 */

#ifndef ENV_SENSOR_H_
#define ENV_SENSOR_H_

/*---------------------- INCLUDED HEADERS ---------------------------------------------------------------------------*/
#include "Kiso_I2CTransceiver.h"
#include "Kiso_Retcode.h"

/*---------------------- MACRO DEFINITIONS --------------------------------------------------------------------------*/

/*---------------------- TYPE DEFINITIONS ---------------------------------------------------------------------------*/

/*---------------------- FUNCTION DECLARATIONS ----------------------------------------------------------------------*/

Retcode_T EnvSensor_init(void);

/*---------------------- GLOBAL VARIABLES DECLARATION ---------------------------------------------------------------*/

#endif /* ENV_SENSOR_H_ */
/**@}*/
