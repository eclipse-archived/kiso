"""
Generic Test Suite
********************************
 
:module: generic_test_suite
 
:synopsis: Create a generic test-suite based on the connected modules
 
.. currentmodule:: generic_test_suite
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0
"""

import logging
import unittest

# Import based on the caller
import sys, os
sys.path.append(os.path.dirname(__file__))
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "cchannel"))
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "test_auxiliaries"))

import message
import generic_test_case

class GenericTestSuite(object):
    class BaseTestSuite(unittest.TestCase):
        """ Base for the setup & teardown to not have ducplicated code """
        def __init__(self, test_auxiliary_list, test_section_id, test_suite_id, args, kwargs):
            """ Initialize generic test-case """
            # Initialize base class
            super(GenericTestSuite.BaseTestSuite, self).__init__(*args, **kwargs)
            # Save list of test auxiliaries to use (already initialize)
            self.test_auxiliary_list = test_auxiliary_list
            # Save test information
            self.test_section_id = test_section_id
            self.test_suite_id = test_suite_id
        
        def cleanup_and_skip(self, info_to_print):
            """ Cleanup
            :param info_to_print: A message you want to print while cleaning up the test
            :type info_to_print: str
            """
            # Send aborts to all auxiliaries
            for aux in self.test_auxiliary_list:
                aux.abort_command()
                aux.unlock_it()
            # Log error message
            logging.fatal(info_to_print)
            # Skip test
            self.skipTest(info_to_print)

        def base_function(self, step_name, test_command):
            logging.info("--------------- {}: {}, {} ---------------".format(step_name, self.test_section_id, self.test_suite_id))
            # Prepare message to send
            testcase_setup_message = message.Message(msg_type=message.MessageType.COMMAND, sub_type=test_command,
                                                        test_section=self.test_section_id, test_suite=self.test_suite_id, test_case=0)
            # Lock all auxiliaries
            for aux in self.test_auxiliary_list:
                locked = aux.lock_it(1)
                if locked is False:
                    self.cleanup_and_skip("{} could not be locked!".format(aux))
            # Send the message to all auxiliaries
            for aux in self.test_auxiliary_list:
                if aux.run_command(testcase_setup_message, blocking=True, timeout_in_s=10) is not True:
                    self.cleanup_and_skip("{} could not be setup!".format(aux))
            # Unlock all auxiliaries
            for aux in self.test_auxiliary_list:
                aux.unlock_it()

    class BasicTestSuiteSetup(BaseTestSuite):
        def __init__(self, test_auxiliary_list, test_section_id, test_suite_id, args, kwargs):
            super(GenericTestSuite.BasicTestSuiteSetup, self).__init__(test_auxiliary_list, test_section_id, test_suite_id, args, kwargs)

        def runTest(self):
            self.base_function("SUITE SETUP", message.MessageCommandType.TEST_SUITE_SETUP)

    class BasicTestSuiteTeardown(BaseTestSuite):
        def __init__(self, test_auxiliary_list, test_section_id, test_suite_id, args, kwargs):
            super(GenericTestSuite.BasicTestSuiteTeardown, self).__init__(test_auxiliary_list, test_section_id, test_suite_id, args, kwargs)

        def runTest(self):
            self.base_function("SUITE TEARDOWN", message.MessageCommandType.TEST_SUITE_TEARDOWN)
            # Do this if the test unittest is running
            if __name__ == '__main__':
                for aux in self.test_auxiliary_list:
                    aux.stop() # So that we  exit. This will be done in the tearDown of the test-section
    
    class BasicTestSuite(unittest.TestSuite):
        """ Inherit from the unittest framework test-suite but build it for our integration tests """
        def __init__(self, modules_to_add_dir, test_filter_pattern, test_auxiliary_list, test_section_id, test_suite_id, args, kwargs):
            """ Initialize our custom unittest-test-suite

            .. note:
                1. Will Load from the given path the integration test modules under test
                2. Will create the test suite
            """
            # Mother class initialization
            super(GenericTestSuite.BasicTestSuite, self).__init__(*args, **kwargs)
            # Add Setup
            self.addTest(GenericTestSuite.BasicTestSuiteSetup(test_auxiliary_list, test_section_id, test_suite_id, args, kwargs))
            # Add the tests
            loader = unittest.TestLoader()
            found_modules = loader.discover(modules_to_add_dir, pattern=test_filter_pattern)
            # Pass to the loaded tests the test_aux_list - Do not touch!
            for test_class_item in found_modules._tests[0]._tests:
                for test_item in test_class_item._tests:
                    test_item.test_auxiliary_list = test_auxiliary_list
            # Add the found modules into the test-suite
            self.addTests(found_modules)
            # Add Teardown
            self.addTest(GenericTestSuite.BasicTestSuiteTeardown(test_auxiliary_list, test_section_id, test_suite_id, args, kwargs))

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

    # Create fake generic test-suite
    class MySuite(GenericTestSuite.BasicTestSuite):
        def __init__(self, *args, **kwargs):
            super(MySuite, self).__init__(modules_to_add_dir='testing/integration/test-coordinator/test_interfaces', test_filter_pattern='*purpose.py', test_auxiliary_list=aux_list, test_section_id=1, test_suite_id=2, args=args, kwargs=kwargs)

    # Start integration test
    suite = MySuite()
    unittest.TextTestRunner().run(suite)
            