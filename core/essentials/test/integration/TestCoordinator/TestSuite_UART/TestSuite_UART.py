import abstract_test_suite
from CChannel import Message

from TestSuite_UART.TestCase_3_a import *



class TestSuite_UART(abstract_test_suite.AbstractTestSuite):

    def setUp(self):
        # adding the test cases
        test_duration = test_durations.get_next_test_duration()
        tlvs = [(TLV_TYPE_TEST_DURATION, str(test_duration))]

        generic_test_case_dict = {'test_case_name': 'Test Case UART', 'test_case_id': 1,
                                  'timeouts': [TIMEOUT, TIMEOUT, 5*test_duration, TIMEOUT], 'tlvs': [None, tlvs, None]}

        self.add_generic_test_case(**generic_test_case_dict)
        # you can use the same dict to add other test case by changing the field that you want as follow:
        # generic_test_case_dict['test_case_name'] = 'Test case b'
        # generic_test_case_dict['test_case_id'] = 2
        # generic_test_case_dict['some field'] = xxx
        # ...
        # self.add_generic_test_case(**generic_test_case_dict)
        #
        # You can also not use a dictionnary and pass the argument directly to the method as follow:
        # self.add_generic_test_case('Test Case a', 1, [TIMEOUT, TIMEOUT, 5*test_duration, TIMEOUT], [None, tlvs, None],
        #                            ['Test Participant A', 'Test Participant B'])

        # self.add_test_case(TestCase_1_b)
        # self.add_test_case(TestCase_1_c)

        # sending setup command to the test participants
        self.generic_setUp()

    def tearDown(self):
        self.generic_tearDown()