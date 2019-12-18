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

/*###################### INCLUDED HEADERS ---------------------------------------------------------------------------*/

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_SOCKET_SERVICE

#include "SocketService.h"
#include "UBloxUtils.h"
#include "Kiso_CellularSocketService.h"
#include "Kiso_Cellular.h"
#include "Kiso_CellularConfig.h"
#include "Engine.h"
#include "AT_UBlox.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

#include <limits.h>

/*###################### MACROS DEFINITION ##########################################################################*/

#define CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT (UINT32_C(1000))

/*###################### LOCAL TYPES DEFINITION #####################################################################*/
struct CellularSocket_Context_S
{
    uint32_t Id;                                                    //< Modem handle associated with this socket.
    bool IsCreatedLocally;                                          //< Marks if this socket context locally allocated.
    bool IsCreatedOnModem;                                          //< Marks if this socket context has a valid modem representation.
    const Cellular_DataContext_T *DataContext;                      //< Data-context associated with this socket.
    CellularSocket_NotifySocketClosed_T OnSocketClosed;             //< Socket-Closed callback associated with this socket.
    CellularSocket_NotifyDataReady_T OnDataReady;                   //< Data-Ready callback associated with this socket.
    CellularSocket_NotifyConnectionAccepted_T OnConnectionAccepted; //< Connection-Accepted callback associated with this socket.
    CellularSocket_Protocol_T Protocol;                             //< Protocol associated with this socket.
    uint16_t LocalPort;                                             //< Local port associated with this socket. If set to zero a random port was selected.
};

struct CellularSocket_CreateAndBindParam_S
{
    CellularSocket_Handle_T *CreatedHandle;

    const Cellular_DataContext_T *DataContext;
    uint16_t LocalPort;
    CellularSocket_Protocol_T Protocol;
    CellularSocket_NotifySocketClosed_T OnSocketClosed;
    CellularSocket_NotifyDataReady_T OnDataReady;
};

struct CellularSocket_ConnectParam_S
{
    struct CellularSocket_Context_S *Context;

    const Cellular_IpAddress_T *RemoteIp;
    uint16_t RemotePort;
};

struct CellularSocket_ListenParam_S
{
    struct CellularSocket_Context_S *Context;

    uint16_t LocalPort;
    CellularSocket_NotifyConnectionAccepted_T OnConnectionAccepted;
};

struct CellularSocket_SendToParam_S
{
    const struct CellularSocket_Context_S *Context;

    const uint8_t *Data;
    uint32_t DataLength;
    const Cellular_IpAddress_T *RemoteIp;
    uint16_t RemotePort;
};

struct CellularSocket_ReceiveFromParam_S
{
    const struct CellularSocket_Context_S *Context;

    uint8_t *Buffer;
    uint32_t BufferLength;
    uint32_t *BytesReceivedOrAvailable;
    Cellular_IpAddress_T *RemoteIp;
    uint16_t *RemotePort;
};

/*###################### LOCAL FUNCTIONS DECLARATION ################################################################*/

static Retcode_T CreateAndBind(void *param, uint32_t len);
static Retcode_T Connect(void *param, uint32_t len);
static Retcode_T Send(void *param, uint32_t len);
static Retcode_T SendTo(void *param, uint32_t len);
static Retcode_T ReceiveFrom(void *param, uint32_t len);
static Retcode_T Receive(void *param, uint32_t len);
static Retcode_T Listen(void *param, uint32_t len);
static Retcode_T Close(void *param, uint32_t len);
static inline Retcode_T FindFreeSlot(uint32_t *freeSlot);
static inline Retcode_T FindSocketById(uint32_t socketId, struct CellularSocket_Context_S **ctx, CellularSocket_Handle_T *handle);
static inline bool IsValidLocally(CellularSocket_Handle_T socket);
static inline bool IsValidOnModem(CellularSocket_Handle_T socket);
static inline Retcode_T CelToUbloxProt(CellularSocket_Protocol_T from, AT_USOCR_Protocol_T *to);

/*###################### VARIABLES DECLARATION ######################################################################*/

static struct CellularSocket_Context_S Sockets[CELLULAR_SOCKET_COUNT];

/*###################### EXPOSED FUNCTIONS IMPLEMENTATION ###########################################################*/

Retcode_T CellularSocket_CreateAndBind(
    CellularSocket_Handle_T *socket,
    const Cellular_DataContext_T *dataContext,
    uint16_t localPort,
    CellularSocket_Protocol_T protocol,
    CellularSocket_NotifySocketClosed_T onSocketClosed,
    CellularSocket_NotifyDataReady_T onDataReady)
{
    if (NULL == socket || NULL == dataContext || NULL == onSocketClosed || NULL == onDataReady || CELLULAR_DATACONTEXTTYPE_INTERNAL != dataContext->Type)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_CreateAndBindParam_S param;
        param.CreatedHandle = socket;
        param.DataContext = dataContext;
        param.LocalPort = localPort;
        param.OnDataReady = onDataReady;
        param.OnSocketClosed = onSocketClosed;
        param.Protocol = protocol;

        return Engine_Dispatch(CreateAndBind, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T CellularSocket_Connect(
    CellularSocket_Handle_T socket,
    const Cellular_IpAddress_T *remoteIp,
    uint16_t remotePort)
{
    if (!IsValidLocally(socket) || NULL == remoteIp || CELLULAR_IPADDRESSTYPE_MAX <= remoteIp->Type)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_ConnectParam_S param;
        param.Context = &(Sockets[(uint32_t)socket]);
        param.RemoteIp = remoteIp;
        param.RemotePort = remotePort;
        return Engine_Dispatch(Connect, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T CellularSocket_Listen(
    CellularSocket_Handle_T socket,
    uint16_t localPort,
    CellularSocket_NotifyConnectionAccepted_T onConnectionAccept)
{
    if (!IsValidLocally(socket))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_ListenParam_S param;
        param.Context = &(Sockets[(uint32_t)socket]);
        param.LocalPort = localPort;
        param.OnConnectionAccepted = onConnectionAccept;
        return Engine_Dispatch(Listen, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T CellularSocket_Send(
    CellularSocket_Handle_T socket,
    const uint8_t *data,
    uint32_t dataLength)
{
    if (!IsValidLocally(socket) || !IsValidOnModem(socket) || NULL == data || 0 >= dataLength)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_SendToParam_S param;
        param.Context = &(Sockets[(uint32_t)socket]);
        param.Data = data;
        param.DataLength = dataLength;
        return Engine_Dispatch(Send, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T CellularSocket_SendTo(
    CellularSocket_Handle_T socket,
    const uint8_t *data,
    uint32_t dataLength,
    const Cellular_IpAddress_T *remoteIp,
    uint16_t remotePort)
{
    if (!IsValidLocally(socket) || !IsValidOnModem(socket) || NULL == data || 0 >= dataLength || NULL == remoteIp || CELLULAR_IPADDRESSTYPE_MAX <= remoteIp->Type)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_SendToParam_S param;
        param.Context = &(Sockets[(uint32_t)socket]);
        param.Data = data;
        param.DataLength = dataLength;
        param.RemoteIp = remoteIp;
        param.RemotePort = remotePort;
        return Engine_Dispatch(SendTo, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T CellularSocket_Receive(
    CellularSocket_Handle_T socket,
    uint8_t *buffer,
    uint32_t bufferLength,
    uint32_t *bytesReceived)
{
    if (!IsValidLocally(socket) || !IsValidOnModem(socket) || NULL == buffer || NULL == bytesReceived || 0U >= bufferLength)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_ReceiveFromParam_S param;
        param.Context = &(Sockets[(uint32_t)socket]);
        param.Buffer = buffer;
        param.BufferLength = bufferLength;
        param.BytesReceivedOrAvailable = bytesReceived;
        return Engine_Dispatch(Receive, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T CellularSocket_ReceiveFrom(
    CellularSocket_Handle_T socket,
    uint8_t *buffer,
    uint32_t bufferLength,
    uint32_t *bytesReceived,
    Cellular_IpAddress_T *remoteIp,
    uint16_t *remotePort)
{
    if (!IsValidLocally(socket) || !IsValidOnModem(socket) || NULL == buffer || NULL == bytesReceived || NULL == remoteIp || NULL == remotePort || 0U >= bufferLength)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_ReceiveFromParam_S param;
        param.Context = &(Sockets[(uint32_t)socket]);
        param.Buffer = buffer;
        param.BufferLength = bufferLength;
        param.BytesReceivedOrAvailable = bytesReceived;
        param.RemoteIp = remoteIp;
        param.RemotePort = remotePort;
        return Engine_Dispatch(ReceiveFrom, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T CellularSocket_QueryBytesAvailable(
    CellularSocket_Handle_T socket,
    uint32_t *numBytesAvailable)
{
    if (!IsValidLocally(socket) || !IsValidOnModem(socket) || NULL == numBytesAvailable)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_ReceiveFromParam_S param;
        param.Context = &(Sockets[(uint32_t)socket]);
        param.Buffer = NULL;
        param.BufferLength = 0;
        param.BytesReceivedOrAvailable = numBytesAvailable;
        return Engine_Dispatch(Receive, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T CellularSocket_Close(
    CellularSocket_Handle_T socket)
{
    if (!IsValidLocally(socket))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularSocket_Context_S *ctx = &(Sockets[(uint32_t)socket]);
        return Engine_Dispatch(Close, CELLULAR_SOCKET_SHORT_ENQUEUE_TIMEOUT, ctx, sizeof(struct CellularSocket_Context_S)); //LCOV_EXCL_BR_LINE
    }
}

void SocketService_NotifySocketAccepted(uint32_t createdSocketId,
                                        const AT_UBlox_Address_T *remoteIp,
                                        uint16_t remotePort,
                                        uint32_t listeningSocketId,
                                        const AT_UBlox_Address_T *localIp,
                                        uint16_t listeningPort)
{
    KISO_UNUSED(localIp);
    KISO_UNUSED(listeningPort);

    struct CellularSocket_Context_S *listeningCtx = NULL;
    struct CellularSocket_Context_S *createdCtx = NULL;
    CellularSocket_Handle_T listeningHandle;
    CellularSocket_Handle_T createdHandle;
    Cellular_IpAddress_T celRemoteIp;
    Retcode_T retcode = FindSocketById(listeningSocketId, &listeningCtx, &listeningHandle);

    if (RETCODE_OK == retcode)
    {
        retcode = FindFreeSlot((uint32_t *)&createdHandle);
    }

    if (RETCODE_OK == retcode)
    {
        createdCtx = &(Sockets[(uint32_t)createdHandle]);
        createdCtx->Id = createdSocketId;
        createdCtx->IsCreatedLocally = true;
        createdCtx->LocalPort = 0;
        createdCtx->Protocol = listeningCtx->Protocol;
        createdCtx->OnDataReady = listeningCtx->OnDataReady;
        createdCtx->OnSocketClosed = listeningCtx->OnSocketClosed;

        retcode = UbloxToCelAddr(remoteIp, &celRemoteIp); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        if (NULL != listeningCtx->OnConnectionAccepted)
        {
            listeningCtx->OnConnectionAccepted(
                createdHandle,
                &celRemoteIp,
                remotePort,
                listeningHandle);
        }
        else
        {
            /* We seem to have inconsistencies in our socket pool... very bad! */
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
        }
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode); //LCOV_EXCL_BR_LINE
    }
}

void SocketService_NotifySocketClosed(uint32_t socketId)
{
    struct CellularSocket_Context_S *ctx = NULL;
    CellularSocket_Handle_T handle;
    Retcode_T retcode = FindSocketById(socketId, &ctx, &handle);

    if (RETCODE_OK == retcode)
    {
        ctx->IsCreatedLocally = false;
        if (NULL != ctx->OnSocketClosed)
        {
            ctx->OnSocketClosed(handle);
        }
        else
        {
            /* We seem to have inconsistencies in our socket pool... very bad! */
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
        }
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode); //LCOV_EXCL_BR_LINE
    }
}

void SocketService_NotifySocketDataReceived(uint32_t socketId, uint32_t length)
{
    KISO_UNUSED(length);

    struct CellularSocket_Context_S *ctx = NULL;
    CellularSocket_Handle_T handle;
    Retcode_T retcode = FindSocketById(socketId, &ctx, &handle);

    if (RETCODE_OK == retcode)
    {
        if (NULL != ctx->OnDataReady)
        {
            ctx->OnDataReady(handle, length);
        }
        else
        {
            /* We seem to have inconsistencies in our socket pool... very bad! */
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
        }
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode); //LCOV_EXCL_BR_LINE
    }
}
/*###################### LOCAL FUNCTIONS IMPLEMENTATION #############################################################*/

/**
 * @brief           Creates and binds a new socket.
 *
 * @details         This function is preparing an internal socket context which is bound to a given address and can 
 *                  later be used in the @e listen() or @e connect() API. (To be encapsulated by via #Engine_Dispatch()).
 *
 * @param[in,out]   param A valid pointer to a #CellularSocket_CreateAndBindParam_S structure that contains the details
 *                  needed to create and bind the socket. On completion the @e CreatedHandle field will contain the 
 *                  newly created socket handle.
 *
 * @param           len Length of #CellularSocket_CreateAndBindParam_S structure.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T CreateAndBind(void *param, uint32_t len)
{
    KISO_UNUSED(len);

    assert(NULL != param);
    assert(sizeof(struct CellularSocket_CreateAndBindParam_S) == len);

    struct CellularSocket_CreateAndBindParam_S *crtParam = (struct CellularSocket_CreateAndBindParam_S *)param;
    struct CellularSocket_Context_S *ctx = NULL;
    AT_USOCR_Param_T usocr;
    Retcode_T retcode = FindFreeSlot((uint32_t *)crtParam->CreatedHandle);

    if (RETCODE_OK == retcode)
    {
        ctx = &(Sockets[(uint32_t)*crtParam->CreatedHandle]);
        ctx->IsCreatedLocally = true;
        ctx->DataContext = crtParam->DataContext;
        ctx->OnDataReady = crtParam->OnDataReady;
        ctx->OnSocketClosed = crtParam->OnSocketClosed;
        ctx->Protocol = crtParam->Protocol;
        ctx->LocalPort = crtParam->LocalPort;
    }

    if (RETCODE_OK == retcode)
    {
        usocr.LocalPort = ctx->LocalPort;
        retcode = CelToUbloxProt(ctx->Protocol, &usocr.Protocol);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = At_Set_USOCR(&usocr, &ctx->Id); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        ctx->IsCreatedOnModem = true;
    }

    /* On "generic" u-blox modems (like LISA-U2) it might be possible/necessary
     * to activate the PSD-profile associated with the provided data-context.
     * Refer to AT commands:
     *  AT+UPSD=<profile_id>,100,<cid_mapping>
     *      -> used to configure profile_id <-> cid association
     *
     *  AT+UPSDA=<profile_id>,3
     *      -> used to activate a PSD-profile and the underlying cid
     *
     * However these commands are not available on SARA-R4/N4 modems. SARA-R4/N4
     * modems only support a single PSD-profile which is mapped and activated
     * automatically.
     *
     * Also note that on "generic" u-blox modems it is discouraged to call
     * AT+CGACT for INTERNAL contexts. On SARA-R4/N4 however this is the only
     * way possible to activate a PDP context (on 2G networks).
     */

    return retcode;
}

/**
 * @brief           Connects a created socket via the modem-internal TCP/IP stack.
 * @details         This function will translate the internal socket context and allocate the socket resources on the 
 *                  modem-internal TCP/IP stack. (To be encapsulated by via #Engine_Dispatch()).
 *
 * @param[in]       param A valid pointer to a #CellularSocket_ConnectParam_S structure that contains details needed to 
 *                  connect the socket to a remote host.
 *
 * @param[in]       len Length of #CellularSocket_ConnectParam_S structure.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T Connect(void *param, uint32_t len)
{
    KISO_UNUSED(len);

    assert(NULL != param);
    assert(sizeof(struct CellularSocket_ConnectParam_S) == len);

    const struct CellularSocket_ConnectParam_S *connectParam = (struct CellularSocket_ConnectParam_S *)param;
    AT_USOCO_Param_T usoco;
    Retcode_T retcode = RETCODE_OK;

    usoco.Socket = connectParam->Context->Id;
    usoco.RemotePort = connectParam->RemotePort;
    retcode = CelToUbloxAddr(connectParam->RemoteIp, &usoco.RemoteAddr); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        retcode = At_Set_USOCO(&usoco); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

/**
 * @brief           Sets the given socket into listening mode. 
 * @details         Incoming connections will automatically be accepted and forwarded onto a new socket published via 
 *                  the connection-accepted callback. This function will translate the internal socket context and 
 *                  allocate the socket resources on the modem-internal TCP/IP stack. To be encapsulated by via 
 *                  #Engine_Dispatch().
 *
 * @param[in]       param A valid pointer to a #CellularSocket_ListenParam_S structure.
 *
 * @param[in]       len Length of #CellularSocket_ListenParam_S structure.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T Listen(void *param, uint32_t len)
{
    KISO_UNUSED(len);

    assert(NULL != param);
    assert(sizeof(struct CellularSocket_ListenParam_S) == len);

    struct CellularSocket_ListenParam_S *listenParam = (struct CellularSocket_ListenParam_S *)param;
    Retcode_T retcode;

    /* Listening on port 0 is not allowed by the u-blox stack. */
    assert(listenParam->LocalPort != 0);

    listenParam->Context->OnConnectionAccepted = listenParam->OnConnectionAccepted;
    listenParam->Context->LocalPort = listenParam->LocalPort;

    AT_USOLI_Param_T usoli;
    usoli.Socket = listenParam->Context->Id;
    usoli.Port = listenParam->LocalPort;
    retcode = At_Set_USOLI(&usoli); //LCOV_EXCL_BR_LINE

    return retcode;
}

/**
 * @brief           Send data over a socket using the modem-internal TCP/IP stack.
 *                  (To be encapsulated by via #Engine_Dispatch()).
 *
 * @note            This also works with UDP sockets provided the socket got 'connected' before.
 *
 * @param[in]       param A valid pointer to a #CellularSocket_SendToParam_S structure that contains
 *                  details needed to send data over the given socket.  RemoteIp and RemotePort
 *                  fields are ignored.
 *
 * @param[in]       len Length of #CellularSocket_SendToParam_S structure.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T Send(void *param, uint32_t len)
{
    KISO_UNUSED(len);

    assert(NULL != param);
    assert(sizeof(struct CellularSocket_SendToParam_S) == len);

    const struct CellularSocket_SendToParam_S *sendToParam = (struct CellularSocket_SendToParam_S *)param;
    AT_USOWR_Param_T usowrParam;
    usowrParam.Socket = sendToParam->Context->Id;
    usowrParam.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;
    usowrParam.Data = sendToParam->Data;
    usowrParam.Length = sendToParam->DataLength;

    AT_USOWR_Resp_T usowrResp;
    return At_Set_USOWR(&usowrParam, &usowrResp); //LCOV_EXCL_BR_LINE
}

/**
 * @brief           Send data over a UDP socket using the modem-internal TCP/IP stack. To be encapsulated 
 *                  via #Engine_Dispatch().
 *
 * @note            Can only be used for UDP sockets!
 *
 * @param[in]       param A valid pointer to a #CellularSocket_SendToParam_S structure that contains
 *                  details needed to send data over the given socket.
 *
 * @param[in]       len Length of #CellularSocket_SendToParam_S structure.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T SendTo(void *param, uint32_t len)
{
    KISO_UNUSED(len);

    assert(NULL != param);
    assert(sizeof(struct CellularSocket_SendToParam_S) == len);

    const struct CellularSocket_SendToParam_S *sendToParam = (struct CellularSocket_SendToParam_S *)param;
    AT_USOST_Param_T usostParam;
    AT_USOST_Resp_T usostResp;

    usostParam.Socket = sendToParam->Context->Id;
    usostParam.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;
    usostParam.Data = sendToParam->Data;
    usostParam.Length = sendToParam->DataLength;
    usostParam.RemotePort = sendToParam->RemotePort;
    Retcode_T retcode = CelToUbloxAddr(sendToParam->RemoteIp, &usostParam.RemoteIp); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        retcode = At_Set_USOST(&usostParam, &usostResp); //LCOV_EXCL_BR_LINE
    }
    return retcode;
}

/**
 * @brief           Receive data from a socket on the modem-internal TCP/IP stack.
 *
 *                  (To be encapsulated by via #Engine_Dispatch()).
 *
 * @note            Can be used for UDP sockets as well, however it is recommended to use
 *                  the ReceiveFrom variant in order to obtain the remote IP and port.
 *
 * @param[in,out]   param A valid pointer to a #CellularSocket_ReceiveFromParam_S structure. Data will
 *                  be received from the declared socket and stored stored inside the provide
 *                  data array. RemoteIp and RemotePort fields are ignored.
 *
 * @param[in]       len Length of #CellularSocket_ReceiveFromParam_S structure.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T Receive(void *param, uint32_t len)
{
    KISO_UNUSED(len);

    assert(NULL != param);
    assert(sizeof(struct CellularSocket_ReceiveFromParam_S) == len);

    struct CellularSocket_ReceiveFromParam_S *recvParam = (struct CellularSocket_ReceiveFromParam_S *)param;

    AT_USORD_Param_T usordParam;
    usordParam.Socket = recvParam->Context->Id;
    usordParam.Length = recvParam->BufferLength;
    usordParam.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;
    AT_USORD_Resp_T usordResp;
    usordResp.Data = recvParam->Buffer;
    Retcode_T retcode = At_Set_USORD(&usordParam, &usordResp); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        *recvParam->BytesReceivedOrAvailable = usordResp.Length;
    }
    return retcode;
}

/** 
 * @brief           Receive data from a UDP socket on the modem-internal TCP/IP stack.
 *
 *                  (To be encapsulated by via #Engine_Dispatch()).
 *
 * @note            Can only be used for UDP sockets!
 *
 * @param[in,out]   param A valid pointer to a #CellularSocket_ReceiveFromParam_S structure. Data will
 *                  be received from the declared socket and stored inside the provided data array.
 *
 * @param[in]       len Length of #CellularSocket_ReceiveFromParam_S structure.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T ReceiveFrom(void *param, uint32_t len)
{
    KISO_UNUSED(len);

    assert(NULL != param);
    assert(sizeof(struct CellularSocket_ReceiveFromParam_S) == len);

    struct CellularSocket_ReceiveFromParam_S *recvParam = (struct CellularSocket_ReceiveFromParam_S *)param;

    AT_USORF_Param_T usorfParam;
    usorfParam.Socket = recvParam->Context->Id;
    usorfParam.Length = recvParam->BufferLength;
    usorfParam.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;
    AT_USORF_Resp_T usorfResp;
    usorfResp.Data = recvParam->Buffer;
    Retcode_T retcode = At_Set_USORF(&usorfParam, &usorfResp); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        *recvParam->BytesReceivedOrAvailable = usorfResp.Length;
        if (0 < usorfParam.Length)
        {
            *recvParam->RemotePort = usorfResp.RemotePort;
            retcode = UbloxToCelAddr(&usorfResp.RemoteIp, recvParam->RemoteIp); //LCOV_EXCL_BR_LINE
        }
        else
        {
            *recvParam->RemotePort = 0;
            recvParam->RemoteIp->Type = CELLULAR_IPADDRESSTYPE_MAX;
            memset(recvParam->RemoteIp->Address.IPv6, 0, sizeof(recvParam->RemoteIp->Address.IPv6));
        }
    }
    return retcode;
}

/**
 * @brief           Close a socket on the modem-internal TCP/IP stack. (To be encapsulated by via #Engine_Dispatch()).
 *
 * @param           param A valid pointer to a #CellularSocket_Context_S structure that contains the
 *                  details of the to-be-closed socket. After successful completion the IsValid
 *                  field will be unset.
 *
 * @param           len Length of #CellularSocket_Context_S structure.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T Close(void *param, uint32_t len)
{
    KISO_UNUSED(len);

    assert(NULL != param);
    assert(sizeof(struct CellularSocket_Context_S) == len);

    struct CellularSocket_Context_S *ctx = (struct CellularSocket_Context_S *)param;
    AT_USOCL_Param_T usocl;
    usocl.Socket = ctx->Id;
    Retcode_T retcode = At_Set_USOCL(&usocl); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        ctx->IsCreatedLocally = false;
        ctx->IsCreatedOnModem = false;
        ctx->Id = 0;
        ctx->LocalPort = 0;
        ctx->OnConnectionAccepted = NULL;
        ctx->OnDataReady = NULL;
        ctx->OnSocketClosed = NULL;
    }

    return retcode;
}

/**
 * @brief           Find a free socket context in the #Sockets pool by verifying the @e IsValid flag.
 *
 * @param[out]      freeSlot Will be set to the index of the free socket context inside the #Sockets pool.
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static inline Retcode_T FindFreeSlot(uint32_t *freeSlot)
{
    assert(NULL != freeSlot);
    for (uint32_t i = 0; i < CELLULAR_SOCKET_COUNT; ++i)
    {
        if (!Sockets[i].IsCreatedLocally)
        {
            *freeSlot = i;
            return RETCODE_OK;
        }
    }
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
}

/**
 * @brief           Finds a socket context in the #Sockets pool by verifying the socketId
 *
 * @return          A #Retcode_T indicating the result of the procedure.
 */
static inline Retcode_T FindSocketById(uint32_t socketId, struct CellularSocket_Context_S **ctx, CellularSocket_Handle_T *handle)
{
    for (uint32_t i = 0; i < CELLULAR_SOCKET_COUNT; ++i)
    {
        if (Sockets[i].IsCreatedLocally && Sockets[i].Id == socketId)
        {
            *ctx = &(Sockets[i]);
            *handle = (CellularSocket_Handle_T)i;
            return RETCODE_OK;
            ;
        }
    }
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
}

/**
 * @brief           Verify if the provided socket handle is locally allocated.
 *
 * @param[in]       socket Handle of the driver socket context.
 *
 * @return          True if the socket handle is valid, false otherwise.
 */
static inline bool IsValidLocally(CellularSocket_Handle_T socket)
{
    return (CELLULAR_SOCKET_COUNT > (uint32_t)socket) && Sockets[(uint32_t)socket].IsCreatedLocally;
}

/**
 * @brief           Verify if the provided socket handle is allocated on the modem stack.
 *
 * @param[in]       socket Handle of the driver socket context.
 *
 * @return          True if the socket handle is valid, false otherwise.
 */
static inline bool IsValidOnModem(CellularSocket_Handle_T socket)
{
    return (CELLULAR_SOCKET_COUNT > (uint32_t)socket) && Sockets[(uint32_t)socket].IsCreatedOnModem;
}

/**
 * @brief           Translate #CellularSocket_Protocol_T to #AT_USOCR_Protocol_T.
 */
static inline Retcode_T CelToUbloxProt(CellularSocket_Protocol_T from, AT_USOCR_Protocol_T *to)
{
    switch (from)
    {
    case CELLULAR_SOCKET_PROTOCOL_TCP:
        *to = AT_USOCR_PROTOCOL_TCP;
        return RETCODE_OK;
    case CELLULAR_SOCKET_PROTOCOL_UDP:
        *to = AT_USOCR_PROTOCOL_UDP;
        return RETCODE_OK;
    default:
        *to = AT_USOCR_PROTOCOL_INVALID;
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }
}
