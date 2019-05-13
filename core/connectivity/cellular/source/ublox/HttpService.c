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

#include "BCDS_CellularHttpService.h"
#include "BCDS_Cellular.h"
#include "BCDS_CellularConfig.h"
#include "UBloxUtils.h"
#include "HttpService.h"

#include "Engine.h"
#include "AT_UBlox.h"

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"


#define CELLULARHTTP_RESULT_FILE "r.slt"
#define CELLULARHTTP_POST_FILE   "r.qst"


static CellularHttp_ResultCallback_T CellularHttp_resultCallback=NULL;

static Retcode_T HttpService_uhttp(void* param, uint32_t length);
static Retcode_T HttpService_uhttpc(void* param, uint32_t length);
static Retcode_T HttpService_ulstFile(void*param, uint32_t length);


static Retcode_T HttpService_udwnFile(void*param, uint32_t length);
static Retcode_T HttpService_udelFile(void*param, uint32_t length);
static Retcode_T HttpService_urdBlock(void*param, uint32_t length);

static Retcode_T HttpService_startHttpRequest(CellularHttp_Request_T * httpRequest);
static Retcode_T HttpService_setupHttp(CellularHttp_Request_T * httpRequest);

static Retcode_T HttpService_WritePostData(CellularHttp_Data_T * postData);
static CellularHttp_Method_T HttpService_UbloxCommandToHttpMethod(AT_UHTTPC_Command_T command);
static CellularHttp_Result_T HttpService_UbloxResultToHttpResult(AT_UHTTPC_Result_T result);


void HttpService_NotifyResult(AT_UHTTP_ProfileId_T profileId, AT_UHTTPC_Command_T command, AT_UHTTPC_Result_T result)
{
	BCDS_UNUSED(profileId);

	if (CellularHttp_resultCallback!=NULL)
	{
		CellularHttp_resultCallback(HttpService_UbloxCommandToHttpMethod(command), HttpService_UbloxResultToHttpResult(result));
	}

}

Retcode_T CellularHttp_Initialize(CellularHttp_ResultCallback_T callback)
{

	Retcode_T retcode = RETCODE_OK;
	if (NULL!=callback)
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

Retcode_T CellularHttp_SendRequest(CellularHttp_Request_T * httpRequest)
{
	Retcode_T retcode = RETCODE_OK;

	if (httpRequest != NULL)
	{
		if (httpRequest->Method == CELLULARHTTP_METHOD_POST)
		{
			retcode =  HttpService_WritePostData(httpRequest->Data);
		}
		retcode = HttpService_setupHttp(httpRequest);
		if (RETCODE_OK == retcode)
		{
			retcode = HttpService_startHttpRequest(httpRequest);
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
	AT_ULSTFILE_Param_T  ulstFileParam;
	if (size!=NULL)
	{

		ulstFileParam.filename = CELLULARHTTP_RESULT_FILE;
		ulstFileParam.opcode = AT_ULSTFILE_OPCODE_SIZE;

		retcode = Engine_Dispatch(HttpService_ulstFile,1000,  &ulstFileParam, 0);
		if (RETCODE_OK == retcode)
		{
			*size = ulstFileParam.filesize;
		}
	}
	else
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
	}
	return retcode;
}


Retcode_T CellularHttp_GetResponse(CellularHttp_Data_T * response)
{
	Retcode_T retcode=RETCODE_OK;
	AT_URDBLOCK_Param_T  urdBlockParam;

	if (NULL != response)
	{
		urdBlockParam.filename = CELLULARHTTP_RESULT_FILE;
		urdBlockParam.buffer=(uint8_t*)response->Buffer;
		urdBlockParam.offset = 0;
		urdBlockParam.length = response->BufferLength;
		retcode = Engine_Dispatch(HttpService_urdBlock,1000,  &urdBlockParam, 0);
	}
	else
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
	}
	return retcode;

}

static Retcode_T HttpService_uhttp(void* param, uint32_t length)
{
	BCDS_UNUSED(length);

	assert(param!=NULL);
	Retcode_T retcode=RETCODE_OK;

	AT_UHTTP_Param_T * atUhttpParam = (AT_UHTTP_Param_T*)param;

	retcode = At_Set_UHTTP(atUhttpParam);

	return retcode;

}



static Retcode_T HttpService_uhttpc(void* param, uint32_t length)
{
	BCDS_UNUSED(length);

	assert(param!=NULL);
	Retcode_T retcode=RETCODE_OK;

	AT_UHTTPC_Param_T * atUhttpcParam = (AT_UHTTPC_Param_T*)param;

	retcode = At_Set_UHTTPC(atUhttpcParam);

	return retcode;

}


Retcode_T HttpService_ulstFile(void*param, uint32_t length)
{
	BCDS_UNUSED(length);
	assert(param!=NULL);
	Retcode_T retcode=RETCODE_OK;

	AT_ULSTFILE_Param_T * atUlstfileParam = (AT_ULSTFILE_Param_T*)param;

	retcode = At_Set_ULSTFILE(atUlstfileParam);

	return retcode;

}

static Retcode_T HttpService_udwnFile(void*param, uint32_t length)
{
	BCDS_UNUSED(length);
	assert(param!=NULL);
	Retcode_T retcode=RETCODE_OK;

	AT_UDWNFILE_Param_T * atUDwnfileParam = (AT_UDWNFILE_Param_T*)param;

	retcode = At_Set_UDWNFILE(atUDwnfileParam);

	return retcode;

}

static Retcode_T HttpService_udelFile(void*param, uint32_t length)
{
	BCDS_UNUSED(length);
	assert(param!=NULL);
	Retcode_T retcode=RETCODE_OK;

	AT_UDELFILE_Param_T * atUDelfileParam = (AT_UDELFILE_Param_T*)param;

	retcode = At_Set_UDELFILE(atUDelfileParam);

	return retcode;

}


static Retcode_T HttpService_urdBlock(void*param, uint32_t length)
{
	BCDS_UNUSED(length);
	assert(param!=NULL);
	Retcode_T retcode=RETCODE_OK;

	AT_URDBLOCK_Param_T * atUrdBlockParam = (AT_URDBLOCK_Param_T*)param;

	retcode = At_Set_URDBLOCK(atUrdBlockParam);

	return retcode;

}


static AT_UHTTPC_Command_T HttpService_HttpMethodToUbloxCommand(CellularHttp_Method_T method)
{
	AT_UHTTPC_Command_T command;
	switch (method)
	{
		case CELLULARHTTP_METHOD_GET:
			command = AT_UHTTPC_COMMAND_GET;
	        break;
		case CELLULARHTTP_METHOD_POST:
			command = AT_UHTTPC_COMMAND_POST_FILE;
			break;
		case CELLULARHTTP_METHOD_HEAD:
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
			method = CELLULARHTTP_METHOD_GET;
	        break;
		case AT_UHTTPC_COMMAND_POST_FILE:
			method = CELLULARHTTP_METHOD_POST;
			break;
		case AT_UHTTPC_COMMAND_HEAD:
			method = CELLULARHTTP_METHOD_HEAD;
			break;
		default:
			method = CELLULARHTTP_METHOD_INVALID;
			break;
	}
	return method;
}

static CellularHttp_Result_T HttpService_UbloxResultToHttpResult(AT_UHTTPC_Result_T httpResult)
{
	CellularHttp_Result_T result;
	switch(httpResult)
	{
		case AT_UHTTPC_RESULT_SUCCESS:
			result = CELLULARHTTP_RESULT_SUCCESS;
			break;
		default:
			result = CELLULARHTTP_RESULT_FAILURE;
			break;
	}
	return result;

}


static AT_UHTTPC_Content_T HttpService_HttpContentToUbloxContent(CellularHttp_ContentType_T contentType)
{
	AT_UHTTPC_Content_T content;
	content = (AT_UHTTPC_Content_T) contentType;
	return  content;

}

static Retcode_T HttpService_setupHttp(CellularHttp_Request_T * httpRequest)
{

	assert(httpRequest!=NULL);
	Retcode_T retcode;
	AT_UHTTP_Param_T uhttpParam;

	uhttpParam.ProfileId = AT_UHTTP_PROFILE_ID_0;
	uhttpParam.OpCode = AT_UHTTP_OPCODE_SERVER_NAME;
	uhttpParam.StringParam = httpRequest->Server;
	retcode = Engine_Dispatch(HttpService_uhttp, 1000, &uhttpParam, 0);

	uhttpParam.OpCode = AT_UHTTP_OPCODE_SECURE_OPTION;
	if (httpRequest->IsSecure)
	{
		uhttpParam.NumericParam = 1;
	}
	else
	{
		uhttpParam.NumericParam = 0;
	}
	retcode = Engine_Dispatch(HttpService_uhttp, 1000, &uhttpParam, 0);

	uhttpParam.OpCode = AT_UHTTP_OPCODE_SERVER_PORT;
	uhttpParam.NumericParam = httpRequest->Port;

	retcode = Engine_Dispatch(HttpService_uhttp, 1000, &uhttpParam, 0);
	return retcode;

}

static Retcode_T HttpService_startHttpRequest(CellularHttp_Request_T * httpRequest)
{

	assert(httpRequest!=NULL);
	Retcode_T retcode;
	AT_UHTTPC_Param_T uhttpcParam;

	uhttpcParam.ProfileId = AT_UHTTP_PROFILE_ID_0;
	uhttpcParam.Command = HttpService_HttpMethodToUbloxCommand(httpRequest->Method);
	uhttpcParam.ContentType = HttpService_HttpContentToUbloxContent(httpRequest->ContentType);
	uhttpcParam.ServerPath = httpRequest->Path;
	uhttpcParam.FileName = (uint8_t*) CELLULARHTTP_RESULT_FILE;
	uhttpcParam.data = (uint8_t*) CELLULARHTTP_POST_FILE;

	retcode = Engine_Dispatch(HttpService_uhttpc, 1000, &uhttpcParam, 0);

	return retcode;
}


static Retcode_T HttpService_WritePostData(CellularHttp_Data_T *postData)
{
	Retcode_T retcode = RETCODE_OK;
	AT_UDELFILE_Param_T  udelFileParam;
	AT_UDWNFILE_Param_T  udwnFileParam;

	if (postData!=NULL)
	{
		udelFileParam.filename = CELLULARHTTP_POST_FILE;
		udwnFileParam.filename = CELLULARHTTP_POST_FILE;
		retcode = Engine_Dispatch(HttpService_udelFile,1000,  &udelFileParam, 0);
		udwnFileParam.buffer=postData->Buffer;
		udwnFileParam.filesize = postData->BufferLength;
		retcode = Engine_Dispatch(HttpService_udwnFile,1000,  &udwnFileParam, 0);
	}
	else
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
	}

	return retcode;
}



