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
 * @defgroup ATRESPONSEQUEUE AT Response Queue
 * @{
 *
 * @brief Defines the interface for the AT response queue.
 *
 * @details Defines the interface for the AT response queue. This queue is used within
 * driver functions to handle AT command responses from the modem. Beware that
 * the buffers passed here are '\0' terminated strings. This interface enables us
 * to write code such as:
 * @code{.c}
 *
 * Retcode_T DidWait;
 * uint8 *ReceiveBuffer;
 * uint32_t ReceiveBufferLen;
 *
 * // responds with +COPS: 0,0,"Swisscom",2
 * UartSend("at+cops?");
 *
 * DidWait = AtResponseQueue_WaitForNamedCmd(DEFAULT_TIMEOUT, "COPS");
 * if(DidWait != RETCODE_OK) FAIL;
 *
 * DidWait = AtResponseQueue_WaitForCmdArg(DEFAULT_TIMEOUT, NULL, NULL);
 * if(DidWait != RETCODE_OK) FAIL;
 *
 * DidWait = AtResponseQueue_WaitForCmdArg(DEFAULT_TIMEOUT, NULL, NULL);
 * if(DidWait != RETCODE_OK) FAIL;
 *
 * DidWait = AtResponseQueue_WaitForCmdArg(DEFAULT_TIMEOUT, ReceiveBuffer, ReceiveBufferLen);
 * if(DidWait != RETCODE_OK) FAIL;
 * ReceiveBuffer[ReceiveBufferLen] = 0;
 * LOG_INFO("Connected to network: %s", ReceiveBuffer);
 * AtResponseQueue_MarkBufferAsUnused(ReceiveBuffer, ReceiveBufferLen);
 *
 * DidWait = AtResponseQueue_WaitForCmdArd(DEFAULT_TIMEOUT, NULL, NULL);
 * if(DidWait != RETCODE_OK) FAIL;
 *
 * DidWait = AtResponseQueue_WaitForNamedResponse(DEFAULT_TIMEOUT, AT_RESPONSE_CODE_OK);
 * if(DidWait != RETCODE_OK) FAIL;
 * @endcode
 *
 * @file
 */

#ifndef AT_RESPONSE_QUEUE_H_
#define AT_RESPONSE_QUEUE_H_

#include "AtResponseParser.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/**
 * @brief The size of the AT response ring buffer in bytes
 */
#define AT_RESPONSE_QUEUE_BUFFER_SIZE (CELLULAR_AT_SEND_BUFFER_SIZE * 2)

/**
 * @brief The types of events triggered by the AT response parser
 * @details The enum will be repsented as a enumeration using one bit per type
 * for better optimization. Types will be used to filter events in the queue
 * through the event mask.
 */
typedef enum
{
    AT_EVENT_TYPE_COMMAND_ECHO = (1 << 0),
    AT_EVENT_TYPE_COMMAND = (1 << 1),
    AT_EVENT_TYPE_COMMAND_ARG = (1 << 2),
    AT_EVENT_TYPE_RESPONSE_CODE = (1 << 3),
    AT_EVENT_TYPE_MISC = (1 << 4),
    AT_EVENT_TYPE_ERROR = (1 << 5),
    AT_EVENT_TYPE_OUT_OF_RANGE = (1 << 6)
} AtEventType_T;

/**
 * @brief
 *   the macro enables all featured events
*/
#define AT_EVENT_TYPE_ALL (AT_EVENT_TYPE_COMMAND_ECHO | AT_EVENT_TYPE_COMMAND | AT_EVENT_TYPE_COMMAND_ARG | AT_EVENT_TYPE_RESPONSE_CODE | AT_EVENT_TYPE_MISC | AT_EVENT_TYPE_ERROR)

/**
 * @brief
 *   the macro enables all featured events except misc
*/
#define AT_EVENT_TYPE_ALL_EXCEPT_MISC (AT_EVENT_TYPE_COMMAND_ECHO | AT_EVENT_TYPE_COMMAND | AT_EVENT_TYPE_COMMAND_ARG | AT_EVENT_TYPE_RESPONSE_CODE | AT_EVENT_TYPE_ERROR)

/**
 * @brief An entry in the AT response queue
 */
typedef struct
{
    AtEventType_T Type;
    AtResponseCode_T ResponseCode;
    uint32_t BufferLength;
    uint8_t Buffer[];
} AtResponseQueueEntry_T;

/**
 * @brief Enumeration representing the result of an AT response queue call.
 */
typedef enum
{
    RETCODE_AT_RESPONSE_QUEUE_TIMEOUT = AT_RESPONSE_PARSER_LAST_CUSTOM_CODE,
    RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT,
    RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT,
    RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE,
    RETCODE_AT_RESPONSE_QUEUE_LAST_CUSTOM_CODE
} AtResponseQueueRetcode_T;

/**
 * @brief Initializes the AT response queue by setting up the queue itself.
 *
 * @retval RETCODE_OK     The queue was setup correctly
 * @retval RETCODE_OUT_OF_RESOURCES Not enough memory to setup the queue
 */
Retcode_T AtResponseQueue_Init(void);

/**
 * @brief Initializes the AT response queue by setting up the queue itself.
 *
 * @retval RETCODE_OK               The queue was setup correctly
 * @retval RETCODE_INVALID_PARAM    Queue not initialized
 */
Retcode_T AtResponseQueue_Deinit(void);

/**
 * @brief Connects the event queue with the response parser. After calling this method,
 * any previously registered AT response parser callback will be deregistered.
 */
void AtResponseQueue_RegisterWithResponseParser(void);

/**
 * @brief Resets the response queue and AT response parser. Use this to restore a known state after an error.
 */
void AtResponseQueue_Reset(void);

/**
 * @brief Waits on the response queue for a command echo event. The received command
 * has to match the name for this function to unblock. If the received event was
 * not a command echo, or did not match the name, the event will not be removed from
 * the queue to enable subsequent error handling.
 *
 * @warning If a Retcode other than RETCODE_OK was received, this function did not remove
 * the event from the queue (to allow subsequent error handling). Thus, calling
 * this function on the queue immediately again, will again yield the same Retcode.
 *
 * @param[in] timeout
 * The time to wait for a command in milliseconds
 * @param[in] buffer
 * The echo string to wait for
 * @param[in] BufferLength
 * The length of the echo string
 *
 * @retval RETCODE_OK A matching command echo was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type, and not removed from the queue.
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT if no matching command echo was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForNamedCmdEcho(uint32_t timeout, const uint8_t *buffer, uint32_t BufferLength);

/**
 * @brief Waits on the response queue for a an arbitrary command echo event. The name of
 * command that unblocked this function is passed out through the buffer pointer.
 * If the received event was not a command echo, the event will not be removed from
 * the queue to enable subsequent error handling; then, no buffer pointer will be returned.
 * If a buffer pointer is received, that buffer has to be marked as unused once it is no longer needed.
 *
 * @warning Beware that you have to mark the received buffer as unused, otherwise you'll cause memory leaks
 * and prevent the driver from working correctly. DO NOT CALL free on this pointer, but use AtResponseQueue_MarkBufferAsUnused.
 *
 * @warning If a Retcode other than RETCODE_OK was received, this function did not remove
 * the event from the queue (to allow subsequent error handling). Thus, calling
 * this function on the queue immediately again, will again yield the same Retcode.
 *
 * @param[in] timeout
 * The time to wait for a command in milliseconds
 * @param[out] BufferPtr
 * The buffer pointer to store the address of the command name buffer in or NULL
 * if you are not interested in the argument content.
 * @param[out] BufferLen
 * The pointer where to store the length of the command name buffer in or NULL
 * if you are not interested in the argument content.
 *
 * @retval RETCODE_OK A command echo was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type, and was not removed from the queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT if no command echo was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForArbitraryCmdEcho(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen);

/**
 * @brief Waits on the response queue for a command event. The received command
 *     has to match the name for this function to unblock. If the received event was
 *     not a command, or did not match the name, the event will not be removed from
 *     the queue to enable subsequent error handling.
 *
 * @warning If a Retcode other than RETCODE_OK was received, this function did not remove
 *    the event from the queue (to allow subsequent error handling). Thus, calling
 *    this function on the queue immediately again, will again yield the same Retcode.
 *
 * @param[in] timeout
 * The time to wait for a command in milliseconds
 * @param[in] buffer
 * The name of the command to wait for
 * @param[in] BufferLength
 * The length of the name string
 *
 * @retval RETCODE_OK A matching command response was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type or did not match the name, and was not removed from the queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT if no matching command was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForNamedCmd(uint32_t timeout, const uint8_t *buffer, uint32_t BufferLength);

/**
 * @brief Waits on the response queue for a an arbitrary command event. The name of
 * command that unblocked this function is passed out through the buffer pointer.
 * If the received event was not a command, the event will not be removed from
 * the queue to enable subsequent error handling; then, no buffer pointer will be returned.
 * If a buffer pointer is received, that buffer has to be marked as unused once it is no longer needed.
 *
 * @warning Beware that you have to mark the received buffer as unused, otherwise you'll cause memory leaks
 * and prevent the driver from working correctly. DO NOT CALL free on this pointer, but use AtResponseQueue_MarkBufferAsUnused.
 *
 * @warning If a Retcode other than RETCODE_OK was received, this function did not remove
 * the event from the queue (to allow subsequent error handling). Thus, calling
 * this function on the queue immediately again, will again yield the same Retcode.
 *
 * @param[in] timeout
 * The time to wait for a command in milliseconds
 * @param[out] BufferPtr
 * The buffer pointer to store the address of the command name buffer in
 * or NULL if you are not interested in the argument content.
 * @param[out] BufferLen
 * The pointer where to store the length of the command name buffer in
 * or NULL if you are not interested in the argument content.
 *
 * @retval RETCODE_OK A command response was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type, and was not removed from the queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT if no command was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForArbitraryCmd(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen);

/**
 * @brief Waits on the response queue for a command event. The received command argument
 * has to match the value for this function to unblock. If the received event was
 * not a command argument, or did not match the value, the event will not be removed from
 * the queue to enable subsequent error handling.
 *
 * @warning If a Retcode other than RETCODE_OK was received, this function did not remove
 * the event from the queue (to allow subsequent error handling). Thus, calling
 * this function on the queue immediately again, will again yield the same Retcode.
 *
 * @param[in] timeout
 * The time to wait for a command in milliseconds
 * @param[in] buffer
 * The command argument value to wait for
 * @param[in] BufferLength
 * The length of the command argument
 *
 * @retval RETCODE_OK A matching command response was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type or did not match the name, and was not removed from the queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT if no matching command was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForNamedCmdArg(uint32_t timeout, const uint8_t *buffer, uint32_t BufferLength);

/**
 * @brief Waits until a command argument is received on the response queue (or the timeout is reached).
 * If the received event was not a command argument, the event will not be removed from
 * the queue to enable subsequent error handling; then, no buffer pointer will be returned.
 * If a buffer pointer is received, that buffer has to be marked as unused once it is no longer needed.
 *
 * @warning Beware that you have to mark the received buffer as unused, otherwise you'll cause memory leaks
 * and prevent the driver from working correctly. DO NOT CALL free on this pointer, but use AtResponseQueue_MarkBufferAsUnused.
 *
 * @warning If a Retcode other than RETCODE_OK was received, this function did not remove
 * the event from the queue (to allow subsequent error handling). Thus, calling
 * this function on the queue immediately again, will again yield the same Retcode.
 *
 * @param[in] timeout    The time to wait for a command argument in milliseconds
 * @param[out] BufferPtr The buffer pointer to store the address of the command arg buffer in
 *        or NULL if you are not interested in the argument content.
 * @param[out] BufferLen The pointer where to store the length of the command arg buffer in
 *        or NULL if you are not interested in the argument content.
 *
 * @retval RETCODE_OK A command argument was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type, and was not removed from the queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT No command argument was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForArbitraryCmdArg(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen);

/**
 * @brief Waits until an specfic response code is received on the response queue (or the timeout is reached).
 * If the received event was not a response code or the response code did not match the expected one,
 * the event will not be removed from the queue to enable subsequent error handling.
 *
 * @warning If a Retcode other than RETCODE_OK was received, this function did not remove
 * the event from the queue (to allow subsequent error handling). Thus, calling
 * this function on the queue immediately again, will again yield the same Retcode.
 *
 * @param[in] timeout
 * The time to wait for the response code in milliseconds
 * @param[in] response
 * The response code to wait for
 *
 * @retval RETCODE_OK A matching response code was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type, and was not removed from the queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT   No response code was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForNamedResponseCode(uint32_t timeout, AtResponseCode_T response);

/**
 * @brief Waits until an arbitrary response code is received on the response queue (or the timeout is reached).
 *     The perticular response code is stored returned through the response parameter (unless that one is NULL).
 *     If the received event was not a response code, the event will not be removed from the queue to enable
 *     subsequent error handling.
 *
 * @warning If a Retcode other than RETCODE_OK was received, this function did not remove
 *    the event from the queue (to allow subsequent error handling). Thus, calling
 *    this function on the queue immediately again, will again yield the same Retcode.
 *
 * @param[in] timeout
 * time to wait for a response code in milliseconds
 * @param[in] response
 * Out parameter to store the received response code
 *
 * @retval RETCODE_OK A response code was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type, and was not removed from the queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT   No response code was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForArbitraryResponseCode(uint32_t timeout, AtResponseCode_T *response);

/**
 * @brief Waits until miscellaneous content is received on the response queue (or the timeout is reached).
 * If the received event was not miscellaneous content, the event will not be removed from
 * the queue to enable subsequent error handling; then, no buffer pointer will be returned.
 * If a buffer pointer is received, that buffer has to be marked as unused once it is no longer needed.
 *
 * @warning Beware that you have to mark the received buffer as unused, otherwise you'll cause memory leaks
 * and prevent the driver from working correctly. DO NOT CALL free on this pointer, but use AtResponseQueue_MarkBufferAsUnused.
 *
 * @param[in] timeout
 * The time to wait for a command in milliseconds
 * @param[out] BufferPtr
 * The buffer pointer to store the address of the content buffer in
 * or NULL if you are not interested in the argument content.
 * @param[out] BufferLen
 * The pointer where to store the length of the content buffer in
 * or NULL if you are not interested in the argument content.
 *
 * @retval RETCODE_OK Some miscellaneous content was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type, and was not removed from the queue
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT if no miscellaneous content was received within the waiting time
 */
Retcode_T AtResponseQueue_WaitForMiscContent(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen);

/**
 * @brief Waits until an event is received, cleans up buffers and then ignores the event. This function is basically
 * a wrapper around AtResponseQueue_GetEvent() to free users from having to cleanup events they are not interested
 * in.
 *
 * @param[in] timeout
 * The time to wait for an event to happen in milliseconds
 *
 * @return RETCODE_OK  An event was received within the waiting time
 * @return RETCODE_AT_RESPONSE_QUEUE_TIMEOUT No event was received within the waiting time
 */
Retcode_T AtResponseQueue_IgnoreEvent(uint32_t timeout);

/**
 * @brief Dequeues an entry from the AT response queue - or times out trying to do so.
 * Under all circumstances, the buffer has to be marked as unused once it is no longer needed.
 *
 * @warning Whenever you can, prefer a WaitFor* function over this one. This function is intended for very complex
 * or irregular cases (such as modem startup). The WaitFor* functions result in semantically richer code.
 *
 * @warning Beware that you have to mark the received buffer as unused, otherwise you'll cause memory leaks
 * and prevent the driver from working correctly. DO NOT CALL free on this pointer, but use AtResponseQueue_MarkBufferAsUnused.
 *
 * @param[in] timeout    The time to wait for a command in milliseconds
 * @param[out] entry  The out parameter to store the dequeued entry in
 *
 * @retval RETCODE_OK  Some event was received within the waiting time
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT if no event was received within the waiting time
 */
Retcode_T AtResponseQueue_GetEvent(uint32_t timeout, AtResponseQueueEntry_T **entry);

/**
 * @brief Marks a chunk of memory as unused, so that it can be reused by the AT response queue for future
 *     response data received. Beware that, after calling this function on a previously received message,
 *     the content of said message is no longer valid.
 */
void AtResponseQueue_MarkBufferAsUnused(void);

/**
 * @brief Gets number of events remained in the AT response queue.
 *
 * @return A number of available events.
 */
uint32_t AtResponseQueue_GetEventCount(void);

/**
 * @brief Enqueues an event in the response event queue
 *
 * @param[in] EventType
 * The type of the event to enqueue
 * @param[in] arg
 * The '\0' terminated payload of the event or NULL if there is none
 * @param[in] len
 * The length of the event payload
 */
void AtResponseQueue_EnqueueEvent(AtEventType_T EventType, const uint8_t *arg, uint32_t len);

/**
 * @brief set the event mask of the at response queue
 *
 * @param[in] eventMask of EventType_T events
 * @returns whether or not the provided event mask is valid or not
 * @retval  RETCODE_OK - if the mask is in the valid range
 * @retval  RETCODE_INVALID_PARAMETER - if the parameter is out of range
 */
Retcode_T AtResponseQueue_SetEventMask(uint32_t eventMask);

/**
 * @brief get the event mask of the AT-response-queue
 *
 * @returns the currently set event mask
 */

uint32_t AtResponseQueue_GetEventMask(void);

/**
 * @brief Clears the content of the response event queue
 *
 * @note In debug mode dumps the content of the queue prior to cleanup
 */
void AtResponseQueue_Clear(void);

#endif /* AT_RESPONSE_QUEUE_H_ */

/** @} */
