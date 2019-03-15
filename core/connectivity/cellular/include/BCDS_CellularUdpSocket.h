/*----------------------------------------------------------------------------*/
/**
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

/**
 * @file
 * @brief BCDS Cellular Udp Socket Interface
 */

/**
 * @defgroup CellularUdpSocket
 * @ingroup Cellular CellularUdpSocket
 * @{
 *
 * @brief This module is used to socket open. send, receive, close and resolve domain name.
 */

#ifndef BCDS_CELLULARUPDSOCKET_H
#define BCDS_CELLULARUPDSOCKET_H

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_CellularConfig.h"

/**
 * @brief   Typedef to represent the cellular UDP socket id
 */
typedef int32_t CellularUdpSocket_T;

/**
 * @brief   Typedef to represent the cellular UDP event
 */
typedef enum
{
    CELLULAR_UDP_EVENT_DATA_RECEIVED,    /**< Udp socket data received */
    CELLULAR_UDP_EVENT_SOCKET_CLOSE,     /**< UDP socket close. */
    CELLULAR_UDP_EVENT_SOCKET_ERROR,     /**< A serious socket error accorded. */
} CellularUdpEvent_T;

/**
 * @brief   This data type represents a function pointer which would be executed
 *          when any socket event is to be notified.
 *
 * @param [in]  socket UDP socket handle of the local machine.
 * @param [in]  ipAddress   Pointer to the IP address of the remote machine.
 * @param [in]  port UDP port number on the remote machine.
 * @param [in]  bufferPointer   Pointer to buffer containing the received data.
 * @param [in]  length  Number of bytes in the received packet.
 *
 */
typedef void (*CellularUdpSocket_Callback_T) (CellularUdpSocket_T socket, CellularUdpEvent_T event);

/**
  * @brief   Pointer to create cellular UDP socket. Allocate a free UDP socket.
  *
  * @param [out] socket :   returned socket ID
  *
  * @param [in]  callback   :   an eventcallback being called after receiving UDP Socket events
  *
  * @retval RETCODE_OK: if the socket is opened
  * @retval RETCODE_INVALID_PARAM: If socket value and callback was null
  * @retval RETCODE_NULL_POINTER: If function pointer is null
  * @retval RETCODE_UNINITIALIZED: If semaphore handle is null
  * @retval RETCODE_CELLULAR_DRIVER_BUSY: If semaphore has timeout and unable to unlock the request lock
  * @retval RETCODE_OUT_OF_RESOURCES: If socket is unavailable
  * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE: If Buffer length mismatches or wrong response received
  * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT: If unable to get queue entry
  * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT: If AT event type error occured
  * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT: If wrong AT event occured
  * @retval RETCODE_FAILURE: If the value is out of range
  * @retval RETCODE_CELLULAR_SOCKET_BIND_FAILED: If Tcp Ip error occured and Other return values depends on #MCU_UART_Send
  */
Retcode_T CellularUdpSocket_Open(CellularUdpSocket_T *socket, CellularUdpSocket_Callback_T callback);

/**
  * @brief   Function to listen to a UPP socket on a specific port.
  *
  * @param [in] socket  :   the socket to listen on
  *
  * @param [in] port    : the local port to listen on
  *
  * @retval RETCODE_NOT_SUPPORTED : Socket Listen is not supported
  *
  */
Retcode_T CellularUdpSocket_Listen(CellularUdpSocket_T socket, uint16_t port);

/**
 *
 * @brief The function to receive the incoming data via cellular UDP socket.
 *
 * When some incoming data arrives, the cellular driver invokes the callback, which
 * is associated with the corresponding socket. The callback has been passed
 * as a parameter to the CellularUdpSocket_Open(). This receive function must be
 * invoked within the callback to get the incoming packet. The received data
 * must be consumed before the return of the callback. The buffer, which
 * contains the data, will be released after the return of the callback.
 *
 * @param [in]  socket
 * UDP socket handle
 *
 * @param [out] ipAddress
 * The source IP address
 *
 * @param [out] port
 * The source UDP port number
 *
 * @param [out] buffer
 * A pointer to buffer containing the incoming data
 *
 * @param [out] length
 * The number of received bytes
 *
 * @retval Status of the UDP receive operation, RETCODE_OK on success
 * @retval RETCODE_INVALID_PARAM If socket id is greater than the maximum or there is null parameters and Other return values depends on #MCU_UART_Send
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED If modem responses aborted
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR If modem responses error
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER If modem responses other
 */
Retcode_T CellularUdpSocket_Receive(CellularUdpSocket_T socket, uint32_t *ipAddress, uint16_t *port,
        uint8_t **buffer, uint32_t * length);

/**
 * @brief This function closes a socket and releases any related resources.
 *
 * If the socket is already closed, then this function has no effect.
 *
 * @param[in] socket
 * It must be a valid socket.
 *
 * @retval RETCODE_OK if the socket is closed
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null
 * @retval RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock and Other return values depends on #MCU_UART_Send
 * @retval RETCODE_SEMAPHORE_ERROR If serial transfer is failed
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED If modem responses aborted
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR If modem responses error
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER If modem responses other
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 */
Retcode_T CellularUdpSocket_Close(CellularUdpSocket_T socket);

/**
 * @brief   The function to send data via cellular UDP socket.
 *
 * This function is blocking. In other words, after the return of this function
 * call, the packet to be sent has been transferred completely to the cellular
 * modem and the modem has acknowledged the transfer. Mind that is not guaranteed
 * that the modem has already sent the packet over
 * the network. The sending to the network happens asynchronously.
 *
 * @param [in]  socket
 * UDP socket handle
 *
 * @param [in]  ipAddress
 * The destination IP address
 *
 * @param [in]  port
 * The destination UDP port number
 *
 * @param [in]  buffer
 * A pointer to buffer containing the data to be sent
 *
 * @param [in]  length
 * The number of bytes in the packet to be sent
 *
 * @retval Status of the UDP send operation, RETCODE_OK on success
 * @retval RETCODE_INVALID_PARAM If socket id is greater than the maximum or there is null parameters
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null and Other return values depends on #MCU_UART_Send
 * @retval RETCODE_SEMAPHORE_ERROR If serial transfer is failed
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 */
Retcode_T CellularUdpSocket_Send(CellularUdpSocket_T socket, const uint32_t ipAddress, uint16_t port,
        uint8_t *buffer, uint32_t length);

/**
 * @brief Resolves a domain name to an IP address via the network provided DNS server.
 *
 * @note  Ensure the modem is in a packet switched data context before calling this function.
 *
 * @param[in]   DomainName          The fully qualified domain name to resolve (e.g., google.com)
 * @param[in]   DomainNameLength    The length of the domain name in bytes
 * @param[out]  IpAddress           The resolved IP address if the return value was RETCODE_OK
 *
 * @retval RETCODE_NOT_SUPPORTED  Resolve Name is not supported for quectel
 * @retval RETCODE_OK on success
 * @retval RETCODE_INVALID_PARAM If there are null parameters
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null
 * @retval RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock
 * @retval RETCODE_FAILURE If the IP address length is not more than two chars
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED If modem responses aborted
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR If modem responses error
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER If modem responses other
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 */
Retcode_T CellularUdpSocket_ResolveIp(uint8_t *DomainName, uint32_t DomainNameLength, uint32_t *IpAddress);

/**
  * @deprecated since version vxx.xx.xx, replaced by #CellularUdpSocket_Open
  * @todo replace vxx.xx.xx with next version
  *
  * @brief   Pointer to create cellular UDP socket. Allocate a free UDP socket.
  *
  * @param [out] socket :   returned socket ID
  *
  * @param [in]  callback   :   an eventcallback being called after receiving UDP Socket events
  *
  * @retval RETCODE_OK: if the socket is opened
  * @retval RETCODE_INVALID_PARAM: If socket value and callback was null
  * @retval RETCODE_NULL_POINTER: If function pointer is null
  * @retval RETCODE_UNINITIALIZED: If semaphore handle is null
  * @retval RETCODE_CELLULAR_DRIVER_BUSY: If semaphore has timeout and unable to unlock the request lock
  * @retval RETCODE_OUT_OF_RESOURCES: If socket is unavailable
  * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE: If Buffer length mismatches or wrong response received
  * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT: If unable to get queue entry
  * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT: If AT event type error occured
  * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT: If wrong AT event occured
  * @retval RETCODE_FAILURE: If the value is out of range
  * @retval RETCODE_CELLULAR_SOCKET_BIND_FAILED: If Tcp Ip error occured and Other return values depends on #MCU_UART_Send
  */
BCDS_DEPRECATED(Retcode_T CellularSocketUdp_Open(CellularUdpSocket_T *socket, CellularUdpSocket_Callback_T callback));

/**
  * @deprecated since version vxx.xx.xx, replaced by #CellularUdpSocket_Listen
  * @todo replace vxx.xx.xx with next version
  *
  * @brief   Function to listen to a UPP socket on a specific port.
  *
  * @param [in] socket  :   the socket to listen on
  *
  * @param [in] port    : the local port to listen on
  *
  * @retval RETCODE_NOT_SUPPORTED : Socket Listen is not supported
  *
  */
BCDS_DEPRECATED(Retcode_T CellularSocketUdp_Listen(CellularUdpSocket_T socket, uint16_t port));

/**
 * @deprecated since version vxx.xx.xx, replaced by #CellularUdpSocket_Receive
 * @todo replace vxx.xx.xx with next version
 *
 * @brief The function to receive the incoming data via cellular UDP socket.
 *
 * When some incoming data arrives, the cellular driver invokes the callback, which
 * is associated with the corresponding socket. The callback has been passed
 * as a parameter to the CellularUdpSocket_Open(). This receive function must be
 * invoked within the callback to get the incoming packet. The received data
 * must be consumed before the return of the callback. The buffer, which
 * contains the data, will be released after the return of the callback.
 *
 * @param [in]  socket
 * UDP socket handle
 *
 * @param [out] ipAddress
 * The source IP address
 *
 * @param [out] port
 * The source UDP port number
 *
 * @param [out] buffer
 * A pointer to buffer containing the incoming data
 *
 * @param [out] length
 * The number of received bytes
 *
 * @retval Status of the UDP receive operation, RETCODE_OK on success
 * @retval RETCODE_INVALID_PARAM If socket id is greater than the maximum or there is null parameters and Other return values depends on #MCU_UART_Send
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED If modem responses aborted
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR If modem responses error
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER If modem responses other
 */
BCDS_DEPRECATED(Retcode_T CellularSocketUdp_Receive(CellularUdpSocket_T socket, uint32_t *ipAddress, uint16_t *port,
        uint8_t **buffer, uint32_t * length));

/**
 * @deprecated since version vxx.xx.xx, replaced by #CellularUdpSocket_Close
 * @todo replace vxx.xx.xx with next version
 *
 * @brief This function closes a socket and releases any related resources.
 *
 * If the socket is already closed, then this function has no effect.
 *
 * @param[in] socket
 * It must be a valid socket.
 *
 * @retval RETCODE_OK if the socket is closed
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null
 * @retval RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock and Other return values depends on #MCU_UART_Send
 * @retval RETCODE_SEMAPHORE_ERROR If serial transfer is failed
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED If modem responses aborted
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR If modem responses error
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER If modem responses other
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 */
BCDS_DEPRECATED(Retcode_T CellularSocketUdp_Close(CellularUdpSocket_T socket));

/**
 * @deprecated since version vxx.xx.xx, replaced by #CellularUdpSocket_Send
 * @todo replace vxx.xx.xx with next version
 *
 * @brief   The function to send data via cellular UDP socket.
 *
 * This function is blocking. In other words, after the return of this function
 * call, the packet to be sent has been transferred completely to the cellular
 * modem and the modem has acknowledged the transfer. Mind that is not guaranteed
 * that the modem has already sent the packet over
 * the network. The sending to the network happens asynchronously.
 *
 * @param [in]  socket
 * UDP socket handle
 *
 * @param [in]  ipAddress
 * The destination IP address
 *
 * @param [in]  port
 * The destination UDP port number
 *
 * @param [in]  buffer
 * A pointer to buffer containing the data to be sent
 *
 * @param [in]  length
 * The number of bytes in the packet to be sent
 *
 * @retval Status of the UDP send operation, RETCODE_OK on success
 * @retval RETCODE_INVALID_PARAM If socket id is greater than the maximum or there is null parameters
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null and Other return values depends on #MCU_UART_Send
 * @retval RETCODE_SEMAPHORE_ERROR If serial transfer is failed
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 */
BCDS_DEPRECATED(Retcode_T CellularSocketUdp_Send(CellularUdpSocket_T socket, const uint32_t ipAddress, uint16_t port,
        uint8_t *buffer, uint32_t length));

/**
 * @deprecated since version vxx.xx.xx, replaced by #CellularUdpSocket_ResolveIp
 * @todo replace vxx.xx.xx with next version
 *
 * @brief Resolves a domain name to an IP address via the network provided DNS server.
 *
 * @note  Ensure the modem is in a packet switched data context before calling this function.
 *
 * @param[in]   DomainName          The fully qualified domain name to resolve (e.g., google.com)
 * @param[in]   DomainNameLength    The length of the domain name in bytes
 * @param[out]  IpAddress           The resolved IP address if the return value was RETCODE_OK
 *
 * @retval RETCODE_NOT_SUPPORTED  Resolve Name is not supported for quectel
 * @retval RETCODE_OK on success
 * @retval RETCODE_INVALID_PARAM If there are null parameters
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null
 * @retval RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock
 * @retval RETCODE_FAILURE If the IP address length is not more than two chars
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED If modem responses aborted
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR If modem responses error
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER If modem responses other
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 */
BCDS_DEPRECATED(Retcode_T Cellular_ResolveName(uint8_t *DomainName, uint32_t DomainNameLength, uint32_t *IpAddress));

#endif /* BCDS_CELLULARUPDSOCKET_H */
/**@} */
