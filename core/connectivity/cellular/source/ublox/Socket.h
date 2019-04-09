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
 * @file
 *
 * @brief Provides functionality for control (open, close, etc.) and querying of a socket
 *
 * @details Enables the following actions for a socket:
 * * open socket
 * * close socket
 * * sending
 * * receiving
 * * connect
 * * disconnect
 * * notify data has been received
 * Also provides the following informative query possibilities:
 * * get peer
 * * get TCP status
 * * get status of the socket
 *
 * @see
 * Refs:
 * * Ref 2: u-blox-ATCommands_Manual_(UBX-13002752).pdf
 **/

#ifndef UBLOX_SOCKET_H
#define UBLOX_SOCKET_H

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_CellularUdpSocket.h"

#if UINT8_C(7) < CELLULAR_SOCKET_COUNT
#error "Ublox only supports 7 sockets maximum"
#endif

/**
 * @brief The socket handle used by the Cellular driver
 */
typedef void* CellularSocket_Handle_T;

/**
 * @brief The types of sockets we support
 */
enum CellularSocket_Type_E
{
    CELLULAR_SOCKET_TYPE_TCP = 6,
    CELLULAR_SOCKET_TYPE_UDP = 17,

    CELLULAR_SOCKET_TYPE_MAX
};
typedef enum CellularSocket_Type_E CellularSocket_Type_T;

/**
 * @brief TCP socket states.
 *
 * @see  Ref 2, p 529, 25.25 Socket control +USOCTL
 */
enum CellularSocket_TcpStatus_E
{
    CELLULAR_SOCKET_TCP_STATUS_INACTIVE = 0,
    CELLULAR_SOCKET_TCP_STATUS_LISTEN,
    CELLULAR_SOCKET_TCP_STATUS_SYN_SENT,
    CELLULAR_SOCKET_TCP_STATUS_SYN_RCVD,
    CELLULAR_SOCKET_TCP_STATUS_ESTABILISHED,
    CELLULAR_SOCKET_TCP_STATUS_FIN_WAIT_1,
    CELLULAR_SOCKET_TCP_STATUS_FIN_WAIT_2,
    CELLULAR_SOCKET_TCP_STATUS_CLOSE_WAIT,
    CELLULAR_SOCKET_TCP_STATUS_CLOSING,
    CELLULAR_SOCKET_TCP_STATUS_LAST_ACK,
    CELLULAR_SOCKET_TCP_STATUS_TIME_WAIT,

    CELLULAR_SOCKET_TCP_STATUS_MAX
};
typedef enum CellularSocket_TcpStatus_E CellularSocket_TcpStatus_T;

/**
 * @brief General socket states.
 */
enum CellularSocket_Status_E
{
    CELLULAR_SOCKET_STATUS_OPEN,
    CELLULAR_SOCKET_STATUS_CONNECTED,
    CELLULAR_SOCKET_STATUS_CLOSED
};
typedef enum CellularSocket_Status_E CellularSocket_Status_T;

/**
 * @brief Callback used when a socket status is updated e.g., the socket is closed or received data
 *
 * @warning If CELLULAR_SOCKET_STATUS_CLOSED is passed to this callback, the application may use the socket handle
 * inside the callback to clean up some resources, however the socket may not be valid AFTER the callback returns!
 * DO NOT USE THE SOCKET-HANDLE AFTER THE CELLULAR_SOCKET_STATUS_CLOSED-CALLBACK RETURNS!
 *
 * @param socket The socket handle which status has changed.
 * @param status The new status of the socket.
 */
typedef void (*CellularSocket_StatusCallback_T)(CellularSocket_Handle_T socket, CellularSocket_Status_T status);

/**
 * @brief Callback used when a socket status is updated e.g., the socket is closed or received data
 *
 * @param socket The socket handle which status has changed.
 * @param BytesAvailable The amount of bytes currently buffered by the modem. It is suggested to use this value
 * in CellularSocket_Receive to read the data.
 */
typedef void (*CellularSocket_DataAvailableCallback_T)(CellularSocket_Handle_T socket, uint32_t BytesAvailable);

/**
 * @brief Opens a socket on the cellular modem.
 *
 * @param[out] socket The socket handle that is filled in to refer to the newly opened socket
 * @param[in] SocketType The type of the socket which should be opened. Currently only CELLULAR_SOCKET_TYPE_TCP is supported.
 * @param[in] StatusCallback A callback which is called when the socket status changes.
 *
 * @retval RETCODE_OK The socket was opened
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_OUT_OF_RESOURCES The modem has no more resources to open a socket
 */
Retcode_T CellularSocket_Open(CellularUdpSocket_T *socket, CellularSocket_Type_T SocketType, CellularUdpSocket_Callback_T StatusCallback);

/**
 * @brief Closes a socket on the cellular modem.
 *
 * @param[in] socket The socket handle of the socket that is to be closed
 *
 * @retval RETCODE_OK The socket was closed
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_INVALID_PARAM The given socket handle is unknown
 */
Retcode_T CellularSocket_Close(CellularUdpSocket_T socket);

/**
 * @brief Sends data over via a socket.
 *
 * @param[in] socket The socket handle of the socket through which data is to be sent
 * @param[in] buffer The data to send
 * @param[in] BufferLength The length of the data that is to be sent
 *
 * @retval RETCODE_OK The data was sent
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_INVALID_PARAM The given socket handle is unknown
 * @retval RETCODE_INCONSITENT_STATE The socket is not open or connected
 */
Retcode_T CellularSocket_Send(CellularSocket_Handle_T socket, uint8_t *buffer, uint32_t BufferLength);

/**
 * @brief   The function to send data via modem UDP socket.
 *
 * This function is blocking. In other words, after the return of this function
 * call, the packet to be sent has been transferred completely to the modem and
 * the last has acknowledged the transfer.
 * Mind that is not guaranteed that the modem has already sent the packet over
 * the network. The sending to the network happens asynchronously.
 *
 * @param [in]  socket UDP socket handle
 * @param [in]  ip The destination IP address
 * @param [in]  port The destination UDP port number
 * @param [in]  buffer A pointer to buffer containing the data to be sent
 * @param [in]  length The number of bytes in the packet to be sent
 *
 * @return Status of the UDP send operation, RETCODE_OK on success or some
 * error code otherwise.
 */
Retcode_T CellularSocket_SendTo(CellularUdpSocket_T socket, uint32_t ip,
        uint16_t port, uint8_t *buffer, uint32_t length);

/**
 * @brief Receives data from a socket.
 *
 * @param[in] socket The socket handle of the socket from which data is to be received
 * @param[in] buffer The buffer where to store the received data
 * @param[in] BufferLength The length of the receive buffer
 * @param[out] BytesRead The amount of bytes read from the socket
 * @param[out] SenderIpAddress The IP address of the data sender. TCP sockets will not set this value. NULL may be passed here.
 * @param[out] SenderPort The port of the data sender. TCP sockets will not set this value. NULL may be passed here.
 *
 * @retval RETCODE_OK Data was received from the socket (BytesRead > 0)
 * @retval RETCODE_FAILURE There was no data to be read from the socket
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_INVALID_PARAM The given socket handle is unknown
 * @retval RETCODE_INCONSITENT_STATE The socket is not open or connected
 */
Retcode_T CellularSocket_Receive(CellularSocket_Handle_T socket, uint8_t *buffer, uint32_t BufferLength, uint32_t *BytesRead, uint32_t *SenderIpAddress, uint16_t *SenderPort);

/**
 * @brief Receives data from specified UDP socket.
 *
 * When some incoming data arrives, the GSM driver invokes the callback, which
 * is associated with the corresponding socket. The callback has been passed
 * as a parameter to the CellularUdpSocket_Open(). This receive function must be
 * invoked within the callback to get the incoming packet. The received data
 * must be consumed before the return of the callback. The buffer, which
 * contains the data, will be released after the return of the callback.
 *
 * @param [in]  SocketId UDP socket ID
 * @param [out] ip The source IP address
 * @param [out] port The source UDP port number
 * @param [out] buffer A pointer to buffer containing the incoming data
 * @param [out] length The number of received bytes
 *
 * @return Status of the UDP send operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 * @warning Should not be used outside socket callback function
 */
Retcode_T CellularSocket_ReceiveFrom(CellularUdpSocket_T SocketId, uint32_t *ip, uint16_t *port, uint8_t **buffer, uint32_t *length);

/**
 * @brief Retrieves the ip and port of the peer the given socket is connected to. If the socket is not connected,
 * zero (in decimal-dotted: "0.0.0.0") and port 0 is returned.
 *
 * @param socket
 * @param SocketStatus
 * @retval RETCODE_OK If the socket peer was successfully queried.
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_INVALID_PARAM One of the given parameters is invalid
 */
Retcode_T CellularSocket_GetPeer(CellularSocket_Handle_T socket, uint32_t* ip, uint16_t* port);

/**
 * @brief Retrieves the TCP status of a socket.
 *
 * @param[in] socket The socket handle of the socket of which to retrieve the status
 * @param[out] SocketStatus The socket status upon correct retrieval
 *
 * @retval RETCODE_OK The socket status was retrieved
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_INVALID_PARAM The given socket handle is unknown
 */
Retcode_T CellularSocket_GetTcpStatus(CellularSocket_Handle_T socket, CellularSocket_TcpStatus_T *SocketStatus);

/**
 * @brief Retrieves the status of a socket.
 *
 * @param[in] socket The socket handle of the socket of which to retrieve the status
 * @param[out] SocketStatus The socket status upon correct retrieval
 *
 * @retval RETCODE_OK The socket status was retrieved
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_INVALID_PARAM The given socket handle is unknown
 */
Retcode_T CellularSocket_GetStatus(CellularSocket_Handle_T socket, CellularSocket_Status_T *SocketStatus);

/**
 * @brief Connects a TCP socket to a remote host.
 *
 * @param[in] socket The socket handle of the socket which to connect
 * @param[in] IpAddress The IP address to connect to
 * @param[in] port The port to connect to
 *
 * @retval RETCODE_OK The connection was established
 * @retval RETCODE_FAILURE The host was not reachable
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_INVALID_PARAM The given socket handle is unknown
 * @retval RETCODE_INCONSITENT_STATE The socket is not open or already connected
 */
Retcode_T CellularSocket_Connect(CellularSocket_Handle_T socket, uint32_t IpAddress, uint16_t port);

/**
 * @brief Disconnects a TCP socket from a remote host
 *
 * @param[in] socket The socket handle of the socket which to disconnect
 *
 * @retval RETCODE_OK The socket was disconnected
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval RETCODE_INVALID_PARAM The given socket handle is unknown
 * @retval RETCODE_INCONSITENT_STATE The socket is not open or connected
 */
Retcode_T CellularSocket_Disconnect(CellularSocket_Handle_T socket);

/**
 * @brief Implements URC handling for UUSOCL (socket closed unexpectedly) events.
 *        This function never blocks longer than required to handle the URC.
 *        If no UUSOCL URC is available, this function returns immediately.
 *
 * @retval RETCODE_OK URC handling was finished successfully.
 * @retval RETCODE_CELLULAR_URC_NOT_PRESENT If the URC this handler deals with was not present in the response queue
 */
Retcode_T CellularSocketUrc_UUSOCL(void);

/**
 * @brief Implements URC handling for UUSORD (read socket data) events.
 *        This function never blocks longer than required to handle the URC.
 *        If no UUSORD URC is available, this function returns immediately.
 *
 * @retval RETCODE_OK URC handling was finished successfully.
 * @retval RETCODE_CELLULAR_URC_NOT_PRESENT If the URC this handler deals with was not present in the response queue
 */
Retcode_T CellularSocketUrc_UUSORD(void);

/**
 * @brief Implements URC handling for UUSORF (read from socket data) events.
 *        This function never blocks longer than required to handle the URC.
 *        If no UUSORF URC is available, this function returns immediately.
 *
 * @retval RETCODE_OK URC handling was finished successfully.
 * @retval RETCODE_CELLULAR_URC_NOT_PRESENT If the URC this handler deals with was not present in the response queue
 */
Retcode_T CellularSocketUrc_UUSORF(void);

#endif /* UBLOX_SOCKET_H */
