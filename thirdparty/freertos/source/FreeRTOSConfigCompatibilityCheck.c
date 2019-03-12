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

#if (BCDS_FREE_RTOS_VERSION_MAJOR != tskKERNEL_VERSION_MAJOR) || \
	(BCDS_FREE_RTOS_VERSION_MINOR != tskKERNEL_VERSION_MINOR) || \
	(BCDS_FREE_RTOS_VERSION_BUILD != tskKERNEL_VERSION_BUILD)
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
