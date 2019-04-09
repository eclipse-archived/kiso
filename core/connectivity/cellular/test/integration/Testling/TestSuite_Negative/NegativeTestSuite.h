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
 * @file	This NegativetestSuite implements the Negative Test cases for Cellular.
 *
 *  For more information, refer Cellular Integration Test Specification for Cellular Driver
 *  (Test Suite 1.4 for Negative test cases)
 */

#ifndef NEGATIVETESTSUITE_H_
#define NEGATIVETESTSUITE_H_

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

/* Put the type and macro definitions here */
#define NEGATIVE_TEST_SUITE_ID      UINT8_C(4)		/**< Test Suite ID representation */
#define NEGATIVE_TEST_CASE_ID_1     UINT8_C(1)		/**< Test Case ID-1 representation */
#define NEGATIVE_TEST_CASE_ID_2     UINT8_C(2)		/**< Test Case ID-2 representation */
#define NEGATIVE_TEST_CASE_ID_3     UINT8_C(3)		/**< Test Case ID-3 representation */
#define NEGATIVE_TEST_CASE_ID_4     UINT8_C(4)		/**< Test Case ID-4 representation */
#define NEGATIVE_TEST_CASE_ID_5     UINT8_C(5)		/**< Test Case ID-5 representation */
#define NEGATIVE_TEST_CASE_ID_6     UINT8_C(6)		/**< Test Case ID-6 representation */
#define NEGATIVE_TEST_CASE_ID_7     UINT8_C(7)		/**< Test Case ID-7 representation */
#define NEGATIVE_TEST_CASE_ID_8     UINT8_C(8)		/**< Test Case ID-8 representation */
#define NEGATIVE_TEST_CASE_ID_9     UINT8_C(9)		/**< Test Case ID-9 representation */
#define NEGATIVE_TEST_CASE_ID_10    UINT8_C(10)		/**< Test Case ID-10 representation */
#define NEGATIVE_TEST_CASE_ID_11    UINT8_C(11)		/**< Test Case ID-11 representation */
#define NEGATIVE_TEST_CASE_ID_12	UINT8_C(12)		/**< Test Case ID-12 representation */
#define NEGATIVE_TEST_CASE_ID_13    UINT8_C(13)		/**< Test Case ID-13 representation */
#define NEGATIVE_TEST_CASE_ID_14    UINT8_C(14)     /**< Test Case ID-14 representation */
#define NEGATIVE_TEST_CASE_ID_15    UINT8_C(15)     /**< Numerical 15 representation */
#define NEGATIVE_TEST_CASE_ID_16    UINT8_C(16)      /**< Test Case ID-16 representation */
#define NEGATIVE_TEST_CASE_ID_17    UINT8_C(17)     /**< Test Case ID-17 representation */
#define NEGATIVE_TEST_CASE_ID_18    UINT8_C(18)     /**< Test Case ID-18 representation */
#define NEGATIVE_TEST_CASE_ID_19    UINT8_C(19)     /**< Test Case ID-19 representation */
#define NEGATIVE_TEST_CASE_ID_20    UINT8_C(20)     /**< Test Case ID-20 representation */
#define NEGATIVE_TEST_CASE_ID_21	UINT8_C(21)		/**< Test Case ID-21 representation */
#define NEGATIVE_TEST_CASE_ID_22	UINT8_C(22)		/**< Test Case ID-22 representation */
#define NEGATIVE_TEST_CASE_ID_23	UINT8_C(23)		/**< Test Case ID-23 representation */
#define NEGATIVE_TEST_CASE_ID_24	UINT8_C(24)		/**< Test Case ID-24 representation */
#define NEGATIVE_TEST_CASE_ID_25	UINT8_C(25)		/**< Test Case ID-25 representation */
#define NEGATIVE_TEST_CASE_ID_26    UINT8_C(26)		/**< Test Case ID-26 representation */
#define NEGATIVE_TEST_CASE_ID_27    UINT8_C(27)		/**< Test Case ID-27 representation */
#define NEGATIVE_TEST_CASE_ID_28    UINT8_C(28)		/**< Test Case ID-28 representation */
#define NEGATIVE_TEST_CASE_ID_29    UINT8_C(29)		/**< Test Case ID-29 representation */
#define NEGATIVE_TEST_CASE_ID_30	UINT8_C(30)		/**< Test Case ID-30 representation */
#define NEGATIVE_TEST_CASE_ID_31	UINT8_C(31)		/**< Test Case ID-31 representation */
#define NEGATIVE_TEST_CASE_ID_32    UINT8_C(32)		/**< Test Case ID-32 representation */
#define NEGATIVE_TEST_CASE_ID_33    UINT8_C(33)		/**< Test Case ID-33 representation */
#define NEGATIVE_TEST_CASE_ID_34    UINT8_C(34)		/**< Test Case ID-34 representation */
#define NEGATIVE_TEST_CASE_ID_35    UINT8_C(35)		/**< Test Case ID-35 representation */
#define NEGATIVE_TEST_CASE_ID_36    UINT8_C(36)     /**< Test Case ID-36 representation */
#define NEGATIVE_TEST_CASE_ID_37    UINT8_C(37)     /**< Test Case ID-37 representation */
#define NEGATIVE_TEST_CASE_ID_38    UINT8_C(38)     /**< Test Case ID-38 representation */
#define NEGATIVE_TEST_CASE_ID_39    UINT8_C(39)     /**< Test Case ID-39 representation */
#define NEGATIVE_TEST_CASE_ID_40    UINT8_C(40)     /**< Test Case ID-40 representation */

#define NEGATIVE_TEST_COUNTER_INIT_VAL_0			UINT32_C(0)		/**< Numerical 0 representation */
#define NEGATIVE_TEST_COUNTER_INIT_VAL_3			UINT32_C(3)		/**< Numerical 3 representation */
#define NEGATIVE_TEST_ITERATION_VAL_2				UINT32_C(2)		/**< Numerical 2 representation */
#define NEGATIVE_TEST_ITERATION_VAL_5				UINT32_C(5)		/**< Numerical 5 representation */
#define NEGATIVE_TEST_ITERATION_VAL_7				UINT32_C(7)		/**< Numerical 7 representation */
#define NEGATIVE_TEST_ITERATION_VAL_20				UINT32_C(20)	/**< Numerical 20 representation */
#define NEGATIVE_TEST_ITERATION_VAL_161				UINT32_C(161)	/**< Numerical 161 representation */
#define NEGATIVE_TEST_ITERATION_VAL_100				UINT32_C(100)	/**< Numerical 100 representation */
#define NEGATIVE_TEST_ITERATION_VAL_30				UINT32_C(30)	/**< Numerical 30 representation */
#define NEGATIVE_TEST_EAT_ENABLE                    UINT8_C(1)		/**< Numerical 1 representation */
#define MAX_SOCKETS_FOR_THIS_TEST_7                 UINT8_C(7)		/**< Numerical 7 representation */
#define	NEGATIVE_TEST_SOCKET_ID                    	INT32_C(-1)		/**< Numerical -1 representation */
#define NEGATIVE_TEST_CASE_COUNT_3                  UINT8_C(3)		/**< Count 3 for Testcase21 */
#define NEGATIVE_TEST_LENGTH_USC2_127               UINT8_C(127)		/**< Numerical 127 representation */

#define WRONGSOCKET_SERVER_IP                       UINT32_C(0xD75C215D) /** Wrong IP Address **/

/* Put the function declarations here */

/**
 * @brief  Registers the suite and the respective test cases for this suite
 *
 */
Retcode_T NegativeTestSuite_Init(void);

#endif /* NEGATIVETESTSUITE_H_ */
