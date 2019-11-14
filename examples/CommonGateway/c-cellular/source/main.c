/*
 * Notice:
 * The code snippet of prvGetRegistersFromStack in this file comes from
 * freertos document, which is available under:
 * https://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html
 * Copyright of this very code snippet does not belong to Robert Bosch GmbH.
 *
 * Apart from the stated code snippet the copyright/license header below applies.
 */

/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     Robert Bosch GmbH - initial contribution
 */

/**
 * @file
 * @brief Implemented first application function main() here.
 */

#include "AppModules.h"
#define KISO_MODULE_ID APP_MODULE_ID_MAIN

#include "App.h"
#include "AppConfig.h"

#include "Kiso_Basics.h"
#include "Kiso_Assert.h"
#include <stdio.h>
#include "Kiso_CmdProcessor.h"
#include "Kiso_Logging.h"
#include "BSP_CommonGateway.h"
#include "Kiso_BSP_LED.h"
#include "Kiso_BSP_Board.h"
#include "Kiso_HAL_Delay.h"

#include "FreeRTOS.h"
#include "task.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

Retcode_T SystemStartup(void);

void ErrorHandler(Retcode_T error, bool isfromIsr);

void AssertIndicationMapping(const unsigned long line, const unsigned char *const file);

static void SysTickPreCallback(void);

extern void xPortSysTickHandler(void);

#if (configCHECK_FOR_STACK_OVERFLOW > 0)
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);
#endif

static Retcode_T Delay(uint32_t delay);

static void PostOsInit(void *param, uint32_t len);

#if configSUPPORT_STATIC_ALLOCATION
/* static memory allocation for the IDLE task */
static StaticTask_t IdleTaskTCBBuffer;
static StackType_t IdleStack[IDLE_TASK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

#if configUSE_TIMERS
static StaticTask_t TimerTaskTCBBuffer;
static StackType_t TimerStack[configTIMER_TASK_STACK_DEPTH];

/* If static allocation is supported then the application must provide the
   following callback function - which enables the application to optionally
   provide the memory that will be used by the timer task as the task's stack
   and TCB. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);
#endif
#endif

KISO_UNUSED_FUNC(static void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress));

KISO_UNUSED_FUNC(void HardFault_Handler(void) __attribute__((naked)));

/*---------------------- VARIABLES DECLARATIONS ---------------------------------------------------------------------*/

static CmdProcessor_T MainCmdProcessor;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

int main(void)
{
    /* Mapping Default Error Handling function */
    Retcode_T returnValue = Retcode_Initialize(ErrorHandler);
    if (RETCODE_OK == returnValue)
    {
        returnValue = SystemStartup();
    }
    if (RETCODE_OK == returnValue)
    {
        /* Preferable approach is  the async recorder over the synchronous variant */
#if (defined(KISO_ASYNC_RECORDER) && KISO_ASYNC_RECORDER)
#if (defined(KISO_UART_APPENDER) && KISO_UART_APPENDER)
        returnValue = Logging_Init(Logging_AsyncRecorder, Logging_UARTAppender);
#endif
#elif (defined(KISO_SYNC_RECORDER) && KISO_SYNC_RECORDER)
#if (defined(KISO_UART_APPENDER) && KISO_UART_APPENDER)
        returnValue = Logging_Init(Logging_SyncRecorder, Logging_UARTAppender);
#endif
#endif
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = CmdProcessor_Initialize(&MainCmdProcessor,
                                              (char *)"MainCmdProcessor", APP_CMDPROCESSOR_MAIN_PRIORITY,
                                              APP_CMDPROCESSOR_MAIN_STACKSIZE, APP_CMDPROCESSOR_MAIN_QLEN);
    }
    if (RETCODE_OK == returnValue)
    {
        /* To be sure that the OS-aware-delay is only called _after_ the
         * FreeRTOS scheduler has started, we enqueue it as the very first
         * CmdProcessor job.
         */
        returnValue = CmdProcessor_Enqueue(&MainCmdProcessor, PostOsInit, NULL, 0U);
    }
    if (RETCODE_OK == returnValue)
    {
        /* Here we enqueue the application initialization into the command
         * processor, such that the initialization function will be invoked
         * once the RTOS scheduler is started below.
         */
        returnValue = CmdProcessor_Enqueue(&MainCmdProcessor, App_InitSystem, &MainCmdProcessor, 0U);
    }
    if (RETCODE_OK != returnValue)
    {
        puts("System Startup failed");
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
        /** \todo: ERROR HANDLING SHOULD BE DONE FOR THE ERRORS RAISED FROM PLATFORM */
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
        BSP_LED_Switch(COMMONGATEWAY_LED_RED_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }
}

#ifndef NDEBUG /* valid only for debug builds */
/**
 * @brief This API is called when function enters an assert
 *
 * @param[in] line : line number where asserted
 * @param[in] file : file name which is asserted
 */
void AssertIndicationMapping(const unsigned long line, const unsigned char *const file)
{
    /* Switch on the LEDs */
    Retcode_T retcode = RETCODE_OK;

    retcode = BSP_LED_Switch(COMMONGATEWAY_LED_ALL, COMMONGATEWAY_LED_COMMAND_ON);

    printf("asserted at Filename %s , line no  %ld \n\r", file, line);

    if (retcode != RETCODE_OK)
    {
        printf("LED's ON failed during assert");
    }
    __asm__("bkpt;");
}
#endif

Retcode_T SystemStartup(void)
{
    Retcode_T returnValue = RETCODE_OK;
    uint32_t param1 = 0;
    void *param2 = NULL;

    /* Initialize the callbacks for the system tick */
    BSP_Board_OSTickInitialize(SysTickPreCallback, NULL);

#ifndef NDEBUG /* valid only for debug builds */
    if (RETCODE_OK == returnValue)
    {
        returnValue = Assert_Initialize(AssertIndicationMapping);
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

#if configSUPPORT_STATIC_ALLOCATION
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &IdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &IdleStack[0];
    *pulIdleTaskStackSize = IDLE_TASK_SIZE;
}
#if configUSE_TIMERS
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &TimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = &TimerStack[0];
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif
#endif

/**
 * @brief This function is SysTick Handler. This is called when ever the IRQ is
 * hit. This is a temporary implementation where the SysTick_Handler() is not
 * directly mapped to xPortSysTickHandler(). Instead it is only called if the
 * scheduler has started.
 */
static void SysTickPreCallback(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}

static void PostOsInit(void *param, uint32_t len)
{
    KISO_UNUSED(param);
    KISO_UNUSED(len);
    HAL_Delay_SetMsHook(Delay);
}

static Retcode_T Delay(uint32_t delay)
{
    vTaskDelay(delay);
    return RETCODE_OK;
}

#if (configCHECK_FOR_STACK_OVERFLOW > 0)
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    KISO_UNUSED(xTask);
    KISO_UNUSED(pcTaskName);
    assert(0);
}
#endif

static void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
{
    /* These are volatile to try and prevent the compiler/linker optimising them
     * away as the variables never actually get used.  If the debugger won't show the
     * values of the variables, make them global my moving their declaration outside
     * of this function. */
    volatile uint32_t r0;
    volatile uint32_t r1;
    volatile uint32_t r2;
    volatile uint32_t r3;
    volatile uint32_t r12;
    volatile uint32_t lr;  /* Link register. */
    volatile uint32_t pc;  /* Program counter. */
    volatile uint32_t psr; /* Program status register. */

    r0 = pulFaultStackAddress[0];
    r1 = pulFaultStackAddress[1];
    r2 = pulFaultStackAddress[2];
    r3 = pulFaultStackAddress[3];

    r12 = pulFaultStackAddress[4];
    lr = pulFaultStackAddress[5];
    pc = pulFaultStackAddress[6];
    psr = pulFaultStackAddress[7];

    KISO_UNUSED(r0);
    KISO_UNUSED(r1);
    KISO_UNUSED(r2);
    KISO_UNUSED(r3);
    KISO_UNUSED(r12);
    KISO_UNUSED(lr);
    KISO_UNUSED(pc);
    KISO_UNUSED(psr);

    __asm__("bkpt");

    /* When the following line is hit, the variables contain the register values. */
    for (;;)
        ;
}

void HardFault_Handler(void)
{
    __asm volatile(
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n");
}
