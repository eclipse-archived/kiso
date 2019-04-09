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
