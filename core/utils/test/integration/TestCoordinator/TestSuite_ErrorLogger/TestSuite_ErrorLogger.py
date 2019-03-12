# Import the configuration
from Configuration import *
# Import the abstract test-suite
import abstract_test_suite
from CChannel import Message
from TestSuite_ErrorLogger.TC_8_GetErrorAt_DiffScenario import *

class TestSuite_ErrorLogger(abstract_test_suite.AbstractTestSuite):
    def setUp(self):
        # adding the test cases
        print("Test Suite ErrorLogger")
        generic_test_case_dict = {'test_case_name': 'Test Case ErrorLogger', 'test_case_id': 1,
                                  'timeouts': [TIMEOUT, TIMEOUT, 60, TIMEOUT], 'tlvs': [None, None, None]}

        generic_test_case_dict['test_case_name'] = 'TC_1_ErrorLoggingSingleError'
        generic_test_case_dict['test_case_id'] = 1
        self.add_generic_test_case(**generic_test_case_dict)

        generic_test_case_dict['test_case_name'] = 'TC_2_LoggingMultipleError_run'
        generic_test_case_dict['test_case_id'] = 2
        self.add_generic_test_case(**generic_test_case_dict)

        generic_test_case_dict['test_case_name'] = 'TC_3_ClearAllError_run'
        generic_test_case_dict['test_case_id'] = 3
        self.add_generic_test_case(**generic_test_case_dict)

        generic_test_case_dict['test_case_name'] = 'TC_4_GetLastError_run'
        generic_test_case_dict['test_case_id'] = 4
        self.add_generic_test_case(**generic_test_case_dict)

        generic_test_case_dict['test_case_name'] = 'TC_5_HasError_run'
        generic_test_case_dict['test_case_id'] = 5
        self.add_generic_test_case(**generic_test_case_dict)

        generic_test_case_dict['test_case_name'] = 'TC_6_CountErrors_run'
        generic_test_case_dict['test_case_id'] = 6
        self.add_generic_test_case(**generic_test_case_dict)

        generic_test_case_dict['test_case_name'] = 'TC_7_GetErrorAt_run'
        generic_test_case_dict['test_case_id'] = 7
        self.add_generic_test_case(**generic_test_case_dict)

        self.add_test_case(TC_8_GetErrorAt_DiffScenario)

        #sending setup command to the test participants
        self.generic_setUp(timeout=20)

    def tearDown(self):
        self.generic_tearDown(timeout=20)
