"""
Communication Channel Via Udp
********************************
 
:module: cc_udp
 
:synopsis: Udp communication channel
 
.. currentmodule:: cc_udp
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0

.. warning: Still under test
"""

import socket
import cchannel_interface
import message

class CCUdp(cchannel_interface.CChannel):
    """ UDP implementation of the coordination channel """

    def __init__(self, destIp, destPort):
        # Initialize the super class
        super(CCUdp, self).__init__()
        # Initialize the socket
        self.destIp = destIp
        self.destPort = destPort
        self.udpsocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # Define the max length
        maxMsgSize = 256

    def _cc_open(self):
        pass

    def _cc_close(self):
        self.udpsocket.close()

    def _cc_send(self, msg):
        rawPacket = msg.serialize()
        self.udpsocket.sendto(bytearray(rawPacket), (self.destIp, self.destPort))

    def _cc_receive(self, timeout=0.0000001):
        self.udpsocket.settimeout(timeout)

        try:
                readRawPacket = self.udpsocket.recv(self.maxMsgSize)
        except:
                return None
        return message.Message().parsePacket(readRawPacket)
