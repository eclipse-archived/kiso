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
 * @file	This LoadtestSuite implements the Load Test cases for Cellular.
 *
 *  For more information, refer Cellular Integration Load Test Specification for Cellular Driver
 *  (Test Suite 1.6 for Load Testing for Cellular Driver)
 */

#ifndef LOADTESTSUITE_H
#define LOADTESTSUITE_H

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

/* Put the type and macro definitions here */
#define INIT_MAX_TIMEOUT_30_SECS              UINT32_C(30000)	/**< Network init max time out in milliseconds */
#define CONNECT_MAX_TIMEOUT_IN_SECS           UINT32_C(180000)	/**< Network connect max time out in milliseconds */
#define DISCONNECT_MAX_TIMEOUT_IN_SECS        UINT32_C(180000)	/**< Network disconnect max time out in milliseconds */
#define EXPECTED_RESPONSE_TIME_CONFIG         UINT32_C(5)       /**< Config API max response time in seconds */
#define EXPECTED_RESPONSE_TIME_POWERON        UINT32_C(20)      /**< PowerOn API max response time in seconds */
#define EXPECTED_RESPONSE_TIME_RESET          UINT32_C(7)       /**< Reset API max response time in seconds */
#define LOAD_TEST_COUNTER_INIT_VAL_0		  UINT8_C(0)		/**< Numerical 0 representation */
#define LOAD_TEST_ITERATION_VALUE		      100UL		        /**< Numerical 100 representation */
#define LOAD_TEST_ITERATION_THREE_SCKT        UINT32_C(5)       /**< Numerical 5 representation */
#define DATA_SEND_LEN                         UINT32_C(185)     /**< Numerical 185 representation */

/* Macro for Loss percentage */
#define UDPSENDFAILURE_TOLERANCE_PER            2UL		/**< 2 percent UDP send loss for iterations*/
#define UDPSENDFAILURE_TOLERANCE(X)           	(X==0)?1UL: ((UDPSENDFAILURE_TOLERANCE_PER*X)/100UL)
#define UDPRECEIVEFAILURE_TOLERANCE_PER         5UL         /**< 5 percent UDP receive loss for iterations */
#define UDPRECEIVEFAILURE_TOLERANCE(X)          (X==0)?1UL: ((UDPRECEIVEFAILURE_TOLERANCE_PER*X)/100UL)
#define UDPMISMATCHFAILURE_TOLERANCE_PER        5UL         /**< 5 percent UDP mismatch for iterations */
#define UDPMISMATCHFAILURE_TOLERANCE(X)         (X==0)?1UL: ((UDPMISMATCHFAILURE_TOLERANCE_PER*X)/100UL)

/* Time delay macro definitions */
#define PACKETSEND_DELAY_500_MILLISECS        UINT32_C(500)     /**< Delay of 500 Milli Sec Sec */
#define TESTCASE14_DELAY_20_SECONDS		      UINT32_C(20000)	/**< Test case delay of 20s for TC 1.6.14 */

/* Put the function declarations here */

/**
 * @brief  Registers the Load suite and the respective test cases for this suite
 *
 * @retval Status of the LOAD test suite operation, RETCODE_OK on success or some
 * error code otherwise.
 */
Retcode_T LoadTestSuite_Init(void);

#endif /* LOADTESTSUITE_H */

