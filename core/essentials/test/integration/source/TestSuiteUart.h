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
 * @brief       Provides an API for the following functionality
 * 
 */
#ifndef TESTSUITE_UART_H_
#define TESTSUITE_UART_H_

/*###################### INCLUDED HEADERS ############################################################################*/

/*###################### MACROS DEFINITION ###########################################################################*/

/*###################### TYPE DEFINITIONS ############################################################################*/

enum TestSuiteUart_Retcodes_E
{
    TESTSUITE_SETUP_TRIGGERED_SEVERAL_TIMES = RETCODE_FIRST_CUSTOM_CODE,
    TESTSUITE_RUN_TRIGGERED_SEVERAL_TIMES,
    TestSuite_Teardown_TRIGGERED_SEVERAL_TIMES,
};

/*###################### EXPORTED FUNCTIONS PROTOTYPES ###############################################################*/

/**
 * @brief       Initializes the uart test suite
 * @details     This function  will register the uart test suites in the Testing module TestSuites register and will 
 *              also register for execution all the test cases belonging to this test suite 
 * @param       id is the identifier to be given to the test suite it will be used in the communication protocol between
 *              the test executor and the test controller @see todo: add link to docu
 */
Retcode_T TestSuiteUart_Initialize(uint8_t id);

/*###################### GLOBAL VARIABLES  ###########################################################################*/

/** @} */

#endif /* TESTSUITE_UART_H_ */
