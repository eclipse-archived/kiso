import unittest
from StressTest.TestCases_StressTest.TestCase_TurnOnOffCellular import *
from StressTest.TestCases_StressTest.TestCase_TurnOnResetCellular import *
from StressTest.TestCases_StressTest.TestCase_ConfigParaWithMaxLength import *
from StressTest.TestCases_StressTest.TestCase_NetworkInfoFromModem import *
from StressTest.TestCases_StressTest.TestCase_CellularNetworkConnect import *
from StressTest.TestCases_StressTest.TestCase_CellularNetworkDisconnect import *
from StressTest.TestCases_StressTest.TestCase_SocketOpen import *
from StressTest.TestCases_StressTest.TestCase_ConfigBeforeInitEvent import *
from StressTest.TestCases_StressTest.TestCase_SocketOpenBeforeConnEvent import *
from StressTest.TestCases_StressTest.TestCase_SocketOpenBeforeDisconnEvent import *
from StressTest.TestCases_StressTest.TestCase_SMSSendContinuously import *
from StressTest.TestCases_StressTest.TestCase_SmsSendThenSocketOpen import *
from StressTest.TestCases_StressTest.TestCase_SmsSendUdpSend import *

class TestSuite_StressTest(unittest.TestSuite):

    suiteId = 7

    def __init__(self,TestSection):
        super(TestSuite_StressTest, self).__init__()
        TestCases = []
        TestCases.append(TestCase_TurnOnOffCellular)
        TestCases.append(TestCase_TurnOnResetCellular)
        TestCases.append(TestCase_ConfigParaWithMaxLength)
        TestCases.append(TestCase_NetworkInfoFromModem)
        TestCases.append(TestCase_CellularNetworkConnect)
        TestCases.append(TestCase_CellularNetworkDisconnect)
        TestCases.append(TestCase_SocketOpen)
        TestCases.append(TestCase_ConfigBeforeInitEvent)
        TestCases.append(TestCase_SocketOpenBeforeConnEvent)
        TestCases.append(TestCase_SocketOpenBeforeDisconnEvent)
        TestCases.append(TestCase_SMSSendContinuously)
        TestCases.append(TestCase_SmsSendThenSocketOpen)
        TestCases.append(TestCase_SmsSendUdpSend)


        for testcase in TestCases:
            self.addTest(testcase(TestSection, self))

    def getSuiteId(self):
        return self.suiteId
