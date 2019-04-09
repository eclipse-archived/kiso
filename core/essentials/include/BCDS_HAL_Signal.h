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
 * @defgroup        HAL_SIGNAL Signal
 * @ingroup         BCDS_ESSENTIALS
 * @{
 *
 * @brief           This is API to support signaling between concurrent execution contexts, i.e. tasks and ISRs.
 *
 * @details
 * HAL provides a basic implementation of the signaling between concurrent execution contexts which is based on a
 * conditional variable and a waiting loop.
 *
 * If an RTOS is used, then hooks should be set to provide a proper signaling functions based on RTOS mechanisms
 * such as semaphores.
 */

#ifndef BCDS_HAL_SIGNAL_H_
#define BCDS_HAL_SIGNAL_H_

#include "BCDS_HAL.h"

/**
 * @brief       Defines the value to pass to HAL_Signal_Wait() in order to wait indefinitely.
 */
#define BCDS_HAL_SIGNAL_WAITFOREVER 0

/**
 * brief        Enumerates a signals state
 */
enum HAL_Signal_Basic_Instance_E
{
    HAL_SIGNAL_NOTINIT = 0xFFFFFFFF, /**< No-OS signal not initialized */
    HAL_SIGNAL_EMPTY = 0, /**< No-OS signal empty */
    HAL_SIGNAL_FULL = 1, /**< No-OS signal fulfilled */
};

/**
 * @brief       Macro to initialize a signal handle.
 */
#define HAL_SIGNAL_HANDLE_INIT  {HAL_SIGNAL_NOTINIT, NULL}

/**
 * @brief       Structure defining a signal object as a combination of a flag used in no-OS context and an abstract
 *              type that could hold a reference to a more complex signal object (e.g. semaphore)
 */
struct _HAL_SignalHandle_S
{
    volatile uint32_t _Basic; /**< Variable used in no-OS context as a signal in concurrent execution contexts*/
    void * _OsHandle; /**< reference to the signaling mechanism hooked-in from the OS in OS context */
};

typedef struct _HAL_SignalHandle_S HAL_SignalHandle_T;

/**
 * @brief       Type definition for the signal hook function pointer.
 *
 * @details     To use the API in OS context the user should pass to HAL_Signal_SetHooks() a reference to a function of
 *              this signature (**Retcode_T function(HAL_SignalHandle_T*, uint32_t)**), handling the wait operation
 *              of a signal object.
 *
 * @param[in]   *handle : reference to the signal to wait for.
 * @param[in]   timeoutInMs: time duration to wait for the signal.
 */
typedef Retcode_T (*HAL_Signal_WaitHook_T)(HAL_SignalHandle_T * handle, uint32_t timeoutInMs);

/**
 * @brief       Type definition for the signal hook function pointer.
 *
 * @details     To use the API in OS context the user should pass to HAL_Signal_SetHooks() a reference to a function of
 *              this signature (**Retcode_T function(HAL_SignalHandle_T*)**), handling the notify operation of a
 *              signal object.
 *
 * @param[in]   *handle : reference to the signal object issuing the notification.
 */
typedef Retcode_T (*HAL_Signal_NotifyHook_T)(HAL_SignalHandle_T * handle);

/**
 * @brief       Set RTOS hooks
 *
 * @details     If an RTOS is used, then hooks should be set to provide a proper wait/notify functions based on RTOS
 *              mechanisms such as semaphores.
 *
 * @param[in]   waitHook: the hook for the wait function
 *
 * @param[in]   notifyHook: the hook for the notify function
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when any one of the param has been NULL
 */
Retcode_T HAL_Signal_SetHooks(HAL_Signal_WaitHook_T waitHook, HAL_Signal_NotifyHook_T notifyHook);

/**
 * @brief       Waits for the signal
 *
 * @brief       The call blocks until the signal is reached or the timeout is expired.
 *
 * @param[in]   handle : Handle to a signal context
 *
 * @param[in]   timeoutInMs : timeout in milliseconds,
 *              - The value BCDS_HAL_SIGNAL_WAITFOREVER sets no Timeout i.e. waits for ever.
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when HAL_Signal_SetHooks() is not initialized.
 *
 * @note        The Usage of wait in ISR Context is not Supported!
 */
Retcode_T HAL_Signal_Wait(HAL_SignalHandle_T * handle, uint32_t timeoutInMs);

/**
 * @brief       Send a signal to handle context
 *
 * @param[in]   handle : Handle to a Signal context
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when HAL_Signal_SetHooks() is not initialized.
 */
Retcode_T HAL_Signal_Notify(HAL_SignalHandle_T * handle);

#endif /* BCDS_HAL_SIGNAL_H_ */
/** @} */
