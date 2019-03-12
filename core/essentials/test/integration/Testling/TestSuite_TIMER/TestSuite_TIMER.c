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

#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID 0

#include "BCDS_Tests.h"
#include "BCDS_Retcode.h"
#include "BCDS_CmdProcessor.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "BSP_BoardSettings.h"
#include "BSP_BoardShared.h"
#include "BSP_BoardType.h"

/* Interface to Test */
#include "BCDS_MCU_Timer.h"

/* BSP provides the handles and initialization routines for MCU RTC timers*/
#include "BCDS_MCU_Timer_Handle.h"

#include "TestSuite_TIMER.h"

#include <stdlib.h>

enum
{
    TEST_CASE_TIMER_1_ID = 1,
    TEST_CASE_TIMER_2_ID,
    TEST_CASE_TIMER_3_ID,
};

/* Custom Return codes */
enum
{
    RETCODE_MCU_TIMER_TEST = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_MCU_TIMER_ERROR, /**< Generic Error code*/
    RETCODE_MCU_TIMER_INIT_FAIL,
    RETCODE_MCU_TIMER_NOT_ZERO,
    RETCODE_MCU_TIMER_WRONG_VALUE
};

/* First two RTC compare values */
#define TEST_RTC_COMPARE_VAL0   UINT32_C(2048)
#define TEST_RTC_COMPARE_VAL1   UINT32_C(1024)

#define TEST_RTC_COMPARE_VAL3   UINT32_C(2048)

#define TEST_BURTC_COMPARE_VAL0 UINT32_C(1024)

#if(TEST_RTC_COMPARE_VAL0 < TEST_RTC_COMPARE_VAL1)
#error "Compare Match Value 0 should be higher than compare match value 1"
#endif


/* ************************************************************************** */
/* * TESTSUITE TIMER DEFINITIONS ******************************************** */
/* ************************************************************************** */
static Retcode_T TestSuite_Timer_Setup(CCMsg_T* ccmsg);
static Retcode_T TestSuite_Timer_Teardown(CCMsg_T* ccmsg);

/* ************************************************************************** */
/* * TESTCASE TIMER DEFINITIONS ********************************************* */
/* ************************************************************************** */

/**
 * RTC Simple and Sequential Match Run
 *
 * @details This test case initializes the MCU RTC timer and sets it to
 *      produce a compare matches at two different counts.
 *
 *      The test waits in a blocking mode until the ISR callback modifies the
 *      volatile variable indicating that the compare match interrupt for
 *      channel 0 was triggered.
 *
 *      After this, it will reenter the blocking mode as before until the
 *      compare match interrupt for channel 1 is triggered
 *
 * @param ccmsg : Currently unused. May be null
 *
 */
static void TestCase_RtcSimpleCompareMatch_Run(CCMsg_T* ccmsg);

/**
 * BURTC Compare Match Run
 *
 * @details This test case initializes the MCU BURTC timer and sets it to
 *      produce a compare match at a certain value.
 *
 *      The test waits in a blocking mode until the ISR callback modifies the
 *      volatile variable indicating that the compare match interrupt for
 *      channel 0 was triggered.
 *
 * @param ccmsg : Currently unused. May be null
 *
 */
static void TestCase_RtcSimultaneousCompareMatch_Run(CCMsg_T* ccmsg);

/**
 * BURTC Compare Match Run
 *
 * @details This test case initializes the MCU BURTC timer and sets it to
 *      produce a compare match at a certain value.
 *
 *      The test waits in a blocking mode until the ISR callback modifies the
 *      volatile variable indicating that the compare match interrupt for
 *      channel 0 was triggered.
 *
 * @param ccmsg : Currently unused. May be null
 *
 */
static void TestCase_BurtcCompareMatch_Run(CCMsg_T* ccmsg);

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

/* Application Level Callback */
static void TEST_TimerCallback(Timer_T timer, struct MCU_Timer_Event_S event);

/* Variables used to communicate ISR events to the test application */
static volatile bool CompareMatch0 = false;
static volatile bool CompareMatch1 = false;
static volatile bool OverFlow = false;

/* ************************************************************************** */
/* * TESTCASE IMPLEMENTATIONS *********************************************** */
/* ************************************************************************** */

Retcode_T TestSuite_Timer_initialize(uint8_t testSuiteId)
{
    Retcode_T retcode = RETCODE_OK;

    /* Register TestSuite 4*/
    retcode = Tests_RegisterTestSuite(testSuiteId, TestSuite_Timer_Setup, TestSuite_Timer_Teardown);

    if (RETCODE_OK == retcode)
    { /* Register Test Cases */
        retcode = Tests_RegisterTestCase(testSuiteId, TEST_CASE_TIMER_1_ID, NULL, TestCase_RtcSimpleCompareMatch_Run, NULL);
    }

    if ( RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(testSuiteId, TEST_CASE_TIMER_2_ID, NULL, TestCase_RtcSimultaneousCompareMatch_Run, NULL);
    }

    if ( RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(testSuiteId, TEST_CASE_TIMER_3_ID, NULL, TestCase_BurtcCompareMatch_Run, NULL);
    }

    return retcode;
}

static Retcode_T TestSuite_Timer_Setup(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);

    /*Reset global volatile variables */
    CompareMatch0 = false;
    CompareMatch1 = false;
    OverFlow = false;

    return RETCODE_OK;
}

static Retcode_T TestSuite_Timer_Teardown(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Rc = RETCODE_OK;

    return Rc;
}

static void TestCase_RtcSimpleCompareMatch_Run(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);

    Retcode_T rc = RETCODE_OK;

    Timer_T timer = (Timer_T) MCU_Timer_GetRtcHandle(); /*  Get the RTC Handle */

    rc = MCU_Timer_Initialize(timer, TEST_TimerCallback);

    uint32_t countValue = UINT32_C(0);
    uint32_t swCounter = UINT32_C(0);

    /* Reset Compare Match Registers */
    CompareMatch1 = false;
    CompareMatch0 = false;

    if (RETCODE_OK == rc)
    { /* Initialization was a success */
        rc = MCU_Timer_GetCountValue(timer, &countValue);
    }

    if (RETCODE_OK == rc)
    {
        if (UINT32_C(0) != countValue)
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_TIMER_INIT_FAIL);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_SetCompareValue(timer, 0, TEST_RTC_COMPARE_VAL0);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_SetCompareValue(timer, 1, TEST_RTC_COMPARE_VAL1);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Start(timer);
    }

    if (RETCODE_OK == rc)
    {
        /* Wait for a Compare Match on Channel 1 */
        for (swCounter = 0; (false == CompareMatch1); swCounter++)
            ;

        /* Wait for a Compare Match on Channel 0 */
        for (swCounter = 0; (false == CompareMatch0); swCounter++)
            ;

    }

    /* Verify Count Value */
    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Stop(timer);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_GetCountValue(timer, &countValue);
    }

    if (RETCODE_OK == rc)
    {
        if (UINT32_C(0) != countValue) /* Count value is 0 */
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_TIMER_NOT_ZERO);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Deinitialize(timer);
    }

    /* Reporting phase */
    if (RETCODE_OK == rc)
    {
        Tests_SendReport(rc, "Timer Works");
    }
    else
    {
        Tests_SendReport(rc, "Timer Error");
    }

}

static void TestCase_RtcSimultaneousCompareMatch_Run(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);

    Retcode_T rc = RETCODE_OK;

    Timer_T timer = (Timer_T) MCU_Timer_GetRtcHandle(); /*  Get the RTC Handle */

    rc = MCU_Timer_Initialize(timer, TEST_TimerCallback);

    uint32_t countValue = UINT32_C(0);
    uint32_t swCounter = UINT32_C(0);

    /* Reset Compare Match Registers */
    CompareMatch1 = false;
    CompareMatch0 = false;

    if (RETCODE_OK == rc)
    { /* Initialization was a success */
        rc = MCU_Timer_GetCountValue(timer, &countValue);
    }

    if (RETCODE_OK == rc)
    {
        if (UINT32_C(0) != countValue)
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_TIMER_INIT_FAIL);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_SetCompareValue(timer, 0, TEST_RTC_COMPARE_VAL3);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_SetCompareValue(timer, 1, TEST_RTC_COMPARE_VAL3);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Start(timer);
    }

    if (RETCODE_OK == rc)
    {
        /* Wait for a simultaneous compare match on channels 0 and 1 */
        for (swCounter = UINT32_C(0); !((true == CompareMatch1) && (true == CompareMatch0)); swCounter++)
            ;
    }

    /* Verify Count Value */
    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Stop(timer);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_GetCountValue(timer, &countValue);
    }

    if (RETCODE_OK == rc)
    {
        if (UINT32_C(0) != countValue) /* Count value is 0 */
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_TIMER_NOT_ZERO);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Deinitialize(timer);
    }

    /* Reporting phase */
    if (RETCODE_OK == rc)
    {
        Tests_SendReport(rc, "Timer Works");
    }
    else
    {
        Tests_SendReport(rc, "Timer Error");
    }
}

static void TestCase_BurtcCompareMatch_Run(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
#if BCDS_FEATURE_EFM32_BURTC
    Timer_T timer = (Timer_T) MCU_Timer_GetBurtcHandle();

    Retcode_T rc = RETCODE_OK;
    uint32_t countValue = 0;
    uint32_t swCounter;

    CompareMatch1 = false;
    CompareMatch0 = false;
    OverFlow = false;

    rc = MCU_Timer_Initialize(timer, TEST_TimerCallback);

    /* Reset the counter value */
    BURTC_CounterReset(); /* Just in case */

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_GetCountValue(timer, &countValue);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_SetCompareValue(timer, 0, TEST_BURTC_COMPARE_VAL0);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Start(timer);
    }

    if (RETCODE_OK == rc)
    {
        /* Wait for a Compare Match on Channel 0 */
        for (swCounter = 0; (false == CompareMatch0); swCounter++)
            ;
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Stop(timer);

    }

    rc = MCU_Timer_GetCountValue(timer, &countValue); /* Read Behavior after Stopping counter*/

    if (RETCODE_OK == rc)
    {
        if (UINT32_C(0) != countValue)
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_TIMER_NOT_ZERO);
        }
    }

    /* Reporting phase */
    if (RETCODE_OK == rc)
    {
        Tests_SendReport(rc, "Timer Works");
    }
    else
    {
        Tests_SendReport(rc, "Timer Error");
    }
#else
    Tests_SendReport(RETCODE_NOT_SUPPORTED, "BURTC is disabled, hence this test does not make sense\n");
#endif
}

/* Application Level Test Timer Callback */
static void TEST_TimerCallback(Timer_T timer, struct MCU_Timer_Event_S event)
{
    (void) timer;

    if (1 == event.CompareMatch)
    {
        if ((1 << 0) & event.CompareChannel)
        {
            CompareMatch0 = true;
        }
        else
        {
            CompareMatch0 = false;
        }

        if ((1 << 1) & event.CompareChannel)
        {
            CompareMatch1 = true;
        }
        else
        {
            CompareMatch1 = false;
        }
    }
    else
    {
        CompareMatch1 = false;
        CompareMatch0 = false;
    }

    if (1 == event.Overflow)
    {
        OverFlow = true;
    }
    else
    {
        OverFlow = false;
    }

    return;
}

