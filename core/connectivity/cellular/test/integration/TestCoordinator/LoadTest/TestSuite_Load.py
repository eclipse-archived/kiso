import unittest
from LoadTest.TestCases_Load.TestCase_LoadSuiteSetUp import *
from LoadTest.TestCases_Load.TestCase_LoadSuiteTearDown import*

from LoadTest.TestCases_Load.TestCase_PowerOn import *
from LoadTest.TestCases_Load.TestCase_Reset import *
from LoadTest.TestCases_Load.TestCase_Init import *
from LoadTest.TestCases_Load.TestCase_ConnectDisconnect import *
from LoadTest.TestCases_Load.TestCase_Configure import *
from LoadTest.TestCases_Load.TestCase_LoadSocketOpenClose import *
from LoadTest.TestCases_Load.TestCase_OpenSendClose import *
from LoadTest.TestCases_Load.TestCase_OpenSendCloseSimul import *
from LoadTest.TestCases_Load.TestCase_OpenSendMultiClose import *
from LoadTest.TestCases_Load.TestCase_OpenSendThreeThenClose import *
from LoadTest.TestCases_Load.TestCase_SocketOperation import *
from LoadTest.TestCases_Load.TestCase_ThreeOpenClose import *
from LoadTest.TestCases_Load.TestCase_SendDataEvery20Sec import *
from LoadTest.TestCases_Load.TestCase_SmsAndSocketOperation import *
import time

class TestSuite_Load(unittest.TestSuite):

    suiteId = 6

    def __init__(self,TestSection):
        super(TestSuite_Load, self).__init__()
        TestCases = []

        TestCases.append(TestCase_LoadSuiteSetUp)
        TestCases.append(TestCase_PowerOn)
        TestCases.append(TestCase_Reset)
        TestCases.append(TestCase_Init)
        TestCases.append(TestCase_Configure)
        TestCases.append(TestCase_ConnectDisconnect)
        TestCases.append(TestCase_LoadSocketOpenClose)
        TestCases.append(TestCase_OpenSendClose)
        TestCases.append(TestCase_ThreeOpenClose)
        TestCases.append(TestCase_OpenSendMultiClose)
        TestCases.append(TestCase_OpenSendThreeThenClose)
        TestCases.append(TestCase_OpenSendCloseSimul)
        TestCases.append(TestCase_SocketOperation)
        TestCases.append(TestCase_SendDataEvery20Sec)
        TestCases.append(TestCase_SmsAndSocketOperation)
        TestCases.append(TestCase_LoadSuiteTearDown)

        for testcase in TestCases:
            self.addTest(testcase(TestSection, self))

    def getSuiteId(self):
        return self.suiteId
