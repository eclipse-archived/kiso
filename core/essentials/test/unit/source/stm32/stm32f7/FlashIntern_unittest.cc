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

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START 

/* setup compile time configuration defines */
#define BCDS_FEATURE_FLASH_INTERN          1

/* include faked interfaces */

#include "stm32f7xx_hal_th.hh"

#include "stm32f7xx_hal_flash_ex_th.hh"

#include "CDDK_Stm32f7MemSectorMap_th.hh"

/* include module under test */
#include "FlashIntern.c"

/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

static  FLASH_OBProgramInitTypeDef testOptionByte;

uint32_t sectorStartAddressSB[NB_SB_SECTOR] = {
    ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1, ADDR_FLASH_SB_SECTOR_2, ADDR_FLASH_SB_SECTOR_3,
    ADDR_FLASH_SB_SECTOR_4, ADDR_FLASH_SB_SECTOR_5, ADDR_FLASH_SB_SECTOR_6, ADDR_FLASH_SB_SECTOR_7,
    ADDR_FLASH_SB_SECTOR_8, ADDR_FLASH_SB_SECTOR_9, ADDR_FLASH_SB_SECTOR_10, ADDR_FLASH_SB_SECTOR_11
};

uint32_t sectorEndAddressSB[NB_SB_SECTOR] = {
    ADDR_FLASH_SB_SECTOR_1 - 1, ADDR_FLASH_SB_SECTOR_2 - 1, ADDR_FLASH_SB_SECTOR_3 - 1, ADDR_FLASH_SB_SECTOR_4 - 1,
    ADDR_FLASH_SB_SECTOR_5 - 1, ADDR_FLASH_SB_SECTOR_6 - 1, ADDR_FLASH_SB_SECTOR_7 - 1, ADDR_FLASH_SB_SECTOR_8 - 1,
    ADDR_FLASH_SB_SECTOR_9 - 1, ADDR_FLASH_SB_SECTOR_10 - 1, ADDR_FLASH_SB_SECTOR_11 - 1, FLASH_END
};

uint32_t expectedOptionBytesSB[NB_SB_SECTOR]= {
    OB_WRP_SECTOR_0, OB_WRP_SECTOR_1, OB_WRP_SECTOR_2, OB_WRP_SECTOR_3,
    OB_WRP_SECTOR_4, OB_WRP_SECTOR_5, OB_WRP_SECTOR_6, OB_WRP_SECTOR_7,
    OB_WRP_SECTOR_8, OB_WRP_SECTOR_9, OB_WRP_SECTOR_10, OB_WRP_SECTOR_11
};

uint32_t sectorStartAddressDB[NB_DB_SECTOR] = {
    ADDR_FLASH_DB_SECTOR_0, ADDR_FLASH_DB_SECTOR_1, ADDR_FLASH_DB_SECTOR_2, ADDR_FLASH_DB_SECTOR_3,
    ADDR_FLASH_DB_SECTOR_4, ADDR_FLASH_DB_SECTOR_5, ADDR_FLASH_DB_SECTOR_6, ADDR_FLASH_DB_SECTOR_7,
    ADDR_FLASH_DB_SECTOR_8, ADDR_FLASH_DB_SECTOR_9, ADDR_FLASH_DB_SECTOR_10, ADDR_FLASH_DB_SECTOR_11,
    ADDR_FLASH_DB_SECTOR_12, ADDR_FLASH_DB_SECTOR_13, ADDR_FLASH_DB_SECTOR_14, ADDR_FLASH_DB_SECTOR_15,
    ADDR_FLASH_DB_SECTOR_16, ADDR_FLASH_DB_SECTOR_17, ADDR_FLASH_DB_SECTOR_18, ADDR_FLASH_DB_SECTOR_19,
    ADDR_FLASH_DB_SECTOR_20, ADDR_FLASH_DB_SECTOR_21, ADDR_FLASH_DB_SECTOR_22, ADDR_FLASH_DB_SECTOR_23
};

uint32_t sectorEndAddressDB[NB_DB_SECTOR] = {
    ADDR_FLASH_DB_SECTOR_1 - 1, ADDR_FLASH_DB_SECTOR_2 - 1, ADDR_FLASH_DB_SECTOR_3 - 1, ADDR_FLASH_DB_SECTOR_4 - 1,
    ADDR_FLASH_DB_SECTOR_5 - 1, ADDR_FLASH_DB_SECTOR_6 - 1, ADDR_FLASH_DB_SECTOR_7 - 1, ADDR_FLASH_DB_SECTOR_8 - 1,
    ADDR_FLASH_DB_SECTOR_9 - 1, ADDR_FLASH_DB_SECTOR_10 - 1, ADDR_FLASH_DB_SECTOR_11 -1 , ADDR_FLASH_DB_SECTOR_12 - 1,
    ADDR_FLASH_DB_SECTOR_13 - 1, ADDR_FLASH_DB_SECTOR_14 - 1, ADDR_FLASH_DB_SECTOR_15 - 1, ADDR_FLASH_DB_SECTOR_16 - 1,
    ADDR_FLASH_DB_SECTOR_17 - 1, ADDR_FLASH_DB_SECTOR_18 - 1, ADDR_FLASH_DB_SECTOR_19 - 1, ADDR_FLASH_DB_SECTOR_20 - 1,
    ADDR_FLASH_DB_SECTOR_21 - 1, ADDR_FLASH_DB_SECTOR_22 - 1, ADDR_FLASH_DB_SECTOR_23 - 1, FLASH_END
};

uint32_t expectedOptionBytesDB[NB_DB_SECTOR] = {
    OB_WRP_DB_SECTOR_0, OB_WRP_DB_SECTOR_1, OB_WRP_DB_SECTOR_2, OB_WRP_DB_SECTOR_3, OB_WRP_DB_SECTOR_4, OB_WRP_DB_SECTOR_5,
    OB_WRP_DB_SECTOR_6, OB_WRP_DB_SECTOR_7, OB_WRP_DB_SECTOR_8, OB_WRP_DB_SECTOR_9, OB_WRP_DB_SECTOR_10, OB_WRP_DB_SECTOR_11,
    OB_WRP_DB_SECTOR_12, OB_WRP_DB_SECTOR_13, OB_WRP_DB_SECTOR_14, OB_WRP_DB_SECTOR_15, OB_WRP_DB_SECTOR_16, OB_WRP_DB_SECTOR_17,
    OB_WRP_DB_SECTOR_18, OB_WRP_DB_SECTOR_19, OB_WRP_DB_SECTOR_20, OB_WRP_DB_SECTOR_21, OB_WRP_DB_SECTOR_22, OB_WRP_DB_SECTOR_23
};

void HAL_FLASHEx_OBGetConfig_customFake(FLASH_OBProgramInitTypeDef *pOBInit)
{
    memcpy(pOBInit, &testOptionByte, sizeof(FLASH_OBProgramInitTypeDef));
}

HAL_StatusTypeDef HAL_FLASHEx_OBProgram_customFake(FLASH_OBProgramInitTypeDef *pOBInit)
{
    memcpy(&testOptionByte, pOBInit, sizeof(FLASH_OBProgramInitTypeDef));

    return HAL_OK;
}

class STM32_FLASH_INTERN_TEST: public testing::Test
{
protected:


    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        RESET_FAKE (HAL_FLASH_Unlock);
        RESET_FAKE (HAL_FLASHEx_Erase);
        RESET_FAKE (HAL_FLASH_GetError);
        RESET_FAKE (HAL_FLASH_Program);
        RESET_FAKE (HAL_FLASH_Lock);
        RESET_FAKE (HAL_FLASHEx_OBGetConfig);
        RESET_FAKE (HAL_FLASH_OB_Unlock);
        RESET_FAKE (HAL_FLASHEx_OBProgram);
        RESET_FAKE (HAL_FLASH_OB_Lock);

        memset(&testOptionByte, 0, sizeof(FLASH_OBProgramInitTypeDef));
        testOptionByte.USERConfig = (testOptionByte.USERConfig & (~FLASH_INTERN_OB_NB_BANK_MASK)) | OB_NDBANK_SINGLE_BANK;
        HAL_FLASHEx_OBGetConfig_fake.custom_fake = HAL_FLASHEx_OBGetConfig_customFake;
    }

    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

/* specify test cases ******************************************************* */

/**
 * The following test cases verify the implementation of the internal flash memory interface
 * driver for the STM32F7 MCU variant.
 */

/* ***************************************************************************************** */
/* MCU_FlashIntern_Initialize() 																	     */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Initialize_success)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Initialize_success: }
     * Successful MCU_FlashIntern_Initialize() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    MCU_FlashIntern_T flashInternInitStruct;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Initialize(flashInternInitStruct);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
}

/* ***************************************************************************************** */
/* MCU_FlashIntern_Erase()                                                                   */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Erase_success)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Erase_success: }
     * Successful MCU_FlashIntern_Erase() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASHEx_Erase_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_OK;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Erase(FLASH_BASE, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASH_Unlock);
    RESET_FAKE (HAL_FLASHEx_Erase);
    RESET_FAKE (HAL_FLASH_Lock);
    RESET_FAKE (HAL_FLASHEx_OBGetConfig);
    RESET_FAKE (HAL_FLASH_OB_Unlock);
    RESET_FAKE (HAL_FLASHEx_OBProgram);
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Erase_errorPropagationTest)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Erase_errorPropagationTest: }
     * Error propagation test when the APIs called from within MCU_FlashIntern_Erase return an error
     */

    /* The API getEraseParameters() returns an error, memory not aligned with a sector address. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Erase(FLASH_BASE + 1, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(UINT32_C(0), HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(UINT32_C(0), HAL_FLASH_GetError_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));

    /* The API HAL_FLASH_Unlock() returns an error. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_Unlock_fake.return_val = HAL_ERROR;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = MCU_FlashIntern_Erase(FLASH_BASE, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(UINT32_C(0), HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(UINT32_C(0), HAL_FLASH_GetError_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED, Retcode_GetCode(retcode));

    /* The API HAL_FLASHEx_Erase() returns an error. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASHEx_Erase_fake.return_val = HAL_ERROR;
    HAL_FLASH_GetError_fake.return_val = FLASH_ERROR_ERS;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = MCU_FlashIntern_Erase(FLASH_BASE, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(UINT32_C(1), HAL_FLASHEx_Erase_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_GetError_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ERASE_ERROR, Retcode_GetCode(retcode));

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASH_Unlock);
    RESET_FAKE (HAL_FLASHEx_Erase);
    RESET_FAKE (HAL_FLASH_GetError);
    RESET_FAKE (HAL_FLASH_Lock);
}

/* ***************************************************************************************** */
/* MCU_FlashIntern_Write() 																	 */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Write_success)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Write_success: }
     * Successful MCU_FlashIntern_Write() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t writeBuffer[20] = {0};
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Program_fake.return_val = HAL_OK;
    HAL_FLASH_Lock_fake.return_val = HAL_OK;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Write(FLASH_BASE, writeBuffer, sizeof(writeBuffer));

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASH_Unlock);
    RESET_FAKE (HAL_FLASH_Program);
    RESET_FAKE (HAL_FLASH_Lock)
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Write_failure_nullPointer)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Write_failure_nullPointer: }
     * MCU_FlashIntern_Write() failing test case.
     */

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Write(FLASH_BASE, NULL, 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Write_failure_invalidParameter)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Write_failure_nullPointer: }
     * MCU_FlashIntern_Write() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t byte = UINT8_C(0);

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Write(FLASH_BASE, &byte, 0);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Write_failure_memoryAddressOutOfBound)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Write_failure_memoryAddressOutOfBound: }
     * MCU_FlashIntern_Write() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t byte = UINT8_C(0);

    /* The provided start address is out of bound (lower limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Write(FLASH_BASE - 1, &byte, 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided start address + length is out of bound (upper limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = MCU_FlashIntern_Write(FLASH_BASE, &byte, FLASH_END - FLASH_BASE + 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Write_errorPropagationTest)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Write_errorPropagationTest: }
     * Error propagation test when the APIs called from within MCU_FlashIntern_Write return an error
     */

    /* The API HAL_FLASH_Unlock() returns an error. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t byte = UINT8_C(0);
    HAL_FLASH_Unlock_fake.return_val = HAL_ERROR;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Write(FLASH_BASE, &byte, 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(UINT32_C(0), HAL_FLASH_Program_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_Lock_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED, Retcode_GetCode(retcode));

    /* The API HAL_FLASH_Program() returns an error. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    RESET_FAKE (HAL_FLASH_Unlock);
    RESET_FAKE (HAL_FLASH_Lock);
    HAL_FLASH_Unlock_fake.return_val = HAL_OK;
    HAL_FLASH_Program_fake.return_val = HAL_ERROR;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = MCU_FlashIntern_Write(FLASH_BASE, &byte, 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_Unlock_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_Program_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_Lock_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_PROG_ERROR, Retcode_GetCode(retcode));

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASH_Unlock);
    RESET_FAKE (HAL_FLASH_Program);
    RESET_FAKE (HAL_FLASH_Lock)
}

/* ***************************************************************************************** */
/* MCU_FlashIntern_Read() 																	 */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Read_success)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Read_success: }
     * Successful MCU_FlashIntern_Read() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t readBuffer[6] = {0};

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Read(FLASH_BASE, readBuffer, 6);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Read_failure_nullPointer)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Read_failure_nullPointer: }
     * MCU_FlashIntern_Read() failing test case.
     */

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Read(FLASH_BASE, NULL, 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Read_failure_invalidParameter)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Read_failure_invalidParameter: }
     * MCU_FlashIntern_Read() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t byte = UINT8_C(0);

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Read(FLASH_BASE, &byte, 0);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Read_failure_memoryAddressOutOfBound)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Read_failure_memoryAddressOutOfBound: }
     * MCU_FlashIntern_Read() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t byte = UINT8_C(0);

    /* The provided start address is out of bound (lower limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Read(FLASH_BASE - 1, &byte, 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided start address + length is out of bound (upper limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = MCU_FlashIntern_Read(FLASH_BASE, &byte, FLASH_END - FLASH_BASE + 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_OTP_Read_failure_invalidParameter)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_OTP_Read_failure_invalidParameter: }
     * Testcase to check MCU_FlashIntern_Read() API for the failure case where the parameter 
	 * passed is invalid.
     */
	 
	/* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t byte = UINT8_C(0);
	
	/* The provided length results in start address greater than end address. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Read(FLASH_OTP_BASE, &byte, 0xFFFFFFFF - FLASH_OTP_BASE + 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_OTP_Read_success)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_OTP_Read_success: }
     * Successful MCU_FlashIntern_Read() test case for reading OTP area.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint8_t readBuffer[6] = {0};

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_Read(FLASH_OTP_BASE, readBuffer, 6);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
}

/* ***************************************************************************************** */
/* MCU_FlashIntern_WriteProtect() 														     */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_WriteProtect_success_enable)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_WriteProtect_success_enable: }
     * Successful MCU_FlashIntern_WriteProtect() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_OB_Unlock_fake.return_val = HAL_OK;
    HAL_FLASHEx_OBProgram_fake.custom_fake = HAL_FLASHEx_OBProgram_customFake;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_WriteProtect(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1 - 1, true);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(2), HAL_FLASHEx_OBGetConfig_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Unlock_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASHEx_OBProgram_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);
    EXPECT_TRUE(testOptionByte.WRPState);
    EXPECT_EQ(OB_WRP_SECTOR_0, testOptionByte.WRPSector);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASHEx_OBGetConfig);
    RESET_FAKE (HAL_FLASH_OB_Unlock);
    RESET_FAKE (HAL_FLASHEx_OBProgram);
    RESET_FAKE (HAL_FLASH_OB_Lock);
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_WriteProtect_success_disable)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_WriteProtect_success_disable: }
     * Successful MCU_FlashIntern_WriteProtect() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_OB_Unlock_fake.return_val = HAL_OK;
    HAL_FLASHEx_OBProgram_fake.custom_fake = HAL_FLASHEx_OBProgram_customFake;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_WriteProtect(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1 - 1, false);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(2), HAL_FLASHEx_OBGetConfig_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Unlock_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASHEx_OBProgram_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);
    EXPECT_FALSE(testOptionByte.WRPState);
    EXPECT_EQ(OB_WRP_SECTOR_0, testOptionByte.WRPSector);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASHEx_OBGetConfig);
    RESET_FAKE (HAL_FLASH_OB_Unlock);
    RESET_FAKE (HAL_FLASHEx_OBProgram);
    RESET_FAKE (HAL_FLASH_OB_Lock);
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_WriteProtect_success_optionByteCalculation_SB)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_WriteProtect_success_optionByteCalculation_SB: }
     * Successful MCU_FlashIntern_WriteProtect() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_OB_Unlock_fake.return_val = HAL_OK;
    HAL_FLASHEx_OBProgram_fake.custom_fake = HAL_FLASHEx_OBProgram_customFake;

    for(uint8_t sectorIndex; sectorIndex < NB_SB_SECTOR; sectorIndex++)
    {
        /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
        Retcode_T retcode = MCU_FlashIntern_WriteProtect(sectorStartAddressSB[sectorIndex], sectorEndAddressSB[sectorIndex], false);

        /* VERIFY : Compare the expected value with the actual one. */
        EXPECT_EQ(RETCODE_OK, retcode);
        EXPECT_FALSE(testOptionByte.WRPState);
        EXPECT_EQ(expectedOptionBytesSB[sectorIndex], testOptionByte.WRPSector);
    }
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_WriteProtect_success_optionByteCalculation_DB)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_WriteProtect_success_optionByteCalculation_DB: }
     * Successful MCU_FlashIntern_WriteProtect() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_OB_Unlock_fake.return_val = HAL_OK;
    HAL_FLASHEx_OBProgram_fake.custom_fake = HAL_FLASHEx_OBProgram_customFake;
    /* Overwrite number of bank configuration for single bank (default) to dual bank.*/
    testOptionByte.USERConfig = (testOptionByte.USERConfig & (~FLASH_INTERN_OB_NB_BANK_MASK)) | OB_NDBANK_DUAL_BANK;

    for(uint8_t sectorIndex; sectorIndex < NB_DB_SECTOR; sectorIndex++)
    {
        /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
        Retcode_T retcode = MCU_FlashIntern_WriteProtect(sectorStartAddressDB[sectorIndex], sectorEndAddressDB[sectorIndex], false);

        /* VERIFY : Compare the expected value with the actual one. */
        EXPECT_EQ(RETCODE_OK, retcode);
        EXPECT_FALSE(testOptionByte.WRPState);
        EXPECT_EQ(expectedOptionBytesDB[sectorIndex], testOptionByte.WRPSector);
    }
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_WriteProtect_failure_memoryAddressOutOfBound)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_WriteProtect_failure_memoryAddressOutOfBound: }
     * MCU_FlashIntern_WriteProtect() failing test case.
     */

    /* Start address is out of the memory's boundaries. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    RESET_FAKE (HAL_FLASH_OB_Lock);

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_WriteProtect(FLASH_BASE - 1, FLASH_END, true);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);

    /* End address is out of the memory's boundaries. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    RESET_FAKE (HAL_FLASH_OB_Lock);

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
     retcode = MCU_FlashIntern_WriteProtect(FLASH_BASE, FLASH_END + 1, true);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);

    /* SETUP: Declare and initialize the test condition required by this test case. */
    RESET_FAKE (HAL_FLASH_OB_Lock);

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
     retcode = MCU_FlashIntern_WriteProtect(FLASH_BASE, FLASH_BASE - 1, true);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASH_OB_Lock);
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_WriteProtect_failure_unalignedAddresses)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_WriteProtect_failure_unalignedAddresses: }
     * MCU_FlashIntern_WriteProtect() failing test case.
     */

    /* Start address is not aligned to a sector's start address. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    RESET_FAKE (HAL_FLASH_OB_Lock);

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_WriteProtect(ADDR_FLASH_SB_SECTOR_0 + 1, FLASH_END, true);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);

    /* End address is not aligned to a sector's end address. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    RESET_FAKE (HAL_FLASH_OB_Lock);

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = MCU_FlashIntern_WriteProtect(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1, true);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASH_OB_Lock);
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_WriteProtect_errorPropagationTest)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_WriteProtect_errorPropagationTest: }
     * Error propagation test when the APIs called from within MCU_FlashIntern_WriteProtect return an error
     */

    /* HAL_FLASH_OB_Unlock() returns an error. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_OB_Unlock_fake.return_val = HAL_ERROR;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = MCU_FlashIntern_WriteProtect(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1 - 1, true);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_FAILED_TO_CONFIGURE_WRITE_PROTECTION, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(2), HAL_FLASHEx_OBGetConfig_fake.call_count);
    EXPECT_EQ(UINT32_C(0), HAL_FLASHEx_OBProgram_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);

    /* HAL_FLASHEx_OBProgram() returns an error. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_OB_Unlock_fake.return_val = HAL_OK;
    HAL_FLASHEx_OBProgram_fake.return_val = HAL_ERROR;
    RESET_FAKE (HAL_FLASH_OB_Lock);
    RESET_FAKE (HAL_FLASHEx_OBGetConfig);

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = MCU_FlashIntern_WriteProtect(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1 - 1, true);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_FAILED_TO_CONFIGURE_WRITE_PROTECTION, Retcode_GetCode(retcode));
    EXPECT_EQ(UINT32_C(2), HAL_FLASHEx_OBGetConfig_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASHEx_OBProgram_fake.call_count);
    EXPECT_EQ(UINT32_C(1), HAL_FLASH_OB_Lock_fake.call_count);

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASH_OB_Unlock);
    RESET_FAKE (HAL_FLASHEx_OBProgram);
}

/* ***************************************************************************************** */
/* checkAddressBounderies() 																 */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, checkAddressBounderies_success)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::checkAddressBounderies_success: }
     * Successful checkAddressBounderies() test case.
     */

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = checkAddressBounderies(FLASH_BASE, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(STM32_FLASH_INTERN_TEST, checkAddressBounderies_failure_memoryAddressOutOfBound)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::checkAddressBounderies_failure_memoryAddressOutOfBound: }
     * checkAddressBounderies() failing test case.
     */

    /* The provided start address is out of bound (lower limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = checkAddressBounderies(FLASH_BASE - 1, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided start address matches the provided end address error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = checkAddressBounderies(FLASH_BASE, FLASH_BASE);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided start address is bigger than the end address error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = checkAddressBounderies(FLASH_BASE + 1, FLASH_BASE);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided end address is out of bound (lower limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = checkAddressBounderies(FLASH_BASE, FLASH_BASE - 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided end address is out of bound (upper limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = checkAddressBounderies(FLASH_BASE, FLASH_END + 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));
}

/* ***************************************************************************************** */
/* getCorrespondingSectors() 															     */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, getCorrespondingSectors_success_DB)
{
    /** @testcase{ TEST_FIXTURE::getCorrespondingSectors_success_DB: }
     * Successful getCorrespondingSectors() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    /* Overwrite number of bank configuration for single bank (default) to dual bank.*/
    testOptionByte.USERConfig = (testOptionByte.USERConfig & (~FLASH_INTERN_OB_NB_BANK_MASK)) | OB_NDBANK_DUAL_BANK;

    for(uint8_t sectorIndex; sectorIndex < NB_DB_SECTOR; sectorIndex++)
    {
        /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
        Retcode_T retcode = getCorrespondingSectors(sectorStartAddressDB[sectorIndex], sectorEndAddressDB[sectorIndex], &startSector, &endSector);

        /* VERIFY : Compare the expected value with the actual one. */
        EXPECT_EQ(sectorIndex, startSector);
        EXPECT_EQ(endSector, startSector + 1);
    }

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    /* Revert number of bank configuration to single bank (default).*/
    testOptionByte.USERConfig = (testOptionByte.USERConfig & (~FLASH_INTERN_OB_NB_BANK_MASK)) | OB_NDBANK_SINGLE_BANK;
}

TEST_F(STM32_FLASH_INTERN_TEST, getCorrespondingSectors_failure_nullPointer)
{
    /** @testcase{ TEST_FIXTURE::getCorrespondingSectors_failure_nullPointer: }
     * getCorrespondingSectors() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getCorrespondingSectors(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1 - 1, NULL, &endSector);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getCorrespondingSectors(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1 - 1, &startSector, NULL);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, getCorrespondingSectors_failure_invalideArgument_SB)
{
    /** @testcase{ TEST_FIXTURE::getCorrespondingSectors_failure_invalideArgument_SB: }
     * getCorrespondingSectors() failing test case.
     */

    /* Start sector is equal to the end sector. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getCorrespondingSectors(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_0 - 1, &startSector, &endSector);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, getCorrespondingSectors_failure_invalideArgument_DB)
{
    /** @testcase{ TEST_FIXTURE::getCorrespondingSectors_failure_invalideArgument_DB: }
     * getCorrespondingSectors() failing test case.
     */

    /* Start sector is equal to the end sector. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    /* Overwrite number of bank configuration for single bank (default) to dual bank.*/
    testOptionByte.USERConfig = (testOptionByte.USERConfig & (~FLASH_INTERN_OB_NB_BANK_MASK)) | OB_NDBANK_DUAL_BANK;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getCorrespondingSectors(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_0 - 1, &startSector, &endSector);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    /* Revert number of bank configuration to single bank (default).*/
    testOptionByte.USERConfig = (testOptionByte.USERConfig & (~FLASH_INTERN_OB_NB_BANK_MASK)) | OB_NDBANK_SINGLE_BANK;
}

TEST_F(STM32_FLASH_INTERN_TEST, getCorrespondingSectors_failure_unalignedAddresses_SB)
{
    /** @testcase{ TEST_FIXTURE::getCorrespondingSectors_failure_unalignedAddresses_SB: }
     * getCorrespondingSectors() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;

    /* Start address is not aligned to a sector's start address. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getCorrespondingSectors(ADDR_FLASH_SB_SECTOR_0 + 1, ADDR_FLASH_SB_SECTOR_1 - 1, &startSector, &endSector);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));

    /* End address is not aligned to a sector's end address. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getCorrespondingSectors(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1, &startSector, &endSector);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, getCorrespondingSectors_failure_unalignedAddresses_DB)
{
    /** @testcase{ TEST_FIXTURE::getCorrespondingSectors_failure_unalignedAddresses_DB: }
     * getCorrespondingSectors() failing test case.
     */

    /* Start address is not aligned to a sector's start address. */
    /* SETUP: Declare and initialize the test condition required by this test case. */
    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    /* Overwrite number of bank configuration for single bank (default) to dual bank.*/
    testOptionByte.USERConfig = (testOptionByte.USERConfig & (~FLASH_INTERN_OB_NB_BANK_MASK)) | OB_NDBANK_DUAL_BANK;

    /* Start address is not aligned to a sector's start address. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getCorrespondingSectors(ADDR_FLASH_SB_SECTOR_0 + 1, ADDR_FLASH_SB_SECTOR_1 - 1, &startSector, &endSector);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));

    /* End address is not aligned to a sector's end address. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getCorrespondingSectors(ADDR_FLASH_SB_SECTOR_0, ADDR_FLASH_SB_SECTOR_1, &startSector, &endSector);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    /* Revert number of bank configuration to single bank (default).*/
    testOptionByte.USERConfig = (testOptionByte.USERConfig & (~FLASH_INTERN_OB_NB_BANK_MASK)) | OB_NDBANK_SINGLE_BANK;
}

/* ***************************************************************************************** */
/* getEraseParameters()                                                                      */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, getEraseParameters_success_massErase)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::getEraseParameters_success_massErase: }
     * Successful getEraseParameters() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    FLASH_EraseInitTypeDef eraseInitStruct;

    eraseInitStruct.TypeErase = 0xFFFFFFFF;
    eraseInitStruct.Sector = 0xFFFFFFFF;
    eraseInitStruct.NbSectors = 0xFFFFFFFF;
    eraseInitStruct.VoltageRange = 0xFFFFFFFF;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(FLASH_TYPEERASE_MASSERASE, eraseInitStruct.TypeErase);
    EXPECT_EQ(0, eraseInitStruct.Sector);
    EXPECT_EQ(NB_SB_SECTOR, eraseInitStruct.NbSectors);
    EXPECT_EQ(FLASH_VOLTAGE_RANGE_3, eraseInitStruct.VoltageRange);
}

TEST_F(STM32_FLASH_INTERN_TEST, getEraseParameters_success_sectorErase)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::getEraseParameters_success_sectorErase: }
     * Successful getEraseParameters() test case.
     */

    FLASH_EraseInitTypeDef eraseInitStruct;

    for(uint8_t sectorIndex; sectorIndex < NB_SB_SECTOR; sectorIndex++)
    {
        /* SETUP: Declare and initialize the test condition required by this test case. */
        eraseInitStruct.TypeErase = 0xFFFFFFFF;
        eraseInitStruct.Sector = 0xFFFFFFFF;
        eraseInitStruct.NbSectors = 0xFFFFFFFF;
        eraseInitStruct.VoltageRange = 0xFFFFFFFF;

        /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
        Retcode_T retcode = getEraseParameters(&eraseInitStruct, sectorStartAddressSB[sectorIndex], sectorEndAddressSB[sectorIndex]);

        /* VERIFY : Compare the expected value with the actual one. */
        EXPECT_EQ(RETCODE_OK, retcode);
        EXPECT_EQ(FLASH_TYPEERASE_SECTORS, eraseInitStruct.TypeErase);
        EXPECT_EQ(sectorIndex, eraseInitStruct.Sector);
        EXPECT_EQ(1, eraseInitStruct.NbSectors);
        EXPECT_EQ(FLASH_VOLTAGE_RANGE_3, eraseInitStruct.VoltageRange);
    }
}

TEST_F(STM32_FLASH_INTERN_TEST, MCU_FlashIntern_Erase_failure_memoryAddressOutOfBound)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::MCU_FlashIntern_Erase_failure_memoryAddressOutOfBound: }
     * MCU_FlashIntern_Erase() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    FLASH_EraseInitTypeDef eraseInitStruct;

    /* The provided start address is out of bound (lower limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE - 1, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided start address matches the provided end address error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE, FLASH_BASE);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided start address is bigger than the end address error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE + 1, FLASH_BASE);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided end address is out of bound (lower limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE, FLASH_BASE - 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));

    /* The provided end address is out of bound (upper limit) error. */
    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE, FLASH_END + 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND, Retcode_GetCode(retcode));
}

TEST_F(STM32_FLASH_INTERN_TEST, getEraseParameters_failure_unalignedAddresses)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::getEraseParameters_failure_unalignedAddresses: }
     * getEraseParameters() failing test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    FLASH_EraseInitTypeDef eraseInitStruct;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE + 1, FLASH_END);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE, FLASH_END - 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getEraseParameters(&eraseInitStruct, FLASH_BASE, FLASH_END - 1);

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR, Retcode_GetCode(retcode));
}

/* ***************************************************************************************** */
/* getFlashError()                                                                           */
/* ***************************************************************************************** */

TEST_F(STM32_FLASH_INTERN_TEST, getFlashError_success)
{
    /** @testcase{ STM32_FLASH_INTERN_TEST::getFlashError_success: }
     * Successful getFlashError() test case.
     */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_GetError_fake.return_val = FLASH_ERROR_NONE;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    Retcode_T retcode = getFlashError();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_OK, retcode);

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_GetError_fake.return_val = FLASH_ERROR_ERS;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getFlashError();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_ERASE_ERROR, Retcode_GetCode(retcode));

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_GetError_fake.return_val = HAL_FLASH_ERROR_PGP;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getFlashError();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_PROG_PARALLELISM_ERROR, Retcode_GetCode(retcode));

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_GetError_fake.return_val = HAL_FLASH_ERROR_PGA;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getFlashError();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_PROG_ALIGMENT_ERROR, Retcode_GetCode(retcode));

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_GetError_fake.return_val = HAL_FLASH_ERROR_WRP;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getFlashError();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_WRITE_PROTECTED_ERROR, Retcode_GetCode(retcode));

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_GetError_fake.return_val = HAL_FLASH_ERROR_OPERATION;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getFlashError();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_FLASH_OPERATION_ERROR, Retcode_GetCode(retcode));

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_GetError_fake.return_val = HAL_FLASH_ERROR_RD;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getFlashError();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_MCU_FLASH_INTERN_READ_PROTECTED_ERROR, Retcode_GetCode(retcode));

    /* SETUP: Declare and initialize the test condition required by this test case. */
    HAL_FLASH_GetError_fake.return_val = HAL_FLASH_ERROR_RD + 1;

    /* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
    retcode = getFlashError();

    /* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INCONSITENT_STATE, Retcode_GetCode(retcode));

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */
    RESET_FAKE (HAL_FLASH_GetError);
}
