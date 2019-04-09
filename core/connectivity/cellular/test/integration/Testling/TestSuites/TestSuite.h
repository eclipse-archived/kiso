/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

#ifndef CELLULAR_TESTSUITE_H
#define CELLULAR_TESTSUITE_H

#define BCDS_TEST_SUITE_POWERCONTROL_ID                     1
#define RunPowerControlId                                   1
#define RunPowerOnOffDly15STestCaseId                       2
#define RunPowerOnOffDly60STestCaseId                       3
#define RunPowerTwiceOnTestCaseId                           4
#define RunPowerTwiceOffTestCaseId                          5
#define RunPowerResetTestCaseId                             6
#define RunPowerOnResetTestCaseId                           7

#define BCDS_TEST_SUITE_SOCKET_ID                           2
#define RunSocketOpenCloseTestCaseId                        1
#define RunSocket3timesOpenCloseId                          2
#define RunSocketOpenDly260SecCloseId                       3
#define RunSocketUdpSendId                                  4
#define RunSocketUdp3TimesDataSendId                        5
#define RunSocketOpenDelayUdpSendId                         6
#define RunSocketUdpSend5TimesId                            7
#define RunSocketOpenUdpSendDlyCloseId                      8
#define RunSocketOpenUdpSendCloseDlyId                      9
#define RunSocketTurnOnConnectId                            10
#define RunSocketDisconnectTurnOffId                        11

#define BCDS_TEST_SUITE_NETWORK_ID                          3
#define RunNetworkInitTestCaseId                            1
#define RunNetworkInitTwiceId                               2
#define RunNetworkGPRSConnectId                             3
#define RunNewtworkInit120SecDlyId                          4
#define RunNewtworkConfig120SecDlyId                        5
#define RunNewtworkConnect120SecDlyId                       6
#define RunNetworkSequence5timesId                          7
#define RunNetworkTurnOnId                                  8
#define RunNetworkTurnOffId                                 9

#define BCDS_TEST_SUITE_QUERY_ID                            4
#define RunQueryNetworkInformationFromModemId               1
#define RunQueryErrorInformationFromModemId                 2
#define RunQueryCellularStatusInitializedId                 3
#define RunQueryCellularStatusConnectedId                   4
#define RunQueryCellularStatusDisconnectedId                5
#define RunQueryTurnOnId                                    6
#define RunQueryNetworkInitId                               7
#define RunQueryNetworkConfigConnectId                      8
#define RunQueryDisconnectId                                9
#define RunQueryTurnOffId                                   10

#define BCDS_TEST_SUITE_NEGATIVE_ID                         5
#define RunNegativeNetworkInitWithOutModemOnId              1
#define RunNegativeConnectInvalidAPNId                      2
#define RunNegativeConnectAPNLengthExceedId                 3
#define RunNegativeConnectInvalidUserNameId                 4
#define RunNegativeConnectUserNameExceedId                  5
#define RunNegativeConnectInvalidPasswordId                 6
#define RunNegativeConnectPasswordLengthExceedId            7
#define RunNegativeConnectNullCellularConfigId              8
#define RunNegativeConnectCellularWithoutConfigId           9
#define RunNegativeDisConnectCellularWithoutConnectId       10
#define RunNegativeConnectCellularTwiceId                   11
#define RunNegativeDisconnectCellularTwiceId                12
#define RunNegativeOffWithoutDisconnectId                   13
#define RunNegativeSocket7TimesOpenId                       14
#define RunNegativeSocketOperationWithoutCloseId            15
#define RunNegativeSendWrongSocketIDId                      16
#define RunNegativeSendWrongSocketIPId                      17
#define RunNegativeOpenWithoutNwConnectId                   18
#define RunNegativeCloseWithouNwConnectId                   19
#define RunNegativeSendwithoutNwConnectId                   20
#define RunNegativeSocketSendWithoutopenId                  21
#define RunNegativeInvalidConfigAfterConnectId              22
#define RunNegativeInvalidConfigAfterConnectTryDisconnectId 23
#define RunNegativeInvalidConfigAfterDisconnectId           24
#define RunNegativeSendDataAfterSocketCloseId               25
#define RunNegativeNetworkInitWhenConnectedId               26
#define RunNegativeConnectNetworkAfterDisconnectId          27
#define RunNegativeSmsSendTextWithExceedMobileNumId         28
#define RunNegativeSmsSendWithExccedDataLengthId            29
#define RunNegativeSmsSendWithExccedDataAndMobileLengthId   30
#define RunNegativeSmsSendWithInvalidMobileNumId            31
#define RunNegativeSmsSendWithInvalidServiceNumberId        32
#define RunNegativeSmsInitTwiceId                           33
#define RunNegativeSmsSendWithZeroLengthId                  34
#define RunNegativeTurnOnId                                 35
#define RunNegativeNetworkInitId                            36
#define RunNegativeNetworkConfigConnectId                   37
#define RunNegativeDisconnectId                             38
#define RunNegativeTurnOffId                                39

#define BCDS_TEST_SUITE_MULTITASK_ID                        6
#define RunMultiTaskTestId                                  1
#define RunMultiTaskTurnOnId                                2
#define RunMultiTaskTurnOffId                               3

#define BCDS_TEST_SUITE_UDP_ID                              7
#define RunUdpSocketReceiveId                               1
#define RunUdpSocketReceiveAfterFourMinDelayId              2
#define RunUdpSocketReceiveAfter30SecDelayId                3
#define RunUdpSocketSend100KTimesId                         4
#define RunUdpSendReceive100KTimesId                        5
#define RunUdp3SocketOpenSend1KTimesId                      6
#define RunUdpSendExceedDataLengthId                        7

#define BCDS_TEST_SUITE_LOAD_ID                             8
#define RunLoadPowerOnId                                    1
#define RunLoadResetId                                      2
#define RunLoadInitId                                       3
#define RunLoadConfigureId                                  4
#define RunLoadConnectDisconnectId                          5
#define RunLoadSocketOpenCloseTestCaseId                    6
#define RunLoadOpenSendCloseTestCaseId                      7
#define RunLoadThreeOpenCloseTestCaseId                     8
#define RunLoadOpenSendMultiCloseTestCaseId                 9
#define RunLoadOpenSendThreeThenCloseTestCaseId             10
#define RunLoadOpenSendCloseSimulTestCaseId                 11
#define RunLoadSocketOperationId                            12
#define RunLoadSendDataEvery20SecId                         13
#define RunLoadSendSmsSocketSendId                          14

#define BCDS_TEST_SUITE_STRESS_ID                           9
#define RunStressTurnOnOffCellularId                        1
#define RunStressTurnOnResetCellularId                      2
#define RunStressConfigParaWithMaxLengthId                  3
#define RunStressNetworkInfoFromModemId                     4
#define RunStressCellularNetworkConnectId                   5
#define RunStressCellularNetworkDisconnectId                6
#define RunStressSocketOpenId                               7
#define RunStressConfigBeforeInitEventId                    8
#define RunStressSocketOpenBeforeConnEventId                9
#define RunStressSocketOpenBeforeDisconnEventId             10
#define RunStressSmsSendContinuouslyId                      11
#define RunStressSmsSendThenSocketOpenId                    12
#define RunStressSmsSendUdpSendId                           13

#define BCDS_TEST_SUITE_SMOKE_ID                            10
#define RunSmokeOnOffId                                     1
#define RunSmokeResetId                                     2
#define RunSmokeGPRSConnectId                               3
#define RunSmokeUdpSocketSendId                             4
#define RunSmokeSmsSendTextId                               5
#define RunSmokeUdpSendReceiveExtendedAsciiId               6

#define BCDS_TEST_SUITE_SMS_ID                              11
#define RunSmsSendWithOneByteFiveTimesId                    1
#define RunSmsSendwith160BytesFiveTimesId                   2
#define RunSmsSendExtendedAsciiFiveTimesId                  3
#define RunSmsSendTo2NumbersId                              4
#define RunSmsSendIncrementalSmsId                          5
#define RunSmsSendDeinitTwiceSmsId                          6
#define RunSmsSetServiceNumberTwiceId                       7
#define RunSmsSendWithDiffCondId                            8
#define RunSmsSendSocketSendId                              9
#define RunSmsSendwithIncDelayId                            10
#define RunSmsSendTurnoffwithoutDeinitSmsId                 11
#define RunSmsSendGetSetServiceNumId                        12
#define RunSmsSendAfterDenitSmsId                           13
#define RunSmsSendWithNullDataId                            14
#define RunSmsSendWithExceedNumId                           15
#define RunSmsSendAndPoweroffId                             16
#define RunSmsSendWith6DigitNoId                            17

#endif /* CELLULAR_TESTSUITE_H */
