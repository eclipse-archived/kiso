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
 * @file SMS test suite header file provide the wrapper APIs.
 *
 * SmsTestSuite implements all the conditions related to sms send, set service center number
 * and get service center number.
 *
 *	For more information, refer Cellular Integration Test Specification for Cellular Driver
 *  (Test Suite 1.10 for Sms test cases)
 */

#ifndef SMSTESTSUITE_H_
#define SMSTESTSUITE_H_

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

/* Put the type and macro definitions here */

#define SMS_TEST_INDEX_VALUE_0             	  UINT32_C(0)   	/**< Numerical 0 representation */
#define SMS_TEST_INDEX_VALUE_1             	  UINT32_C(1)   	/**< Numerical 1 representation */
#define SMS_TEST_INDEX_VALUE_5             	  UINT32_C(5)   	/**< Numerical 5 representation */
#define SMS_TEST_INDEX_VALUE_6             	  UINT32_C(6)   	/**< Numerical 6 representation */
#define SMS_TEST_LENGTH_VALUE_20              UINT32_C(20)   	/**< Numerical 20 representation */
#define SMS_TEST_ITIRATIVE_VALUE_40           UINT32_C(40)   	/**< Numerical 40 representation */
#define SMS_TEST_LENGTH_VALUE_50              UINT32_C(50)   	/**< Numerical 50 representation */
#define SMS_TEST_LENGTH_VALUE_100             UINT32_C(100)   	/**< Numerical 100 representation */
#define SMS_TEST_LENGTH_VALUE_90              UINT32_C(90)     /**< Numerical 90 representation */
#define SMS_TEST_LENGTH_VALUE_160             UINT32_C(160)   	/**< Numerical 160 representation */
#define SMS_TEST_LENGTH_USC2_127              UINT32_C(127)   	/**< Numerical 127 representation */
#define SMS_SEND_ITERATION_DELAY_MILLI_SECS   UINT32_C(50000)   /**< Timeout for sending the text SMS data */


/**
 * @brief  Registers the SMS suite and the respective test cases for this suite
 *
 * @retval Status of the SMS test suite operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T SmsTestSuite_Init(void);

#endif /* SMSTESTSUITE_H_ */
