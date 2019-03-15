"""
:module: TestSection_Cellular
:platform: Windows
:synopsis: Cellular Test Section implementation

:Copyright: (C) Bosch Connected Devices and Solutions. All Rights Reserved. Confidential.
"""

from AbstractTestSection import *
from CChannel.CCUdp import *
from CChannel.CCUart import *
from CChannel import Message
from CChannel.TlvTypes import *
from TestSuites import *
from CellularTestSuite import *
import Configuration

class TestSection_Cellular(AbstractTestSection):

    #Constructor
    def __init__(self, testExecutor):
        super(TestSection_Cellular, self).__init__("TestSection_Cellular", testExecutor)
        self.sectionId = 1
        self.add_test_suite(TestSuite_PowerControl, BCDS_TEST_SUITE_POWERCONTROL_ID)
        self.add_test_suite(TestSuite_Socket, BCDS_TEST_SUITE_SOCKET_ID)
        self.add_test_suite(TestSuite_Network, BCDS_TEST_SUITE_NETWORK_ID)
        self.add_test_suite(TestSuite_Query, BCDS_TEST_SUITE_QUERY_ID)
#        self.add_test_suite(TestSuite_Negative, BCDS_TEST_SUITE_NEGATIVE_ID)
        self.add_test_suite(TestSuite_MultiTask, BCDS_TEST_SUITE_MULTITASK_ID)

        # Default test suite
        suites = []
        # Specific test suites
        if("UDP_TEST" in Configuration.TESTSUITE):
            suites = [TestSuite_UdpTest(self)]
        if("LOAD_STRESS_TEST" in Configuration.TESTSUITE):
            suites = [TestSuite_StressTest(self),TestSuite_Load(self)]
        if("SMOKE_TEST" in Configuration.TESTSUITE):
            suites = [TestSuite_Smoke(self)]

        for suite in suites:
            self.addTestSuite(suite)

    def setUp(self):
        print("Test participant COM_port: {}".format(Configuration.COM_PORT))
        self.addTestParticipantProxy("Cellular", CCUart(Configuration.COM_PORT))
        message = Message.Message(msgType=COMMAND, msgToken=self.getMsgToken(),
                                  type=TestSectionSetup, testSection=self.sectionId)

        self.TP1 = self.getTestParticipantProxy("Cellular")
        ack = self.TP1.sendAndWaitForAck(message, TIMEOUT)
        self.assertEqual(NO_ERROR, ack.getErrorCode(), "we don't expect any error code")

    def tearDown(self):
        print ("Section Tear down")
        message = Message.Message(msgType=COMMAND, msgToken=self.getMsgToken(),
                          type=TestSectionTeardown,
                          testSection=self.sectionId)

        ack = self.TP1.sendAndWaitForAck(message, TIMEOUT)
        self.assertEqual(NO_ERROR, ack.getErrorCode(), "we don't expect any error code")

    def getNextTestDuration(self):
        return self.testExecutor.getNextTestDuration()

    def getNext3TestDurations(self):
        return self.testExecutor.getNext3TestDurations()

    def getRandomTestDuration(self):
        return self.testExecutor.getRandomTestDuration()
