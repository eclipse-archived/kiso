"""
Integration Test Framework
**************************
 
:module: integration_test
 
:synopsis: Entry point to the integration test framework
 
.. currentmodule:: integration_test
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0
 
"""

import logging
import argparse
import json

import os, sys
sys.path.append(os.path.dirname(__file__))
sys.path.append(os.path.join(os.path.dirname(__file__), "test-coordinator"))
import test_factory_and_execution


def initialize_logging(logfile):
    """ Initialize the logging """
    logging.basicConfig(
        format='%(asctime)s [%(levelname)s] %(module)s: %(message)s', datefmt='%m/%d/%Y %H:%M:%S',
        level=logging.INFO,
        filename=logfile)

class CmdLineParser(object):
    """ Parse the command line.
    """
    def __init__(self):
        # Initialize the parser
        self.parser = argparse.ArgumentParser(description='Integration Test Entry Point')
        # Add all needed arguments for the parser
        self.parser.add_argument("-c", "--test-configuration-file", required=True, help="Path and file of the integration test configuration:'*.ini' format.")
        self.parser.add_argument("-b", "--binary", default='.', required=False, help="File of the compiled test binary. If you do not have them, please refer to the step 1.")
        self.parser.add_argument("-l", "--logging", default='integ_test.log', required=False, help="Logging file name.")

    def get_cmd_line_arguments(self):
        return vars(self.parser.parse_args())

class CfgParser(object):
    """ Parse a given ini file
    """
    def __init__(self, filename):
        with open(filename) as json_cfg_file:
            self.configuration = json.load(json_cfg_file)
    
    def get_configuration(self):
        return self.configuration

if __name__ == "__main__":
    # Get cmd line arguments
    cmd_line_dict = CmdLineParser().get_cmd_line_arguments()
    # Get ini configuration
    cfg_dict = CfgParser(cmd_line_dict["test_configuration_file"]).get_configuration()
    # Set the logging
    initialize_logging(cmd_line_dict["logging"])
    # Run tests
    logging.info("cfg_dict: {}".format(cfg_dict))
    test_factory_and_execution.run(cfg_dict)
    



