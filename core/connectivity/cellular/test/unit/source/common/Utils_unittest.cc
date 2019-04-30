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

#define portNUM_CONFIGURABLE_REGIONS    1
#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_UTILS

#include "CellularModule.h"
#include "AtResponseParser_th.hh"
#include "AtResponseQueue_th.hh"

#undef RETCODE
#define RETCODE(severity,code) ((Retcode_T) code)

#undef BCDS_MODULE_ID
#include "Utils.c"

#ifndef countof
#   define countof(a) (sizeof(a) / sizeof(*(a)))
#endif

static AtResponseCode_T WaitArbitraryResponseValue = AT_RESPONSE_CODE_OK;

static Retcode_T WaitArbitraryResponseResult = RETCODE_OK;

static Retcode_T WaitArbitaryResponseCustom(uint32_t timeout, AtResponseCode_T *response)
{
    BCDS_UNUSED(timeout);

    *response = WaitArbitraryResponseValue;
    return WaitArbitraryResponseResult;
}

} //-- extern "C"

FFF_DEFINITION_BLOCK_END

class CellularUtils: public testing::Test
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


TEST_F(CellularUtils, TestCellularUtilsHexToBin)
{
    uint8_t data[2];
    data[0] = '0';
    uint8_t bin[1];
    uint32_t BinLength = sizeof(bin);

    data[1] = 'a';
    EXPECT_EQ(RETCODE_OK, CellularUtils_HexToBin((const uint8_t *) data, bin, BinLength));
    EXPECT_EQ(0x0A, bin[0]);

    data[1] = 'A';
    EXPECT_EQ(RETCODE_OK, CellularUtils_HexToBin((const uint8_t *) data, bin, BinLength));
    EXPECT_EQ(0x0A, bin[0]);

    data[1] = '.';
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUtils_HexToBin((const uint8_t *) data, bin, BinLength));

    data[1] = ':';
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUtils_HexToBin((const uint8_t *) data, bin, BinLength));

    data[1] = 'G';
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUtils_HexToBin((const uint8_t *) data, bin, BinLength));

    data[1] = 'g';
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUtils_HexToBin((const uint8_t *) data, bin, BinLength));

    data[1] = '0';
    data[0] = 'g';
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUtils_HexToBin((const uint8_t *) data, bin, BinLength));
}

TEST_F(CellularUtils, TestCellularUtilsBinToHex)
{
    uint8_t data[1];
    char bin[2];
    uint32_t DataLength = sizeof(data);

    data[0] = 0x0A;
    CellularUtils_BinToHex((const uint8_t *) data, bin, DataLength);
    EXPECT_EQ('0', bin[0]);
    EXPECT_EQ('A', bin[1]);

    data[0] = 0xF3;
    CellularUtils_BinToHex((const uint8_t *) data, bin, DataLength);
    EXPECT_EQ('F', bin[0]);
    EXPECT_EQ('3', bin[1]);
}

TEST_F(CellularUtils, strtol)
{
    int32_t number = 0;
    const char *NumberString = "123";
    Retcode_T retcode = CellularUtils_Strtol((const uint8_t *) NumberString, strlen(NumberString),
            &number);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(123, number);

    NumberString = "a";
    EXPECT_EQ(RETCODE_FAILURE, CellularUtils_Strtol((const uint8_t *) NumberString, 1, &number));
    EXPECT_EQ(123, number);

    NumberString = ".";
    EXPECT_EQ(RETCODE_FAILURE, CellularUtils_Strtol((const uint8_t *) NumberString, 1, &number));
    EXPECT_EQ(123, number);
}

TEST_F(CellularUtils, StrtolTestOverflow)
{
    int32_t number = 123;
    const char *NumberString = "9999999999";
    Retcode_T retcode = CellularUtils_Strtol((const uint8_t *) NumberString, strlen(NumberString),
            &number);

    EXPECT_EQ(RETCODE_INVALID_PARAM, retcode);
    EXPECT_EQ(123, number);
}

TEST_F(CellularUtils, StrtolTestObviousOverflow)
{
    int32_t number = 123;
    const char *NumberString = "999999999999999999";
    Retcode_T retcode = CellularUtils_Strtol((const uint8_t *) NumberString, strlen(NumberString),
            &number);

    EXPECT_EQ(RETCODE_INVALID_PARAM, retcode);
    EXPECT_EQ(123, number);
}

TEST_F(CellularUtils, TestCellularUtilsStrtolFail)
{
    uint8_t buffer[1];
    int32_t number;

    EXPECT_EQ(RETCODE_NULL_POINTER, CellularUtils_Strtol(NULL, 1, &number));
    EXPECT_EQ(RETCODE_NULL_POINTER, CellularUtils_Strtol(buffer, 1, NULL));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUtils_Strtol(buffer, 12, &number));
}

TEST_F(CellularUtils, TestParseIpAddress)
{
    Retcode_T retcode;
    uint32_t ResultIpAddress;

    retcode = CellularUtils_ParseIpAddress((uint8_t *)"192.168.2.100", strlen("192.168.2.100"),
            &ResultIpAddress);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ((uint32_t)((((((192 << 8) | 168) << 8) | 2) << 8) | 100), ResultIpAddress);

    retcode = CellularUtils_ParseIpAddress((uint8_t *)"300.168.2.100", strlen("300.168.2.100"),
            &ResultIpAddress);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    retcode = CellularUtils_ParseIpAddress((uint8_t *)"200.1680.2.100", strlen("200.1680.2.100"),
            &ResultIpAddress);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    retcode = CellularUtils_ParseIpAddress((uint8_t *)"100.168.2", strlen("100.168.2"),
            &ResultIpAddress);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    retcode = CellularUtils_ParseIpAddress(NULL, 0, &ResultIpAddress);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));

    retcode = CellularUtils_ParseIpAddress((uint8_t *)"200.1680.2.100", 0, &ResultIpAddress);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));

    retcode = CellularUtils_ParseIpAddress((uint8_t *)"200.1680.2.100", strlen("200.1680.2.100"),
            NULL);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));

    retcode = CellularUtils_ParseIpAddress((uint8_t *)"20.0016.2.100", strlen("200.0016.2.100"),
            &ResultIpAddress);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUtils_ParseIpAddress((uint8_t *)"192.168.2.100",
            CELLULAR_UTILS_MAX_IP_STR_LENGTH + 1, &ResultIpAddress));
    EXPECT_EQ(RETCODE_FAILURE, CellularUtils_ParseIpAddress((uint8_t *)"192.+.2.100",
            strlen("192.+.2.100"), &ResultIpAddress));
    EXPECT_EQ(RETCODE_FAILURE, CellularUtils_ParseIpAddress((uint8_t *)"192.a.2.100",
            strlen("192.a.2.100"), &ResultIpAddress));
}

TEST_F(CellularUtils, TestCellularUtilsConvertAtResponseRetcodeToCellularRetcode)
{
    uint32_t code;

    for(code = RETCODE_AT_RESPONSE_QUEUE_TIMEOUT;
            code <= RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE; code++)
    {
        Retcode_T result = CellularUtils_ConvertAtResponseRetcodeToCellularRetcode((Retcode_T)code);
        EXPECT_EQ(true, ((result >= RETCODE_CELLULAR_NOT_RESPONSIVE) &&
                (result <= RETCODE_CELLULAR_SIM_NOT_READY)));
    }
}

TEST_F(CellularUtils, TestCellularUtilsConvertCellularResponseCodeToCellularRetcode)
{
    uint32_t code;

    EXPECT_EQ(RETCODE_OK, CellularUtils_ConvertCellularResponseCodeToCellularRetcode(AT_RESPONSE_CODE_OK));

    for(code = AT_RESPONSE_CODE_CONNECT; code <= AT_RESPONSE_CODE_ABORTED; code++)
    {
        Retcode_T result = CellularUtils_ConvertCellularResponseCodeToCellularRetcode((AtResponseCode_T)code);
        EXPECT_EQ(true, ((result >= RETCODE_CELLULAR_NOT_RESPONSIVE) &&
                (result <= RETCODE_CELLULAR_SIM_NOT_READY)));
    }
}

TEST_F(CellularUtils, TestCellularUtilsStrtolBounds)
{
    struct {
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
            {".", -1, -3, -1, RETCODE_FAILURE}
    };

    Retcode_T retcode;
    int32_t number;

    for(uint32_t i = 0; i < countof(a); i++)
    {
        retcode = CellularUtils_StrtolBounds((uint8_t *)a[i].str, strlen(a[i].str), &number,
                a[i].LowerBound, a[i].UpperBound);
        EXPECT_EQ(retcode, a[i].retcode);
        EXPECT_EQ(number, a[i].number);
    }
}

TEST_F(CellularUtils, TestCellularUtilsWaitForAndHandleResponseCode)
{
    AtResponseQueue_WaitForArbitraryResponseCode_fake.custom_fake = WaitArbitaryResponseCustom;
    WaitArbitraryResponseValue = AT_RESPONSE_CODE_OK;
    EXPECT_EQ(RETCODE_OK, CellularUtils_WaitForAndHandleResponseCode(0, RETCODE_OK));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUtils_WaitForAndHandleResponseCode(0,
            RETCODE_INVALID_PARAM));
    WaitArbitraryResponseValue = AT_RESPONSE_CODE_ERROR;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONDED_ERROR, CellularUtils_WaitForAndHandleResponseCode(0,
            RETCODE_OK));
    WaitArbitraryResponseResult = RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT;
    WaitArbitraryResponseValue = AT_RESPONSE_CODE_ERROR;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularUtils_WaitForAndHandleResponseCode(0,
            RETCODE_OK));
    WaitArbitraryResponseResult = RETCODE_OK;
}

TEST_F(CellularUtils, TestCellularUtilsTrimFlukeCharacters)
{
    const char *TestBuffer = "\0AT+CGMM\r\r\n";
    uint8_t *result;
    uint32_t NewLength;

    result = CellularUtils_TrimFlukeCharacters((uint8_t *) TestBuffer, 11, &NewLength);
    EXPECT_EQ(10U, NewLength);
    EXPECT_EQ((uint8_t *)TestBuffer + 1, result);

    TestBuffer = "\tAT+CGMM\r\r\n";
    result = CellularUtils_TrimFlukeCharacters((uint8_t *) TestBuffer, 11, &NewLength);
    EXPECT_EQ(10U, NewLength);
    EXPECT_EQ((uint8_t *)TestBuffer + 1, result);
    TestBuffer = "\rAT+CGM\tM\r\n";
    result = CellularUtils_TrimFlukeCharacters((uint8_t *) TestBuffer, 11, &NewLength);
    EXPECT_EQ(11U, NewLength);
    EXPECT_EQ((uint8_t *)TestBuffer, result);
    TestBuffer = "\0";
    result = CellularUtils_TrimFlukeCharacters((uint8_t *) TestBuffer, 1, &NewLength);
    EXPECT_EQ(0U, NewLength);
    EXPECT_EQ((uint8_t *)TestBuffer+1, result);
    TestBuffer = "\n";
    result = CellularUtils_TrimFlukeCharacters((uint8_t *) TestBuffer, 1, &NewLength);
    EXPECT_EQ(1U, NewLength);
    EXPECT_EQ((uint8_t *)TestBuffer, result);

    uint8_t TestValue[1];
    TestValue[0]= 0x7F;
    result = CellularUtils_TrimFlukeCharacters((uint8_t *) TestValue, 1, &NewLength);
    EXPECT_EQ(0U, NewLength);
    EXPECT_EQ((uint8_t *)TestValue+1, result);
}
