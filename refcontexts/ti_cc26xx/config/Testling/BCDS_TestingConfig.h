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
 * @file Add a brief description here.
 *
 * Put here the documentation of this header file. Explain the interface exposed
 * by this header, e.g. what is the purpose of use, how to use it, etc.
 */

#ifndef BCDS_TestingConfig_H
#define BCDS_TestingConfig_H

/** Macro to choose the reboot feature */
#ifndef BCDS_FEATURE_TEST_REBOOT
#define BCDS_FEATURE_TEST_REBOOT 0
#endif

/** Macros to define the Test Runner's task */
#define TEST_RUNNER_TASK_PRIO			3
#ifndef TEST_RUNNER_TASK_STACK_DEPTH
#define TEST_RUNNER_TASK_STACK_DEPTH	1024
#endif
#define TEST_RUNNER_QUEUE_SIZE			5

/* macro to select which CChannel to use for integration testing */
#ifndef USB_CCHANNEL
#define USB_CCHANNEL       0
#endif

#ifndef SERIAL_CCHANNEL
#define SERIAL_CCHANNEL       1
#endif

/** Macros defining the maximum number of Test Suites */
#ifndef TEST_MAX_NUMBER_OF_TEST_SUITES
#define TEST_MAX_NUMBER_OF_TEST_SUITES                  10
#endif

/** Macros defining the maximum number of Test Cases per Test Suite */
#ifndef TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE
#define TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE    12
#endif

#endif /* BCDS_TestingConfig_H */

