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
 * @ingroup KISO_CELLULAR
 * @defgroup KISO_CELLULAR_SOCKETS Sockets
 * @{
 * @brief Cellular Socket Data Service API.
 *
 * @details If the Cellular modem offers an internal TCP/IP stack, this data
 * service will map a Berkeley-like socket interface to this stack.
 * In case the selected Cellular variant does not provide an internal IP-stack,
 * this API may return a #Retcode_T indicating #RETCODE_NOT_SUPPORTED.
 *
 * The service offers a POSIX-like socket interface with the significant
 * difference that the recv()/recvfrom() are non-blocking. Instead a callback
 * mechanism is used to notify the user about the reception of bytes.
 *
 * @par Regarding socket creation:
 * Generally speaking the idea behind the data-context parameter is the user may
 * want to create sockets on different data-contexts. A data-context controls
 * how (packet-)data is routed through the cellular network and what quality of
 * service (QoS) is associated with it. One could imagine having two or more
 * data-contexts, one with high QoS for VoIP and a second - lower QoS - context
 * for SMS or TCP/IP communication.
 *
 * As some modems implement their own TCP/IP-stack on the modem two types of
 * data-context are to be known: INTERNAL and EXTERNAL. A INTERNAL data-context
 * may only be used with the modem internal TCP/IP-stack. While EXTERNAL
 * data-contexts require the modem integrator (i.e. us) to come up with their
 * own TCP/IP-stack. Therefore the driver clearly differentiates between the
 * two.
 *
 * The socket-service depends on the modem internal TCP/IP-stack, so if one
 * tries to create a socket with an EXTERNAL data-context, this operation is
 * expected to fail (depending on variant implementation).
 *
 * @par u-blox internal TCP/IP-stack:
 * u-blox modems generally offer a integrated TCP/IP-stack. The way they
 * organize their extended services is by profiles. Note that there is a
 * difference between @e PSD-profile (Packet-Switched-Data) and @e data-context,
 * meaning @e profile-id and @e cid refer to different things. When wanting to
 * create a socket on a u-blox modem one generally has to activate a PSD-profile
 * which internally is linked to a specific data-context (cid). Some variants
 * allow the user to switch this PSD-profile-to-cid mapping via:
 * @code
 *  AT+UPSD=<profile_id>,100,<cid>
 * @endcode
 * By default all PSD-profiles are mapped to the same cid (cid #8). If one wants
 * to configure the APN, QoS, etc. for the internal TCP/IP-stack, u-blox
 * recommends to perform the configuration through @e +UPSD and @b not via the
 * @e +CGDCONT commands (which would be the 3GPP standardized way of managing
 * data-contexts). Note however that LTE is inherently packet-based so the modem
 * has to activate a data-context during the network registration procedure (then
 * called default-EPS-bearer). And as the data-context configuration cannot be
 * changed while the context is active, settings like APN, QoS, etc. have to be
 * configured before joining the network. It is currently unknown if the LTE
 * default-EPS-bearer setup can be performed through @e +UPSD, because
 * SARA-R4/SARA-N4 behave somewhat different compared to the bulk of u-blox
 * products. SARA-R4/SARA-N4 modems only support a single INTERNAL data-context
 * (i.e. PSD-profile) but do @b not allow to change the mapping of which. This
 * PSD-profile is hard-mapped to the default-EPS-bearer (in LTE networks at
 * least). Additionally @e +UPSD can not be used to configure a APN or QoS, so
 * the way to go is through @e +CGDCONT. This also means that internal sockets
 * must use the APN of the default-EPS-bearer. It appears though that a user may
 * create additional EXTERNAL data-contexts (with differing APNs) if he can
 * provide his own TCP/IP-stack.
 *
 * @file
 */
#ifndef KISO_CELLULARSOCKETSERVICE_H_
#define KISO_CELLULARSOCKETSERVICE_H_

#include "Kiso_Cellular.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

#define CELLULAR_SOCKET_PORT_RANDOM (UINT16_C(0))

typedef uint32_t CellularSocket_Handle_T;

enum CellularSocket_Protocol_E
{
    CELLULAR_SOCKET_PROTOCOL_TCP,
    CELLULAR_SOCKET_PROTOCOL_UDP
};
typedef enum CellularSocket_Protocol_E CellularSocket_Protocol_T;

/**
 * @brief Callback to notify the client about data arriving at the socket.
 *
 * @note This callback is only called if the socket buffer was previously empty
 * and new data arrived. Please call #CellularSocket_ReceiveFrom or
 * #CellularSocket_Receive and check the @e bytesReceived parameter after each
 * call. Call these receive APIs continuously until @e bytesReceived is less
 * than the amount requested in the receive-call.
 *
 * @note In case of UDP sockets this callback will repeat for each new datagram
 * that becomes the front-buffer of the #CellularSocket_Receive()/
 * #CellularSocket_ReceiveFrom() APIs.
 * E.g.:
 * - datagram 'A' is received
 * - callback is called with bytes available for 'A'
 * - datagram 'B' is received
 * - no callback, because 'A' is still in the front-buffer
 * - datagram 'A' is read completely
 * - callback is called with bytes available for 'B'
 *
 * @param[in] socket
 * Handle of the local socket that just received data.
 *
 * @param[in] numBytesAvailable
 * Number of bytes available to read on the socket. In case of UDP sockets
 * this is the number of bytes in the current packet.
 */
typedef void (*CellularSocket_NotifyDataReady_T)(
    CellularSocket_Handle_T socket,
    uint32_t numBytesAvailable);

/**
 * @brief Callback to notify the client about a new incoming connection. The
 * connection is automatically accepted and the newly associated socket is
 * published by the @e acceptedSocket parameter. This socket can afterwards be
 * used to communicate with the remote endpoint.
 *
 * @param[in] acceptedSocket
 * Handle of the accepted socket.
 *
 * @param[in] remoteIp
 * IP of the remote endpoint. Note that this parameter may only be valid for the
 * execution time of the callback.
 *
 * @param[in] remotePort
 * Port of the remote endpoint.
 *
 * @param[in] listeningSocket
 * Handle of the socket that accept the incoming connection.
 */
typedef void (*CellularSocket_NotifyConnectionAccepted_T)(
    CellularSocket_Handle_T acceptedSocket,
    const Cellular_IpAddress_T *remoteIp,
    uint16_t remotePort,
    CellularSocket_Handle_T listeningSocket);

/**
 * @brief Callback to notify the client about the closure the given socket
 * caused by @b external influence (i.e. not directly initiated by client).
 *
 * @param[in] closedSocket
 */
typedef void (*CellularSocket_NotifySocketClosed_T)(
    CellularSocket_Handle_T closedSocket);

/**
 * @brief Create a new socket with the given protocol and bind it to a address
 * inherited by the data-context.
 *
 * This is the equivalent to POSIX @e socket() and @e bind(). Afterwards - and
 * depending on the protocol assigned to the socket - calls to
 * #CellularSocket_Listen, #CellularSocket_Connect or #CellularSocket_SendTo
 * may be performed.
 *
 * @note The maximum amount of sockets that can be created may depend on the
 * Cellular IP-stack in use. Check the Cellular config-header for details.
 *
 * @param[out] socket
 * Will be set to the initialized socket handle after completion.
 *
 * @param[in] dataContext
 * Data-context on which to create the socket instance. This will impact
 * packet-routing through an APN.
 *
 * @param[in] localPort
 * Optional local port to bind the socket to. Must be set to
 * #CELLULAR_SOCKET_PORT_RANDOM if the stack should choose a free port at
 * random, @b or the socket is going to be used as a listener socket.
 *
 * @param[in] protocol
 * Protocol to associate with this socket.
 *
 * @param[in] onSocketClosed
 * Callback to notify about the closing of the socket.
 *
 * @param[in] onDataReady
 * Callback to notify about data available on the socket.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_CreateAndBind(
    CellularSocket_Handle_T *socket,
    const Cellular_DataContext_T *dataContext,
    uint16_t localPort,
    CellularSocket_Protocol_T protocol,
    CellularSocket_NotifySocketClosed_T onSocketClosed,
    CellularSocket_NotifyDataReady_T onDataReady);

/**
 * @brief Establishes a connection between the specified socket and the remote
 * host. For TCP sockets this will automatically perform the TCP negotiation and
 * open a connection. For UDP sockets this will simply associate the remote host
 * with this socket and prepare it for sending. Calls to #CellularSocket_SendTo
 * are still possible though.
 * A optional local port can also be associated with the connection. If
 * @e localPort is set to 0, the stack will chose a port at random.
 *
 * This is the equivalent to POSIX @e connect() with the slight difference that
 * a local port can be assigned.
 *
 * @param[in] socket
 * Handle of the socket to connect.
 *
 * @param[in] remoteIp
 * IP of the remote host to connect to/associate the socket with.
 *
 * @param[in] remotePort
 * Port of the remote host to connect to/associate the socket with.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_Connect(
    CellularSocket_Handle_T socket,
    const Cellular_IpAddress_T *remoteIp,
    uint16_t remotePort);

/**
 * @brief Mark this socket as a passive socket, i.e. waiting for incoming
 * connections. Incoming TCP connections are automatically accepted and
 * forwarded onto a new socket as published by the callback. The socket
 * callbacks for close and data-ready events are copied over from the listener
 * socket and automatically associated with the new socket.
 *
 * For UDP sockets this does nothing except bind the socket to the given port.
 *
 * The listener socket may no longer be usable as sender/receiver socket.
 *
 * This is the equivalent to POSIX @e listen() with the difference that incoming
 * connections are automatically and asynchronously accepted.
 *
 * @param[in] socket
 * Socket to set to listening mode.
 *
 * @param[in] localPort
 * Local port to listen on for incoming connections.
 *
 * @param[in] onConnectionAccept
 * Callback to be called once incoming connection is established. The connection
 * is automatically accepted before invocation of the callback.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_Listen(
    CellularSocket_Handle_T socket,
    uint16_t localPort,
    CellularSocket_NotifyConnectionAccepted_T onConnectionAccept);

/**
 * @brief Sends data to a remove host on an already connected socket.
 *
 * This is the equivalent to POSIX @e send().
 *
 * @note #CellularSocket_Connect is required before calling this. Use
 * #CellularSocket_SendTo for UDP connections that have no fixed remote host
 * associated.
 *
 * @param[in] socket
 * Handle of the socket to send on.
 *
 * @param[in] data
 * Data to send over the socket.
 *
 * @param[in] dataLength
 * Length of data.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_Send(
    CellularSocket_Handle_T socket,
    const uint8_t *data,
    uint32_t dataLength);

/**
 * @brief Sends data to a remote host given by parameter. Only for UDP sockets!
 *
 * This is the equivalent to POSIX @e sendto().
 *
 * @param[in] socket
 * Handle of the socket to send on.
 *
 * @param[in] data
 * Data to send over the socket.
 *
 * @param[in] dataLength
 * Length of data.
 *
 * @param[in] remoteIp
 * Remote IP of the host to send to.
 *
 * @param[in] remotePort
 * Remote port of the host to send to.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_SendTo(
    CellularSocket_Handle_T socket,
    const uint8_t *data,
    uint32_t dataLength,
    const Cellular_IpAddress_T *remoteIp,
    uint16_t remotePort);

/**
 * @brief Receive incoming data on the given socket.
 *
 * In the event of incoming data, the data-service invokes the @e onDataReady
 * callback, which is associated with the corresponding socket. The callback
 * has been passed as a parameter to the #CellularSocket_CreateAndBind().
 * The data is buffered internally (or on the modem) for a limited amount of
 * time and should therefore be read out promptly.
 *
 * The caller should note the @e bytesReceived out-parameter and continue to
 * call #CellularSocket_ReceiveFrom() until @e bytesReceived is less than the
 * passed @e bufferLength to ensure the receive pipeline is flushed.
 *
 * This is the equivalent to POSIX @e recv().
 *
 * @param[in] socket
 * Handle of the socket to receive from.
 *
 * @param[out] buffer
 * Buffer to write the received data into.
 *
 * @param[in] bufferLength
 * Limit of bytes to be stored into data buffer.
 *
 * @param[out] bytesReceived
 * Amount of bytes actually written into buffer.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_Receive(
    CellularSocket_Handle_T socket,
    uint8_t *buffer,
    uint32_t bufferLength,
    uint32_t *bytesReceived);

/**
 * @brief Receive incoming data on the given UDP socket.
 *
 * In the event of incoming data, the data-service invokes the @e onDataReady
 * callback, which is associated with the corresponding socket. The callback
 * has been passed as a parameter to the #CellularSocket_CreateAndBind().
 * The data is buffered internally (or on the modem) for a limited amount of
 * time and should therefore be read out promptly.
 *
 * The caller should note the @e bytesReceived out-parameter and continue to
 * call #CellularSocket_ReceiveFrom() until @e bytesReceived is less than the
 * passed @e bufferLength to ensure the receive pipeline is flushed.
 *
 * This is the equivalent to POSIX @e recvfrom().
 *
 * @param[in] socket
 * Handle of the socket to receive from.
 *
 * @param[out] buffer
 * Buffer to write the received data into.
 *
 * @param[in] bufferLength
 * Limit of bytes to be stored into data buffer.
 *
 * @param[out] bytesReceived
 * Amount of bytes actually written into buffer.
 *
 * @param[out] remoteIp
 * Will be set to the source IP where the data came from. Relevant for UDP
 * sockets.
 *
 * @param[out] remotePort
 * Will be set to the source port where the data came from. Relevant for UDP
 * sockets.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_ReceiveFrom(
    CellularSocket_Handle_T socket,
    uint8_t *buffer,
    uint32_t bufferLength,
    uint32_t *bytesReceived,
    Cellular_IpAddress_T *remoteIp,
    uint16_t *remotePort);

/**
 * @brief Query the driver/modem how many bytes are currently buffered and ready
 * for read-out. For UDP sockets this will only query the number of bytes
 * currently held in the front-buffer.
 *
 * @param[in] socket
 * The handle of the socket to query.
 *
 * @param[out] numBytesAvailable
 * Will contain the current number of bytes available to read.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_QueryBytesAvailable(
    CellularSocket_Handle_T socket,
    uint32_t *numBytesAvailable);

/**
 * @brief Close the socket identified by the given Id.
 *
 * @param[in] socket
 * Handle of the socket to be closed.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularSocket_Close(
    CellularSocket_Handle_T socket);

#endif /* KISO_CELLULARSOCKETSERVICE_H_ */

/** @} */
