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
 * @file
 *
 *
 *  Module test specification for the BCDS_Queue module.
 *
 * ****************************************************************************/

#include <gtest.h>

extern "C"
{
#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_QUEUE

#if BCDS_FEATURE_QUEUE

#include "BCDS_Basics.h"
#include "BCDS_Assert_th.hh"
#include "BCDS_Retcode_th.hh"
#include "FreeRTOS_th.hh"
#include "semphr_th.hh"
#include "task_th.hh"

#include "Queue.c"
}

#define QUEUE_BUFFER_SIZE               128
#define QUEUE_MESSAGE_COUNT             4
#define QUEUE_MESSAGE_COUNT_EXPECTED1   UINT32_C(6)
#define QUEUE_MESSAGE_COUNT_EXPECTED2   UINT32_C(4)
#define QUEUE_MESSAGE_COUNT_EXPECTED3   UINT32_C(4)
#define QUEUE_TEST_ITEM_MAGIC           UINT32_C(0x11335577)

typedef struct
{
    uint32_t Magic;
    uint32_t PayloadSize;
    char Payload[];
} QueueTestItem_T;

const char *QUEUE_MESSAGES[QUEUE_MESSAGE_COUNT] = { "These ", "are test ", "messages, which are", "used by the unit test" };
static uint8_t QueueBuffer[QUEUE_BUFFER_SIZE];

/* create test fixture initializing all variables automatically */
class BCDS_Queue: public testing::Test
{
protected:
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE (xSemaphoreCreateBinary);
        RESET_FAKE (xSemaphoreCreateMutex);
        RESET_FAKE (xSemaphoreTake);
        RESET_FAKE (xSemaphoreGive);
        RESET_FAKE (vQueueDelete);
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

/* specify test cases ******************************************************* */
TEST_F(BCDS_Queue, Queue_Create_InvalidParam1)
{
    /** @testcase{ BCDS_Queue::Queue_Create_InvalidParam1: }
     *
     * API is used to check if the initialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(NULL, QueueBuffer, sizeof(QueueBuffer));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Create_InvalidParam2)
{
    /** @testcase{ BCDS_Queue::Queue_Create_InvalidParam2: }
     *
     * API is used to check if the initialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(&q, NULL, QUEUE_BUFFER_SIZE);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Create_InvalidParam3)
{
    /** @testcase{ BCDS_Queue::Queue_Create_InvalidParam3: }
     *
     * API is used to check if the initialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(&q, QueueBuffer, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Create_SemaphoreBinaryFail)
{
    /** @testcase{ BCDS_Queue::Queue_Create_SemaphoreBinaryFail: }
     *
     * API is used to check if the initialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) NULL;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Create_MutexFail)
{
    /** @testcase{ BCDS_Queue::Queue_Create_MutexFail: }
     *
     * API is used to check if the initialize function will detect invalid parameters
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Create_Success)
{
    /** @testcase{ BCDS_Queue::Queue_Create_Success: }
     *
     * API is used to check if the initialize function will execute correctly
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Delete)
{
    /** @testcase{ BCDS_Queue::Queue_Delete: }
     *
     * API is used to check if the deinitialize function will detect invalid parameters
     */

    /* SETUP & EXECISE */
    Retcode_T retcode = Queue_Delete(NULL);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));

    Queue_T q;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    EXPECT_EQ(RETCODE_OK, Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer)));
    EXPECT_EQ(RETCODE_OK, Queue_Delete(&q));
    EXPECT_EQ(UINT32_C(2), vQueueDelete_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Put_Fail)
{
    /** @testcase{ BCDS_Queue::Queue_Put_Fail: }
     *
     * API is used to check if Queue_Put() function will detect out of resources condition
     * (the queue needs room for QueueItem_T element and message)
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    uint8_t msg[QUEUE_BUFFER_SIZE];
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Put(&q, msg, sizeof(msg), NULL, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Put_Success)
{
    /** @testcase{ BCDS_Queue::Queue_Put_Success: }
     *
     * API is used to check if Queue_Put() function will execute successfully
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Put(&q, QUEUE_MESSAGES[0], strlen(QUEUE_MESSAGES[0]), NULL, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(2), xSemaphoreGive_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Clear_Success)
{
    /** @testcase{ BCDS_Queue::Queue_Clear_Success: }
     *
     * API is used to check if Queue_Clear() function will execute successfully
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    /* EXECISE & VERIFY: call relevant production code Interface with appropriate test inputs  */
    EXPECT_EQ(RETCODE_OK, Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer)));
    EXPECT_EQ(RETCODE_OK, Queue_Put(&q, QUEUE_MESSAGES[0], strlen(QUEUE_MESSAGES[0]), NULL, 0));
    EXPECT_EQ(UINT32_C(1), Queue_Count(&q));
    /* Verify Queue_Clear() */
    Queue_Clear(&q);
    EXPECT_EQ(UINT32_C(0), Queue_Count(&q));
}

TEST_F(BCDS_Queue, Queue_Get_Fail1)
{
    /** @testcase{ BCDS_Queue::Queue_Get_Fail1: }
     *
     * API is used to check if Queue_Get() function will detect no message condition
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;
    char *data;
    uint32_t size;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retcode = Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    xSemaphoreTake_fake.return_val = pdFALSE;
    retcode = Queue_Get(&q, (void **)&data, &size, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_SEMAPHORE_ERROR, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(3), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Get_Fail2)
{
    /** @testcase{ BCDS_Queue::Queue_Get_Fail2: }
     *
     * API is used to check if Queue_Get() function will detect message with race condition
     * (message being purged when sema received)
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;
    char *data;
    uint32_t size;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    EXPECT_EQ(RETCODE_OK, Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer)));

    xSemaphoreTake_fake.return_val = pdTRUE;
    retcode = Queue_Get(&q, (void **)&data, &size, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_UNEXPECTED_BEHAVIOR, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(4), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(3), xSemaphoreGive_fake.call_count);
}

TEST_F(BCDS_Queue, Queue_Get_Success1)
{
    /** @testcase{ BCDS_Queue::Queue_Get_Success1: }
     *
     * API is used to check if Queue_Get() function will execute correctly (no Payload)
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;
    const char *msg = QUEUE_MESSAGES[0];
    uint32_t msg_len = strlen(msg);
    char *data = NULL;
    uint32_t size = 0;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    EXPECT_EQ(RETCODE_OK, Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer)));
    EXPECT_EQ(RETCODE_OK, Queue_Put(&q, msg, msg_len, NULL, 0));
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(2), xSemaphoreGive_fake.call_count);

    retcode = Queue_Get(&q, (void **)&data, &size, 0);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(3), xSemaphoreGive_fake.call_count);
    EXPECT_EQ(true, NULL != data);
    EXPECT_EQ(true, 0 != size);
    EXPECT_EQ(true, data != msg);
    EXPECT_EQ(true, size == msg_len);
    EXPECT_EQ(0, memcmp(data, msg, size));
    EXPECT_EQ(UINT32_C(1), Queue_Count(&q));

    /* Verify Queue_Purge() */
    EXPECT_EQ(RETCODE_OK, Queue_Purge(&q));
    EXPECT_EQ(UINT32_C(0), Queue_Count(&q));
}

TEST_F(BCDS_Queue, Queue_Get_Success2)
{
    /** @testcase{ BCDS_Queue::Queue_Get_Success2: }
     *
     * API is used to check if Queue_Get() function will execute correctly when queue is full
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;
    uint32_t msg_index, msg_count;

    char *data = NULL;
    uint32_t size = 0;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    EXPECT_EQ(RETCODE_OK, Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer)));

    for(msg_count = 0, msg_index = 0;; msg_count++)
    {
        retcode = Queue_Put(&q, QUEUE_MESSAGES[msg_index], strlen(QUEUE_MESSAGES[msg_index]), NULL, 0);
        if (RETCODE_OK != retcode)
        {
            break;
        }
        msg_index = (msg_index < QUEUE_MESSAGE_COUNT - 1) ? msg_index + 1 : 0;
    }

    EXPECT_EQ(QUEUE_MESSAGE_COUNT_EXPECTED1, msg_count);
    EXPECT_EQ(QUEUE_MESSAGE_COUNT_EXPECTED1, Queue_Count(&q));

    for(msg_index = 0; 0 != msg_count; msg_count--)
    {
        retcode = Queue_Get(&q, (void **)&data, &size, 0);

        EXPECT_EQ(RETCODE_OK, retcode);
        if (RETCODE_OK != retcode)
        {
            return;
        }

        const char *msg = QUEUE_MESSAGES[msg_index];
        uint32_t msg_len = strlen(msg);
        msg_index = (msg_index < QUEUE_MESSAGE_COUNT - 1) ? msg_index + 1 : 0;

        /* VERIFY : Compare the expected with actual */
        EXPECT_EQ(true, NULL != data);
        EXPECT_EQ(true, 0 != size);
        EXPECT_EQ(true, data != msg);
        EXPECT_EQ(true, size == msg_len);
        EXPECT_EQ(0, memcmp(data, msg, size));

        retcode = Queue_Purge(&q);
        EXPECT_EQ(RETCODE_OK, retcode);
        if (RETCODE_OK != retcode)
        {
            return;
        }
    }
}

TEST_F(BCDS_Queue, Queue_Get_Success3)
{
    /** @testcase{ BCDS_Queue::Queue_Get_Success3: }
     *
     * API is used to check if Queue_Get() function will execute correctly when queue is written in front
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;
    uint32_t msg_count, msg_index_read, msg_index_write;

    char *data = NULL;
    uint32_t size = 0;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    EXPECT_EQ(RETCODE_OK, Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer)));

    /* First fill the queue completely */
    for(msg_count = 0, msg_index_write = 0;; msg_count++)
    {
        retcode = Queue_Put(&q, QUEUE_MESSAGES[msg_index_write], strlen(QUEUE_MESSAGES[msg_index_write]), NULL, 0);
        if (RETCODE_OK != retcode)
        {
            break;
        }
        msg_index_write = (msg_index_write < QUEUE_MESSAGE_COUNT - 1) ? msg_index_write + 1 : 0;
    }

    EXPECT_EQ(QUEUE_MESSAGE_COUNT_EXPECTED1, msg_count);
    EXPECT_EQ(QUEUE_MESSAGE_COUNT_EXPECTED1, Queue_Count(&q));

    /* Read from the queue, but the last message */
    for(msg_index_read = 0; msg_count > 1; msg_count--)
    {
        retcode = Queue_Get(&q, (void **)&data, &size, 0);

        EXPECT_EQ(RETCODE_OK, retcode);
        if (RETCODE_OK != retcode)
        {
            return;
        }

        const char *msg = QUEUE_MESSAGES[msg_index_read];
        uint32_t msg_len = strlen(msg);
        msg_index_read = (msg_index_read < QUEUE_MESSAGE_COUNT - 1) ? msg_index_read + 1 : 0;

        /* VERIFY : Compare the expected with actual */
        EXPECT_EQ(true, NULL != data);
        EXPECT_EQ(true, 0 != size);
        EXPECT_EQ(true, data != msg);
        EXPECT_EQ(true, size == msg_len);
        EXPECT_EQ(0, memcmp(data, msg, size));

        retcode = Queue_Purge(&q);
        EXPECT_EQ(RETCODE_OK, retcode);
        if (RETCODE_OK != retcode)
        {
            return;
        }
    }

    EXPECT_EQ(UINT32_C(1), Queue_Count(&q));

    /* Continue writing to the queue with data, which previously didn't fit */
    for(msg_count = 0;; msg_count++)
    {
        retcode = Queue_Put(&q, QUEUE_MESSAGES[msg_index_write], strlen(QUEUE_MESSAGES[msg_index_write]), NULL, 0);
        if (RETCODE_OK != retcode)
        {
            break;
        }
        msg_index_write = (msg_index_write < QUEUE_MESSAGE_COUNT - 1) ? msg_index_write + 1 : 0;
    }

    EXPECT_EQ(QUEUE_MESSAGE_COUNT_EXPECTED2, msg_count);

    /* Read out the rest messages from the queue */
    ++msg_count;
    for(; 0 != msg_count; msg_count--)
    {
        retcode = Queue_Get(&q, (void **)&data, &size, 0);

        EXPECT_EQ(RETCODE_OK, retcode);
        if (RETCODE_OK != retcode)
        {
            return;
        }

        const char *msg = QUEUE_MESSAGES[msg_index_read];
        uint32_t msg_len = strlen(msg);
        msg_index_read = (msg_index_read < QUEUE_MESSAGE_COUNT - 1) ? msg_index_read + 1 : 0;

        /* VERIFY : Compare the expected with actual */
        EXPECT_EQ(true, NULL != data);
        EXPECT_EQ(true, 0 != size);
        EXPECT_EQ(true, data != msg);
        EXPECT_EQ(true, size == msg_len);
        EXPECT_EQ(0, memcmp(data, msg, size));

        retcode = Queue_Purge(&q);
        EXPECT_EQ(RETCODE_OK, retcode);
        if (RETCODE_OK != retcode)
        {
            return;
        }
    }

    EXPECT_EQ(UINT32_C(0), Queue_Count(&q));
}

TEST_F(BCDS_Queue, Queue_Get_Success4)
{
    /** @testcase{ BCDS_Queue::Queue_Get_Success4: }
     *
     * API is used to check if Queue_Get() function will execute correctly with Payload
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Queue_T q;
    QueueTestItem_T m;
    Retcode_T retcode;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t) 1;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;
    uint32_t msg_index, msg_count;

    QueueTestItem_T *data = NULL;
    uint32_t data_size = 0;

    /* EXECISE & VERIFY: call relevant production code Interface with appropriate test inputs  */
    EXPECT_EQ(RETCODE_OK, Queue_Create(&q, QueueBuffer, sizeof(QueueBuffer)));

    m.Magic = QUEUE_TEST_ITEM_MAGIC;
    for(msg_count = 0, msg_index = 0;; msg_count++)
    {
        const char *msg = QUEUE_MESSAGES[msg_index];
        uint32_t msg_len = strlen(msg);

        m.PayloadSize = msg_len;
        retcode = Queue_Put(&q, &m, sizeof(m), msg, msg_len);
        if (RETCODE_OK != retcode)
        {
            break;
        }
        msg_index = (msg_index < QUEUE_MESSAGE_COUNT - 1) ? msg_index + 1 : 0;
    }

    EXPECT_EQ(QUEUE_MESSAGE_COUNT_EXPECTED3, msg_count);
    EXPECT_EQ(QUEUE_MESSAGE_COUNT_EXPECTED3, Queue_Count(&q));

    for(msg_index = 0; 0 != msg_count; msg_count--)
    {
        retcode = Queue_Get(&q, (void **)&data, &data_size, 0);

        EXPECT_EQ(RETCODE_OK, retcode);
        if (RETCODE_OK != retcode)
        {
            return;
        }

        const char *msg = QUEUE_MESSAGES[msg_index];
        uint32_t msg_len = strlen(msg);
        msg_index = (msg_index < QUEUE_MESSAGE_COUNT - 1) ? msg_index + 1 : 0;

        /* VERIFY : Compare the expected with actual */
        EXPECT_EQ(true, NULL != data);
        EXPECT_EQ(true, 0 != data_size);
        EXPECT_EQ(true, QUEUE_TEST_ITEM_MAGIC == data->Magic);
        EXPECT_EQ(true, msg_len == data->PayloadSize);
        EXPECT_EQ(true, msg != data->Payload);
        EXPECT_EQ(0, memcmp(data->Payload, msg, data->PayloadSize));
        EXPECT_EQ(RETCODE_OK, Queue_Purge(&q));
    }
}
#else
}
#endif /* if BCDS_FEATURE_QUEUE */
