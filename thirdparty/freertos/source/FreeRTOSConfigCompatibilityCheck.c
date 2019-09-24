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
 * 	The module validates FreeRTOSConfig.h compatibility with the FreeRTOS kernel used.
 *
 */

/* module includes ********************************************************** */

/* system header files */

/* additional interface header files */
#include "FreeRTOS.h"
#include "task.h"

/* own header files */

/* configuration tests ****************************************************** */

#if (KISO_FREE_RTOS_VERSION_MAJOR != tskKERNEL_VERSION_MAJOR) || \
    (KISO_FREE_RTOS_VERSION_MINOR != tskKERNEL_VERSION_MINOR) || \
    (KISO_FREE_RTOS_VERSION_BUILD != tskKERNEL_VERSION_BUILD)
#error "The FreeRTOS library and the FreeRTOSConfig.h provided for the build have a version mismatch."
#endif

/* constant definitions ***************************************************** */

/* function prototypes ****************************************************** */

/* local variables ********************************************************** */

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/* global functions ********************************************************* */

/** ************************************************************************* */
