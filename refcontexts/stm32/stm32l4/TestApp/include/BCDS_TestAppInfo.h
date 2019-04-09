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

/**
 * @file 
 * @brief Add a brief description here.
 *
 * @details Put here the documentation of this header file. Explain the interface exposed
 * by this header, e.g. what is the purpose of use, how to use it, etc.
 */

#ifndef BCDS_TESTAPPINFO_H_
#define BCDS_TESTAPPINFO_H_

/**
 * @brief BCDS_MODULE_ID for each Test App C module
 * @info  usage:
 *      #undef BCDS_MODULE_ID
 *      #define BCDS_MODULE_ID BCDS_BLE_MODULE_ID_xxx
 */
enum BCDS_TestApp_ModuleID_E
{
    BCDS_TEST_APP_MODULE_ID_TEST_SECTION = 1,
    BCDS_TEST_APP_MODULE_ID_SYSTEM = 2,
    BCDS_TEST_APP_MODULE_ID_MAIN = 3,
    BCDS_TEST_APP_MODULE_ID_APP = 4,
    BCDS_TEST_APP_MODULE_ID_LED = 5,
    BCDS_TEST_APP_MODULE_ID_FAULT_HANDLER = 6,
/* Define next module ID here and assign a value to it! */
};
#endif /* BCDS_TESTAPPINFO_H_ */
