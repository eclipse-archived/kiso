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

#ifndef BCDS_CELLULARHTTPSERVICE_H_
#define BCDS_CELLULARHTTPSERVICE_H_


#include "BCDS_Cellular.h"

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"


/**
 * @brief Cellular http methods
 */
enum CellularHttp_Method_E
{
	CELLULARHTTP_METHOD_HEAD=0,
	CELLULARHTTP_METHOD_GET,
	CELLULARHTTP_METHOD_POST,
	CELLULARHTTP_METHOD_INVALID=255
};
typedef enum CellularHttp_Method_E CellularHttp_Method_T;

/**
 * @brief Cellular http content types
 */
enum CellularHttp_ContentType_E
{
	CELLULARHTTP_CONTENTTYPE_APP_X_WWW_URL  = 0,  //!<    application/x-www-form-urlencoded
	CELLULARHTTP_CONTENTTYPE_TEXT_PLAIN     = 1,  //!<    text/plain
	CELLULARHTTP_CONTENTTYPE_APP_OCTET      = 2,  //!<    application/octet-stream
	CELLULARHTTP_CONTENTTYPE_MULTIPLART     = 3,  //!<    multipart/form-data
	CELLULARHTTP_CONTENTTYPE_APP_JSON       = 4,  //!<    application/json
	CELLULARHTTP_CONTENTTYPE_APP_XML        = 5,  //!<    application/xml
};
typedef enum CellularHttp_ContentType_E CellularHttp_ContentType_T;


/**
 * @brief Cellular http result
 */
enum CellularHttp_Result_E
{
	CELLULARHTTP_RESULT_SUCCESS,
	CELLULARHTTP_RESULT_FAILURE
};
typedef enum CellularHttp_Result_E CellularHttp_Result_T;

typedef void (*CellularHttp_ResultCallback_T)(CellularHttp_Method_T method, CellularHttp_Result_T result);

/**
 * @brief Cellular http request/response data structures
 */
struct CellularHttp_Data_S
{
	uint32_t BufferLength;
	void * Buffer;
};
typedef struct CellularHttp_Data_S CellularHttp_Data_T;


/**
 * @brief Cellular http request structure
 */
struct CellularHttp_Request_S
{
	CellularHttp_Method_T Method;
	const uint8_t * Server;
	const uint8_t * Path;
	uint16_t Port;
	bool IsSecure;
    CellularHttp_ContentType_T ContentType;
	CellularHttp_Data_T * Data;
};
typedef struct CellularHttp_Request_S CellularHttp_Request_T;


/**
 * @brief Initialize http callback to return http events
 *
 * @param[in] callback
 * callback to be triggered on event
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularHttp_Initialize(CellularHttp_ResultCallback_T callback);

/**
 * @brief Send Http a request
 *
 * @param[in] parameters for the send request
  *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularHttp_SendRequest(CellularHttp_Request_T * httpRequest);

/**
 * @brief Read the http response on successfull request
 *
 * @param[in] httpResponse the response data structure for the result
  *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularHttp_GetResponse(CellularHttp_Data_T * httpResponse);

/**
 * @brief Read the http response size successfull request
 *
 * @param[out] size read out the size of the request
  *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularHttp_GetResponseSize(uint32_t *size);



#endif /* BCDS_CELLULARHTTPSERVICE_H__ */
