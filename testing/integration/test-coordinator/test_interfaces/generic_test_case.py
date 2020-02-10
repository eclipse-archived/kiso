"""
Generic Test
**************************
 
:module: generic_test_case
 
:synopsis: Base for all tests-cases
 
.. currentmodule:: generic_test_case
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0

.. note:: TODO later on will inherit from a metaclass to get the id parameters
"""

import logging
import unittest
import timeout_decorator

# Import based on the caller
import sys, os
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "cchannel"))
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "test_auxiliaries"))

# Import the framework modules
import message

class GenericTest(object):
    class BasicTest(unittest.TestCase):
        """ Base for test-cases """
        def __init__(self, test_auxiliary_list, test_section_id, test_suite_id, test_case_id, args, kwargs):
            """ Initialize generic test-case """
            # Initialize base class
            super(GenericTest.BasicTest, self).__init__(*args, **kwargs)
            # Save list of test auxiliaries to use (already initialize)
            self.test_auxiliary_list = test_auxiliary_list
            # Save test information
            self.test_section_id = test_section_id
            self.test_suite_id = test_suite_id
            self.test_case_id = test_case_id

        def cleanup_and_skip(self, info_to_print):
            """ Cleanup what was created in the Test-case 
            
            :param info_to_print: A message you want to print while cleaning up the test
            :type info_to_print: str
            """
            # Send aborts to all auxiliaries
            for aux in self.test_auxiliary_list:
                aux.abort_command()
            # Unlock all auxiliaries
            for aux in self.test_auxiliary_list:
                aux.unlock_it()
            # Log error message
            logging.fatal(info_to_print)
            # Skip test
            self.skipTest(info_to_print)

        def evaluate_message(self, aux, reported_message):
            """ Evaluate the report content

            :param aux: Auxiliary that sent the answer (Just for log perpose)
            :type aux: test_auxiliary_interface.AuxiliaryInterface()

            :param reported_message: received message to evaluate
            :type reported_message: message.Message()

            :return: True - Report received (does not mean it was successfull) / False - Report not received 
            """
            return_value = False
            if reported_message is not None:
                # Verify message content
                if reported_message.get_message_type() == message.MessageType.LOG:
                    # It is a log message
                    logging.info(reported_message)
                elif reported_message.get_message_type() == message.MessageType.REPORT:
                    # If the report is a fail, log and abort the test
                    if reported_message.get_message_sub_type() == message.MessageReportType.TEST_FAILED:
                        self.cleanup_and_skip("Test running on {} failed with {}".format(aux, reported_message))
                    # If success, return 
                    return_value = True
                else:
                    # Unexpected message received
                    logging.info("Unexpected message received: {}".format(reported_message))
            return return_value

        def setUp(self):
            logging.info("--------------- SETUP: {}, {}, {} ---------------".format(self.test_section_id, self.test_suite_id, self.test_case_id))
            # Prepare message to send
            testcase_setup_message = message.Message(msg_type=message.MessageType.COMMAND, sub_type=message.MessageCommandType.TEST_CASE_SETUP,
                                                        test_section=self.test_section_id, test_suite=self.test_suite_id, test_case=self.test_case_id)
            # Lock all auxiliaries
            for aux in self.test_auxiliary_list:
                locked = aux.lock_it(1)
                if locked is False:
                    self.cleanup_and_skip("{} could not be locked!".format(aux))
            # Send the message to all auxiliaries
            for aux in self.test_auxiliary_list:
                if aux.run_command(testcase_setup_message, blocking=True, timeout_in_s=10) is not True:
                    self.cleanup_and_skip("{} could not be setup!".format(aux))
        
        #@timeout_decorator.timeout(5) # Timeout of 10 second as generic test-case # TODO: Only working on linux
        def test_run(self):
            logging.info("--------------- RUN: {}, {}, {} ---------------".format(self.test_section_id, self.test_suite_id, self.test_case_id))
            # Prepare message to send
            testcase_run_message = message.Message(msg_type=message.MessageType.COMMAND, sub_type=message.MessageCommandType.TEST_CASE_RUN,
                                                        test_section=self.test_section_id, test_suite=self.test_suite_id, test_case=self.test_case_id)
            # Send test start through all auxiliaries
            for aux in self.test_auxiliary_list:
                if aux.run_command(testcase_run_message, blocking=True, timeout_in_s=10) is not True:
                    self.cleanup_and_skip("{} could not be run!".format(aux))
            # Loop until all messages are recieved
            list_of_aux_with_received_reports = [False]*len(self.test_auxiliary_list)
            while False in list_of_aux_with_received_reports:
                # Loop through all auxiliaries
                for i, aux in enumerate(self.test_auxiliary_list):
                    if list_of_aux_with_received_reports[i] == False:
                        # Wait for a report
                        reported_message = aux.wait_and_get_report()
                        # Check the received message
                        list_of_aux_with_received_reports[i] = self.evaluate_message(aux, reported_message)

        def tearDown(self):
            logging.info("--------------- TEARDOWN: {}, {}, {} ---------------".format(self.test_section_id, self.test_suite_id, self.test_case_id))
            # Prepare message to send
            testcase_setup_message = message.Message(msg_type=message.MessageType.COMMAND, sub_type=message.MessageCommandType.TEST_CASE_TEARDOWN,
                                                        test_section=self.test_section_id, test_suite=self.test_suite_id, test_case=self.test_case_id)
            # Send the message to all auxiliaries
            for aux in self.test_auxiliary_list:
                aux.run_command(testcase_setup_message, blocking=True, timeout_in_s=1)
            # Unlock all auxiliaries
            for aux in self.test_auxiliary_list:
                aux.unlock_it()

            # Do this if the test unittest is running
            if __name__ == '__main__':
                for aux in self.test_auxiliary_list:
                    aux.stop() # So that we  exit. This will be done in the tearDown of the test-section

def define_test_case_parameters(test_auxiliary_list, test_section_id, test_suite_id, test_case_id):
    """ Decorator """
    def generate_modified_class(Cls):
        """ Generates the same class but with a sligthly different init
        We want to fill the initializiation parameters with the above parameters
        """
        class NewClass(Cls):
            def __init__(self, *args, **kwargs):
                super(NewClass, self).__init__(test_auxiliary_list, test_section_id, test_suite_id, test_case_id, args, kwargs)
        return NewClass
    return generate_modified_class

##################################### TEST SECTION ##########################################

if __name__ == '__main__':
    import example_test_auxiliary
    import cc_example
    import threading

    # Initialize logging"
    logging.basicConfig(
        format='%(asctime)s [%(levelname)s] %(module)s: %(message)s', datefmt='%m/%d/%Y %H:%M:%S',
        level=logging.INFO,
        stream=sys.stderr)

    # Create a fake environment
    c1 = cc_example.CCExample()
    c2 = cc_example.CCExample()

    aux1 = example_test_auxiliary.ExampleAuxiliary("aux1", c1)
    aux2 = example_test_auxiliary.ExampleAuxiliary("aux2", c2)

    aux1.create_instance()
    aux2.create_instance()

    aux_list = [aux1, aux2]

    # Create fake generic unittest
    @define_test_case_parameters(aux_list, 1, 2, 3)
    class MyTest(GenericTest.BasicTest):
        pass

    # Start integration test
    unittest.main()