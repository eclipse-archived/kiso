/*****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 * ****************************************************************************/

#include "BCDS_TestlingInfo.h"
#define BCDS_MODULE_ID  BCDS_TEST_APP_MODULE_ID_TEST_SECTION

#include "BCDS_Tests.h"
#include "BCDS_TestSection.h"
#include "BCDS_TestSuite.h"
#include "PowerControlTestSuite.h"
#include "SocketTestSuite.h"
#include "NetworkTestSuite.h"
#include "NegativeTestSuite.h"
#include "QueryTestSuite.h"
#include "UdpTestSuite.h"
#include "LoadTestSuite.h"
#include "StressTestSuite.h"
#include "SmokeTestSuite.h"
#include "SmsTestSuite.h"
#include "MultiTaskHandleTest.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include "BCDS_TestSection.h"

/* Cellular Test Section ID Define */

#define TEST_SECTION_ID_CELLULAR UINT32_C(1)

static Retcode_T TestSection_Setup(CCMsg_T *ccmsg);
static Retcode_T TestSection_Teardown(CCMsg_T *ccmsg);

SemaphoreHandle_t SemaphoreHandle; /* Semaphore assigned to a variable of type SemaphoreHandle_t  */

/** The description is in the Public header */
Retcode_T TestSection_Initialize(void)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Tests_Initialize(TEST_SECTION_ID_CELLULAR, TestSection_Setup, TestSection_Teardown);

    if (RETCODE_OK == retcode)
    {
        SemaphoreHandle = xSemaphoreCreateBinary();

        if (SemaphoreHandle != NULL)
        {
            if (RETCODE_OK == retcode)
            {
                retcode = PowerControlTestSuite_Init();
            }

            if (RETCODE_OK == retcode)
            {
                retcode = SocketTestSuite_Init();
            }

            if (RETCODE_OK == retcode)
            {
                retcode = NetworkTestSuite_Init();
            }

            if (RETCODE_OK == retcode)
            {
                retcode = NegativeTestSuite_Init();
            }

            if (RETCODE_OK == retcode)
            {
                retcode = QueryTestSuite_Init();
            }

            if (RETCODE_OK == retcode)
            {
                retcode = UdpTestSuite_Init();
            }

            if (RETCODE_OK == retcode)
            {
                retcode = LoadTestSuite_Init();
            }

            if (RETCODE_OK == retcode)
            {
                retcode = StressTestSuite_Init();
            }

            if (RETCODE_OK == retcode)
            {
                retcode = SmokeTestSuite_Init();
            }
            if (RETCODE_OK == retcode)
			{
				retcode = SmsTestSuite_Init();
			}
            if (RETCODE_OK == retcode)
            {
                retcode = MultiTaskHandleTestSuite_Init();
            }
        }
    }
    return retcode;
}

static Retcode_T TestSection_Setup(CCMsg_T *ccmsg)
{
    BCDS_UNUSED(ccmsg);

    Retcode_T retcode = RETCODE_OK;

    retcode = Cellular_Initialize(&ParseEventCallback);
#ifdef CELLULAR_SMS_TEST

    if(RETCODE_OK == RetStatus)
    {
        retcode = CellularSms_Initialize(&SmsEventCallback);
    }

#endif

    return (retcode);
}


static Retcode_T TestSection_Teardown(CCMsg_T *ccmsg)
{
    BCDS_UNUSED(ccmsg);

    Retcode_T retcode = RETCODE_OK;

    retcode = Cellular_Uninitialize();
#ifdef CELLULAR_SMS_TEST

    if(RETCODE_OK == RetStatus)
    {
        retcode = CellularSms_Deinitialize();
    }

#endif

    return (retcode);
}
