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
 * @file Signal.c
 *
 * @brief Provide a mechanism to Wait for a notification
 *
 * @note
 */
#include "Kiso_Essentials.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_SIGNAL

/* additional interface header files */
#include "Kiso_Basics.h"

#ifdef __GNUC__

/* own header files */
#include "Kiso_HAL_Signal.h"
#include "Cmsis.h"

static Retcode_T NoOs_Signal_Wait(HAL_SignalHandle_T *handle, uint32_t timeout);
static Retcode_T NoOs_Signal_Notify(HAL_SignalHandle_T *handle);

HAL_Signal_WaitHook_T _Signal_WaitHook = NoOs_Signal_Wait;
HAL_Signal_NotifyHook_T _Signal_NotifyHook = NoOs_Signal_Notify;

Retcode_T HAL_Signal_SetHooks(HAL_Signal_WaitHook_T waitHook, HAL_Signal_NotifyHook_T notifyHook)
{
    if ( NULL != waitHook && NULL != notifyHook)
    {

        _Signal_WaitHook = waitHook;
        _Signal_NotifyHook = notifyHook;

        return RETCODE_OK;
    }
    else
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
}

Retcode_T HAL_Signal_Wait(HAL_SignalHandle_T *handle, uint32_t timeoutInMs)
{

    if (NULL != _Signal_WaitHook)
    {
        return _Signal_WaitHook((HAL_SignalHandle_T *) handle, timeoutInMs);
    }
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
}

Retcode_T HAL_Signal_Notify(HAL_SignalHandle_T * handle)
{
    if (NULL != _Signal_NotifyHook)
    {
        return _Signal_NotifyHook((HAL_SignalHandle_T *) handle);
    }
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
}

/* Note: __LDREXW and __STREXW are CMSIS functions */
/* timeout is not supported now.
 *
 * 0 = wait forever
 * 0 != timeout in ms
 */
static Retcode_T NoOs_Signal_Wait(HAL_SignalHandle_T *handle, uint32_t timeout)
{
    uint32_t status = 0;

    if (0 != timeout)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
    }

    if (0 != __get_IPSR())
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_SEMAPHORE_IN_ISR);
    }

    /* First Start initialize the Handle */
    if (handle->_Basic == (uint32_t) HAL_SIGNAL_NOTINIT)
    {
        handle->_Basic = (uint32_t) HAL_SIGNAL_EMPTY;
    }

    do
    {
        while (__LDREXW(&(handle->_Basic)) != (uint32_t) HAL_SIGNAL_FULL); /* Wait until Lock_Variable is free */
        status = __STREXW((uint32_t) HAL_SIGNAL_EMPTY, &(handle->_Basic)); /* Try to set Lock_Variable */
    } while (status != 0); /*retry until lock successfully*/
    /* Do not start any other memory access/ until memory barrier is completed*/
    __DMB();
    return RETCODE_OK;
}

/**
 * \note The code was taken from the book
 * 'The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors'
 *
 * It uses LDREXW (Load Register Exclusiv) and STREXW (Store Register Exclusive)
 * to realize a simple mutex without the need to disable interrupts.
 */
static Retcode_T NoOs_Signal_Notify(HAL_SignalHandle_T *handle)
{
    __DMB(); /* Ensure memory operations completed before releasing lock */
    handle->_Basic = (uint32_t) HAL_SIGNAL_FULL;
    return RETCODE_OK;
}

#endif /*__GNUC__*/
