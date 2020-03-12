/*******************************************************************************
 * Copyright (c) 2010-2020 Robert Bosch GmbH
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
 ******************************************************************************/

/**
 * @file
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief Provides conversion from the bsp board interface to the testapp
 * expected interface.
 */

#if defined(CommonGateway)
#include "BSP_CommonGateway.h"
#elif defined(NucleoF767)
#include "BSP_NucleoF767.h"
#endif

#if defined(CommonGateway)

#define TEST_BOARD_LED_ALL COMMONGATEWAY_LED_ALL
#define TEST_BOARD_LED_PASS COMMONGATEWAY_LED_GREEN_ID
#define TEST_BOARD_LED_FAIL COMMONGATEWAY_LED_BLUE_ID
#define TEST_BOARD_LED_PANIC COMMONGATEWAY_LED_RED_ID
#define TEST_BOARD_LED_COMMAND_ON COMMONGATEWAY_LED_COMMAND_ON
#define TEST_BOARD_LED_COMMAND_OFF COMMONGATEWAY_LED_COMMAND_OFF
#define TEST_BOARD_LED_COMMAND_TOGGLE COMMONGATEWAY_LED_COMMAND_TOGGLE

#elif defined(NucleoF767)

#define TEST_BOARD_LED_ALL NUCLEOF767_LED_ALL
#define TEST_BOARD_LED_PASS NUCLEOF767_LED_GREEN_ID
#define TEST_BOARD_LED_FAIL NUCLEOF767_LED_BLUE_ID
#define TEST_BOARD_LED_PANIC NUCLEOF767_LED_RED_ID
#define TEST_BOARD_LED_COMMAND_ON NUCLEOF767_LED_COMMAND_ON
#define TEST_BOARD_LED_COMMAND_OFF NUCLEOF767_LED_COMMAND_OFF
#define TEST_BOARD_LED_COMMAND_TOGGLE NUCLEOF767_LED_COMMAND_TOGGLE

#endif

/** @} */
