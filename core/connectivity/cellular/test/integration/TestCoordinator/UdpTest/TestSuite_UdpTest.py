import unittest
from UdpTest.TestCases_Udp.TestCase_SocketTestSuiteSetup import *
from UdpTest.TestCases_Udp.TestCase_SocketTestSuiteTeardown import *
from UdpTest.TestCases_Udp.TestCase_SocketReceive import *
from UdpTest.TestCases_Udp.TestCase_SocketReceiveAfter4MinDelay import *
from UdpTest.TestCases_Udp.TestCase_SocketReceiveAfter30SecDelay import *
from UdpTest.TestCases_Udp.TestCase_SocketSend100KTime import *
from UdpTest.TestCases_Udp.TestCase_SendReceive100KTime import *
from UdpTest.TestCases_Udp.TestCase_ThreeSocketsOpenSend1KTime import *
from UdpTest.TestCases_Udp.TestCase_SendExceedDataLength import *

class TestSuite_UdpTest(unittest.TestSuite):

    suiteId = 8

    def __init__(self,TestSection):
        super(TestSuite_UdpTest, self).__init__()
        TestCases = []
        
        TestCases.append(TestCase_SocketTestSuiteSetup)
        TestCases.append(TestCase_SocketReceive)
        TestCases.append(TestCase_SocketReceiveAfter4MinDelay)
        TestCases.append(TestCase_SocketReceiveAfter30SecDelay)
        TestCases.append(TestCase_SocketSend100KTime)
        TestCases.append(TestCase_SendReceive100KTime)
        #TestCases.append(TestCase_ThreeSocketsOpenSend1KTime)
        TestCases.append(TestCase_SendExceedDataLength)
        TestCases.append(TestCase_SocketTestSuiteTeardown)

        for testcase in TestCases:
            self.addTest(testcase(TestSection, self))

    def getSuiteId(self):
        return self.suiteId
