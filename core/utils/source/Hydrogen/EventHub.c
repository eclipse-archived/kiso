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
 */
/*----------------------------------------------------------------------------*/

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
