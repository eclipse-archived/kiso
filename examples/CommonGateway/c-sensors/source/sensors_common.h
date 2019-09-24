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
/**@}*/
