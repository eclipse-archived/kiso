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
/* Define next module ID here and assign a value to it! */
};
#endif /* BCDS_TESTAPPINFO_H_ */
