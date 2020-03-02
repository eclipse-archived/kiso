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
 * @ingroup KISO_CELLULAR_COMMON
 * @defgroup ENGINE Engine
 * @{
 *
 * @brief The engine of the Cellular driver.
 *
 * @details This module manages the AtResponseParser-task and
 * CellularDriver-task and forwards data that is received by the cellular
 * communications channel to the AtResponseParser.
 *
 * The CellularDriver-task takes care of dispatching the AtResponse-queue and
 * the CellularRequest-queue, it therefore manages these two queues internally.
 *
 * The AtResponseParser-task is activated by incoming traffic from the
 * BSP-rxCallbacks. When the comm-channel holds sufficient data, the task will
 * start execution and consume the rxBuffers' content. The data is parsed by the
 * AtResponseParser-module and results are stored into the AtResponse-queue.
 * From there they may be consumed by various modules inside the
 * Cellular-driver.
 *
 * @file
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include "Kiso_Cellular.h"
#include "Kiso_CellularConfig.h"

#include "AtResponseParser.h"

#include "Kiso_MCU_UART.h"

#include "FreeRTOS.h"
#include "semphr.h"

/**
 * @brief AT command footer found at the end of every AT command.
 */
#define ENGINE_ATCMD_FOOTER ("\r\n")

/**
 * @brief Initializes the Engine. Allocates necessary RTOS resources and starts
 * the CellularDriver- and AtResponseParser-task. It also initializes the
 * request- and response-queue.
 *
 * @param [in] onStateChanged
 * Callback to be called in case of any state change.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Engine_Initialize(Cellular_StateChanged_T onStateChanged);

/**
 * @brief Deinitializes the Engine. Deallocates necessary RTOS resources and
 * stops the CellularDriver- and AtResponseParser-task. It also deinitializes
 * the request- and response-queue.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Engine_Deinitialize(void);

/**
 * @brief Enables or disables the fluke character filter. "Fluke characters" are
 * all chars/bytes less than 0x20 and greater than 0x7E, except
 * 0x0A and 0x0C (CR, LF). Such characters come to existence during the
 * cellular startup, probably when the modem turns on the level shifter side.
 *
 * @warning Beware that the fluke filter might interfere with the modems regular
 * operation and should only be used during the modem startup phase.
 *
 * @param[in] flukeFilterEnabled
 * @code true @endcode if the fluke filter is to be enabled,
 * @code false @endcode otherwise.
 */
void Engine_SetFlukeCharFilterEnabled(bool flukeFilterEnabled);

/**
 * @brief Transition the engine into a new state and notify the user.
 *
 * @param[in] newState
 * New state to transition to.
 *
 * @param param
 * Arbitrary parameter directly passed to the user-callback. May be NULL.
 *
 * @param len
 * Length of arbitrary parameter. May be zero.
 */
void Engine_NotifyNewState(Cellular_State_T newState, void *param, uint32_t len);

/**
 * @brief Sends a command to the modem.
 *
 * @param[in] buffer
 * The command to send.
 *
 * @param[in] bufferLength
 * The length of the command to send.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Engine_SendAtCommand(const uint8_t *buffer, uint32_t bufferLength);

/**
 * @brief Sends a command to the modem and wait for its echo.
 *
 * @param[in] str
 * The command to send as C-string.
 *
 * @param[in] bufferLength
 * The length of the command to send.
 *
 * @param[in] timeout
 * The time to wait for a command echo in milliseconds.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Engine_SendAtCommandWaitEcho(const uint8_t *str, uint32_t bufferLength, uint32_t timeout);

typedef Retcode_T (*CellularRequest_CallableFunction_T)(void *parameter, uint32_t ParameterLength);

/**
 * @brief Enqueues the given item for execution by the CellularDriver-task.
 * The caller can wait for the queue to become available (if full), by setting
 * the parameter timeout to x > 0. If a timeout of 0 is passed, the caller will
 * not wait for the queue to be available and perhaps return
 * RETCODE_CELLULAR_DRIVER_BUSY if the queue is full.
 *
 * @param[in] function
 * A valid pointer to a #CellularRequest_CallableFunction_T.
 *
 * @param[in] timeout
 * The max. timeout in ticks that the caller should be waiting for the
 * availability of dispatcher.
 *
 * @param[in] parameter
 * An optional pointer to some user-defined structure to be passed to the
 * #CellularRequest_CallableFunction_T-execution (can be NULL).
 *
 * @param[in] parameterLength
 * An optional length to be passed to the
 * #CellularRequest_CallableFunction_T-execution.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Engine_Dispatch(CellularRequest_CallableFunction_T function, uint32_t timeout, void *parameter, uint32_t parameterLength);

/**
 * @brief Notify the engine transceiver in case echo mode is entered or exited.
 * This will only have an impact on the #Engine_SendAtCommandWaitEcho()
 * function, which if echoMode is set to @code false @endcode will behave just
 * like #Engine_SendAtCommand().
 *
 * @param[in] echoMode
 * Set @code true @endcode to enable echo-waiting, @code false @endcode otherwise.
 */
void Engine_EchoModeEnabled(bool echoMode);

/**
 * @brief Utility buffer which can be used by the AT command handlers for
 * generating their AT strings and send via #Engine_SendAtCommand() or
 * #Engine_SendAtCommandWaitEcho(). This way all command generators share a
 * common data-pool and don't have to allocate large buffers on the stack or
 * locally within their modules.
 *
 * @note The size of this buffer can be configured in the config-header. Take
 * special care to choose it large enough to support all the AT commands you
 * expect to use. With regards to Sockets and UDP, it is recommended to make
 * this buffer large enough to fit a full UDP packet + AT command overhead.
 */
extern char Engine_AtSendBuffer[CELLULAR_AT_SEND_BUFFER_SIZE];

#endif /* ENGINE_H_ */

/** @} */
