from Configuration import *

import abstract_test_case
from CChannel import Message

from TestSuite_I2C.test_duration import test_durations


class TestCase_1_a(abstract_test_case.AbstractTestCase):
    test_case_id = 1

    def setUp(self):
        print("==============================")
        print("SetUp: TestCase_I2C ")
        print("==============================")
        ack = self.send_setup_command(EMBEDDED_TEST_PARTICIPANT, TIMEOUT)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")

        ack = self.send_setup_command(JAVA_TEST_PARTICIPANT, TIMEOUT)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")

        ack = self.send_setup_command(BEAGLEBONE_TEST_PARTICIPANT, TIMEOUT)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")

    def runTest(self):
        print("==============================")
        print("Run: TestCase_I2C ")
        print("==============================")

        test_duration = test_durations.get_next_test_duration()
        tlvs = [(TLV_TYPE_TEST_DURATION, str(test_duration))]

        ack = self.send_run_command(EMBEDDED_TEST_PARTICIPANT, TIMEOUT, tlvs)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")
        ack = self.send_run_command(JAVA_TEST_PARTICIPANT, TIMEOUT, tlvs)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")
        ack = self.send_run_command(BEAGLEBONE_TEST_PARTICIPANT, TIMEOUT, tlvs)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")

        result = self.wait_for_reports([EMBEDDED_TEST_PARTICIPANT, JAVA_TEST_PARTICIPANT, BEAGLEBONE_TEST_PARTICIPANT],
                                       5 * test_duration)

        report1 = self.get_last_report(EMBEDDED_TEST_PARTICIPANT)
        report2 = self.get_last_report(JAVA_TEST_PARTICIPANT)
        report3 = self.get_last_report(BEAGLEBONE_TEST_PARTICIPANT)

        self.assertEqual(True, result, "the waitForReports has timed out...\n"
                         + "max time duration: " + str(test_duration)
                         + "\ntime duration 1:   " + str(test_duration) + str(report1)
                         + "\ntime duration 2:   " + str(test_duration) + str(report2)
                         + "\ntime duration 3:   " + str(test_duration) + str(report3))

        self.assertNotEqual(None, report1, "report shouldn't be empty")
        self.assertEqual(Message.NO_ERROR, report1.getErrorCode(), "OK, this is the end of the test case")

        self.assertNotEqual(None, report2, "report shouldn't be empty")
        self.assertEqual(Message.NO_ERROR, report2.getErrorCode(), "OK, this is the end of the test case")

        self.assertNotEqual(None, report3, "report shouldn't be empty")
        self.assertEqual(Message.NO_ERROR, report3.getErrorCode(), "OK, this is the end of the test case")

    def tearDown(self):
        print("==============================")
        print("TearDown: TestCase_I2C ")
        print("==============================")
        ack = self.send_teardown_command(EMBEDDED_TEST_PARTICIPANT, TIMEOUT)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")
        ack = self.send_teardown_command(JAVA_TEST_PARTICIPANT, TIMEOUT)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")
        ack = self.send_teardown_command(BEAGLEBONE_TEST_PARTICIPANT, TIMEOUT)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")
