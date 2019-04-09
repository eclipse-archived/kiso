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
 * @file Add a brief description here.
 *
 * Put here the documentation of this header file. Explain the interface exposed
 * by this header, e.g. what is the purpose of use, how to use it, etc.
 */

#ifndef BCDS_TestingConfig_H
#define BCDS_TestingConfig_H

/** Macro to choose the reboot feature */
#define BCDS_FEATURE_TEST_REBOOT		0

/** Macros to define the Test Runner's task */
#define TEST_RUNNER_TASK_PRIO			2
#define TEST_RUNNER_TASK_STACK_DEPTH	1024
#define TEST_RUNNER_QUEUE_SIZE			5

/* macro to select which CChannel to use for integration testing */
#define SERIAL_CCHANNEL       1

/** Macros defining the maximum number of Test Suites */
#ifndef TEST_MAX_NUMBER_OF_TEST_SUITES
#define TEST_MAX_NUMBER_OF_TEST_SUITES                  10
#endif

/** Macros defining the maximum number of Test Cases per Test Suite */
#ifndef TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE
#define TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE    12
#endif

#ifndef BCDS_LEUART_CC
#define BCDS_LEUART_CC                  0
#endif

#endif /* BCDS_TestingConfig_H */

