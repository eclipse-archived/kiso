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
 * @defgroup    KISO_C_EXAMPLE_SENSORS Sensors Example
 * @ingroup     KISO_EXAMPLES
 * @{
 *
 * @brief       Demo application featuring basic usage of sensors.
 *
 * @details     Demonstrates usage of Kiso APIs for working with accelerometer and environment sensors 
 *              in CommonGateway board.
 * 
 */

#ifndef SENSORS_COMMON_H_
#define SENSORS_COMMON_H_

/*---------------------- INCLUDED HEADERS ---------------------------------------------------------------------------*/

#include "Kiso_Basics.h"

/*---------------------- MACRO DEFINITIONS --------------------------------------------------------------------------*/

/*---------------------- TYPE DEFINITIONS ---------------------------------------------------------------------------*/

/*---------------------- FUNCTION DECLARATIONS ----------------------------------------------------------------------*/

/**
 * @brief This is a template function where the user can write his custom application.
 *
 * @param[in] CmdProcessorHandle Handle of the main command-processor
 *
 * @param[in] param2  Currently not used will be used in future
 *
 */
void appInitSystem(void *CmdProcessorHandle, uint32_t param2);

#endif /* SENSORS_COMMON_H_ */
/**
 * @}
 */
