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
 * @brief Implemented first application function main() here.
 */

#include "AppInfo.h"
#include "BCDS_BSP_LED.h"
#include "BCDS_BSP_Board.h"
#include <stdio.h>
#include "BSP_CommonGateway.h"
#include "BCDS_Basics.h"
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sensors_common.h"
/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID CGW_APP_MODULE_ID_MAIN

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

Retcode_T systemStartup(void);

void ErrorHandler(Retcode_T error, bool isfromIsr);

void assertIndicationMapping(const unsigned long line, const unsigned char * const file);

static void SysTickPreCallback(void);

extern void xPortSysTickHandler(void);

/*---------------------- VARIABLES DECLARATIONS ---------------------------------------------------------------------*/

static CmdProcessor_T MainCmdProcessor;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

int main(void)
{
    /* Mapping Default Error Handling function */
    Retcode_T returnValue = Retcode_Initialize(ErrorHandler);
    if (RETCODE_OK == returnValue)
    {
        returnValue = systemStartup();
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue =
                CmdProcessor_Initialize(&MainCmdProcessor, (char *) "MainCmdProcessor", TASK_PRIO_MAIN_CMD_PROCESSOR, TASK_STACK_SIZE_MAIN_CMD_PROCESSOR, TASK_Q_LEN_MAIN_CMD_PROCESSOR);
    }
    if (RETCODE_OK == returnValue)
    {
        /* Here we enqueue the application initialization into the command
         * processor, such that the initialization function will be invoked
         * once the RTOS scheduler is started below.
         */
        returnValue = CmdProcessor_Enqueue(&MainCmdProcessor, appInitSystem, &MainCmdProcessor, UINT32_C(0));
    }
    if (RETCODE_OK != returnValue)
    {
        printf("System Startup failed");
        assert(false);
    }
    /* start scheduler */
    vTaskStartScheduler();
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
void ErrorHandler(Retcode_T error, bool isfromIsr)
{
    if (false == isfromIsr)
    {
        /** @TODO ERROR HANDLING SHOULD BE DONE FOR THE ERRORS RAISED FROM PLATFORM */
        uint32_t PackageID = Retcode_GetPackage(error);
        uint32_t ErrorCode = Retcode_GetCode(error);
        uint32_t ModuleID = Retcode_GetModuleId(error);
        Retcode_Severity_T SeverityCode = Retcode_GetSeverity(error);

        if (RETCODE_SEVERITY_FATAL == SeverityCode)
            printf("Fatal error from package %u , Error code: %u and module ID is :%u \r\n",(unsigned int) PackageID ,(unsigned int) ErrorCode, (unsigned int) ModuleID);

        if (RETCODE_SEVERITY_ERROR == SeverityCode)
            printf("Severe error from package %u , Error code: %u and module ID is :%u \r\n",(unsigned int) PackageID , (unsigned int) ErrorCode, (unsigned int) ModuleID);
    }
    else
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_RED_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }

}

#ifndef NDEBUG /* valid only for debug builds */
/**
 * @brief This API is called when function enters an assert
 *
 * @param[in] line : line number where asserted
 * @param[in] file : file name which is asserted
 *
 */

void assertIndicationMapping(const unsigned long line, const unsigned char * const file)
{
    /* Switch on the LEDs */
    Retcode_T retcode = RETCODE_OK;

    retcode = BSP_LED_Switch(COMMONGATEWAY_LED_ALL,COMMONGATEWAY_LED_COMMAND_ON);

    printf("asserted at Filename %s , line no  %ld \n\r", file, line);

    if (retcode != RETCODE_OK)
    {
        printf("LED's ON failed during assert");
    }

}
#endif

Retcode_T systemStartup(void)
{
    Retcode_T returnValue = RETCODE_OK;
    uint32_t param1 = 0;
    void* param2 = NULL;

    /* Initialize the callbacks for the system tick */
    BSP_Board_OSTickInitialize(SysTickPreCallback, NULL);

#ifndef NDEBUG /* valid only for debug builds */
    if (RETCODE_OK == returnValue)
    {
        returnValue = Assert_Initialize(assertIndicationMapping);
    }
#endif
    /* First we initialize the board. */
    if (RETCODE_OK == returnValue)
    {
        returnValue = BSP_Board_Initialize(param1, param2);
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = BSP_LED_Connect();
    }

    if (RETCODE_OK == returnValue)
    {
        returnValue = BSP_LED_Enable(COMMONGATEWAY_LED_ALL);
    }
    return returnValue;
}

/**
 * @brief This function is SysTick Handler.
 * This is called when ever the IRQ is hit.
 * This is a temporary implementation where
 * the SysTick_Handler() is not directly mapped
 * to xPortSysTickHandler(). Instead it is only
 * called if the scheduler has started.
 */
static void SysTickPreCallback(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}
