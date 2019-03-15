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
 * @file Network test suite will check network initialization,Configuration
 * and GPRS connection functionality.
 *
 * For more details,refer "CELLULAR_Integration_Test_Spec.md" file in specification folder
 *
 */

#ifndef NetworkTestSuite_H
#define NetworkTestSuite_H

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

/* Put the type and macro definitions here */
#define DELAY_120_SECS              UINT32_C(120000)
#define NETWORK_SEQUENCE_COUNT      UINT32_C(5)
#define NWINIT_TWICE_TESTCASE_COUNT UINT8_C(2)

/* Put the function declarations here */
Retcode_T NetworkTestSuite_Init(void);


#endif /* NetworkTestSuite_H */
