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
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_LOGGING_APPENDER_SWO

#if BCDS_FEATURE_LOGGING

/* include faked interfaces */
#include "BCDS_Retcode_th.hh"
/* The core_cm4.h is not properly masked. Banning the redundant definition guard. */
#define __CORE_CM4_H_DEPENDANT
#include "core_cm4_th.hh"
#include "stm32l4xx_th.hh"

/* The core_cm4.h is not properly mocked. Defining the necessary fake function. */
FAKE_VALUE_FUNC(uint32_t,ITM_SendChar,uint32_t )

/* include module under test */
#include "SwoAppender.c"
}

/* define a fake call-back function for the frame processor related tests */

/* create test fixture initializing all variables automatically */

class SwoAppender: public testing::Test
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

TEST_F(SwoAppender, SwoAppenderInit)
{
    /** @testcase{ SwoAppender::SwoAppenderInit: }
     *
     * API used to test the SwoAppenderInit functionality
     */
    EXPECT_EQ(RETCODE_OK, Logging_SwoAppender->Init(NULL));
}

TEST_F(SwoAppender, SwoAppenderWrite)
{
    /** @testcase{ SwoAppender::SwoAppenderWrite: }
     *
     * API used to test the SwoAppenderWrite functionality
     */
    const char message[] = "SwoAppenderWriteMessage";
    {
        EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_NULL_POINTER), Logging_SwoAppender->Write(NULL,0UL));
    }
    {
        EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_INVALID_PARAM), Logging_SwoAppender->Write(message, 0UL));
    }
    {
        EXPECT_EQ(RETCODE_OK, Logging_SwoAppender->Write(message, 5UL));
    }
}
#else
}
#endif /* if BCDS_FEATURE_LOGGING */
