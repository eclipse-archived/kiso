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
 *      Module test specification for the RingBuffer_unittest.cc module.
 * 
 * @details
 *      The unit test file template follows the Four-Phase test pattern.
 * 
 * @file
 **/

/* Setup testing framework ************************************************** */

/* Include gtest interface */
#include <gtest.h>

#include <string.h>
#include <unistd.h>

/* Start of global scope symbol and fake definitions section */
extern "C"
{
/* Setup compile time configuration defines */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_RINGBUFFER

#if KISO_FEATURE_RINGBUFFER

/* Include faked interfaces */
#include "Kiso_Assert_th.hh"
#include "Kiso_Retcode_th.hh"

/* Include module under test */
#define inline
#include "RingBuffer.c"
#undef inline

    /* End of global scope symbol and fake definitions section */
}

/* Define a fake call-back function for the frame processor related tests */
Retcode_T RunTimeError = RETCODE_OK;
static void Retcode_CustomRaiseError(Retcode_T error)
{
    RunTimeError = error;
}

/* Create test fixture initializing all variables automatically */

#define TEST_LOW_BUFFER_SIZE (15U)
#define TEST_TOTAL_DATA_TO_BE_READ_WRITTEN (100U)
#define TEST_PRODUCER_WRITE_BLOCKS (TEST_TOTAL_DATA_TO_BE_READ_WRITTEN / 4U)

class UartRingBuffer_InitTest : public testing::Test
{
public:
    RingBuffer_T ringBuffer;
    uint8_t localBuffer[TEST_LOW_BUFFER_SIZE];

protected:
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
        Retcode_RaiseError_fake.custom_fake = Retcode_CustomRaiseError;
        memset(&ringBuffer, 0, sizeof(RingBuffer_T));
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* Nothing to do if clean up is not required */
    }
};

/* Specify test cases ******************************************************* */

/**
 *  @brief
 *      Initialize the buffer and ensure zero values.
 *  
 *   
 */
TEST_F(UartRingBuffer_InitTest, BufferInit)
{
    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));

    /* Validate implementation internal details*/
    EXPECT_EQ(localBuffer, ringBuffer.Base);
    EXPECT_EQ(UINT32_C(0), ringBuffer.ReadIndex);
    EXPECT_EQ(UINT32_C(0), ringBuffer.WriteIndex);
    EXPECT_EQ(sizeof(localBuffer), ringBuffer.Size);
}

TEST_F(UartRingBuffer_InitTest, BufferInitNullCheck)
{
    RingBuffer_Initialize(NULL, localBuffer, sizeof(localBuffer));

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM), RunTimeError);
    EXPECT_EQ(1U, Retcode_RaiseError_fake.call_count);
    RESET_FAKE(Retcode_RaiseError);

    RingBuffer_Initialize(&ringBuffer, NULL, sizeof(localBuffer));

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM), RunTimeError);
    EXPECT_EQ(1U, Retcode_RaiseError_fake.call_count);
    RESET_FAKE(Retcode_RaiseError);

    RingBuffer_Initialize(&ringBuffer, NULL, sizeof(localBuffer));

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM), RunTimeError);
    EXPECT_EQ(1U, Retcode_RaiseError_fake.call_count);
    RESET_FAKE(Retcode_RaiseError);

    RingBuffer_Initialize(&ringBuffer, localBuffer, 0U);

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM), RunTimeError);
    EXPECT_EQ(1U, Retcode_RaiseError_fake.call_count);
    RESET_FAKE(Retcode_RaiseError);
}

TEST_F(UartRingBuffer_InitTest, BufferWriteSufficientSpace)
{
    uint8_t dummyData[TEST_LOW_BUFFER_SIZE - 1];
    uint32_t written = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));
    memset(dummyData, 'a', sizeof(dummyData));

    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(sizeof(dummyData), written);
}

TEST_F(UartRingBuffer_InitTest, BufferWriteInsufficientSpace)
{
    uint8_t dummyData[TEST_LOW_BUFFER_SIZE + 1];
    uint32_t written = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));
    memset(dummyData, 'a', sizeof(dummyData));

    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(ringBuffer.Size - 1, written);

    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(0U, written);
}

TEST_F(UartRingBuffer_InitTest, BufferWriteMultiple)
{
    uint8_t dummyData[TEST_LOW_BUFFER_SIZE - TEST_LOW_BUFFER_SIZE / 2 - 1];
    uint32_t written = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));
    memset(dummyData, 'a', sizeof(dummyData));

    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(sizeof(dummyData), written);

    memset(dummyData, 'b', sizeof(dummyData));
    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(sizeof(dummyData), written);
}

TEST_F(UartRingBuffer_InitTest, BufferWriteMultipleInsufficientSpace)
{
    uint8_t dummyData[TEST_LOW_BUFFER_SIZE - TEST_LOW_BUFFER_SIZE / 2 - 2];
    uint32_t written = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));
    memset(dummyData, 'a', sizeof(dummyData));

    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(sizeof(dummyData), written);

    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(sizeof(dummyData), written);

    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(ringBuffer.Size - 2 * sizeof(dummyData) - 1, written);

    written = RingBuffer_Write(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(0U, written);
}

TEST_F(UartRingBuffer_InitTest, BufferReadOnEmpty)
{
    uint8_t dummyData[TEST_LOW_BUFFER_SIZE - 1];
    uint32_t read = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));

    read = RingBuffer_Read(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(0U, read);
}

TEST_F(UartRingBuffer_InitTest, BufferReadOnEmptyNullCheck)
{
    uint8_t dummyData[TEST_LOW_BUFFER_SIZE - 1];
    uint32_t read = 0;

    read = RingBuffer_Read(NULL, dummyData, sizeof(dummyData));
    EXPECT_EQ(0U, read);

    read = RingBuffer_Read(&ringBuffer, NULL, sizeof(dummyData));
    EXPECT_EQ(0U, read);

    read = RingBuffer_Read(NULL, NULL, sizeof(dummyData));
    EXPECT_EQ(0U, read);
}

TEST_F(UartRingBuffer_InitTest, BufferReadMultipleOnEmpty)
{
    uint8_t dummyData[TEST_LOW_BUFFER_SIZE - 1];
    uint32_t nRead = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));

    nRead = RingBuffer_Read(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(0U, nRead);

    nRead = RingBuffer_Read(&ringBuffer, dummyData, sizeof(dummyData));
    EXPECT_EQ(0U, nRead);
}

TEST_F(UartRingBuffer_InitTest, BufferReadFromFull)
{
    uint8_t writeData[TEST_LOW_BUFFER_SIZE - 1];
    uint8_t readData[TEST_LOW_BUFFER_SIZE - 1];
    uint32_t written = 0;
    uint32_t nRead = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));
    memset(writeData, 'a', sizeof(writeData));
    memset(readData, '0', sizeof(readData));

    written = RingBuffer_Write(&ringBuffer, writeData, sizeof(writeData));
    EXPECT_EQ(ringBuffer.Size - 1, written);

    nRead = RingBuffer_Read(&ringBuffer, readData, sizeof(readData));
    EXPECT_EQ(written, nRead);

    EXPECT_TRUE(memcmp(writeData, readData, written) == 0);

    nRead = RingBuffer_Read(&ringBuffer, readData, sizeof(readData));
    EXPECT_EQ(0U, nRead);
}

TEST_F(UartRingBuffer_InitTest, BufferMultipleReadWrite)
{
    uint8_t writeData[TEST_LOW_BUFFER_SIZE - 2];
    uint8_t readData[TEST_LOW_BUFFER_SIZE - 2];
    uint32_t written = 0;
    uint32_t nRead = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));

    for (unsigned int i = 0; i < TEST_LOW_BUFFER_SIZE; ++i)
    {
        memset(writeData, ('a' + i) % 256, sizeof(writeData));
        memset(readData, '0', sizeof(readData));

        written = RingBuffer_Write(&ringBuffer, writeData, sizeof(writeData));
        EXPECT_EQ(sizeof(writeData), written);

        nRead = RingBuffer_Read(&ringBuffer, readData, sizeof(readData));
        EXPECT_EQ(written, nRead);

        EXPECT_TRUE(memcmp(writeData, readData, written) == 0);

        nRead = RingBuffer_Read(&ringBuffer, readData, sizeof(readData));
        EXPECT_EQ(0U, nRead);
    }
}

TEST_F(UartRingBuffer_InitTest, BufferMultipleReadWriteOddSize)
{
    uint8_t writeData[TEST_LOW_BUFFER_SIZE - TEST_LOW_BUFFER_SIZE / 2 - 2];
    uint8_t readData[TEST_LOW_BUFFER_SIZE - TEST_LOW_BUFFER_SIZE / 2 - 2];
    uint32_t written = 0;
    uint32_t nRead = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));

    for (unsigned int i = 0; i < 10; ++i)
    {
        memset(writeData, 'a' + i, sizeof(writeData));
        memset(readData, '0', sizeof(readData));

        written = RingBuffer_Write(&ringBuffer, writeData, sizeof(writeData));
        EXPECT_EQ(sizeof(writeData), written);

        nRead = RingBuffer_Read(&ringBuffer, readData, sizeof(readData));
        EXPECT_EQ(written, nRead);

        EXPECT_TRUE(memcmp(writeData, readData, written) == 0);

        nRead = RingBuffer_Read(&ringBuffer, readData, sizeof(readData));
        EXPECT_EQ(0U, nRead);
    }
}

TEST_F(UartRingBuffer_InitTest, BufferMultipleReadWriteNonSame)
{
    uint8_t writeData[TEST_LOW_BUFFER_SIZE / 4];
    uint8_t readData[2 * sizeof(writeData)];
    uint32_t written = 0;
    uint32_t nRead = 0;

    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));
    memset(writeData, 'a', sizeof(writeData));
    memset(readData, '0', sizeof(readData));

    for (unsigned int i = 0; i < 10; ++i)
    {
        written = RingBuffer_Write(&ringBuffer, writeData, sizeof(writeData));
        EXPECT_EQ(sizeof(writeData), written);

        written = RingBuffer_Write(&ringBuffer, writeData, sizeof(writeData));
        EXPECT_EQ(sizeof(writeData), written);

        nRead = RingBuffer_Read(&ringBuffer, readData, sizeof(readData));
        EXPECT_EQ(sizeof(readData), nRead);

        EXPECT_TRUE(memcmp(writeData, readData, written) == 0);
        EXPECT_TRUE(memcmp(writeData, &readData[sizeof(writeData)], written) == 0);

        nRead = RingBuffer_Read(&ringBuffer, readData, sizeof(readData));
        EXPECT_EQ(0U, nRead);
    }
}

TEST_F(UartRingBuffer_InitTest, RingBufferResetNullCheck)
{

    RingBuffer_Reset(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM), RunTimeError);
    EXPECT_EQ(1U, Retcode_RaiseError_fake.call_count);
    RESET_FAKE(Retcode_RaiseError);
}

TEST_F(UartRingBuffer_InitTest, RingBufferReset)
{
    RingBuffer_T ringBufferReset;
    uint8_t RingbufferData[5] = {'b', 'o', 's', 'c', 'h'};

    ringBufferReset.Base = RingbufferData;
    ringBufferReset.WriteIndex = 5;
    ringBufferReset.ReadIndex = 6;
    ringBufferReset.Size = 5;

    RingBuffer_Reset(&ringBufferReset);

    EXPECT_EQ(0U, RingbufferData[0]);
    EXPECT_EQ(0U, RingbufferData[1]);
    EXPECT_EQ(0U, RingbufferData[2]);
    EXPECT_EQ(0U, RingbufferData[3]);
    EXPECT_EQ(0U, RingbufferData[4]);
    EXPECT_EQ(0U, ringBufferReset.WriteIndex);
    EXPECT_EQ(0U, ringBufferReset.ReadIndex);
    EXPECT_EQ(5U, ringBufferReset.Size);
}

TEST_F(UartRingBuffer_InitTest, RingBuffer_Reset)
{
    RingBuffer_T ringBufferReset;
    uint8_t index;
    uint8_t RingbufferData[101];

    for (index = 0; index < 100; index++)
    {
        RingbufferData[index] = index;
    }
    ringBufferReset.Base = RingbufferData;
    ringBufferReset.WriteIndex = index;
    ringBufferReset.ReadIndex = index;
    ringBufferReset.Size = index;

    RingBuffer_Reset(&ringBufferReset);
    for (index = 0; index < 100; index++)
    {
        EXPECT_EQ(0U, RingbufferData[index]);
    }

    EXPECT_EQ(0U, ringBufferReset.WriteIndex);
    EXPECT_EQ(0U, ringBufferReset.ReadIndex);
    EXPECT_EQ(index, ringBufferReset.Size);
}

#if 0 /** \todo: provide pthread capabilities */

static void* consume(void* arg)
{
    uint8_t readData[TEST_TOTAL_DATA_TO_BE_READ_WRITTEN];
    uint32_t nRead = 0;
    uint32_t totalRead = 0;
    UartRingBuffer_InitTest* instance = (UartRingBuffer_InitTest*) arg;

    memset(readData, '0', sizeof(readData));

    EXPECT_EQ(sizeof(instance->localBuffer) - 1, RingBuffer_getFree(&(instance->ringBuffer)));
    EXPECT_EQ(0, RingBuffer_getUsed(&(instance->ringBuffer)));

    while (totalRead < TEST_TOTAL_DATA_TO_BE_READ_WRITTEN)
    {
        nRead = RingBuffer_Read(&(instance->ringBuffer), readData + totalRead, sizeof(readData) - totalRead);
        totalRead += nRead;
        usleep(500000); // 0.5 second = 500 000 microseconds
    }

    for (unsigned int i = 0, j = 0; i < sizeof(readData); ++i, j = i / TEST_PRODUCER_WRITE_BLOCKS)
    {
        EXPECT_EQ(j, readData[i]);
    }

    return NULL;
}

static void* produce(void* arg)
{
    uint8_t writeData[TEST_PRODUCER_WRITE_BLOCKS];
    uint32_t written = 0;
    uint32_t totalWritten = 0;
    uint8_t nWrites = 0;
    UartRingBuffer_InitTest* instance = (UartRingBuffer_InitTest*) arg;

    EXPECT_EQ(sizeof(instance->localBuffer) - 1, RingBuffer_getFree(&(instance->ringBuffer)));
    EXPECT_EQ(0, RingBuffer_getUsed(&(instance->ringBuffer)));

    while (totalWritten < TEST_TOTAL_DATA_TO_BE_READ_WRITTEN)
    {
        memset(writeData, nWrites, sizeof(writeData));

        written = RingBuffer_Write(&(instance->ringBuffer), writeData, sizeof(writeData));
        EXPECT_EQ(sizeof(writeData), written);

        usleep(200000); // 0.2 second = 200 000 microseconds
    }

    return NULL;
}

TEST_F(UartRingBuffer_InitTest, ConsumerProducerAsyncAccess)
{
    RingBuffer_Initialize(&ringBuffer, localBuffer, sizeof(localBuffer));

    EXPECT_EQ(sizeof(localBuffer) - 1, RingBuffer_getFree(&ringBuffer));
    EXPECT_EQ(0, RingBuffer_getUsed(&ringBuffer));

    pthread_t consumerThread;
    pthread_t producerThread;

    pthread_create(&consumerThread, NULL, consume, this);
    pthread_create(&producerThread, NULL, produce, this);

    pthread_join(consumerThread, NULL);
    pthread_join(producerThread, NULL);
}
#endif
#else
}
#endif /* if KISO_FEATURE_RINGBUFFER */
