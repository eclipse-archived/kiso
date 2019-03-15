import unittest
from SmokeTest.TestCases_Smoke.TestCase_OnOff import *
from SmokeTest.TestCases_Smoke.TestCase_OnReset import *
from SmokeTest.TestCases_Smoke.TestCase_GPRSConnect import *
from SmokeTest.TestCases_Smoke.TestCase_UdpSocketSend import *
from SmokeTest.TestCases_Smoke.TestCase_SmsFunctionality import *
from SmokeTest.TestCases_Smoke.TestCase_SendReceiveExtendedAscii import *

import time

class TestSuite_Smoke(unittest.TestSuite):

    suiteId = 9

    def __init__(self,TestSection):
        super(TestSuite_Smoke, self).__init__()
        TestCases = []
        TestCases.append(TestCase_OnOff)
        TestCases.append(TestCase_OnReset)
        TestCases.append(TestCase_GPRSConnect)
        TestCases.append(TestCase_UdpSocketSend)
        TestCases.append(TestCase_SmsFunctionality)
        TestCases.append(TestCase_SendReceiveExtendedAscii)

        for testcase in TestCases:
            self.addTest(testcase(TestSection, self))

    def getSuiteId(self):
        return self.suiteId
