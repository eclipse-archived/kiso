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
 *
 */
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
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_LED_BUTTON

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "blinky_led.h"
#include "BSP_Sensgate.h"
#include "BCDS_BSP_LED.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "timers.h"

static Retcode_T LedInitialize(void);

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
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    AppCmdProcessor = (CmdProcessor_T *) CmdProcessorHandle;
    BCDS_UNUSED(param2);
    Retcode_T returnVal = RETCODE_OK;

    returnVal = LedInitialize();

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
/* Routine to Initialize the LED */
static Retcode_T LedInitialize(void)
{
    Retcode_T returnVal = RETCODE_OK;
    returnVal = BSP_LED_Connect();
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Enable((uint32_t) SENSGATE_LED_ALL);
    }
    if (RETCODE_OK == returnVal)
    {
        printf("LED Initialization succeed without error %u \n", (unsigned int) returnVal);
    }
    else
    {
        printf(" Error occurred in LED Initialization routine %u \n", (unsigned int) returnVal);
    }
    return returnVal;
}



void blink_led(void* param1, uint32_t param2)
{
    (void)param2;
	BSP_LED_Switch(SENSGATE_LED_BLUE_ID, SENSGATE_LED_COMMAND_TOGGLE);
    vTaskDelay(500);
    CmdProcessor_Enqueue((CmdProcessor_T*)param1,blink_led,param1,0);
}
/**@} */

/** ************************************************************************* */

