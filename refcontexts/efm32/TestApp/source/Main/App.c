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


/* module includes ********************************************************** */

#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_TEST_APP_MODULE_ID_APP

/* own header files */
#include "App.h"

/* additional interface header files */
#include "BCDS_CmdProcessor.h"
#include "System.h"
#include "FreeRTOS.h"
#include "task.h"

/* local variables ********************************************************** */

/** command Processor handle which holds the  commandprocessor name, Task and Queues Handles */
static CmdProcessor_T Application_cmdHandle;

/* global functions ********************************************************* */

Retcode_T App_Start(void)
{
    Retcode_T rc;

	rc = System_Initialize(&Application_cmdHandle);

	if (RETCODE_OK == rc)
	{
		/* vTaskStartScheduler() must never return.
		 * RTOS destroys the system
		 * stack context inside the function.
		 */
		vTaskStartScheduler();
	}

    return rc;
}

/** ************************************************************************* */
