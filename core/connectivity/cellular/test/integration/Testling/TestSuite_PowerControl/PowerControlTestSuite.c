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

#include "BCDS_TestlingInfo.h"
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_POWERCONTROL_TEST_SUITE

#include "BCDS_Basics.h"
#include "BCDS_Tests.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "CellularSupport.h"
#include "PowerControlTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_POWERCONTROL_ID

extern SemaphoreHandle_t SemaphoreHandle;
extern Cellular_CallbackEvent_T ParseEvent;

/** The description is in the private header */
static void RunPowerControl(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* Test case */
        retcode = CellularSupport_TurnOn(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "On fail");
            return;
        }
        retcode = CellularSupport_TurnOff(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "Off fail");
            return;
        }
    }

    Tests_SendReport(RETCODE_OK, "PASS");

    return;
}

/** The description is in the private header */
static void RunPowerOnOffDly15STestCase(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* Test case */
        retcode = CellularSupport_TurnOn(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "on fail");
            return;
        }

        vTaskDelay(DELAY_15_SECS / portTICK_RATE_MS);

        retcode = CellularSupport_TurnOff(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "off fail");
            return;
        }

    }

    Tests_SendReport(RETCODE_OK, "PASS");

    return;
}

/** The description is in the private header */
static void RunPowerOnOffDly60STestCase(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T retcode = RETCODE_OK;
    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* Test case */
        retcode = CellularSupport_TurnOn(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "on fail");
            return;
        }

        vTaskDelay(DELAY_60_SECS / portTICK_RATE_MS);

        retcode = CellularSupport_TurnOff(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "off fail");
            return;
        }

    }

    Tests_SendReport(RETCODE_OK, "PASS");

    return;
}

/** The description is in the private header */
static void RunPowerTwiceOffTestCase(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* Test case */
        retcode = CellularSupport_TurnOn(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "on fail");
            return;
        }

        retcode = CellularSupport_TurnOff(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "off fail");
            return;
        }

        retcode = CellularSupport_TurnOff(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "twice off fail");
            return;
        }

    }

    Tests_SendReport(RETCODE_OK, "PASS");

    return;
}

/** The description is in the private header */
static void RunPowerResetTestCase(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(RETCODE_OK, "PASS");
    }
    else
    {
        /* Test case */
        retcode = CellularSupport_Reset(MessagePtr);

        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "reset fail");
            return;
        }
        else
        {
            Tests_SendReport(RETCODE_OK, "PASS");
            return;
        }
    }

    return;
}

/** The description is in the private header */
static void RunPowerOnResetTestCase(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T retcode = RETCODE_OK;
    int8_t ErrorCode;
    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* Test case */
        retcode = CellularSupport_TurnOn(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "on fail");
            return;
        }

        retcode = CellularSupport_Reset(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "reset fail");
            return;
        }

        ErrorCode = xSemaphoreTake(SemaphoreHandle,
                (NETWORK_INIT_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));

        if (pdPASS != ErrorCode)
        {
            Tests_SendReport(retcode, "reset init event fail");
            return;
        }

        if ((CELLULAR_EVENT_INITIALIZED == ParseEvent) || (CELLULAR_EVENT_INITIALIZING == ParseEvent))
        {
            retcode = RETCODE_OK;
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }

        if(RETCODE_OK == retcode)
        {
            retcode = CellularSupport_TurnOff(MessagePtr);
            if (RETCODE_OK != retcode)
            {
                Tests_SendReport(retcode, "TurnOff fail");
                return;
            }
        }
    }

    Tests_SendReport(RETCODE_OK, "PASS");

    return;
}

/** The description is in the public header */
Retcode_T PowerControlTestSuite_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == retcode)
    {
        /* TC 1.1.1 Turn modem ON and OFF */
        retcode = TEST_REGISTER_TC(RunPowerControl);
    }

    if (RETCODE_OK == retcode)
    {
        /* TC 1.1.2 Turn ON, OFF stability (time delay-shorter duration) */
        retcode = TEST_REGISTER_TC(RunPowerOnOffDly15STestCase);
    }

    if (RETCODE_OK == retcode)
    {
        /* TC 1.1.3 Turn ON, OFF stability (time delay-longer duration) */
        retcode = TEST_REGISTER_TC(RunPowerOnOffDly60STestCase);
    }

    if (RETCODE_OK == retcode)
    {
        /* TC 1.1.4 Turn modem OFF twice */
        retcode = TEST_REGISTER_TC(RunPowerTwiceOffTestCase);
    }

    if (RETCODE_OK == retcode)
    {
        /* TC 1.1.5 Reset modem */
        retcode = TEST_REGISTER_TC(RunPowerResetTestCase);
    }

    if (RETCODE_OK == retcode)
    {
        /* TC 1.1.6 Reset modem, after Modem ON */
        retcode = TEST_REGISTER_TC(RunPowerOnResetTestCase);
    }

    return retcode;
}
