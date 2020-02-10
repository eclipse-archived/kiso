"""
Test Entry
********************************
 
:module: test_factory_and_execution
 
:synopsis: Create based on the configuration the tests environment and execute the tests in it.
 
.. currentmodule:: test_factory_and_execution
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0

.. warning:: Still under test

.. note::
    1. Create based on a dictionary (test config entry), a list of auxiliaries
    2. Instantiate them
    3. Glob a list of test-suite folders
    4. Generate a list of test-suites with a list of test-cases
    5. Loop per suite
    6. Gather result
"""

# Import python needed modules
import logging
import unittest

# Import all integration test modules
import cchannel
import test_auxiliaries
import test_interfaces.generic_test_suite


def create_auxiliary(aux_dict):
    # Create channel
    cc = cchannel.cchannel_dict[aux_dict["channel_type"]](**aux_dict["channel_cfg"])
    # Create aux with channel
    return test_auxiliaries.auxiliary_dict[aux_dict["auxiliary_type"]](aux_dict["auxiliary_name"], channel=cc, **aux_dict["auxiliary_cfg"])

def create_test_suite(test_suite_dict, aux_list):
    return test_interfaces.generic_test_suite.GenericTestSuite.BasicTestSuite(modules_to_add_dir=test_suite_dict["suite_dir"], test_filter_pattern=test_suite_dict["test_filter_pattern"], test_auxiliary_list=aux_list, test_section_id=0, test_suite_id=test_suite_dict["test_suite_id"], args=[], kwargs={})


def run(test_entry_configuration):
    try:
        # Create the list of auxiliaries
        list_of_auxiliaries = []
        for auxiliary_configuration in test_entry_configuration["auxiliary_list"]:
            list_of_auxiliaries.append(create_auxiliary(auxiliary_configuration))
        # Create the list of test-suites
        list_of_test_suites = []
        for test_suite_configuration in test_entry_configuration["test_suite_list"]:
            list_of_test_suites.append(create_test_suite(test_suite_configuration, list_of_auxiliaries))
        # Instantiate the auxiliaries
        for aux in list_of_auxiliaries:
            aux.create_instance()
        # Start the test-suites & publish result
        for suite in list_of_test_suites:
            unittest.TextTestRunner().run(suite)
    except Exception as e:
        logging.error(e)

    # De-instantiate the auxiliaries & shut them off
    for aux in list_of_auxiliaries:
        aux.delete_instance()
        aux.stop()

if __name__ == '__main__':
    import sys
    # Initialize logging"
    logging.basicConfig(
        format='%(asctime)s [%(levelname)s] %(module)s: %(message)s', datefmt='%m/%d/%Y %H:%M:%S',
        level=logging.INFO,
        stream=sys.stderr)

    # Construct manualy a test_entry_configuration
    test_entry_configuration = {
        "auxiliary_list":[
            {
                "channel_type":"cc_example",
                "channel_cfg":{},
                "auxiliary_name":"aux1",
                "auxiliary_type":"example_test_auxiliary",
                "auxiliary_cfg":{}
            },
            {
                "channel_type":"cc_example",
                "channel_cfg":{},
                "auxiliary_name":"aux2",
                "auxiliary_type":"example_test_auxiliary",
                "auxiliary_cfg":{}
            }
        ],
        "test_suite_list":[
            {
                "suite_dir":'testing/integration/test-coordinator/test_interfaces',
                "test_filter_pattern":'*purpose.py',
                "test_suite_id":1
            }
        ]
    }
    # Run the test entry
    run(test_entry_configuration)

