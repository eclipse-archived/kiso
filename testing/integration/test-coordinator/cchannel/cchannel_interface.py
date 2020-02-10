"""
Communication Channel Interface
********************************
 
:module: cchannel_interface
 
:synopsis: Interface for a channel
 
.. currentmodule:: cchannel_interface
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0

"""
import threading
import logging

class CChannel(object):
    """ Abstract class for coordination channel # TODO: check decorator with with to ensure the close"""

    def __init__(self):
        self._lock = threading.RLock()

    def cc_open(self):
        """ Open a thread-safe channel """
        # If we successfully lock the channel, open it
        if (self._lock.acquire(blocking=False)):
            self._cc_open()
        else:
            raise ConnectionRefusedError
        
    def cc_close(self):
        """ Close a thread-safe channel """
        # Close channel and release lock
        self._cc_close()
        self._lock.release()

    def cc_send(self, message_to_send, timeout):
        """ Send a thread-safe message on the channel and wait for an acknowledgement
        
        :param message_to_send: message to send
        :type message_to_send: Message()

        :param timeout: time in second to wait for sending a message
        :type timeout: integer
        """
        if (self._lock.acquire(blocking=False)):
            self._cc_send(message_to_send)
        else:
            raise ConnectionRefusedError
        self._lock.release()

    def cc_receive(self, timeout):
        """ Read a thread-safe message on the channel and send an acknowledgement

        :param timeout: time in second to wait for reading a message
        :type timeout: integer
        
        :return: Message if successful, None else
        """
        received_message = None
        if (self._lock.acquire(blocking=False)):
            # Store received message
            received_message = self._cc_receive(timeout)
        else:
            raise ConnectionRefusedError
        self._lock.release()
        return received_message

    def _cc_open(self):
        """ Open the channel
        """
        raise NotImplementedError

    def _cc_close(self):
        """ Close the channel
        """
        raise NotImplementedError

    def _cc_send(self, message):
        """ Sends the message on the channel

        :param message: Message to send out
        :type message: message.Message()
        """
        raise NotImplementedError

    def _cc_receive(self, timeout):
        """ How to receive something from the channel

        :param timeout: Time to wait in second for a message to be received
        :type timeout: integer

        :return: message.Message() - If one received / None - If not
        """
        raise NotImplementedError