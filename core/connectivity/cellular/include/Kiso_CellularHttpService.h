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

/**
 * @ingroup KISO_CELLULAR
 * @defgroup KISO_CELLULAR_HTTP HTTP
 * @{
 * @brief Cellular Hyper Text Transfer Service API.
 *
 * @file
 */
#ifndef KISO_CELLULARHTTPSERVICE_H_
#define KISO_CELLULARHTTPSERVICE_H_

#include "Kiso_Cellular.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/**
 * @brief Cellular http methods
 */
enum CellularHttp_Method_E
{
    CELLULAR_HTTP_METHOD_HEAD = 0,
    CELLULAR_HTTP_METHOD_GET,
    CELLULAR_HTTP_METHOD_POST,
    CELLULAR_HTTP_METHOD_INVALID = 255
};
typedef enum CellularHttp_Method_E CellularHttp_Method_T;

/**
 * @brief Cellular http content types
 */
enum CellularHttp_ContentType_E
{
    CELLULAR_HTTP_CONTENTTYPE_APP_X_WWW_URL = 0, //!< application/x-www-form-urlencoded
    CELLULAR_HTTP_CONTENTTYPE_TEXT_PLAIN = 1,    //!< text/plain
    CELLULAR_HTTP_CONTENTTYPE_APP_OCTET = 2,     //!< application/octet-stream
    CELLULAR_HTTP_CONTENTTYPE_MULTIPLART = 3,    //!< multipart/form-data
    CELLULAR_HTTP_CONTENTTYPE_APP_JSON = 4,      //!< application/json
    CELLULAR_HTTP_CONTENTTYPE_APP_XML = 5,       //!< application/xml
};
typedef enum CellularHttp_ContentType_E CellularHttp_ContentType_T;

/**
 * @brief Cellular http result
 */
enum CellularHttp_Result_E
{
    CELLULAR_HTTP_RESULT_SUCCESS,
    CELLULAR_HTTP_RESULT_FAILURE
};
typedef enum CellularHttp_Result_E CellularHttp_Result_T;

typedef void (*CellularHttp_ResultCallback_T)(CellularHttp_Method_T method, CellularHttp_Result_T result);

/**
 * @brief Cellular http request/response data structures
 */
struct CellularHttp_Data_S
{
    uint32_t BufferSize;   //!< the actual buffer size in bytes
    uint32_t BufferLength; //!< the current number of bytes stored in the buffer
    uint8_t *Buffer;       //!< the pointer to the buffer of size BufferSize
};
typedef struct CellularHttp_Data_S CellularHttp_Data_T;

/**
 * @brief Cellular http request structure
 */
struct CellularHttp_Request_S
{
    CellularHttp_Method_T Method;           //!< the http method (CellularHttp_Method_E) to be used
    const char *Server;                     //!< server IP address or name server
    const char *Path;                       //!< path of the resource to access
    uint16_t Port;                          //!< server port to be used
    bool IsSecure;                          //!< use a secure connection
    CellularHttp_ContentType_T ContentType; //!< content type specification (CellularHttp_ContentType_E)
    CellularHttp_Data_T *Data;              //!< cellular data structure (struct CellularHttp_Data_S)
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
 * @param[in] httpRequest Parameters for the send request
  *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularHttp_SendRequest(const CellularHttp_Request_T *httpRequest);

/**
 * @brief Read the http response on successfull request
 *
 * @param[in] httpResponse the response data structure for the result
  *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularHttp_GetResponse(CellularHttp_Data_T *httpResponse);

/**
 * @brief Read the http response size successfull request
 *
 * @param[out] size read out the size of the request
  *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularHttp_GetResponseSize(uint32_t *size);

#endif /* KISO_CELLULARHTTPSERVICE_H__ */

/** @} */
