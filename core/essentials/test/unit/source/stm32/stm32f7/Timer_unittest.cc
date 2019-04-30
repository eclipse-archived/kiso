/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START 

/* include faked interface */
extern "C"
{
/* setup compile time configuration defines */

/* instead of BCDS_BSP_BoardConfig.h mock */
#define BCDS_FEATURE_TIMER 1
/* include faked interfaces */
#include "BCDS_Basics.h"

#include "BCDS_HAL_th.hh"

#include "stm32f7xx_th.hh"
#include "stm32f7xx_hal_tim_th.hh"

#undef BCDS_FEATURE_TIMER
#define BCDS_FEATURE_TIMER 1
#include "Timer.c"

} /* extern "C"*/
/* End of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END 

class TimerTestContext
{
public:
    TimerTestContext()
    {
        mTestAppCallbackCount = 0;
        // Configure DMA mode by default
        mBSPHandletimer.TimMode = STM_TIMER_MODE_INTERRUPT;
        TimerTestContext::TestContext = this;
    }
    /* A static pointer that will be set upon setup to be able to access
     * the test instance from within static functions */
     static TimerTestContext* TestContext ;

    /* Declare a static function used as application callback for Timer */
    static void TestAppCallback(Timer_T timer, struct MCU_Timer_Event_S event);

    uint32_t mTestAppCallbackCount;
    TIM_HandleTypeDef mSTMHandletimer;
    struct MCU_Timer_Handle_S mBSPHandletimer;
    struct MCU_Timer_Event_S mLastEvent;
};

	/* static member variable */
TimerTestContext* TimerTestContext::TestContext = NULL;
	/* static member function */
void TimerTestContext::TestAppCallback(Timer_T timer, struct MCU_Timer_Event_S event)
{
    if (TestContext)
    {
        UNUSED(timer);
        TestContext->mLastEvent = event;
        /* Do something useful, e.g. increment a counter */
        TestContext->mTestAppCallbackCount++;
    }
}



/* Create an instance for the test context */
TimerTestContext testContext;
TIM_HandleTypeDef timerHandle;
TIM_TypeDef timerDef;

class BCDS_Timertest: public testing::Test
{
public:

protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        memset(&timerHandle, 0, sizeof(timerHandle));
        memset(&timerDef, 0, sizeof(timerDef));
        timerDef.SR = 0x00;
        timerHandle.Instance = &timerDef;
        testContext.mBSPHandletimer.TimerHandle.Instance = &timerDef;
    }

    virtual void TearDown()
    {
        RESET_FAKE(HAL_TIM_Base_Init);
        RESET_FAKE(HAL_TIM_Base_DeInit);
        RESET_FAKE(HAL_TIM_Base_Start_IT);
        RESET_FAKE(HAL_TIM_Base_Stop_IT);
        RESET_FAKE(HAL_TIM_IRQHandler);
    }
};


TEST_F(BCDS_Timertest, TimerInit)
{
    /* Here we test the MCU_Timer_Initialize function */

    /* call initialize function without a valid Timer handler */
    Retcode_T rc = MCU_Timer_Initialize(0, TimerTestContext::TestAppCallback);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM), rc);

    /* Call initialize function with missing callback function  */
    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NULL_POINTER), rc);


    /* Make copy of Timer object */
    TimerTestContext testContextBackup = testContext;

    /* Copy object back and test function for HAL return code failure*/
    testContext = testContextBackup;
    HAL_TIM_Base_Init_fake.return_val = HAL_ERROR;
    HAL_TIM_Base_Init_fake.call_count =0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_DEINITIALIZED;

    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_DEINITIALIZED);
    EXPECT_EQ(1,HAL_TIM_Base_Init_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_INIT_FAIL), rc);

    /* Call initialize function with bloking mode  */
    testContext = testContextBackup;
    HAL_TIM_Base_Init_fake.return_val = HAL_OK;
    testContext.mBSPHandletimer.TimMode = STM_TIMER_MODE_BLOCKING;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_DEINITIALIZED;
    HAL_TIM_Base_Init_fake.call_count = 0;
    /* call initialize function with correct parameters  */
    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_DEINITIALIZED);
    EXPECT_EQ(1,HAL_TIM_Base_Init_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);

    /* Call initialize function with correct parameters  */
    testContext = testContextBackup;
    HAL_TIM_Base_Init_fake.return_val = HAL_OK;
    testContext.mBSPHandletimer.TimMode = STM_TIMER_MODE_INTERRUPT;
    HAL_TIM_Base_Init_fake.call_count = 0;
    /* call initialize function with correct parameters  */
    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_INITIALIZED);
    EXPECT_EQ(1,HAL_TIM_Base_Init_fake.call_count);
    EXPECT_EQ(RETCODE_OK, rc);

    /* Try to Initialize Twice  */
    testContext = testContextBackup;
    HAL_TIM_Base_Init_fake.return_val = HAL_OK;
    HAL_TIM_Base_Init_fake.call_count =0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_INITIALIZED;
    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_INITIALIZED);
    EXPECT_EQ(0,HAL_TIM_Base_Init_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_ALREADY_INITIALIZED), rc);

    /* Try to Initialize while timer is Running  */
    testContext = testContextBackup;
    HAL_TIM_Base_Init_fake.return_val = HAL_OK;
    HAL_TIM_Base_Init_fake.call_count =0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_ACTIVE;
    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_ACTIVE);
    EXPECT_EQ(0,HAL_TIM_Base_Init_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_ALREADY_INITIALIZED), rc);

    /* Try to Initialize while timer is in idle mode  */
    testContext = testContextBackup;
    HAL_TIM_Base_Init_fake.return_val = HAL_OK;
    HAL_TIM_Base_Init_fake.call_count =0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_IDLE;
    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_IDLE);
    EXPECT_EQ(0,HAL_TIM_Base_Init_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_ALREADY_INITIALIZED), rc);

    /* restore initial state */
    testContext.mBSPHandletimer.TimMode = STM_TIMER_MODE_INTERRUPT;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_DEINITIALIZED;
    /* call initialize function with correct parameters in DMA mode for further tests  */
    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);
    EXPECT_EQ(RETCODE_OK, rc);

}


TEST_F(BCDS_Timertest, StartTimer)
{
    /* here we test the MCU_Timer_Start function */

    /* Call MCU_Timer_Start function without a valid Timer handler */
    Retcode_T rc = MCU_Timer_Start(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM), rc);

    /* make copy of Timer object */
    TimerTestContext testContextBackup = testContext;

    /* Call MCU_Timer_Start function with correct parameters  */
    testContext = testContextBackup;
    HAL_TIM_Base_Start_IT_fake.return_val = HAL_OK;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_IDLE;
    rc = MCU_Timer_Start((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_ACTIVE);
    EXPECT_EQ(1,HAL_TIM_Base_Start_IT_fake.call_count);
    EXPECT_EQ(RETCODE_OK, rc);

    /* Call MCU_Timer_Start function when timer is in Active State */
    testContext = testContextBackup;
    HAL_TIM_Base_Start_IT_fake.return_val = HAL_ERROR;
    HAL_TIM_Base_Start_IT_fake.call_count = 0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_IDLE;
    rc = MCU_Timer_Start((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_IDLE);
    EXPECT_EQ(1,HAL_TIM_Base_Start_IT_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_START_FAIL), rc);

    /* Call MCU_Timer_Start function when timer is in Deinit State */
    testContext = testContextBackup;
    HAL_TIM_Base_Start_IT_fake.return_val = HAL_OK;
    HAL_TIM_Base_Start_IT_fake.call_count = 0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_DEINITIALIZED;
    rc = MCU_Timer_Start((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_DEINITIALIZED);
    EXPECT_EQ(0,HAL_TIM_Base_Start_IT_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_NOINIT), rc);

    /* Call MCU_Timer_Start function when timer is in Already Active State */
    testContext = testContextBackup;
    HAL_TIM_Base_Start_IT_fake.return_val = HAL_OK;
    HAL_TIM_Base_Start_IT_fake.call_count = 0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_ACTIVE;
    rc = MCU_Timer_Start((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_ACTIVE);
    EXPECT_EQ(0,HAL_TIM_Base_Start_IT_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_IS_ACTIVE), rc);

    /* Call MCU_Timer_Start function with out assigning callbacks  */
    testContext = testContextBackup;
    HAL_TIM_Base_Start_IT_fake.return_val = HAL_OK;
    HAL_TIM_Base_Start_IT_fake.call_count = 0;
    rc = MCU_Timer_Deinitialize((Timer_T) &testContext.mBSPHandletimer);

    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_IDLE;
    rc = MCU_Timer_Start((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_IDLE);
    EXPECT_EQ(0,HAL_TIM_Base_Start_IT_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

}

TEST_F(BCDS_Timertest, StopTimer)
{
    /* hHere we test the MCU_Timer_Stop function */

    /* Call MCU_Timer_Stop function without a valid Timer handler */
    Retcode_T rc = MCU_Timer_Stop(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM), rc);

    /* make copy of Timer object */
    HAL_TIM_Base_Init_fake.return_val = HAL_OK;
    testContext.mBSPHandletimer.TimMode = STM_TIMER_MODE_INTERRUPT;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_DEINITIALIZED;
    HAL_TIM_Base_Init_fake.call_count = 0;
    /* call initialize function with correct parameters  */
    rc = MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);

    /* Call MCU_Timer_Stop function with correct parameters  */
    TimerTestContext testContextBackup = testContext;
    testContext = testContextBackup;
    HAL_TIM_Base_Stop_IT_fake.return_val = HAL_OK;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_ACTIVE;
    rc = MCU_Timer_Stop((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_IDLE);
    EXPECT_EQ(1,HAL_TIM_Base_Stop_IT_fake.call_count);
    EXPECT_EQ(RETCODE_OK, rc);

    /* Test MCU_Timer_Stop function with HAL returns fail */
    testContext = testContextBackup;
    HAL_TIM_Base_Stop_IT_fake.return_val = HAL_ERROR;
    HAL_TIM_Base_Stop_IT_fake.call_count = 0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_ACTIVE;
    rc = MCU_Timer_Stop((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_ACTIVE);
    EXPECT_EQ(1,HAL_TIM_Base_Stop_IT_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_STOP_FAIL), rc);

    /* Test MCU_Timer_Stop function with IDLE state */
    testContext = testContextBackup;
    HAL_TIM_Base_Stop_IT_fake.return_val = HAL_OK;
    HAL_TIM_Base_Stop_IT_fake.call_count = 0;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_IDLE;
    rc = MCU_Timer_Stop((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_IDLE);
    EXPECT_EQ(0,HAL_TIM_Base_Stop_IT_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_IS_NOT_ACTIVE), rc);

    /* Call MCU_Timer_Start function with out assigning callbacks  */
    testContext = testContextBackup;
    HAL_TIM_Base_Stop_IT_fake.return_val = HAL_OK;
    HAL_TIM_Base_Stop_IT_fake.call_count = 0;
    rc = MCU_Timer_Deinitialize((Timer_T) &testContext.mBSPHandletimer);

    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_ACTIVE;
    rc = MCU_Timer_Stop((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(testContext.mBSPHandletimer.Status, MCU_TIMER_STATUS_ACTIVE);
    EXPECT_EQ(0,HAL_TIM_Base_Stop_IT_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

}


TEST_F(BCDS_Timertest, GetTimerStatus)
{
    /* First try with wrong handle */
	MCU_Timer_Status_T status;
	status = MCU_Timer_GetStatus(0);
    EXPECT_EQ(status, MCU_TIMER_STATUS_ERROR);

    /* Then use a valid handle */
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_IDLE;
    status = MCU_Timer_GetStatus((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(status, MCU_TIMER_STATUS_IDLE);
}

/**
 * This test is currently just used to call the Timer interrupt callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_Timertest, TimerIRQCallback)
{
    /* First try with totally wrong parameters */
	RESET_FAKE(HAL_TIM_IRQHandler);
	MCU_BSP_TIMER_IRQ_Callback(0);
    EXPECT_EQ(0, HAL_TIM_IRQHandler_fake.call_count);
    /* Then use a valid handle */
    MCU_BSP_TIMER_IRQ_Callback((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(1, HAL_TIM_IRQHandler_fake.call_count);
    RESET_FAKE(HAL_TIM_IRQHandler);
}


/**
 * This test is currently just used to call the HAL_TIM_PeriodElapsedCallback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_Timertest,PeriodElapsedCallback)
{
    /* First try with totally wrong parameters */
	testContext.mTestAppCallbackCount = 0;
	HAL_TIM_PeriodElapsedCallback(NULL);
    EXPECT_EQ(0,testContext.mTestAppCallbackCount);

    /* Then use a valid handle */
    HAL_TIM_Base_Init_fake.return_val = HAL_OK;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_DEINITIALIZED;
    MCU_Timer_Initialize((Timer_T) &testContext.mBSPHandletimer, &TimerTestContext::TestAppCallback);
    HAL_TIM_PeriodElapsedCallback(&testContext.mBSPHandletimer.TimerHandle);
    EXPECT_EQ(1,testContext.mLastEvent.CompareMatch);
    EXPECT_EQ(1,testContext.mTestAppCallbackCount);

    /* Test HAL_TIM_PeriodElapsedCallback without a callback function */
    testContext.mBSPHandletimer.AppCallback = NULL;
    testContext.mLastEvent.CompareMatch = 0;
    testContext.mTestAppCallbackCount   = 0;
    HAL_TIM_PeriodElapsedCallback(&testContext.mBSPHandletimer.TimerHandle);
    EXPECT_EQ(0,testContext.mLastEvent.CompareMatch);
    EXPECT_EQ(0,testContext.mTestAppCallbackCount);
}

/**
 * @attention This should be the last test case executed because it deintializes
 * the local SPI Handle of the testContext.
 * After that test it is necessary to call MCU_Timer_Initialize again if more tests
 * should be run after this one.
 */
TEST_F(BCDS_Timertest, TimerDeInit)
{
    /* First try with totally wrong parameters */
    Retcode_T rc = MCU_Timer_Deinitialize(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM), rc);
    /* Then use the correct handle */
    HAL_TIM_Base_DeInit_fake.return_val = HAL_OK;
    testContext.mBSPHandletimer.Status = MCU_TIMER_STATUS_IDLE;
    rc = MCU_Timer_Deinitialize((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(1, HAL_TIM_Base_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_OK, rc);
    /* check if the pointers are reset to default */
    EXPECT_EQ((uint32_t)testContext.mBSPHandletimer.AppCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandletimer.IRQCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandletimer.TimerStartFunPtr,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandletimer.TimerStopFunPtr,NULL);

    HAL_TIM_Base_DeInit_fake.return_val = HAL_ERROR;
    HAL_TIM_Base_DeInit_fake.call_count   = 0;
    rc = MCU_Timer_Deinitialize((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(1, HAL_TIM_Base_DeInit_fake.call_count);
    EXPECT_NE(RETCODE_OK, rc);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_DEINIT_FAIL), rc);

    HAL_TIM_Base_DeInit_fake.return_val = HAL_OK;
    testContext.mBSPHandletimer.Status  = MCU_TIMER_STATUS_ACTIVE;
    HAL_TIM_Base_DeInit_fake.call_count = 0;
    rc = MCU_Timer_Deinitialize((Timer_T) &testContext.mBSPHandletimer);
    EXPECT_EQ(0, HAL_TIM_Base_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_IS_ACTIVE),rc);


}


TEST_F(BCDS_Timertest, SetCompareValue)
{
    /* Test the APIs with dummy parameter as this function is not supported */
	Retcode_T rc     = RETCODE_OK;
	uint32_t channel = 0;
	uint32_t value   = 0;

    rc = MCU_Timer_SetCompareValue((Timer_T) &testContext.mBSPHandletimer, channel,value);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED), rc);

}

TEST_F(BCDS_Timertest, GetCompareValue)
{
    /* Test the APIs with dummy parameter as this function is not supported */
	Retcode_T rc     = RETCODE_OK;
	uint32_t channel = 0;
	uint32_t value   = 0;

    rc = MCU_Timer_GetCompareValue((Timer_T) &testContext.mBSPHandletimer, channel, &value);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED), rc);

}

TEST_F(BCDS_Timertest, GetCountValue)
{
    /* Test the APIs with dummy parameter as this function is not supported */

	Retcode_T rc		= RETCODE_OK;
	uint32_t countValue = 0;

    rc = MCU_Timer_GetCountValue((Timer_T) &testContext.mBSPHandletimer, &countValue);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED), rc);

}

TEST_F(BCDS_Timertest, MaxCount)
{
    /* Test the APIs with dummy parameter as this function is not supported */

	Retcode_T rc     	= RETCODE_OK;
	uint32_t maxCount   = 0;

    rc = MCU_Timer_GetMaxCount((Timer_T) &testContext.mBSPHandletimer, &maxCount);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED), rc);

}

TEST_F(BCDS_Timertest, GetFrequency)
{
    /* Test the APIs with dummy parameter as this function is not supported */

	Retcode_T rc     	 = RETCODE_OK;
	uint32_t frequency   = 0;

    rc = MCU_Timer_GetClockFrequency((Timer_T) &testContext.mBSPHandletimer, &frequency);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED), rc);

}
