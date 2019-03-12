from Configuration import *
from CChannel.Message import *
from CChannel.CCUsb import *
import abstract_test_case
from CChannel import Message
import time

class TC_8_GetErrorAt_DiffScenario(abstract_test_case.AbstractTestCase):
    test_case_id = 8
    usb_retry_count = 2
    usb_reboot_time = 10

    def setUp(self):

        ack = self.send_setup_command(EMBEDDED_TEST_PARTICIPANT, TIMEOUT)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")

    def runTest(self):
        print("==============================")
        print("Run: TC_8_GetErrorAt_DiffScenario ")
        print("==============================")

        ack = self.send_run_command(EMBEDDED_TEST_PARTICIPANT, 60)
        self.assertEqual(NO_ERROR, ack.getErrorCode(), "we don't expect any error code")

        self.wait_for_usb_reboot(EMBEDDED_TEST_PARTICIPANT, self.usb_retry_count, self.usb_reboot_time)

        result = self.wait_for_reports([EMBEDDED_TEST_PARTICIPANT], 5*20)

        report1 = self.get_last_report(EMBEDDED_TEST_PARTICIPANT)

        self.assertEqual(True, result, "the waitForReports has timed out...\n")

        self.assertNotEqual(None, report1, "report shouldn't be empty")
        self.assertEqual(Message.NO_ERROR, report1.getErrorCode(), "OK, this is the end of the test case")


    def tearDown(self):
        ack = self.send_teardown_command(EMBEDDED_TEST_PARTICIPANT, 10)
        self.assertEqual(Message.NO_ERROR, ack.getErrorCode(), "")
