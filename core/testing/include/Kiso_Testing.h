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
 * @brief       This module manages the integration testing protocol and communication between the device under test 
                and the test coordinator 
 * 
 */

#ifndef KISO_TESTING_H_
#define KISO_TESTING_H_

/*###################### INCLUDED HEADERS ############################################################################*/

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"
#include "Kiso_Testing_Config.h"

/*###################### MACROS DEFINITION ###########################################################################*/

/*###################### TYPE DEFINITIONS ############################################################################*/
/**
 * @brief           Enumerates the internal units in the Kiso_Testing library which could return Retcode conform error 
 *                  codes. In case of error, one can refer to this enumeration to identify which source file has issued
 *                  it. 
*/
enum KISO_TESTING_Modules_E
{
    KISO_MODULE_ID_TESTING = 1,
    KISO_MODULE_ID_TESTING_TESTREGISTRY,         /**< KISO_TESTS_MODULE_ID_TESTREGISTRY */
    KISO_MODULE_ID_TESTING_CCHANNEL,             /**< KISO_TESTS_MODULE_ID_CCHANNEL */
    KISO_MODULE_ID_TESTING_SERIALMSGTRANSCEIVER, /**< KISO_TESTS_MODULE_ID_SERIALMSGTRANSCEIVER */
    KISO_MODULE_ID_TESTING_TESTRUNNER,           /**< KISO_TESTS_MODULE_ID_TESTRUNNER */
    KISO_MODULE_ID_TESTING_SERIAL,               /**< KISO_TESTS_MODULE_ID_SERIAL */
    KISO_MODULE_ID_TESTING_TESTENTRY,
};

/**
 * @brief           Enumerates the special return codes that could be returned from the different units in the 
 *                  Kiso_Testing library.
*/
enum KISO_TESTING_Retcodes_E
{
    RETCODE_TESTING_SUITE_ALREADY_REGISTERED = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_TESTING_CASE_ALREADY_REGISTERED,
    RETCODE_TESTING_CCHANNEL_INITIALISATION_FAILED,
    RETCODE_TESTING_INCOMPLETE_MESSAGE_RECEIVED,
    RETCODE_TESTING_CRC_MISMATCH,
    RETCODE_TESTING_VERSION_MISMATCH,
    RETCODE_TESTING_TLVELEMENT_NOT_FOUND,
    RETCODE_TESTING_REPORT_TIMEOUT,
    RETCODE_TESTING_CCHANNEL_NOT_SPECIFIED,
};

/**
 * @brief           Enumerates the types of messages exchanged between the Test_Executor and the Test_Controller 
 */
enum CCMsg_MessageType_E
{
    CCMSG_TYPE_COMMAND,
    CCMSG_TYPE_REPORT,
    CCMSG_TYPE_ACK,
};

/**
 * @brief           Encapsulates the elements composing the message header.
 */
typedef struct MessageHeader_S
{
    uint8_t messageInfo; /**< version is 2 bits, message type is 2 bits and the remaining 4 bits are reserved */
    uint8_t messageToken;
    uint8_t messageType;
    uint8_t errorCode;
    uint8_t testEntry;
    uint8_t testSuite;
    uint8_t testCase;
    uint8_t payloadLength;
} MsgHdr_T;

/**
 * @brief           Encapsulates the elements composing a TLV object(type, length and value)
 */
typedef struct TlvElement_S
{
    uint8_t type;
    uint8_t length;
    char *value;
} TlvElt_T;

/**
 *  @brief          Encapsulates the elements composing a message..
 */
typedef struct CCMsg_S
{
    MsgHdr_T header; /**< Header of the message */
    uint8_t payload[TEST_MAX_PAYLOAD_LENGTH];
    TlvElt_T tlvArray[CCHANNEL_MAX_NUMBER_OF_TLV_ELEMENTS]; /**< Parsed TLV array (from receive buffer) */
    uint8_t payloadIndex;
    uint8_t tlvArrayIndex;
    uint8_t numberOfTlvElements; /**< Number of TLV elements for the message */
    uint8_t rebootCounter;
    bool isFree : 1; /**< This bit indicates whether the received message is processed by test runner or not */
} CCMsg_T;

/**
 * @brief           Defines a prototype type for the setup functions of the test suites and test cases.
 */
typedef Retcode_T (*SetupFct_T)(CCMsg_T *ccmsg);

/**
 * @brief           Defines a prototype type for the tear down functions of the test suites and test cases.
 */
typedef Retcode_T (*TearDownFct_T)(CCMsg_T *ccmsg);

/**
 * @brief           Sefines a prototype type for the run functions of the test cases.
 */
typedef void (*RunFct_T)(CCMsg_T *ccmsg);

/*###################### EXPORTED FUNCTIONS PROTOTYPES ###############################################################*/

/**
 * @brief           Initializes the Testing Framework.
 * @details         this
 * @param[in]       eId Id of the Test Entry
 * @return          RETCODE_OK if initialized successfully error code otherwise
 */
Retcode_T Tests_Initialize(uint8_t eId, SetupFct_T setup, TearDownFct_T teardown);

/**
 * @brief           Registers a Test Suite
 * @details         todo mak explain what the function does currently
 * @param[in]       sId The identifier of the TestSuite to register
 * @param[in]       setup A reference to the setup function of the TestSuite
 * @param[in]       teardown A reference to the tear down function of the TestSuite
 * @note            setup and tear down functions pointers can be null if nothing has to be done.
 * @return          RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T Tests_RegisterTestSuite(uint8_t sId, SetupFct_T setup, TearDownFct_T teardown);

/**
 * @brief           Registers a Test Case
 * @details         todo mak explain what the function does 
 * @param[in]       sId identifier of the Test Suite the test to register
 * @param[in]       cId identifier of the Test Case to register
 * @param[in]       setup A reference to the setup function of the Test Suite
 * @param[in]       run A reference to the run function of the Test Suite
 * @param[in]       teardown A reference to the tear down function of the Test Suite
 * @note            setup and tear down functions pointers can be null if nothing has to be done.
 * @return          RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T Tests_RegisterTestCase(uint8_t sId, uint8_t cId, SetupFct_T setup, RunFct_T run, TearDownFct_T teardown);

/**
 * @brief           Sends a result of a test case execution.
 * @details         todo mak explain what the function does 
 * @param[in]       result The test result code (0: success / otherwise: failure)
 * @param[in]       reason  A 0-terminating string stating a reason. It can be NULL, if no reason should be sent.
 */
void Tests_SendReport(uint8_t result, char *reason);

/**
 * @brief           Gets a tlv element using the type of the tlvElement input
 * @details         todo mak explain what the function does 
 * @param[in]       ccmsg A reference to the message in which to look for the element
 * @param[in,out]   tlvElement A reference to where to store the element if found.
 *                  tlvElement->type is used as input to find the element's value
 * @return          RETCODE_OK in case of success, error code otherwise.
 *                  todo mak: make explicit the type search. interface is hiding information.
 */
Retcode_T Tests_GetTlvElement(CCMsg_T *ccmsg, TlvElt_T *tlvElement);

#endif /* KISO_TESTING_H_ */

/** @} */
