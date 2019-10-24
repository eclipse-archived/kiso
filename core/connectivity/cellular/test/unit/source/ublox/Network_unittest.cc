/*******************************************************************************
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
 ******************************************************************************/

#include <gtest.h>

extern "C"
{

#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_NETWORK

#include "Engine_th.hh"
#include "At3Gpp27007_th.hh"
#include "AT_UBlox_th.hh"
#include "Power_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#include "Kiso_Logging_th.hh"
#undef LOG_DEBUG
#define LOG_DEBUG(...) \
    do                 \
    {                  \
    } while (0)

#include "Network.c"

#define TEST_ICCID "1234567891234567890"
}

class TS_Network : public testing::Test
{
protected:
    virtual void SetUp()
    {
        RESET_FAKE(Engine_Dispatch);
        RESET_FAKE(At_Set_CGACT);
        RESET_FAKE(At_Set_CGPADDR);
        RESET_FAKE(At_Get_CCID);
        RESET_FAKE(At_Set_UMNOPROF);
        FFF_RESET_HISTORY();
    }
};

static Retcode_T At_Get_CCID_fakedfunc1(AT_CCID_Resp_T *resp)
{
    char testIccid[AT_CCID_TYPE_20CHAR + 1] = TEST_ICCID;
    resp->Type = AT_CCID_TYPE_19CHAR;
    memcpy(resp->Iccid, testIccid, (size_t)resp->Type);
    return RETCODE_OK;
}
static Retcode_T Engine_Dispatch_fakedfunc(CellularRequest_CallableFunction_T function, uint32_t timeout, void *parameter, uint32_t ParameterLength)
{
    KISO_UNUSED(timeout);

    Retcode_T retcode = function(parameter, ParameterLength);

    return retcode;
}
/*######################################################################################################################
 * Testing Cellular_ConfigureDataContext()
######################################################################################################################*/
TEST_F(TS_Network, Cellular_ConfigureDataContext_Success)
{
    Retcode_T retcode;
    Cellular_DataContextParameters_T ctxParam;

    ctxParam.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    ctxParam.ApnSettings.ApnName = "test.apn";
    ctxParam.ApnSettings.AuthMethod = CELLULAR_APNAUTHMETHOD_NONE;
    ctxParam.ApnSettings.Username = NULL;
    ctxParam.ApnSettings.Password = NULL;
    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_ConfigureDataContext(0, &ctxParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_ConfigureDataContext_Fail_InvalidContext)
{
    Retcode_T retcode;
    Cellular_DataContextParameters_T *invalidCctxParam = NULL;

    retcode = Cellular_ConfigureDataContext(0, invalidCctxParam);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_Network, Cellular_ConfigureDataContext_Fail_InvalidContextId)
{
    Retcode_T retcode;
    uint32_t invalidCid = 10;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_ConfigureDataContext(invalidCid, NULL);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_Network, Cellular_ConfigureDataContext_Fail_Dispatch)
{
    Retcode_T retcode;
    Cellular_DataContextParameters_T ctxParam;

    ctxParam.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    ctxParam.ApnSettings.ApnName = "test.apn";
    ctxParam.ApnSettings.AuthMethod = CELLULAR_APNAUTHMETHOD_NONE;
    ctxParam.ApnSettings.Username = NULL;
    ctxParam.ApnSettings.Password = NULL;

    Engine_Dispatch_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    retcode = Cellular_ConfigureDataContext(0, &ctxParam);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

/*######################################################################################################################
 * Testing Cellular_RegisterOnNetwork()
######################################################################################################################*/
TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_1)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = CELLULAR_RAT_DEFAULT;
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_2)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = CELLULAR_RAT_GSM;
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_3)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = CELLULAR_RAT_LTE;
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_4)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = CELLULAR_RAT_LTE_CAT_M1;
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_5)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = CELLULAR_RAT_LTE_CAT_NB1;
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_6)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = CELLULAR_RAT_UMTS;
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_7)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = (Cellular_RadioAccessTechnology_T)(CELLULAR_RAT_GSM | CELLULAR_RAT_UMTS);
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_8)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = (Cellular_RadioAccessTechnology_T)(CELLULAR_RAT_GSM | CELLULAR_RAT_UMTS | CELLULAR_RAT_LTE);
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_9)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = (Cellular_RadioAccessTechnology_T)(CELLULAR_RAT_GSM | CELLULAR_RAT_LTE);
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Success_10)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = (Cellular_RadioAccessTechnology_T)(CELLULAR_RAT_UMTS | CELLULAR_RAT_LTE);
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Fail_InalidRAT)
{
    Retcode_T retcode;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(NULL);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Fail_InalidParam)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = (Cellular_RadioAccessTechnology_T)(0xFFFFFFFF);
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(retcode));
}

TEST_F(TS_Network, Cellular_RegisterOnNetwork_Fail_At_Set_UMNOPROF)
{
    Retcode_T retcode;
    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = (Cellular_RadioAccessTechnology_T)(CELLULAR_RAT_LTE_CAT_M1);
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;
    At_Set_UMNOPROF_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_RegisterOnNetwork(&nwParam);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);
}

/*######################################################################################################################
 * Testing Cellular_ActivateDataContext()
######################################################################################################################*/

TEST_F(TS_Network, Cellular_ActivateDataContext_Success)
{
    static const Cellular_DataContext_T *DataContext;
    Retcode_T retcode;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_ActivateDataContext(0, &DataContext);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_ActivateDataContext_Fail_NULL_Context)
{
    Retcode_T retcode;
    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_ActivateDataContext(0, NULL);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_Network, Cellular_ActivateDataContext_Fail_InvalidCid)
{
    static const Cellular_DataContext_T *DataContext;
    Retcode_T retcode;
    uint32_t invalidCid = 10;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_ActivateDataContext(invalidCid, &DataContext);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

/*######################################################################################################################
 * Testing Cellular_DeactivateDataContext()
######################################################################################################################*/

TEST_F(TS_Network, Cellular_DeactivateDataContext_Success)
{
    static const Cellular_DataContext_T *DataContext;
    Retcode_T retcode;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    retcode = Cellular_ActivateDataContext(0, &DataContext);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = Cellular_DeactivateDataContext(0);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_Network, Cellular_DeactivateDataContext_Fail_InvalidCid)
{
    Retcode_T retcode;
    uint32_t invalidCid = 10;

    retcode = Cellular_DeactivateDataContext(invalidCid);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_Network, Cellular_DeactivateDataContext_Fail)
{
    static const Cellular_DataContext_T *DataContext;
    Retcode_T retcode;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_ActivateDataContext(0, &DataContext);

    EXPECT_EQ(RETCODE_OK, retcode);

    At_Set_CGACT_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    retcode = Cellular_DeactivateDataContext(0);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);
}

/*######################################################################################################################
 * Testing Cellular_QueryIccid()
######################################################################################################################*/

TEST_F(TS_Network, Cellular_QueryIccid_Success)
{
    Retcode_T retcode;
    char ccid[AT_CCID_TYPE_20CHAR + 1];
    uint32_t ccidLen = AT_CCID_TYPE_20CHAR;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    At_Get_CCID_fake.custom_fake = At_Get_CCID_fakedfunc1;

    retcode = Cellular_QueryIccid(ccid, &ccidLen);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_STREQ(ccid, TEST_ICCID);
}

TEST_F(TS_Network, Cellular_QueryIccid_Fail_invalid_buffer)
{
    Retcode_T retcode;
    uint32_t ccidLen = AT_CCID_TYPE_20CHAR;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    At_Get_CCID_fake.custom_fake = At_Get_CCID_fakedfunc1;

    retcode = Cellular_QueryIccid(NULL, &ccidLen);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_Network, Cellular_QueryIccid_Fail_invalid_bufferLen_Reference)
{
    Retcode_T retcode;
    char ccid[AT_CCID_TYPE_20CHAR + 1];
    // uint32_t ccidLen = AT_CCID_TYPE_20CHAR;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    At_Get_CCID_fake.custom_fake = At_Get_CCID_fakedfunc1;

    retcode = Cellular_QueryIccid(ccid, NULL);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_Network, Cellular_QueryIccid_Fail_invalid_bufferLen_InitValue)
{
    Retcode_T retcode;
    char ccid[AT_CCID_TYPE_20CHAR + 1];
    uint32_t ccidLen = AT_CCID_TYPE_INVALID;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    At_Get_CCID_fake.custom_fake = At_Get_CCID_fakedfunc1;

    retcode = Cellular_QueryIccid(ccid, &ccidLen);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

/*######################################################################################################################
 * Testing Cellular_QueryNetworkInfo()
######################################################################################################################*/

TEST_F(TS_Network, Cellular_QueryNetworkInfo_Success)
{
    Retcode_T retcode;
    Cellular_NetworkInfo_T networkInfo;

    retcode = Cellular_QueryNetworkInfo(&networkInfo);

    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(retcode));
}

/*######################################################################################################################
 * Testing Cellular_SetAirPlaneMode()
######################################################################################################################*/

TEST_F(TS_Network, Cellular_SetAirPlaneMode_Success)
{
    Retcode_T retcode;

    retcode = Cellular_SetAirPlaneMode(true);

    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(retcode));
}

/*######################################################################################################################
 * Testing Cellular_DisconnectFromNetwork()
######################################################################################################################*/
TEST_F(TS_Network, Cellular_DisconnectFromNetwork_Success)
{
    Retcode_T retcode;
    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = Cellular_DisconnectFromNetwork();

    EXPECT_EQ(RETCODE_OK, retcode);
}