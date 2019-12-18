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

/**
 *
 * @brief
 *      Module test specification for the EventHub_unittest.cc module.
 *
 * @details
 *      The unit test file template follows the Four-Phase test pattern.
 *
 * @file
 **/

/* include gtest interface */
#include <gtest.h>

/* Start of global scope symbol and fake definitions section */
extern "C"
{
/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_EVENTHUB

#if KISO_FEATURE_EVENTHUB

/* include faked interfaces */
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#include "task_th.hh"
#include "fff.h"

#include "queue_th.hh"
#include "semphr_th.hh"

/* Include module under test */
#include "EventHub.c"

} /* End of global scope symbol and fake definitions section */

#if (CONFIG_EVENTHUB_MAX_OBSERVERS <= 0)
#error "CONFIG_EVENTHUB_MAX_OBSERVERS must not be less or equal to 0"
#endif

static const TaskEvent_T gTestEvent = 0xAA;
static const uint32_t gTestData = 0xBB;
static const void *gTestDataPtr = &gTestData;

/* Fake function created to pass as a argument to the  cmd_Enqueue function*/
void fake_fn(void *, uint32_t)
{
    ;
}

bool eventReceived_A;
bool dataCorrect_A;

bool eventReceived_B;
bool dataCorrect_B;

bool eventReceived_All;
bool dataCorrect_All;

void TestObserver_A(TaskEvent_T event, const void *data)
{
    eventReceived_A = (gTestEvent == event);
    dataCorrect_A = (gTestDataPtr == data);
}

void TestObserver_B(TaskEvent_T event, const void *data)
{
    eventReceived_B = (gTestEvent == event);
    dataCorrect_B = (gTestDataPtr == data);
}

void TestObserver_All(TaskEvent_T event, const void *data)
{
    KISO_UNUSED(event);
    eventReceived_All = true;
    dataCorrect_All = (gTestDataPtr == data);
}

class EventHubTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        RESET_FAKE(Retcode_RaiseError);
        RESET_FAKE(xSemaphoreCreateMutex);
        RESET_FAKE(xSemaphoreTake);
        RESET_FAKE(xSemaphoreGive);

        FFF_RESET_HISTORY();
    }
};

TEST_F(EventHubTest, EventHubInitializeSuccess)
{
    /** @testcase{ eventHub::EventHubInitializeSuccess: }
     * Test EventHub init success
      */

    EventHub_T eventHub;
    Retcode_T retVal = RETCODE_FAILURE;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;

    retVal = EventHub_Initialize(&eventHub);

    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(0), eventHub.observerCount);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(EventHubTest, EventHubInitializeFailure)
{
    /** @testcase{ eventHub::EventHubInitializeFailure: }
      * Test EventHub init failures
      */

    Retcode_T retVal = RETCODE_OK;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;

    retVal = EventHub_Initialize(NULL);

    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(EventHubTest, EventHubObserverSuccess)
{
    /** @testcase{ eventHub::EventHubObserverSuccess: }
     * Test EventHub observe success
     */
    EventHub_T eventHub;
    Retcode_T retVal = RETCODE_FAILURE;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)1;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_Observe(&eventHub, TestObserver_A, (TaskEvent_T)gTestEvent);

    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), eventHub.observerCount);
}

TEST_F(EventHubTest, EventHubObserverHubNull)
{
    /** @testcase{ eventHub::EventHubObserverHubNull: }
     * Test EventHub observe failures
     */
    Retcode_T retVal = RETCODE_OK;

    retVal = EventHub_Observe(NULL, TestObserver_A, (TaskEvent_T)gTestEvent);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
}

TEST_F(EventHubTest, EventHubObserverHandlerNull)
{
    /** @testcase{ eventHub::EventHubObserverHandlerNull: }
     * Test EventHub observe failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;

    retVal = EventHub_Observe(&eventHub, NULL, (TaskEvent_T)gTestEvent);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
}

TEST_F(EventHubTest, EventHubObserverHubLockNull)
{
    /** @testcase{ eventHub::EventHubObserverHubLockNull: }
     * Test EventHub observe failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;
    eventHub.lock = (SemaphoreHandle_t)0;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_Observe(&eventHub, TestObserver_A, (TaskEvent_T)gTestEvent);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
}

TEST_F(EventHubTest, EventHubObserverNotNullHubObserverCount)
{
    /** @testcase{ eventHub::EventHubObserverNotNullHubObserverCount: }
     * Test EventHub observe failures
     */
    EventHub_T eventHub;
    Retcode_T retVal = RETCODE_OK;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)1;
    eventHub.observerCount = CONFIG_EVENTHUB_MAX_OBSERVERS;

    retVal = EventHub_Observe(&eventHub, TestObserver_A, (TaskEvent_T)gTestEvent);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(EventHubTest, EventHubObserverSemTakeNotPass)
{
    /** @testcase{ eventHub::EventHubObserverSemTakeNotPass: }
     * Test EventHub observe failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;
    eventHub.observerCount = 0;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_Observe(&eventHub, TestObserver_A, (TaskEvent_T)gTestEvent);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
}

TEST_F(EventHubTest, EventHubObserveSemGiveNotPass)
{
    /** @testcase{ eventHub::EventHubObserveSemGiveNotPass: }
     * Test EventHub observe failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)0;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_Observe(&eventHub, TestObserver_A, (TaskEvent_T)gTestEvent);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(EventHubTest, EventHubObserveAllSuccess)
{
    /** @testcase{ eventHub::EventHubObserveAllSuccess: }
     * Test EventHub observe all events success
     */
    Retcode_T retVal = RETCODE_FAILURE;
    EventHub_T eventHub;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)1;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_ObserveAll(&eventHub, TestObserver_A);

    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_EQ((uint32_t)1, eventHub.observerCount);
}

TEST_F(EventHubTest, EventHubObserverAllHubNull)
{
    /** @testcase{ eventHub::EventHubObserverAllHubNull: }
     * Test EventHub observe all events failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_ObserveAll(NULL, TestObserver_A);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
}

TEST_F(EventHubTest, EventHubObserverAllHandlerNull)
{
    /** @testcase{ eventHub::EventHubObserverAllHandlerNull: }
     * Test EventHub observe all events failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_ObserveAll(&eventHub, NULL);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
}

TEST_F(EventHubTest, EventHubObserverAllHubLockNull)
{
    /** @testcase{ eventHub::EventHubObserverAllHubLockNull: }
     * Test EventHub observe all events failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;
    eventHub.lock = (SemaphoreHandle_t)0;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_ObserveAll(&eventHub, TestObserver_A);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
}

TEST_F(EventHubTest, EventHubObserverAllNotNullHubObserverCount)
{
    /** @testcase{ eventHub::EventHubObserverAllNotNullHubObserverCount: }
     * Test EventHub observe failures
     */
    EventHub_T eventHub;
    Retcode_T retVal = RETCODE_OK;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)1;
    eventHub.lock = (SemaphoreHandle_t)1;
    eventHub.observerCount = CONFIG_EVENTHUB_MAX_OBSERVERS;

    retVal = EventHub_ObserveAll(&eventHub, TestObserver_A);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(EventHubTest, EventHubObserverAllSemTakeNotPass)
{
    /** @testcase{ eventHub::EventHubObserverAllSemTakeNotPass: }
     * Test EventHub observe failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;
    eventHub.observerCount = 0;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_ObserveAll(&eventHub, TestObserver_A);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
}

TEST_F(EventHubTest, EventHubObserverAllSemGiveNotPass)
{
    /** @testcase{ eventHub::EventHubObserverAllSemGiveNotPass: }
     * Test EventHub observe failures
     */
    Retcode_T retVal = RETCODE_OK;
    EventHub_T eventHub;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)0;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_ObserveAll(&eventHub, TestObserver_A);

    EXPECT_NE(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(EventHubTest, EventHub_Notify_Success)
{
    /** @testcase{ eventHub::EventHub_Notify_Success: }
     * Test EventHub notify success
     */

    EventHub_T eventHub;
    Retcode_T rc;
    uint32_t expectedObservers = 0;

    eventReceived_A = false;
    dataCorrect_A = false;
    eventReceived_B = false;
    dataCorrect_B = false;
    eventReceived_All = false;
    dataCorrect_All = false;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)1;
    (void)EventHub_Initialize(&eventHub);
    (void)EventHub_Observe(&eventHub, TestObserver_A, (TaskEvent_T)gTestEvent);
    expectedObservers++;
    /* Depending on the config, we might not have enough slots for another observe */
#if (CONFIG_EVENTHUB_MAX_OBSERVERS >= 2)
    (void)EventHub_Observe(&eventHub, TestObserver_B, (TaskEvent_T)gTestEvent);
    expectedObservers++;
#endif
#if (CONFIG_EVENTHUB_MAX_OBSERVERS >= 3)
    (void)EventHub_ObserveAll(&eventHub, TestObserver_All);
    expectedObservers++;
#endif
    rc = EventHub_Notify(&eventHub, (TaskEvent_T)gTestEvent, (const void *)gTestDataPtr);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
    EXPECT_EQ(expectedObservers, eventHub.observerCount);
    EXPECT_EQ(true, eventReceived_A);
    EXPECT_EQ(true, dataCorrect_A);
#if (CONFIG_EVENTHUB_MAX_OBSERVERS >= 2)
    EXPECT_EQ(true, eventReceived_B);
    EXPECT_EQ(true, dataCorrect_B);
#endif
#if (CONFIG_EVENTHUB_MAX_OBSERVERS >= 3)
    EXPECT_EQ(true, eventReceived_All);
    EXPECT_EQ(true, dataCorrect_All);
#endif
}

TEST_F(EventHubTest, EventHubNotifyHubNull)
{
    /** @testcase{ eventHub::EventHubNotifyHubNull: }
     * Test EventHub notify failures
     */
    EventHub_T eventHub;
    Retcode_T retVal = RETCODE_OK;
    eventReceived_A = false;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t)1;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)1;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_Observe(&eventHub, TestObserver_A, (TaskEvent_T)gTestEvent);
    retVal = EventHub_Notify(NULL, (TaskEvent_T)gTestEvent, NULL);

    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(EventHubTest, EventHubNotifyHubLockNull)
{
    /** @testcase{ eventHub::EventHubNotifyHubLockNull: }
     * Test EventHub notify failures
     */
    EventHub_T eventHub;
    Retcode_T retVal = RETCODE_OK;
    eventHub.lock = (SemaphoreHandle_t)0;

    retVal = EventHub_Initialize(&eventHub);
    retVal = EventHub_Observe(&eventHub, TestObserver_A, (TaskEvent_T)gTestEvent);
    retVal = EventHub_Notify(&eventHub, (TaskEvent_T)gTestEvent, NULL);

    EXPECT_NE(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
}

TEST_F(EventHubTest, EventHubNotifySemTakeNotPass)
{
    /** @testcase{ eventHub::EventHubNotifySemTakeNotPass: }
     * Test EventHub notify failures
     */
    EventHub_T eventHub;
    Retcode_T retVal = RETCODE_OK;
    eventReceived_A = false;
    eventHub.observerCount = 0;
    xSemaphoreTake_fake.return_val = (uint32_t)1;

    retVal = EventHub_Notify(&eventHub, (TaskEvent_T)gTestEvent, NULL);

    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(true, eventReceived_A);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
}

TEST_F(EventHubTest, EventHubNotifySemGiveNotPass)
{
    /** @testcase{ eventHub::EventHubNotifySemGiveNotPass: }
     * Test EventHub notify failures
     */
    EventHub_T eventHub;
    Retcode_T retVal = RETCODE_OK;
    eventHub.observerCount = 0;
    xSemaphoreTake_fake.return_val = (uint32_t)1;
    xSemaphoreGive_fake.return_val = (uint32_t)0;

    retVal = EventHub_Notify(&eventHub, (TaskEvent_T)gTestEvent, NULL);

    EXPECT_NE(RETCODE_NULL_POINTER, Retcode_GetCode(retVal));
    EXPECT_NE(RETCODE_UNINITIALIZED, Retcode_GetCode(retVal));
    EXPECT_EQ(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retVal));
    EXPECT_NE(true, eventReceived_A);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}
#else
}
#endif /* if KISO_FEATURE_EVENTHUB */