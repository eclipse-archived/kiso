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
 * @file	This file provide the wrapper APIs for StressTestSuite.
 *
 *  For more information, refer Cellular Integration Test Specification for Cellular Driver
 *  (Test Suite 1.7 for Cellular Driver stress Testing)
 */

#ifndef STRESSTESTSUITE_H
#define STRESSTESTSUITE_H

/* Include all headers which are needed by this file. */

/* Put the type and macro definitions here */
#define STRESS_TEST_INDEX_VALUE_100             UINT32_C(100)   /**< Numerical 100 representation */
#define STRESS_TEST_INDEX_VALUE             	UINT32_C(100)   /**< Numerical 3 representation */
#define STRESS_TEST_INDEX_VALUE_30             	UINT32_C(30)   	/**< Numerical 30 representation */
#define STRESS_TEST_INDEX_INIT_VAL_0			UINT32_C(0)	 	/**< Numerical 0 representation */
#define STRESS_TEST_EAT_ENABLE                  UINT8_C(1)		/**< Numerical 1 representation */
#define STRESS_TEST_NO_ERROR                  	UINT8_C(0)		/**< Numerical 0 representation */
#define STRESS_TEST_SMS_SEND_SUCCESSFUL         UINT8_C(5)      /**< Numerical 5 representation */
#define STRESS_TEST_UDP_DATA_SEND_LEN           UINT32_C(100)     /**< Numerical 100 representation */
#define STRESS_TEST_DELAY_10_MIN                UINT32_C(600000)  /**<Delay of 10 Minute */
#define STRESS_TEST_COUNTRY_CODE                "404"			/**< Airtel and vodafone country code- 404 */
#define STRESS_TEST_NETWORK_CODE                "94"			/**< Network code for Airtel-94 and Vodafone-43 */

/* Put the function declarations here */

/**
 * @brief  Registers the Stress Test suite and the respective test cases for this suite
 *
 * @retval Status of the Stress test suite operation, RETCODE_OK on success or some
 * error code otherwise.
 */
Retcode_T StressTestSuite_Init(void);

#endif /* STRESSTESTSUITE_H */

