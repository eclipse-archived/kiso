/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/****************************************************************************
 * Modified by Robert Bosch GmbH.
 *****************************************************************************/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

/********************** Configuration of FreeRTOS ****************************/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
#ifndef KISO_MODULE_ID
#define KISO_MODULE_ID 1
#endif

#include "Kiso_Assert.h"
    extern uint32_t SystemCoreClock;
#endif

    /* KISO FreeRTOS Configuration version information */
#define KISO_FREE_RTOS_VERSION_MAJOR (10)
#define KISO_FREE_RTOS_VERSION_MINOR (0)
#define KISO_FREE_RTOS_VERSION_BUILD (1)

    /* Modes of operations of operation system */
#define configUSE_PREEMPTION (1)
#define configSUPPORT_DYNAMIC_ALLOCATION (1)
#define configSUPPORT_STATIC_ALLOCATION (1)

/* Energy saving modes */
#define configUSE_TICKLESS_IDLE (1)
#define configUSE_IDLE_HOOK (0)
#define configUSE_TICK_HOOK (0)
#define configTICK_RATE_HZ ((TickType_t)1000)

/* Main functions*/
#define configMAX_PRIORITIES (9)
#define configMINIMAL_STACK_SIZE ((unsigned short)100)
#define configTOTAL_HEAP_SIZE ((size_t)(30000))
#define configMAX_TASK_NAME_LEN (16)
#define configUSE_TRACE_FACILITY (0)
#define configUSE_16_BIT_TICKS (0)
#define configIDLE_SHOULD_YIELD (0)
#define configUSE_MUTEXES (1)
#define configUSE_RECURSIVE_MUTEXES (1)
#define configUSE_COUNTING_SEMAPHORES (1)
#define configQUEUE_REGISTRY_SIZE (0)
#define configUSE_QUEUE_SETS (1)

/* Hook function related definitions. */
#define configUSE_TICK_HOOK (0)
#define configCHECK_FOR_STACK_OVERFLOW (0)
#define configUSE_MALLOC_FAILED_HOOK (0)

/* Run time stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS (0)

/* Software timer related definitions. */
#define configUSE_TIMERS (1)
#define configTIMER_TASK_PRIORITY (5)
#define configTIMER_QUEUE_LENGTH (40)
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE + 160)

/* Idle task related definitions. */
#define IDLE_TASK_SIZE (configMINIMAL_STACK_SIZE)

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet (1)
#define INCLUDE_uxTaskPriorityGet (1)
#define INCLUDE_vTaskDelete (1)
#define INCLUDE_vTaskSuspend (1)
#define INCLUDE_xResumeFromISR (1)
#define INCLUDE_vTaskDelayUntil (1)
#define INCLUDE_vTaskDelay (1)
#define INCLUDE_xTaskGetSchedulerState (1)
#define INCLUDE_xTaskGetCurrentTaskHandle (1)
#define INCLUDE_uxTaskGetStackHighWaterMark (0)
#define INCLUDE_xTaskGetIdleTaskHandle (0)
#define INCLUDE_xTimerGetTimerDaemonTaskHandle (0)
#define INCLUDE_pcTaskGetTaskName (1)
#define INCLUDE_eTaskGetState (0)
#define INCLUDE_xTimerPendFunctionCall (1)

/* lwip required config */
#define INCLUDE_xTaskAbortDelay (1)
/* is 10 a good number to set? */
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS (10)

/* Default value of CPU clock (RC)*/
#define configCPU_CLOCK_HZ (SystemCoreClock)

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
    /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
#define configPRIO_BITS __NVIC_PRIO_BITS
#else
#define configPRIO_BITS (4) /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
 * function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY (0xF)

/* The highest interrupt priority that can be used by any interrupt service
 * routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
 * INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
 * PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY (5)

/* Interrupt nesting behaviour configuration. */

/* Interrupt priorities used by the kernel port layer itself.  These are generic
 * to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
 * See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#ifndef NDEBUG /* valid only for debug builds */
/* Mapping the FreeRTOS configASSERT() to KISO assert() Function. */
#define configASSERT(x) assert(x)
#endif

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
 * standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler

/* Task monitor support macros. By Default Tsak monitor is enabled */
#define configUSE_APPLICATION_TASK_TAG (1)

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_CONFIG_H */
