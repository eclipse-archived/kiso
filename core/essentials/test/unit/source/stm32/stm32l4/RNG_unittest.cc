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

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START
extern "C"
{
/* include faked interfaces */
#include "BCDS_HAL_th.hh"

#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"

#include "stm32l4xx_hal_th.hh"
#include "stm32l4xx_hal_rng_th.hh"

/* setup compile time configuration defines */
#undef BCDS_FEATURE_RNG
#define BCDS_FEATURE_RNG 1
/* include module under test */
#include "RNG.c"
}
/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

class TS_MCU_RNG_Initialize: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        ;

        RESET_FAKE(HAL_RNG_Init);
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TS_MCU_RNG_Initialize, Success)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;

    HAL_RNG_Init_fake.return_val = HAL_OK;

    retcode = MCU_RNG_Initialize(rng);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, HAL_RNG_Init_fake.call_count);
}

TEST_F(TS_MCU_RNG_Initialize, InvalidHandle_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = NULL;

    HAL_RNG_Init_fake.return_val = HAL_OK;

    retcode = MCU_RNG_Initialize(rng);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
    EXPECT_EQ(0U, HAL_RNG_Init_fake.call_count);
}

TEST_F(TS_MCU_RNG_Initialize, HalInit_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;

    HAL_RNG_Init_fake.return_val = HAL_ERROR;

    retcode = MCU_RNG_Initialize(rng);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);
    EXPECT_EQ(1U, HAL_RNG_Init_fake.call_count);
}

class TS_MCU_RNG_Deinitialize: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        ;

        RESET_FAKE(HAL_RNG_DeInit);
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TS_MCU_RNG_Deinitialize, Success)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;

    HAL_RNG_DeInit_fake.return_val = HAL_OK;

    retcode = MCU_RNG_Deinitialize(rng);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, HAL_RNG_DeInit_fake.call_count);
}

TEST_F(TS_MCU_RNG_Deinitialize, InvalidHandle_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = NULL;

    HAL_RNG_DeInit_fake.return_val = HAL_OK;

    retcode = MCU_RNG_Deinitialize(rng);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
    EXPECT_EQ(0U, HAL_RNG_DeInit_fake.call_count);
}

TEST_F(TS_MCU_RNG_Deinitialize, HalDeInit_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;

    HAL_RNG_DeInit_fake.return_val = HAL_ERROR;

    retcode = MCU_RNG_Deinitialize(rng);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);
    EXPECT_EQ(1U, HAL_RNG_DeInit_fake.call_count);
}

static uint8_t* RandomData = NULL;
static uint32_t RandomDataLength = 0;
static uint32_t RandomDataPosition = 0;

HAL_StatusTypeDef Fake_HAL_RNG_GenerateRandomNumber(RNG_HandleTypeDef* rng,
        uint32_t* random32Bits)
{
    BCDS_UNUSED(rng);

    if (RandomDataPosition < RandomDataLength)
    {
        *random32Bits = *((uint32_t*) &(RandomData[RandomDataPosition]));
        RandomDataPosition += sizeof(uint32_t);
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

class TS_MCU_RNG_Generate: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        ;

        RESET_FAKE(HAL_RNG_GenerateRandomNumber);
        HAL_RNG_GenerateRandomNumber_fake.custom_fake =
                Fake_HAL_RNG_GenerateRandomNumber;

        RandomData = NULL;
        RandomDataLength = 0;
        RandomDataPosition = 0;
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TS_MCU_RNG_Generate, Aligned_Success)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;
    uint8_t buffer[512];
    uint32_t length = sizeof(buffer);
    memset(buffer, 0U, length);

    uint8_t expectedData[length];
    for (uint32_t i = 0; i < length; ++i)
    {
        expectedData[i] = (uint8_t) rand();
    }
    RandomData = expectedData;
    RandomDataLength = length;

    retcode = MCU_RNG_Generate(rng, buffer, length);

    EXPECT_EQ(RETCODE_OK, retcode);
    for (uint32_t i = 0; i < length; ++i)
    {
        EXPECT_EQ(expectedData[i], buffer[i]);
    }
}

TEST_F(TS_MCU_RNG_Generate, Unaligned_Success)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;
    uint8_t buffer[512 + 1];
    uint32_t length = sizeof(buffer);
    memset(buffer, 0U, sizeof(buffer));

    uint8_t expectedData[length];
    for (uint32_t i = 0; i < length; ++i)
    {
        expectedData[i] = (uint8_t) rand();
    }
    RandomData = expectedData;
    RandomDataLength = length;

    retcode = MCU_RNG_Generate(rng, buffer, length);

    EXPECT_EQ(RETCODE_OK, retcode);
    for (uint32_t i = 0; i < length; ++i)
    {
        EXPECT_EQ(expectedData[i], buffer[i]);
    }
}

TEST_F(TS_MCU_RNG_Generate, OverflowCheckAligned_Success)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;
    uint8_t buffer[512 + 16];
    uint32_t length = sizeof(buffer) - 16;
    memset(buffer, 0U, length);

    uint8_t expectedData[sizeof(buffer)];
    for (uint32_t i = 0; i < length; ++i)
    {
        expectedData[i] = (uint8_t) rand();
    }
    for (uint32_t i = length; i < sizeof(buffer); i++)
    {
        buffer[i] = 0xAA;
        expectedData[i] = 0xFF;
    }
    RandomData = expectedData;
    RandomDataLength = length;

    retcode = MCU_RNG_Generate(rng, buffer, length);

    EXPECT_EQ(RETCODE_OK, retcode);
    for (uint32_t i = 0; i < length; ++i)
    {
        EXPECT_EQ(expectedData[i], buffer[i]);
    }
    for (uint32_t i = length; i < sizeof(buffer); i++)
    {
        EXPECT_NE(expectedData[i], buffer[i]);
    }
}

TEST_F(TS_MCU_RNG_Generate, OverflowCheckUnaligned_Success)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;
    uint8_t buffer[512 + 10];
    uint32_t length = sizeof(buffer) - 10;
    memset(buffer, 0U, length);

    uint8_t expectedData[sizeof(buffer)];
    for (uint32_t i = 0; i < length; ++i)
    {
        expectedData[i] = (uint8_t) rand();
    }
    for (uint32_t i = length; i < sizeof(buffer); i++)
    {
        buffer[i] = 0xAA;
        expectedData[i] = 0xFF;
    }
    RandomData = expectedData;
    RandomDataLength = length;

    retcode = MCU_RNG_Generate(rng, buffer, length);

    EXPECT_EQ(RETCODE_OK, retcode);
    for (uint32_t i = 0; i < length; ++i)
    {
        EXPECT_EQ(expectedData[i], buffer[i]);
    }
    for (uint32_t i = length; i < sizeof(buffer); i++)
    {
        EXPECT_NE(expectedData[i], buffer[i]);
    }
}

TEST_F(TS_MCU_RNG_Generate, InvalidHandle_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = NULL;
    uint8_t buffer[1024];
    uint32_t length = sizeof(buffer);

    retcode = MCU_RNG_Generate(rng, buffer, length);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_MCU_RNG_Generate, InvalidBuffer_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;
    uint8_t* buffer = NULL;
    uint32_t length = 1024;

    retcode = MCU_RNG_Generate(rng, buffer, length);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_MCU_RNG_Generate, InvalidBuffer_errorPropagation)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_T rng = (RNG_T) 1;
    uint8_t buffer[1024];
    uint32_t length = 1024;

    RESET_FAKE (HAL_RNG_GenerateRandomNumber);
    HAL_RNG_GenerateRandomNumber_fake.return_val = HAL_BUSY;

    /* Length is superior to a word representation. */
    retcode = MCU_RNG_Generate(rng, buffer, length);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), retcode);

    HAL_RNG_GenerateRandomNumber_fake.return_val = HAL_TIMEOUT;
    retcode = MCU_RNG_Generate(rng, buffer, length);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), retcode);

    HAL_RNG_GenerateRandomNumber_fake.return_val = HAL_ERROR;
    retcode = MCU_RNG_Generate(rng, buffer, length);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);

    /* Length is inferior to a word representation. */
    length = sizeof(uint32_t) - 1;
    retcode = MCU_RNG_Generate(rng, buffer, length);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);

    RESET_FAKE (HAL_RNG_GenerateRandomNumber);
}
