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
 * @ingroup UTILS
 *
 * @defgroup EVENTHUB EventHub
 * @{
 *
 * @brief
 *      A framework for distributing events in a system
 *
 * @details
 *      A module can register a callback to be called when another module informs
 *      the system that an event has occurred. This can be used for detecting
 *      connection to or disconnection from a network, updated LWM2M resources and
 *      so on. By default, the module allows for up to 16 observers per event hub
 *      but this can be overridden by changing the CONFIG_EVENTHUB_MAX_OBSERVERS flag.
 *
 * @file
 **/

#ifndef KISO_EVENTHUB_H_
#define KISO_EVENTHUB_H_

#include "Kiso_Utils.h"

#if KISO_FEATURE_EVENTHUB

/* KISO interface header files */
#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/* May be overridden by the project makefile */
#ifndef CONFIG_EVENTHUB_MAX_OBSERVERS
#define CONFIG_EVENTHUB_MAX_OBSERVERS (16)
#endif

typedef uint32_t TaskEvent_T;

typedef void (*EventHandler_T)(TaskEvent_T event, void *data);

struct EventObserver_S
{
    TaskEvent_T event;
    bool allEvents;
    EventHandler_T handler;
};
typedef struct EventObserver_S EventObserver_T;

struct EventHub_S
{
    uint32_t observerCount;
    EventObserver_T observers[CONFIG_EVENTHUB_MAX_OBSERVERS];
    void *lock;
};
typedef struct EventHub_S EventHub_T;

/**
 * @brief
 *      This function initializes a given event hub handle.
 *
 * @param[in,out] hub
 *      A pointer to an EventHub structure
 *
 * @retval #RETCODE_OK
 *      When the event hub is initialized successfully
 * @retval #RETCODE_NULL_POINTER
 *      When the hub parameter pointer is NULL
 * @retval #RETCODE_SEMAPHORE_ERROR
 *      When the associated lock is not created successfully
 * 
 */
Retcode_T EventHub_Initialize(EventHub_T *hub);

/**
 * @brief
 *      This function adds an observe of a given event to a hub
 *
 * @param[in] hub
 *      A pointer to an EventHub structure
 * @param[in] handler
 *      A function pointer to the user-code run function that
 *      should be called when the event is signaled
 * @param[in] event
 *      The event that should be observed
 *
 * @retval #RETCODE_OK
 *      When the observation is added successfully
 * @retval #RETCODE_NULL_POINTER
 *      When the hub parameter pointer is NULL
 * @retval #RETCODE_UNINITIALIZED
 *      When the hub has not been initialized previously
 * @retval #RETCODE_SEMAPHORE_ERROR
 *      When the hub lock can't be taken or released successfully
 * @retval #RETCODE_OUT_OF_RESOURCES
 *      When the maximum number of observation is already reached
 *      (see #CONFIG_EVENTHUB_MAX_OBSERVERS)
 * 
 */
Retcode_T EventHub_Observe(EventHub_T *hub, EventHandler_T handler, TaskEvent_T event);

/**
 * @brief
 *      This function adds an observe of all events to a hub
 *
 * @param[in] hub
 *      A pointer to an EventHub structure
 * @param[in] handler
 *      A function pointer to the user-code run function that
 *      should be called when the event is signaled
 *
 * @retval #RETCODE_OK
 *      When observations for all event are added successfully
 * @retval #RETCODE_NULL_POINTER
 *      When hub or handler pointer is NULL
 * 
 */
Retcode_T EventHub_ObserveAll(EventHub_T *hub, EventHandler_T handler);

/**
 * @brief
 *      This function informs the hub that a given event has occurred
 *
 * @param[in] hub
 *      A pointer to an EventHub structure
 * @param[in] event
 *      The event that occurred
 * @param[in] data
 *      Additional data to hand to the observer
 *
 * @note
 *      Event notifications from interrupt context is not supported.
 *
 * @retval #RETCODE_OK
 *      When all observers have been notified successfully
 * @retval #RETCODE_NULL_POINTER
 *      When the hub pointer is NULL
 * @retval #RETCODE_UNINITIALIZED
 *      When the hub has not been initialized previously
 * @retval #RETCODE_SEMAPHORE_ERROR
 *      When the hub lock can't be taken or released successfully
 * 
 */
Retcode_T EventHub_Notify(EventHub_T *hub, TaskEvent_T Event, void *data);

#endif /* if KISO_FEATURE_EVENTHUB */

#endif /* KISO_EVENTHUB_H_ */
/**@} */
