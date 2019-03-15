/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @file
 *
 * @brief Provides functionality for control (open, close, etc.) and querying of a socket
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_SOCKET

#include "CellularModule.h"
#include "CellularBsp.h"
#include <stdio.h>
#include <inttypes.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "task.h"
#include "CellularCommon.h"
#include "CellularResponseQueue.h"
#include "CellularAtResponseParser.h"
#include "Control.h"
#include "CellularEngine.h"
#include "CellularUtils.h"
#include "Network.h"
#include "Command.h"
#include "BCDS_Logging.h"
#include "Socket.h"

/**
 * @brief Socket info parameters:
 *
 * @see  Ref 1, p 506, 25.25 Socket control +USOCTL
 */
enum CellularSocket_InfoParameter_E
{
    CELLULAR_SOCKET_INFO_TYPE = 0,
    CELLULAR_SOCKET_INFO_LAST_ERROR = 1,
    CELLULAR_SOCKET_INFO_BYTES_SENT = 2,
    CELLULAR_SOCKET_INFO_BYTES_RECEIVED = 3,
    CELLULAR_SOCKET_INFO_REMOTE_IP_PORT = 4,

    CELLULAR_SOCKET_INFO_STATUS = 10,
    CELLULAR_SOCKET_INFO_UNACKNOWLEGED_DATA = 11,

    CELLULAR_SOCKET_INFO_MAX
};
typedef enum CellularSocket_InfoParameter_E CellularSocket_InfoParameter_T;

#define CELLULAR_SOCKET_RECEIVE_FROM_LENGTH_MAX    (UINT32_C(1500))
#define CELLULAR_SOCKET_PORT_MAX                   (UINT32_C(65535))

/**
 * @brief The internal socket typed needed for receiving a data
 */
typedef struct
{
    uint32_t Ip;
    uint16_t Port;
    uint32_t BytesAvailable;
    uint32_t Length;
    uint8_t Buffer[CELLULAR_SOCKET_RECEIVE_FROM_LENGTH_MAX];
} CellularSocket_Data_T;

/**
 * @brief The internal representation of a socket.
 */
struct CellularSocket_S
{
    /**
     * @brief The socket ID that is used to identify the socket on the modem.
     */
    CellularUdpSocket_T SocketId;

    /**
     * @brief This callback will be used to notify socket-state changes.
     *
     * @warning If CELLULAR_SOCKET_STATUS_CLOSED is passed to this callback, the application may use the socket handle
     * inside the callback to clean up some resources, however the socket may not be valid AFTER the callback returns!
     * DO NOT USE THE SOCKET-HANDLE AFTER THE CELLULAR_SOCKET_STATUS_CLOSED-CALLBACK RETURNS!
     */
    CellularUdpSocket_Callback_T StatusCallback;

    /**
     * @brief Flag that marks this socket as valid/invalid. If false this socket may be overwritten by OpenSocket-calls.
     */
    bool SocketValid;

    CellularSocket_Type_T SocketType;
    CellularSocket_Data_T Data;
};
typedef struct CellularSocket_S CellularSocket_T;

struct CellularSocket_ConnectRequest_S
{
    CellularSocket_T *Socket;
    uint32_t IpAddress;
    uint16_t Port;
};
typedef struct CellularSocket_ConnectRequest_S CellularSocket_ConnectRequest_T;

struct CellularSocket_RemoteIpPort_S
{
    uint32_t* Ip;
    uint16_t* Port;
};
typedef struct CellularSocket_RemoteIpPort_S CellularSocket_RemoteIpPort_T;

struct CellularSocket_SendRequest_S
{
    CellularSocket_T* Socket;
    uint8_t *Buffer;
    uint32_t BufferLength;
};
typedef struct CellularSocket_SendRequest_S CellularSocket_SendRequest_T;

struct CellularSocket_ReceiveRequest_S
{
    CellularSocket_T *Socket;
    uint8_t *Buffer;
    uint32_t BufferLength;
    uint32_t BytesRead;
};
typedef struct CellularSocket_ReceiveRequest_S CellularSocket_ReceiveRequest_T;

struct CellularSocket_QueryStatusRequest_S
{
    CellularSocket_T* Socket;
    CellularSocket_InfoParameter_T Parameter;
    void* Result;
};
typedef struct CellularSocket_QueryStatusRequest_S CellularSocket_QueryInfoRequest_T;

typedef struct
{
    CellularUdpSocket_T SocketId;
    uint32_t *Ip;
    uint16_t *Port;
    uint8_t **Buffer;
    uint32_t *Length;
} CellularSocket_ReceiveFromRequest_T;

typedef struct
{
    CellularUdpSocket_T SocketId;
    uint32_t Ip;
    uint16_t Port;
    uint8_t *Buffer;
    uint32_t Length;
} CellularSocket_SendToRequest_T;

typedef struct
{
    const char *Cmd;
    uint8_t *Buffer;
    uint32_t Length;
} CellularSocket_SendToData_T;

#define CELLULAR_SOCKET_MAX_COUNT              CELLULAR_SOCKET_COUNT /* The maximum number of sockets that can be managed */
#define CELLULAR_SOCKET_MAX_SEND_AT_A_TIME     (UINT32_C(128))
#define CELLULAR_SOCKET_MAX_RECEIVE_AT_A_TIME  (UINT32_C(128))
#define CELLULAR_SOCKET_CMD_BUFFER_SIZE        (UINT32_C(128))

#define CELLULAR_SOCKET_ENQUEUE_TIMEOUT        (UINT32_C(1000))
#define CELLULAR_SOCKET_RESPONSE_TIMEOUT       (UINT32_C(10000))
#define CELLULAR_SOCKET_URC_TIMEOUT            (UINT32_C(100))
#define CELLULAR_SOCKET_ERROR_DELAY            (UINT32_C(1000))
#define CELLULAR_SOCKET_SEND_DELAY             (UINT32_C(250))

#define IPV4_STR_SIZE                       (UINT32_C(16))

typedef enum
{
    CELLULAR_TCPIP_ERROR_CODE_MIN = -22,
    CELLULAR_TCPIP_ERROR_CODE_SUCCESS = 0,
    CELLULAR_TCPIP_ERROR_CODE_BIND_SOCKET_FAILED = 568,
    CELLULAR_TCPIP_ERROR_CODE_MAX = 575,
} Cellular_TcpIpErrorCode_T;

static CellularSocket_T SocketStore[CELLULAR_SOCKET_MAX_COUNT];
static CellularSocket_T *SocketReceiving = NULL;
static CellularSocket_SendToData_T SocketSendToData;

static const char *CELLULAR_SOCKET_TYPE_STR[CELLULAR_SOCKET_TYPE_COUNT] =
        {
                "TCP",
                "UDP",
                "TCP LISTENER",
                "UDP SERVICE"
        };

static const char *CELLULAR_SOCKET_QIURC_STR[CELLULAR_SOCKET_QIURC_COUNT] =
        {
                "\"closed\"",
                "\"recv\"",
                "\"incoming\"",
                "\"pdpdeact\""
        };

static char *IpToStr(uint32_t ip, char *str)
{
    char *b = (char *) &ip;
    (void) snprintf(str, IPV4_STR_SIZE, "%d.%d.%d.%d", b[0], b[1], b[2], b[3]); /*lint -esym(515 516,snprintf) */
    return str;
}

static CellularSocket_T *FindAvailableSocket(void)
{
    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        if (!SocketStore[i].SocketValid)
        {
            SocketStore[i].SocketId = i;
            return &SocketStore[i];
        }
    }

    return NULL;
}

static void InvalidateSocket(CellularUdpSocket_T SocketId)
{
    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        if (SocketId == SocketStore[i].SocketId)
        {
            SocketStore[i].SocketValid = false;
            SocketStore[i].StatusCallback = NULL;
        }
    }
}

static CellularSocket_T *GetSocket(CellularUdpSocket_T SocketId)
{
    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        if (SocketStore[i].SocketValid && SocketId == SocketStore[i].SocketId)
        {
            return &SocketStore[i];
        }
    }

    return NULL;
}

static Retcode_T OpenSocket(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    CellularSocket_OpenRequest_T* req = (CellularSocket_OpenRequest_T*) parameter;

    /* find next available socket */
    CellularSocket_T* AvailableSocket = FindAvailableSocket();
    if (NULL == AvailableSocket)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    Retcode_T retcode;
    uint8_t *arg;
    uint32_t len;

    char cmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE];
    char IpStr[IPV4_STR_SIZE];
    int count;

    if (0 != req->LocalPort)
    {
        count = snprintf(cmd, sizeof(cmd), ATCMD_OPEN_SOCKET_SERVER_FMT,
                req->ContextId,
                (unsigned int) AvailableSocket->SocketId,
                CELLULAR_SOCKET_TYPE_STR[req->SocketType],
                req->DomainName ? req->DomainName : IpToStr(req->IpAddress, IpStr),
                req->RemotePort,
                req->LocalPort);
    }
    else
    {
        count = snprintf(cmd, sizeof(cmd), ATCMD_OPEN_SOCKET_CLIENT_FMT,
                req->ContextId,
                (unsigned int) AvailableSocket->SocketId,
                CELLULAR_SOCKET_TYPE_STR[req->SocketType],
                req->DomainName ? req->DomainName : IpToStr(req->IpAddress, IpStr),
                req->RemotePort);
    }

    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, cmd);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = AtResponseQueue_WaitForNamedResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, AT_RESPONSE_CODE_OK);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (uint8_t *) ATCMD_OPEN_SOCKET_RESP, strlen(ATCMD_OPEN_SOCKET_RESP));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* parse ConnectId */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &arg, &len);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    int32_t ConnectId;
    retcode = CellularUtils_StrtolBounds(arg, len, &ConnectId, 0, CELLULAR_SOCKET_MAX_COUNT - 1);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* parse ErrorCode */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &arg, &len);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    int32_t ErrorCode;
    retcode = CellularUtils_StrtolBounds(arg, len, &ErrorCode, (int) CELLULAR_TCPIP_ERROR_CODE_MIN, (int) CELLULAR_TCPIP_ERROR_CODE_MAX);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    if ((int) CELLULAR_TCPIP_ERROR_CODE_SUCCESS == ErrorCode)
    {
        InvalidateSocket(ConnectId);
        *(req->SocketId) = ConnectId;

        AvailableSocket->SocketId = ConnectId;
        AvailableSocket->SocketType = req->SocketType;
        AvailableSocket->StatusCallback = req->StatusCallback;
        AvailableSocket->Data.BytesAvailable = 0;
        AvailableSocket->SocketValid = true;
    }
    else if ((int) CELLULAR_TCPIP_ERROR_CODE_BIND_SOCKET_FAILED == ErrorCode)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_SOCKET_BIND_FAILED);
    }

    return retcode;
}

static Retcode_T CloseSocket(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    CellularSocket_T* socket = GetSocket(*(CellularUdpSocket_T *) parameter);
    if (NULL == socket)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    char cmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE];

    int count = snprintf(cmd, sizeof(cmd), ATCMD_CLOSE_SOCKET_FMT, (unsigned int) socket->SocketId);
    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, cmd);

    retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);
    if (RETCODE_OK == retcode)
    {
        InvalidateSocket(socket->SocketId);
    }

    return retcode;
}

static Retcode_T SendSocketDataParser(AtResponseParserArgument_T *arg)
{
    if (NULL == arg || 0 == SocketSendToData.Length)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    char buffer[CELLULAR_SOCKET_CMD_BUFFER_SIZE];
    uint32_t BytesRead, UnreadCount;
    Retcode_T ret = RETCODE_OK;
    const char *resp = NULL;

    /* consume the command */
    uint32_t t0 = xTaskGetTickCount();
    while ((xTaskGetTickCount() - t0) < CELLULAR_SOCKET_RESPONSE_TIMEOUT)
    {
        ret = Cellular_ReadData((uint8_t*) buffer, (sizeof(buffer) - 1), &BytesRead);

        if (RETCODE_OK != ret)
        {
            LOG_ERROR("Retcode: %d", (int ) ret);
            return ret;
        }

        if (BytesRead)
        {
            buffer[BytesRead] = '\0';
            resp = strstr(buffer, ATCMD_SEND_DATA_START_RESP);
            (void) AtResponseParser_Parse((uint8_t *) buffer, resp ? (uint32_t)(resp - buffer) : BytesRead);
            if (resp)
            {
                AtResponseQueue_EnqueueEvent(AT_EVENT_TYPE_RESPONSE_CODE, NULL, 0);
                break;
            }
        }
        (void) xSemaphoreTake(arg->Semaphore, CELLULAR_SOCKET_SEND_DELAY / portTICK_PERIOD_MS);
    }

    if (NULL == resp)
    {
        LOG_ERROR("no start data response received");
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }

    /* consume the data, so it won't go to default AT response parser */
    t0 = xTaskGetTickCount();
    while (SocketSendToData.Length && (xTaskGetTickCount() - t0) < CELLULAR_SOCKET_RESPONSE_TIMEOUT)
    {
        UnreadCount = SocketSendToData.Length > sizeof(buffer) ?
                sizeof(buffer) : SocketSendToData.Length;

        ret = Cellular_ReadData((uint8_t*) buffer, UnreadCount, &BytesRead);

        if ((RETCODE_OK != ret) || (0 == BytesRead))
        {
            (void) xSemaphoreTake(arg->Semaphore, CELLULAR_SOCKET_SEND_DELAY / portTICK_PERIOD_MS);
            continue;
        }

        SocketSendToData.Length -= BytesRead;
    }

    if (SocketSendToData.Length)
    {
        LOG_ERROR("not all data consumed");
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }

    return RETCODE_OK;
}

static Retcode_T SocketSendTo(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    CellularSocket_SendToRequest_T* request = (CellularSocket_SendToRequest_T*) parameter;

    CellularSocket_T *socket = GetSocket(request->SocketId);
    if (NULL == socket)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    char cmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE];
    char IpStr[IPV4_STR_SIZE];
    int count;

    if (CELLULAR_SOCKET_TYPE_UDP_SERVICE == socket->SocketType)
    {
        count = snprintf(cmd, sizeof(cmd), ATCMD_SEND_DATA_UDP_SERVICE_FMT,
                (unsigned int) request->SocketId,
                (unsigned int) request->Length,
                IpToStr(request->Ip, IpStr),
                request->Port);
    }
    else
    {
        count = snprintf(cmd, sizeof(cmd), ATCMD_SEND_DATA_FMT,
                (unsigned int) request->SocketId,
                (unsigned int) request->Length);
    }

    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* set custom AT command parser */
    SocketSendToData.Cmd = cmd;
    SocketSendToData.Buffer = request->Buffer;
    SocketSendToData.Length = request->Length + strlen(ATCMD_AT_CRLF);

    CellularEngine_SetCustomAtResponseParser(SendSocketDataParser);

    /* send the command */
    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, cmd);
    if (RETCODE_OK != retcode)
    {
        CellularEngine_SetCustomAtResponseParser(NULL);
        return retcode;
    }

    /* wait for modem is ready to receive the data */
    retcode = AtResponseQueue_WaitForNamedResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, AT_RESPONSE_CODE_OK);
    if (RETCODE_OK != retcode)
    {
        CellularEngine_SetCustomAtResponseParser(NULL);
        return retcode;
    }

    /* send the data */
    retcode = CellularEngine_SendAtCommand(request->Buffer, request->Length);
    if (RETCODE_OK != retcode)
    {
        CellularEngine_SetCustomAtResponseParser(NULL);
        return retcode;
    }

    uint8_t *buffer;
    uint32_t BufferLen;
    retcode = AtResponseQueue_WaitForMiscContent(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &buffer, &BufferLen);

    if (RETCODE_OK == retcode)
    {
        if (0 != memcmp(buffer, ATCMD_SEND_DATA_OK_RESP, strlen(ATCMD_SEND_DATA_OK_RESP)))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
        }
        AtResponseQueue_MarkBufferAsUnused();
    }

    return retcode;
}

Retcode_T CellularSocket_Open(CellularSocket_OpenRequest_T *request)
{
    if (NULL == request || NULL == request->SocketId)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, OpenSocket, (void*) request, sizeof(request));
    if ((int) RETCODE_CELLULAR_SOCKET_BIND_FAILED == Retcode_GetCode(retcode))
    {
        /* Try again after a delay */
        vTaskDelay(CELLULAR_SOCKET_ERROR_DELAY / portTICK_PERIOD_MS);
        retcode = CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, OpenSocket, (void*) request, sizeof(request));
    }

    return retcode;
}

Retcode_T CellularSocket_Close(CellularUdpSocket_T socket)
{
    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, CloseSocket, (void*) &socket, sizeof(socket));
}

Retcode_T CellularSocket_SendTo(CellularUdpSocket_T socket, uint32_t ip, uint16_t port, uint8_t *buffer, uint32_t length)
{
    if ((uint32_t) socket >= CELLULAR_SOCKET_MAX_COUNT || NULL == buffer || 0 == length)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularSocket_SendToRequest_T request =
            {
                    .SocketId = socket,
                    .Ip = ip,
                    .Port = port,
                    .Buffer = buffer,
                    .Length = length
            };

    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, SocketSendTo, (void*) &request, sizeof(request));
}

static Retcode_T ReceiveSocketDataParser(AtResponseParserArgument_T *arg)
{
    if (NULL == arg || (UART_T)0 == arg->Device || NULL == arg->Semaphore || NULL == SocketReceiving)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    char buffer[CELLULAR_SOCKET_CMD_BUFFER_SIZE];
    Retcode_T ret;
    uint32_t BytesRead;
    const uint32_t UnreadCount = sizeof(buffer) - 1;
    const char *str = NULL;

    uint32_t t = xTaskGetTickCount();

    while ((xTaskGetTickCount() - t) < CELLULAR_SOCKET_RESPONSE_TIMEOUT)
    {
        /* read string */
        ret = Cellular_ReadData((uint8_t*) buffer, UnreadCount, &BytesRead);

        if ((RETCODE_OK != ret) || (0 == BytesRead))
        {
            LOG_ERROR("Retcode: %d, bytes_read: %d", (int ) ret, (int ) BytesRead);
            return ret;
        }

        /* make c-string */
        buffer[BytesRead] = '\0';

        /* check if the string contains expected response */
        str = strstr(buffer, ATCMD_RECEIVE_DATA_RESP);
        if (str)
        {
            break;
        }

        /* No expected response present, invoke default parser */
        (void) AtResponseParser_Parse((uint8_t *) buffer, BytesRead);

        /* wait for next string */
        if (pdPASS != xSemaphoreTake(arg->Semaphore, CELLULAR_SOCKET_RESPONSE_TIMEOUT))
        {
            LOG_ERROR("no response within %d ms", (int) CELLULAR_SOCKET_RESPONSE_TIMEOUT);
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
        }
    }

    if (NULL == str)
    {
        LOG_ERROR("no expected response");
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }

    /* invalidate old socket data */
    SocketReceiving->Data.Length = 0;

    uint32_t length;
    uint32_t port;
    uint32_t ip;
    char IpStr[IPV4_STR_SIZE];

    int count = sscanf(str, ATCMD_RECEIVE_DATA_RESP_FMT, (unsigned int *) &length, IpStr, (unsigned int *) &port);
    if (0 == count)
    {
        LOG_ERROR("invalid response: %s", str);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else if (1 == count || 0 == length)
    {
        /* No data present */
        LOG_WARNING("no data present");
        return RETCODE_OK;
    }
    else if (ATCMD_RECEIVE_DATA_RESP_ARG_COUNT != count)
    {
        LOG_ERROR("invalid response: %s", str);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* check scanned value bounds */
    if ((length > sizeof(SocketReceiving->Data.Buffer))
            || (port > CELLULAR_SOCKET_PORT_MAX))
    {
        LOG_ERROR("invalid length: %u", (unsigned int ) length);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (RETCODE_OK != CellularUtils_ParseIpAddress((uint8_t *) IpStr, strlen(IpStr), &ip))
    {
        LOG_ERROR("invalid ip: %s", IpStr);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* check if data already present */
    const char *data = strchr(str, '\n');
    if (NULL == data)
    {
        LOG_ERROR("no data present");
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }

    ++data;
    uint32_t received = 0;
    uint32_t remained = UnreadCount - (uint32_t)(data - buffer);
    remained = (remained > length) ? length : remained;

    if (remained > 0)
    {
        memcpy(SocketReceiving->Data.Buffer, data, remained);
        received += remained;
    }

    /* continue receiving data */
    while (received < length)
    {
        if (pdPASS != xSemaphoreTake(arg->Semaphore, CELLULAR_SOCKET_RESPONSE_TIMEOUT))
        {
            LOG_ERROR("no response within %d ms", (int) CELLULAR_SOCKET_RESPONSE_TIMEOUT);
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
        }

        ret = Cellular_ReadData(&SocketReceiving->Data.Buffer[received], (length - received), &BytesRead);

        if ((RETCODE_OK != ret) || (0 == BytesRead))
        {
            LOG_ERROR("Retcode: %d, BytesRead: %d", (int ) ret, (int ) BytesRead);
            return ret;
        }
        received += BytesRead;
    }

    /* set scanned values */
    SocketReceiving->Data.Ip = ip;
    SocketReceiving->Data.Port = port;
    SocketReceiving->Data.Length = length;

    return RETCODE_OK;
}

static Retcode_T SocketReceiveFrom(CellularSocket_T *socket)
{
    char cmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE];
    int count = snprintf(cmd, sizeof(cmd), ATCMD_RECEIVE_DATA_FMT,
            (unsigned int) socket->SocketId, sizeof(socket->Data.Buffer));

    if (count <= 0 || (uint32_t) count >= sizeof(cmd))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* set custom AT command parser */
    SocketReceiving = socket;
    CellularEngine_SetCustomAtResponseParser(ReceiveSocketDataParser);

    /* send the command */
    Retcode_T retcode = CellularEngine_SendAtCmdWaitEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, cmd);
    if (RETCODE_OK != retcode)
    {
        CellularEngine_SetCustomAtResponseParser(NULL);
        return retcode;
    }

    /* wait for OK */
    retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);
    if (RETCODE_OK != retcode || 0 == socket->Data.Length)
    {
        socket->Data.BytesAvailable = 0;
        return retcode;
    }

    if (socket->Data.BytesAvailable > socket->Data.Length)
    {
        socket->Data.BytesAvailable -= socket->Data.Length;
    }
    else
    {
        socket->Data.BytesAvailable = 0;
    }

    return retcode;
}

Retcode_T CellularSocket_ReceiveFrom(CellularUdpSocket_T SocketId, uint32_t *ip, uint16_t *port, uint8_t **buffer, uint32_t *length)
{
    if ((uint32_t) SocketId >= CELLULAR_SOCKET_MAX_COUNT || NULL == ip || NULL == port || NULL == buffer || NULL == length)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularSocket_T *socket = GetSocket(SocketId);
    if (NULL == socket || 0 == socket->Data.BytesAvailable)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = SocketReceiveFrom(socket);

    if (RETCODE_OK == retcode)
    {
        *ip = socket->Data.Ip;
        *port = socket->Data.Port;
        *length = socket->Data.Length;
        *buffer = socket->Data.Buffer;
    }
    else
    {
        socket->Data.BytesAvailable = 0;
    }

    return retcode;
}

void CellularSocket_NotifyDataReceived(void)
{
    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        if (SocketStore[i].SocketValid && SocketStore[i].StatusCallback
                && 0 != SocketStore[i].Data.BytesAvailable)
        {
            SocketStore[i].StatusCallback(SocketStore[i].SocketId, CELLULAR_UDP_EVENT_DATA_RECEIVED);
        }
    }
}

/* put URC handler implementations here */
Retcode_T CellularSocketUrc_QIURC(void)
{
    Retcode_T retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *) ATCMD_SOCKET_URC, strlen(ATCMD_SOCKET_URC));
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    uint8_t *buffer;
    uint32_t BufferLen;

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    int urc;
    for (urc = (int) CELLULAR_SOCKET_QIURC_CLOSED; urc < (int) CELLULAR_SOCKET_QIURC_COUNT; ++urc)
    {
        if (0 == memcmp(CELLULAR_SOCKET_QIURC_STR[urc], buffer, BufferLen))
        {
            break;
        }
    }

    AtResponseQueue_MarkBufferAsUnused();
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode || (int) CELLULAR_SOCKET_QIURC_COUNT == urc)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    LOG_DEBUG("URC: %s %s,%.*s", ATCMD_SOCKET_URC, CELLULAR_SOCKET_QIURC_STR[urc], (int ) BufferLen, buffer);

    if ((int) CELLULAR_SOCKET_QIURC_PDP_DEACTIVATED != urc)
    {
        // get ConnectId
        CellularUdpSocket_T SocketId;
        retcode = CellularUtils_StrtolBounds(buffer, BufferLen, &SocketId, 0, CELLULAR_SOCKET_MAX_COUNT - 1);
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK != retcode)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        CellularSocket_T *socket = GetSocket(SocketId);
        if (NULL == socket)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        if ((int) CELLULAR_SOCKET_QIURC_CLOSED == urc)
        {
            if (socket->StatusCallback)
            {
                socket->StatusCallback(socket->SocketId, CELLULAR_UDP_EVENT_SOCKET_CLOSE);
            }
            InvalidateSocket(SocketId);
        }
        else
        {
            socket->Data.BytesAvailable = 1;
        }
    }
    else
    {
        // get contextid
        int32_t contextid;
        retcode = CellularUtils_StrtolBounds(buffer, BufferLen, &contextid,
        CELLULAR_NETWORK_PROFILE_ID_LOW, CELLULAR_NETWORK_PROFILE_ID_HIGH);
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK != retcode)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }
        CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_DISCONNECTED, CELLULAR_EVENT_DISCONNECTED, NULL);
        CellularSocket_InvalidateAll();
    }

    return retcode;
}

void CellularSocket_InvalidateAll(void)
{
    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        if (SocketStore[i].SocketValid && SocketStore[i].StatusCallback)
        {
            SocketStore[i].StatusCallback(SocketStore[i].SocketId, CELLULAR_UDP_EVENT_SOCKET_CLOSE);
        }
        SocketStore[i].SocketValid = false;
        SocketStore[i].StatusCallback = NULL;
    }
}
