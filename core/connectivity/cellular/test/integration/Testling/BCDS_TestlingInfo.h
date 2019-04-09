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

#ifndef BCDS_TestlingInfo_H
#define BCDS_TestlingInfo_H

#include "TestSuite.h"

/**
 * @brief Range of available module ID's for the test application
 */
enum BCDS_Testling_ModuleID_E
{
    BCDS_TESTLING_CELLULAR_MODULE_ID_TEST_SECTION = 1,
    BCDS_TESTLING_CELLULAR_MODULE_ID_STRESS_TEST_SUITE = 2,
    BCDS_TESTLING_CELLULAR_MODULE_ID_SMOKE_TEST_SUITE = 3,
    BCDS_TESTLING_CELLULAR_MODULE_ID_POSITIVE_TEST_SUITE = 4,
    BCDS_TESTLING_CELLULAR_MODULE_ID_NEGATIVE_TEST_SUITE = 5,
    BCDS_TESTLING_CELLULAR_MODULE_ID_LOAD_TEST_SUITE = 6,
    BCDS_TESTLING_CELLULAR_MODULE_ID_CELLULAR_SUPPORT = 7,
    BCDS_TESTLING_CELLULAR_MODULE_ID_SYSTEM = 8,
    BCDS_TESTLING_CELLULAR_MODULE_ID_MAIN = 9,
    BCDS_TESTLING_CELLULAR_MODULE_ID_APP = 10,
    BCDS_TESTLING_CELLULAR_MODULE_ID_LED = 11,
    BCDS_TESTLING_CELLULAR_MODULE_ID_CELLULAR_APP = 12,
    BCDS_TESTLING_CELLULAR_MODULE_ID_POWERCONTROL_TEST_SUITE = 13,
    BCDS_TESTLING_CELLULAR_MODULE_ID_QUERY_TEST_SUITE = 14,
    BCDS_TESTLING_CELLULAR_MODULE_ID_SMS_TEST_SUITE = 15,
    BCDS_TESTLING_CELLULAR_MODULE_ID_SOCKET_TEST_SUITE = 16,
    BCDS_TESTLING_CELLULAR_MODULE_ID_UDP_TEST_SUITE = 17,
    BCDS_TESTLING_CELLULAR_MODULE_ID_MULTI_TASK_HANDLE_TEST_SUITE = 18,
    BCDS_TESTLING_CELLULAR_MODULE_ID_NETWORK_TEST_SUITE = 19
};

#endif /* BCDS_TestlingInfo_H */