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
#include "BCDS_Logging.h"
#include "Socket.h"

/**
 * @brief Socket info parameters:
 *
 * @see  Ref 2, p 529, 25.25 Socket control +USOCTL
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

#define CELLULAR_SOCKET_RECEIVE_FROM_LENGTH_MAX    (UINT32_C(1024))
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

    CellularSocket_Data_T Data;
};
typedef struct CellularSocket_S CellularSocket_T;

struct CellularSocket_OpenRequest_S
{
    CellularUdpSocket_T *SocketId;
    CellularSocket_Type_T SocketType;
    CellularUdpSocket_Callback_T StatusCallback;
    uint16_t LocalPort;
};
typedef struct CellularSocket_OpenRequest_S CellularSocket_OpenRequest_T;

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

#define CELLULAR_SOCKET_MAX_COUNT          CELLULAR_SOCKET_COUNT /* The maximum number of sockets that can be managed */
#define CELLULAR_SOCKET_MAX_SEND_AT_A_TIME (UINT32_C(128))
#define CELLULAR_SOCKET_MAX_RECEIVE_AT_A_TIME (UINT32_C(128))
#define CELLULAR_SOCKET_SEND_POST_AT_WAIT  (UINT32_C(50000)) /* 50 milliseconds, as per Cellular AT Commands Manual, page 492, section 25.10.4 */
#define CELLULAR_SOCKET_ENQUEUE_TIMEOUT    (UINT32_C(1000))
#define CELLULAR_SOCKET_RESPONSE_TIMEOUT   (UINT32_C(10000)) /* 10 seconds */
#define CELLULAR_SOCKET_URC_TIMEOUT        (UINT32_C(100))   /* 100 milliseconds */
#define CELLULAR_SOCKET_CMD_BUFFER_SIZE    (UINT32_C(64))

static CellularSocket_T SocketStore[CELLULAR_SOCKET_MAX_COUNT];

static Retcode_T CloseSocket(void* parameter, uint32_t ParameterLength);

static CellularSocket_T *FindAvailableSocket(void)
{
    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        if (!SocketStore[i].SocketValid)
        {
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
    CellularSocket_OpenRequest_T* SocketRequest = (CellularSocket_OpenRequest_T*) parameter;

    /* find next available socket */
    CellularSocket_T* AvailableSocket = FindAvailableSocket();

    if (NULL == AvailableSocket)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    uint8_t* ResponseSocketIdBuff = NULL;
    uint32_t ResponseSocketIdBuffLength = 0xFF;
    bool CleanupRequired = false;
    Retcode_T retcode;

    /* create socket on modem */
    switch (SocketRequest->SocketType)
    {
    case CELLULAR_SOCKET_TYPE_TCP:
        case CELLULAR_SOCKET_TYPE_UDP:
            if (0 == SocketRequest->LocalPort)
            {
                char cmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE];
                int count = snprintf(cmd, sizeof(cmd), "AT+USOCR=%d\r\n", SocketRequest->SocketType); /*lint -esym(515 516,snprintf) */
                if (count <= 0 || (uint32_t) count >= sizeof(cmd))
                {
                    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                }

                const char* ResponseCmd = "USOCR";

                retcode = CellularEngine_SendAtCommand((uint8_t*) cmd, strlen(cmd));

                if (RETCODE_OK == retcode)
                {
                    retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (uint8_t *) cmd, strlen(cmd) - 2);
                }

                if (RETCODE_OK == retcode)
                {
                    retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (uint8_t *) ResponseCmd, strlen(ResponseCmd));
                }

                if (RETCODE_OK == retcode)
                {
                    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &ResponseSocketIdBuff, &ResponseSocketIdBuffLength);
                }

                if (RETCODE_OK == retcode)
                {
                    /* OK, so we just received something like an argument. If this argument is an Id we assume we just
                     * opened a socket. If something from now on goes wrong, we need to make sure that we at least
                     * try to close this socket! */
                    CleanupRequired = true;
                }
            }
            else
            {
                /* TODO TCP BINDING CURRENTLY NOT SUPPORTED */
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
            }
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
            break;
    }

    int32_t id = 0xFF;

    /* extract id from response buffer */
    if (CleanupRequired)
    {
        retcode = CellularUtils_StrtolBounds(ResponseSocketIdBuff, ResponseSocketIdBuffLength, &id, 0, CELLULAR_SOCKET_MAX_COUNT - 1);
        AtResponseQueue_MarkBufferAsUnused();
    }

    /* store socket info in local socket */
    if (RETCODE_OK == retcode)
    {
        /* Now that we have the id we need to verify that there are no local socket stored with the same id
         * If there are, we will mark them as "invalid". */
        InvalidateSocket(id);

        AvailableSocket->SocketId = id;
        AvailableSocket->StatusCallback = SocketRequest->StatusCallback;
        AvailableSocket->SocketValid = true;
        AvailableSocket->Data.BytesAvailable = 0;
        *(SocketRequest->SocketId) = id;
    }

    if (RETCODE_OK != retcode && CleanupRequired)
    {
        CellularSocket_T CleanupSocket;
        CleanupSocket.SocketId = id;
        CleanupSocket.SocketValid = true;
        CleanupSocket.StatusCallback = NULL;
        CleanupSocket.Data.BytesAvailable = 0;
        (void) CloseSocket((void*) &CleanupSocket, 0); /* in case something went wrong, at least try to close the socket */
    }

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);
}

static Retcode_T CloseSocket(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    CellularSocket_T* socket = GetSocket(*(CellularUdpSocket_T *) parameter);
    if (NULL == socket)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode;

    const char* BaseCmd = "AT+USOCL=";
    uint32_t CmdBuffLength = strlen(BaseCmd) + 3 + 2; /* +3 is the max number of digits a uint8_t can hold +2 for \r\n */
    uint8_t cmd[CmdBuffLength];
    uint32_t CmdLength = 0;

    memset(cmd, 0, CmdBuffLength);
    CmdLength = snprintf((char *) cmd, CmdBuffLength, "%s%u\r\n", BaseCmd, (unsigned int) socket->SocketId);

    if (CmdLength > CmdBuffLength)
    {
        LOG_ERROR("CellularSocket_Close: error while constructing command!");
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    retcode = CellularEngine_SendAtCommand((uint8_t*) cmd, CmdLength);

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, cmd, CmdLength - 2);
    }

    retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);

    if (RETCODE_OK == retcode)
    {
        InvalidateSocket(socket->SocketId);
    }

    return retcode;
}

static Retcode_T ConnectSocket(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = RETCODE_OK;

    CellularSocket_ConnectRequest_T* ConnectRequest = (CellularSocket_ConnectRequest_T*) parameter;

    const char* AtCmdTemplate = "AT+USOCO=x,\"xxx.Xxx.Xxx.Xxx\",xxxxx\r\n";
    uint8_t AtCmd[strlen(AtCmdTemplate)];
    uint8_t echo[sizeof(AtCmd) - 2];
    int AtCmdLength = 0;
    int AtCmdLengthNoLfCr = 0;

    /* construct AtCmd */
    CellularUdpSocket_T SocketId = ConnectRequest->Socket->SocketId;
    uint8_t* IpBase = (uint8_t*) &ConnectRequest->IpAddress; /* we cast to uint8_t to allow easy access to each octet */
    uint16_t port = ConnectRequest->Port;

    AtCmdLength = snprintf((char*) AtCmd, sizeof(AtCmd), "AT+USOCO=%u,\"%u.%u.%u.%u\",%u\r\n", (unsigned int) SocketId, IpBase[0], IpBase[1], IpBase[2], IpBase[3], port);

    if (AtCmdLength < 0 || AtCmdLength > ((int) sizeof(AtCmd)))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* set AtCmdLengthNoLfCr (AtCmd with out the \r\n) to get the length of the echo */
    if (RETCODE_OK == retcode)
    {
        AtCmdLengthNoLfCr = AtCmdLength - strlen("\r\n");
       /* send AtCmd */
        retcode = CellularEngine_SendAtCommand(AtCmd, sizeof(AtCmd));
    }

    /* wait for echo */
    if (RETCODE_OK == retcode)
    {
        /* set echo to 0 */
        memset(echo, 0, sizeof(echo));

        /* choose AtCmdLengthNoLfCr if it is smaller than sizeof(echo) (we can just cast to uint32_t, zero check is done before) */
        uint32_t EchoLength = (uint32_t) AtCmdLengthNoLfCr < sizeof(echo) ? (uint32_t) AtCmdLengthNoLfCr : sizeof(echo);

        /* copy AtCmd into echo w/o the \r\n at the end (not needed for echo),
         * also if AtCmdLength is smaller than sizeof(echo) we use AtCmdLength */
        (void) strncpy((char*) echo, (const char*) AtCmd, EchoLength);

        retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, echo, EchoLength);
    }

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);
}

static Retcode_T QuerySocketInfo(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    CellularSocket_QueryInfoRequest_T* QueryInfoRequest = (CellularSocket_QueryInfoRequest_T*) parameter;

    const char* cmd = "USOCTL";
    uint8_t AtCmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE];
    uint8_t ParameterStr[3]; /* two-digit-%u + NULL-terminator just to be safe */
    uint8_t SocketIdStr[2]; /* single-digit-%u + NULL-terminator just to be safe */
    int AtCmdLength = 0;
    int ParameterStrLength = 0;
    int SocketIdStrLength = 0;

    /* construct ParameterStr */
    ParameterStrLength = snprintf((char*) ParameterStr, sizeof(ParameterStr), "%u", QueryInfoRequest->Parameter);
    if (ParameterStrLength < 0 || ParameterStrLength > ((int) sizeof(ParameterStr)))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* construct SocketIdStr */
    SocketIdStrLength = snprintf((char*) SocketIdStr, sizeof(SocketIdStr), "%u", (unsigned int) QueryInfoRequest->Socket->SocketId);
    if (SocketIdStrLength < 0 || SocketIdStrLength > ((int) sizeof(SocketIdStr)))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* construct AtCmd */
    AtCmdLength = snprintf((char*) AtCmd, sizeof(AtCmd), "AT+USOCTL=%s,%s\r\n", SocketIdStr, ParameterStr);
    if (AtCmdLength < 0 || AtCmdLength > ((int) sizeof(AtCmd)))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* send AtCmd */
    Retcode_T retcode = CellularEngine_SendAtCommand(AtCmd, strlen((const char*) AtCmd));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* wait for echo */
    retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, AtCmd, AtCmdLength - 2);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* wait for cmd */
    retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (const uint8_t*) cmd, strlen(cmd));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* wait for socket id argument echo */
    retcode = AtResponseQueue_WaitForNamedCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, SocketIdStr, strlen((const char*) SocketIdStr));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* wait for parameter value argument echo */
    retcode = AtResponseQueue_WaitForNamedCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, ParameterStr, strlen((const char*) ParameterStr));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    uint8_t* buffer;
    uint32_t BufferLength;

    /* wait for socket status argument */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &buffer, &BufferLength);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    int32_t PortInt32 = 0;
    int32_t IntegerFromString = 0;
    CellularSocket_RemoteIpPort_T* IpPort = NULL;

    switch (QueryInfoRequest->Parameter)
    {
    case CELLULAR_SOCKET_INFO_TYPE:
        retcode = CellularUtils_StrtolBounds(buffer, BufferLength, &IntegerFromString,
                (int32_t) CELLULAR_SOCKET_TYPE_TCP, (int32_t) CELLULAR_SOCKET_TYPE_UDP);

        if (RETCODE_OK == retcode)
        {
            *((CellularSocket_Type_T*) QueryInfoRequest->Result) = (CellularSocket_Type_T) IntegerFromString;
        }
        break;
    case CELLULAR_SOCKET_INFO_STATUS:
        retcode = CellularUtils_StrtolBounds(buffer, BufferLength, &IntegerFromString,
                (int32_t) CELLULAR_SOCKET_TCP_STATUS_INACTIVE, (int32_t) CELLULAR_SOCKET_TCP_STATUS_TIME_WAIT);

        if (RETCODE_OK == retcode)
        {
            *((CellularSocket_TcpStatus_T*) QueryInfoRequest->Result) = (CellularSocket_TcpStatus_T) IntegerFromString;
        }
        break;
    case CELLULAR_SOCKET_INFO_REMOTE_IP_PORT:
        IpPort = (CellularSocket_RemoteIpPort_T*) QueryInfoRequest->Result;

        if (BufferLength >= strlen("\"0.0.0.0\""))
        {
            /* We want to cut off the ""'s at the beginning and end of the string. */
            retcode = CellularUtils_ParseIpAddress(buffer + 1, BufferLength - 2, IpPort->Ip);
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        if (RETCODE_OK == retcode)
        {
            AtResponseQueue_MarkBufferAsUnused();
            retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &buffer, &BufferLength);
        }

        if (RETCODE_OK == retcode)
        {
            retcode = CellularUtils_Strtol(buffer, BufferLength, &PortInt32);
        }

        if (RETCODE_OK == retcode)
        {
            if (PortInt32 < 0 || PortInt32 >= UINT16_MAX)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            }
        }

        if (RETCODE_OK == retcode)
        {
            *(IpPort->Port) = (uint16_t) PortInt32;
        }

        break;
    case CELLULAR_SOCKET_INFO_LAST_ERROR:
        case CELLULAR_SOCKET_INFO_BYTES_SENT:
        case CELLULAR_SOCKET_INFO_BYTES_RECEIVED:
        case CELLULAR_SOCKET_INFO_UNACKNOWLEGED_DATA:
        retcode = CellularUtils_Strtol(buffer, BufferLength, &IntegerFromString);

        if (RETCODE_OK == retcode)
        {
            *((uint32_t*) QueryInfoRequest->Result) = (uint32_t) IntegerFromString;
        }
        break;
    default: /* this should never happen */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        break;
    }

    AtResponseQueue_MarkBufferAsUnused();

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);
}

static Retcode_T SendSocket(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = RETCODE_OK;

    CellularSocket_SendRequest_T* request = (CellularSocket_SendRequest_T*) parameter;

    CellularSocket_T *socket = request->Socket;

    // disable AT command echo (we don't want to mirror all the data we're going to send back to us for no good reason)
    const char *DisableEchoAtCommand = "ATE0\r\n";
    retcode = CellularEngine_SendAtCommand((uint8_t *) DisableEchoAtCommand, strlen(DisableEchoAtCommand));
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_IgnoreEvent(CELLULAR_SOCKET_RESPONSE_TIMEOUT);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, AT_RESPONSE_CODE_OK);
    }

    // start sending
    if (RETCODE_OK == retcode)
    {
        bool ByteSetSendSuccessfully = true;
        uint32_t BytesSentSoFar = 0;
        char cmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE + 2 * CELLULAR_SOCKET_MAX_SEND_AT_A_TIME];

        while (BytesSentSoFar < request->BufferLength && ByteSetSendSuccessfully)
        {
            // decide how many bytes to send
            uint32_t BytesToSend = (request->BufferLength - BytesSentSoFar);
            if (BytesToSend > CELLULAR_SOCKET_MAX_SEND_AT_A_TIME)
            {
                BytesToSend = CELLULAR_SOCKET_MAX_SEND_AT_A_TIME;
            }

            // prepare AT command
            uint32_t offset = snprintf(cmd, sizeof(cmd), "AT+USOWR=%"PRIu32",%"PRIu32",\"", socket->SocketId, BytesToSend);
            if (offset >= sizeof(cmd))
            {
                return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }

            /* put data */
            CellularUtils_BinToHex(&request->Buffer[BytesSentSoFar], &cmd[offset], BytesToSend);
            offset += 2 * BytesToSend;

            if (offset >= sizeof(cmd))
            {
                return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }

            /* put footer */
            offset += snprintf(&cmd[offset], sizeof(cmd) - offset, "\"\r\n");
            if (offset >= sizeof(cmd))
            {
                return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }

            // send AT command
            bool CommandSentSuccessfully = false;
            retcode = CellularEngine_SendAtCommand((uint8_t *) cmd, offset);

            // we disabled the AT command echo earlier, so we don't expect an echo but the command directly
            if (RETCODE_OK == retcode)
            {
                CommandSentSuccessfully = true;
                retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (const uint8_t *) "USOWR", strlen("USOWR"));
            }
            if (RETCODE_OK == retcode)
            {
                uint8_t *ArgBuffer;
                uint32_t ArgBufferLen;

                retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &ArgBuffer, &ArgBufferLen);
                if (RETCODE_OK == retcode)
                {
                    AtResponseQueue_MarkBufferAsUnused();
                }
            }
            if (RETCODE_OK == retcode)
            {
                uint8_t *ArgBuffer;
                uint32_t ArgBufferLen;

                retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &ArgBuffer, &ArgBufferLen);
                if (RETCODE_OK == retcode)
                {
                    // extract number of bytes we sent
                    int32_t BytesSent;
                    retcode = CellularUtils_StrtolBounds(ArgBuffer, ArgBufferLen, &BytesSent, 0, INT32_MAX);

                    if (RETCODE_OK == retcode)
                    {
                        BytesSentSoFar += BytesSent;
                    }
                    else if (Retcode_GetCode(retcode) == (uint32_t) RETCODE_INVALID_PARAM)
                    {
                        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
                    }

                    AtResponseQueue_MarkBufferAsUnused();
                }
            }
            if (CommandSentSuccessfully)
            {
                retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);
            }

            ByteSetSendSuccessfully = (RETCODE_OK == retcode);
        }
    }

    // reenable the echo ... we HAVE to do this no matter what happened above
    const char *EnableEchoAtCommand = "ATE1\r\n";
    Retcode_T ReenableEchoRetcode = CellularEngine_SendAtCommand((uint8_t *) EnableEchoAtCommand, strlen(EnableEchoAtCommand));

    ReenableEchoRetcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, ReenableEchoRetcode);

    if (RETCODE_OK == retcode && RETCODE_OK != ReenableEchoRetcode)
    {
        retcode = ReenableEchoRetcode;
    }

    return retcode;
}

static Retcode_T SendSocketTo(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    CellularSocket_SendToRequest_T* request = (CellularSocket_SendToRequest_T*) parameter;

    CellularSocket_T *socket = GetSocket(request->SocketId);
    if (NULL == socket)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* disable AT command echo */
    Retcode_T retcode;
    retcode = Cellular_EnableEcho(false);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    uint8_t *ip = (uint8_t *) &request->Ip;
    char cmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE + 2 * CELLULAR_SOCKET_MAX_SEND_AT_A_TIME];
    uint32_t index = 0;
    uint32_t BytesToSend = request->Length;

    while (0 != BytesToSend)
    {
        uint32_t length = BytesToSend > CELLULAR_SOCKET_MAX_SEND_AT_A_TIME ?
        CELLULAR_SOCKET_MAX_SEND_AT_A_TIME : BytesToSend;

        /* put command header */
        uint32_t offset = snprintf(cmd, sizeof(cmd), "AT+USOST=%u,\"%u.%u.%u.%u\",%u,%u,\"",
                (unsigned int) request->SocketId, ip[0], ip[1], ip[2], ip[3], request->Port, (unsigned int) length);
        if (offset >= sizeof(cmd))
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }

        /* put data */
        CellularUtils_BinToHex(&request->Buffer[index], &cmd[offset], length);

        /* put footer */
        offset += 2 * length;
        if (offset >= sizeof(cmd))
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }

        uint32_t size = snprintf(&cmd[offset], sizeof(cmd) - offset, "\"\r\n") + offset;
        if (size >= sizeof(cmd))
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }

        /* send the command */
        retcode = CellularEngine_SendAtCommand((uint8_t *) cmd, size);
        if (RETCODE_OK != retcode)
        {
            return retcode;
        }

        /* wait for response */
        retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (const uint8_t *) "USOST", strlen("USOST"));
        if (RETCODE_OK != retcode)
        {
            return retcode;
        }

        uint8_t *buffer;
        uint32_t BufferLen;

        /* get socket ID */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &buffer, &BufferLen);
        if (RETCODE_OK != retcode)
        {
            return retcode;
        }

        CellularUdpSocket_T SocketId;
        retcode = CellularUtils_StrtolBounds(buffer, BufferLen, &SocketId, 0, CELLULAR_SOCKET_MAX_COUNT - 1);
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK != retcode || SocketId != request->SocketId)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        /* get length */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
        if (RETCODE_OK != retcode)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        uint32_t RespLength;
        retcode = CellularUtils_StrtolBounds(buffer, BufferLen, (int32_t *) &RespLength, 1, CELLULAR_SOCKET_RECEIVE_FROM_LENGTH_MAX);
        AtResponseQueue_MarkBufferAsUnused();
        if (RETCODE_OK != retcode || RespLength != length)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        /* wait for OK */
        retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);
        if (RETCODE_OK != retcode)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        }

        index += length;
        BytesToSend -= length;
    }

    /* enable AT command echo */
    return Cellular_EnableEcho(true);
}

static Retcode_T ReceiveSocket(void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    Retcode_T retcode = RETCODE_OK;

    CellularSocket_ReceiveRequest_T* request = (CellularSocket_ReceiveRequest_T*) parameter;

    CellularSocket_T *socket = (CellularSocket_T *) request->Socket;

    // start receiving
    uint8_t AtCmdBuffer[strlen("AT+USORD=x,xxx\r\n00") + 2 * CELLULAR_SOCKET_MAX_RECEIVE_AT_A_TIME];

    // decide how many bytes to send
    uint32_t BytesToRead = request->BufferLength;
    if (BytesToRead > CELLULAR_SOCKET_MAX_RECEIVE_AT_A_TIME)
        BytesToRead = CELLULAR_SOCKET_MAX_RECEIVE_AT_A_TIME;

    // prepare AT command
    (void) snprintf((char *) AtCmdBuffer, sizeof(AtCmdBuffer), "AT+USORD=%"PRIu32",%"PRIu32"\r\n", socket->SocketId, BytesToRead);
    uint32_t AtCommandLength = strlen((const char *) AtCmdBuffer);

    // send AT command
    retcode = CellularEngine_SendAtCommand(AtCmdBuffer, AtCommandLength);

    // wait for command echo
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (const uint8_t *) AtCmdBuffer, AtCommandLength - 2);
    }

    // wait for command
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (const uint8_t *) "USORD", strlen("USORD"));
    }

    // wait for socket ID
    if (RETCODE_OK == retcode)
    {
        uint8_t *ArgBuffer;
        uint32_t ArgBufferLen;

        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &ArgBuffer, &ArgBufferLen);
        if (RETCODE_OK == retcode)
        {
            AtResponseQueue_MarkBufferAsUnused();
        }
    }

    // wait for bytes read
    uint32_t BytesRead = 0;
    if (RETCODE_OK == retcode)
    {
        uint8_t *ArgBuffer;
        uint32_t ArgBufferLen;

        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &ArgBuffer, &ArgBufferLen);
        if (RETCODE_OK == retcode)
        {
            // extract number of bytes we sent
            int32_t BytesReadParsed;
            retcode = CellularUtils_StrtolBounds(ArgBuffer, ArgBufferLen, &BytesReadParsed, 0, INT32_MAX);

            if (Retcode_GetCode(retcode) == (uint32_t) RETCODE_INVALID_PARAM)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
                BytesReadParsed = 0;
            }
            BytesRead = (uint32_t) BytesReadParsed;
            AtResponseQueue_MarkBufferAsUnused();
        }
    }

    // wait for the bytes themselves
    if (RETCODE_OK == retcode)
    {
        uint8_t *ArgBuffer;
        uint32_t ArgBufferLen;

        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &ArgBuffer, &ArgBufferLen);
        if (RETCODE_OK == retcode)
        {
            if (ArgBufferLen >= 2 * BytesRead)
            {
                // convert HEX chars to bytes
                retcode = CellularUtils_HexToBin(&ArgBuffer[1], request->Buffer, BytesRead);
                if (RETCODE_OK == retcode)
                {
                    request->BytesRead = BytesRead;
                }
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            }
            AtResponseQueue_MarkBufferAsUnused();
        }
    }

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);
}

Retcode_T CellularSocket_Open(CellularUdpSocket_T *socket, CellularSocket_Type_T SocketType, CellularUdpSocket_Callback_T StatusCallback)
{
    if (NULL == socket || SocketType >= CELLULAR_SOCKET_TYPE_MAX || NULL == StatusCallback)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularSocket_OpenRequest_T SocketRequest;
    SocketRequest.SocketId = socket;
    SocketRequest.SocketType = SocketType;
    SocketRequest.StatusCallback = StatusCallback;
    SocketRequest.LocalPort = 0;

    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, OpenSocket, (void*) &SocketRequest, sizeof(SocketRequest));
}

Retcode_T CellularSocket_Close(CellularUdpSocket_T socket)
{
    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, CloseSocket, (void*) &socket, sizeof(socket));
}

Retcode_T CellularSocket_Send(CellularSocket_Handle_T socket, uint8_t *buffer, uint32_t BufferLength)
{
    if (NULL == socket || !((CellularSocket_T*) socket)->SocketValid || NULL == buffer || 0 == BufferLength)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularSocket_SendRequest_T request =
            {
                    .Socket = (CellularSocket_T*) socket,
                    .Buffer = buffer,
                    .BufferLength = BufferLength
            };

    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, SendSocket, (void*) &request, sizeof(request));
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

    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, SendSocketTo, (void*) &request, sizeof(request));
}

Retcode_T CellularSocket_Receive(CellularSocket_Handle_T socket, uint8_t *buffer, uint32_t BufferLength, uint32_t *BytesRead, uint32_t *SenderIpAddress, uint16_t *SenderPort)
{
    BCDS_UNUSED(SenderIpAddress);
    BCDS_UNUSED(SenderPort);

    if (NULL == socket || NULL == buffer || 0 == BufferLength || NULL == BytesRead)
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);

    CellularSocket_ReceiveRequest_T request =
            {
                    .Socket = (CellularSocket_T *) socket,
                    .Buffer = buffer,
                    .BufferLength = BufferLength,
                    .BytesRead = 0
            };

    Retcode_T retcode = CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, ReceiveSocket, (void*) &request, sizeof(request));

    if (RETCODE_OK == retcode)
    {
        *BytesRead = request.BytesRead;
    }

    return retcode;
}

static Retcode_T ReceiveSocketFrom(CellularSocket_T *socket)
{
    /* send the command */
    Retcode_T retcode;
    char cmd[CELLULAR_SOCKET_CMD_BUFFER_SIZE];
    (void) snprintf(cmd, sizeof(cmd), "AT+USORF=%u,%u\r\n", (unsigned int) socket->SocketId, sizeof(socket->Data.Buffer));

    retcode = CellularEngine_SendAtCommand((uint8_t *) cmd, strlen(cmd));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* wait for command echo */
    retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (uint8_t *) cmd, strlen(cmd) - 2);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* wait for answer */
    retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_SOCKET_RESPONSE_TIMEOUT, (uint8_t *) "USORF", strlen("USORF"));
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    uint8_t *buffer;
    uint32_t BufferLen;

    /* get socket ID */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_RESPONSE_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    CellularUdpSocket_T SocketId;
    retcode = CellularUtils_StrtolBounds(buffer, BufferLen, &SocketId, 0, CELLULAR_SOCKET_MAX_COUNT - 1);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode || SocketId != socket->SocketId)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* get ip */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    uint32_t ip;
    retcode = CellularUtils_ParseIpAddress(buffer + 1, BufferLen - 2, &ip);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* get port */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    uint32_t port;
    retcode = CellularUtils_StrtolBounds(buffer, BufferLen, (int32_t *) &port, 0, CELLULAR_SOCKET_PORT_MAX);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* get length */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    uint32_t length;
    retcode = CellularUtils_StrtolBounds(buffer, BufferLen, (int32_t *) &length, 1, CELLULAR_SOCKET_RECEIVE_FROM_LENGTH_MAX);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    if (length > sizeof(socket->Data.Buffer))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    /* get data */
    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    if (BufferLen != ((2 * length) + 2)) /* two hex characters represent one byte */
    {
        AtResponseQueue_MarkBufferAsUnused();
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    /* convert hex to bytes */
    retcode = CellularUtils_HexToBin(&buffer[1], socket->Data.Buffer, length);
    AtResponseQueue_MarkBufferAsUnused();

    /* wait for OK */
    retcode = CellularUtils_WaitForAndHandleResponseCode(CELLULAR_SOCKET_RESPONSE_TIMEOUT, retcode);

    if (RETCODE_OK == retcode)
    {
        socket->Data.Ip = ip;
        socket->Data.Port = port;
        socket->Data.Length = length;

        if (socket->Data.BytesAvailable > length)
        {
            socket->Data.BytesAvailable -= length;
        }
        else
        {
            socket->Data.BytesAvailable = 0;
        }
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

    Retcode_T retcode = ReceiveSocketFrom(socket);

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

Retcode_T CellularSocket_GetPeer(CellularSocket_Handle_T socket, uint32_t* ip, uint16_t* port)
{
    if (NULL == socket || NULL == ip || NULL == port)
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);

    CellularSocket_RemoteIpPort_T IpPort;
    IpPort.Ip = ip;
    IpPort.Port = port;

    CellularSocket_QueryInfoRequest_T QueryInfoRequest;
    QueryInfoRequest.Socket = (CellularSocket_T*) socket;
    QueryInfoRequest.Parameter = CELLULAR_SOCKET_INFO_REMOTE_IP_PORT;
    QueryInfoRequest.Result = &IpPort;

    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, QuerySocketInfo, (void*) &QueryInfoRequest, sizeof(QueryInfoRequest));
}

Retcode_T CellularSocket_GetTcpStatus(CellularSocket_Handle_T socket, CellularSocket_TcpStatus_T *SocketStatus)
{
    if (NULL == socket || NULL == SocketStatus)
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);

    CellularSocket_QueryInfoRequest_T QueryInfoRequest;
    QueryInfoRequest.Socket = (CellularSocket_T*) socket;
    QueryInfoRequest.Parameter = CELLULAR_SOCKET_INFO_STATUS;
    QueryInfoRequest.Result = SocketStatus;

    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, QuerySocketInfo, (void*) &QueryInfoRequest, sizeof(QueryInfoRequest));
}

Retcode_T CellularSocket_GetStatus(CellularSocket_Handle_T socket, CellularSocket_Status_T *SocketStatus)
{
    if (NULL == socket || NULL == SocketStatus)
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);

    CellularSocket_Type_T SocketType = CELLULAR_SOCKET_TYPE_MAX;
    CellularSocket_QueryInfoRequest_T QueryInfoRequest;

    QueryInfoRequest.Socket = (CellularSocket_T*) socket;
    QueryInfoRequest.Parameter = CELLULAR_SOCKET_INFO_TYPE;
    QueryInfoRequest.Result = &SocketType;

    Retcode_T retcode = CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, QuerySocketInfo, (void*) &QueryInfoRequest, sizeof(QueryInfoRequest));

    if (RETCODE_OK == retcode)
    {
        switch (*((CellularSocket_Type_T*) QueryInfoRequest.Result))
        {
        case CELLULAR_SOCKET_TYPE_TCP:
            case CELLULAR_SOCKET_TYPE_UDP:
            /* OK, socket seems to exist ... now check if its connected */
            *SocketStatus = CELLULAR_SOCKET_STATUS_OPEN;

            uint32_t ip;
            uint16_t port;

            CellularSocket_RemoteIpPort_T IpPort;
            IpPort.Ip = &ip;
            IpPort.Port = &port;
            QueryInfoRequest.Socket = (CellularSocket_T*) socket;
            QueryInfoRequest.Parameter = CELLULAR_SOCKET_INFO_REMOTE_IP_PORT;
            QueryInfoRequest.Result = &IpPort;
            retcode = CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, QuerySocketInfo, (void*) &QueryInfoRequest, sizeof(QueryInfoRequest));

            if (RETCODE_OK == retcode)
            {
                /* port 0 is a valid port, so don't check port */
                if (0 != *IpPort.Ip)
                {
                    *SocketStatus = CELLULAR_SOCKET_STATUS_CONNECTED;
                }
            }
            break;
        default: /* this should never happen */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
            break;
        }
    }
    else if (Retcode_GetCode(retcode) == (uint32_t) RETCODE_CELLULAR_RESPONDED_ERROR)
    {
        *SocketStatus = CELLULAR_SOCKET_STATUS_CLOSED;

        /* this is not an error, so reset retcode */
        retcode = RETCODE_OK;
    }

    return retcode;
}

Retcode_T CellularSocket_Connect(CellularSocket_Handle_T socket, uint32_t IpAddress, uint16_t port)
{
    if (NULL == socket)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularSocket_ConnectRequest_T ConnectRequest;
    ConnectRequest.Socket = (CellularSocket_T *) socket;
    ConnectRequest.IpAddress = IpAddress;
    ConnectRequest.Port = port;

    return CellularRequest_Dispatch(CELLULAR_SOCKET_ENQUEUE_TIMEOUT, ConnectSocket, (void*) &ConnectRequest, sizeof(ConnectRequest));
}

Retcode_T CellularSocket_Disconnect(CellularSocket_Handle_T socket)
{
    BCDS_UNUSED(socket);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
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
Retcode_T CellularSocketUrc_UUSOCL(void)
{
    Retcode_T result = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    Retcode_T retcode;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) "UUSOCL", strlen("UUSOCL"));

    uint8_t* buffer = NULL;
    uint32_t BufferLength = 0;
    int32_t id = 0xFF;

    if (RETCODE_OK == retcode)
    {
        LOG_INFO("URC for UUSOCL");

        /* wait for the rest of the URC (we expect the socket id to as a CmdArg) */
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLength);
    }

    /* parse the socket id from the CmdArg-buffer */
    if (RETCODE_OK == retcode)
    {
        retcode = CellularUtils_StrtolBounds(buffer, BufferLength, &id, 0, CELLULAR_SOCKET_MAX_COUNT - 1);
        AtResponseQueue_MarkBufferAsUnused();
    }

    if (RETCODE_OK == retcode)
    {
        /* invalidify all sockets with the parsed id */
        for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; ++i)
        {
            if (SocketStore[i].SocketId == id && SocketStore[i].SocketValid)
            {
                if (NULL != SocketStore[i].StatusCallback)
                {
                    SocketStore[i].StatusCallback(SocketStore[i].SocketId, CELLULAR_UDP_EVENT_SOCKET_CLOSE);
                }
                SocketStore[i].SocketValid = false;
                SocketStore[i].StatusCallback = NULL;
            }
        }

        result = RETCODE_OK;
    }

    return result;
}

static Retcode_T CellularSocketUrc_UUSORD_UUSORF(const char *cmd)
{
    Retcode_T retcode;
    uint8_t *buffer;
    uint32_t BufferLen;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *) cmd, strlen(cmd));

    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    LOG_INFO("URC for %s", cmd);

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    // parse socket ID
    CellularUdpSocket_T SocketId;
    retcode = CellularUtils_StrtolBounds(buffer, BufferLen, &SocketId, 0, CELLULAR_SOCKET_MAX_COUNT - 1);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_SOCKET_URC_TIMEOUT, &buffer, &BufferLen);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    // parse length
    int32_t length;
    retcode = CellularUtils_StrtolBounds(buffer, BufferLen, &length, 1, INT32_MAX);
    AtResponseQueue_MarkBufferAsUnused();
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    CellularSocket_T *socket = GetSocket(SocketId);
    if (socket)
    {
        socket->Data.BytesAvailable = length;
    }

    return retcode;
}

Retcode_T CellularSocketUrc_UUSORD(void)
{
    return CellularSocketUrc_UUSORD_UUSORF("UUSORD");
}

Retcode_T CellularSocketUrc_UUSORF(void)
{
    return CellularSocketUrc_UUSORD_UUSORF("UUSORF");
}
