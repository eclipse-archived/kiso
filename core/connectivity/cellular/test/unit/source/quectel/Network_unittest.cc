/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

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
        "1", "1", "1", "192.168.1.2", NULL
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
const char *CmdArgCustomResp6[] =
{
        "1", NULL, "5", "5", NULL, "5", "5", "5", NULL, "9", "9", "", NULL, "10", "10", "10", "10",
        NULL, "NULL", "0", "0", NULL
};
const char *CmdArgCustomResp7[] =
{
        "5", NULL, "5", "1", NULL, "5", "1", "4", NULL, "5", "1", "4", "5", NULL, "5", "5", "5",
        "1", "5", "0", NULL, "0", NULL
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
}

TEST_F(CellularControlNetwork, TestConvertToRadioAccessTechnologyFail)
{
    CellularNetwork_Rat_T value;
    const char data[] = " ";
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

    CmdArgCustomResp = CmdArgCustomResp0+3;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRegistrationStatus(&status));
    CmdArgCustomResp = CmdArgCustomResp0+4;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRegistrationStatus(&status));
    CmdArgCustomResp = CmdArgCustomResp0+5;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRegistrationStatus(&status));
    CmdArgCustomResp = CmdArgCustomResp0+6;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryRegistrationStatus(&status));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryRegistrationStatus(&status));

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryRegistrationStatus(&status));

    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryRegistrationStatus(&status));
}

TEST_F(CellularControlNetwork, TestCellularQueryNetworkRegistrationStatusPass)
{
    CellularNetwork_RegistrationStatus_T status;
    CmdArgCustomResp = CmdArgCustomResp0;

    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryRegistrationStatus(&status));
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(5U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularConfigureNetworkRegistrationInformation)
{
    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK,
            CellularNetwork_ConfigureRegistrationInformation(CELLULAR_NETWORK_REG_INFO_URC_LOC));
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularNetworkUrcCREG)
{
    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp0+3;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp0+4;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp0+5;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetworkUrc_CREG());
    CmdArgCustomResp = CmdArgCustomResp6;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp++;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp++;
    EXPECT_EQ(RETCODE_OK, CellularNetworkUrc_CREG());
    CmdArgCustomResp++;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetworkUrc_CREG());
    CmdArgCustomResp++;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetworkUrc_CREG());
    CmdArgCustomResp++;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetworkUrc_CREG());
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetworkUrc_CREG());
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularNetworkUrc_CREG());
    EXPECT_EQ(13U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(38U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    CmdArgCustomResp = CmdArgCustomResp0;
}

TEST_F(CellularControlNetwork, TestCellularDeregisterFromNetwork)
{
    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK, Cellular_DeregisterFromNetwork());
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularEnableAutomaticNetworkSelection)
{
    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_EnableAutomaticSelection());
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularQueryCurrentNetworkOperatorFail)
{
    CellularNetwork_CurrentOperator_T op =
    {
            (CellularNetwork_SelectionMode_T) 0, (CellularNetwork_FormatOperatorName_T) 0,
            (CellularNetwork_Rat_T) 0, false, false, false, false, ""
    };
    CmdArgCustomResp = CmdArgCustomResp0;

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentOperator(NULL));

    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp = CmdArgCustomResp6;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp++;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp++;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentOperator(&op));
    CmdArgCustomResp++;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentOperator(&op));

    CmdArgCustomResp = CmdArgCustomResp0;

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentOperator(&op));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
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
    EXPECT_NE(0, strncmp(op.Operator, "", strlen(op.Operator)));
    EXPECT_EQ(CellularEngine_SendAtCommand_fake.arg1_val, (uint32_t)strlen(cmd));

    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularSetPsdProfileFail)
{
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SetPsdProfile(NULL));
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
    profile.Protocol = CELLULAR_PSD_PROTOCOL_IPV4;
    profile.Apn = "apn";
    profile.Username = "username";
    profile.Password = "password";
    profile.Authentication = CELLULAR_PSD_AUTH_NONE;

    EXPECT_EQ(RETCODE_OK, CellularNetwork_SetPsdProfile(&profile));
}

TEST_F(CellularControlNetwork, TestCellularActivatePdpContext)
{
    CmdArgCustomResp = CmdArgCustomResp1;

    EXPECT_EQ(RETCODE_OK, CellularNetwork_ActivatePdpContext());
    EXPECT_EQ(2U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_ActivatePdpContext());
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_ActivatePdpContext());
}

TEST_F(CellularControlNetwork, TestCellularDeactivatePdpContext)
{
    EXPECT_EQ(RETCODE_OK, CellularNetwork_DeactivatePdpContext());
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularQueryCurrentNetworkSignalQuality)
{
    CellularNetwork_SignalQuality_T SignalQuality;

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentSignalQuality(NULL));

    CmdArgCustomResp = CmdArgCustomResp0;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_QueryCurrentSignalQuality(&SignalQuality));
    EXPECT_EQ(1U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestGetNetworkAssignedData)
{
    CellularNetwork_AssignedParameterQuery_T req;
    req.ContextId = 5;
    req.ContextState = 0;
    req.ContextType = 0;
    req.IpAddress = 0;
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_OK;
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_OK;

    CmdArgCustomResp = CmdArgCustomResp7;
    EXPECT_EQ(RETCODE_INVALID_PARAM, GetNetworkAssignedData(&req));
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_INVALID_PARAM, GetNetworkAssignedData(&req));
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_INVALID_PARAM, GetNetworkAssignedData(&req));
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_INVALID_PARAM, GetNetworkAssignedData(&req));
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_INVALID_PARAM, GetNetworkAssignedData(&req));
    EXPECT_EQ(RETCODE_INVALID_PARAM, GetNetworkAssignedData(&req));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, GetNetworkAssignedData(&req));
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, GetNetworkAssignedData(&req));
    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, GetNetworkAssignedData(&req));
    CmdArgCustomResp = CmdArgCustomResp1;
    req.ContextId = 1;
    EXPECT_EQ(RETCODE_OK, GetNetworkAssignedData(&req));
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, GetNetworkAssignedData(&req));

    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, GetNetworkAssignedData(&req));

    EXPECT_EQ(12U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(12U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(11U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(25U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(9U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularControlNetwork, TestCellularSelectNetworkRadioAccessTechnology)
{

    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SelectRadioAccessTechnology(NULL));

    CellularNetwork_RadioAccessTechnology_T p;
    p.Selection = (CellularNetwork_ParameterScanMode_T) 99999999;
    p.Preference = (CellularNetwork_ParameterScanSequence_T) 0;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SelectRadioAccessTechnology(&p));
    p.Selection = (CellularNetwork_ParameterScanMode_T) 0;
    p.Preference = (CellularNetwork_ParameterScanSequence_T) 99999999;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_SelectRadioAccessTechnology(&p));
    p.Preference = (CellularNetwork_ParameterScanSequence_T) 0;
    EXPECT_EQ(RETCODE_OK, CellularNetwork_SelectRadioAccessTechnology(&p));

}

TEST_F(CellularControlNetwork, TestConvertToNetworkQuality)
{
    CellularNetwork_Quality_T mnq = CELLULAR_NETWORK_QUALITY_0;
    uint8_t data[2];
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNetworkQuality(NULL,0,&mnq));
    data[0] = 0x33;
    data[1] = 0x33;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNetworkQuality(data,3,&mnq));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNetworkQuality(data,sizeof(data),&mnq));
    data[0] = 0x39;
    data[1] = 0x39;
    EXPECT_EQ(RETCODE_OK, ConvertToNetworkQuality(data,sizeof(data),&mnq));
}

TEST_F(CellularControlNetwork, TestConvertToNetworkRssi)
{
    CellularNetwork_Rssi_T mnr = CELLULAR_NETWORK_RSSI_51_DBM;

    uint8_t data[2];
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNetworkRssi(NULL,0,NULL));
    data[0] = 0x33;
    data[1] = 0x33;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNetworkRssi(data,3,&mnr));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, ConvertToNetworkRssi(data,sizeof(data),&mnr));
    data[0] = 0x39;
    data[1] = 0x39;
    EXPECT_EQ(RETCODE_OK, ConvertToNetworkRssi(data,2,&mnr));
}

TEST_F(CellularControlNetwork, TestCellularQueryNetworkSignalQuality)
{
    CellularNetwork_SignalQuality_T p =
    {
            CELLULAR_NETWORK_RSSI_51_DBM, CELLULAR_NETWORK_QUALITY_0, false, false
    };
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentSignalQuality(NULL));

    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentSignalQuality(&p));
    CmdArgCustomResp = CmdArgCustomResp6;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularNetwork_QueryCurrentSignalQuality(&p));

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentSignalQuality(&p));

    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularNetwork_QueryCurrentSignalQuality(&p));
}

static uint8_t SimCardCounter = 0;

static Retcode_T Cellular_IsSimCardInserted_custom(bool *SimCardInserted)
{
    *SimCardInserted = !(0 == SimCardCounter);
    SimCardCounter++;
    return (2 == SimCardCounter) ? (Retcode_T) RETCODE_INVALID_PARAM : RETCODE_OK;
}

TEST_F(CellularControlNetwork, TestCellularNetworkInit)
{
    Cellular_IsSimCardInserted_fake.custom_fake = Cellular_IsSimCardInserted_custom;
    EXPECT_EQ(RETCODE_CELLULAR_SIM_NOT_READY, CellularNetwork_Init());
    EXPECT_EQ(RETCODE_CELLULAR_SIM_NOT_READY, CellularNetwork_Init());
    EXPECT_EQ(RETCODE_OK, CellularNetwork_Init());
    SimCardCounter = 0;
}

