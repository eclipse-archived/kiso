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

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START

extern "C"
{
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif /* __STDC_FORMAT_MACROS */

#define portNUM_CONFIGURABLE_REGIONS    1
#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_SOCKET

/* include faked interfaces */
#include "CellularModule.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "semphr_th.hh"
#include "BCDS_MCU_UART_th.hh"
#include "AtResponseParser_th.hh"
#include "AtResponseQueue_th.hh"
#include "Control_th.hh"
#include "Engine_th.hh"

#undef RETCODE
#define RETCODE(severity,code) ((Retcode_T) code)

#undef BCDS_MODULE_ID
#include "Utils.c"

#undef BCDS_MODULE_ID
#include "Socket.c"

#include "Log_th.hh"

const char *CmdArgCustomResp1[] =
        {
                "0", "1", "2", "3", "4", "5", "6", NULL
        };
const char *CmdArgCustomResp2[] =
        {
                "0", "1", "\"A5\"", NULL
        };
const char *CmdArgCustomResp3[] =
        {
                "0", "0", "1", NULL
        };
const char *CmdArgCustomResp4[] =
        {
                "0", "0", "\"192.168.1.2\"", "1234", "4", "\"02468ACE\"", NULL
        };
const char *CmdArgCustomResp5[] =
        {
                "\"192.168.1.2\"", "1234", NULL
        };
const char *CmdArgCustomResp6[] =
        {
                "17", "\"192.168.1.2\"", "1234", NULL
        };
const char *CmdArgCustomResp7[] =
        {
                "\"1.2\"", "1234", NULL
        };
const char *CmdArgCustomResp8[] =
        {
                "0", "9999999999", NULL
        };
const char *CmdArgCustomResp9[] =
        {
                "0", "\"192.168.1.2\"", NULL
        };
const char *CmdArgCustomResp10[] =
        {
                "0", "\"192.168.1.2\"", "1234", NULL
        };
const char *CmdArgCustomResp11[] =
        {
                "0", "\"192.168.1.2\"", "1234", "4", NULL
        };
const char *CmdArgCustomResp12[] =
        {
                "0", NULL
        };
const char *CmdArgCustomResp14[] =
        {
                "0", "0", "\"192.168.1.2\"", "1234", "4", "\"02468AC\"", NULL
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

static Retcode_T RequestDispatchCustom(uint32_t timeout,
        CellularRequest_CallableFunction_T function, void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(timeout);
    return function(parameter, ParameterLength);
}

static void SocketCb(CellularUdpSocket_T socket, CellularUdpEvent_T event)
{
    BCDS_UNUSED(socket);
    BCDS_UNUSED(event);
}

static Retcode_T WaitArbitaryResponseCustom(uint32_t timeout, AtResponseCode_T *response)
{
    BCDS_UNUSED(timeout);

    *response = AT_RESPONSE_CODE_OK;
    return RETCODE_OK;
}

static Retcode_T sendAtCmdWaitEcho_Custom(uint32_t timeout, const char *str)
{
    (void) CellularEngine_SendAtCommand((uint8_t *) str, strlen(str));
    return AtResponseQueue_WaitForNamedCmdEcho(timeout, (uint8_t *) str, strlen(str) - 2);
}

} //-- extern "C"

/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

class CellularSocket: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE(CellularEngine_SendAtCommand);
        RESET_FAKE(AtResponseQueue_WaitForNamedCmdEcho);
        RESET_FAKE(AtResponseQueue_WaitForNamedCmd);
        RESET_FAKE(AtResponseQueue_WaitForArbitraryCmdArg);
        RESET_FAKE(AtResponseQueue_WaitForArbitraryResponseCode);
        RESET_FAKE(AtResponseQueue_WaitForNamedResponseCode);
        RESET_FAKE(AtResponseQueue_WaitForMiscContent);
        RESET_FAKE(CellularRequest_Dispatch);
        RESET_FAKE(xSemaphoreTake);

        CellularRequest_Dispatch_fake.custom_fake = RequestDispatchCustom;
        AtResponseQueue_WaitForArbitraryResponseCode_fake.custom_fake =
                WaitArbitaryResponseCustom;
        AtResponseQueue_WaitForArbitraryCmdArg_fake.custom_fake = WaitArbitraryCmdArgCustom;
        AtResponseQueue_WaitForMiscContent_fake.custom_fake = WaitArbitraryCmdArgCustom;
        CellularEngine_SendAtCmdWaitEcho_fake.custom_fake = sendAtCmdWaitEcho_Custom;
    }

    virtual void TearDown()
    {
        ;
    }
};

/* specify test cases ******************************************************* */

TEST_F(CellularSocket, TestCellularOpenSocket_Fail)
{
    CellularUdpSocket_T socket;

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(NULL, CELLULAR_SOCKET_TYPE_TCP, SocketCb));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_MAX, SocketCb));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_TCP, NULL));
    EXPECT_EQ(RETCODE_NOT_SUPPORTED, CellularSocket_Open(&socket, (CellularSocket_Type_T) 2, SocketCb));

    CmdArgCustomResp = CmdArgCustomResp6;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_TCP, SocketCb));
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_TCP, SocketCb));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_TCP, SocketCb));
}

TEST_F(CellularSocket, TestCellularOpenSocket_Success)
{
    CellularUdpSocket_T socket;

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp1;

    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
        EXPECT_EQ(i, (uint32_t)socket);
    }

    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT,
            AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    /* test socket count limit */
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularSocket_Open(&socket,
            CELLULAR_SOCKET_TYPE_UDP, SocketCb));
}

TEST_F(CellularSocket, TestCellularCloseSocket)
{
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Close(CELLULAR_SOCKET_MAX_COUNT));

    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        EXPECT_EQ(RETCODE_OK, CellularSocket_Close((CellularUdpSocket_T)i));
    }

    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    CmdArgCustomResp = CmdArgCustomResp1;
    CellularUdpSocket_T socket;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_UDP, SocketCb));

    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Close((CellularUdpSocket_T)0));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Close((CellularUdpSocket_T)0));

}

TEST_F(CellularSocket, TestCellularSocketSend)
{
    uint8_t buffer[1];
    uint32_t BufferLength = sizeof(buffer);
    CellularUdpSocket_T SocketId;

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&SocketId, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Send(NULL, buffer, BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Send(socket, NULL, BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Send(socket, buffer, 0));

    if (NULL != socket)
    {
        socket->SocketValid = false;
        EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Send(socket, buffer, BufferLength));
        socket->SocketValid = true;
    }
    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Send(socket, buffer, BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Send(socket, buffer, BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Send(socket, buffer, BufferLength));
    CmdArgCustomResp = CmdArgCustomResp8;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_Send(socket, buffer, BufferLength));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp2;

    EXPECT_EQ(RETCODE_OK, CellularSocket_Send(socket, buffer, BufferLength));
    EXPECT_EQ(17U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(7U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(11U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(12U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    /*Test more failing scenarios*/
    AtResponseQueue_WaitForNamedResponseCode_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Send(socket, buffer, BufferLength));
    AtResponseQueue_IgnoreEvent_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Send(socket, buffer, BufferLength));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Send(socket, buffer, BufferLength));
}

TEST_F(CellularSocket, TestCellularSocketReceive)
{
    uint8_t buffer[1] =
    {   0};
    uint32_t BufferLength = sizeof(buffer);
    uint32_t BytesRead = sizeof(buffer);
    CellularUdpSocket_T SocketId;

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&SocketId, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Receive(NULL, buffer, BufferLength, &BytesRead,
            NULL, NULL));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Receive(socket, NULL, BufferLength, &BytesRead,
            NULL, NULL));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Receive(socket, buffer, 0, &BytesRead, NULL,
            NULL));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Receive(socket, buffer, BufferLength, NULL,
            NULL, NULL));
    BufferLength = 129;
    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead,
            NULL, NULL));
    BufferLength = sizeof(buffer);
    CmdArgCustomResp = CmdArgCustomResp1;
    ++CmdArgCustomResp = CmdArgCustomResp;
    ++CmdArgCustomResp = CmdArgCustomResp;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_Receive(socket, buffer, BufferLength,
            &BytesRead, NULL, NULL));
    CmdArgCustomResp = CmdArgCustomResp2;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead, NULL,
            NULL));
    CmdArgCustomResp = CmdArgCustomResp3;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead,
            NULL, NULL));
    CmdArgCustomResp = CmdArgCustomResp2;
    ++CmdArgCustomResp;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead,
            NULL, NULL));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead,
            NULL, NULL));
    CmdArgCustomResp = CmdArgCustomResp8;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_Receive(socket, buffer, BufferLength,
            &BytesRead, NULL, NULL));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp2;

    EXPECT_EQ(RETCODE_OK, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead, NULL, NULL));
    EXPECT_EQ(9U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(9U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(9U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(18U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(9U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    /* check data */
    EXPECT_EQ(0xA5, buffer[0]);
    EXPECT_EQ(1U, BytesRead);

    /*Test more failing scenarios*/
    CmdArgCustomResp = CmdArgCustomResp2;
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead, NULL,
            NULL));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead, NULL,
            NULL));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Receive(socket, buffer, BufferLength, &BytesRead, NULL,
            NULL));
    CmdArgCustomResp = NULL;

}

TEST_F(CellularSocket, TestCellularSocketSendTo)
{
    uint32_t ip = 0;
    uint16_t port = 0;
    uint8_t buffer[1];
    uint32_t BufferLength = sizeof(buffer);
    CellularUdpSocket_T socket = 0;

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(CELLULAR_SOCKET_MAX_COUNT, ip, port, buffer,
            BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(socket, ip, port, NULL, BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(socket, ip, port, buffer, 0));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_SendTo(socket, ip, port, buffer,
            BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo((CellularUdpSocket_T) 2, ip, port, buffer,
            BufferLength));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    EXPECT_EQ(RETCODE_OK, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    EXPECT_EQ(4U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(6U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    /*Test more failing scenarios*/
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    CmdArgCustomResp = CmdArgCustomResp8;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_SendTo(socket, ip, port, buffer,
            BufferLength));
    CmdArgCustomResp = CmdArgCustomResp12;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_SendTo(socket, ip, port, buffer,
            BufferLength));
    CmdArgCustomResp = CmdArgCustomResp8;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_SendTo(socket, ip, port, buffer,
            BufferLength));
    CmdArgCustomResp = CmdArgCustomResp3;
    ++CmdArgCustomResp;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_SendTo(socket, ip, port, buffer,
            BufferLength));

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    Cellular_EnableEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_OK;

    EXPECT_EQ(RETCODE_OK, CellularSocket_Close(socket));
}

TEST_F(CellularSocket, TestCellularSocketReceiveFrom)
{
    const uint8_t data[] =
    {   0x02, 0x46, 0x8A, 0xCE};
    uint32_t ip;
    uint16_t port;
    uint8_t *buffer;
    uint32_t BufferLength;
    CellularUdpSocket_T SocketId = 0;

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(CELLULAR_SOCKET_MAX_COUNT, &ip, &port,
            &buffer, &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, NULL, &port, &buffer,
            &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, NULL, &buffer,
            &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, NULL,
            &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer, NULL));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
            &BufferLength));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp4;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&SocketId, CELLULAR_SOCKET_TYPE_UDP, SocketCb));

    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);
    if (NULL != socket)
    {
        socket->Data.BytesAvailable = sizeof(data);
    }
    EXPECT_EQ(RETCODE_OK, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer, &BufferLength));
    EXPECT_EQ(0U, socket->Data.BytesAvailable);

    /* check data */
    EXPECT_EQ((uint32_t)((192 << 24) | (168 << 16) | (1 << 8) | 2), ip);
    EXPECT_EQ(1234, port);
    EXPECT_EQ(0, memcmp(buffer, data, sizeof(data)));
    EXPECT_EQ(BufferLength, sizeof(data));

    if (NULL != socket)
    {
        socket->Data.BytesAvailable = sizeof(data);
        CellularSocket_NotifyDataReceived();

        /*Test more failing scenarios*/
        EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
                &BufferLength));
        CmdArgCustomResp = CmdArgCustomResp8;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_ReceiveFrom(SocketId, &ip, &port,
                &buffer, &BufferLength));
        CmdArgCustomResp = CmdArgCustomResp8;
        ++CmdArgCustomResp;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_ReceiveFrom(SocketId, &ip, &port,
                &buffer, &BufferLength));
        CmdArgCustomResp = CmdArgCustomResp12;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_ReceiveFrom(SocketId, &ip, &port,
                &buffer, &BufferLength));
        CmdArgCustomResp = CmdArgCustomResp9;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_ReceiveFrom(SocketId, &ip, &port,
                &buffer, &BufferLength));
        CmdArgCustomResp = CmdArgCustomResp10;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_ReceiveFrom(SocketId, &ip, &port,
                &buffer, &BufferLength));
        CmdArgCustomResp = CmdArgCustomResp11;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_ReceiveFrom(SocketId, &ip, &port,
                &buffer, &BufferLength));
        CmdArgCustomResp = CmdArgCustomResp14;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_ReceiveFrom(SocketId, &ip, &port,
                &buffer, &BufferLength));
        EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
                &BufferLength));

        AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_FAILURE, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
                &BufferLength));
        AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
        socket->Data.BytesAvailable = sizeof(data);
        EXPECT_EQ(RETCODE_FAILURE, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
                &BufferLength));
        CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
        socket->Data.BytesAvailable = sizeof(data);
    }

    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
            &BufferLength));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_OK;
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_OK;

    EXPECT_EQ(RETCODE_OK, CellularSocket_Close(SocketId));
}

TEST_F(CellularSocket, TestCellularGetSocketPeer)
{
    uint32_t ip;
    uint16_t port;

    CellularUdpSocket_T SocketId;

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&SocketId, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_GetPeer(NULL, &ip, &port));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_GetPeer(socket, NULL, &port));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_GetPeer(socket, &ip, NULL));
    CmdArgCustomResp = CmdArgCustomResp7;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_GetPeer(socket, &ip, &port));
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_GetPeer(socket, &ip, &port));
    CmdArgCustomResp = CmdArgCustomResp5;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocket_GetPeer(socket, &ip, &port));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp5;

    EXPECT_EQ(RETCODE_OK, CellularSocket_GetPeer(socket, &ip, &port));
    EXPECT_EQ(5U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(5U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(5U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(6U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(5U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

}

TEST_F(CellularSocket, TestCellularGetSocketTcpStatus)
{
    CellularSocket_TcpStatus_T status;
    CellularUdpSocket_T SocketId;

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&SocketId, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_GetTcpStatus(NULL, &status));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_GetTcpStatus(socket, NULL));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp2;

    EXPECT_EQ(RETCODE_OK, CellularSocket_GetTcpStatus(socket, &status));
    EXPECT_EQ(2U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);
}

TEST_F(CellularSocket, TestCellularGetSocketStatus)
{
    CellularSocket_Status_T status;
    CellularUdpSocket_T SocketId;

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&SocketId, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_GetStatus(NULL, &status));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_GetStatus(socket, NULL));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp6;

    EXPECT_EQ(RETCODE_OK, CellularSocket_GetStatus(socket, &status));
    EXPECT_EQ(3U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(3U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(3U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(4U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(3U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    /* test more failing scenarios*/
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_GetStatus(socket, &status));
    AtResponseQueue_WaitForNamedCmdArg_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_GetStatus(socket, &status));
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_GetStatus(socket, &status));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_GetStatus(socket, &status));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_GetStatus(socket, &status));
}

TEST_F(CellularSocket, TestCellularSocketConnect)
{
    CellularUdpSocket_T SocketId;

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&SocketId, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Connect(NULL, 0, 0));

    /* test passing scenario */
    EXPECT_EQ(RETCODE_OK, CellularSocket_Connect(&socket, 0, 0));
    EXPECT_EQ(2U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryResponseCode_fake.call_count);

    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Connect(&socket, 0, 0));
}

TEST_F(CellularSocket, TestCellularSocketDisconnect)
{
    CellularUdpSocket_T SocketId;

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&SocketId, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);

    EXPECT_EQ(RETCODE_NOT_SUPPORTED, CellularSocket_Disconnect(socket));
}

TEST_F(CellularSocket, TestCellularSocketUrcUUSOCL)
{
    CellularUdpSocket_T socket;

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp1;

    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    EXPECT_EQ((uint32_t) 0, (uint32_t)socket);

    CmdArgCustomResp = CmdArgCustomResp1;

    EXPECT_EQ(RETCODE_OK, CellularSocketUrc_UUSOCL());
    EXPECT_EQ(RETCODE_OK, CellularSocketUrc_UUSOCL());
    EXPECT_EQ(3U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(3U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);

    /*Test failing scenarios*/
    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularSocketUrc_UUSOCL());
    CmdArgCustomResp = CmdArgCustomResp8;
    ++CmdArgCustomResp;
    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularSocketUrc_UUSOCL());
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularSocketUrc_UUSOCL());
}

TEST_F(CellularSocket, TestCellularSocketUrcUUSORD)
{
    CellularUdpSocket_T socket;

    CmdArgCustomResp = CmdArgCustomResp2;
    EXPECT_EQ(RETCODE_OK, CellularSocketUrc_UUSORD());

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&socket, CELLULAR_SOCKET_TYPE_UDP, SocketCb));
    EXPECT_EQ((uint32_t) 0, (uint32_t)socket);

    CmdArgCustomResp = CmdArgCustomResp2;
    EXPECT_EQ(RETCODE_OK, CellularSocketUrc_UUSORD());
    EXPECT_EQ(3U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(5U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);

    /*Test failing scenarios*/
    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_UUSORD());
    CmdArgCustomResp = CmdArgCustomResp12;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_UUSORD());
    CmdArgCustomResp = CmdArgCustomResp8;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_UUSORD());
    CmdArgCustomResp = CmdArgCustomResp5;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_UUSORD());
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularSocketUrc_UUSORD());
}

TEST_F(CellularSocket, TestCellularSocketUrcUUSORF)
{
    CmdArgCustomResp = CmdArgCustomResp2;

    EXPECT_EQ(RETCODE_OK, CellularSocketUrc_UUSORF());
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
}
