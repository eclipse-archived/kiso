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

#define portNUM_CONFIGURABLE_REGIONS    1
#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_NETWORK

#include "CellularModule.h"
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "queue_th.hh"
#include "AtResponseParser_th.hh"
#include "AtResponseQueue_th.hh"
#include "Engine_th.hh"
#include "Control_th.hh"
#include "Socket_th.hh"

#undef RETCODE
#define RETCODE(severity,code) ((Retcode_T) code)

#undef BCDS_MODULE_ID
#include "Utils.c"

#undef BCDS_MODULE_ID
#include "Network.c"

#include "Log_th.hh"

} //-- extern "C"

FFF_DEFINITION_BLOCK_END

const char *CmdArgCustomResp0[] =
{
        "0", "0", "0", "0", "0", "0", "0", NULL
};
const char *CmdArgCustomResp1[] =
{
        "0", "0", "192.168.1.2", NULL
};
const char *CmdArgCustomResp2[] =
{
        "0", "8", "1", NULL
};
const char *CmdArgCustomResp3[] =
{
        "0", "192.168.1.2", NULL
};
const char *CmdArgCustomResp4[] =
{
        "RAT:\"UMTS\"", "MCC:240, MNC:  5, LAC:0091, CI:0910620, DLF:10812, ULF:9862", NULL
};
const char *CmdArgCustomResp5[] =
{
        "1", "5", "9", "10", NULL
};
const char *CmdArgCustomResp6[] =
{
        "0", "0", "0", "RAT:\"UMTS\"",
        "MCC:240, MNC:  5, LAC:0091, CI:0910620, DLF:10812, ULF:9862", NULL
};
const char *CmdArgCustomResp7[] =
{
        "0", "0", "8", NULL
};
const char *CmdArgCustomResp8[] =
{
        "0", "8", "192.168.1.2", NULL
};

static const char **CmdArgCustomResp = NULL;

static Retcode_T WaitArbitraryCmdArgCustom(uint32_t timeout, uint8_t **BufferPtr,
        uint32_t *BufferLen)
{
    BCDS_UNUSED(timeout);

    if (!CmdArgCustomResp || !*CmdArgCustomResp)
    {
        return RETCODE_INVALID_PARAM;
    }

    *BufferPtr = (uint8_t *) *CmdArgCustomResp;
    *BufferLen = strlen(*CmdArgCustomResp);
    ++CmdArgCustomResp;
    return RETCODE_OK;
}

static Retcode_T WaitArbitaryResponseCustom(uint32_t timeout, AtResponseCode_T *response)
{
    BCDS_UNUSED(timeout);

    *response = AT_RESPONSE_CODE_OK;
    return RETCODE_OK;
}

static Retcode_T RequestDispatchCustom(uint32_t timeout,
        CellularRequest_CallableFunction_T function, void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(timeout);
    return function(parameter, ParameterLength);
}

static Retcode_T sendAtCmdWaitEcho_Custom(uint32_t timeout, const char *str)
{
    (void) CellularEngine_SendAtCommand((uint8_t *) str, strlen(str));
    return AtResponseQueue_WaitForNamedCmdEcho(timeout, (uint8_t *) str, strlen(str) - 2);
}

class CellularControlNetwork: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE(CellularEngine_SendAtCommand);
        RESET_FAKE(CellularRequest_Dispatch);
        RESET_FAKE(AtResponseQueue_WaitForArbitraryResponseCode);
        RESET_FAKE(AtResponseQueue_WaitForNamedCmdEcho);
        RESET_FAKE(AtResponseQueue_WaitForNamedCmd);
        RESET_FAKE(AtResponseQueue_WaitForArbitraryCmdArg);
        RESET_FAKE(AtResponseQueue_WaitForMiscContent);
        RESET_FAKE(AtResponseQueue_WaitForNamedResponseCode);
        RESET_FAKE(Cellular_IsSimCardInserted);

        CellularRequest_Dispatch_fake.custom_fake = RequestDispatchCustom;
        AtResponseQueue_WaitForArbitraryCmdArg_fake.custom_fake = WaitArbitraryCmdArgCustom;
        CellularEngine_SendAtCmdWaitEcho_fake.custom_fake = sendAtCmdWaitEcho_Custom;
        AtResponseQueue_WaitForArbitraryResponseCode_fake.custom_fake =
                WaitArbitaryResponseCustom;

    }

    virtual void TearDown()
    {
        ;
    }
};

TEST_F(CellularControlNetwork, TestConvertToNRegStatusN)
{
    //-- Retcode_T ConvertToNRegStatusN(uint8_t* data, uint32_t length,
    //      CellularNetwork_RegistrationInfo_T* value)
    CellularNetwork_RegistrationInfo_T eValue;
    Retcode_T ret;
    uint8_t data[255];
    uint32_t length = 1;
    data[1] = 0x41;
    data[0] = 0x30;

    ret = ConvertToNRegStatusN(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_DEFAULT);
    data[0] = 0x31;
    ret = ConvertToNRegStatusN(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_URC);
    data[0] = 0x32;
    ret = ConvertToNRegStatusN(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_URC_LOC);
    data[0] = 0x33;
    ret = ConvertToNRegStatusN(data, length, &eValue);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_INVALID);
    data[1] = 0;
    data[0] = 48;
    ret = ConvertToNRegStatusN(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_DEFAULT);
    data[0] = 0x31;
    ret = ConvertToNRegStatusN(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_URC);
    data[0] = 0x32;
    ret = ConvertToNRegStatusN(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_URC_LOC);
    data[0] = 0xFF;
    ret = ConvertToNRegStatusN(data, length, &eValue);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_INVALID);
    ret = ConvertToNRegStatusN(data, 666, &eValue);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_INFO_INVALID);
}

TEST_F(CellularControlNetwork, TestConvertToNRegStatusNFail)
{
    uint8_t data[2];
    CellularNetwork_RegistrationInfo_T value;

    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNRegStatusN(NULL, 1, &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNRegStatusN(data, 0, &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNRegStatusN(data, 1, NULL));
}

TEST_F(CellularControlNetwork, TestConvertToNRegStatusStat)
{
    //-- Retcode_T ConvertToNRegStatusStat(uint8_t* data, uint32_t length,
    //      CellularNetwork_RegistrationStatusStat_T* value)
    CellularNetwork_RegistrationStatusStat_T eValue;
    Retcode_T ret;
    uint8_t data[255];
    uint32_t length = 1;
    data[1] = 0x41;
    data[0] = 0x30;
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_NOT);
    data[0] = 0x31;
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_HOME);
    data[0] = 0x32;
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_NOT_AND_SEARCH);
    data[0] = 0x33;
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_DENIED);
    data[0] = 0x34;
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_UNKOWN);
    data[0] = 0x35;
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_ROAMING);
    data[0] = 0x36;
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_SMSONLY_HOME);
    data[0] = 0x37;
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_SMSONLY_ROAMING);
    data[0] = 0x39;//-- 57 dec
    ret = ConvertToNRegStatusStat(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_CSFB_NOT_PREF_HOME);

    strncpy((char*) data, "10", sizeof(data));
    ret = ConvertToNRegStatusStat(data, 2, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_CSFB_NOT_PREF_ROAMING);

    strncpy((char*) data, "255", sizeof(data));
    ret = ConvertToNRegStatusStat(data, 3, &eValue);
    EXPECT_NE(ret, RETCODE_OK);

    strncpy((char*) data, "8", sizeof(data));
    ret = ConvertToNRegStatusStat(data, 1, &eValue);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_INVALID);

    strncpy((char*) data, "33", sizeof(data));
    ret = ConvertToNRegStatusStat(data, 2, &eValue);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_REG_STAT_INVALID);
}

TEST_F(CellularControlNetwork, TestConvertToNRegStatusStatFail)
{
    uint8_t data[2];
    CellularNetwork_RegistrationStatusStat_T value;

    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNRegStatusStat(NULL, 1, &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNRegStatusStat(data, 3, &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNRegStatusStat(data, 1, NULL));
}

TEST_F(CellularControlNetwork, TestGetNRegStatusCi)
{
    //-- static Retcode_T GetNRegStatusCi(uint8_t* data, uint32_t length, uint32_t* value)
    const char Test1[] = "CAFEBABE";//-- 3405691582 dec
    const char Test2[] = "0815";//-- 2069 dec
    const char Test3[] = "00";
    const char Test4[] = "0";
    const char Test5[] = "X";
    const char Test6[] = "Quatsch7";

    uint32_t value;
    Retcode_T ret;

    ret = GetNRegStatusCi((uint8_t*) Test1, sizeof(Test1), &value);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(0xCAFEBABE, value);

    ret = GetNRegStatusCi((uint8_t*) Test2, sizeof(Test2), &value);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(0x0815U, value);

    ret = GetNRegStatusCi((uint8_t*) Test3, sizeof(Test3), &value);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(0U, value);

    ret = GetNRegStatusCi((uint8_t*) Test4, sizeof(Test4), &value);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(0U, value);

    ret = GetNRegStatusCi((uint8_t*) Test5, sizeof(Test5), &value);
    EXPECT_NE(ret, RETCODE_OK);

    ret = GetNRegStatusCi((uint8_t*) Test6, sizeof(Test6), &value);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(0xFFFFFFFF, value);

    ret = GetNRegStatusCi(NULL, 42, &value);
    EXPECT_NE(ret, RETCODE_OK);
}

TEST_F(CellularControlNetwork, TestGetNRegStatusCiFail)
{
    const char data[] = " ";
    uint32_t length = strlen(data);
    uint32_t value;

    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, GetNRegStatusCi(NULL, length, &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, GetNRegStatusCi((uint8_t *) data, 0, &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, GetNRegStatusCi((uint8_t *) data, length, NULL));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, GetNRegStatusCi((uint8_t *) data, length,
            &value));
}

TEST_F(CellularControlNetwork, TestConvertToRadioAccessTechnology)
{
    // static Retcode_T ConvertToRadioAccessTechnology(uint8_t* data, uint32_t length,
    //      CellularNetwork_Rat_T* rat) //-- Ref 1: AcT
    CellularNetwork_Rat_T eValue;
    Retcode_T ret;
    uint8_t data[255];
    uint32_t length = 1;
    data[1] = 0x41;
    data[0] = 0x30;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_GSM);
    data[0] = 0x31;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_GSM_COMPACT);
    data[0] = 0x32;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_UTRAN_UMTS);
    data[0] = 0x32;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_UTRAN_UMTS);
    data[0] = 0x33;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_GSM_EDGE);
    data[0] = 0x34;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_UTRAN_HSDPA);
    data[0] = 0x35;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_UTRAN_HSUPA);
    data[0] = 0x36;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_UTRAN_HSDPA_HSUPA);

    data[0] = 0x37;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_LTE);

    strncpy((char*) data, "255", sizeof(data));
    ret = ConvertToRadioAccessTechnology(data, 3, &eValue);
    EXPECT_EQ(ret, RETCODE_OK);//-- sic
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_INVALID);//-- sic

    data[0] = 23;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_INVALID);

    data[0] = 5;
    ret = ConvertToRadioAccessTechnology(data, length, &eValue);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(eValue, CELLULAR_NETWORK_RAT_INVALID);
}

TEST_F(CellularControlNetwork, TestConvertToRadioAccessTechnologyFail)
{
    CellularNetwork_Rat_T value;
    const char data[2] = " ";
    uint32_t length = strlen(data);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRadioAccessTechnology(NULL, length,
            &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRadioAccessTechnology((uint8_t *) data,
            0, &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRadioAccessTechnology((uint8_t *) data,
            4, &value));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRadioAccessTechnology((uint8_t *) data,
            length, NULL));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRadioAccessTechnology((uint8_t *) data,
            length, &value));
}

TEST_F(CellularControlNetwork, TestGetNRegStatusLac)
{
    //-- Retcode_T GetNRegStatusLac(uint8_t* data, uint32_t length, uint32_t* value)
    const char Test1[] = "BABE";
    const char Test2[] = "6";
    const char Test3[] = "ALIEN SEX FIEND  FLY IN THE OINTMENT";
    uint32_t value;
    Retcode_T ret;

    ret = GetNRegStatusLac((uint8_t*) Test1, sizeof(Test1), &value);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(0xBABEU, value);

    ret = GetNRegStatusLac((uint8_t*) Test2, sizeof(Test2), &value);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(0x6U, value);

    ret = GetNRegStatusLac((uint8_t*) Test3, sizeof(Test3), &value);
    EXPECT_EQ(ret, RETCODE_OK);
    EXPECT_EQ(0xAU, value);

    ret = GetNRegStatusLac((uint8_t*) Test3 + 1, sizeof(Test3) - 1, &value);
    EXPECT_NE(ret, RETCODE_OK);
    EXPECT_EQ(0xFFFFU, value);

    ret = GetNRegStatusLac(NULL, 13, &value);
    EXPECT_NE(ret, RETCODE_OK);

    ret = GetNRegStatusLac(NULL, 13, NULL);
    EXPECT_NE(ret, RETCODE_OK);
}

TEST_F(CellularControlNetwork, TestCellularQueryNetworkRegistrationStatusFail)
{
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryRegistrationStatus(NULL));
    CellularNetwork_RegistrationStatus_T status;

    CmdArgCustomResp = CmdArgCustomResp2;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRegistrationStatus(&status));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryRegistrationStatus(&status));
    CmdArgCustomResp = CmdArgCustomResp2;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRegistrationStatus(&status));
    CmdArgCustomResp = CmdArgCustomResp2;
    ++CmdArgCustomResp;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryRegistrationStatus(&status));
    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryRegistrationStatus(&status));
    CmdArgCustomResp = CmdArgCustomResp4;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryRegistrationStatus(&status));

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryRegistrationStatus(&status));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryRegistrationStatus(&status));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryRegistrationStatus(&status));
}

TEST_F(CellularControlNetwork, TestCellularQueryNetworkRegistrationStatusPass)
{
    CellularNetwork_RegistrationStatus_T status;
    CmdArgCustomResp = CmdArgCustomResp0;

    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRegistrationStatus(&status));
    CmdArgCustomResp = CmdArgCustomResp0;
    ++CmdArgCustomResp;
    ++CmdArgCustomResp;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRegistrationStatus(&status));
    EXPECT_EQ(2U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(10U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularConfigureNetworkRegistrationInformation)
{
    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK,
            CellularNetwork_ConfigureRegistrationInformation(CELLULAR_NETWORK_REG_INFO_URC_LOC));
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE,
            CellularNetwork_ConfigureRegistrationInformation(CELLULAR_NETWORK_REG_INFO_URC_LOC));
}

TEST_F(CellularControlNetwork, TestCellularNetworkUrcCREG)
{
    CmdArgCustomResp = CmdArgCustomResp+3;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp0+4;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp0+5;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp5;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp5+1;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp5+2;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp5+3;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    EXPECT_EQ(9U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(29U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularNetworkUrc_CREG());
}

TEST_F(CellularControlNetwork, TestCellularDeregisterFromNetwork)
{
    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK, Cellular_DeregisterFromNetwork());
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_DeregisterFromNetwork());
}

TEST_F(CellularControlNetwork, TestCellularEnableAutomaticNetworkSelection)
{
    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_EnableAutomaticSelection());
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_EnableAutomaticSelection());
}

TEST_F(CellularControlNetwork, TestCellularQueryCurrentNetworkOperatorFail)
{
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentOperator(NULL));
    CellularNetwork_CurrentOperator_T op =
    {
            (CellularNetwork_SelectionMode_T) 0, (CellularNetwork_FormatOperatorName_T) 0,
            (CellularNetwork_Rat_T) 0, false, false, false, false, ""
    };

    CmdArgCustomResp = CmdArgCustomResp0+3;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp = CmdArgCustomResp0+4;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp = CmdArgCustomResp0+5;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp = CmdArgCustomResp0+6;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryCurrentOperator(&op));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentOperator(&op));

    CmdArgCustomResp = CmdArgCustomResp5+2;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp = CmdArgCustomResp5+3;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryCurrentOperator(&op));

    CmdArgCustomResp = CmdArgCustomResp5;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp = CmdArgCustomResp6;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp = CmdArgCustomResp6;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryCurrentOperator(&op));

    CmdArgCustomResp = CmdArgCustomResp0;
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentOperator(&op));

    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentOperator(&op));

    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentOperator(&op));
}

TEST_F(CellularControlNetwork, TestCellularQueryCurrentNetworkOperatorPass)
{
    CellularNetwork_CurrentOperator_T op =
    {
            (CellularNetwork_SelectionMode_T) 0, (CellularNetwork_FormatOperatorName_T) 0,
            (CellularNetwork_Rat_T) 0, false, false, false, false, ""
    };
    const char cmd[] = "AT+COPS?\r\n";
    CmdArgCustomResp = CmdArgCustomResp0;

    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryCurrentOperator(&op));

    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_NE(0, strncmp(op.Operator,"",strlen(op.Operator)));
    EXPECT_EQ(CellularEngine_SendAtCommand_fake.arg1_val, (uint32_t)strlen(cmd));

    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularSetNetworkAPNFail)
{
    const char apn[] = "test";

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SetAPN(NULL, strlen(apn)));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SetAPN((uint8_t *) apn, 0));

    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_SetAPN((uint8_t *) apn, strlen(apn)));
}

TEST_F(CellularControlNetwork, TestCellularSetNetworkAPNPass)
{
    const char apn[] = "test";

    EXPECT_EQ(RETCODE_OK, CellularNetwork_SetAPN((uint8_t *) apn, strlen(apn)));
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularSetPsdProfileFail)
{
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SetPsdProfile(NULL));

    Cellular_PsdProfile_T profile =
    {
            0, (Cellular_PsdProtocol_T) 0, NULL, NULL, NULL, NULL, NULL,
            (Cellular_PsdAuthentication_T) 0, NULL, (Cellular_PsdDataCompression_T) 0,
            (Cellular_PsdHeaderCompression_T) 0, (Cellular_PsdQosPrecedence_T) 0
    };

    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_SetPsdProfile(&profile));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_SetPsdProfile(&profile));
}

TEST_F(CellularControlNetwork, TestCellularSetPsdProfilePass)
{
    Cellular_PsdProfile_T profile =
    {
            0, (Cellular_PsdProtocol_T) 0, NULL, NULL, NULL, NULL, NULL,
            (Cellular_PsdAuthentication_T) 0, NULL, (Cellular_PsdDataCompression_T) 0,
            (Cellular_PsdHeaderCompression_T) 0, (Cellular_PsdQosPrecedence_T) 0
    };

    EXPECT_EQ(RETCODE_OK, CellularNetwork_SetPsdProfile(&profile));
    profile.Id = 0;
    profile.Apn = NULL;
    profile.Protocol = CELLULAR_PSD_PROTOCOL_IPV4;
    profile.Username = "username";
    profile.Password = "password";
    profile.Authentication = CELLULAR_PSD_AUTH_NONE;

    EXPECT_EQ(RETCODE_OK, CellularNetwork_SetPsdProfile(&profile));

    profile.Apn = "apn";
    EXPECT_EQ(RETCODE_OK, CellularNetwork_SetPsdProfile(&profile));
}

TEST_F(CellularControlNetwork, TestCellularActivatePdpContext)
{
    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_ActivatePdpContext());
    CmdArgCustomResp = CmdArgCustomResp1;

    EXPECT_EQ(RETCODE_OK, CellularNetwork_ActivatePdpContext());
    EXPECT_EQ(4U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(3U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_ActivatePdpContext());
}

TEST_F(CellularControlNetwork, TestCellularDeactivatePdpContext)
{
    EXPECT_EQ(RETCODE_OK, CellularNetwork_DeactivatePdpContext());
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularVerifyPdpContext)
{
    bool success = false;

    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_VerifyPdpContext(&success));
    CmdArgCustomResp = CmdArgCustomResp4;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, Cellular_VerifyPdpContext(&success));
    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, Cellular_VerifyPdpContext(&success));
    CmdArgCustomResp = CmdArgCustomResp0+6;
    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_VerifyPdpContext(&success));
    CmdArgCustomResp = CmdArgCustomResp7+1;
    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_VerifyPdpContext(&success));
    CmdArgCustomResp = CmdArgCustomResp8;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_VerifyPdpContext(&success));

    CmdArgCustomResp = CmdArgCustomResp2;

    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_VerifyPdpContext(NULL));
    EXPECT_EQ(RETCODE_OK, Cellular_VerifyPdpContext(&success));
    EXPECT_EQ(true, success);
    EXPECT_EQ(7U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(7U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(7U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(15U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_VerifyPdpContext(&success));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_VerifyPdpContext(&success));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_VerifyPdpContext(&success));

}

TEST_F(CellularControlNetwork, TestCellularSelectNetworkRadioAccessTechnologyFail)
{
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SelectRadioAccessTechnology(NULL));

    CellularNetwork_RadioAccessTechnology_T rat =
    {
            (CellularNetwork_SelectedRadioAccessTechnology_T) 0,
            (CellularNetwork_PreferredRadioAccessTechnology_T) 0, false, false
    };
    uint32_t sel;

    for (sel = CELLULAR_NETWORK_SEL_RAT_LTE_SINGLEMODE;
            sel <= CELLULAR_NETWORK_SEL_RAT_UMTS_LTE_DUALMODE; sel++)
    {
        rat.Selection = (CellularNetwork_SelectedRadioAccessTechnology_T)sel;
        rat.Preference = CELLULAR_NETWORK_PREF_RAT_GSM;
        EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SelectRadioAccessTechnology(&rat));
    }

    rat.Selection = CELLULAR_NETWORK_SEL_RAT_GSM_SINGLEMODE;
    rat.Preference = CELLULAR_NETWORK_PREF_RAT_LTE;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SelectRadioAccessTechnology(&rat));

    rat.Selection = (CellularNetwork_SelectedRadioAccessTechnology_T)sel;
    rat.Preference = CELLULAR_NETWORK_PREF_RAT_GSM;
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SelectRadioAccessTechnology(&rat));
}

TEST_F(CellularControlNetwork, TestCellularSelectNetworkRadioAccessTechnologyPass)
{
    CellularNetwork_RadioAccessTechnology_T rat =
    {
            (CellularNetwork_SelectedRadioAccessTechnology_T) 0,
            (CellularNetwork_PreferredRadioAccessTechnology_T) 0, false, false
    };
    uint32_t sel;
    uint32_t call_count = 0;

    for (sel = CELLULAR_NETWORK_SEL_RAT_GSM_SINGLEMODE;
            sel < CELLULAR_NETWORK_SEL_RAT_LTE_SINGLEMODE; sel++)
    {
        rat.Selection = (CellularNetwork_SelectedRadioAccessTechnology_T)sel;
        rat.Preference = CELLULAR_NETWORK_PREF_RAT_GSM;
        EXPECT_EQ(RETCODE_OK, CellularNetwork_SelectRadioAccessTechnology(&rat));
        ++call_count;

        EXPECT_EQ(call_count, CellularEngine_SendAtCommand_fake.call_count);
        EXPECT_EQ(call_count, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
        EXPECT_EQ(call_count, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

        rat.Preference = CELLULAR_NETWORK_PREF_RAT_UMTS;
        EXPECT_EQ(RETCODE_OK, CellularNetwork_SelectRadioAccessTechnology(&rat));
        ++call_count;

        EXPECT_EQ(call_count, CellularEngine_SendAtCommand_fake.call_count);
        EXPECT_EQ(call_count, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
        EXPECT_EQ(call_count, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
    }
}

TEST_F(CellularControlNetwork, TestConvertToRatSel) //Assuming this function is not intended to be static
{
    const char *data = "7";
    CellularNetwork_SelectedRadioAccessTechnology_T sel;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatSel(NULL,0,&sel));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatSel((uint8_t*)data,0,&sel));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatSel((uint8_t*)data,1,NULL));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatSel((uint8_t*)data,1,&sel));

    data = "6";
    EXPECT_EQ(RETCODE_OK, ConvertToRatSel((uint8_t*)data,1,&sel));
}

TEST_F(CellularControlNetwork, TestConvertToRatPref) //Assuming this function is not intended to be static
{
    const char *data = "7";
    CellularNetwork_PreferredRadioAccessTechnology_T sel;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatPref(NULL,0,&sel));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatPref((uint8_t*)data,0,&sel));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatPref((uint8_t*)data,1,NULL));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatPref((uint8_t*)data,1,&sel));

    data = "1";
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToRatPref((uint8_t*)data,1,&sel));
    data = "2";
    EXPECT_EQ(RETCODE_OK, ConvertToRatPref((uint8_t*)data,1,&sel));
}

TEST_F(CellularControlNetwork, TestCellularQueryNetworkRadioAccessTechnology)
{
    CellularNetwork_RadioAccessTechnology_T rat =
    {
            (CellularNetwork_SelectedRadioAccessTechnology_T) 0,
            (CellularNetwork_PreferredRadioAccessTechnology_T) 0, false, false
    };

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryRadioAccessTechnology(&rat));
    CmdArgCustomResp = CmdArgCustomResp2+1;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryRadioAccessTechnology(&rat));
    CmdArgCustomResp = CmdArgCustomResp0+6;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRadioAccessTechnology(&rat));

    CmdArgCustomResp = CmdArgCustomResp0;

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryRadioAccessTechnology(NULL));

    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRadioAccessTechnology(&rat));
    EXPECT_EQ(4U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(7U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryRadioAccessTechnology(&rat));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryRadioAccessTechnology(&rat));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryRadioAccessTechnology(&rat));
}

TEST_F(CellularControlNetwork, TestCellularQueryCellEnvironment)
{
    Cellular_CellEnvironmentData_T data;
    AtResponseQueue_WaitForMiscContent_fake.custom_fake = WaitArbitraryCmdArgCustom;
    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCellEnvironment(&data));
    CmdArgCustomResp = CmdArgCustomResp4;

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCellEnvironment(NULL));
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryCellEnvironment(&data));
    EXPECT_EQ(2U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForMiscContent_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedResponseCode_fake.call_count);

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCellEnvironment(&data));
    CellularEngine_SendAtCmdWaitEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCellEnvironment(&data));

}

TEST_F(CellularControlNetwork, TestCellularNetworkUrcUUPSDA)
{
    CmdArgCustomResp = CmdArgCustomResp3;

    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_UUPSDA());
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularNetworkUrcUUPSDD)
{
    CmdArgCustomResp = CmdArgCustomResp1;

    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_UUPSDD());
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularQueryCurrentNetworkSignalQuality)
{
    CellularNetwork_SignalQuality_T SignalQuality;

    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
    CmdArgCustomResp = CmdArgCustomResp0+6;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
    CmdArgCustomResp = CmdArgCustomResp1;

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentSignalQuality(NULL));

    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
    EXPECT_EQ(4U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(7U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
}

static uint8_t SimCardCounter = 0;

static Retcode_T Cellular_IsSimCardInserted_custom(bool *SimCardInserted)
{
    *SimCardInserted = !(0 == SimCardCounter);
    SimCardCounter++;
    return (2 == SimCardCounter) ? (Retcode_T) RETCODE_FAILURE : RETCODE_OK;
}

TEST_F(CellularControlNetwork, TestCellularNetworkInit)
{
    Cellular_IsSimCardInserted_fake.custom_fake = Cellular_IsSimCardInserted_custom;
    EXPECT_EQ(RETCODE_CELLULAR_SIM_NOT_READY, CellularNetwork_Init());
    EXPECT_EQ(RETCODE_CELLULAR_SIM_NOT_READY, CellularNetwork_Init());
    EXPECT_EQ(RETCODE_OK, CellularNetwork_Init());
}
