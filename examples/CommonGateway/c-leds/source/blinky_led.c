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
 * @ingroup
 *
 * @defgroup
 * @{
 *
 * @brief Blinky LED example
 *
 * @details Demo application demonstrates how to use the LED API from KISO.
 *
 **/

/* own header files */
#include "AppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID APP_MODULE_ID_BLINKY_LED

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "blinky_led.h"
#include "BSP_CommonGateway.h"
#include "BCDS_BSP_LED.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "BCDS_Logging.h"
#include "SEGGER_RTT.h"

void blink_led(void* param1, uint32_t param2);

/*Application Command Processor Instance */
CmdProcessor_T *AppCmdProcessor;

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
	(void)param2;
	Retcode_T returnVal = RETCODE_OK;

	// Initialize logging module

	returnVal = Logging_Init(Logging_SyncRecorder, Logging_UARTAppender);
    if (RETCODE_OK == returnVal)
    {
    	LOG_DEBUG(" Logging was started successfully");
    }

	// Initialize the blinking led
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    AppCmdProcessor = (CmdProcessor_T *) CmdProcessorHandle;


    if (RETCODE_OK == returnVal)
    {
        CmdProcessor_Enqueue((CmdProcessor_T*)AppCmdProcessor,blink_led,(void *)AppCmdProcessor,0);
    }
    if (RETCODE_OK != returnVal)
    {
        printf("Error in Initializing the BLINKY LED APPLICATION \n ");
    }
    else
    {
        printf("Successful Initialize of BLINKY LED APPLICATION \n ");
    }
}

void blink_led(void* param1, uint32_t param2)
{
    (void)param1;
    (void)param2;
	BSP_LED_Switch(COMMONGATEWAY_LED_BLUE_ID, COMMONGATEWAY_LED_COMMAND_TOGGLE);
	LOG_DEBUG("Led switch");
    vTaskDelay(500);
    CmdProcessor_Enqueue((CmdProcessor_T*)param1,blink_led,param1,0);
}
/**@} */

/** ************************************************************************* */

