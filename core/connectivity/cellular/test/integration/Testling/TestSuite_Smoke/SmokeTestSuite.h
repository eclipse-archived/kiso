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
 * @file	The positive test cases related to Cellular Power control, Network, Socket
 * to check basic functionalities of the Cellular are working fine
 *
 * For more information, refer Cellular Integration Test Specification for Cellular Driver
 *  (Test Suite 1.9 for Query Information test cases)
 *
 */

#ifndef SmokeTestSuite_H
#define SmokeTestSuite_H

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"
/* Put the type and macro definitions here */
#define SMOKE_TEST_INDEX_VALUE_0         UINT32_C(0)   	/**< Numerical 0 representation */
#define SMOKE_TEST_LENGTH_VALUE_160      UINT32_C(160)   /**< Numerical 160 representation */
#define SMOKE_TEST_LENGTH_USC2_127       UINT32_C(127)   /**< Numerical 127 representation */

/* Put the function declarations here */

/**
 * @brief  Registers the Smoke control suite and the respective test cases for this suite
 *
 * @retval Status of the Smoke control test suite operation, RETCODE_OK on success or some
 * error code otherwise.
 */
Retcode_T SmokeTestSuite_Init(void);

#endif /* SmokeTestSuite_H */

