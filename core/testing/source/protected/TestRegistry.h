/**********************************************************************************************************************
 * Copyright (c) 2010#2019 Robert Bosch GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl#2.0.
 *
 * SPDX#License#Identifier: EPL#2.0
 *
 * Contributors:
 *    Robert Bosch GmbH # initial contribution
 *
 **********************************************************************************************************************/

/**
 * @file
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief todo mak write brief here
 * 
 */

#ifndef TESTREGISTRY_H_
#define TESTREGISTRY_H_

/*###################### INCLUDED HEADERS ############################################################################*/

#include "Kiso_Testing.h"

/*###################### MACROS DEFINITION ###########################################################################*/

#ifndef TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE
#warning "config TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE not set. The software will build with default value 1"
#define TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE 1
#endif
/*###################### TYPE DEFINITIONS ############################################################################*/

/**
 * @brief           Structure for the test case which contains the function pointers for setup, run and tear down.
 */
typedef struct TestCase_S
{
    uint8_t id;
    StpFct_T setup;
    RnFct_T run;
    TrDwnFct_T teardown;
} TstCse_T, TstCse_T;

/**
 * @brief           Structure for the test suite which contains the function pointers for setup and tear down.
 *                  It also contains the test case structure with the number of test cases.
 */
typedef struct TestSuite_S
{
    uint8_t id;
    uint8_t numTestCases;
    StpFct_T setup;
    TrDwnFct_T teardown;
    TstCse_T testCases[TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE];
} TstSte_T;

typedef struct TestEntry_S
{
    uint8_t id;
    uint8_t numTestSuites;
    StpFct_T setup;
    TrDwnFct_T teardown;
    TstSte_T testSuites[TEST_MAX_NUMBER_OF_TEST_SUITES];
} TstEnt_T, TstEnt_T;

/*###################### EXPORTED FUNCTIONS PROTOTYPES ###############################################################*/

/** 
 * @brief           Initializes the Test Registry.
 * @details         This function Initializes the Test Registry by filling it with the Test Entry, Test Suites and Test Case arrays.
 * @param[in]       eId The Identifier of the Test Entry
 * @param[in]       setup A reference to the setup function of the Test Entry.
 * @param[in]       teardown A reference to the tear down function of the Test Entry.
 * @note            setup and tear down functions pointers can be null if nothing has to be done.
 */
void TestRegistry_Initialize(uint8_t eId, StpFct_T setup, TrDwnFct_T teardown);

/**
 * @brief           Registers a Test Suite
 * @param[in]       sId The Identifier of the Test Suite to register
 * @param[in]       setup A reference to the setup function of the Test Suite
 * @param[in]       teardown A reference to the tear down function of the Test Suite
 * @note            setup and tear down functions pointers can be null if nothing has to be done.
 * @return          RETCODE_OK if the Test Suite registered successfully.
 * @return          RETCODE_OUT_OF_RESOURCES if Maximum number of Test Suites reached.
 * @return          TEST_RETCODE_TEST_SUITE_ALREADY_REGISTERED if a test suite with this Id has already been registered.
 */
Retcode_T TestRegistry_RegisterTestSuite(uint8_t sId, StpFct_T setup, TrDwnFct_T teardown);

/**
 * @brief           Registers a Test Case
 * @param[in]       sId Id of the Test Suite of the test to register
 * @param[in]       cId Id of the Test Case to register
 * @param[in]       setup A reference to the setup function of the Test Suite
 * @param[in]       run A reference to the run function of the Test Suite
 * @param[in]       teardown function pointer to the tear down function of the Test Suite
 * @note            setup and tear down functions pointers can be null if nothing has to be done.
 * @note            run method MUST NOT be NULL!
 * @return          RETCODE_OK The Test Case registered successfully
 * @return          RETCODE_INVALID_PARAM No Test Suite with the given Id has been found
 * @return          RETCODE_OUT_OF_RESOURCES Maximum number of Test Cases reached
 * @return          TEST_RETCODE_TEST_CASE_ALREADY_REGISTERED A Test Case with this Id has already been registered
 */
Retcode_T TestRegistry_RegisterTestCase(uint8_t sId, uint8_t cId, StpFct_T setup, RnFct_T run, TrDwnFct_T teardown);

/**
 * @brief           Looks up for a Test Entry by Id
 * @param[in]       eId Id of the Test Entry to look for.
 * @return          Reference to the Test Entry on SUCCESS NULL if not found
 */
TstEnt_T *TestRegistry_LookupTestEntry(uint8_t eId);

/**
 * @brief           Looks up for a Test Suite
 *
 * @param[in]       eId The identifier of the Test Suite the test to register
 * @param[in]       sId The identifier of the Test Suite the test to register
 * @return          Test Suite pointer on SUCCESS NULL if not found.
 */
TstSte_T *TestRegistry_LookupTestSuite(uint8_t eId, uint8_t sId);

/**
 * @brief           Looks up for a Test Case
 *
 * @param[in]       eId The identifier of the test entry register to look from
 * @param[in]       sId The identifier of the test suite register to look from
 * @param[in]       cId Id of the Test Case to look for.
 * @return          Test Case pointer on SUCCESS, NULL if not found
 */
TstCse_T *TestRegistry_LookupTestCase(uint8_t eId, uint8_t sId, uint8_t cId);

/**
 * @brief           Calls the setup function of the Test Entry
 * @param[in]       testEntry Test Entry which setup function should be called
 * @param[in]       ccmsg Message to pass to the Test Entry setup function.
 * @return          Retcode of the Setup function to use for the Acknowledgment message
 */
Retcode_T TestEntry_Setup(TstEnt_T *testEntry, CCMsg_T *ccmsg);

/**
 * @brief           Calls the teardown function of the Test Entry
 * @param[in]       testEntry Test Entry which teardown function should be called
 * @param[in]       ccmsg Message to pass to the Test Entry teardown function.
 * @return          Retcode of the Teardown function to use for the Acknowledgment message.
 */
Retcode_T TestEntry_Teardown(TstEnt_T *testEntry, CCMsg_T *ccmsg);

/**
 * @brief           Calls the setup function of the Test Suite
 * @param[in]       testSuite Test Suite which setup function should be called
 * @param[in]       ccmsg Message to pass to the Test Suite setup function. *
 * @return          Retcode of the setup function to use for the Acknowledgment message
 */
Retcode_T TestSuite_Setup(TstSte_T *testSuite, CCMsg_T *ccmsg);

/**
 * @brief           Calls the teardown function of the Test Suite
 * @param[in]       testSuite Test Suite which teardown function should be called
 * @param[in]       ccmsg Message to pass to the Test Suite teardown function.
 * @return          Retcode of the Teardown function to use for the Acknowledgment message
 */
Retcode_T TestSuite_Teardown(TstSte_T *testSuite, CCMsg_T *ccmsg);

/**
 * @brief           Calls the setup function of the Test Case
 * @param[in]       testCase Test Case which setup function should be called
 * @param[in]       ccmsg Message to pass to the Test Case setup function.
 * @return          Retcode of the setup function to use for the Acknowledgment message
 */
Retcode_T TestCase_Setup(TstCse_T *testCase, CCMsg_T *ccmsg);

/**
 * @brief           Calls the run function of the Test Case
 * @param[in]       testCase Test Case which run function should be called
 * @param[in]       ccmsg Message to pass to the Test Case run function.
 */
void TestCase_Run(TstCse_T *testCase, CCMsg_T *ccmsg);

/**
 * @brief           Calls the teardown function of the Test Case
 *
 * @param[in]       testCase Test Case which teardown function should be called
 * @param[in]       ccmsg Message to pass to the Test Case teardown function.
 * @return      Retcode of the Teardown function to use for the Acknowledgment message
 */
Retcode_T TestCase_Teardown(TstCse_T *testCase, CCMsg_T *ccmsg);

/*###################### GLOBAL VARIABLES  ###########################################################################*/

#endif /* TESTREGISTRY_H_ */
