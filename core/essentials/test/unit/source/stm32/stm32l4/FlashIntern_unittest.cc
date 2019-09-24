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

/* Include all headers which are needed by this file in the following order:
 * Own public header
 * Own protected header
 * Own private header
 * System header (if necessary)
 * Other headers
 */
#include <gtest.h>

extern "C"
{
#include "Kiso_Basics.h"
#include "Kiso_HAL_th.hh"

#if KISO_FEATURE_FLASH_INTERN
/* include faked interface */
#include "Kiso_RetCode_th.hh"
#include "stm32l4xx_hal_th.hh"
#include "stm32l4xx_hal_flash_th.hh"
#include "stm32l4xx_hal_flash_ex_th.hh"

#undef FLASH_SIZE
#define FLASH_SIZE UINT32_C(0x00080000)

/* Put constant and variable definitions here */

/* Put private function declarations here */

/* Put function implementations here */

/* include module under test */
#include "FlashIntern.c"

} /* extern "C"*/

/* create test fixture initializing all variables automatically */

class MCU_FlashIntern : public testing::Test
{
protected:
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        /* Initialize execution environment */
        RESET_FAKE(HAL_FLASHEx_Erase);
        RESET_FAKE(HAL_FLASH_Unlock);
        RESET_FAKE(HAL_FLASH_Lock);
        RESET_FAKE(HAL_FLASH_Program);
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* Do nothing */
    }
};

/* specify test cases ******************************************************* */

/**
 * This test case evaluates the functionality of checkAddressBounderies()
 */
TEST_F(MCU_FlashIntern, checkAddressBounderies)
{
    /* StartAddress out of bound */
    uint32_t StartAddress = UINT32_C(0x00000000);
    uint32_t Length = UINT32_C(0);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        checkAddressBounderies(StartAddress, StartAddress + Length));

    /* Start and end addresses are the same */
    StartAddress = UINT32_C(0x08000000);
    Length = UINT32_C(0);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        checkAddressBounderies(StartAddress, StartAddress + Length));

    /* End address out of bound */
    StartAddress = UINT32_C(0x08000000);
    Length = FLASH_SIZE + UINT32_C(8);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        checkAddressBounderies(StartAddress, StartAddress + Length));

    /* Success */
    StartAddress = UINT32_C(0x08000000);
    Length = UINT32_C(0x00070000);

    EXPECT_EQ(RETCODE_OK, checkAddressBounderies(StartAddress, StartAddress + Length));
}

/**
 * This test case evaluates the functionality of checkParamAlignment()
 */
TEST_F(MCU_FlashIntern, checkParamAlignment)
{
    /* StartAddress unaligned */
    uint32_t StartAddress = UINT32_C(0x00000001);
    uint32_t Length = UINT32_C(0);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_INVALID_ADDRESS),
        checkParamAlignment(StartAddress, Length));

    /* Unaligned length */
    StartAddress = UINT32_C(0x08000000);
    Length = UINT32_C(1);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM),
        checkParamAlignment(StartAddress, Length));

    /* Success */
    StartAddress = UINT32_C(0x08000000);
    Length = UINT32_C(0x00070000);

    EXPECT_EQ(RETCODE_OK, checkParamAlignment(StartAddress, Length));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Initialize()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Initialize)
{
    MCU_FlashIntern_T flashInternInitStruct;

    EXPECT_EQ(RETCODE_OK, MCU_FlashIntern_Initialize(flashInternInitStruct));
}

/**
 * This test case evaluates the functionality of FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase)
{
    uint32_t StartAddress = UINT32_C(0x00000000);
    uint32_t Length = UINT32_C(0);

    HAL_FLASHEx_Erase_fake.return_val = HAL_ERROR;
    HAL_FLASH_Unlock_fake.return_val = HAL_ERROR;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(0u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase2)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x08000000);

    HAL_FLASHEx_Erase_fake.return_val = HAL_ERROR;
    HAL_FLASH_Unlock_fake.return_val = HAL_ERROR;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(0u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase3)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x00040000);

    HAL_FLASHEx_Erase_fake.return_val = HAL_ERROR;
    HAL_FLASH_Unlock_fake.return_val = HAL_ERROR;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase4)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x00040000);

    HAL_FLASHEx_Erase_fake.return_val = HAL_ERROR;
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_LOCK_FAILED),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase5)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x00040000);

    HAL_FLASHEx_Erase_fake.return_val = HAL_OK;
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_LOCK_FAILED),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase6)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x00060000);

    HAL_FLASHEx_Erase_fake.return_val = HAL_OK;
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_LOCK_FAILED),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(2u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase7)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x00060000);

    HAL_FLASHEx_Erase_fake.return_val = HAL_OK;
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_OK;

    EXPECT_EQ(RETCODE_OK, MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(2u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase8)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x00060000);

    HAL_FLASHEx_Erase_fake.return_val = HAL_ERROR;
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_OK;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ERASE_ERROR),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase9)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x00060000);

    HAL_StatusTypeDef myReturnVals[2] = {HAL_OK, HAL_ERROR};
    SET_RETURN_SEQ(HAL_FLASHEx_Erase, myReturnVals, 2);

    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_OK;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ERASE_ERROR),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(2u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Erase()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Erase10)
{
    uint32_t StartAddress = UINT32_C(0x09000000);
    uint32_t Length = UINT32_C(0);

    HAL_FLASHEx_Erase_fake.return_val = HAL_ERROR;
    HAL_FLASH_Unlock_fake.return_val = HAL_ERROR;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Erase(StartAddress, StartAddress + Length));

    EXPECT_EQ(0u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Read()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Read)
{
    uint32_t StartAddress = UINT32_C(0x00000000);

    uint8_t Buffer[80];
    uint32_t Length = sizeof(Buffer) / sizeof(uint8_t);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Read(StartAddress, Buffer,
                             Length));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Read()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Read1)
{
    uint32_t StartAddress = UINT32_C(0x00000000);

    uint32_t Length = 0;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Read(StartAddress, NULL, Length));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Read()
 */
/* Not executed, because reading from the start address will crash the gtest */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Read2)
{
    //uint32_t StartAddress = UINT32_C(0x08060000);

    //uint8_t Buffer[80];
    //uint32_t Length = sizeof(Buffer) / sizeof(uint8_t);
    //EXPECT_EQ( RETCODE_OK, MCU_FlashIntern_Read(StartAddress, Buffer, Length));
}
/**
 * This test case evaluates the functionality of MCU_FlashIntern_Read()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Read3)
{
    uint32_t StartAddress = UINT32_C(0x08080000);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Read(StartAddress, NULL, 0));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Read()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Read4)
{
    uint32_t StartAddress = UINT32_C(0x08060000);

    uint8_t Buffer[80];
    uint32_t Length = 0x00000001 / sizeof(uint8_t);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM),
        MCU_FlashIntern_Read(StartAddress, Buffer, Length));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Read()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Read5)
{
    uint32_t StartAddress = UINT32_C(0x08060000);

    uint32_t Length = 0x00000008 / sizeof(uint8_t);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER),
        MCU_FlashIntern_Read(StartAddress, NULL, Length));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Write()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Write)
{
    uint32_t StartAddress = UINT32_C(0);
    uint8_t Buffer[80];
    uint32_t Length = sizeof(Buffer) / sizeof(uint8_t);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Write(StartAddress, Buffer,
                              Length));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Write()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Write2)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint8_t Buffer[80];
    uint32_t Length = 0x08000000 / sizeof(uint8_t);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Write(StartAddress, Buffer, Length));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Write()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Write3)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint32_t Length = UINT32_C(0x00000008);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER),
        MCU_FlashIntern_Write(StartAddress, NULL, Length));
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Write()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Write4)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint8_t Buffer[80];
    uint32_t Length = sizeof(Buffer) / sizeof(uint8_t);

    HAL_FLASH_Unlock_fake.return_val = HAL_ERROR;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;
    HAL_FLASH_Program_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED),
        MCU_FlashIntern_Write(StartAddress, Buffer, Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASH_Program_fake.call_count);
    EXPECT_EQ(0u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Write()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Write5)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint8_t Buffer[80];
    uint32_t Length = sizeof(Buffer) / sizeof(uint8_t);

    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;
    HAL_FLASH_Program_fake.return_val = HAL_ERROR;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_LOCK_FAILED),
        MCU_FlashIntern_Write(StartAddress, Buffer, Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Program_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Write()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Write6)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint8_t Buffer[80];
    uint32_t Length = sizeof(Buffer) / sizeof(uint8_t);

    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_ERROR;
    HAL_FLASH_Program_fake.return_val = HAL_OK;

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_LOCK_FAILED),
        MCU_FlashIntern_Write(StartAddress, Buffer, Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(Length / sizeof(uint64_t), HAL_FLASH_Program_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Write()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Write7)
{
    uint32_t StartAddress = UINT32_C(0x08000000);
    uint8_t Buffer[80];
    uint32_t Length = sizeof(Buffer) / sizeof(uint8_t);

    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_OK;
    HAL_FLASH_Program_fake.return_val = HAL_OK;

    EXPECT_EQ(RETCODE_OK,
              MCU_FlashIntern_Write(StartAddress, Buffer, Length));

    EXPECT_EQ(1u, HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(Length / sizeof(uint64_t), HAL_FLASH_Program_fake.call_count);
    EXPECT_EQ(1u, HAL_FLASH_Lock_fake.call_count);
}

/**
 * This test case evaluates the functionality of MCU_FlashIntern_Write()
 */
TEST_F(MCU_FlashIntern, MCU_FlashIntern_Write8)
{
    uint32_t StartAddress = UINT32_C(0x09000000);
    uint8_t Buffer[80];
    uint32_t Length = sizeof(Buffer) / sizeof(uint8_t);

    EXPECT_EQ(
        RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND),
        MCU_FlashIntern_Write(StartAddress, Buffer, Length));
}
#else
}
#endif /* KISO_FEATURE_FLASH_INTERN */
