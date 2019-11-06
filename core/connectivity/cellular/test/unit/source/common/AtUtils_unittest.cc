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

#include <gtest.h>

FFF_DEFINITION_BLOCK_START

extern "C"
{

#ifndef GTEST
#define GTEST
#endif /* GTEST */

#define portNUM_CONFIGURABLE_REGIONS 1

#include "AtResponseParser_th.hh"
#include "AtResponseQueue_th.hh"

#undef RETCODE
#define RETCODE(severity, code) ((Retcode_T)code)

#undef KISO_MODULE_ID
#include "AtUtils.c"

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(*(a)))
#endif

    static AtResponseCode_T WaitArbitraryResponseValue = AT_RESPONSE_CODE_OK;

    static Retcode_T WaitArbitraryResponseResult = RETCODE_OK;

    static Retcode_T WaitArbitaryResponseCustom(uint32_t timeout, AtResponseCode_T *response)
    {
        KISO_UNUSED(timeout);

        *response = WaitArbitraryResponseValue;
        return WaitArbitraryResponseResult;
    }

} //-- extern "C"

FFF_DEFINITION_BLOCK_END

class Utils : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE(AtResponseQueue_WaitForArbitraryResponseCode);
    }

    virtual void TearDown()
    {
        ;
    }
};

TEST_F(Utils, strtol)
{
    int32_t number = 0;
    const char *NumberString = "123";
    Retcode_T retcode = Utils_Strtol((const uint8_t *)NumberString, strlen(NumberString),
                                     &number);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(123, number);

    NumberString = "a";
    EXPECT_EQ(RETCODE_FAILURE, Utils_Strtol((const uint8_t *)NumberString, 1, &number));
    EXPECT_EQ(123, number);

    NumberString = ".";
    EXPECT_EQ(RETCODE_FAILURE, Utils_Strtol((const uint8_t *)NumberString, 1, &number));
    EXPECT_EQ(123, number);
}

TEST_F(Utils, StrtolTestOverflow)
{
    int32_t number = 123;
    const char *NumberString = "9999999999";
    Retcode_T retcode = Utils_Strtol((const uint8_t *)NumberString, strlen(NumberString),
                                     &number);

    EXPECT_EQ(RETCODE_INVALID_PARAM, retcode);
    EXPECT_EQ(123, number);
}

TEST_F(Utils, StrtolTestObviousOverflow)
{
    int32_t number = 123;
    const char *NumberString = "999999999999999999";
    Retcode_T retcode = Utils_Strtol((const uint8_t *)NumberString, strlen(NumberString),
                                     &number);

    EXPECT_EQ(RETCODE_INVALID_PARAM, retcode);
    EXPECT_EQ(123, number);
}

TEST_F(Utils, TestUtilsStrtolFail)
{
    uint8_t buffer[1];
    int32_t number;

    EXPECT_EQ(RETCODE_NULL_POINTER, Utils_Strtol(NULL, 1, &number));
    EXPECT_EQ(RETCODE_NULL_POINTER, Utils_Strtol(buffer, 1, NULL));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Utils_Strtol(buffer, 12, &number));
}

TEST_F(Utils, TestUtilsConvertAtResponseRetcodeToCellularRetcode)
{
    uint32_t code;

    for (code = RETCODE_AT_RESPONSE_QUEUE_TIMEOUT;
         code <= RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE; code++)
    {
        Retcode_T result = Utils_ConvertAtResponseRetcodeToCellularRetcode((Retcode_T)code);
        EXPECT_EQ(true, ((result >= RETCODE_CELLULAR_NOT_RESPONSIVE) &&
                         (result <= RETCODE_CELLULAR_SIM_NOT_READY)));
    }
}

TEST_F(Utils, TestUtilsConvertCellularResponseCodeToCellularRetcode)
{
    uint32_t code;

    EXPECT_EQ(RETCODE_OK, Utils_ConvertCellularResponseCodeToCellularRetcode(AT_RESPONSE_CODE_OK));

    for (code = AT_RESPONSE_CODE_CONNECT; code <= AT_RESPONSE_CODE_ABORTED; code++)
    {
        Retcode_T result = Utils_ConvertCellularResponseCodeToCellularRetcode((AtResponseCode_T)code);
        EXPECT_EQ(true, ((result >= RETCODE_CELLULAR_NOT_RESPONSIVE) &&
                         (result <= RETCODE_CELLULAR_SIM_NOT_READY)));
    }
}

TEST_F(Utils, TestUtilsStrtolBounds)
{
    struct
    {
        const char *str;
        int32_t number;
        int32_t LowerBound;
        int32_t UpperBound;
        Retcode_T retcode;
    } a[] =
        {
            {"5", 5, 3, 7, RETCODE_OK},
            {"3", 3, 3, 7, RETCODE_OK},
            {"7", 7, 3, 7, RETCODE_OK},
            {"8", 7, 3, 7, RETCODE_INVALID_PARAM},
            {"2", 3, 3, 7, RETCODE_INVALID_PARAM},
            {"-2", -2, -3, -1, RETCODE_OK},
            {"-4", -3, -3, -1, RETCODE_INVALID_PARAM},
            {"0", -1, -3, -1, RETCODE_INVALID_PARAM},
            {".", -1, -3, -1, RETCODE_FAILURE}};

    Retcode_T retcode;
    int32_t number;

    for (uint32_t i = 0; i < countof(a); i++)
    {
        retcode = Utils_StrtolBounds((const uint8_t *)a[i].str, strlen(a[i].str), &number,
                                     a[i].LowerBound, a[i].UpperBound);
        EXPECT_EQ(retcode, a[i].retcode);
        EXPECT_EQ(number, a[i].number);
    }
}

TEST_F(Utils, TestUtilsWaitForAndHandleResponseCode)
{
    AtResponseQueue_WaitForArbitraryResponseCode_fake.custom_fake = WaitArbitaryResponseCustom;
    WaitArbitraryResponseValue = AT_RESPONSE_CODE_OK;
    EXPECT_EQ(RETCODE_OK, Utils_WaitForAndHandleResponseCode(0, RETCODE_OK));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Utils_WaitForAndHandleResponseCode(0,
                                                                        RETCODE_INVALID_PARAM));
    WaitArbitraryResponseValue = AT_RESPONSE_CODE_ERROR;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONDED_ERROR, Utils_WaitForAndHandleResponseCode(0,
                                                                                   RETCODE_OK));
    WaitArbitraryResponseResult = RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT;
    WaitArbitraryResponseValue = AT_RESPONSE_CODE_ERROR;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, Utils_WaitForAndHandleResponseCode(0,
                                                                                       RETCODE_OK));
    WaitArbitraryResponseResult = RETCODE_OK;
}

TEST_F(Utils, TestUtilsTrimFlukeCharacters)
{
    const char *TestBuffer = "\0AT+CGMM\r\r\n";
    const uint8_t *result;
    uint32_t NewLength;

    result = Utils_TrimFlukeCharacters((const uint8_t *)TestBuffer, 11, &NewLength);
    EXPECT_EQ(10U, NewLength);
    EXPECT_EQ((const uint8_t *)TestBuffer + 1, result);

    TestBuffer = "\tAT+CGMM\r\r\n";
    result = Utils_TrimFlukeCharacters((const uint8_t *)TestBuffer, 11, &NewLength);
    EXPECT_EQ(10U, NewLength);
    EXPECT_EQ((const uint8_t *)TestBuffer + 1, result);
    TestBuffer = "\rAT+CGM\tM\r\n";
    result = Utils_TrimFlukeCharacters((const uint8_t *)TestBuffer, 11, &NewLength);
    EXPECT_EQ(11U, NewLength);
    EXPECT_EQ((const uint8_t *)TestBuffer, result);
    TestBuffer = "\0";
    result = Utils_TrimFlukeCharacters((const uint8_t *)TestBuffer, 1, &NewLength);
    EXPECT_EQ(0U, NewLength);
    EXPECT_EQ((const uint8_t *)TestBuffer + 1, result);
    TestBuffer = "\n";
    result = Utils_TrimFlukeCharacters((const uint8_t *)TestBuffer, 1, &NewLength);
    EXPECT_EQ(1U, NewLength);
    EXPECT_EQ((const uint8_t *)TestBuffer, result);

    uint8_t TestValue[1];
    TestValue[0] = 0x7F;
    result = Utils_TrimFlukeCharacters((const uint8_t *)TestValue, 1, &NewLength);
    EXPECT_EQ(0U, NewLength);
    EXPECT_EQ((const uint8_t *)TestValue + 1, result);
}
