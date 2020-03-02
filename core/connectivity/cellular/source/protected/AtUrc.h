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
 * @ingroup KISO_CELLULAR_COMMON
 * @defgroup ATURC URC
 * @{
 *
 * @brief Defines prototypes for various URC handling routines.
 *
 * @details Defines prototypes for various URC (unsolicited response code)
 * handling routines, which are implemented in other driver modules. As such
 * modules generally include a reference to the Engine (which implements URC
 * handling), we have to prevent a circular dependency - thus we can't simply
 * include the headers of the other modules in Engine.
 *
 * @file
 */

#ifndef ATURC_H_
#define ATURC_H_

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

typedef Retcode_T (*CellularUrcHandler_T)(void);

/**
 * @brief Triggers the handling of unsolicited response code events in the AT
 * response queue (if there are any).
 *
 * @retval RETCODE_OK If all URC handlers returned successfully. One URC handler
 * failing will not prevent the others from being called.
 *
 * @retval RETCODE_CELLULAR_URC_NOT_PRESENT If no URC handler had to do any
 * work.
 */
Retcode_T Urc_HandleResponses(void);

#endif /* ATURC_H_ */

/** @} */
