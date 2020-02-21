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
 * @ingroup KISO_CELLULAR_VARIANT_UBLOX
 * @file
 * @brief Internal header for managing the u-blox socket-service.
 */
#ifndef SOCKETSERVICE_H_
#define SOCKETSERVICE_H_

#include "AT_UBlox.h"

#include "Kiso_Basics.h"

/**
 * @brief Called by the URC handler to notify that data got received on a
 * connected socket.
 *
 * @param[in] socketId
 * Socket-Id of the socket which received data.
 *
 * @param[in] length
 * Number of available on the socket.
 */
void SocketService_NotifySocketDataReceived(uint32_t socketId, uint32_t length);

/**
 * @brief Called by the URC handler to notify that a listening socket accepted a
 * new incoming TCP connection. The u-blox TCP/IP stack already accepted the
 * connection and allocated a new socket for it.
 *
 * @param[in] createdSocketId
 * Socket-Id of the accepted TCP socket connection. Use this to communicate with
 * the connection partner.
 *
 * @param[in] remoteIp
 * Pointer to the IP of the remote host.
 *
 * @param[in] remotePort
 * Port of the remote host.
 *
 * @param[in] listeningSocketId
 * Socket-Id of the listening socket that was used to initiate the
 * communication. This is @b not the socket used to communicate with the newly
 * established connection partner!
 *
 * @param[in] localIp
 * Pointer to the local IP of the socket.
 *
 * @param[in] listeningPort
 * Port on which the listening socket was listening.
 */
void SocketService_NotifySocketAccepted(uint32_t createdSocketId,
                                        const AT_UBlox_Address_T *remoteIp,
                                        uint16_t remotePort,
                                        uint32_t listeningSocketId,
                                        const AT_UBlox_Address_T *localIp,
                                        uint16_t listeningPort);

/**
 * @brief Called by the URC handler to notify that a socket got closed by
 * external influence (i.e. loss of connection).
 *
 * @param[in] socketId
 * Socket-Id of the socket that got closed.
 */
void SocketService_NotifySocketClosed(uint32_t socketId);

#endif /* SOCKETSERVICE_H_ */
