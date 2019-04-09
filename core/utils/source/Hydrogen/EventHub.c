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

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_EVENTHUB

#include "BCDS_EventHub.h"

#if BCDS_FEATURE_EVENTHUB

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

#include <xdc/std.h>
#include <ICall.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

Retcode_T EventHub_Initialize(EventHub_T *hub)
{
    if (NULL == hub)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_NULL_POINTER);
    }

    (void)memset(hub, 0, sizeof(EventHub_T));

    /* Init semaphore */
    Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    hub->lock = (Semaphore_Handle)Semaphore_create((xdc_Int)0, &semParams,NULL);

    if(NULL == hub->lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_SEMAPHORE_ERROR);
    }

    Semaphore_post((Semaphore_Handle)hub->lock);

    return RETCODE_OK;
}

Retcode_T EventHub_Observe(EventHub_T *hub, EventHandler_T handler, TaskEvent_T event)
{
    Retcode_T retval = RETCODE_OK;

    if (NULL == hub || NULL == handler)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_NULL_POINTER);
    }

    if (NULL == hub->lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_UNINITIALIZED);
    }

    if(TRUE != Semaphore_pend((Semaphore_Handle)hub->lock, BIOS_WAIT_FOREVER))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_SEMAPHORE_ERROR);
    }

    if (CONFIG_EVENTHUB_MAX_OBSERVERS == hub->observerCount)
    {
        retval = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_OUT_OF_RESOURCES);
    }

    if (retval == RETCODE_OK)
    {
        hub->observers[hub->observerCount].event = event;
        hub->observers[hub->observerCount].allEvents = false;
        hub->observers[hub->observerCount].handler = handler;
        hub->observerCount++;
    }

    Semaphore_post((Semaphore_Handle)hub->lock);

    return retval;
}

Retcode_T EventHub_ObserveAll(EventHub_T *hub, EventHandler_T handler)
{
    Retcode_T retval = RETCODE_OK;

    if (NULL == hub || NULL == handler)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_NULL_POINTER);
    }

    if (NULL == hub->lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_UNINITIALIZED);
    }

    if(TRUE != Semaphore_pend((Semaphore_Handle)hub->lock, BIOS_WAIT_FOREVER))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_SEMAPHORE_ERROR);
    }

    if (CONFIG_EVENTHUB_MAX_OBSERVERS == hub->observerCount)
    {
        retval = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_OUT_OF_RESOURCES);
    }

    if (retval == RETCODE_OK)
    {
        hub->observers[hub->observerCount].allEvents = true;
        hub->observers[hub->observerCount].handler = handler;
        hub->observerCount++;
    }

    Semaphore_post((Semaphore_Handle)hub->lock);

    return retval;
}

Retcode_T EventHub_Notify(EventHub_T *hub, TaskEvent_T event, void *data)
{
    if (NULL == hub)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_NULL_POINTER);
    }

    if (NULL == hub->lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_UNINITIALIZED);
    }

    if(TRUE != Semaphore_pend((Semaphore_Handle)hub->lock, BIOS_WAIT_FOREVER))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T )RETCODE_SEMAPHORE_ERROR);
    }

    uint32_t i;
    for(i = 0; i < hub->observerCount; i++)
    {
        if (NULL != hub->observers[i].handler)
        {
            if (hub->observers[i].allEvents || event == hub->observers[i].event)
            {
                hub->observers[i].handler(event, data);
            }
        }
    }

    Semaphore_post((Semaphore_Handle)hub->lock);

    return RETCODE_OK;
}

#endif /* if BCDS_FEATURE_EVENTHUB */
