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
 *
 *
 *  Mockup implementation for the task module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef KISO_TASK_TH_HH_
#define KISO_TASK_TH_HH_

/* include system header files */
#include <gtest.h>

/* ban task.h interface header */
#define INC_TASK_H

#include "FreeRTOS_th.hh"
#include "portmacro_th.hh"
#include "list_th.hh"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
typedef void* TaskHandle_t;

typedef void (*TaskHookFunction_t)(void* parameter);

typedef enum
{
    eRunning = 0, /* A task is querying the state of itself, so must be running. */
    eReady, /* The task being queried is in a read or pending ready list. */
    eBlocked, /* The task being queried is in the Blocked state. */
    eSuspended, /* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
    eDeleted /* The task being queried has been deleted, but its TCB has not yet been freed. */
} eTaskState;

typedef enum
{
    eNoAction = 0, /* Notify the task without updating its notify value. */
    eSetBits, /* Set bits in the task's notification value. */
    eIncrement, /* Increment the task's notification value. */
    eSetValueWithOverwrite, /* Set the task's notification value to a specific value even if the previous value has not yet been read by the task. */
    eSetValueWithoutOverwrite /* Set the task's notification value if the previous value has been read by the task. */
} eNotifyAction;

typedef struct xTIME_OUT
{
    BaseType_t xOverflowCount;
    TickType_t xTimeOnEntering;
} TimeOut_t;

typedef struct xMEMORY_REGION
{
    void *pvBaseAddress;
    uint32_t ulLengthInBytes;
    uint32_t ulParameters;
} MemoryRegion_t;

typedef struct xTASK_PARAMETERS
{
    TaskFunction_t pvTaskCode;
    const char * const pcName;
    uint16_t usStackDepth;
    void *pvParameters;
    UBaseType_t uxPriority;
    StackType_t *puxStackBuffer;
    MemoryRegion_t xRegions[2];
} TaskParameters_t;

typedef struct xTASK_STATUS
{
    TaskHandle_t xHandle; /* The handle of the task to which the rest of the information in the structure relates. */
    const char *pcTaskName; /* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */
    UBaseType_t xTaskNumber; /* A number unique to the task. */
    eTaskState eCurrentState; /* The state in which the task existed when the structure was populated. */
    UBaseType_t uxCurrentPriority; /* The priority at which the task was running (may be inherited) when the structure was populated. */
    UBaseType_t uxBasePriority; /* The priority to which the task will return if the task's current priority has been inherited to avoid unbounded priority inversion when obtaining a mutex.  Only valid if configUSE_MUTEXES is defined as 1 in FreeRTOSConfig.h. */
    uint32_t ulRunTimeCounter; /* The total run time allocated to the task so far, as defined by the run time stats clock.  See http://www.freertos.org/rtos-run-time-stats.html.  Only valid when configGENERATE_RUN_TIME_STATS is defined as 1 in FreeRTOSConfig.h. */
    uint16_t usStackHighWaterMark; /* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack. */
} TaskStatus_t;

typedef enum
{
    eAbortSleep = 0, /* A task has been made ready or a context switch pended since portSUPPORESS_TICKS_AND_SLEEP() was called - abort entering a sleep mode. */
    eStandardSleep, /* Enter a sleep mode that will not last any longer than the expected idle time. */
    eNoTasksWaitingTimeout /* No tasks are waiting for a timeout so it is safe to enter a sleep mode that can only be exited by an external interrupt. */
} eSleepModeStatus;

#define tskIDLE_PRIORITY            (0U)

#define taskSCHEDULER_SUSPENDED     ((BaseType_t) 0)
#define taskSCHEDULER_NOT_STARTED   ((BaseType_t) 1)
#define taskSCHEDULER_RUNNING       ((BaseType_t) 2)

/* Fakes for portmacro defines */
FAKE_VOID_FUNC(taskYIELD)

FAKE_VOID_FUNC(taskENTER_CRITICAL)
FAKE_VOID_FUNC(taskENTER_CRITICAL_FROM_ISR)
FAKE_VOID_FUNC(taskEXIT_CRITICAL)
FAKE_VOID_FUNC(taskEXIT_CRITICAL_FROM_ISR)

FAKE_VOID_FUNC(taskDISABLE_INTERRUPTS)
FAKE_VOID_FUNC(taskENABLE_INTERRUPTS)

FAKE_VALUE_FUNC(BaseType_t, xTaskNotify, TaskHandle_t, uint32_t, eNotifyAction)
FAKE_VALUE_FUNC(BaseType_t, xTaskNotifyAndQuery, TaskHandle_t, uint32_t, eNotifyAction, uint32_t*)

FAKE_VALUE_FUNC(BaseType_t, xTaskNotifyFromISR, TaskHandle_t, uint32_t, eNotifyAction, BaseType_t*)
FAKE_VALUE_FUNC(BaseType_t, xTaskNotifyAndQueryFromISR, TaskHandle_t, uint32_t, eNotifyAction, uint32_t*, BaseType_t*)
FAKE_VALUE_FUNC(BaseType_t, xTaskNotifyGive, TaskHandle_t)

/* Fakes for functions */
FAKE_VOID_FUNC(vTaskAllocateMPURegions, TaskHandle_t, MemoryRegion_t*)

FAKE_VOID_FUNC(vTaskDelete, TaskHandle_t)

FAKE_VOID_FUNC(vTaskDelay, TickType_t)
FAKE_VOID_FUNC(vTaskDelayUntil, TickType_t*, TickType_t)

FAKE_VALUE_FUNC(UBaseType_t, uxTaskPriorityGet, TaskHandle_t)
FAKE_VALUE_FUNC(UBaseType_t, uxTaskPriorityGetFromISR, TaskHandle_t)

FAKE_VALUE_FUNC(eTaskState, eTaskGetState, TaskHandle_t)

FAKE_VOID_FUNC(vTaskPrioritySet, TaskHandle_t, UBaseType_t)

FAKE_VOID_FUNC(vTaskSuspend, TaskHandle_t)
FAKE_VOID_FUNC(vTaskResume, TaskHandle_t)
FAKE_VALUE_FUNC(BaseType_t, xTaskResumeFromISR, TaskHandle_t)

FAKE_VOID_FUNC(vTaskStartScheduler)
FAKE_VOID_FUNC(vTaskEndScheduler)

FAKE_VOID_FUNC(VTaskSuspendAll)
FAKE_VALUE_FUNC(BaseType_t, xTaskResumeAll)

FAKE_VALUE_FUNC(TickType_t, xTaskGetTickCount)
FAKE_VALUE_FUNC(TickType_t, xTaskGetTickCountFromISR)

FAKE_VALUE_FUNC(UBaseType_t, uxTaskGetNumberOfTasks)

FAKE_VALUE_FUNC(char*, pcTaskGetTaskName, TaskHandle_t)

FAKE_VALUE_FUNC(UBaseType_t, uxTaskGetStackHighWaterMark, TaskHandle_t)

FAKE_VOID_FUNC(vTaskSetApplicationTaskTag, TaskHandle_t, TaskHookFunction_t)
FAKE_VALUE_FUNC(TaskHookFunction_t, xTaskGetApplicationTaskTag, TaskHandle_t)

FAKE_VOID_FUNC(vTaskSetThreadLocalStoragePointer, TaskHandle_t, BaseType_t, void*)
FAKE_VOID_FUNC(pvTaskGetThreadLocalStoragePointer, TaskHandle_t, BaseType_t)

FAKE_VALUE_FUNC(BaseType_t, xTaskCallApplicationTaskHook, TaskHandle_t, void*)

FAKE_VALUE_FUNC(TaskHandle_t, xTaskGetIdleTaskHandle)

FAKE_VALUE_FUNC(UBaseType_t, uxTaskGetSystemState, TaskStatus_t*, UBaseType_t, uint32_t*)

FAKE_VOID_FUNC(vTaskList, char*)
FAKE_VOID_FUNC(vTaskGetRunTimeStats, char*)

FAKE_VALUE_FUNC(BaseType_t, xTaskGenericNotify, TaskHandle_t, uint32_t, eNotifyAction, uint32_t*)
FAKE_VALUE_FUNC(BaseType_t, xTaskGenericNotifyFromISR, TaskHandle_t, uint32_t, eNotifyAction, uint32_t*, BaseType_t*)
FAKE_VALUE_FUNC(BaseType_t, xTaskNotifyWait, uint32_t, uint32_t, uint32_t*, TickType_t)
FAKE_VOID_FUNC(vTaskNotifyGiveFromISR, TaskHandle_t, BaseType_t*)
FAKE_VALUE_FUNC(uint32_t, ulTaskNotifyTake, BaseType_t, TickType_t)

FAKE_VALUE_FUNC(BaseType_t, xTaskIncrementTick)

FAKE_VOID_FUNC(vTaskPlaceOnEventList, List_t*, TickType_t)
FAKE_VOID_FUNC(vTaskPlaceOnUnorderedEventList, List_t*, TickType_t, TickType_t)
FAKE_VOID_FUNC(vTaskPlaceOnEventListRestricted, List_t*, TickType_t, BaseType_t)
FAKE_VALUE_FUNC(BaseType_t, xTaskRemoveFromEventList, List_t*)
FAKE_VALUE_FUNC(BaseType_t, xTaskRemoveFromUnorderedEventList, ListItem_t*, TickType_t)

FAKE_VOID_FUNC(vTaskSwitchContext)

FAKE_VALUE_FUNC(TickType_t, uxTaskResetEventItemValue)

FAKE_VALUE_FUNC(TaskHandle_t, xTaskGetCurrentTaskHandle)

FAKE_VOID_FUNC(vTaskSetTimeOutState, TimeOut_t*)
FAKE_VALUE_FUNC(BaseType_t, xTaskCheckForTimeOut, TimeOut_t*, TickType_t*)

FAKE_VOID_FUNC(vTaskMissedYield)

FAKE_VALUE_FUNC(BaseType_t, xTaskGetSchedulerState)

FAKE_VOID_FUNC(vTaskPriorityInherit, TaskHandle_t)
FAKE_VALUE_FUNC(BaseType_t, xTaskPriorityDisinherit, TaskHandle_t)

#if (configSUPPORT_STATIC_ALLOCATION == 1)
FAKE_VALUE_FUNC(TaskHandle_t, xTaskCreateStatic, TaskFunction_t, const char *, uint32_t, void *, UBaseType_t, StackType_t *, StaticTask_t *)
FAKE_VALUE_FUNC(BaseType_t, xTaskCreateRestrictedStatic, const TaskParameters_t *, TaskHandle_t *)

#endif /* configSUPPORT_STATIC_ALLOCATION */

#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
FAKE_VALUE_FUNC(BaseType_t, xTaskCreate, TaskHookFunction_t, const char *, unsigned short, void *, UBaseType_t, TaskHandle_t*)
FAKE_VALUE_FUNC(BaseType_t, xTaskCreateRestricted, TaskParameters_t*, TaskHandle_t*)
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */

FAKE_VALUE_FUNC(UBaseType_t, uxTaskGetTaskNumber, TaskHandle_t)
FAKE_VOID_FUNC(vTaskSetTaskNumber, TaskHandle_t, UBaseType_t)

FAKE_VOID_FUNC(vTaskStepTick, TickType_t)

FAKE_VALUE_FUNC(eSleepModeStatus, eTaskConfirmSleepModeStatus)

FAKE_VALUE_FUNC(void*, pvTaskIncrementMutexHeldCount)

#endif /* KISO_TASK_TH_HH_ */

/** ************************************************************************* */
