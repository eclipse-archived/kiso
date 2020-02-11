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

#ifndef KISO_TESTING_CONFIG_H
#define KISO_TESTING_CONFIG_H

#ifndef TEST_RUNNER_TASK_PRIO
#define TEST_RUNNER_TASK_PRIO 2
#endif

#ifndef TEST_RUNNER_TASK_STACK_DEPTH
#define TEST_RUNNER_TASK_STACK_DEPTH 1024
#endif

#ifndef TEST_RUNNER_QUEUE_SIZE
#define TEST_RUNNER_QUEUE_SIZE 5U
#endif

#ifndef TEST_MAX_NUMBER_OF_TEST_SUITES
#define TEST_MAX_NUMBER_OF_TEST_SUITES 16
#endif

/** Macros defining the maximum number of Test Cases per Test Suite */
#ifndef TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE
#define TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE 16
#endif

#ifndef CCHANNEL_MAX_NUMBER_OF_TLV_ELEMENTS
#define CCHANNEL_MAX_NUMBER_OF_TLV_ELEMENTS 2
#endif

#ifndef TEST_MAX_PAYLOAD_LENGTH
#define TEST_MAX_PAYLOAD_LENGTH 248U
#endif

#endif /* KISO_TESTING_CONFIG_H */
