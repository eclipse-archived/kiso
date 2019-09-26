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

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_HTTP_SERVICE

#include "Kiso_CellularHttpService.h"
#include "Kiso_Cellular.h"
#include "Kiso_CellularConfig.h"
#include "UBloxUtils.h"
#include "HttpService.h"

#include "Engine.h"
#include "AT_UBlox.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

#define CELLULAR_HTTP_RESULT_FILE "r.slt"
#define CELLULAR_HTTP_POST_FILE "r.qst"

struct CellularHttp_GetResponseParam_S
{
    AT_URDBLOCK_Param_T param;
    AT_URDBLOCK_Resp_T resp;
};

static CellularHttp_ResultCallback_T CellularHttp_resultCallback = NULL;

static Retcode_T HttpService_uhttp(void *param, uint32_t length);
static Retcode_T HttpService_uhttpc(void *param, uint32_t length);
static Retcode_T HttpService_ulstFile(void *param, uint32_t length);

static Retcode_T HttpService_udwnFile(void *param, uint32_t length);
static Retcode_T HttpService_udelFile(void *param, uint32_t length);
static Retcode_T HttpService_urdBlock(void *param, uint32_t length);

static Retcode_T HttpService_StartHttpRequest(const CellularHttp_Request_T *httpRequest);
static Retcode_T HttpService_SetupHttp(const CellularHttp_Request_T *httpRequest);

static Retcode_T HttpService_WritePostData(const CellularHttp_Data_T *postData);
static CellularHttp_Method_T HttpService_UbloxCommandToHttpMethod(AT_UHTTPC_Command_T command);
static CellularHttp_Result_T HttpService_UbloxResultToHttpResult(AT_UHTTPC_Result_T result);

void HttpService_NotifyResult(AT_UHTTP_ProfileId_T profileId, AT_UHTTPC_Command_T command, AT_UHTTPC_Result_T result)
{
    KISO_UNUSED(profileId);

    if (CellularHttp_resultCallback != NULL)
    {
        CellularHttp_resultCallback(HttpService_UbloxCommandToHttpMethod(command), HttpService_UbloxResultToHttpResult(result));
    }
}

Retcode_T CellularHttp_Initialize(CellularHttp_ResultCallback_T callback)
{
    Retcode_T retcode = RETCODE_OK;
    if (NULL != callback)
    {
        CellularHttp_resultCallback = callback;
        retcode = RETCODE_OK;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    return retcode;
}

Retcode_T CellularHttp_SendRequest(const CellularHttp_Request_T *httpRequest)
{
    Retcode_T retcode = RETCODE_OK;

    if (httpRequest != NULL)
    {
        if (httpRequest->Method == CELLULAR_HTTP_METHOD_POST)
        {
            retcode = HttpService_WritePostData(httpRequest->Data);
        }
        retcode = HttpService_SetupHttp(httpRequest);
        if (RETCODE_OK == retcode)
        {
            retcode = HttpService_StartHttpRequest(httpRequest);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    return retcode;
}

Retcode_T CellularHttp_GetResponseSize(uint32_t *size)
{
    Retcode_T retcode;
    AT_ULSTFILE_Param_T ulstFileParam;
    if (size != NULL)
    {

        ulstFileParam.Filename = CELLULAR_HTTP_RESULT_FILE;
        ulstFileParam.Opcode = AT_ULSTFILE_OPCODE_SIZE;

        retcode = Engine_Dispatch(HttpService_ulstFile, 1000, &ulstFileParam, 0);
        if (RETCODE_OK == retcode)
        {
            *size = ulstFileParam.Filesize;
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    return retcode;
}

Retcode_T CellularHttp_GetResponse(CellularHttp_Data_T *response)
{
    Retcode_T retcode = RETCODE_OK;
    struct CellularHttp_GetResponseParam_S param;

    if (NULL != response)
    {
        param.param.Filename = CELLULAR_HTTP_RESULT_FILE;
        param.param.Offset = 0;
        param.param.Size = response->BufferSize;
        param.resp.Size = 0; /* will be overwritten by modem response */
        param.resp.Data = response->Buffer;
        retcode = Engine_Dispatch(HttpService_urdBlock, 1000, &param, sizeof(param));
        if (RETCODE_OK == retcode)
        {
            response->BufferLength = param.resp.Size;
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    return retcode;
}

static Retcode_T HttpService_uhttp(void *param, uint32_t length)
{
    KISO_UNUSED(length);

    assert(param != NULL);
    Retcode_T retcode = RETCODE_OK;

    AT_UHTTP_Param_T *atUhttpParam = (AT_UHTTP_Param_T *)param;

    retcode = At_Set_UHTTP(atUhttpParam);

    return retcode;
}

static Retcode_T HttpService_uhttpc(void *param, uint32_t length)
{
    KISO_UNUSED(length);

    assert(param != NULL);
    Retcode_T retcode = RETCODE_OK;

    AT_UHTTPC_Param_T *atUhttpcParam = (AT_UHTTPC_Param_T *)param;

    retcode = At_Set_UHTTPC(atUhttpcParam);

    return retcode;
}

Retcode_T HttpService_ulstFile(void *param, uint32_t length)
{
    KISO_UNUSED(length);
    assert(param != NULL);
    Retcode_T retcode = RETCODE_OK;

    AT_ULSTFILE_Param_T *atUlstfileParam = (AT_ULSTFILE_Param_T *)param;

    retcode = At_Set_ULSTFILE(atUlstfileParam);

    return retcode;
}

static Retcode_T HttpService_udwnFile(void *param, uint32_t length)
{
    KISO_UNUSED(length);
    assert(param != NULL);
    Retcode_T retcode = RETCODE_OK;

    AT_UDWNFILE_Param_T *atUDwnfileParam = (AT_UDWNFILE_Param_T *)param;

    retcode = At_Set_UDWNFILE(atUDwnfileParam);

    return retcode;
}

static Retcode_T HttpService_udelFile(void *param, uint32_t length)
{
    KISO_UNUSED(length);
    assert(param != NULL);
    Retcode_T retcode = RETCODE_OK;

    AT_UDELFILE_Param_T *atUDelfileParam = (AT_UDELFILE_Param_T *)param;

    retcode = At_Set_UDELFILE(atUDelfileParam);

    return retcode;
}

static Retcode_T HttpService_urdBlock(void *param, uint32_t length)
{
    KISO_UNUSED(length);
    assert(param != NULL);

    struct CellularHttp_GetResponseParam_S *getResponseParam = (struct CellularHttp_GetResponseParam_S *)param;

    return At_Set_URDBLOCK(&getResponseParam->param, &getResponseParam->resp);
}

static AT_UHTTPC_Command_T HttpService_HttpMethodToUbloxCommand(CellularHttp_Method_T method)
{
    AT_UHTTPC_Command_T command;
    switch (method)
    {
    case CELLULAR_HTTP_METHOD_GET:
        command = AT_UHTTPC_COMMAND_GET;
        break;
    case CELLULAR_HTTP_METHOD_POST:
        command = AT_UHTTPC_COMMAND_POST_FILE;
        break;
    case CELLULAR_HTTP_METHOD_HEAD:
        command = AT_UHTTPC_COMMAND_HEAD;
        break;
    default:
        command = AT_UHTTPC_COMMAND_INVALID;
        break;
    }
    return command;
}

static CellularHttp_Method_T HttpService_UbloxCommandToHttpMethod(AT_UHTTPC_Command_T command)
{
    CellularHttp_Method_T method;
    switch (command)
    {
    case AT_UHTTPC_COMMAND_GET:
        method = CELLULAR_HTTP_METHOD_GET;
        break;
    case AT_UHTTPC_COMMAND_POST_FILE:
        method = CELLULAR_HTTP_METHOD_POST;
        break;
    case AT_UHTTPC_COMMAND_HEAD:
        method = CELLULAR_HTTP_METHOD_HEAD;
        break;
    default:
        method = CELLULAR_HTTP_METHOD_INVALID;
        break;
    }
    return method;
}

static CellularHttp_Result_T HttpService_UbloxResultToHttpResult(AT_UHTTPC_Result_T httpResult)
{
    CellularHttp_Result_T result;
    switch (httpResult)
    {
    case AT_UHTTPC_RESULT_SUCCESS:
        result = CELLULAR_HTTP_RESULT_SUCCESS;
        break;
    default:
        result = CELLULAR_HTTP_RESULT_FAILURE;
        break;
    }
    return result;
}

static AT_UHTTPC_Content_T HttpService_HttpContentToUbloxContent(CellularHttp_ContentType_T contentType)
{
    AT_UHTTPC_Content_T content;
    content = (AT_UHTTPC_Content_T)contentType;
    return content;
}

static Retcode_T HttpService_SetupHttp(const CellularHttp_Request_T *httpRequest)
{
    assert(httpRequest != NULL);
    Retcode_T retcode;
    AT_UHTTP_Param_T uhttpParam;

    uhttpParam.ProfileId = AT_UHTTP_PROFILE_ID_0;
    uhttpParam.Opcode = AT_UHTTP_OPCODE_SERVER_NAME;
    uhttpParam.Value.String = httpRequest->Server;
    retcode = Engine_Dispatch(HttpService_uhttp, 1000, &uhttpParam, 0);

    uhttpParam.Opcode = AT_UHTTP_OPCODE_SECURE_OPTION;
    if (httpRequest->IsSecure)
    {
        uhttpParam.Value.Numeric = 1;
    }
    else
    {
        uhttpParam.Value.Numeric = 0;
    }
    retcode = Engine_Dispatch(HttpService_uhttp, 1000, &uhttpParam, 0);

    uhttpParam.Opcode = AT_UHTTP_OPCODE_SERVER_PORT;
    uhttpParam.Value.Numeric = httpRequest->Port;

    retcode = Engine_Dispatch(HttpService_uhttp, 1000, &uhttpParam, 0);
    return retcode;
}

static Retcode_T HttpService_StartHttpRequest(const CellularHttp_Request_T *httpRequest)
{
    assert(httpRequest != NULL);
    Retcode_T retcode;
    AT_UHTTPC_Param_T uhttpcParam;

    uhttpcParam.ProfileId = AT_UHTTP_PROFILE_ID_0;
    uhttpcParam.Command = HttpService_HttpMethodToUbloxCommand(httpRequest->Method);
    uhttpcParam.ContentType = HttpService_HttpContentToUbloxContent(httpRequest->ContentType);
    uhttpcParam.PathOnServer = httpRequest->Path;
    uhttpcParam.ResponseFilename = CELLULAR_HTTP_RESULT_FILE;
    uhttpcParam.Payload = CELLULAR_HTTP_POST_FILE;

    retcode = Engine_Dispatch(HttpService_uhttpc, 1000, &uhttpcParam, 0);

    return retcode;
}

static Retcode_T HttpService_WritePostData(const CellularHttp_Data_T *postData)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UDELFILE_Param_T udelFileParam;
    AT_UDWNFILE_Param_T udwnFileParam;

    if (postData != NULL)
    {
        udelFileParam.Filename = CELLULAR_HTTP_POST_FILE;
        udwnFileParam.Filename = CELLULAR_HTTP_POST_FILE;
        retcode = Engine_Dispatch(HttpService_udelFile, 1000, &udelFileParam, 0);
        udwnFileParam.Data = postData->Buffer;
        udwnFileParam.DataSize = postData->BufferLength;
        retcode = Engine_Dispatch(HttpService_udwnFile, 1000, &udwnFileParam, 0);
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    return retcode;
}
