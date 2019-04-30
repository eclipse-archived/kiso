/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

#include <gtest.h>

FFF_DEFINITION_BLOCK_START

extern "C"
{
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif /* __STDC_FORMAT_MACROS */

#define portNUM_CONFIGURABLE_REGIONS    1
#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_SOCKET

#include "CellularModule.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "semphr_th.hh"
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

#define CELLULAR_SOCKET_TYPE_MAX   CELLULAR_SOCKET_TYPE_COUNT
#define RECEIVED_DATA   "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
const char *CmdArgCustomResp1[] =
        {
                "0", "0", "1", "0", "2", "0", "3", "0", "4", "0", "5", "0",
                "6", "0", "7", "0", "8", "0", "9", "0", "10", "0", "11", "0", NULL
        };
const char *CmdArgCustomResp2[] =
        {
                "0", "0", "\"recv\"", "0", "\"closed\"", "0", "\"pdpdeact\"",
                "1", "\"unhandled\"", "0", NULL
        };

const char *CmdArgCustomResp3[] =
        {
                "0", "0", "SEND OK", NULL
        };
const char *CmdArgCustomResp4[] =
        {
                "0", "0", "16", "192.168.1.2", "1234", "4", "192.168.1.2", "1234", NULL
        };
const char *CmdArgCustomResp5[] =
        {
                "1", "3", "1", NULL, "1", "576", "1", "568", "13", NULL
        };
const char *CmdArgCustomResp6[] =
        {
                "0", "1", "0", "SEND OK", "0", "3", "0", NULL
        };
const char ReceiveDataResp[] = "+QIRD: 16,\"192.168.1.2\",1234\r\n" RECEIVED_DATA "\nOK\n";
const char ReceiveDataResp2[] = "+QIRD: 4,\"192.168.1.2\",1234\r\n\x02\x46\x8A\xCE\nOK\n";
const char ReceiveDataResp3[] = "+QI>RD: 16,\"192.168.1.2\",1234\r\n" RECEIVED_DATA "\nOK\n";
const char ReceiveDataResp4[] = "STA> TEST";

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

static uint32_t CbCount = 0;

static void SocketCb(CellularUdpSocket_T socket, CellularUdpEvent_T event)
{
    BCDS_UNUSED(socket);
    BCDS_UNUSED(event);
    CbCount++;
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

static uint32_t UartReceiveCustomAttr;
static const void *UartReceiveCustomData;

static Retcode_T UartReceiveReturnVal;

Retcode_T Cellular_ReadDataCustom(uint8_t* data, uint32_t dataLength, uint32_t* dataRead)
{
    if (dataLength >= UartReceiveCustomAttr)
    {
        *dataRead = UartReceiveCustomAttr;
    }
    else
    {
        *dataRead = dataLength;
    }

    if (UartReceiveCustomAttr)
    {
        memcpy(data, UartReceiveCustomData, *dataRead);
        UartReceiveCustomData = (void *) ((uint32_t) UartReceiveCustomData + *dataRead);
        UartReceiveCustomAttr -= *dataRead;
    }

    return UartReceiveReturnVal;
}

static uint32_t XTickCounter = 0;

static uint32_t xTaskTickCount(void)
{
    XTickCounter += 1000;
    return XTickCounter;
}

} //-- extern "C"

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
        RESET_FAKE (xSemaphoreTake);

        RESET_FAKE (MCU_UART_Receive);
        RESET_FAKE (MCU_UART_GetRxCount);

        CellularRequest_Dispatch_fake.custom_fake = RequestDispatchCustom;
        AtResponseQueue_WaitForArbitraryResponseCode_fake.custom_fake =
                WaitArbitaryResponseCustom;
        AtResponseQueue_WaitForArbitraryCmdArg_fake.custom_fake = WaitArbitraryCmdArgCustom;
        AtResponseQueue_WaitForMiscContent_fake.custom_fake = WaitArbitraryCmdArgCustom;
        CellularEngine_SendAtCmdWaitEcho_fake.custom_fake = sendAtCmdWaitEcho_Custom;
        Cellular_ReadData_fake.custom_fake = Cellular_ReadDataCustom;
        xTaskGetTickCount_fake.custom_fake = xTaskTickCount;

        UartReceiveReturnVal = RETCODE_OK;
    }

    virtual void TearDown()
    {
        ;
    }
};

TEST_F(CellularSocket, TestCellularOpenSocket_Fail)
{
    CellularUdpSocket_T socket;
    CellularSocket_OpenRequest_T r;

    r.ContextId = CELLULAR_NETWORK_PROFILE_ID_DEFAULT;
    r.SocketId = &socket;
    r.StatusCallback = SocketCb;
    /* To comply with GSM API force configuration to UDP Service */
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(NULL));
    r.SocketId = NULL;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(&r));
    r.SocketId = &socket;

    CmdArgCustomResp = CmdArgCustomResp5+4;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(&r));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(&r));
    CmdArgCustomResp = CmdArgCustomResp5+2;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_Open(&r));

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Open(&r));

    AtResponseQueue_WaitForNamedResponseCode_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Open(&r));

    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_Open(&r));
}

TEST_F(CellularSocket, TestCellularOpenSocket_Success)
{
    /* test passing scenario */
    CellularUdpSocket_T socket;
    CmdArgCustomResp = CmdArgCustomResp1;
    CellularSocket_OpenRequest_T r;

    r.ContextId = CELLULAR_NETWORK_PROFILE_ID_DEFAULT;
    r.SocketId = &socket;
    r.StatusCallback = SocketCb;
    /* To comply with GSM API force configuration to UDP Service */
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;

    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    EXPECT_EQ(0U, (uint32_t)socket);
    r.DomainName = NULL;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    EXPECT_EQ(1U, (uint32_t)socket);
    r.LocalPort = 1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    EXPECT_EQ(2U, (uint32_t)socket);
    r.DomainName = "127.0.0.1";
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    EXPECT_EQ(3U, (uint32_t)socket);
    r.LocalPort = 0;

    for (uint32_t i = 4U; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    }

    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(2 * CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ(CELLULAR_SOCKET_MAX_COUNT, AtResponseQueue_WaitForNamedResponseCode_fake.call_count);

    /* test socket count limit */
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularSocket_Open(&r));
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

}

TEST_F(CellularSocket, TestSendSocketDataParser)
{
    EXPECT_EQ(RETCODE_INVALID_PARAM, SendSocketDataParser(NULL));

    AtResponseParserArgument_T arg;
    CellularUdpSocket_T SocketId = 0;
    uint32_t ip = 0;
    uint16_t port = 0;
    uint8_t buffer[1];
    uint32_t BufferLength = sizeof(buffer);
    CellularSocket_OpenRequest_T r;
    r.SocketId = &SocketId;
    r.StatusCallback = SocketCb;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;

    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    EXPECT_EQ(RETCODE_OK, CellularSocket_SendTo(SocketId, ip, port, buffer, BufferLength));

    EXPECT_EQ(RETCODE_UNEXPECTED_BEHAVIOR, SendSocketDataParser(&arg));

    UartReceiveCustomData = ReceiveDataResp3;
    UartReceiveCustomAttr = sizeof(ReceiveDataResp3);

    EXPECT_EQ(RETCODE_UNEXPECTED_BEHAVIOR, SendSocketDataParser(&arg));

    UartReceiveCustomData = ReceiveDataResp4;
    UartReceiveCustomAttr = sizeof(ReceiveDataResp4)-1;
    EXPECT_EQ(RETCODE_UNEXPECTED_BEHAVIOR, SendSocketDataParser(&arg));
}

TEST_F(CellularSocket, TestReceiveSocketDataParser)
{
    EXPECT_EQ(RETCODE_INVALID_PARAM, ReceiveSocketDataParser(NULL));

    AtResponseParserArgument_T arg = { NULL, 0 };

    SocketReceiving = NULL;

    EXPECT_EQ(RETCODE_INVALID_PARAM, ReceiveSocketDataParser(&arg));
    UartReceiveCustomData = ReceiveDataResp;
    UartReceiveCustomAttr = sizeof(ReceiveDataResp);

    CellularUdpSocket_T SocketId = 0;
    CellularSocket_OpenRequest_T r;
    r.SocketId = &SocketId;
    r.StatusCallback = SocketCb;
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;

    arg.Device = (UART_T) 1;
    EXPECT_EQ(RETCODE_INVALID_PARAM, ReceiveSocketDataParser(&arg));
    arg.Semaphore = (SemaphoreHandle_t) 1;
    EXPECT_EQ(RETCODE_INVALID_PARAM, ReceiveSocketDataParser(&arg));

    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);
    if(socket != NULL)
    {
        socket->Data.BytesAvailable = UartReceiveCustomAttr;
    }

    SocketReceiving = socket;

    EXPECT_EQ(RETCODE_OK, ReceiveSocketDataParser(&arg));
    EXPECT_EQ(RETCODE_OK, CellularSocket_Close(SocketId));

    CellularUdpSocket_T SocketId1 = 1;
    CmdArgCustomResp = CmdArgCustomResp3;
    r.SocketId = &SocketId1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    CellularSocket_T *socket1 = GetSocket(SocketId1);
    EXPECT_NE((CellularSocket_T *) NULL, socket1);

    SocketReceiving = socket1;
    UartReceiveCustomData = "QIRD: 16,\"192.168.1.2\",1234\r\n" RECEIVED_DATA "\nOK\n";
    UartReceiveCustomAttr = sizeof("QIRD: 16,\"192.168.1.2\",1234\r\n" RECEIVED_DATA "\nOK\n");

    xSemaphoreTake_fake.return_val = pdPASS;
    EXPECT_EQ(RETCODE_OK, ReceiveSocketDataParser(&arg));

    UartReceiveReturnVal = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, ReceiveSocketDataParser(&arg));

    UartReceiveCustomData = ReceiveDataResp4;
    UartReceiveCustomAttr = sizeof(ReceiveDataResp4);
    EXPECT_EQ(RETCODE_FAILURE, ReceiveSocketDataParser(&arg));
    xSemaphoreTake_fake.return_val = pdFAIL;
    EXPECT_EQ(RETCODE_FAILURE, ReceiveSocketDataParser(&arg));

    EXPECT_EQ(RETCODE_OK, CellularSocket_Close(SocketId1));
    CmdArgCustomResp = CmdArgCustomResp3;
}

TEST_F(CellularSocket, TestCellularSocketNotifyDataReceived)
{
    CellularUdpSocket_T SocketId;
    const uint8_t data[] =
    {   0x02, 0x46, 0x8A, 0xCE};
    CbCount = 0;

    CellularSocket_OpenRequest_T r;
    r.SocketId = &SocketId;
    r.StatusCallback = SocketCb;
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;

    CmdArgCustomResp = CmdArgCustomResp1;

    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));

    CellularSocket_NotifyDataReceived();
    EXPECT_EQ(0U, CbCount);

    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL,socket);
    if(socket != NULL)
    {
        socket->Data.BytesAvailable = sizeof(data);
    }
    CellularSocket_NotifyDataReceived();
    EXPECT_EQ(1U, CbCount);

    EXPECT_EQ(RETCODE_OK, CellularSocket_Close(SocketId));

}

TEST_F(CellularSocket, TestCellularSocketSendTo)
{
    uint32_t ip = 0;
    uint16_t port = 0;
    uint8_t buffer[1];
    uint32_t BufferLength = sizeof(buffer);
    CellularUdpSocket_T socket = 0;

    CellularSocket_OpenRequest_T r;
    r.SocketId = &socket;
    r.StatusCallback = SocketCb;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(CELLULAR_SOCKET_MAX_COUNT, ip, port, buffer,
                    BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(socket, ip, port, NULL, BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(socket, ip, port, buffer, 0));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo((CellularUdpSocket_T) NULL,ip, port, buffer,
                    BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_SendTo(CELLULAR_SOCKET_MAX_COUNT-1, ip, port, buffer,
                    BufferLength));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp3;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    EXPECT_EQ(RETCODE_OK, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));

    CmdArgCustomResp = CmdArgCustomResp3;
    r.SocketType = CELLULAR_SOCKET_TYPE_TCP_LISTENER;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    EXPECT_EQ(RETCODE_OK, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));

    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    AtResponseQueue_WaitForNamedResponseCode_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_SendTo(socket, ip, port, buffer, BufferLength));

    EXPECT_EQ(10U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForMiscContent_fake.call_count);
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

    CellularSocket_OpenRequest_T r;
    r.SocketId = &SocketId;
    r.StatusCallback = SocketCb;
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(CELLULAR_SOCKET_MAX_COUNT, &ip, &port,
                    &buffer, &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(CELLULAR_SOCKET_MAX_COUNT-1, &ip, &port,
                    &buffer, &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, NULL, &port, &buffer,
                    &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, NULL, &buffer,
                    &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, NULL,
                    &BufferLength));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
                    NULL));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
                    &BufferLength));

    /* test passing scenario */
    CmdArgCustomResp = CmdArgCustomResp4;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));

    CellularSocket_T *socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);
    if(socket != NULL)
    {
        socket->Data.BytesAvailable = sizeof(data);
    }

    xSemaphoreTake_fake.return_val = pdPASS;
    SocketReceiving = socket;
    uint32_t UnreadCount;

    AtResponseParserArgument_T arg;
    arg.Semaphore = (SemaphoreHandle_t) 1;
    arg.Device = (UART_T) 1;

    /* test long data */
    UartReceiveCustomData = ReceiveDataResp;
    UartReceiveCustomAttr = sizeof(ReceiveDataResp) - 1;
    UnreadCount = (uint32_t)(strchr(ReceiveDataResp, '\n') - ReceiveDataResp) + 1;

    EXPECT_EQ(RETCODE_OK, ReceiveSocketDataParser(&arg));
    EXPECT_EQ(RETCODE_OK, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer, &BufferLength));
    EXPECT_EQ(0U, socket->Data.BytesAvailable);
    EXPECT_EQ((uint32_t)((192 << 24) | (168 << 16) | (1 << 8) | 2), ip);
    EXPECT_EQ(1234, port);
    EXPECT_EQ(0, memcmp(buffer, RECEIVED_DATA, sizeof(RECEIVED_DATA) - 1));
    EXPECT_EQ(BufferLength, sizeof(RECEIVED_DATA) - 1);

    /* test short data */
    if(socket != NULL)
    {
        socket->Data.BytesAvailable = sizeof(data);
    }
    UartReceiveCustomData = ReceiveDataResp2;
    UartReceiveCustomAttr = UnreadCount = sizeof(ReceiveDataResp2) - 1;

    EXPECT_EQ(RETCODE_OK, ReceiveSocketDataParser(&arg));
    EXPECT_EQ(RETCODE_OK, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer, &BufferLength));
    if(socket != NULL)
    {
        EXPECT_EQ(0U, socket->Data.BytesAvailable);
    }

    /* check data */
    EXPECT_EQ((uint32_t)((192 << 24) | (168 << 16) | (1 << 8) | 2), ip);
    EXPECT_EQ(1234, port);
    EXPECT_EQ(0, memcmp(buffer, data, sizeof(data)));
    EXPECT_EQ(BufferLength, sizeof(data));

    /* Test more failing scenarios*/
    if(socket != NULL)
    {
        socket->Data.BytesAvailable = sizeof(data);
    }
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
                    &BufferLength));

    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_OK;
    AtResponseQueue_WaitForArbitraryResponseCode_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularSocket_ReceiveFrom(SocketId, &ip, &port, &buffer,
                    &BufferLength));

    AtResponseQueue_WaitForArbitraryResponseCode_fake.return_val = RETCODE_OK;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Close(SocketId));
}

TEST_F(CellularSocket, TestCellularSocketUrcQIURC)
{
    CmdArgCustomResp = CmdArgCustomResp2;
    CellularUdpSocket_T SocketId = 0;
    CellularSocket_OpenRequest_T r;
    r.SocketId = &SocketId;
    r.StatusCallback = SocketCb;
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;
    CellularSocket_T *socket;

    CellularSocket_InvalidateAll();
    socket = GetSocket(SocketId);
    EXPECT_EQ((CellularSocket_T *) NULL, socket);

    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);

    if(socket != NULL)
    {
        EXPECT_EQ(0U, socket->Data.BytesAvailable);
    }

    /* check urc "recv" */
    EXPECT_EQ(RETCODE_OK, CellularSocketUrc_QIURC());
    socket = GetSocket(SocketId);
    EXPECT_NE((CellularSocket_T *) NULL, socket);
    if(socket != NULL)
    {
        EXPECT_NE(0U, socket->Data.BytesAvailable);
    }

    /* check urc "closed" */
    EXPECT_EQ(RETCODE_OK, CellularSocketUrc_QIURC());
    socket = GetSocket(SocketId);
    EXPECT_EQ((CellularSocket_T *) NULL, socket);

    /* check urc "pdpdeact" */
    EXPECT_EQ(RETCODE_OK, CellularSocketUrc_QIURC());
    socket = GetSocket(SocketId);
    EXPECT_EQ((CellularSocket_T *) NULL, socket);

    /* check unhandled urc */
    CmdArgCustomResp = CmdArgCustomResp2+2;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_QIURC());

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_OK;
    CmdArgCustomResp = CmdArgCustomResp6;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_QIURC());
    socket = GetSocket(SocketId);
    EXPECT_EQ((CellularSocket_T *) NULL, socket);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_QIURC());
    socket = GetSocket(SocketId);
    EXPECT_EQ((CellularSocket_T *) NULL, socket);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_QIURC());
    socket = GetSocket(SocketId);
    EXPECT_EQ((CellularSocket_T *) NULL, socket);

    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularSocketUrc_QIURC());
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_OK;

    CmdArgCustomResp = NULL;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_QIURC());

    CmdArgCustomResp = CmdArgCustomResp2;
    EXPECT_EQ(RETCODE_CELLULAR_RESPONSE_UNEXPECTED, CellularSocketUrc_QIURC());

    CellularSocket_InvalidateAll();

}

TEST_F(CellularSocket, TestCellularSocketInvalidateAll)
{
    /* test passing scenario */
    CellularUdpSocket_T socket;
    CellularSocket_OpenRequest_T r;
    r.SocketId = &socket;
    r.StatusCallback = SocketCb;
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;
    CmdArgCustomResp = CmdArgCustomResp1;

    for (uint32_t i = 0; i < CELLULAR_SOCKET_MAX_COUNT; i++)
    {
        EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
        EXPECT_EQ(i, (uint32_t)socket);
    }

    /* test socket count limit */
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularSocket_Open(&r));

    EXPECT_EQ(RETCODE_OK, CellularSocket_Close((CellularUdpSocket_T)0));

    CellularSocket_InvalidateAll();

    CmdArgCustomResp = CmdArgCustomResp1;
    EXPECT_EQ(RETCODE_OK, CellularSocket_Open(&r));
    EXPECT_EQ(RETCODE_OK, CellularSocket_Close(socket));
}
