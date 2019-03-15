from Configuration import *
from CChannel.CCUart import *
from CChannel.Message import *
from CChannel.TlvTypes import *

import unittest
import logging
import time

class TestCase_LoadSuiteTearDown(unittest.TestCase):

    testCaseId = 15

    def __init__(self, testSection, testSuite):
        super(TestCase_LoadSuiteTearDown, self).__init__()
        self.testSection = testSection
        self.testSuite = testSuite
        self.logger = logging.getLogger(__name__)

    def setUp(self):
         pass

    def runTest(self):
        pass

    def tearDown(self):
        print("===========================================")
        print("TearDown: TestCase_LoadSuiteTearDown ")
        print("============================================")

        self.TP1 = self.testSection.getTestParticipantProxy("TP1_COSP")

        self.logger.debug("[TC] TP1 " + str(self.TP1.getName()))

        message = Message(msgType=COMMAND, msgToken=self.testSection.getMsgToken(),
                          type=TestSuiteTeardown,
                          testSection=self.testSection.getSectionId(),
                          testSuite=self.testSuite.getSuiteId(),
                          testCase=self.testCaseId)

        ack = self.TP1.sendAndWaitForAck(message, TIMEOUT)
        self.assertEqual(NO_ERROR, ack.getErrorCode(), "we don't expect any error code")

