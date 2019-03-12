from Configuration import *
from CChannel.Message import *
from CChannel.TlvTypes import *
import unittest
import logging
import socket
import time


class UtilsTestCase(unittest.TestCase):
    def __init__(self, testSection, testSuite):
        super(UtilsTestCase, self).__init__()
        self.testSection = testSection
        self.testSuite = testSuite
        # TODO: add TP name in configuration file
        self.logger = logging.getLogger(__name__)

    def helper_setup(self):
        print("=====================================")
        print("SetUp: ", type(self).__name__, " ")
        print("=====================================")

        self.TP1 = self.testSection.getTestParticipantProxy("TestParticipant_1")
        self.logger.debug("[TC] TP1 " + str(self.TP1.getName()))

        message = Message(msgType=COMMAND, msgToken=self.testSection.getMsgToken(),
                          type=TestCaseSetup,
                          testSection=self.testSection.getSectionId(),
                          testSuite=self.testSuite.getSuiteId(),
                          testCase=self.testCaseId)

        ack = self.TP1.sendAndWaitForAck(message, TIMEOUT)
        self.assertEqual(NO_ERROR, ack.getErrorCode(), "we don't expect any error code")

    def helper_teardown(self):
        print("====================================")
        print("TearDown: ", type(self).__name__, " ")
        print("====================================")

        message = Message(msgType=COMMAND, msgToken=self.testSection.getMsgToken(),
                          type=TestCaseTeardown,
                          testSection=self.testSection.getSectionId(),
                          testSuite=self.testSuite.getSuiteId(),
                          testCase=self.testCaseId)

        ack = self.TP1.sendAndWaitForAck(message, TIMEOUT)
        self.assertEqual(NO_ERROR, ack.getErrorCode(), "we don't expect any error code")

    def helper_runtest(self):
        print("====================================")
        print("Run: ", type(self).__name__, " ")
        print("====================================")

        messageTP1 = Message(msgType=COMMAND, msgToken=self.testSection.getMsgToken(),
                             type=TestCaseRun,
                             testSection=self.testSection.getSectionId(),
                             testSuite=self.testSuite.getSuiteId(),
                             testCase=self.testCaseId)

        testDuration = self.testSection.getNextTestDuration()

        # messageTP1.addTLVElement(TLV_TYPE_TEST_DURATION, str(testDuration))

        self.logger.debug("[TC] sending command to TP1" + str(messageTP1.serialize()))

        ack = self.TP1.sendAndWaitForAck(messageTP1, TIMEOUT)
        self.assertEqual(NO_ERROR, ack.getErrorCode(), "we don't expect any error code")

        return testDuration
