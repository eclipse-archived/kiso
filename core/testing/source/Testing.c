
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
 *
 * @brief
 * This file implements the promised API functions in Kiso_Testing.h with help of the components TestRunner TestRegistry
 * and CChannel.
 */
/*###################### INCLUDED HEADERS ############################################################################*/

#include "Kiso_Testing.h"
#include "TestRegistry.h"
#include "TestRunner.h"
#include "CChannel.h"

/*###################### MACROS DEFINITION ###########################################################################*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_MODULE_ID_TESTING

/*###################### LOCAL_TYPES DEFINITION ######################################################################*/

/*###################### LOCAL FUNCTIONS DECLARATION #################################################################*/

/*###################### VARIABLES DECLARATION #######################################################################*/

/*###################### EXPOSED FUNCTIONS IMPLEMENTATION ############################################################*/

/* @see Kiso_Testing.h for function description */
Retcode_T Tests_Initialize(uint8_t eId, StpFct_T setup, TrDwnFct_T teardown)
{
    TestRegistry_Initialize(eId, setup, teardown);
    return TestRunner_Initialize();
}

/* @see Kiso_Testing.h for function description */
Retcode_T Tests_RegisterTestSuite(uint8_t sId, StpFct_T setup, TrDwnFct_T teardown)
{
    return TestRegistry_RegisterTestSuite(sId, setup, teardown);
}

/* @see Kiso_Testing.h for function description */
Retcode_T Tests_RegisterTestCase(uint8_t sId, uint8_t cId, StpFct_T setup, RnFct_T run, TrDwnFct_T teardown)
{
    return TestRegistry_RegisterTestCase(sId, cId, setup, run, teardown);
}

/* @see Kiso_Testing.h for function description */
void Tests_SendReport(uint8_t result, char *reason)
{
    TestRunner_SendReport(result, reason);
}

/* @see Kiso_Testing.h for function description */
Retcode_T Tests_GetTlvElement(CCMsg_T *ccmsg, TlvElt_T *tlvElement)
{
    return CChannel_GetTlvElement(ccmsg, tlvElement);
}

/*###################### LOCAL FUNCTIONS IMPLEMENTATION ##############################################################*/