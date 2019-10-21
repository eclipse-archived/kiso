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
#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_HTTP_SERVICE

#include "UBloxUtils_th.hh"
#include "Engine_th.hh"
#include "AT_UBlox_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#undef KISO_MODULE_ID
#include "HttpService.c"
}

bool resultCallbackInvoked = false;

class TS_HttpService : public testing::Test
{
protected:
    virtual void SetUp()
    {
        RESET_FAKE(Engine_Dispatch);
        FFF_RESET_HISTORY();
        resultCallbackInvoked = false;
    }
};

static Retcode_T Engine_Dispatch_fakedfunc(CellularRequest_CallableFunction_T function, uint32_t timeout, void *parameter, uint32_t ParameterLength)
{
    KISO_UNUSED(timeout);

    Retcode_T retcode = function(parameter, ParameterLength);

    return retcode;
}

static void CellularHttp_ResultCallback(CellularHttp_Method_T method, CellularHttp_Result_T result)
{
    KISO_UNUSED(method);
    KISO_UNUSED(result);
    resultCallbackInvoked = true;
    std::cout << "     --- Hello from CellularHttp_ResultCallback ---" << std::endl;
}

TEST_F(TS_HttpService, CellularHttp_Initialize_fail)
{
    Retcode_T retcode;
    retcode = CellularHttp_Initialize(NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

TEST_F(TS_HttpService, CellularHttp_Initialize_success)
{
    Retcode_T retcode;
    retcode = CellularHttp_Initialize(CellularHttp_ResultCallback);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_HttpService, CellularHttp_SendRequest_Fail_null_pointer)
{
    Retcode_T retcode;
    retcode = CellularHttp_SendRequest(NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

TEST_F(TS_HttpService, CellularHttp_SendRequest_EngineDispatch_fail)
{
    Retcode_T retcode;
    CellularHttp_Request_T req;
    req.Method = CELLULAR_HTTP_METHOD_GET;
    req.Server = "testServer";
    req.Path = "/testPath";
    req.Port = 80;
    req.ContentType = CELLULAR_HTTP_CONTENTTYPE_TEXT_PLAIN;
    req.Data = NULL;
    req.IsSecure = false;

    Engine_Dispatch_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    retcode = CellularHttp_SendRequest(&req);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);
}

TEST_F(TS_HttpService, CellularHttp_SendRequest_Success_GET)
{
    Retcode_T retcode;
    CellularHttp_Request_T req;
    req.Method = CELLULAR_HTTP_METHOD_GET;
    req.Server = "testServer";
    req.Path = "/testPath";
    req.Port = 80;
    req.ContentType = CELLULAR_HTTP_CONTENTTYPE_TEXT_PLAIN;
    req.Data = NULL;
    req.IsSecure = false;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = CellularHttp_SendRequest(&req);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_HttpService, CellularHttp_SendRequest_Success_POST)
{
    Retcode_T retcode;
    uint8_t testBuffer[10] = {116, 101, 115, 116, 0, 0, 0, 0, 0, 0};
    CellularHttp_Data_S data;
    data.Buffer = testBuffer;
    data.BufferLength = 10;
    data.BufferSize = 10;

    CellularHttp_Request_T req;
    req.Method = CELLULAR_HTTP_METHOD_POST;
    req.Server = "testServer";
    req.Path = "/testPath";
    req.Port = 80;
    req.ContentType = CELLULAR_HTTP_CONTENTTYPE_TEXT_PLAIN;
    req.Data = &data;
    req.IsSecure = false;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = CellularHttp_SendRequest(&req);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_HttpService, CellularHttp_SendRequest_Success_HEAD)
{
    Retcode_T retcode;
    uint8_t testBuffer[10] = {116, 101, 115, 116, 0, 0, 0, 0, 0, 0};
    CellularHttp_Data_S data;
    data.Buffer = testBuffer;
    data.BufferLength = 10;
    data.BufferSize = 10;

    CellularHttp_Request_T req;
    req.Method = CELLULAR_HTTP_METHOD_HEAD;
    req.Server = "testServer";
    req.Path = "/testPath";
    req.Port = 80;
    req.ContentType = CELLULAR_HTTP_CONTENTTYPE_TEXT_PLAIN;
    req.Data = &data;
    req.IsSecure = false;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = CellularHttp_SendRequest(&req);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_HttpService, CellularHttp_SendRequest_Success_InvalidMethod)
{
    Retcode_T retcode;
    uint8_t testBuffer[10] = {116, 101, 115, 116, 0, 0, 0, 0, 0, 0};
    CellularHttp_Data_T data;
    data.Buffer = testBuffer;
    data.BufferLength = 10;
    data.BufferSize = 10;

    CellularHttp_Request_T req;
    req.Method = (enum CellularHttp_Method_E)10;
    req.Server = "testServer";
    req.Path = "/testPath";
    req.Port = 80;
    req.ContentType = CELLULAR_HTTP_CONTENTTYPE_TEXT_PLAIN;
    req.Data = &data;
    req.IsSecure = false;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = CellularHttp_SendRequest(&req);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_HttpService, CellularHttp_GetResponseSize_fail)
{
    Retcode_T retcode;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = CellularHttp_GetResponseSize(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

TEST_F(TS_HttpService, CellularHttp_GetResponseSize_fail_EngineDispatch_Fail)
{
    Retcode_T retcode;
    uint32_t size = 0;

    Engine_Dispatch_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    retcode = CellularHttp_GetResponseSize(&size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);
}

TEST_F(TS_HttpService, CellularHttp_GetResponseSize_Success)
{
    Retcode_T retcode;
    uint32_t size = 0;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    retcode = CellularHttp_GetResponseSize(&size);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_HttpService, CellularHttp_GetResponse_Fail)
{
    Retcode_T retcode;
    retcode = CellularHttp_GetResponse(NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
}

TEST_F(TS_HttpService, CellularHttp_GetResponse_Success)
{
    Retcode_T retcode;
    uint8_t testBuffer[10] = {116, 101, 115, 116, 0, 0, 0, 0, 0, 0};
    CellularHttp_Data_T data;
    data.Buffer = testBuffer;
    data.BufferLength = 10;
    data.BufferSize = 10;

    retcode = CellularHttp_GetResponse(&data);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_HttpService, HttpService_NotifyResult_NoCallback)
{
    AT_UHTTP_ProfileId_T profileId = AT_UHTTP_PROFILE_ID_0;
    AT_UHTTPC_Command_T command = AT_UHTTPC_COMMAND_POST_FILE;
    AT_UHTTPC_Result_T result = AT_UHTTPC_RESULT_SUCCESS;
    HttpService_NotifyResult(profileId, command, result);
    EXPECT_TRUE(resultCallbackInvoked);
}

TEST_F(TS_HttpService, HttpService_NotifyResult_HEAD_success)
{
    Retcode_T retcode;
    AT_UHTTP_ProfileId_T profileId = AT_UHTTP_PROFILE_ID_0;
    AT_UHTTPC_Command_T command = AT_UHTTPC_COMMAND_HEAD;
    AT_UHTTPC_Result_T result = AT_UHTTPC_RESULT_SUCCESS;

    retcode = CellularHttp_Initialize(CellularHttp_ResultCallback);

    EXPECT_EQ(RETCODE_OK, retcode);
    HttpService_NotifyResult(profileId, command, result);
    EXPECT_TRUE(resultCallbackInvoked);
}

TEST_F(TS_HttpService, HttpService_NotifyResult_GET_success)
{
    Retcode_T retcode;
    AT_UHTTP_ProfileId_T profileId = AT_UHTTP_PROFILE_ID_0;
    AT_UHTTPC_Command_T command = AT_UHTTPC_COMMAND_GET;
    AT_UHTTPC_Result_T result = AT_UHTTPC_RESULT_SUCCESS;

    retcode = CellularHttp_Initialize(CellularHttp_ResultCallback);

    EXPECT_EQ(RETCODE_OK, retcode);
    HttpService_NotifyResult(profileId, command, result);
    EXPECT_TRUE(resultCallbackInvoked);
}

TEST_F(TS_HttpService, HttpService_NotifyResult_POST_success)
{
    Retcode_T retcode;
    AT_UHTTP_ProfileId_T profileId = AT_UHTTP_PROFILE_ID_0;
    AT_UHTTPC_Command_T command = AT_UHTTPC_COMMAND_POST_FILE;
    AT_UHTTPC_Result_T result = AT_UHTTPC_RESULT_SUCCESS;

    retcode = CellularHttp_Initialize(CellularHttp_ResultCallback);

    EXPECT_EQ(RETCODE_OK, retcode);
    HttpService_NotifyResult(profileId, command, result);
    EXPECT_TRUE(resultCallbackInvoked);
}

TEST_F(TS_HttpService, HttpService_NotifyResult_POST_Invalid)
{
    Retcode_T retcode;
    AT_UHTTP_ProfileId_T profileId = AT_UHTTP_PROFILE_ID_0;
    AT_UHTTPC_Command_T command = AT_UHTTPC_COMMAND_POST_FILE;
    AT_UHTTPC_Result_T result = AT_UHTTPC_RESULT_SUCCESS;

    retcode = CellularHttp_Initialize(CellularHttp_ResultCallback);

    EXPECT_EQ(RETCODE_OK, retcode);
    HttpService_NotifyResult(profileId, command, result);
    EXPECT_TRUE(resultCallbackInvoked);
}
