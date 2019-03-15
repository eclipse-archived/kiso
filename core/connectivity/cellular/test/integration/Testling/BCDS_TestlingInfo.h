/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/
 
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