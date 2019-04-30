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

FFF_DEFINITION_BLOCK_START

/* include faked interface */
extern "C"
{
/* setup compile time configuration defines */
#undef BCDS_FEATURE_CRC
#define BCDS_FEATURE_CRC 1

/* include faked interfaces */
#include "BCDS_HAL_th.hh"

#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"

#include "stm32f7xx_th.hh"
#include "stm32f7xx_hal_th.hh"
#include "stm32f7xx_hal_crc_th.hh"
#include "stm32f7xx_hal_rcc_th.hh"

#undef BCDS_FEATURE_MCU_CRC
#define BCDS_FEATURE_MCU_CRC 1

/* Include unit under test */
#include "CRC.c"

} /* extern "C"*/

FFF_DEFINITION_BLOCK_END

class BCDS_CRCtest: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

    }

    virtual void TearDown()
    {
        RESET_FAKE(HAL_CRC_Init);
        RESET_FAKE(HAL_CRC_DeInit);
        RESET_FAKE(HAL_CRC_Calculate);
    }
};

/*************************specify test cases******************************* */

TEST_F(BCDS_CRCtest, Crc8InvalidParmeters)
{
	/** @testcase{ BCDS_CRCtest::Crc8InvalidParmeters: }
	 * MCU_CRC8() with Inavalid parameters test cases.
	 */

    /*Failure due to NULL parameters */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    Retcode_T retcode   			 = RETCODE_OK;
    struct MCU_CRC8_S *initParm     = NULL;
    uint8_t testData     			= UINT8_C(1);
    uint8_t testCrc      			= UINT8_C(1);
    uint32_t len         			= UINT32_C(1);

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC8(initParm,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));


    /*Failure due to NULL pointer assigned to input data Buffer */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    struct MCU_CRC8_S initParms;
	initParms.GeneratePolynomial   = UINT8_C(1);
	initParms.InitVal              = UINT8_C(1);
    uint8_t *testDataPtr           = NULL;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC8(&initParms,testDataPtr,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to output data Buffer */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    uint8_t *testCrcPtr = NULL;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC8(&initParms,&testData,len,testCrcPtr);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));
}

TEST_F(BCDS_CRCtest, Crc8Success)
{
	/** @testcase{ BCDS_CRCtest::Crc8Success: }
	 * MCU_CRC8() with CRC calculate success test case.
	 */

	/* SETUP: Declare and initialize the test condition required by this test case. */
	Retcode_T retcode;
	struct MCU_CRC8_S initParms;
	initParms.GeneratePolynomial      = UINT8_C(1);
	initParms.InitVal                 = UINT8_C(1);
    uint8_t testData                  = UINT8_C(1);
    uint8_t testCrc                   = UINT8_C(1);
    uint8_t CrcVal                    = UINT8_C(10);
    uint32_t len                      = UINT32_C(1);
	HAL_CRC_Init_fake.return_val      = HAL_OK;
	HAL_CRC_DeInit_fake.return_val    = HAL_OK;
	HAL_CRC_Calculate_fake.return_val = CrcVal;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC8(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
	EXPECT_EQ(testCrc, CrcVal);
	EXPECT_EQ(RETCODE_OK, retcode);
}


TEST_F(BCDS_CRCtest, Crc8RetcodeFailure)
{
	/** @testcase{ BCDS_CRCtest::Crc8RetcodeFailure: }
	 * MCU_CRC8() with return code failure test case.
	 */

	/* testcase for HAL_CRC_Init failure */

	/* SETUP: Declare and initialize the test condition required by this test case. */
	Retcode_T retcode;
	struct MCU_CRC8_S initParms;
	initParms.GeneratePolynomial      = UINT8_C(0);
	initParms.InitVal                 = UINT8_C(0);
    uint8_t testData                  = UINT8_C(1);
    uint8_t testCrc                   = UINT8_C(1);
    uint8_t CrcVal                    = UINT8_C(10);
    uint32_t len                      = UINT32_C(1);
	HAL_CRC_Init_fake.return_val      = HAL_ERROR;
	HAL_CRC_DeInit_fake.return_val    = HAL_OK;
	HAL_CRC_Calculate_fake.return_val = CrcVal;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC8(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_CRC_HAL_INIT_FAIL, Retcode_GetCode(retcode));

	/* testcase for HAL_CRC_DeInit failure */

	/* SETUP: Declare and initialize the test condition required by this test case. */
	initParms.GeneratePolynomial       = UINT8_C(0);
	initParms.InitVal                  = UINT8_C(0);
	HAL_CRC_Init_fake.return_val       = HAL_OK;
	HAL_CRC_DeInit_fake.return_val     = HAL_ERROR;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC8(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(2U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_CRC_HAL_DEINIT_FAIL, Retcode_GetCode(retcode));

}

TEST_F(BCDS_CRCtest, Crc16InvalidParmeters )
{
	/** @testcase{ BCDS_CRCtest::Crc16InvalidParmeters: }
	 * MCU_CRC16() with Invalid parameters test cases.
	 */

    /*Failure due to NULL parameters */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    Retcode_T retcode      				= RETCODE_OK;
    struct MCU_CRC16_S *initParm      	= NULL;
    uint16_t testData      				= UINT16_C(1);
    uint16_t testCrc      				= UINT16_C(1);
    uint32_t len           				= UINT32_C(1);

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC16(initParm,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));


    /*Failure due to NULL pointer assigned to input data Buffer */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    struct MCU_CRC16_S initParms;
	initParms.GeneratePolynomial   = UINT16_C(1);
	initParms.InitVal              = UINT16_C(1);
    uint16_t *testDataPtr          = NULL;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC16(&initParms,testDataPtr,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to output data Buffer */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    uint16_t *testCrcPtr = NULL;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC16(&initParms,&testData,len,testCrcPtr);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));
}


TEST_F(BCDS_CRCtest, Crc16Success)
{
	/** @testcase{ BCDS_CRCtest::Crc16Success: }
	 * MCU_CRC16() CRC 16-Bit calculate success test case.
	 */

	/* SETUP: Declare and initialize the test condition required by this test case. */
	Retcode_T retcode;
	struct MCU_CRC16_S initParms ;
	initParms.GeneratePolynomial      = UINT16_C(1);
	initParms.InitVal                 = UINT16_C(1);
    uint16_t testData                 = UINT16_C(1);
    uint16_t testCrc                  = UINT16_C(1);
    uint16_t CrcVal                   = UINT16_C(28);
    uint32_t len                      = UINT32_C(1);
	HAL_CRC_Init_fake.return_val      = HAL_OK;
	HAL_CRC_DeInit_fake.return_val    = HAL_OK;
	HAL_CRC_Calculate_fake.return_val = CrcVal;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC16(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
	EXPECT_EQ(testCrc, CrcVal);
	EXPECT_EQ(RETCODE_OK, retcode);
}


TEST_F(BCDS_CRCtest, Crc16RetcodeFailure)
{
	/** @testcase{ BCDS_CRCtest::Crc16retcodefail: }
	 * MCU_CRC16() return code failure test case.
	 */

	/* testcase for HAL_CRC_Init failure */

	/* SETUP: Declare and initialize the test condition required by this test case. */
	Retcode_T retcode;
	struct MCU_CRC16_S initParms ;
	initParms.GeneratePolynomial      = UINT16_C(1);
	initParms.InitVal                 = UINT16_C(1);
    uint16_t testData                 = UINT16_C(1);
    uint16_t testCrc                  = UINT16_C(1);
    uint16_t CrcVal                   = UINT16_C(28);
    uint32_t len                      = UINT32_C(1);
	HAL_CRC_Init_fake.return_val      = HAL_ERROR;
	HAL_CRC_DeInit_fake.return_val    = HAL_OK;
	HAL_CRC_Calculate_fake.return_val = CrcVal;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC16(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_CRC_HAL_INIT_FAIL, Retcode_GetCode(retcode));

	/* testcase for HAL_CRC_DeInit failure */

	/* SETUP: Declare and initialize the test condition required by this test case. */
	initParms.GeneratePolynomial      = UINT16_C(0);
	initParms.InitVal                 = UINT16_C(0);
	HAL_CRC_Init_fake.return_val      = HAL_OK;
	HAL_CRC_DeInit_fake.return_val    = HAL_ERROR;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC16(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(2U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_CRC_HAL_DEINIT_FAIL, Retcode_GetCode(retcode));

}


TEST_F(BCDS_CRCtest, Crc32InvalidParmeters )
{
	/** @testcase{ BCDS_CRCtest::Crc32InvalidParmeters: }
	 * MCU_CRC32() invalid parameters(NULL) test cases.
	 */

    /*Failure due to NULL parameters */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    Retcode_T retcode;
    struct MCU_CRC32_S *initParm       = NULL;
    uint32_t testData       		   = UINT32_C(1);
    uint32_t testCrc                   = UINT32_C(1);
    uint32_t len                       = UINT32_C(1);

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC32(initParm,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));


    /*Failure due to NULL pointer assigned to input data Buffer */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    struct MCU_CRC32_S initParms;
	initParms.GeneratePolynomial      = UINT32_C(1);
	initParms.InitVal                 = UINT32_C(1);
	initParms.DataFormat			  = MCU_CRC_DATA_8BIT;
    uint32_t *testDataPtr             = NULL;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC32(&initParms,testDataPtr,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

    /*Failure due to NULL pointer assigned to output data Buffer */

	/* SETUP: Declare and initialize the test condition required by this test case.*/
    uint32_t *testCrcPtr = NULL;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs */
    retcode = MCU_CRC32(&initParms,&testData,len,testCrcPtr);

	/* VERIFY : Compare the expected value with the actual one. */
    EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
    EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(retcode));

	/* SETUP: Declare and initialize the test condition required by this test case. */
    testData                  			= UINT32_C(1);
    testCrc                   			= UINT32_C(1);
    len                       			= UINT32_C(1);
	initParms.GeneratePolynomial    	= UINT32_C(0);
	initParms.InitVal               	= UINT32_C(0);
	initParms.DataFormat				= (MCU_CRC_DataType_E)5;
	HAL_CRC_Init_fake.return_val    	= HAL_OK;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC32(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(0U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(BCDS_CRCtest, Crc32Success)
{
	/** @testcase{ BCDS_CRCtest::Crc32Success: }
	 * MCU_CRC32() CRC calculate success test case.
	 */

	/* SETUP: Declare and initialize the test condition required by this test case. */
	Retcode_T retcode;
	struct MCU_CRC32_S initParms ;
	initParms.GeneratePolynomial       = MCU_CRC32_POLY_ETHERNET;
	initParms.InitVal                  = MCU_CRC32_INIT_VALUE_DEFAULT;
	initParms.DataFormat               = MCU_CRC_DATA_8BIT;
    uint32_t testData                  = UINT32_C(1);
    uint32_t testCrc                   = UINT32_C(1);
    uint32_t CrcVal                    = UINT32_C(28);
    uint32_t len                       = UINT32_C(1);
	HAL_CRC_Init_fake.return_val       = HAL_OK;
	HAL_CRC_DeInit_fake.return_val     = HAL_OK;
	HAL_CRC_Calculate_fake.return_val  = CrcVal;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC32(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
	EXPECT_EQ(testCrc, CrcVal);
	EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(BCDS_CRCtest, Crc32RetcodeFailure)
{
	/** @testcase{ BCDS_CRCtest::Crc32RetcodeFailure: }
	 * MCU_CRC32() retcode failure test case.
	 */

	/* SETUP: Declare and initialize the test condition required by this test case. */
	Retcode_T retcode;
	struct MCU_CRC32_S initParms ;
	initParms.GeneratePolynomial      = MCU_CRC32_POLY_ETHERNET;
	initParms.InitVal                 = MCU_CRC32_INIT_VALUE_DEFAULT;
	initParms.DataFormat			  = MCU_CRC_DATA_32BIT;
    uint32_t testData                 = UINT32_C(1);
    uint32_t testCrc                  = UINT32_C(1);
    uint32_t CrcVal                   = UINT32_C(28);
    uint32_t len                      = UINT32_C(1);
	HAL_CRC_Init_fake.return_val      = HAL_ERROR;
	HAL_CRC_DeInit_fake.return_val    = HAL_OK;
	HAL_CRC_Calculate_fake.return_val = CrcVal;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC32(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(1U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(0U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(0U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_CRC_HAL_INIT_FAIL, Retcode_GetCode(retcode));

	/* SETUP: Declare and initialize the test condition required by this test case. */
	initParms.GeneratePolynomial    = UINT32_C(0);
	initParms.InitVal               = UINT32_C(0);
	initParms.DataFormat			= MCU_CRC_DATA_16BIT;
	HAL_CRC_Init_fake.return_val    = HAL_OK;
	HAL_CRC_DeInit_fake.return_val  = HAL_ERROR;

	/* EXECISE: calls relevant code Interface with the corresponding test inputs.  */
	retcode = MCU_CRC32(&initParms,&testData,len,&testCrc);

	/* VERIFY : Compare the expected value with the actual one. */
	EXPECT_EQ(2U, HAL_CRC_Init_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_DeInit_fake.call_count);
	EXPECT_EQ(1U, HAL_CRC_Calculate_fake.call_count);
    EXPECT_EQ(RETCODE_SEVERITY_ERROR, Retcode_GetSeverity(retcode));
    EXPECT_EQ(RETCODE_CRC_HAL_DEINIT_FAIL, Retcode_GetCode(retcode));
}
