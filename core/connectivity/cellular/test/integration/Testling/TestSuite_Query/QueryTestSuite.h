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
 *  This QuerytestSuite implements All the conditions related to network information, error info,
 *  cellular status(initializing,initialized,connected,disconnected) and validating the same.
 *
 *  For more information, refer Cellular Integration Test Specification for Cellular Driver
 *  (Test Suite 1.5 for Query Information test cases)
 */

#ifndef QUERYTESTSUITE_H_
#define QUERYTESTSUITE_H_

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

/* Put the type and macro definitions here */
#define QUERY_TEST_EAT_ENABLE                   UINT8_C(1)		/**< Numerical 1 representation */
#define QUERY_TEST_CASE_NETWORK_INFO_LENGTH		UINT8_C(16)		/**< Numerical 16 representation */
#define QUERY_TEST_NO_ERROR                  	UINT8_C(0)		/**< Numerical 0 representation */
#define QUERY_TEST_NW_CODE_LEN_POSITION        	UINT8_C(14)		/**< Numerical 14 representation */
#define QUERY_TEST_STRING_END_POSITION        	UINT8_C(15)		/**< Numerical 15 representation */
#define QUERY_TEST_ASCII_CON_VAL                UINT8_C(48)		/**< Numerical 48 representation */

/* Put the function declarations here */

/**
 * @brief  Registers the suite and the respective test cases for this suite
 *
 */
Retcode_T QueryTestSuite_Init(void);

#endif /* QUERYTESTSUITE_H_ */
