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


/* header definition ******************************************************** */
#ifndef APPCONFIG_H_
#define APPCONFIG_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */
#include "PriorityConfig.h"

/* @todo : 04 Jan 2016, mta5cob - The Task stack size and priority
 * are not precise. Need to analyze and provide a proper value. */
#define APPLICATION_CMD_TASK_STACK          UINT32_C(300)
#define APPLICATION_CMD_TASK_PRI            TASK_PRIORITY_APP
#define APPLICATION_TASK_QUEUE_LEN          UINT8_C(20)

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

#endif /* APPCONFIG_H_ */

/** ************************************************************************* */
