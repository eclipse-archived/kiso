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
 *
 *  Mockup implementation for the FreeRTOSConfig file.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef KISO_FREERTOS_CONFIG_TH_HH_
#define KISO_FREERTOS_CONFIG_TH_HH_

/* include system header files */

/* wrap the real FreeRTOSConfig.h header */
#include "FreeRTOSConfig.h"

#undef configASSERT
#define configASSERT(expression) if (!(expression)) {assert(pdFAIL); }

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

#endif /* KISO_FREERTOS_CONFIG_TH_HH_ */

/** ************************************************************************* */
