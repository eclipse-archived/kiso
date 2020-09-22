import pykiso
import logging

from pykiso.auxiliaries import dut

from pykiso import message


@pykiso.define_test_parameters(entry_id=1, suite_id=1, aux_list=[dut])
class SuiteSetup(pykiso.BasicTestSuiteSetup):
    pass


@pykiso.define_test_parameters(entry_id=1, suite_id=1, aux_list=[dut])
class SuiteTearDown(pykiso.BasicTestSuiteTeardown):
    pass


@pykiso.define_test_parameters(entry_id=1, suite_id=1, case_id=1, aux_list=[dut])
class MyTest(pykiso.BasicTest):
    def setUp(self):
        logging.info("### SETUP")

        msg = message.Message(
            msg_type=message.MessageType.COMMAND,
            sub_type=message.MessageCommandType.TEST_CASE_SETUP,
            test_entry=1,
            test_suite=1,
            test_case=1,
        )
        msg_sent = dut.send_message(msg)
        self.assertTrue(msg_sent)

        ack = dut.receive_message()
        self.assertEqual(message.MessageType.ACK, ack.msg_type)
        self.assertEqual(message.MessageAckType.ACK, ack.sub_type)

        super().setUp()

    def test_run(self):
        logging.info("### RUN")

        msg = message.Message(
            msg_type=message.MessageType.COMMAND,
            sub_type=message.MessageCommandType.TEST_CASE_RUN,
            test_entry=1,
            test_suite=1,
            test_case=1,
        )
        msg_sent = dut.send_message(msg)
        self.assertTrue(msg_sent)

        ack = dut.receive_message()
        self.assertEqual(message.MessageType.ACK, ack.msg_type)
        self.assertEqual(message.MessageAckType.ACK, ack.sub_type)

        logging.info("### Executing test case on hardware...")

        super().test_run()

    def tearDown(self):
        logging.info("### TEARDOWN")

        msg = message.Message(
            msg_type=message.MessageType.COMMAND,
            sub_type=message.MessageCommandType.TEST_CASE_TEARDOWN,
            test_entry=1,
            test_suite=1,
            test_case=1,
        )
        msg_sent = dut.send_message(msg)
        self.assertTrue(msg_sent)

        ack = dut.receive_message()
        self.assertEqual(message.MessageType.ACK, ack.msg_type)
        self.assertEqual(message.MessageAckType.ACK, ack.sub_type)

        super().tearDown()
