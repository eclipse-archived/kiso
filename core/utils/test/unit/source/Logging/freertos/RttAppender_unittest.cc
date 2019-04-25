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

/* setup testing framework ************************************************** */

/* include gtest */
#include <gtest.h>

extern "C"
{
/* setup compile time configuration defines */
#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_LOGGING_APPENDER_RTT

#if BCDS_FEATURE_LOGGING

/* include faked interfaces */
#include "BCDS_Retcode_th.hh"

/* include module under test */
#include "Logging_RttAppender.c"
}

/* define a fake call-back function for the frame processor related tests */

/* create test fixture initializing all variables automatically */

class RttAppender: public testing::Test
{
protected:

    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        ;

    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
    }
};

TEST_F(RttAppender, RttAppenderInit)
{
    /** @testcase{ RttAppender::RttAppenderInit: }
     *
     * API used to test the RttAppenderInit functionality
     */
    EXPECT_EQ(RETCODE_OK, Logging_RttAppender->Init(NULL));
}

TEST_F(RttAppender, RttAppenderWrite)
{
    /** @testcase{ RttAppender::RttAppenderWrite: }
     *
     * API used to test the RttAppenderWrite functionality
     */
    const char message[] = "RttAppenderWriteMessage";
    {
        EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_NULL_POINTER), Logging_RttAppender->Write(NULL,0UL));
    }
    {
        EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_INVALID_PARAM), Logging_RttAppender->Write(message, 0UL));
    }
    {
        EXPECT_EQ(RETCODE_OK, Logging_RttAppender->Write(message, 5UL));
    }
}
#else
}
#endif /* if BCDS_FEATURE_LOGGING */
