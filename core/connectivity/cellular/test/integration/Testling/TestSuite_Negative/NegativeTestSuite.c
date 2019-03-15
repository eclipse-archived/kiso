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

#include "BCDS_TestlingInfo.h"
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_NEGATIVE_TEST_SUITE

#include "BCDS_TestSuite.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include "CellularSupport.h"
#include "NegativeTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_NEGATIVE_ID

extern CellularUdpSocket_T SocketidLists[8];
extern SemaphoreHandle_t SemaphoreHandle; /* Semaphore assigned to a variable of type SemaphoreHandle_t  */

static Retcode_T SetupModemGPRSConnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        return (Retcode);
    }
    Retcode = CellularSupport_TurnOn(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        return (Retcode);
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        return (Retcode);
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        return (Retcode);
    }

    Retcode = CellularSupport_Connect(MessagePtr);
    return (Retcode);
}

static void RunNegativeConnectInvalidAPN(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    Retcode = CellularSupport_NetworkInit(MessagePtr);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }
    ConfigSetting.ApnName[0] = 'n';
    ConfigSetting.ApnName[1] = 'e';
    ConfigSetting.ApnName[2] = 't';
    ConfigSetting.ApnName[3] = '\0';
    ConfigSetting.ApnUserName[0] = 'w';
    ConfigSetting.ApnUserName[1] = '\0';
    ConfigSetting.ApnPassword[0] = 'w';
    ConfigSetting.ApnPassword[1] = '\0';

    Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeConnectAPNLengthExceed(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue <= NEGATIVE_TEST_ITERATION_VAL_100; CountValue++)
    {
        ConfigSetting.ApnName[CountValue] = 'n';
    }
    ConfigSetting.ApnName[100] = '\0';
    ConfigSetting.ApnUserName[0] = 'w';
    ConfigSetting.ApnUserName[1] = '\0';
    ConfigSetting.ApnPassword[0] = 'w';
    ConfigSetting.ApnPassword[1] = '\0';

    /* config. the modem */
    Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    /* Here,actually implemented the test case failure case */
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }

    Retcode = Cellular_Connect();

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeConnectInvalidUserName(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    ConfigSetting.ApnName[0] = 'w';
    ConfigSetting.ApnName[1] = 'w';
    ConfigSetting.ApnName[2] = 'w';
    ConfigSetting.ApnName[3] = '\0';
    ConfigSetting.ApnUserName[0] = 'g';
    ConfigSetting.ApnUserName[1] = '\0';
    ConfigSetting.ApnPassword[0] = 'w';
    ConfigSetting.ApnPassword[1] = '\0';

    /* config. the modem */
    Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }
    Retcode = CellularSupport_Connect(MessagePtr);

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeConnectUserNameExceed(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;

    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    ConfigSetting.ApnName[0] = 'w';
    ConfigSetting.ApnName[1] = 'w';
    ConfigSetting.ApnName[2] = 'w';
    ConfigSetting.ApnName[3] = '\0';

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue <= NEGATIVE_TEST_ITERATION_VAL_30; CountValue++)
    {
        ConfigSetting.ApnUserName[CountValue] = 'g';
    }
    ConfigSetting.ApnUserName[30] = '\0';
    ConfigSetting.ApnPassword[0] = 'w';
    ConfigSetting.ApnPassword[1] = '\0';

    /* config. the modem */
    Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    /* Making test case fail */
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }
    Retcode = Cellular_Connect();

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeConnectInvalidPassword(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    /* check the germany network */
    ConfigSetting.ApnName[0] = 'w';
    ConfigSetting.ApnName[1] = 'w';
    ConfigSetting.ApnName[2] = 'w';
    ConfigSetting.ApnName[3] = '\0';
    ConfigSetting.ApnUserName[0] = 'w';
    ConfigSetting.ApnUserName[1] = '\0';
    ConfigSetting.ApnPassword[0] = 't';
    ConfigSetting.ApnPassword[1] = '\0';

    /* config. the modem */
    Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }
    Retcode = CellularSupport_Connect(MessagePtr);

    /* if config is fail check the connect return status */
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeConnectPasswordLengthExceed(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    ConfigSetting.ApnName[0] = 'w';
    ConfigSetting.ApnName[1] = 'w';
    ConfigSetting.ApnName[2] = 'w';
    ConfigSetting.ApnName[3] = '\0';
    ConfigSetting.ApnUserName[0] = 'w';
    ConfigSetting.ApnUserName[1] = '\0';
    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue <= NEGATIVE_TEST_ITERATION_VAL_30; CountValue++)
    {
        ConfigSetting.ApnPassword[CountValue] = 't';
    }
    ConfigSetting.ApnPassword[30] = '\0';

    /* config. the modem */
    Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }
    Retcode = Cellular_Connect();

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeConnectNullCellularConfig(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue < NEGATIVE_TEST_ITERATION_VAL_5; CountValue++)
    {
        Retcode = CellularSupport_NetworkInit(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: NW init fail");
            return;
        }

        Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

        Retcode = Cellular_Connect();

        if (RETCODE_OK == Retcode)
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            Tests_SendReport(Retcode, "Fail: NW connect fail");
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeConnectCellularWithoutConfig(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    Retcode = Cellular_Connect();

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeDisConnectCellularWithoutConnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }

    Retcode = Cellular_Disconnect();

    if (RETCODE_OK == Retcode)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        Tests_SendReport(Retcode, "Fail: NW disconnect fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeConnectCellularTwice(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }
    Retcode = CellularSupport_Connect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }
    Retcode = Cellular_Connect();

    if (RETCODE_OK == Retcode)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeDisconnectCellularTwice(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }

    Retcode = CellularSupport_Connect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }

    Retcode = CellularSupport_DisConnect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW disconnect fail");
        return;
    }

    Retcode = Cellular_Disconnect();

    if (RETCODE_OK == Retcode)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        Tests_SendReport(Retcode, "Fail: NW disconnect fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeOffWithoutDisconnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }

    Retcode = CellularSupport_Connect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }

    Retcode = CellularSupport_TurnOff(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Turn off fail");
        return;
    }

    Retcode = CellularSupport_TurnOn(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Turn on fail");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }

    Retcode = CellularSupport_Connect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }

    Retcode = CellularSupport_DisConnect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW disconnect fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeSocket7TimesOpen(CCMsg_T* MessagePtr)
{
    CellularUdpSocket_T SocketIdReceived[CELLULAR_SOCKET_COUNT + NEGATIVE_TEST_COUNTER_INIT_VAL_3];
    Retcode_T Retcode = RETCODE_OK;
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;
    uint32_t OpenedSocketAlready = NEGATIVE_TEST_COUNTER_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue < CELLULAR_SOCKET_COUNT; CountValue++)
    {
        SocketIdReceived[CountValue] = -1;
        Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketIdReceived[CountValue]);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode,CellularSupport_StringAttach( "Fail: Socket Open or socket id invalid", CountValue));
            for (OpenedSocketAlready = NEGATIVE_TEST_COUNTER_INIT_VAL_0; OpenedSocketAlready < CountValue; OpenedSocketAlready++)
            {
                CellularSupport_SocketClose(MessagePtr, SocketIdReceived[OpenedSocketAlready]);
            }
            return;
        }
    }

    for (CountValue = CELLULAR_SOCKET_COUNT; CountValue < (CELLULAR_SOCKET_COUNT + NEGATIVE_TEST_COUNTER_INIT_VAL_3); CountValue++)
    {
        SocketIdReceived[CountValue] = -1;
        Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketIdReceived[CountValue]);
        if (RETCODE_OK == Retcode)
        {
            for (OpenedSocketAlready = NEGATIVE_TEST_COUNTER_INIT_VAL_0; OpenedSocketAlready < CountValue; OpenedSocketAlready++)
            {
                CellularSupport_SocketClose(MessagePtr, SocketIdReceived[OpenedSocketAlready]);
            }
            Tests_SendReport(RETCODE_FAILURE, "Fail: More than three sockets are opened");
            return;
        }
    }

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue < CELLULAR_SOCKET_COUNT; CountValue++)
    {
        Retcode = CellularSupport_SocketClose(MessagePtr, SocketIdReceived[CountValue]);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: Socket Close fail");
            return;
        }
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeSocketOperationWithoutClose(CCMsg_T* MessagePtr)
{

    CellularUdpSocket_T SocketIdReceived[CELLULAR_SOCKET_COUNT];
    CellularUdpSocket_T SocketId = -1;
    Retcode_T Retcode = RETCODE_OK;
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;
    uint32_t OpenedSocketAlready = NEGATIVE_TEST_COUNTER_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue < CELLULAR_SOCKET_COUNT; CountValue++)
    {
        SocketIdReceived[CountValue] = -1;
        Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketIdReceived[CountValue]);
        if (RETCODE_OK != Retcode)
        {
            for (OpenedSocketAlready = NEGATIVE_TEST_COUNTER_INIT_VAL_0; OpenedSocketAlready < CountValue; OpenedSocketAlready++)
            {
                CellularSupport_SocketClose(MessagePtr, SocketIdReceived[OpenedSocketAlready]);
            }
            Tests_SendReport(Retcode, "Fail: Socket Open or socket id invalid");
            return;
        }
    }

    Retcode = CellularSupport_DisConnect(MessagePtr);
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW disconnect fail");
        return;
    }

    Retcode = CellularSupport_TurnOff(MessagePtr);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Turn off fail");
        return;
    }

    Retcode = SetupModemGPRSConnect(MessagePtr);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW Connect fail");
        return;
    }

    Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Socket Open fail");
        return;
    }
    Retcode = CellularSupport_UdpSendReceive(MessagePtr, SocketId);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Packet Send fail");
        return;
    }

    Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Socket Close fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is available at above i.e.,declaration of function */
static void RunNegativeSocketSendWithoutopen(CCMsg_T* MessagePtr)
{
    CellularUdpSocket_T SocketId = -1;
    uint32_t TestcaseCount = 0;
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (TestcaseCount = 0; TestcaseCount < NEGATIVE_TEST_CASE_COUNT_3; TestcaseCount++)
    {
        Retcode = CellularSupport_UdpSend(MessagePtr, SocketId);

        if (RETCODE_OK == Retcode)
        {
            Tests_SendReport(Retcode, "fail: Udp_Data send Fail");
            return;
        }

        Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "fail: Socket open");
            return;
        }

        Retcode = CellularSupport_UdpSendReceive(MessagePtr, SocketId);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "fail: Udp_Data send");
            return;
        }

        Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "fail: Socket close");
            return;
        }
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeInvalidConfigAfterConnectTryDisconnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    ConfigSetting.ApnName[0] = 'n';
    ConfigSetting.ApnName[1] = 'e';
    ConfigSetting.ApnName[2] = 't';
    ConfigSetting.ApnName[3] = '\0';
    ConfigSetting.ApnUserName[0] = 'w';
    ConfigSetting.ApnUserName[1] = '\0';
    ConfigSetting.ApnPassword[0] = 'w';
    ConfigSetting.ApnPassword[1] = '\0';

    Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    Retcode = CellularSupport_DisConnect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW disconnect fail");
        return;
    }

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue < NEGATIVE_TEST_CASE_COUNT_3; CountValue++)
    {
        Retcode = Cellular_Connect();

        if (RETCODE_OK == Retcode)
        {
            Tests_SendReport(Retcode, "Fail: NW connect fail");
            return;
        }
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeInvalidConfigAfterDisconnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_DisConnect(MessagePtr);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW disconnect fail");
        return;
    }

    ConfigSetting.ApnName[0] = 'n';
    ConfigSetting.ApnName[1] = 'e';
    ConfigSetting.ApnName[2] = 't';
    ConfigSetting.ApnName[3] = '\0';
    ConfigSetting.ApnUserName[0] = 'w';
    ConfigSetting.ApnUserName[1] = '\0';
    ConfigSetting.ApnPassword[0] = 'w';
    ConfigSetting.ApnPassword[1] = '\0';

    Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue <= NEGATIVE_TEST_ITERATION_VAL_2; CountValue++)
    {
        Retcode = Cellular_Connect();
        if (RETCODE_OK == Retcode)
        {
            Tests_SendReport(Retcode, "Fail: NW connect fail");
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeSendDataAfterSocketClose(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;
    Retcode_T FailRetcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    CellularUdpSocket_T SocketId = NEGATIVE_TEST_SOCKET_ID;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: socket open fail");
        return;
    }

    Retcode = CellularSupport_UdpSend(MessagePtr, SocketId);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: socket send fail");
        return;
    }

    Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: socket close fail");
        return;
    }
	Retcode = CellularSupport_UdpSend(MessagePtr, SocketId);
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(FailRetcode, "Fail: NW Config fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeNetworkInitWhenConnected(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }

    Retcode = CellularSupport_Connect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeSendWrongSocketID(CCMsg_T* MessagePtr)
{

    CellularUdpSocket_T SocketId = -1;
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;
    CellularUdpSocket_T WrongSocketId = NEGATIVE_TEST_COUNTER_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    if (RETCODE_OK != CellularSupport_SocketOpen(MessagePtr, &SocketId))
    {
        Tests_SendReport(Retcode, "Socket Open fail");
        return;
    }

    WrongSocketId = SocketId + UINT32_C(1);

        if (RETCODE_OK == CellularSupport_UdpSendReceive(MessagePtr, WrongSocketId))
        {
            Tests_SendReport(Retcode, "Packet Send fail");
            return;
        }

    Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Socket Close fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

/** The description is in the private header */
static void RunNegativeSendWrongSocketIP(CCMsg_T* MessagePtr)
{
    CellularUdpSocket_T SocketId = -1;
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;
    uint8_t data[4] = { 'C', 'O', 'S', 'P' };
    int8_t ErrorCode;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    if (RETCODE_OK != CellularSupport_SocketOpen(MessagePtr, &SocketId))
    {
        Tests_SendReport(Retcode, "Socket Open fail");
        return;
    }

    /* udp data send */
    Retcode = CellularUdpSocket_Send(SocketId, (const uint32_t) WRONGSOCKET_SERVER_IP,
    SOCKET_SERVER_PORT, data, (uint32_t) SOCKET_UDP_DATA_LENGTH);

    if (RETCODE_OK != Retcode)
    {
        CellularSupport_SocketClose(MessagePtr, SocketId);
        Tests_SendReport(Retcode, "Cellular send is failed");
        return;
    }

    ErrorCode = xSemaphoreTake(SemaphoreHandle, (SOCKET_SEND_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));
    CellularSupport_SocketClose(MessagePtr, SocketId);

    if (pdTRUE != ErrorCode)
    {
        Tests_SendReport(RETCODE_OK, "PASS");

    }else{
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        Tests_SendReport(Retcode, " Data should be Received");
    }

}

/** The description is in the private header */
static void RunNegativeOpenWithoutNwConnect(CCMsg_T* MessagePtr)
{
    CellularUdpSocket_T SocketId = -1;
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

        Retcode = CellularSupport_DisConnect(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "GPRS Disconnect Fail");
            return;
        }

        if (RETCODE_OK == Retcode)
        {
            Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

            if (RETCODE_OK == Retcode)
            {
                CellularSupport_SocketClose(MessagePtr, SocketId);
                Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                Tests_SendReport(Retcode, "Socket Open Pass");
                return;

            }
        }


    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

/** The description is in the private header */
static void RunNegativeCloseWithouNwConnect(CCMsg_T* MessagePtr)
{
    CellularUdpSocket_T SocketId = -1;
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Socket Open fail");
        return;
    }

    if (RETCODE_OK == Retcode)
    {

        Retcode = CellularSupport_DisConnect(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "GPRS Disconnect Fail");
            return;
        }

        if (RETCODE_OK == Retcode)
        {
            Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);

            if (RETCODE_OK == Retcode)
            {
                Tests_SendReport(Retcode, "Socket Close Pass");
                return;

            }
        }

    }

    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

/** The description is in the private header */
static void RunNegativeSendwithoutNwConnect(CCMsg_T* MessagePtr)
{

    CellularUdpSocket_T SocketId = -1;
    Retcode_T Retcode = (Retcode_T) RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Socket Open fail");
        return;
    }

    if (RETCODE_OK == Retcode)
    {

        Retcode = CellularSupport_DisConnect(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "GPRS Disconnect Fail");
            return;
        }

        if (RETCODE_OK == Retcode)
        {
            Retcode = CellularSupport_UdpSendReceive(MessagePtr, SocketId);

            if (RETCODE_OK == Retcode)
            {
                Tests_SendReport(Retcode, "Packet Send Pass");
                return;

            }
        }

    }

    if (RETCODE_OK != CellularSupport_SocketClose(MessagePtr, SocketId))
    {
        Tests_SendReport(Retcode, "Socket Close fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

static void RunNegativeConnectNetworkAfterDisconnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_NetworkInit(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW init fail");
        return;
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }

    Retcode = CellularSupport_Connect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW connect fail");
        return;
    }

    Retcode = CellularSupport_DisConnect(MessagePtr);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW disconnect fail");
        return;
    }

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue < NEGATIVE_TEST_ITERATION_VAL_5; CountValue++)
    {
        Retcode = Cellular_Connect();
        if (RETCODE_OK == Retcode)
        {
            Tests_SendReport(Retcode, "Fail: NW connect fail");
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

/** The description is in the private header */
static void RunNegativeInvalidConfigAfterConnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    ConfigSetting.ApnName[0] = 'n';
    ConfigSetting.ApnName[1] = 'e';
    ConfigSetting.ApnName[2] = 't';
    ConfigSetting.ApnName[3] = '\0';
    ConfigSetting.ApnUserName[0] = 'w';
    ConfigSetting.ApnUserName[1] = '\0';
    ConfigSetting.ApnPassword[0] = 'w';
    ConfigSetting.ApnPassword[1] = '\0';

    Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    for (CountValue = NEGATIVE_TEST_COUNTER_INIT_VAL_0; CountValue < NEGATIVE_TEST_CASE_COUNT_3; CountValue++)
    {
        Retcode = Cellular_Connect();

        if (RETCODE_OK == Retcode)
        {
            Tests_SendReport(Retcode, "Fail: NW connect fail");
            return;
        }
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeSmsSendTextWithExceedMobileNum(CCMsg_T* MessagePtr)
{
	Retcode_T Retcode = RETCODE_FAILURE;
	uint32_t Index;
	uint8_t DataValue = UINT8_C(0x41);
	uint8_t MobileNumber[22] = "+91822001549712345678";
	uint8_t TextMessage[22];
	memset(TextMessage,'\0',sizeof(TextMessage));
	if (NULL == MessagePtr)
	{
		Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(Retcode, " input parameter null ");
		return;
	}
	for(Index = NEGATIVE_TEST_COUNTER_INIT_VAL_0; Index <= NEGATIVE_TEST_ITERATION_VAL_20; Index++)
	{
		TextMessage[Index] = DataValue++;
	}


	Retcode = CellularSms_SendText(MobileNumber,TextMessage,strlen((const char*) TextMessage));
	if (RETCODE_OK == Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS send fail");
		return;
	}

	Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeSmsSendWithExccedDataLength(CCMsg_T* MessagePtr)
{
	Retcode_T Retcode = RETCODE_FAILURE;
	uint32_t Index;
	uint8_t DataValue = UINT8_C(1);
	uint8_t MobileNumber[16] = "+918220015497";
	uint8_t TextMessage[162];
	memset(TextMessage,'\0',sizeof(TextMessage));
	if (NULL == MessagePtr)
	{
		Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(Retcode, " input parameter null ");
		return;
	}
	for(Index = NEGATIVE_TEST_COUNTER_INIT_VAL_0; Index < NEGATIVE_TEST_ITERATION_VAL_161; Index++)
	{
		TextMessage[Index] = DataValue++;
		if(NEGATIVE_TEST_LENGTH_USC2_127 < DataValue)
		{
			DataValue = UINT8_C(1);
		}
	}

	Retcode = CellularSms_SendText(MobileNumber,TextMessage,strlen((const char*) TextMessage));
	if (RETCODE_OK == Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS send fail");
		return;
	}

	Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeSmsSendWithExccedDataAndMobileLength(CCMsg_T* MessagePtr)
{
	Retcode_T Retcode = RETCODE_FAILURE;
	uint32_t Index;
	uint8_t DataValue = UINT8_C(1);
	uint8_t MobileNumber[22] = "+91822001549712345678";
	uint8_t TextMessage[162];
	memset(TextMessage,'\0',sizeof(TextMessage));
	if (NULL == MessagePtr)
	{
		Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(Retcode, " input parameter null ");
		return;
	}
	for(Index = NEGATIVE_TEST_COUNTER_INIT_VAL_0; Index < NEGATIVE_TEST_ITERATION_VAL_161; Index++)
	{
		TextMessage[Index] = DataValue++;
		if(NEGATIVE_TEST_LENGTH_USC2_127 < DataValue)
		{
			DataValue = UINT8_C(1);
		}
	}

	Retcode = CellularSms_SendText(MobileNumber,TextMessage,strlen((const char*) TextMessage));
	if (RETCODE_OK == Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS send fail");
		return;
	}

	Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeSmsSendWithInvalidMobileNum(CCMsg_T* MessagePtr)
{
	Retcode_T Retcode = RETCODE_FAILURE;
	uint32_t Index;
	uint8_t DataValue = UINT8_C(0x41);
	uint8_t MobileNumber[15] = "+91ABCDEF1230";
	uint8_t TextMessage[21];
	memset(TextMessage,'\0',sizeof(TextMessage));
	if (NULL == MessagePtr)
	{
		Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(Retcode, " input parameter null ");
		return;
	}
	for(Index = NEGATIVE_TEST_COUNTER_INIT_VAL_0; Index <= NEGATIVE_TEST_ITERATION_VAL_20; Index++)
	{
		TextMessage[Index] = DataValue++;
	}


	Retcode = CellularSupport_SmsSendText(MobileNumber,TextMessage,strlen((const char*) TextMessage));
	if (RETCODE_OK == Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS send fail");
		return;
	}

	Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunNegativeSmsSendWithInvalidServiceNumber(CCMsg_T* MessagePtr)
{
	Retcode_T Retcode = RETCODE_FAILURE;
	uint32_t Index;
	uint8_t DataValue = UINT8_C(0x41);
	uint8_t ServiceNumber[15] = "+91ABCDEF1230";
	uint8_t MobileNumber[16] = "+918220015497";
	uint8_t TextMessage[21];
	memset(TextMessage,'\0',sizeof(TextMessage));
	if (NULL == MessagePtr)
	{
		Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(Retcode, " input parameter null ");
		return;
	}
	for(Index = NEGATIVE_TEST_COUNTER_INIT_VAL_0; Index <= NEGATIVE_TEST_ITERATION_VAL_20; Index++)
	{
		TextMessage[Index] = DataValue++;
	}

	Retcode = CellularSupport_SmsSendText(MobileNumber,TextMessage,strlen((const char*) TextMessage));
	if (RETCODE_OK != Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS send fail");
		return;
	}
	Retcode = CellularSms_SetServiceCenter(ServiceNumber);
	if (RETCODE_OK != Retcode)
	{
		Tests_SendReport(Retcode, "Fail: Set service num fail");
		return;
	}
	Retcode = CellularSupport_SmsSendText(MobileNumber,TextMessage,strlen((const char*) TextMessage));
	if (RETCODE_OK == Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS send fail");
		return;
	}

	Tests_SendReport(RETCODE_OK, "PASS");
}
static void RunNegativeSmsInitTwice(CCMsg_T* MessagePtr)
{
	Retcode_T Retcode = RETCODE_FAILURE;

	if (NULL == MessagePtr)
	{
		Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(Retcode, " input parameter null ");
		return;
	}

	Retcode = CellularSms_Initialize(SmsEventCallback);
	if (RETCODE_OK == Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS init pass");
		return;
	}

	Tests_SendReport(RETCODE_OK, "PASS");

}


static void RunNegativeSmsSendWithZeroLength(CCMsg_T* MessagePtr)
{
	Retcode_T Retcode = RETCODE_FAILURE;
	uint8_t MobileNumber[16] = "+918220015497";
	uint8_t TextMessage[2];
	memset(TextMessage,'\0',sizeof(TextMessage));
	if (NULL == MessagePtr)
	{
		Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(Retcode, " input parameter null ");
		return;
	}


	Retcode = CellularSupport_SmsSendText(MobileNumber,TextMessage,strlen((const char*) TextMessage));
	if (RETCODE_OK == Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS send fail");
		return;
	}

	Tests_SendReport(RETCODE_OK, "PASS");
}

Retcode_T NegativeTestSuite_Init(void)
{
    Retcode_T Retcode = RETCODE_OK;

    /* cellular on/connect test section register */
    Retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.2 network connection fail with invalid configuration settings  */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectInvalidAPN);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.3 network connection fail with invalid configuration settings (APN name- length exceeded)  */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectAPNLengthExceed);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.4 network connection fail with invalid configuration settings (user name) */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectInvalidUserName);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.5 network connection fail with invalid configuration settings (user name)-more than max size */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectUserNameExceed);
    }
    if (RETCODE_OK == Retcode)
    {
        /*TC 1.4.6 Check for network connection fail with invalid configuration settings (Password) */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectInvalidPassword);
    }
    if (RETCODE_OK == Retcode)
    {
        /*TC 1.4.7 Check for network connection fail with invalid configuration settings (Password)-more than max size */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectPasswordLengthExceed);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.8 Check for network connection fail with invalid configuration settings ( 5 times ) */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectNullCellularConfig);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.9 Connect to Cellular network without configuration */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectCellularWithoutConfig);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.10 Disconnecting the Cellular network without connection */
        Retcode = TEST_REGISTER_TC(RunNegativeDisConnectCellularWithoutConnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.11 Connect the Cellular network twice */
        Retcode = TEST_REGISTER_TC(RunNegativeConnectCellularTwice);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.12 Disconnecting the Cellular network twice*/
        Retcode = TEST_REGISTER_TC(RunNegativeDisconnectCellularTwice);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.13 Turn OFF the Cellular modem, without Disconnecting the Cellular network*/
        Retcode = TEST_REGISTER_TC(RunNegativeOffWithoutDisconnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.14 Socket Open 7 times*/
        Retcode = TEST_REGISTER_TC(RunNegativeSocket7TimesOpen);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.15 Test for Socket operation without socket close ( 3 times )*/
        Retcode = TEST_REGISTER_TC(RunNegativeSocketOperationWithoutClose);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.16 Send data to opened socket at wrong SOCKET ID for ( 10 times )*/
        Retcode = TEST_REGISTER_TC(RunNegativeSendWrongSocketID);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.17 Send data to opened socket at wrong IP address for ( 10 times )*/
        Retcode = TEST_REGISTER_TC(RunNegativeSendWrongSocketIP);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.18 Test for Socket open without network connect*/
        Retcode = TEST_REGISTER_TC(RunNegativeOpenWithoutNwConnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.19 Test for Socket close without network connect*/
        Retcode = TEST_REGISTER_TC(RunNegativeCloseWithouNwConnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.20 Test for Socket send without network connect*/
        Retcode = TEST_REGISTER_TC(RunNegativeSendwithoutNwConnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.21 Send GPRS data without opening the socket */
        Retcode = TEST_REGISTER_TC(RunNegativeSocketSendWithoutopen);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.22 Invalid config after GPRS connection */
        Retcode = TEST_REGISTER_TC(RunNegativeInvalidConfigAfterConnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.23 Invalid config after GPRS Disconnection */
        Retcode = TEST_REGISTER_TC(RunNegativeInvalidConfigAfterConnectTryDisconnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.24: Test for wrong configuration after disconnected state (3 times) */
        Retcode = TEST_REGISTER_TC(RunNegativeInvalidConfigAfterDisconnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.25: Test for Socket send after socket close */
        Retcode = TEST_REGISTER_TC(RunNegativeSendDataAfterSocketClose);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.26: Init in connected state */
        Retcode = TEST_REGISTER_TC(RunNegativeNetworkInitWhenConnected);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.4.27:  Connect after disconnection*/
        Retcode = TEST_REGISTER_TC(RunNegativeConnectNetworkAfterDisconnect);
    }
    if (RETCODE_OK == Retcode)
	{
		/* TC 1.4.28:  Send SMS to a invalid mobile number length (21 bytes) */
		Retcode = TEST_REGISTER_TC(RunNegativeSmsSendTextWithExceedMobileNum);
	}
    if (RETCODE_OK == Retcode)
	{
		/* TC 1.4.29:  Send SMS with invalid message length (161 bytes) */
		Retcode = TEST_REGISTER_TC(RunNegativeSmsSendWithExccedDataLength);
	}
    if (RETCODE_OK == Retcode)
	{
		/* TC 1.4.30:  Send SMS with invalid message length (161 bytes) and invalid mobile number length (21 bytes) */
		Retcode = TEST_REGISTER_TC(RunNegativeSmsSendWithExccedDataAndMobileLength);
	}
    if (RETCODE_OK == Retcode)
	{
		/* TC 1.4.31:  Send SMS with valid message length (20 bytes) and invalid mobile number (14 bytes Alphanumeric) */
		Retcode = TEST_REGISTER_TC(RunNegativeSmsSendWithInvalidMobileNum);
	}
    if (RETCODE_OK == Retcode)
	{
		/* TC 1.4.32:  Send SMS to a invalid service center number (14 bytes Alphanumeric) */
		Retcode = TEST_REGISTER_TC(RunNegativeSmsSendWithInvalidServiceNumber);
	}
    if (RETCODE_OK == Retcode)
	{
		/* TC 1.4.33:  SMS Initialize twice */
		Retcode = TEST_REGISTER_TC(RunNegativeSmsInitTwice);
	}
    if (RETCODE_OK == Retcode)
	{
		/* TC 1.4.34:  Send SMS with zero length of Message */
		Retcode = TEST_REGISTER_TC(RunNegativeSmsSendWithZeroLength);
	}

    if (RETCODE_OK == Retcode)
    {
        Retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunNegativeTurnOnId, NULL, RunTurnOn, NULL);
    }

    if (RETCODE_OK == Retcode)
    {
        Retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunNegativeNetworkInitId, NULL, RunNetworkInit, NULL);
    }

    if (RETCODE_OK == Retcode)
    {
        Retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunNegativeNetworkConfigConnectId, NULL, RunNetworkConfigConnect, NULL);
    }

    if (RETCODE_OK == Retcode)
    {
        Retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunNegativeDisconnectId, NULL, RunDisconnect, NULL);
    }

    if (RETCODE_OK == Retcode)
    {
        Retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunNegativeTurnOffId, NULL, RunTurnOff, NULL);
    }

    return (Retcode);
}
