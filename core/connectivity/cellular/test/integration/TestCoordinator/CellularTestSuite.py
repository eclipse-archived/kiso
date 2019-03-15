"""
:module: TestSuite_Cellular
:platform: Windows
:synopsis: Cellular Test Suite implementation

:Copyright: (C) Bosch Connected Devices and Solutions. All Rights Reserved. Confidential.
"""

import abstract_test_suite
from CChannel import Message
from CChannel.TlvTypes import *
from TestSuites import *
from Configuration import *

class TestSuite_PowerControl(abstract_test_suite.AbstractTestSuite):
    def setUp(self):
        self.add_generic_test_case('RunPowerControl', RunPowerControlId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunPowerOnOffDly15STestCase', RunPowerOnOffDly15STestCaseId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunPowerOnOffDly60STestCase', RunPowerOnOffDly60STestCaseId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunPowerOnResetTestCase', RunPowerOnResetTestCaseId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.generic_setUp(20)

    def tearDown(self):
        self.generic_tearDown()


class TestSuite_Socket(abstract_test_suite.AbstractTestSuite):
    def setUp(self):
        self.add_generic_test_case('RunSocketTurnOnConnect', RunSocketTurnOnConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunSocketOpenCloseTestCase', RunSocketOpenCloseTestCaseId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunSocket3timesOpenClose', RunSocket3timesOpenCloseId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunSocketOpenDly260SecClose', RunSocketOpenDly260SecCloseId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunSocketUdpSend', RunSocketUdpSendId, [1, 1, TIMEOUT, 1], [None, [(23, UDP_SEND_DATA), (24, UDP_SEND_DATA_LENGTH)], None])
#        self.add_generic_test_case('RunSocketUdp3TimesDataSend', RunSocketUdp3TimesDataSendId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunSocketOpenDelayUdpSend', RunSocketOpenDelayUdpSendId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunSocketUdpSend5Times', RunSocketUdpSend5TimesId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunSocketOpenUdpSendDlyClose', RunSocketOpenUdpSendDlyCloseId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunSocketOpenUdpSendCloseDly', RunSocketOpenUdpSendCloseDlyId, [1, 1, TIMEOUT, 1], [None, None, None])        
        self.add_generic_test_case('RunSocketDisconnectTurnOff', RunSocketDisconnectTurnOffId, [1, 1, TIMEOUT, 1], [None, None, None])     
        self.generic_setUp(20)

    def tearDown(self):
        self.generic_tearDown()


class TestSuite_Network(abstract_test_suite.AbstractTestSuite):
    def setUp(self):
        self.add_generic_test_case('RunNetworkTurnOn', RunNetworkTurnOnId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNetworkInitTestCase', RunNetworkInitTestCaseId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNetworkInitTwice', RunNetworkInitTwiceId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNetworkGPRSConnect', RunNetworkGPRSConnectId, [1, 1, TIMEOUT, 1], [None, [(TestCaseRun, CONFIG_APN), (TestCaseRun, CONFIG_USERNAME), (TestCaseRun,CONFIG_PASSWORD)], None])
#        self.add_generic_test_case('RunNewtworkInit120SecDly', RunNewtworkInit120SecDlyId, [1, 1, TIMEOUT, 1], [None, [(TestCaseRun, CONFIG_APN), (TestCaseRun, CONFIG_USERNAME), (TestCaseRun,CONFIG_PASSWORD)], None])
#        self.add_generic_test_case('RunNewtworkConfig120SecDly', RunNewtworkConfig120SecDlyId, [1, 1, TIMEOUT, 1], [None, [(TestCaseRun, CONFIG_APN), (TestCaseRun, CONFIG_USERNAME), (TestCaseRun,CONFIG_PASSWORD)], None])
#        self.add_generic_test_case('RunNewtworkConnect120SecDly', RunNewtworkConnect120SecDlyId, [1, 1, TIMEOUT, 1], [None, [(TestCaseRun, CONFIG_APN), (TestCaseRun, CONFIG_USERNAME), (TestCaseRun,CONFIG_PASSWORD)], None])
#        self.add_generic_test_case('RunNetworkSequence5times', RunNetworkSequence5timesId, [1, 1, TIMEOUT, 1], [None, [(TestCaseRun, CONFIG_APN), (TestCaseRun, CONFIG_USERNAME), (TestCaseRun,CONFIG_PASSWORD)], None])
        self.add_generic_test_case('RunNetworkTurnOff', RunNetworkTurnOffId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.generic_setUp(20)

    def tearDown(self):
        self.generic_tearDown()


class TestSuite_Query(abstract_test_suite.AbstractTestSuite):
    def setUp(self):
        self.add_generic_test_case('RunQueryTurnOn', RunQueryTurnOnId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryNetworkInformationFromModem', RunQueryNetworkInformationFromModemId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryErrorInformationFromModem', RunQueryErrorInformationFromModemId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryCellularStatusInitialized', RunQueryCellularStatusInitializedId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryCellularStatusConnected', RunQueryCellularStatusConnectedId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryCellularStatusDisconnected', RunQueryCellularStatusDisconnectedId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryNetworkInit', RunQueryNetworkInitId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryNetworkConfigConnect', RunQueryNetworkConfigConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryDisconnect', RunQueryDisconnectId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunQueryTurnOff', RunQueryTurnOffId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.generic_setUp(20)

    def tearDown(self):
        self.generic_tearDown()


class TestSuite_Negative(abstract_test_suite.AbstractTestSuite):
    def setUp(self):
        self.add_generic_test_case('RunNegativeTurnOn', RunNegativeTurnOnId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeConnectInvalidAPN', RunNegativeConnectInvalidAPNId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeConnectAPNLengthExceed', RunNegativeConnectAPNLengthExceedId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeConnectInvalidUserName', RunNegativeConnectInvalidUserNameId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeConnectUserNameExceed', RunNegativeConnectUserNameExceedId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeConnectInvalidPassword', RunNegativeConnectInvalidPasswordId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeConnectPasswordLengthExceed', RunNegativeConnectPasswordLengthExceedId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeConnectNullCellularConfig', RunNegativeConnectNullCellularConfigId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeConnectCellularWithoutConfig', RunNegativeConnectCellularWithoutConfigId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeTurnOff', RunNegativeTurnOffId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeTurnOn', RunNegativeTurnOnId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeDisConnectCellularWithoutConnect', RunNegativeDisConnectCellularWithoutConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeConnectCellularTwice', RunNegativeConnectCellularTwiceId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeDisconnectCellularTwice', RunNegativeDisconnectCellularTwiceId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeOffWithoutDisconnect', RunNegativeOffWithoutDisconnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeTurnOn', RunNegativeTurnOnId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeNetworkInit', RunNegativeNetworkInitId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeNetworkConfigConnect', RunNegativeNetworkConfigConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeSocket7TimesOpen', RunNegativeSocket7TimesOpenId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeSocketOperationWithoutClose', RunNegativeSocketOperationWithoutCloseId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeSendWrongSocketID', RunNegativeSendWrongSocketIDId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSendWrongSocketIP', RunNegativeSendWrongSocketIPId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeDisconnect', RunNegativeDisconnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeOpenWithoutNwConnect', RunNegativeOpenWithoutNwConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeCloseWithouNwConnect', RunNegativeCloseWithouNwConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSendwithoutNwConnect', RunNegativeSendwithoutNwConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeNetworkConfigConnect', RunNegativeNetworkConfigConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSocketSendWithoutopen', RunNegativeSocketSendWithoutopenId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeInvalidConfigAfterConnect', RunNegativeInvalidConfigAfterConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeNetworkInit', RunNegativeNetworkInitId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeInvalidConfigAfterConnectTryDisconnect', RunNegativeInvalidConfigAfterConnectTryDisconnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeInvalidConfigAfterDisconnect', RunNegativeInvalidConfigAfterDisconnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeNetworkConfigConnect', RunNegativeNetworkConfigConnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSendDataAfterSocketClose', RunNegativeSendDataAfterSocketCloseId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeNetworkInitWhenConnected', RunNegativeNetworkInitWhenConnectedId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeConnectNetworkAfterDisconnect', RunNegativeConnectNetworkAfterDisconnectId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSmsSendTextWithExceedMobileNum', RunNegativeSmsSendTextWithExceedMobileNumId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSmsSendWithExccedDataLength', RunNegativeSmsSendWithExccedDataLengthId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSmsSendWithExccedDataAndMobileLength', RunNegativeSmsSendWithExccedDataAndMobileLengthId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSmsSendWithInvalidMobileNum', RunNegativeSmsSendWithInvalidMobileNumId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSmsSendWithInvalidServiceNumber', RunNegativeSmsSendWithInvalidServiceNumberId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSmsInitTwice', RunNegativeSmsInitTwiceId, [1, 1, TIMEOUT, 1], [None, None, None])
#        self.add_generic_test_case('RunNegativeSmsSendWithZeroLength', RunNegativeSmsSendWithZeroLengthId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunNegativeTurnOff', RunNegativeTurnOffId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.generic_setUp(20)

    def tearDown(self):
        self.generic_tearDown()


class TestSuite_MultiTask(abstract_test_suite.AbstractTestSuite):
    def setUp(self):
        self.add_generic_test_case('RunMultiTaskTurnOn', RunMultiTaskTurnOnId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunMultiTaskTest', RunMultiTaskTestId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.add_generic_test_case('RunMultiTaskTurnOff', RunMultiTaskTurnOffId, [1, 1, TIMEOUT, 1], [None, None, None])
        self.generic_setUp(20)

    def tearDown(self):
        self.generic_tearDown()
