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
 *
 * @brief
 *      Task Monitor Interface Implementation
 *
 * @details
 *      This source file implements following features:
 *      - TaskMonitor_Initialize()
 *      - TaskMonitor_Register()
 *      - TaskMonitor_Update()
 *      - TaskMonitor_Check()
 * 
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_TASKMONITOR

/* Include Kiso_TaskMonitor interface header */
#include "Kiso_TaskMonitor.h"

#if KISO_FEATURE_TASKMONITOR

/* KISO basics header files */
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "task.h"

#if (configUSE_APPLICATION_TASK_TAG == 0)
#error Enable configUSE_APPLICATION_TASK_TAG macro in FreeRTOSConfig.h
#endif /* configUSE_APPLICATION_TASK_TAG */

/* Structure saving the data of the registered task */
struct TaskMonitor_TaskInfo_S
{
    TaskHandle_t Task;
    uint32_t UpperLimitTickTime;
    bool IsReg;
};

/*The data type for  registered task */
typedef struct TaskMonitor_TaskInfo_S TaskMonitor_TaskInfo_T;

/* constant and variable definitions */

TaskMonitor_TaskInfo_T TaskInfo[KISO_TASKMONITOR_MAX_TASKS];

/*  The description of the function is available in Kiso_TaskMonitor.h */
Retcode_T TaskMonitor_Initialize(void)
{
    uint32_t loopcnt;
    for (loopcnt = 0U; loopcnt < KISO_TASKMONITOR_MAX_TASKS; loopcnt++)
    {
        TaskInfo[loopcnt].IsReg = false;
        TaskInfo[loopcnt].Task = NULL;
        TaskInfo[loopcnt].UpperLimitTickTime = 0UL;
    }
    return RETCODE_OK;
}

/*  The description of the function is available in Kiso_TaskMonitor.h */
Retcode_T TaskMonitor_Register(void *task, uint32_t upperLimit)
{
    uint32_t loopcnt;
    Retcode_T ret = RETCODE_OK;
    if ((NULL != task) && (0UL != upperLimit))
    {
        for (loopcnt = 0U; loopcnt < KISO_TASKMONITOR_MAX_TASKS; loopcnt++)
        {
            if (true != TaskInfo[loopcnt].IsReg)
            {
                TaskInfo[loopcnt].Task = (TaskHandle_t)task;
                TaskInfo[loopcnt].UpperLimitTickTime = ((upperLimit * 1000) / portTICK_RATE_MS);
                TaskInfo[loopcnt].IsReg = true;
                break;
            }
        }
        if (KISO_TASKMONITOR_MAX_TASKS <= loopcnt)
        {
            ret = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_TASKMONITOR_BUFFER_FULL_ERROR);
        }
    }
    else
    {
        ret = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_INVALID_PARAM);
    }
    return ret;
}

/* The description of the function is available in header file */
KISO_INLINE void TaskMonitor_Update(unsigned int *taskTag, unsigned int tickCount)
{
    *taskTag = tickCount;
}

/*  The description of the function is available in Kiso_TaskMonitor.h */
bool TaskMonitor_Check(void)
{
    uint32_t loopcnt;
    uint32_t taskTickTime;
    uint32_t taskTagValue[KISO_TASKMONITOR_MAX_TASKS];
    uint32_t diffTickTime;
    uint32_t TickSyncCheck;

    TickType_t currentTickTime;
    bool ret = false;
    for (loopcnt = 0U; loopcnt < KISO_TASKMONITOR_MAX_TASKS; loopcnt++)
    {
        taskTagValue[loopcnt] = 0UL;
        if (TaskInfo[loopcnt].IsReg)
        {
            const TaskHookFunction_t taskTag = xTaskGetApplicationTaskTag(TaskInfo[loopcnt].Task);
            taskTagValue[loopcnt] = *(const uint32_t *)&taskTag;
        }
    }
    currentTickTime = xTaskGetTickCount();
    for (loopcnt = 0U; loopcnt < KISO_TASKMONITOR_MAX_TASKS; loopcnt++)
    {
        if (TaskInfo[loopcnt].IsReg)
        {
            taskTickTime = taskTagValue[loopcnt];
            /* Note :
             * Some times xTaskGetTickCount() is Providing the value less than updated time
             * of xTickCount due to task synchronization.
             * Hence count ticket count is updated to taskTag value.
             */
            if ((currentTickTime < taskTickTime))
            {
                TickSyncCheck = taskTickTime - currentTickTime;
                if (TickSyncCheck <= 2UL)
                {
                    currentTickTime = taskTickTime;
                }
            }
            diffTickTime =
                (currentTickTime >= taskTickTime) ? (currentTickTime - taskTickTime) : ((UINT32_MAX - taskTickTime) + currentTickTime);
            if (diffTickTime > TaskInfo[loopcnt].UpperLimitTickTime)
            {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

#endif /* if KISO_FEATURE_TASKMONITOR */
