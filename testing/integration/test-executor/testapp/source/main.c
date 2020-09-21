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
 * @brief This file implements the main() of the test executor software. After initializing the test board and 
 * starting the OS scheduler, it shall call the function in the feature integration tests responsible for initializing
 * and starting the tests (e.g. TestEntry_Initialize() under /core/essentials/test/integration/source/TestEntry.c)
 *
 * @file
**/

/*###################### INCLUDED HEADERS ############################################################################*/

#include "AppModules.h"
#include "BSP_Proxy.h"
#include "Kiso_Retcode.h"
#include "Kiso_BSP_LED.h"
#include "Kiso_BSP_Board.h"
#include "Kiso_Basics.h"
#include "Kiso_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

/*###################### MACROS DEFINITION ###########################################################################*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_APP_MODULE_MAIN

#define TASK_PRIO_MAIN_CMD_PROCESSOR (UINT32_C(1))
#define TASK_STACK_SIZE_MAIN_CMD_PROCESSOR (UINT16_C(700))
#define TASK_Q_LEN_MAIN_CMD_PROCESSOR (UINT32_C(10))

/*###################### FUNCTIONS DECLARATION #######################################################################*/

extern void xPortSysTickHandler(void);                                /*link-time function to be provided by the freertos library */
extern Retcode_T TestEntry_Initialize(void *param1, uint32_t param2); /* link-time function to be provided by the library implementing the feature integration tests*/

static Retcode_T systemStartup(void);
static void systemInit(void *param1, uint32_t param2);
static void ErrorHandler(Retcode_T error, bool isfromIsr);
static void assertIndicationMapping(const unsigned long line, const unsigned char *const file);
static void SysTickPreCallback(void);

#if configSUPPORT_STATIC_ALLOCATION
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);
#endif
#if configSUPPORT_STATIC_ALLOCATION && configUSE_TIMERS
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);
#endif

/*###################### VARIABLES DECLARATIONS ######################################################################*/

static CmdProcessor_T MainCmdProcessor;

/*###################### EXPOSED FUNCTIONS IMPLEMENTATION ############################################################*/

int main(void)
{
    /* Mapping Default Error Handling function */
    Retcode_T retcode = Retcode_Initialize(ErrorHandler);

#ifndef NDEBUG
    if (RETCODE_OK == retcode)
    {
        retcode = Assert_Initialize(assertIndicationMapping);
    }
#endif /*NDEBUG*/
    if (RETCODE_OK == retcode)
    {
        retcode = systemStartup();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = CmdProcessor_Initialize(&MainCmdProcessor,
                                          (char *)"MainCmdProcessor",
                                          TASK_PRIO_MAIN_CMD_PROCESSOR,
                                          TASK_STACK_SIZE_MAIN_CMD_PROCESSOR,
                                          TASK_Q_LEN_MAIN_CMD_PROCESSOR);
    }
    if (RETCODE_OK == retcode)
    {
        /* Here we enqueue the application initialization into the command
         * processor, such that the initialization function will be invoked
         * once the RTOS scheduler is started below.
         */
        retcode = CmdProcessor_Enqueue(&MainCmdProcessor,
                                       systemInit,
                                       &MainCmdProcessor,
                                       UINT32_C(0));
    }

    if (RETCODE_OK != retcode)
    {
        printf("System Startup failed");
        assert(false);
    }
    /* start scheduler */
    vTaskStartScheduler();
}

/*###################### LOCAL FUNCTIONS IMPLEMENTATION ##############################################################*/
/**
 * @brief       Starts the system up.
 * @details     This function will execute before the scheduler starts and it is intended to make the target board 
 *              ready for operation.
 * @return      Returns RETCODE_OK in case of success, error code otherwise. 
 */
Retcode_T systemStartup(void)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t param1 = 0;
    void *param2 = NULL;

    /* Initialize the callbacks for the system tick */
    BSP_Board_OSTickInitialize(SysTickPreCallback, NULL);
    retcode = BSP_Board_Initialize(param1, param2);
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_LED_Connect();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_LED_Enable(TEST_BOARD_LED_ALL);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_LED_Switch(TEST_BOARD_LED_ALL, TEST_BOARD_LED_COMMAND_ON);
    }
    return retcode;
}

void systemInit(void *param1, uint32_t param2)
{
    KISO_UNUSED(param1);
    KISO_UNUSED(param2);
    Retcode_T retcode = TestEntry_Initialize(NULL, 0U);
    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }
}

/**
 * @brief       Error handler function.
 * @details     This function is called when Retcode_RaiseError() function is invoked, it is used to report the error to
 *              the user.
 * @param[in]   error: Error code raised.
 * @param[in]   isfromIsr: if true then the ErrorHandler is being executed from an ISR context and not all the services
 *              are available.
 */
void ErrorHandler(Retcode_T error, bool isfromIsr)
{
    if (!isfromIsr)
    {
        /** \todo: ERROR HANDLING SHOULD BE DONE FOR THE ERRORS RAISED FROM PLATFORM */
        uint32_t PackageID = Retcode_GetPackage(error);
        uint32_t ErrorCode = Retcode_GetCode(error);
        uint32_t ModuleID = Retcode_GetModuleId(error);
        Retcode_Severity_T SeverityCode = Retcode_GetSeverity(error);

        if (RETCODE_SEVERITY_FATAL == SeverityCode)
        {
            printf("Fatal Error:[%u] from Module:[%u] in Package:[%u]\r\n",
                   (unsigned int)ErrorCode,
                   (unsigned int)ModuleID,
                   (unsigned int)PackageID);
        }
        else if (RETCODE_SEVERITY_ERROR == SeverityCode)
        {
            printf("Severe Error:[%u] from Module:[%u] in Package:[%u]\r\n",
                   (unsigned int)ErrorCode,
                   (unsigned int)ModuleID,
                   (unsigned int)PackageID);
        }
        BSP_LED_Switch(TEST_BOARD_LED_ALL, TEST_BOARD_LED_COMMAND_OFF);
        BSP_LED_Switch(TEST_BOARD_LED_PANIC, TEST_BOARD_LED_COMMAND_ON);
    }
    else
    {

        BSP_LED_Switch(TEST_BOARD_LED_ALL, TEST_BOARD_LED_COMMAND_OFF);
        BSP_LED_Switch(TEST_BOARD_LED_PANIC, TEST_BOARD_LED_COMMAND_ON);
    }
}

#ifndef NDEBUG
/**
 * @brief       This API is called when function enters an assert
 * @param[in]   line : line number where asserted.
 * @param[in]   file : file name which is asserted.
 */
void assertIndicationMapping(const unsigned long line, const unsigned char *const file)
{
    (void)BSP_LED_Switch(TEST_BOARD_LED_ALL, TEST_BOARD_LED_COMMAND_ON);
    printf("asserted at Filename %s , line no  %ld \n\r", file, line);
}
#endif /* NDEBUG */

/**
 * @brief       This function is a hook from FreeRTOS to systick.
 * @details     This function is called when ever the Systick IRQ is hit. This is a temporary implementation where the 
 *              SysTick_Handler() is not directly mapped to xPortSysTickHandler(). Instead it is only called if the 
 *              scheduler has started.
 */
static void SysTickPreCallback(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}

#if configSUPPORT_STATIC_ALLOCATION

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[IDLE_TASK_SIZE];

/**
 * @brief       If static allocation is supported then the application must provide the following callback function
 * @details     Enables the application to optionally provide the memory that will be used by the idle task as the 
 *              task's stack and TCB.
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = IDLE_TASK_SIZE;
}
#endif

#if configSUPPORT_STATIC_ALLOCATION && configUSE_TIMERS
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

/**
 * @brief       If static allocation and timers are supported then the application must provide the following callback function
 * @details     Enables the application to optionally provide the memory that will be used by the timer task as the 
 *              task's stack and TCB.
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = &xTimerStack[0];
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif
