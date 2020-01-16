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
 * @brief Implemented first application function main() here.
 */

#include "AppInfo.h"
#include "blinky_led.h"
#include "Kiso_BSP_LED.h"
#include "Kiso_BSP_Board.h"
#include <stdio.h>

#include "Kiso_Basics.h"
#include "Kiso_Assert.h"
#include "Kiso_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_NucleoL4R5ZI.h"
/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID /* Module ID define before including Basics package*/
#define KISO_MODULE_ID APP_MODULE_ID_MAIN

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

Retcode_T systemStartup(void);

void ErrorHandler(Retcode_T error, bool isfromIsr);

void assertIndicationMapping(const unsigned long line, const unsigned char *const file);

static void SysTickPreCallback(void);

extern void xPortSysTickHandler(void);

#if configSUPPORT_STATIC_ALLOCATION
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
#endif
#if configSUPPORT_STATIC_ALLOCATION && configUSE_TIMERS
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);
#endif

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
        printf("System Startup success");
        returnValue =
            CmdProcessor_Initialize(&MainCmdProcessor, (char *)"MainCmdProcessor", TASK_PRIO_MAIN_CMD_PROCESSOR, TASK_STACK_SIZE_MAIN_CMD_PROCESSOR, TASK_Q_LEN_MAIN_CMD_PROCESSOR);
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
            printf("Fatal error from package %u , Error code: %u and module ID is :%u \r\n", (unsigned int)PackageID, (unsigned int)ErrorCode, (unsigned int)ModuleID);

        if (RETCODE_SEVERITY_ERROR == SeverityCode)
            printf("Severe error from package %u , Error code: %u and module ID is :%u \r\n", (unsigned int)PackageID, (unsigned int)ErrorCode, (unsigned int)ModuleID);
    }
    else
    {
        BSP_LED_Switch(NUCLEOL4R5ZI_LED_RED_ID, NUCLEOL4R5ZI_LED_COMMAND_ON);
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

void assertIndicationMapping(const unsigned long line, const unsigned char *const file)
{
    /* Switch on the LEDs */
    Retcode_T retcode = RETCODE_OK;

    if (RETCODE_OK == retcode)
    {
        retcode = BSP_LED_Switch(NUCLEOL4R5ZI_LED_BLUE_ID, NUCLEOL4R5ZI_LED_COMMAND_ON);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_LED_Switch(NUCLEOL4R5ZI_LED_RED_ID, NUCLEOL4R5ZI_LED_COMMAND_ON);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_LED_Switch(NUCLEOL4R5ZI_LED_GREEN_ID, NUCLEOL4R5ZI_LED_COMMAND_ON);
    }
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
    void *param2 = NULL;

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
        returnValue = BSP_LED_Enable(NUCLEOL4R5ZI_LED_ALL);
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

#if configSUPPORT_STATIC_ALLOCATION
/* static memory allocation for the IDLE task */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[IDLE_TASK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = IDLE_TASK_SIZE;
}
#endif

#if configSUPPORT_STATIC_ALLOCATION && configUSE_TIMERS
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

/* If static allocation is supported then the application must provide the
   following callback function - which enables the application to optionally
   provide the memory that will be used by the timer task as the task's stack
   and TCB. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = &xTimerStack[0];
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif
