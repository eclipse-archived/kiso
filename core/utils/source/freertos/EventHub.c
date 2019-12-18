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
 *      Event Hub Interface Implementation
 *
 * @details
 *      This source file implements following features:
 *      - EventHub_Initialize()
 *      - EventHub_Observe()
 *      - EventHub_ObserveAll()
 *      - EventHub_Notify()
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_EVENTHUB

/* Include Kiso_EventHub interface header */
#include "Kiso_EventHub.h"

#if KISO_FEATURE_EVENTHUB

/* KISO basics header files */
#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*  The description of the function is available in Kiso_EventHub.h */
Retcode_T EventHub_Initialize(EventHub_T *hub)
{
    if (NULL == hub)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_NULL_POINTER);
    }

    memset(hub, 0, sizeof(EventHub_T));

    hub->lock = (SemaphoreHandle_t)xSemaphoreCreateMutex();
    if (NULL == hub->lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_SEMAPHORE_ERROR);
    }

    return RETCODE_OK;
}

/*  The description of the function is available in Kiso_EventHub.h */
Retcode_T EventHub_Observe(EventHub_T *hub, EventHandler_T handler, TaskEvent_T event)
{
    Retcode_T retval = RETCODE_OK;

    if (NULL == hub || NULL == handler)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_NULL_POINTER);
    }

    if (NULL == hub->lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_UNINITIALIZED);
    }

    if (pdPASS != xSemaphoreTake((SemaphoreHandle_t)hub->lock, portMAX_DELAY))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_SEMAPHORE_ERROR);
    }

    if (CONFIG_EVENTHUB_MAX_OBSERVERS == hub->observerCount)
    {
        retval = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_OUT_OF_RESOURCES);
    }

    if (retval == RETCODE_OK)
    {
        hub->observers[hub->observerCount].event = event;
        hub->observers[hub->observerCount].allEvents = false;
        hub->observers[hub->observerCount].handler = handler;
        hub->observerCount++;
    }

    if (pdPASS != xSemaphoreGive((SemaphoreHandle_t)hub->lock))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_SEMAPHORE_ERROR);
    }

    return retval;
}

/*  The description of the function is available in Kiso_EventHub.h */
Retcode_T EventHub_ObserveAll(EventHub_T *hub, EventHandler_T handler)
{
    Retcode_T retval = RETCODE_OK;

    if (NULL == hub || NULL == handler)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_NULL_POINTER);
    }

    if (NULL == hub->lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_UNINITIALIZED);
    }

    if (pdPASS != xSemaphoreTake((SemaphoreHandle_t)hub->lock, portMAX_DELAY))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_SEMAPHORE_ERROR);
    }

    if (CONFIG_EVENTHUB_MAX_OBSERVERS == hub->observerCount)
    {
        retval = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_OUT_OF_RESOURCES);
    }

    if (retval == RETCODE_OK)
    {
        hub->observers[hub->observerCount].allEvents = true;
        hub->observers[hub->observerCount].handler = handler;
        hub->observerCount++;
    }

    if (pdPASS != xSemaphoreGive((SemaphoreHandle_t)hub->lock))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_SEMAPHORE_ERROR);
    }

    return retval;
}

/*  The description of the function is available in Kiso_EventHub.h */
Retcode_T EventHub_Notify(EventHub_T *hub, TaskEvent_T event, const void *data)
{
    if (NULL == hub)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_NULL_POINTER);
    }

    if (NULL == hub->lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_UNINITIALIZED);
    }

    if (pdPASS != xSemaphoreTake((SemaphoreHandle_t)hub->lock, portMAX_DELAY))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_SEMAPHORE_ERROR);
    }

    uint32_t i;
    for (i = 0; i < hub->observerCount; i++)
    {
        if (NULL != hub->observers[i].handler)
        {
            if (hub->observers[i].allEvents || event == hub->observers[i].event)
            {
                hub->observers[i].handler(event, data);
            }
        }
    }

    if (pdPASS != xSemaphoreGive((SemaphoreHandle_t)hub->lock))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_SEMAPHORE_ERROR);
    }

    return RETCODE_OK;
}

#endif /* if KISO_FEATURE_EVENTHUB */
