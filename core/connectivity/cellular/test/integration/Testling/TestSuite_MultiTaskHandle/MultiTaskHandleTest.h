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
 * @file
 *  This MultiTask Handling Test Suite implements the Test cases related to
 *  Multi task for Cellular Driver.
 *
 *  For more information, refer Cellular Integration Test Specification for Cellular Driver
 *  (Test Suite 1.11 for Cellular Driver Multitask Handling Test)
 *
 */

#ifndef Multitaskhandletest_H
#define Multitaskhandletest_H

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

/* Put the type and macro definitions here */
#define DELAY_TIME_TASK_1               UINT8_C(100)    /**< Task delay time in milliseconds */
#define DELAY_TIME_TASK_2               UINT8_C(200)    /**< Task delay time in milliseconds */
#define STACK_SIZE_TASK_1               UINT8_C(512)    /**< Stack Size in bytes */
#define STACK_SIZE_TASK_2               UINT8_C(512)    /**< Stack Size in bytes */
#define PRIORITY_TASK_1                 UINT8_C(1)      /**< Task priority */
#define PRIORITY_TASK_2                 UINT8_C(2)      /**< Task priority */
#define MAX_TIMEOUT_FOR_TEST_CASE_WAIT  UINT32_C(5000)  /**< Maximum time in milliseconds test case will wait
                                                             for running task and time-out if semaphore is not taken. */

/* Put the function declarations here */

/**
 * @brief  Registers the Multi Task Handle Test suite and the respective test case for this suite
 *
 * @retval Status of the Multi Task Handle  test suite operation, RETCODE_OK on success or some
 * error code otherwise.
 */
Retcode_T MultiTaskHandleTestSuite_Init(void);

#endif /* Multitaskhandletest_H */
