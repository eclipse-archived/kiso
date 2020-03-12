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
 * @brief       Provides conversion from the bsp board interface to the testapp expected interface 
 * 
 */

/*###################### INCLUDED HEADERS ############################################################################*/
#if defined(CommonGateway)
#include "BSP_CommonGateway.h"
#elif defined(NucleoL4R5ZI)
#include "BSP_NucleoL4R5ZI.h"
#endif

/*###################### MACROS DEFINITION ###########################################################################*/

#if defined(CommonGateway)

#define TEST_BOARD_LED_ALL COMMONGATEWAY_LED_ALL
#define TEST_BOARD_LED_PASS COMMONGATEWAY_LED_GREEN_ID
#define TEST_BOARD_LED_FAIL COMMONGATEWAY_LED_BLUE_ID
#define TEST_BOARD_LED_PANIC COMMONGATEWAY_LED_RED_ID
#define TEST_BOARD_LED_COMMAND_ON COMMONGATEWAY_LED_COMMAND_ON
#define TEST_BOARD_LED_COMMAND_OFF COMMONGATEWAY_LED_COMMAND_OFF
#define TEST_BOARD_LED_COMMAND_TOGGLE COMMONGATEWAY_LED_COMMAND_TOGGLE

#elif defined(NucleoL4R5ZI)

#define TEST_BOARD_LED_ALL NUCLEOL4R5ZI_LED_ALL
#define TEST_BOARD_LED_PASS NUCLEOL4R5ZI_LED_GREEN_ID
#define TEST_BOARD_LED_FAIL NUCLEOL4R5ZI_LED_BLUE_ID
#define TEST_BOARD_LED_PANIC NUCLEOL4R5ZI_LED_RED_ID
#define TEST_BOARD_LED_COMMAND_ON NUCLEOL4R5ZI_LED_COMMAND_ON
#define TEST_BOARD_LED_COMMAND_OFF NUCLEOL4R5ZI_LED_COMMAND_OFF
#define TEST_BOARD_LED_COMMAND_TOGGLE NUCLEOL4R5ZI_LED_COMMAND_TOGGLE
#endif

/*###################### TYPE DEFINITIONS ############################################################################*/

/*###################### EXPORTED FUNCTIONS PROTOTYPES ###############################################################*/

/*###################### GLOBAL VARIABLES  ###########################################################################*/

/** @} */