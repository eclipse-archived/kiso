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
#include "gtest.h"

extern "C"
{
#include "Kiso_Basics.h"
#include "Kiso_HAL_th.hh"

#if KISO_FEATURE_MCU_CRC
/* include faked interfaces */
#include "Kiso_Retcode_th.hh"
#include "stm32l4xx_th.hh"
#include "stm32l4xx_hal_th.hh"
#include "stm32l4xx_hal_crc_th.hh"

// implements all the necessary fake functions
#undef __HAL_RCC_CRC_CLK_ENABLE
    FAKE_VOID_FUNC(__HAL_RCC_CRC_CLK_ENABLE)

#undef __HAL_RCC_CRC_CLK_DISABLE
    FAKE_VOID_FUNC(__HAL_RCC_CRC_CLK_DISABLE)

/* Include unit under test */
#include "CRC.c"

} /* extern "C"*/

class KISO_CRCtest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
        /* Initialize execution environment */
        RESET_FAKE(HAL_CRC_Init);
        RESET_FAKE(HAL_CRC_DeInit);
        RESET_FAKE(HAL_CRC_Calculate);
    }

    virtual void TearDown()
    {
        ; /* Do nothing */
    }
};

/*************************specify test cases******************************* */

TEST_F(KISO_CRCtest, Crc8InvalidParmeters)
{
    /** @testcase{ KISO_CRCtest::Crc8InvalidParmeters: }
	 * MCU_CRC8() with Inavalid parameters test cases.
	 */

    Retcode_T retcode = RETCODE_OK;
    struct MCU_CRC8_S *initParm = NULL;
    uint8_t testData = UINT8_C(1);
    uint8_t testCrc = UINT8_C(1);
    uint32_t len = UINT32_C(1);

    retcode = MCU_CRC8(initParm, &testData, len, &testCrc);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to input data Buffer */

    struct MCU_CRC8_S initParms;
    initParms.GeneratePolynomial = UINT8_C(1);
    initParms.InitVal = UINT8_C(1);
    uint8_t *testDataPtr = NULL;

    retcode = MCU_CRC8(&initParms, testDataPtr, len, &testCrc);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to output data Buffer */

    uint8_t *testCrcPtr = NULL;
    retcode = MCU_CRC8(&initParms, &testData, len, testCrcPtr);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));
}

TEST_F(KISO_CRCtest, Crc8Success)
{
    /** @testcase{ KISO_CRCtest::Crc8Success: }
	 * MCU_CRC8() with CRC calculate success test case.
	 */

    Retcode_T retcode;
    struct MCU_CRC8_S initParms;
    initParms.GeneratePolynomial = UINT8_C(1);
    initParms.InitVal = UINT8_C(1);
    uint8_t testData = UINT8_C(1);
    uint8_t testCrc = UINT8_C(1);
    uint8_t CrcVal = UINT8_C(10);
    uint32_t len = UINT32_C(1);
    HAL_CRC_Init_fake.return_val = HAL_OK;
    HAL_CRC_DeInit_fake.return_val = HAL_OK;
    HAL_CRC_Calculate_fake.return_val = CrcVal;

    retcode = MCU_CRC8(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(testCrc, CrcVal);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(KISO_CRCtest, Crc8RetcodeFailure)
{
    /** @testcase{ KISO_CRCtest::Crc8RetcodeFailure: }
	 * MCU_CRC8() with return code failure test case.
	 */

    /* testcase for HAL_CRC_Init failure */

    Retcode_T retcode;
    struct MCU_CRC8_S initParms;
    initParms.GeneratePolynomial = UINT8_C(0);
    initParms.InitVal = UINT8_C(0);
    uint8_t testData = UINT8_C(1);
    uint8_t testCrc = UINT8_C(1);
    uint8_t CrcVal = UINT8_C(10);
    uint32_t len = UINT32_C(1);
    HAL_CRC_Init_fake.return_val = HAL_ERROR;
    HAL_CRC_DeInit_fake.return_val = HAL_OK;
    HAL_CRC_Calculate_fake.return_val = CrcVal;

    retcode = MCU_CRC8(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    /* testcase for HAL_CRC_DeInit failure */

    initParms.GeneratePolynomial = UINT8_C(0);
    initParms.InitVal = UINT8_C(0);
    HAL_CRC_Init_fake.return_val = HAL_OK;
    HAL_CRC_DeInit_fake.return_val = HAL_ERROR;

    retcode = MCU_CRC8(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(2U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(KISO_CRCtest, Crc16InvalidParmeters)
{
    /** @testcase{ KISO_CRCtest::Crc16InvalidParmeters: }
	 * MCU_CRC16() with Invalid parameters test cases.
	 */

    /*Failure due to NULL parameters */

    Retcode_T retcode = RETCODE_OK;
    struct MCU_CRC16_S *initParm = NULL;
    uint16_t testData = UINT16_C(1);
    uint16_t testCrc = UINT16_C(1);
    uint32_t len = UINT32_C(1);

    retcode = MCU_CRC16(initParm, &testData, len, &testCrc);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to input data Buffer */

    struct MCU_CRC16_S initParms;
    initParms.GeneratePolynomial = UINT16_C(1);
    initParms.InitVal = UINT16_C(1);
    uint16_t *testDataPtr = NULL;

    retcode = MCU_CRC16(&initParms, testDataPtr, len, &testCrc);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to output data Buffer */

    uint16_t *testCrcPtr = NULL;

    retcode = MCU_CRC16(&initParms, &testData, len, testCrcPtr);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));
}

TEST_F(KISO_CRCtest, Crc16Success)
{
    /** @testcase{ KISO_CRCtest::Crc16Success: }
	 * MCU_CRC16() CRC 16-Bit calculate success test case.
	 */

    Retcode_T retcode;
    struct MCU_CRC16_S initParms;
    initParms.GeneratePolynomial = UINT16_C(1);
    initParms.InitVal = UINT16_C(1);
    uint16_t testData = UINT16_C(1);
    uint16_t testCrc = UINT16_C(1);
    uint16_t CrcVal = UINT16_C(28);
    uint32_t len = UINT32_C(1);
    HAL_CRC_Init_fake.return_val = HAL_OK;
    HAL_CRC_DeInit_fake.return_val = HAL_OK;
    HAL_CRC_Calculate_fake.return_val = CrcVal;

    retcode = MCU_CRC16(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(testCrc, CrcVal);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(KISO_CRCtest, Crc16RetcodeFailure)
{
    /** @testcase{ KISO_CRCtest::Crc16retcodefail: }
	 * MCU_CRC16() return code failure test case.
	 */

    /* testcase for HAL_CRC_Init failure */

    Retcode_T retcode;
    struct MCU_CRC16_S initParms;
    initParms.GeneratePolynomial = UINT16_C(1);
    initParms.InitVal = UINT16_C(1);
    uint16_t testData = UINT16_C(1);
    uint16_t testCrc = UINT16_C(1);
    uint16_t CrcVal = UINT16_C(28);
    uint32_t len = UINT32_C(1);
    HAL_CRC_Init_fake.return_val = HAL_ERROR;
    HAL_CRC_DeInit_fake.return_val = HAL_OK;
    HAL_CRC_Calculate_fake.return_val = CrcVal;

    retcode = MCU_CRC16(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    /* testcase for HAL_CRC_DeInit failure */

    initParms.GeneratePolynomial = UINT16_C(0);
    initParms.InitVal = UINT16_C(0);
    HAL_CRC_Init_fake.return_val = HAL_OK;
    HAL_CRC_DeInit_fake.return_val = HAL_ERROR;

    retcode = MCU_CRC16(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(2U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(KISO_CRCtest, Crc32InvalidParmeters)
{
    /** @testcase{ KISO_CRCtest::Crc32InvalidParmeters: }
	 * MCU_CRC32() invalid parameters(NULL) test cases.
	 */

    /*Failure due to NULL parameters */

    Retcode_T retcode;
    struct MCU_CRC32_S *initParm = NULL;
    uint32_t testData = UINT32_C(1);
    uint32_t testCrc = UINT32_C(1);
    uint32_t len = UINT32_C(1);

    retcode = MCU_CRC32(initParm, &testData, len, &testCrc);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to input data Buffer */

    struct MCU_CRC32_S initParms;
    initParms.GeneratePolynomial = UINT32_C(1);
    initParms.InitVal = UINT32_C(1);
    initParms.DataFormat = MCU_CRC_DATA_8BIT;
    uint32_t *testDataPtr = NULL;

    retcode = MCU_CRC32(&initParms, testDataPtr, len, &testCrc);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to output data Buffer */

    uint32_t *testCrcPtr = NULL;

    retcode = MCU_CRC32(&initParms, &testData, len, testCrcPtr);

    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    testData = UINT32_C(1);
    testCrc = UINT32_C(1);
    len = UINT32_C(1);
    initParms.GeneratePolynomial = UINT32_C(0);
    initParms.InitVal = UINT32_C(0);
    initParms.DataFormat = (MCU_CRC_DataType_E)5;
    HAL_CRC_Init_fake.return_val = HAL_OK;

    retcode = MCU_CRC32(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(KISO_CRCtest, Crc32Success)
{
    /** @testcase{ KISO_CRCtest::Crc32Success: }
	 * MCU_CRC32() CRC calculate success test case.
	 */

    Retcode_T retcode;
    struct MCU_CRC32_S initParms;
    initParms.GeneratePolynomial = MCU_CRC32_POLY_ETHERNET;
    initParms.InitVal = MCU_CRC32_INIT_VALUE_DEFAULT;
    initParms.DataFormat = MCU_CRC_DATA_8BIT;
    uint32_t testData = UINT32_C(1);
    uint32_t testCrc = UINT32_C(1);
    uint32_t CrcVal = UINT32_C(28);
    uint32_t len = UINT32_C(1);
    HAL_CRC_Init_fake.return_val = HAL_OK;
    HAL_CRC_DeInit_fake.return_val = HAL_OK;
    HAL_CRC_Calculate_fake.return_val = CrcVal;

    retcode = MCU_CRC32(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(testCrc, CrcVal);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(KISO_CRCtest, Crc32RetcodeFailure)
{
    /** @testcase{ KISO_CRCtest::Crc32RetcodeFailure: }
	 * MCU_CRC32() retcode failure test case.
	 */

    /* SETUP: Declare and initialize the test condition required by this test case. */
    Retcode_T retcode;
    struct MCU_CRC32_S initParms;
    initParms.GeneratePolynomial = MCU_CRC32_POLY_ETHERNET;
    initParms.InitVal = MCU_CRC32_INIT_VALUE_DEFAULT;
    initParms.DataFormat = MCU_CRC_DATA_32BIT;
    uint32_t testData = UINT32_C(1);
    uint32_t testCrc = UINT32_C(1);
    uint32_t CrcVal = UINT32_C(28);
    uint32_t len = UINT32_C(1);
    HAL_CRC_Init_fake.return_val = HAL_ERROR;
    HAL_CRC_DeInit_fake.return_val = HAL_OK;
    HAL_CRC_Calculate_fake.return_val = CrcVal;

    retcode = MCU_CRC32(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    initParms.GeneratePolynomial = UINT32_C(0);
    initParms.InitVal = UINT32_C(0);
    initParms.DataFormat = MCU_CRC_DATA_16BIT;
    HAL_CRC_Init_fake.return_val = HAL_OK;
    HAL_CRC_DeInit_fake.return_val = HAL_ERROR;

    retcode = MCU_CRC32(&initParms, &testData, len, &testCrc);

    EXPECT_EQ(2U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}
#else
}
#endif /* KISO_FEATURE_MCU_CRC */
