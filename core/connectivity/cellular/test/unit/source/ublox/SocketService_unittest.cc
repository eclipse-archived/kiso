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
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_SOCKET_SERVICE

#include "UBloxUtils_th.hh"
#include "Engine_th.hh"
#include "AT_UBlox_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#undef KISO_MODULE_ID
#include "SocketService.c"
}

class TS_SocketService : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
        RESET_FAKE(Engine_Dispatch);
        RESET_FAKE(At_Set_USORD);
        RESET_FAKE(Retcode_RaiseError);
        memset(Sockets, 0, sizeof(Sockets));
    }
};

static void HandleSocketClosed(CellularSocket_Handle_T closedSocket)
{
    KISO_UNUSED(closedSocket);
    std::cout << "Socket Closed";
}

static void HandleSocketDataReady(CellularSocket_Handle_T socket, uint32_t numBytesAvailable)
{
    KISO_UNUSED(socket);
    KISO_UNUSED(numBytesAvailable);
    std::cout << "Data Ready";
}

static void notifySocketAccepted(CellularSocket_Handle_T acceptedSocket, const Cellular_IpAddress_T *remoteIp, uint16_t remotePort, CellularSocket_Handle_T listeningSocket)
{
    KISO_UNUSED(acceptedSocket);
    KISO_UNUSED(remoteIp);
    KISO_UNUSED(remotePort);
    KISO_UNUSED(listeningSocket);
    std::cout << "Socket accepted";
}

static Retcode_T Engine_Dispatch_fakedfunc(CellularRequest_CallableFunction_T function, uint32_t timeout, void *parameter, uint32_t ParameterLength)
{
    KISO_UNUSED(timeout);

    Retcode_T retcode = function(parameter, ParameterLength);

    return retcode;
}

/*######################################################################################################################
 * Testing CellularSocket_CreateAndBind()
######################################################################################################################*/
TEST_F(TS_SocketService, CellularSocket_CreateAndBind_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SocketService, CellularSocket_CreateAndBind_OutofRessources)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;
    Retcode_T retcode;
    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    for (uint32_t i = 0; i < CELLULAR_SOCKET_COUNT; i++)
    {
        retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
        EXPECT_EQ(RETCODE_OK, retcode);
    }
    retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES), retcode);
}

TEST_F(TS_SocketService, CellularSocket_CreateAndBind_TCP_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_TCP, HandleSocketClosed, HandleSocketDataReady);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SocketService, CellularSocket_CreateAndBind_Invalid_context)
{
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, NULL, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_CreateAndBind_Invalid_Parameter_socket)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    Retcode_T retcode = CellularSocket_CreateAndBind(NULL, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_CreateAndBind_Invalid_DataReadyHandler)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_CreateAndBind_Invalid_CLosedHandler)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, NULL, HandleSocketDataReady);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_CreateAndBind_Invalid_Context_type)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_EXTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_CreateAndBind_Invalid_protocol)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;
    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, (enum CellularSocket_Protocol_E)2, HandleSocketClosed, HandleSocketDataReady);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR), retcode);
}

/*######################################################################################################################
 * Testing CellularSocket_Connect()
######################################################################################################################*/
TEST_F(TS_SocketService, CellularSocket_Connect_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SocketService, CellularSocket_Connect_Invalid_AddressType)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_MAX;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_Connect_Invalid_Socket)
{
    Retcode_T retcode;
    CellularSocket_Handle_T socket = (CellularSocket_Handle_T)CELLULAR_SOCKET_COUNT + 1;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_Connect_Inalid_RemoteIP)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, NULL, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

/*######################################################################################################################
 * Testing CellularSocket_Send()
######################################################################################################################*/
TEST_F(TS_SocketService, CellularSocket_Send_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_Send(socket, (const uint8_t *)dataToSend, strlen(dataToSend));
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SocketService, CellularSocket_Send_InvalidDataLength)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_Send(socket, (const uint8_t *)dataToSend, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_Send_NullData)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_Send(socket, NULL, strlen(dataToSend));
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_Send_NotCreatedLocally)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    Sockets[(uint32_t)socket].IsCreatedLocally = false;

    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_Send(socket, (const uint8_t *)dataToSend, strlen(dataToSend));
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_Send_NotValidOnModem)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    Sockets[(uint32_t)socket].IsCreatedOnModem = false;

    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_Send(socket, (const uint8_t *)dataToSend, strlen(dataToSend));
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

/*######################################################################################################################
 * Testing CellularSocket_SendTo()
######################################################################################################################*/
TEST_F(TS_SocketService, CellularSocket_SendTo_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;
    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_SendTo(socket, (const uint8_t *)dataToSend, strlen(dataToSend), &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SocketService, CellularSocket_SendTo_InvalidDataLength)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;
    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_SendTo(socket, (const uint8_t *)dataToSend, 0, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_SendTo_NullData)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;
    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_SendTo(socket, NULL, strlen(dataToSend), &remoteIp, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_SendTo_NotCreatedLocally)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;
    Sockets[(uint32_t)socket].IsCreatedLocally = false;
    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_SendTo(socket, (const uint8_t *)dataToSend, strlen(dataToSend), &remoteIp, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_SendTo_NotValidOnModem)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;
    Sockets[(uint32_t)socket].IsCreatedOnModem = false;
    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_SendTo(socket, (const uint8_t *)dataToSend, strlen(dataToSend), &remoteIp, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_SendTo_InvalidIP)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint16_t remotePort = 8080;
    Sockets[(uint32_t)socket].IsCreatedOnModem = false;
    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_SendTo(socket, (const uint8_t *)dataToSend, strlen(dataToSend), NULL, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_SendTo_InvalidIPType)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_MAX;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;
    Sockets[(uint32_t)socket].IsCreatedOnModem = false;
    const char *dataToSend = "APP_TCP_TEST_DATA";

    retcode = CellularSocket_SendTo(socket, (const uint8_t *)dataToSend, strlen(dataToSend), &remoteIp, remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

/*######################################################################################################################
 * Testing CellularSocket_Listen()
######################################################################################################################*/
TEST_F(TS_SocketService, CellularSocket_Listen_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    Cellular_IpAddress_T localIp;
    localIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    localIp.Address.IPv4[0] = 1;
    localIp.Address.IPv4[1] = 0;
    localIp.Address.IPv4[2] = 0;
    localIp.Address.IPv4[3] = 127;
    uint16_t listeningPort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint16_t localPort = 3116;
    At_Set_USOLI_fake.return_val = RETCODE_OK;
    retcode = CellularSocket_Listen(socket, localPort, notifySocketAccepted);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(At_Set_USOLI_fake.call_count, 1U);
    //
    SocketService_NotifySocketAccepted(0, (AT_UBlox_Address_T *)&remoteIp, remotePort, 0, (AT_UBlox_Address_T *)&localIp, listeningPort);
    EXPECT_EQ(Retcode_RaiseError_fake.call_count, 0U);
}

TEST_F(TS_SocketService, CellularSocket_Listen_Fail_invalidSocket)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;
    uint16_t listeningPort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    Sockets[(uint32_t)socket].IsCreatedLocally = false;

    retcode = CellularSocket_Listen(socket, listeningPort, notifySocketAccepted);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

/*######################################################################################################################
 * Testing CellularSocket_Receive()
######################################################################################################################*/
TEST_F(TS_SocketService, CellularSocket_Receive_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint32_t rxBufLen = 10;
    uint8_t rxBuf[10];
    uint32_t bytesReceived = 0;

    retcode = CellularSocket_Receive(socket, rxBuf, rxBufLen, &bytesReceived);
    EXPECT_EQ(RETCODE_OK, retcode);
    SocketService_NotifySocketDataReceived(socket, bytesReceived);
    EXPECT_EQ(Retcode_RaiseError_fake.call_count, 0U);
}

TEST_F(TS_SocketService, CellularSocket_Receive_Invalid_socket1)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint32_t rxBufLen = 10;
    uint8_t rxBuf[10];
    uint32_t bytesReceived = 0;
    Sockets[(uint32_t)socket].IsCreatedLocally = false;
    retcode = CellularSocket_Receive(socket, rxBuf, rxBufLen, &bytesReceived);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_Receive_Invalid_socket2)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint32_t rxBufLen = 10;
    uint8_t rxBuf[10];
    uint32_t bytesReceived = 0;
    Sockets[(uint32_t)socket].IsCreatedOnModem = false;
    retcode = CellularSocket_Receive(socket, rxBuf, rxBufLen, &bytesReceived);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_Receive_Invalid_Buffer)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint32_t rxBufLen = 10;
    uint32_t bytesReceived = 0;
    Sockets[(uint32_t)socket].IsCreatedOnModem = false;
    retcode = CellularSocket_Receive(socket, NULL, rxBufLen, &bytesReceived);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_Receive_InvalidBufferLength)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint8_t rxBuf[10];
    uint32_t bytesReceived = 0;

    retcode = CellularSocket_Receive(socket, rxBuf, 0, &bytesReceived);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

/*######################################################################################################################
 * Testing CellularSocket_ReceiveFrom()
######################################################################################################################*/

TEST_F(TS_SocketService, CellularSocket_ReceiveFrom_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    uint16_t remotePort;

    uint32_t rxBufLen = 10;
    uint8_t rxBuf[10];
    uint32_t bytesReceived = 0;

    retcode = CellularSocket_ReceiveFrom(socket, rxBuf, rxBufLen, &bytesReceived, &remoteIp, &remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SocketService, CellularSocket_ReceiveFrom_Invalid_socket1)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    uint16_t remotePort;
    const uint32_t rxBufLen = 10;
    uint8_t rxBuf[10];
    uint32_t bytesReceived = 0;
    Sockets[(uint32_t)socket].IsCreatedLocally = false;

    retcode = CellularSocket_ReceiveFrom(socket, rxBuf, rxBufLen, &bytesReceived, &remoteIp, &remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_ReceiveFrom_Invalid_socket2)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    uint16_t remotePort = 8080;
    uint32_t rxBufLen = 10;
    uint8_t rxBuf[10];
    uint32_t bytesReceived;
    Sockets[(uint32_t)socket].IsCreatedOnModem = false;

    retcode = CellularSocket_ReceiveFrom(socket, rxBuf, rxBufLen, &bytesReceived, &remoteIp, &remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_ReceiveFrom_Invalid_Buffer)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    uint16_t remotePort;
    uint32_t rxBufLen = 10;
    uint32_t bytesReceived = 0;
    Sockets[(uint32_t)socket].IsCreatedOnModem = false;
    retcode = CellularSocket_ReceiveFrom(socket, NULL, rxBufLen, &bytesReceived, &remoteIp, &remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_ReceiveFrom_InvalidBufferLength)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    uint16_t remotePort;
    uint8_t rxBuf[10];
    uint32_t bytesReceived;

    retcode = CellularSocket_ReceiveFrom(socket, rxBuf, 0, &bytesReceived, &remoteIp, &remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_ReceiveFrom_InvalidRemoteIP)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint16_t remotePort;
    uint32_t rxBufLen = 10;
    uint8_t rxBuf[10];
    uint32_t bytesReceived = 0;

    retcode = CellularSocket_ReceiveFrom(socket, rxBuf, rxBufLen, &bytesReceived, NULL, &remotePort);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_ReceiveFrom_InvalidRemotePort)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    uint32_t rxBufLen = 10;
    uint8_t rxBuf[10];
    uint32_t bytesReceived = 0;

    retcode = CellularSocket_ReceiveFrom(socket, rxBuf, rxBufLen, &bytesReceived, &remoteIp, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

/*######################################################################################################################
 * Testing CellularSocket_Close()
######################################################################################################################*/
TEST_F(TS_SocketService, CellularSocket_Close_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = CellularSocket_Close(socket);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SocketService, CellularSocket_Close_Fail_invalidSocket)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);
    Sockets[(uint32_t)socket].IsCreatedLocally = false;
    retcode = CellularSocket_Close(socket);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

/*######################################################################################################################
 * Testing ()
######################################################################################################################*/

TEST_F(TS_SocketService, CellularSocket_QueryBytesAvailable_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);
    uint32_t numBytesAvailable = 0;

    retcode = CellularSocket_QueryBytesAvailable(socket, &numBytesAvailable);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SocketService, CellularSocket_QueryBytesAvailable_invalidSocket1)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);
    uint32_t numBytesAvailable = 0;
    Sockets[(uint32_t)socket].IsCreatedLocally = false;
    retcode = CellularSocket_QueryBytesAvailable(socket, &numBytesAvailable);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_QueryBytesAvailable_invalidSocket2)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);
    uint32_t numBytesAvailable = 0;
    Sockets[(uint32_t)socket].IsCreatedOnModem = false;
    retcode = CellularSocket_QueryBytesAvailable(socket, &numBytesAvailable);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_QueryBytesAvailable_invalidReference)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = CellularSocket_QueryBytesAvailable(socket, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_SocketService, CellularSocket_QueryBytesAvailable)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);
    uint32_t numBytesAvailable = 0;
    At_Set_USORD_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    retcode = CellularSocket_QueryBytesAvailable(socket, &numBytesAvailable);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);
}
/*######################################################################################################################
 * Testing SocketService_NotifySocketAccepted()
######################################################################################################################*/
TEST_F(TS_SocketService, SocketService_NotifySocketAccepted_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    Cellular_IpAddress_T localIp;
    localIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    localIp.Address.IPv4[0] = 1;
    localIp.Address.IPv4[1] = 0;
    localIp.Address.IPv4[2] = 0;
    localIp.Address.IPv4[3] = 127;
    uint16_t listeningPort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = CellularSocket_Listen(socket, listeningPort, notifySocketAccepted);
    EXPECT_EQ(RETCODE_OK, retcode);

    uint32_t createdSocketId = 123U;
    uint32_t listeningSocketId = Sockets[(uint32_t)socket].Id;

    SocketService_NotifySocketAccepted(createdSocketId, (AT_UBlox_Address_T *)&remoteIp, remotePort, listeningSocketId, (AT_UBlox_Address_T *)&localIp, listeningPort);
    EXPECT_EQ(Retcode_RaiseError_fake.call_count, 0U);
}

TEST_F(TS_SocketService, SocketService_NotifySocketAccepte_Fail)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    Cellular_IpAddress_T localIp;
    localIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    localIp.Address.IPv4[0] = 1;
    localIp.Address.IPv4[1] = 0;
    localIp.Address.IPv4[2] = 0;
    localIp.Address.IPv4[3] = 127;
    uint16_t listeningPort = 8080;
    uint32_t createdSocketId = 0;
    uint32_t listeningSocketId = 1;
    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    SocketService_NotifySocketAccepted(createdSocketId, (AT_UBlox_Address_T *)&remoteIp, remotePort, listeningSocketId, (AT_UBlox_Address_T *)&localIp, listeningPort);
    EXPECT_EQ(Retcode_RaiseError_fake.call_count, 1U);
}
/*######################################################################################################################
 * Testing SocketService_NotifySocketClosed()
######################################################################################################################*/
TEST_F(TS_SocketService, SocketService_NotifySocketClosed_Success)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = CellularSocket_Close(socket);
    EXPECT_EQ(RETCODE_OK, retcode);
    SocketService_NotifySocketClosed(Sockets[(uint32_t)socket].Id);
    EXPECT_EQ(Retcode_RaiseError_fake.call_count, 1U);
}

TEST_F(TS_SocketService, SocketService_NotifySocketClosed_Fail)
{
    static Cellular_DataContext_T DataContext;
    DataContext.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    CellularSocket_Handle_T socket;

    Engine_Dispatch_fake.custom_fake = Engine_Dispatch_fakedfunc;

    Retcode_T retcode = CellularSocket_CreateAndBind(&socket, &DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    EXPECT_EQ(RETCODE_OK, retcode);

    Cellular_IpAddress_T remoteIp;
    remoteIp.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    remoteIp.Address.IPv4[0] = 1;
    remoteIp.Address.IPv4[1] = 0;
    remoteIp.Address.IPv4[2] = 0;
    remoteIp.Address.IPv4[3] = 127;
    uint16_t remotePort = 8080;

    retcode = CellularSocket_Connect(socket, &remoteIp, remotePort);
    EXPECT_EQ(RETCODE_OK, retcode);

    SocketService_NotifySocketClosed(Sockets[(uint32_t)socket].Id);
    EXPECT_EQ(Retcode_RaiseError_fake.call_count, 0U);
}