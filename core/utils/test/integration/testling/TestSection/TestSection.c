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
