"""
Auxiliary Interface
**************************
 
:module: test_auxiliary_interface
 
:synopsis: Entry point to the integration test framework
 
.. currentmodule:: integration_test
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0

.. warning:: Abort may not work, was not tested yet
 
"""
import threading, queue
import logging

# Ensure lock and queue umique reference: needed because python will do a copy of the created object when the unittests are called
list_of_thread_ressources =[]

class AuxiliaryInterface(threading.Thread):
    """ Interfaces for all defining the interaction between the test-coordinator and the test auxiliaries
    """

    def __init__(self, name="None", channel=None):
        """ Auxiliary thread initialization

        :param name: Name of the auxiliary
        :type name: string

        :param channel: Channel to use to communicate with the associated entity
        :type channel: cchannel.CChannel()
        """
        # Initialize thread class
        super(AuxiliaryInterface, self).__init__()
        # Save the name
        self.name = name
        # Define thread control attributes & methods
        global list_of_thread_ressources
        self.rc_index = len(list_of_thread_ressources)
        list_of_thread_ressources.append({
            'lock':threading.RLock(),
            'queue_in':queue.Queue(),
            'queue_out':queue.Queue(),
            'stop_event':threading.Event()
        })
        # Save the communication channel
        self.channel = channel
        # Create state
        self.is_instance = False
        # Start thread
        self.start()

    def lock_it(self, timeout_in_s):
        """ Lock to ensure exclusivity

        :param timeout_in_s: How many second you want to wait for the lock
        :type timeout_in_s: integer

        :return: True - Lock done / False - Lock failed
        """
        global list_of_thread_ressources
        return list_of_thread_ressources[self.rc_index]['lock'].acquire(timeout=timeout_in_s)

    def unlock_it(self):
        """ Unlock exclusivity
        """
        global list_of_thread_ressources
        list_of_thread_ressources[self.rc_index]['lock'].release()

    def create_instance(self):
        """ Create an auxiliary instance and ensure the communication to it
        
        :return: message.Message() - Contain received message
        """
        global list_of_thread_ressources
        if list_of_thread_ressources[self.rc_index]['lock'].acquire():
            # Trigger the internal requests
            list_of_thread_ressources[self.rc_index]['queue_in'].put("create_auxiliary_instance")
            # Wait until the request was processed
            report = list_of_thread_ressources[self.rc_index]['queue_out'].get() # TODO add try & except?
            # Release the above lock
            list_of_thread_ressources[self.rc_index]['lock'].release()
            # Return the report
            return report
    
    def delete_instance(self):
        """ Delete an auxiliary instance and its communication to it
        
        :return: message.Message() - Contain received message
        """
        global list_of_thread_ressources
        if list_of_thread_ressources[self.rc_index]['lock'].acquire():
            # Trigger the internal requests
            list_of_thread_ressources[self.rc_index]['queue_in'].put("delete_auxiliary_instance")
            # Wait until the request was processed TODO add try & except?
            report = list_of_thread_ressources[self.rc_index]['queue_out'].get()
            # Release the above lock
            list_of_thread_ressources[self.rc_index]['lock'].release()
            # Return the report
            return report

    def run_command(self, command_message, blocking=True, timeout_in_s=0):
        """ Send a test request

        :param command_message: command request to the auxiliary
        :type command_message: message.Message

        :param extended_payload: data you would like to populate the command with
        :type extended_payload: binary_string

        :param blocking: If you want the command request to be blocking or not
        :type blocking: boolean

        :param timeout_in_s: Number of time (in s) you want to wait for an answer
        :type timeout_in_s: integer

        :return: True - Successfully sent / False - Failed by sending / None
        """
        global list_of_thread_ressources
        return_code = False
        logging.info("Command called {} in {}".format(command_message, self))
        if list_of_thread_ressources[self.rc_index]['lock'].acquire():
            
            # Trigger the internal requests
            list_of_thread_ressources[self.rc_index]['queue_in'].put(("command", command_message))
            # Wait until the test request was received
            try:
                return_code = list_of_thread_ressources[self.rc_index]['queue_out'].get(blocking, timeout_in_s)
            except queue.Empty:
                pass
            # Release the above lock
            list_of_thread_ressources[self.rc_index]['lock'].release()
            # Return the ack_report if exists
        return return_code
    
    def wait_and_get_report(self, blocking=False, timeout_in_s=0):
        """ Wait for the report of the previous sent test request

        :return: a message.Message() - Message received / None - nothing received
        """
        global list_of_thread_ressources
        try:
            return list_of_thread_ressources[self.rc_index]['queue_out'].get(blocking, timeout_in_s)
        except queue.Empty:
            pass

    def abort_command(self):
        """ Force test to abort

        :return: True - Abort was a success / False - if not # TODO maybe use the signal lib for the abort?
        """
        global list_of_thread_ressources
        report = None
        logging.info("abort waited at {}".format(self))
        if list_of_thread_ressources[self.rc_index]['lock'].acquire():
            # Abort what is currently running in the task
            report = self._abort_command()
            # Release the above lock
            list_of_thread_ressources[self.rc_index]['lock'].release()
            # Return the report
        return report

    def stop(self):
        """ Force the thread to stop himselve
        """
        list_of_thread_ressources[self.rc_index]['stop_event'].set()
        

        
    def run(self):
        """ Run function """
        global list_of_thread_ressources
        logging.info(list_of_thread_ressources)
        while not list_of_thread_ressources[self.rc_index]['stop_event'].is_set() :
            # Step 1: Check if a request is available & process it
            request = ""
            # Check if a request was recieved
            if not list_of_thread_ressources[self.rc_index]['queue_in'].empty():
                request = list_of_thread_ressources[self.rc_index]['queue_in'].get_nowait()
            # Process the request
            if request == "create_auxiliary_instance" and self.is_instance == False :
                # Call the internal instance creation method
                return_code = self._create_auxiliary_instance()
                # Based on the result set status:
                self.is_instance = return_code
                # Enqueue the result for the request caller
                list_of_thread_ressources[self.rc_index]['queue_out'].put(return_code)
            elif request == "delete_auxiliary_instance" and self.is_instance == True:
                # Call the internal instance delete method
                return_code = self._delete_auxiliary_instance()
                # Based on the result set status:
                self.is_instance = not return_code
                # Enqueue the result for the request caller
                list_of_thread_ressources[self.rc_index]['queue_out'].put(return_code)
            elif type(request) == tuple and self.is_instance == True:
                # If the instance is created, send the requested command to the instance via the internal method
                list_of_thread_ressources[self.rc_index]['queue_out'].put(self._run_command(*request)) # Remapping from the parameters is done in the function
            elif request != "":
                # A request was received but could not be processed
                logging.warning("Oups, something went wrong, {} will not be processed!".format(request))
                logging.warning("Aux status: {}".format(self.__dict__))

            # Step 2: Check if something was received from the aux instance if instance was created
            if self.is_instance == True:
                received_message = self._receive_message(timeout_in_s=0)
                # If yes, send it via the out queue
                if received_message is not None:
                    list_of_thread_ressources[self.rc_index]['queue_out'].put(received_message)
        # Thread stop command was set
        logging.info("{} was stopped".format(self))
        # Delete thread ressources and list entry:
        del list_of_thread_ressources[self.rc_index]['lock']
        del list_of_thread_ressources[self.rc_index]['queue_in']
        del list_of_thread_ressources[self.rc_index]['queue_out']
        del list_of_thread_ressources[self.rc_index]['stop_event']
        # Delete auxiliary external instance if not done
        if self.is_instance == True:
            self._delete_auxiliary_instance()


    def _create_auxiliary_instance(self):
        """ Create the auxiliary instance with witch we will communicate

        :return: True - Successfully created / False - Failed by creation

        .. note: Errors should be logged via the logging with the right level
        """
        raise NotImplementedError

    def _delete_auxiliary_instance(self):
        """ Delete the auxiliary instance with witch we will communicate

        :return: True - Successfully deleted / False - Failed deleting

        .. note: Errors should be logged via the logging with the right level
        """
        raise NotImplementedError

    def _run_command(self, unused_parameter, command_message):
        """ Run a command for the auxiliary

        :param unused_parameter: Not to be used
        :type unused_parameter: None

        :param command_message: command in form of a message to run
        :type command_message: message.Message

        :return: True - Successfully received bv the instance / False - Failed sending

        .. note: Errors should be logged via the logging with the right level
        """
        raise NotImplementedError

    def _abort_command(self):
        """ Abort the sent command
        """
        raise NotImplementedError

    def _receive_message(self, timeout_in_s):
        """ Defines what needs to be done as a receive message. Such as, what do I need to do to receive a message

        :param timeout_in_s: How much time to block on the receive
        :type timeout_in_s: integer

        :return: message.Message - If one received / None - Else
        """
        raise NotImplementedError



