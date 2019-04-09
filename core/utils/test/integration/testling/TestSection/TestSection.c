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

/* own header files */

/* additional interface header files */
#include "BCDS_Tests.h"
#include "BCDS_TestSection.h"
#include "TestSuite_ErrorLogger.h"

#define TEST_SECTION_ID_UTILS 1
#define TEST_SUITE_ID_ERRORLOGGER 1
Retcode_T TestSection_Initialize(void)
{
    Retcode_T RetStatus = RETCODE_OK;

    RetStatus = Tests_Initialize(TEST_SECTION_ID_UTILS, NULL, NULL);
    if (RETCODE_OK == RetStatus)
    {
        RetStatus = TestSuite_ErrorLogger_Initialize(TEST_SUITE_ID_ERRORLOGGER);
    }

    return (RetStatus);
}
