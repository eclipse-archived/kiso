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

#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID 0

#include "BCDS_Tests.h"
#include "BCDS_Retcode.h"
#include "BCDS_CmdProcessor.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "BCDS_BSP_Board.h"

#include "BSP_BoardShared.h"
#include "BSP_BoardType.h"
#include "BSP_BoardSettings.h"

/*
 * Test Defs to remove in later use
 *
 */

#define SLEEP_PERIOD_IN_S    (UINT32_C(2)) /* Sleep period in s */
#define EXPECTED_RTC_FREQ    (UINT32_C(32768))
#define EXPECTED_BURTC_FREQ  (UINT32_C(1000)) /* Expected ms Tick */

/* Interface to Test */

#include "BCDS_MCU_Sleep.h"

#include "BCDS_MCU_Timer.h"

#include "BCDS_MCU_Timer_Handle.h"

#include "TestSuite_Sleep.h"

#include <stdlib.h>

#if !(BCDS_FEATURE_SLEEP)
#error "Feature Sleep has not been activated"
#endif

#if !(BCDS_FEATURE_TIMER)
#error "Feature Timer not activated"
#elif !(BCDS_FEATURE_EFM32_RTC)
#error "RTC Not activated"
#elif !(BCDS_FEATURE_EFM32_BURTC)
#error "BURTC Not activated"
#elif !(BCDS_FEATURE_EFM32_LETIMER)
#error "LETIMER Not activated"
#elif !(BCDS_FEATURE_EFM32_TIMER)
#error "TIMER Not activated"
#endif

enum
{
    TEST_CASE_SLEEP_EM3_ID = 1,
    TEST_CASE_SLEEP_EM2_ID,
};

/*
 * enumeration of custom return codes
 */
enum
{
    RETCODE_MCU_SLEEP_TEST = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_MCU_SLEEP_ERROR, /**< Generic Error code*/
    RETCODE_MCU_SLEEP_WRONG_COUNTVALUE,
};

#define TEST_RTC_COMPARE_VAL0   UINT32_C(1024)
#define TEST_RTC_COMPARE_VAL1   UINT32_C(2048)

#define TEST_RTC_COMPARE_VAL2   UINT32_C(2048)
#define TEST_RTC_COMPARE_VAL3   UINT32_C(2048)

/* ************************************************************************** */
/* * TESTSUITE 4 DEFINITIONS ************************************************ */
/* ************************************************************************** */
static Retcode_T TestSuite_Sleep_Setup(CCMsg_T* ccmsg);
static Retcode_T TestSuite_Sleep_Teardown(CCMsg_T* ccmsg);

/* ************************************************************************** */
/* * TESTCASES 4 DEFINITIONS ************************************************ */
/* ************************************************************************** */
static void TestCase_SleepToEm3_Run(CCMsg_T* ccmsg); /* Go to EM3 Sleep mode */
static void TestCase_SleepToEm2_Run(CCMsg_T* ccmsg); /* Go to EM2 Sleep mode */

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

/* Application Level Callback */
static Retcode_T TESTAPP_BurtcPreSleepCallback(uint32_t mode);
static Retcode_T TESTAPP_BurtcPostSleepCallback(uint32_t mode);

static volatile bool CompareMatch0 = false;
static volatile bool CompareMatch1 = false;
static volatile bool OverFlow = false;

/* ************************************************************************** */
/* * TESTCASE IMPLEMENTATIONS *********************************************** */
/* ************************************************************************** */

Retcode_T TestSuite_Sleep_initialize(uint8_t testSuiteId)
{
    Retcode_T retcode = RETCODE_OK;

    /* Register TestSuite 4*/
    retcode = Tests_RegisterTestSuite(testSuiteId, TestSuite_Sleep_Setup, TestSuite_Sleep_Teardown);

    if (RETCODE_OK == retcode)
    { /* Register Test Cases */
        retcode = Tests_RegisterTestCase(testSuiteId, TEST_CASE_SLEEP_EM3_ID, NULL, TestCase_SleepToEm3_Run, NULL);
    }

    if ( RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(testSuiteId, TEST_CASE_SLEEP_EM2_ID, NULL, TestCase_SleepToEm2_Run, NULL);
    }

    return retcode;
}

static Retcode_T TestSuite_Sleep_Setup(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);

    CompareMatch0 = false;
    CompareMatch1 = false;
    OverFlow = false;

    return RETCODE_OK;
}

static Retcode_T TestSuite_Sleep_Teardown(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Rc = RETCODE_OK;

    return Rc;
}

/**
 * Sleep to EM3.
 * Confirmation: BURTC ran, RTC did not!
 */
static void TestCase_SleepToEm3_Run(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);

    uint32_t sleepLevel = UINT32_C(0);
    uint32_t burtcFreq = UINT32_C(0);
    uint32_t rtcFreq = UINT32_C(0);
    uint32_t burtcCountValue = UINT32_C(0);
    uint32_t rtcCountValue = UINT32_C(0);

    Retcode_T rc = RETCODE_OK;

    Timer_T rtc = (Timer_T) MCU_Timer_GetRtcHandle(); /*  Get the RTC Handle */
    Timer_T burtc = (Timer_T) MCU_Timer_GetBurtcHandle(); /* Get the BURTC Handle */

    rc = MCU_Timer_Initialize(rtc, NULL);

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Initialize(burtc, NULL);
    }

    if (RETCODE_OK == rc)
    { /* Initialize Sleep */
        rc = MCU_Sleep_Initialize();
    }

    if (RETCODE_OK == rc)
    {
        /* Validate BURTC Frequency */
        burtcFreq = BURTC_ClockFreqGet();
        rtcFreq = CMU_ClockFreqGet(cmuClock_RTC);

        if ((EXPECTED_BURTC_FREQ != burtcFreq) || (EXPECTED_RTC_FREQ != rtcFreq))
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }

    }

    /* Validate Lowest Sleep Level */
    if (RETCODE_OK == rc)
    {
        sleepLevel = MCU_Sleep_GetLowestSleepMode();

        if (UINT32_C(3) != sleepLevel) /* Will sleep through EM3*/
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_SetCompareValue(burtc, 0, (SLEEP_PERIOD_IN_S * EXPECTED_BURTC_FREQ)); /* Sleep for 1s*/
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Start(burtc);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Start(rtc);
    }

    /* Go to sleep */
    if (RETCODE_OK == rc)
    {
        BSP_Board_DisableSysTick();

        TESTAPP_BurtcPreSleepCallback(sleepLevel);

        MCU_Sleep_EnterSleep(sleepLevel);

        TESTAPP_BurtcPostSleepCallback(sleepLevel);

        BSP_Board_EnableSysTick();

    }

    rc = MCU_Timer_GetCountValue(burtc, &burtcCountValue);
    rc = MCU_Timer_GetCountValue(rtc, &rtcCountValue);

    if (RETCODE_OK == rc)
    {
        if ((SLEEP_PERIOD_IN_S * EXPECTED_BURTC_FREQ) > burtcCountValue)
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == rc)
    {
        /* Check that the RTC was not active during the period */
        if (UINT32_C(100) < rtcCountValue)
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Stop(burtc);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Stop(rtc);
    }

    /* Reporting phase */
    if (RETCODE_OK == rc)
    {
        Tests_SendReport(rc, "EM3 Sleep success");
    }
    else
    {
        Tests_SendReport(rc, "EM3 Sleep failure");
    }
}

/**
 * Sleep to EM2.
 * Confirmation: BURTC ran, RTC did also!
 */
static void TestCase_SleepToEm2_Run(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);

    uint32_t sleepLevel = UINT32_C(0);
    uint32_t burtcFreq = UINT32_C(0);
    uint32_t rtcFreq = UINT32_C(0);
    uint32_t burtcCountValue = UINT32_C(0);
    uint32_t rtcCountValue = UINT32_C(0);

    Retcode_T rc = RETCODE_OK;

    Timer_T rtc = (Timer_T) MCU_Timer_GetRtcHandle(); /*  Get the RTC Handle */
    Timer_T burtc = (Timer_T) MCU_Timer_GetBurtcHandle(); /* Get the BURTC Handle */

    rc = MCU_Timer_Initialize(rtc, NULL);

    /* Disable RTC Compare Match Settings */
    RTC_IntDisable((RTC_IEN_COMP0 | RTC_IEN_COMP1));

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Initialize(burtc, NULL);
    }

    if (RETCODE_OK == rc)
    { /* Initialize Sleep */
        rc = MCU_Sleep_Initialize();
    }

    if (RETCODE_OK == rc)
    {
        /* Validate BURTC Frequency */
        burtcFreq = BURTC_ClockFreqGet();
        rtcFreq = CMU_ClockFreqGet(cmuClock_RTC);

        if ((EXPECTED_BURTC_FREQ != burtcFreq) || (EXPECTED_RTC_FREQ != rtcFreq))
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }

    }

    if (RETCODE_OK == rc)
    {
        /* Block EM3  */
        rc = MCU_Sleep_BlockSleepMode(UINT32_C(3));
    }

    /* Validate Lowest Sleep Level */
    if (RETCODE_OK == rc)
    {
        sleepLevel = MCU_Sleep_GetLowestSleepMode();

        if (UINT32_C(2) != sleepLevel) /* Will sleep through EM2*/
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_SetCompareValue(burtc, 0, (SLEEP_PERIOD_IN_S * EXPECTED_BURTC_FREQ)); /* Sleep for 1s*/
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Start(burtc);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Start(rtc);
    }

    /* Go to sleep */
    if (RETCODE_OK == rc)
    {
        INT_Disable();

        BSP_Board_DisableSysTick();

        TESTAPP_BurtcPreSleepCallback(sleepLevel);

        MCU_Sleep_EnterSleep(sleepLevel);

        TESTAPP_BurtcPostSleepCallback(sleepLevel);

        BSP_Board_EnableSysTick();

        INT_Enable();
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_GetCountValue(burtc, &burtcCountValue);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_GetCountValue(rtc, &rtcCountValue);
    }

    if (RETCODE_OK == rc)
    {
        if ((SLEEP_PERIOD_IN_S * EXPECTED_BURTC_FREQ) > burtcCountValue)
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == rc)
    {
        /* Check that the RTC was active during the period */
        if (burtcCountValue >= rtcCountValue)
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Stop(burtc);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Stop(rtc);
    }

    /* Reporting phase */
    if (RETCODE_OK == rc)
    {
        Tests_SendReport(rc, "EM2 Sleep Success");
    }
    else
    {
        Tests_SendReport(rc, "EM2 Sleep Failure");
    }
}

/**
 * This preSleep Callback disables USB and the Systick in order to sleep
 */
static Retcode_T TESTAPP_BurtcPreSleepCallback(uint32_t mode)
{
    BCDS_UNUSED(mode);

    NVIC_DisableIRQ(USB_IRQn);
    NVIC_ClearPendingIRQ(USB_IRQn);

    return RETCODE_OK;
}

/**
 * This postSleep Callback restores the USB irq and the SysTick
 */
static Retcode_T TESTAPP_BurtcPostSleepCallback(uint32_t mode)
{
    BCDS_UNUSED(mode);

    NVIC_EnableIRQ(USB_IRQn);

    return RETCODE_OK;
}

