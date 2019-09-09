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

/* include gtest interface */
#include <gtest.h>

extern "C"
{
#include "Kiso_HAL_th.hh"
#include "em_device_th.hh"

/* Silence Module ID not-defined configuration */
#define KISO_MODULE_ID 0

#if KISO_FEATURE_TIMER

// TODO: These come from em_device.h - check if needed
// #define RTC_PRESENT
// #define RTC_COUNT 1

// #define TIMER_PRESENT
// #define TIMER_COUNT 0

// #define LETIMER_PRESENT
// #define LETIMER_COUNT 0

#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"


#include "em_cmu_th.hh"
#include "em_rtc_th.hh"
#include "em_burtc_th.hh"
#include "em_timer_th.hh"
#include "em_letimer_th.hh"


#ifndef _RTC_CNT_CNT_MASK
#define _RTC_CNT_CNT_MASK 0xFFFFFFUL
#endif

#ifndef _RTC_CNT_CNT_SHIFT
#define _RTC_CNT_CNT_SHIFT 0
#endif

BURTC_TypeDef BURTC_HW;
BURTC_TypeDef * BURTC = &BURTC_HW;

/**
 * Module under test
 */
#include "Timer.c"

FAKE_VOID_FUNC(TimerTestCallback, Timer_T, struct MCU_Timer_Event_S);

/* ************************************************************************** */
/* * BURTC DRIVER *********************************************************** */
/* ************************************************************************** */

/* Initialize and Deinitialize Structs */
const BURTC_Init_TypeDef BurtcInitStruct = BURTC_INIT_DEFAULT;
const BURTC_Init_TypeDef BurtcDeInitStruct = BURTC_INIT_DEFAULT;

/* Default Generic Initializer */
const uint32_t GenericInitStruct = UINT32_C(0);
const uint32_t GenericDeInitStruct = UINT32_C(0);



uint32_t GenericHW = UINT32_C(0);

/* BURTC Driver Struct */
struct MCU_Timer_Handle_S BSP_BURTC_Handle = {
        &BURTC_HW, /* BURTC HW Handle */
        EM_BURTC, /* Timer Type */
        &BurtcInitStruct, /* Initialize Struct */
        &BurtcDeInitStruct, /* Deinitialize Struct */
        MCU_TIMER_STATUS_INVALID, /* Init Timer */
        NULL /* Default Callback */
};

/* Generic Driver Struct */
struct MCU_Timer_Handle_S BSP_Generic_Handle = {
        &GenericHW, /* BURTC HW Handle */
        EM_RTC, /* Timer Type */
        &GenericInitStruct, /* Initialize Struct */
        &GenericDeInitStruct, /* Deinitialize Struct */
        MCU_TIMER_STATUS_INVALID, /* Init Timer */
        NULL /* Default Callback */
};

struct MCU_Timer_Handle_S * BSP_BURTC_HandleCopy;
struct MCU_Timer_Handle_S * BSP_Generic_HandleCopy;


}

/* end of global scope symbol and fake definitions section */

class HAL_MCU_Timer_Test: public testing::Test
{
protected:

virtual void SetUp()
{
    FFF_RESET_HISTORY()

    /* BURTC Fakes */
    RESET_FAKE (BURTC_CompareGet);
    RESET_FAKE (BURTC_CompareSet);
    RESET_FAKE (BURTC_CounterGet);
    RESET_FAKE (BURTC_Enable);
    RESET_FAKE (BURTC_Init);
    RESET_FAKE (BURTC_IntGet);

    RESET_FAKE (RTC_CompareGet);
    RESET_FAKE (RTC_CompareSet);
    RESET_FAKE (RTC_CounterGet);
    RESET_FAKE (RTC_Enable);
    RESET_FAKE (RTC_Init);
    RESET_FAKE (RTC_IntGet);

    RESET_FAKE (TIMER_CaptureGet);
    RESET_FAKE (TIMER_CompareSet);
    RESET_FAKE (TIMER_CounterGet);
    RESET_FAKE (TIMER_Enable);
    RESET_FAKE (TIMER_Init);
    RESET_FAKE (TIMER_IntGet);

    RESET_FAKE (LETIMER_CompareGet);
    RESET_FAKE (LETIMER_CompareSet);
    RESET_FAKE (LETIMER_CounterGet);
    RESET_FAKE (LETIMER_Enable);
    RESET_FAKE (LETIMER_Init);
    RESET_FAKE (LETIMER_IntGet);

    /* Callback Fakes */
    RESET_FAKE (TimerTestCallback);
    RESET_FAKE(Retcode_RaiseError);

    /* Copy the default handle into memory */
    BSP_BURTC_HandleCopy = (struct MCU_Timer_Handle_S *) malloc(sizeof(struct MCU_Timer_Handle_S));
    BSP_Generic_HandleCopy = (struct MCU_Timer_Handle_S *) malloc(sizeof(struct MCU_Timer_Handle_S));


    memcpy((void*) BSP_BURTC_HandleCopy,   (void *) &BSP_BURTC_Handle, sizeof(BSP_BURTC_Handle));
    memcpy((void*) BSP_Generic_HandleCopy, (void *) &BSP_Generic_Handle, sizeof(BSP_Generic_Handle));

}

virtual void TearDown()
{
    /* Restore the default handles */

    memcpy((void *) &BSP_BURTC_Handle,   (void*) BSP_BURTC_HandleCopy, sizeof(BSP_BURTC_Handle));
    memcpy((void*) &BSP_Generic_Handle, (void *) BSP_Generic_HandleCopy, sizeof(BSP_Generic_Handle));

    free((void *) BSP_BURTC_HandleCopy);
    free((void *) BSP_Generic_HandleCopy);
}
};

#define TIMER_TEST_INITIALIZE 1
#define TIMER_TEST_DEINITIALIZE 1
#define TIMER_TEST_SET_COMPARE 1
#define TIMER_TEST_GET_COMPARE 1
#define TIMER_TEST_GET_COUNT 1
#define TIMER_TEST_GET_MAXCOUNT 1
#define TIMER_TEST_START 1
#define TIMER_TEST_STOP 1

TEST_F(HAL_MCU_Timer_Test, RetcodeAndModuleIdIntegrity)
{
    ASSERT_EQ(KISO_ESSENTIALS_MODULE_ID_TIMER, KISO_MODULE_ID );
}



/* ************************************************************************** */
/* * TIMER INITIALIZE TESTS ************************************************* */
/* ************************************************************************** */
#if (TIMER_TEST_INITIALIZE)
TEST_F(HAL_MCU_Timer_Test, TimerInitialize_NULL)
{
    Timer_T timer = NULL;

    Retcode_T rc;

    rc =  MCU_Timer_Initialize(timer, TimerTestCallback);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}

/* This Test corrupts the HW Handle */
TEST_F(HAL_MCU_Timer_Test, TimerInitialize_HW_NULL)
{
    Timer_T timer = (Timer_T)&BSP_BURTC_Handle;
    void * hwHandle = (void *)&(BSP_BURTC_Handle.HwHandle);

    /* Corrupt HW Handle */
    memset(hwHandle, 0, sizeof(BSP_BURTC_Handle.HwHandle));

    Retcode_T rc;

    rc =  MCU_Timer_Initialize(timer, TimerTestCallback);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOHWHANDLE), rc);
}

/* This Test corrupts the InitStruct Handle */
TEST_F(HAL_MCU_Timer_Test, TimerInitialize_Init_NULL)
{
    Timer_T timer = (Timer_T)&BSP_BURTC_Handle;

    void * hwHandle = (void *)&(BSP_BURTC_Handle.InitHandle);

    /* Corrupt HW Handle */
    memset(hwHandle, 0, sizeof(BSP_BURTC_Handle.InitHandle));

    Retcode_T rc;

    rc =  MCU_Timer_Initialize(timer, TimerTestCallback);

    ASSERT_EQ( RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOINIT), rc);
}

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerInitialize_BURTC)
{
    Timer_T timer = (Timer_T)&BSP_BURTC_Handle;

    Retcode_T rc;

    ASSERT_EQ(NULL, BSP_BURTC_Handle.Callback);

    rc = MCU_Timer_Initialize(timer, TimerTestCallback);
    ASSERT_EQ(RETCODE_OK, rc);

    /* Assert that BURTC Init was called */
    ASSERT_EQ(1, BURTC_Init_fake.call_count);

    /* Assert that the Timer Test Callback was registered */
    ASSERT_EQ(&TimerTestCallback, BSP_BURTC_Handle.Callback);
}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerInitialize_RTC)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt HW Handle */
    *type = EM_RTC;

    Retcode_T rc;

    rc = MCU_Timer_Initialize(timer, TimerTestCallback);

    ASSERT_EQ(RETCODE_OK, rc);

    /* Assert that BURTC Init was called */
    ASSERT_EQ(1, RTC_Init_fake.call_count);

    /* Assert that the Timer Test Callback was registered */
    ASSERT_EQ(&TimerTestCallback, BSP_Generic_Handle.Callback);
}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerInitialize_TIMER)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt HW Handle */
    *type = EM_TIMER;

    Retcode_T rc;

    rc = MCU_Timer_Initialize(timer, TimerTestCallback);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
/*    ASSERT_EQ(RETCODE_OK, rc); */

    /* Assert that BURTC Init was called */
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
/*    ASSERT_EQ(1, TIMER_Init_fake.call_count); */

    /* Assert that the Timer Test Callback was not registered */
    ASSERT_EQ(NULL, BSP_Generic_Handle.Callback);
}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */

#if (KISO_FEATURE_EFM32_LETIMER)
TEST_F(HAL_MCU_Timer_Test, TimerInitialize_LETIMER)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt HW Handle */
    *type = EM_LETIMER;

    Retcode_T rc;

    rc = MCU_Timer_Initialize(timer, TimerTestCallback);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
/*    ASSERT_EQ(RETCODE_OK, rc); */

    /* Assert that BURTC Init was called */
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
/*    ASSERT_EQ(1, TIMER_Init_fake.call_count); */

    /* Assert that the Timer Test Callback was not registered */
    ASSERT_EQ(NULL, BSP_Generic_Handle.Callback);
}
#endif /* KISO_FEATURE_EFM32_LETIMER */

TEST_F(HAL_MCU_Timer_Test, TimerInitialize_INVALID_PARAM)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt type */
    *type = (enum Hal_MCU_Timer_E)(5);

    Retcode_T rc;

    rc = MCU_Timer_Initialize(timer, TimerTestCallback);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    /* Assert that BURTC Init was called */
    ASSERT_EQ(0, RTC_Init_fake.call_count);
    ASSERT_EQ(0, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);

    /* Assert that the Timer Test Callback was not registered */
    ASSERT_EQ(NULL, BSP_Generic_Handle.Callback);
}
#endif


/* ************************************************************************** */
/* * TIMER DEINITIALIZE TESTS *********************************************** */
/* ************************************************************************** */
#if TIMER_TEST_DEINITIALIZE
TEST_F(HAL_MCU_Timer_Test, TimerDeInitialize_NULL)
{
    Timer_T timer = NULL;

    Retcode_T rc;

    rc =  MCU_Timer_Deinitialize(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

    ASSERT_EQ(0, RTC_Init_fake.call_count);
    ASSERT_EQ(0, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
}


TEST_F(HAL_MCU_Timer_Test, TimerDeInitialize_NULL_HW)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt HW Handle */
    void * hwHandle = (void *)&(BSP_Generic_Handle.HwHandle);
    memset(hwHandle, 0, sizeof(BSP_Generic_Handle.HwHandle));

    Retcode_T rc;

    rc =  MCU_Timer_Deinitialize(timer);

    /* HW HANDLE INVALID */
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOHWHANDLE), rc);

    ASSERT_EQ(0, RTC_Init_fake.call_count);
    ASSERT_EQ(0, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
}

TEST_F(HAL_MCU_Timer_Test, TimerDeInitialize_NoDeinitHandle)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt DeInit Handle */
    void * initHandle = (void *)&(BSP_Generic_Handle.DeInitHandle);
    memset(initHandle, 0, sizeof(BSP_Generic_Handle.DeInitHandle));

    Retcode_T rc;

    rc =  MCU_Timer_Deinitialize(timer);

    /* HW HANDLE INVALID */
    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0, RTC_Init_fake.call_count);
    ASSERT_EQ(0, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
}

#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerDeInitialize_RTC)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt DeInit Handle */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt type */
    *type = (enum Hal_MCU_Timer_E)(EM_RTC);

    Retcode_T rc;

    rc =  MCU_Timer_Deinitialize(timer);

    /* HW HANDLE INVALID */
    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1, RTC_Init_fake.call_count);
    ASSERT_EQ(0, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerDeInitialize_BURTC)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt DeInit Handle */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt type */
    *type = (enum Hal_MCU_Timer_E)(EM_BURTC);

    Retcode_T rc;

    rc =  MCU_Timer_Deinitialize(timer);

    /* HW HANDLE INVALID */
    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0, RTC_Init_fake.call_count);
    ASSERT_EQ(1, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerDeInitialize_TIMER)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt DeInit Handle */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt type */
    *type = (enum Hal_MCU_Timer_E)(EM_TIMER);

    Retcode_T rc;

    rc =  MCU_Timer_Deinitialize(timer);

    /* HW HANDLE INVALID */
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Init_fake.call_count);
    ASSERT_EQ(0, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */

#if (KISO_FEATURE_EFM32_LETIMER)
TEST_F(HAL_MCU_Timer_Test, TimerDeInitialize_LETIMER)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt DeInit Handle */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt type */
    *type = (enum Hal_MCU_Timer_E)(EM_LETIMER);

    Retcode_T rc;

    rc =  MCU_Timer_Deinitialize(timer);

    /* HW HANDLE INVALID */
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Init_fake.call_count);
    ASSERT_EQ(0, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_LETIMER) */

TEST_F(HAL_MCU_Timer_Test, TimerDeInitialize_INVALID_PARAM)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt DeInit Handle */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);

    /* Corrupt type */
    *type = (enum Hal_MCU_Timer_E)(10);

    Retcode_T rc;

    rc =  MCU_Timer_Deinitialize(timer);

    /* HW HANDLE INVALID */
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Init_fake.call_count);
    ASSERT_EQ(0, BURTC_Init_fake.call_count);
    ASSERT_EQ(0, LETIMER_Init_fake.call_count);
    ASSERT_EQ(0, TIMER_Init_fake.call_count);
}
#endif

/* ************************************************************************** */
/* * TIMER SET COMPARE TESTS ************************************************ */
/* ************************************************************************** */
#if TIMER_TEST_SET_COMPARE

TEST_F(HAL_MCU_Timer_Test, TimerSetCompare_NULLPOINTER)
{

    Retcode_T rc;

    Timer_T timer = (uint32_t)NULL;

    rc = MCU_Timer_SetCompareValue(timer, 0, 1);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}


TEST_F(HAL_MCU_Timer_Test, TimerSetCompare_HWPOINTER)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt HW Handle */
    uint32_t * hwHandle =  (uint32_t *)&(BSP_Generic_Handle.HwHandle);
    *hwHandle = (uint32_t )NULL;

    rc = MCU_Timer_SetCompareValue(timer, 0, 1);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOHWHANDLE), rc);
}

#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerSetCompare_RTC)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_RTC;

    rc = MCU_Timer_SetCompareValue(timer, 0, 1);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1, RTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareSet_fake.call_count);
    ASSERT_EQ(0, TIMER_CompareSet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerSetCompare_BURTC)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_BURTC;

    rc = MCU_Timer_SetCompareValue(timer, 0, 1);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0, RTC_CompareSet_fake.call_count);
    ASSERT_EQ(1, BURTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareSet_fake.call_count);
    ASSERT_EQ(0, TIMER_CompareSet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerSetCompare_TIMER)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_TIMER;

    rc = MCU_Timer_SetCompareValue(timer, 0, 1);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareSet_fake.call_count);
    ASSERT_EQ(0, TIMER_CompareSet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerSetCompare_LETIMER)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_LETIMER;

    rc = MCU_Timer_SetCompareValue(timer, 0, 1);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareSet_fake.call_count);
    ASSERT_EQ(0, TIMER_CompareSet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_LETIMER) */

TEST_F(HAL_MCU_Timer_Test, TimerSetCompare_INVALID_TYPE)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )(10);

    rc = MCU_Timer_SetCompareValue(timer, 0, 1);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareSet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareSet_fake.call_count);
    ASSERT_EQ(0, TIMER_CompareSet_fake.call_count);

}
#endif

/* ************************************************************************** */
/* * TIMER GET COMPARE TESTS ************************************************ */
/* ************************************************************************** */
#if TIMER_TEST_GET_COMPARE

TEST_F(HAL_MCU_Timer_Test, TimerGetCompare_NULLPOINTER)
{

    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)NULL;

    rc = MCU_Timer_GetCompareValue(timer, 0, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}


TEST_F(HAL_MCU_Timer_Test, TimerGetCompare_HWPOINTER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt HW Handle */
    uint32_t * hwHandle =  (uint32_t *)&(BSP_Generic_Handle.HwHandle);
    *hwHandle = (uint32_t )NULL;

    rc = MCU_Timer_GetCompareValue(timer, 0, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOHWHANDLE), rc);
}

TEST_F(HAL_MCU_Timer_Test, TimerGetCompare_NULL_VALUE)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;
    rc = MCU_Timer_GetCompareValue(timer, 0, NULL);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

    ASSERT_EQ(0, RTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CaptureGet_fake.call_count);

}

#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerGetCompare_RTC)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_RTC;

    rc = MCU_Timer_GetCompareValue(timer, 0, &value);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1, RTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CaptureGet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerGetCompare_BURTC)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_BURTC;

    rc = MCU_Timer_GetCompareValue(timer, 0, &value);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0, RTC_CompareGet_fake.call_count);
    ASSERT_EQ(1, BURTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CaptureGet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerGetCompare_TIMER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_TIMER;

    rc = MCU_Timer_GetCompareValue(timer, 0, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CaptureGet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */

#if (KISO_FEATURE_EFM32_LETIMER)
TEST_F(HAL_MCU_Timer_Test, TimerGetCompare_LETIMER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_LETIMER;

    rc = MCU_Timer_GetCompareValue(timer, 0, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CaptureGet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_LETIMER) */


TEST_F(HAL_MCU_Timer_Test, TimerGetCompare_INVALID_TYPE)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )(10);

    rc = MCU_Timer_GetCompareValue(timer, 0, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CompareGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CompareGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CaptureGet_fake.call_count);

}
#endif

/* ************************************************************************** */
/* * TIMER GET COUNT TESTS ************************************************** */
/* ************************************************************************** */
#if TIMER_TEST_GET_COUNT
TEST_F(HAL_MCU_Timer_Test, TimerGetCount_NULLPOINTER)
{

    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (uint32_t)NULL;

    rc = MCU_Timer_GetCountValue(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}


TEST_F(HAL_MCU_Timer_Test, TimerGetCount_HWPOINTER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt HW Handle */
    uint32_t * hwHandle =  (uint32_t *)&(BSP_Generic_Handle.HwHandle);
    *hwHandle = (uint32_t )NULL;

    rc = MCU_Timer_GetCountValue(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOHWHANDLE), rc);
}

TEST_F(HAL_MCU_Timer_Test, TimerGetCount_NULL_VALUE)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    rc = MCU_Timer_GetCountValue(timer, NULL);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);


    ASSERT_EQ(0, RTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CounterGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CounterGet_fake.call_count);

}

#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerGetCount_RTC)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_RTC;

    rc = MCU_Timer_GetCountValue(timer, &value);

    ASSERT_EQ(RETCODE_OK, rc);


    ASSERT_EQ(1, RTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CounterGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CounterGet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerGetCount_BURTC)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_BURTC;

    rc = MCU_Timer_GetCountValue(timer, &value);

    ASSERT_EQ(RETCODE_OK, rc);


    ASSERT_EQ(0, RTC_CounterGet_fake.call_count);
    ASSERT_EQ(1, BURTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CounterGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CounterGet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerGetCount_TIMER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_TIMER;

    rc = MCU_Timer_GetCountValue(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);


    ASSERT_EQ(0, RTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CounterGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CounterGet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */

#if (KISO_FEATURE_EFM32_LETIMER)
TEST_F(HAL_MCU_Timer_Test, TimerGetCount_LETIMER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_LETIMER;

    rc = MCU_Timer_GetCountValue(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);


    ASSERT_EQ(0, RTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CounterGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CounterGet_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_LETIMER) */


TEST_F(HAL_MCU_Timer_Test, TimerGetCount_INVALID_TYPE)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )(10);

    rc = MCU_Timer_GetCountValue(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, BURTC_CounterGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_CounterGet_fake.call_count);
    ASSERT_EQ(0, TIMER_CounterGet_fake.call_count);

}
#endif

/* ************************************************************************** */
/* * TIMER GET MAXCOUNT TESTS *********************************************** */
/* ************************************************************************** */
#if TIMER_TEST_GET_MAXCOUNT
TEST_F(HAL_MCU_Timer_Test, TimerGetMaxCount_NULLPOINTER)
{

    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)NULL;

    rc = MCU_Timer_GetMaxCount(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}


TEST_F(HAL_MCU_Timer_Test, TimerGetMaxCount_HWPOINTER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt HW Handle */
    uint32_t * hwHandle =  (uint32_t *)&(BSP_Generic_Handle.HwHandle);
    *hwHandle = (uint32_t )NULL;

    rc = MCU_Timer_GetMaxCount(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOHWHANDLE), rc);
}

TEST_F(HAL_MCU_Timer_Test, TimerGetMaxCount_NULL_VALUE)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    rc = MCU_Timer_GetMaxCount(timer, NULL);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}

#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerGetMaxCount_RTC)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_RTC;

    rc = MCU_Timer_GetMaxCount(timer, &value);

    ASSERT_EQ(_RTC_CNT_CNT_MASK, value);

    ASSERT_EQ(RETCODE_OK, rc);


}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerGetMaxCount_BURTC)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_BURTC;

    rc = MCU_Timer_GetMaxCount(timer, &value);

    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(_BURTC_CNT_CNT_MASK, value);


}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerGetMaxCount_TIMER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_TIMER;

    rc = MCU_Timer_GetMaxCount(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */

#if (KISO_FEATURE_EFM32_LETIMER)
TEST_F(HAL_MCU_Timer_Test, TimerGetMaxCount_LETIMER)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_LETIMER;

    rc = MCU_Timer_GetMaxCount(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}
#endif /* if (KISO_FEATURE_EFM32_LETIMER) */


TEST_F(HAL_MCU_Timer_Test, TimerGetMaxCount_INVALID_TYPE)
{
    Retcode_T rc;
    uint32_t value;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )(10);

    rc = MCU_Timer_GetMaxCount(timer, &value);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);



}
#endif


/* ************************************************************************** */
/* * TIMER START TESTS ****************************************************** */
/* ************************************************************************** */
#if TIMER_TEST_START
TEST_F(HAL_MCU_Timer_Test, TimerStart_NULL_POINTER)
{
    Retcode_T rc;


    Timer_T timer = 0;

    rc = MCU_Timer_Start(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

}


TEST_F(HAL_MCU_Timer_Test, TimerStart_NOHWHANDLE)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    uint32_t * hwHandle = (uint32_t *)&(BSP_Generic_Handle.HwHandle);
    *hwHandle = (uint32_t )NULL;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_RTC;

    rc = MCU_Timer_Start(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOHWHANDLE), rc);

}

#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerStart_RTC)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_RTC;

    rc = MCU_Timer_Start(timer);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1, RTC_Enable_fake.call_count);
    ASSERT_EQ(0, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerStart_BURTC)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_BURTC;

    rc = MCU_Timer_Start(timer);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0, RTC_Enable_fake.call_count);
    ASSERT_EQ(1, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_LETIMER)
TEST_F(HAL_MCU_Timer_Test, TimerStart_LETIMER)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_LETIMER;

    rc = MCU_Timer_Start(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Enable_fake.call_count);
    ASSERT_EQ(0, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_LETIMER) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerStart_TIMER)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_TIMER;

    rc = MCU_Timer_Start(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Enable_fake.call_count);
    ASSERT_EQ(0, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */

TEST_F(HAL_MCU_Timer_Test, TimerStart_INVALID)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )(10);

    rc = MCU_Timer_Start(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Enable_fake.call_count);
    ASSERT_EQ(0, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);
}

#endif


/* ************************************************************************** */
/* * TIMER STOP TESTS ******************************************************* */
/* ************************************************************************** */
#if TIMER_TEST_STOP
TEST_F(HAL_MCU_Timer_Test, TimerStop_NULL_POINTER)
{
    Retcode_T rc;

    Timer_T timer = 0;

    rc = MCU_Timer_Stop(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

}


TEST_F(HAL_MCU_Timer_Test, TimerStop_NOHWHANDLE)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    uint32_t * hwHandle = (uint32_t *)&(BSP_Generic_Handle.HwHandle);
    *hwHandle = (uint32_t )NULL;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_RTC;

    rc = MCU_Timer_Stop(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_NOHWHANDLE), rc);

}

#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerStop_RTC)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_RTC;

    rc = MCU_Timer_Stop(timer);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1, RTC_Enable_fake.call_count);
    ASSERT_EQ(0, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);

}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerStop_BURTC)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_BURTC;

    rc = MCU_Timer_Stop(timer);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0, RTC_Enable_fake.call_count);
    ASSERT_EQ(1, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_LETIMER)
TEST_F(HAL_MCU_Timer_Test, TimerStop_LETIMER)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_LETIMER;

    rc = MCU_Timer_Stop(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Enable_fake.call_count);
    ASSERT_EQ(0, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_LETIMER) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, TimerStop_TIMER)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )EM_TIMER;

    rc = MCU_Timer_Stop(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Enable_fake.call_count);
    ASSERT_EQ(0, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */

TEST_F(HAL_MCU_Timer_Test, TimerStop_INVALID)
{
    Retcode_T rc;

    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    /* Corrupt Type Information */
    uint32_t * type =  (uint32_t *)&(BSP_Generic_Handle.HwType);
    *type = (uint32_t )(10);

    rc = MCU_Timer_Stop(timer);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0, RTC_Enable_fake.call_count);
    ASSERT_EQ(0, BURTC_Enable_fake.call_count);
    ASSERT_EQ(0, LETIMER_Enable_fake.call_count);
    ASSERT_EQ(0, TIMER_Enable_fake.call_count);
}

#endif


/* ************************************************************************** */
/* * TIMER EXTRACT FLAGS RTC ************************************************ */
/* ************************************************************************** */
#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, TimerExtractFlagsRTC)
{
    struct MCU_Timer_Event_S event;
    uint32_t flags;

    ASSERT_EQ(2, RTC_IF_COMP0);
    ASSERT_EQ(4, RTC_IF_COMP1);
    ASSERT_EQ(2+4, RTC_IF_COMP0 | RTC_IF_COMP1);

    flags = 0;

    event =  extractFlags_RTC(flags);

    ASSERT_EQ(0, event.CompareMatch);
    ASSERT_EQ(0, event.CompareChannel);

    /* Compare match Channel 0*/
    flags = RTC_IF_COMP0;
    event =  extractFlags_RTC(flags);

    ASSERT_EQ(1, event.CompareMatch);
    ASSERT_EQ(MCU_TIMER_COMPARE_CHANNEL0, event.CompareChannel);

    /* Compare match Channel 1*/
    flags = RTC_IF_COMP1;
    event =  extractFlags_RTC(flags);

    ASSERT_EQ(1, event.CompareMatch);
    ASSERT_EQ(MCU_TIMER_COMPARE_CHANNEL1, event.CompareChannel);


    /* Compare match Channel 1 and 2*/
    flags = RTC_IF_COMP0 | RTC_IF_COMP1;
    event =  extractFlags_RTC(flags);

    ASSERT_EQ(1, event.CompareMatch);
    ASSERT_EQ(MCU_TIMER_COMPARE_CHANNEL0 | MCU_TIMER_COMPARE_CHANNEL1, event.CompareChannel);


    /* Test for Overflow */
    /* Compare match Channel 1 and 2*/
    flags = RTC_IF_OF;
    event =  extractFlags_RTC(flags);

    ASSERT_EQ(0, event.CompareMatch);
    ASSERT_EQ(1, event.Overflow);

}
#endif /* if (KISO_FEATURE_EFM32_RTC) */


/* ************************************************************************** */
/* * TIMER EXTRACT FLAGS BURTC ********************************************** */
/* ************************************************************************** */
#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, TimerExtractFlagsBURTC)
{
    struct MCU_Timer_Event_S event;
    uint32_t flags;

    ASSERT_EQ(2, BURTC_IF_COMP0);

    flags = 0;

    event =  extractFlags_BURTC(flags);

    ASSERT_EQ(0, event.CompareMatch);
    ASSERT_EQ(0, event.CompareChannel);

    /* Compare match Channel 0*/
    flags = BURTC_IF_COMP0;
    event =  extractFlags_BURTC(flags);

    ASSERT_EQ(true, event.CompareMatch);
    ASSERT_EQ(MCU_TIMER_COMPARE_CHANNEL0, event.CompareChannel);

    /* LFXCO Fail */
    flags = BURTC_IF_LFXOFAIL;
    event =  extractFlags_BURTC(flags);

    ASSERT_EQ(0, event.CompareMatch);
    ASSERT_EQ(TIMER_LFXCO_FAILURE, event.ErrorCode);


    /* Compare match Channel 1 and 2*/
    flags = RTC_IF_COMP0 | RTC_IF_COMP1;
    event =  extractFlags_RTC(flags);

    ASSERT_EQ(1, event.CompareMatch);
    ASSERT_EQ(MCU_TIMER_COMPARE_CHANNEL0 | MCU_TIMER_COMPARE_CHANNEL1, event.CompareChannel);


    /* Test for Overflow */
    /* Compare match Channel 1 and 2*/
    flags = RTC_IF_OF;
    event =  extractFlags_RTC(flags);

    ASSERT_EQ(0, event.CompareMatch);
    ASSERT_EQ(1, event.Overflow);

}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */


/* ************************************************************************** */
/* * GET TIMER EVENTS ******************************************************* */
/* ************************************************************************** */
#if (KISO_FEATURE_EFM32_RTC)
TEST_F(HAL_MCU_Timer_Test, getTimerEvent_RTC)
{
    struct MCU_Timer_Event_S event;

  //  uint32_t timer = &BSP_Generic_Handle;

    /* Corrupt Type Information */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);
    *type = EM_RTC;

    event = getTimerEvent(&BSP_Generic_Handle);

    ASSERT_EQ(1, RTC_IntGet_fake.call_count);
    ASSERT_EQ(0, BURTC_IntGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_IntGet_fake.call_count);
    ASSERT_EQ(0, TIMER_IntGet_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_RTC) */

#if (KISO_FEATURE_EFM32_BURTC)
TEST_F(HAL_MCU_Timer_Test, getTimerEvent_BURTC)
{
    struct MCU_Timer_Event_S event;

  //  uint32_t timer = &BSP_Generic_Handle;

    /* Corrupt Type Information */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);
    *type = EM_BURTC;

    event = getTimerEvent(&BSP_Generic_Handle);

    ASSERT_EQ(0, RTC_IntGet_fake.call_count);
    ASSERT_EQ(1, BURTC_IntGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_IntGet_fake.call_count);
    ASSERT_EQ(0, TIMER_IntGet_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_BURTC) */

#if (KISO_FEATURE_EFM32_LETIMER)
TEST_F(HAL_MCU_Timer_Test, getTimerEvent_LETIMER)
{
    struct MCU_Timer_Event_S event;

  //  uint32_t timer = &BSP_Generic_Handle;

    /* Corrupt Type Information */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);
    *type = EM_LETIMER;

    event = getTimerEvent(&BSP_Generic_Handle);

    ASSERT_EQ(0, RTC_IntGet_fake.call_count);
    ASSERT_EQ(0, BURTC_IntGet_fake.call_count);
  //  ASSERT_EQ(1, LETIMER_IntGet_fake.call_count);
    ASSERT_EQ(0, TIMER_IntGet_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_LETIMER) */

#if (KISO_FEATURE_EFM32_TIMER)
TEST_F(HAL_MCU_Timer_Test, getTimerEvent_TIMER)
{
    struct MCU_Timer_Event_S event;

  //  uint32_t timer = &BSP_Generic_Handle;

    /* Corrupt Type Information */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);
    *type = EM_TIMER;

    event = getTimerEvent(&BSP_Generic_Handle);

    ASSERT_EQ(0, RTC_IntGet_fake.call_count);
    ASSERT_EQ(0, BURTC_IntGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_IntGet_fake.call_count);
  //  ASSERT_EQ(1, TIMER_IntGet_fake.call_count);
}
#endif /* if (KISO_FEATURE_EFM32_TIMER) */


TEST_F(HAL_MCU_Timer_Test, getTimerEvent_FAIL)
{
    struct MCU_Timer_Event_S event;

  //  uint32_t timer = &BSP_Generic_Handle;

    /* Corrupt Type Information */
    enum Hal_MCU_Timer_E * type = (enum Hal_MCU_Timer_E *)&(BSP_Generic_Handle.HwType);
    *type = (enum Hal_MCU_Timer_E)10;

    event = getTimerEvent(&BSP_Generic_Handle);

    ASSERT_EQ(0, RTC_IntGet_fake.call_count);
    ASSERT_EQ(0, BURTC_IntGet_fake.call_count);
    ASSERT_EQ(0, LETIMER_IntGet_fake.call_count);
    ASSERT_EQ(0, TIMER_IntGet_fake.call_count);
}

/* ************************************************************************** */
/* * TIMER CALLBACK EXECUTE ************************************************* */
/* ************************************************************************** */
TEST_F(HAL_MCU_Timer_Test,  MCU_Timer_Callback_NULLTIMER)
{
    Timer_T timer = UINT32_C(0);

    MCU_Timer_Callback(timer);

    ASSERT_EQ(1, Retcode_RaiseError_fake.call_count);
    ASSERT_EQ(0, TimerTestCallback_fake.call_count);


}

TEST_F(HAL_MCU_Timer_Test,  MCU_Timer_Callback_NULLCALLBACK)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    MCU_TIMER_Callback_T * callback = &(BSP_Generic_Handle.Callback);

    *callback = (MCU_TIMER_Callback_T)NULL;

    MCU_Timer_Callback(timer);

    ASSERT_EQ(0, Retcode_RaiseError_fake.call_count);
    ASSERT_EQ(0, TimerTestCallback_fake.call_count);
}

TEST_F(HAL_MCU_Timer_Test,  MCU_Timer_Callback)
{
    Timer_T timer = (Timer_T)&BSP_Generic_Handle;

    MCU_TIMER_Callback_T * callback = &(BSP_Generic_Handle.Callback);

    *callback = (MCU_TIMER_Callback_T)TimerTestCallback;

    MCU_Timer_Callback(timer);

    ASSERT_EQ(0, Retcode_RaiseError_fake.call_count);
    ASSERT_EQ(1, TimerTestCallback_fake.call_count);
}
//TimerTestCallback

#else
}
#endif