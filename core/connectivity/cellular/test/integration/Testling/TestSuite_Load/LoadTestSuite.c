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
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_LOAD_TEST_SUITE

#include "BCDS_TestConfig.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include "CellularSupport.h"
#include "SmsTestSuite.h"
#include "LoadTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_LOAD_ID

extern SemaphoreHandle_t SemaphoreHandle; /* Semaphore assigned to a variable of type SemaphoreHandle_t  */
Cellular_CallbackEvent_T ParseEvent; /* Cellular Callback Event status such as Cellular Initializing,Initialized,Data Received,Connect,Disconnect,Error */
static CellularUdpSocket_T SocketId = -1;

/**
 * @brief   The function verify that after calling the Power ON API the Event has received within Threshold Limit.
 * It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadPowerOn(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Reset API the Event has received within Threshold Limit.
 * It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadReset(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Init API the Event has received within Threshold Limit.
 * It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadInit(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Connect API the Event has received within Threshold Limit.
 * It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadConnectDisconnect(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Config API the response has received within Threshold Limit.
 * It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadConfigure(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Socket Open and Close API the response has received within Threshold Limit.
 * It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadSocketOpenCloseTestCase(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Socket Open, Send and Close API the response has received within Threshold Limit.
 * It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadOpenSendCloseTestCase(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Socket Open and Close API for three sockets the response has received within Threshold Limit.
 * It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadThreeOpenCloseTestCase(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Socket Open, Send packet multiple times and Close API for three sockets
 * the response has received within Threshold Limit. It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadOpenSendMultiCloseTestCase(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Socket Open and Send packet for three sockets and Close API for three sockets
 * the response has received within Threshold Limit. It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadOpenSendThreeThenCloseTestCase(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Socket Open for three and Send packet for three sockets and Close API for three sockets
 * the response has received within Threshold Limit. It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadOpenSendCloseSimulTestCase(CCMsg_T* MessagePtr);

/**
 * @brief   The function verify that after calling the Power On, Config, Connect, Socket Open, Send packet,
 * Close Socket, Disconnect and Power Off API, the response has received within Threshold Limit. It has been validated for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadSocketOperation(CCMsg_T* MessagePtr);

/**
 * @brief   The function verifies that, after sending UDP data to a opened socket, for every 20 seconds once, the send and receive are not affected
 *  It has been validated for 300 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadSendDataEvery20Sec(CCMsg_T* MessagePtr);

/**
 * @brief   The function verifies , Sending SMS and UDP packet one after another, the send and receive are not affected.
 *  It has been validated for 10 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunLoadSendSmsSocketSend(CCMsg_T* MessagePtr);

static void RunLoadPowerOn(CCMsg_T* MessagePtr)
{
    uint32_t CountValue;
    if (NULL == MessagePtr)
    {
        Tests_SendReport(RETCODE_NULL_POINTER, "NULL");
        return;
    }

    for (CountValue = LOAD_TEST_COUNTER_INIT_VAL_0; CountValue < LOAD_TEST_ITERATION_VALUE; CountValue++)
    {
        int8_t ErrorCode;
        uint32_t StartTime, EndTime, ActualResponseTime;
        Retcode_T Retcode = RETCODE_OK;

        StartTime = xTaskGetTickCount();

        Retcode = Cellular_PowerControl(CELLULAR_POWER_STATE_ON);

        EndTime = xTaskGetTickCount();

        ActualResponseTime = EndTime - StartTime;

        if ((EXPECTED_RESPONSE_TIME_POWERON < ActualResponseTime) || (RETCODE_OK != Retcode))
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: RETCODE_OK is not received within max timeout", CountValue));
            return;
        }

        ErrorCode = xSemaphoreTake(SemaphoreHandle, (INIT_MAX_TIMEOUT_30_SECS / portTICK_RATE_MS));

        if (pdTRUE != ErrorCode)
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: Event is not received within max timeout", CountValue));
            return;
        }
        if ((CELLULAR_EVENT_INITIALIZED != ParseEvent) && (CELLULAR_EVENT_INITIALIZING != ParseEvent))
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: Relevant Event is not received within max timeout", CountValue));
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, NULL);
}

static void RunLoadReset(CCMsg_T* MessagePtr)
{
    uint32_t CountValue;
    uint32_t StartTime, EndTime, ActualResponseTime;
    Retcode_T Retcode = RETCODE_OK;
    if (NULL == MessagePtr)
    {
        Tests_SendReport(RETCODE_NULL_POINTER, "NULL");
        return;
    }

    for (CountValue = LOAD_TEST_COUNTER_INIT_VAL_0; CountValue < LOAD_TEST_ITERATION_VALUE; CountValue++)
    {
        int8_t ErrorCode;

        StartTime = xTaskGetTickCount();

        Retcode = Cellular_Control(CELLULAR_RESET, NULL);

        EndTime = xTaskGetTickCount();

        ActualResponseTime = EndTime - StartTime;

        if ((EXPECTED_RESPONSE_TIME_RESET < ActualResponseTime) || (RETCODE_OK != Retcode))
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: RETCODE_OK is not received within max timeout", CountValue));
            return;
        }

        ErrorCode = xSemaphoreTake(SemaphoreHandle, (INIT_MAX_TIMEOUT_30_SECS / portTICK_RATE_MS));

        if (pdTRUE != ErrorCode)
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: Event is not received within max timeout", CountValue));
            return;
        }
        if ((CELLULAR_EVENT_INITIALIZED != ParseEvent) && (CELLULAR_EVENT_INITIALIZING != ParseEvent))
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: Relevant Event is not received within max timeout", CountValue));
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, NULL);
}

static void RunLoadInit(CCMsg_T* MessagePtr)
{
    uint32_t CountValue;
    Retcode_T Retcode = RETCODE_OK;
    if (NULL == MessagePtr)
    {
        Tests_SendReport(RETCODE_NULL_POINTER, "NULL");
        return;
    }

    for (CountValue = LOAD_TEST_COUNTER_INIT_VAL_0; CountValue < LOAD_TEST_ITERATION_VALUE; CountValue++)
    {
        int8_t ErrorCode;

        Retcode = Cellular_NetworkInit();

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Init Fail");
            return;
        }

        ErrorCode = xSemaphoreTake(SemaphoreHandle, (INIT_MAX_TIMEOUT_30_SECS / portTICK_RATE_MS));

        if (pdTRUE != ErrorCode)
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: Event is not received within max timeout", CountValue));
            return;
        }
        if ((CELLULAR_EVENT_INITIALIZED != ParseEvent) && (CELLULAR_EVENT_INITIALIZING != ParseEvent))
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: Relevant Event is not received within max timeout", CountValue));
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, NULL);
}

static void RunLoadConnectDisconnect(CCMsg_T* MessagePtr)
{
    uint32_t CountValue;
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Tests_SendReport(RETCODE_NULL_POINTER, "NULL");
        return;
    }

    for (CountValue = LOAD_TEST_COUNTER_INIT_VAL_0; CountValue < 10; CountValue++)
    {
        int8_t ErrorCode;

        Retcode = CellularSupport_NetworkInit(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: NwInit", CountValue));
            return;
        }

        Retcode = CellularSupport_NetworkConfig(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: NwConfig", CountValue));
            return;
        }

        Retcode = Cellular_Connect();

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: Connect");
            return;
        }

        ErrorCode = xSemaphoreTake(SemaphoreHandle, (CONNECT_MAX_TIMEOUT_IN_SECS / portTICK_RATE_MS));

        if (pdTRUE != ErrorCode)
        {
            Tests_SendReport(Retcode, "No event received");
            return;
        }

        if (CELLULAR_EVENT_CONNECTED != ParseEvent)
        {
            Tests_SendReport(Retcode, "Connected event not received");
            return;
        }

        Retcode = Cellular_Disconnect();

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: NwDisconnect", CountValue));
            return;
        }

        ErrorCode = xSemaphoreTake(SemaphoreHandle, (DISCONNECT_MAX_TIMEOUT_IN_SECS / portTICK_RATE_MS));

        if (pdTRUE != ErrorCode)
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: Event is not received within max timeout", CountValue));
            return;
        }
        if (CELLULAR_EVENT_DISCONNECTED != ParseEvent)
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: Relevant Event is not received within max timeout", CountValue));
            return;
        }

    }

    Tests_SendReport(RETCODE_OK, "Pass");

}

static void RunLoadConfigure(CCMsg_T* MessagePtr)
{
    uint32_t CountValue;
    Retcode_T Retcode = RETCODE_OK;
    if (NULL == MessagePtr)
    {
        Tests_SendReport(RETCODE_NULL_POINTER, "NULL");
        return;
    }

    for (CountValue = LOAD_TEST_COUNTER_INIT_VAL_0; CountValue < LOAD_TEST_ITERATION_VALUE; CountValue++)
    {
        uint32_t StartTime, EndTime, ActualResponseTime;

        Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

        memcpy(ConfigSetting.ApnName, APN_NAME, strlen(APN_NAME));
        memset(ConfigSetting.ApnUserName, '\0', sizeof(ConfigSetting.ApnUserName));
        memset(ConfigSetting.ApnPassword, '\0', sizeof(ConfigSetting.ApnPassword));

        Retcode = CellularSupport_NetworkInit(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Init Fail");
            return;
        }

        StartTime = xTaskGetTickCount();

        Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

        EndTime = xTaskGetTickCount();

        ActualResponseTime = EndTime - StartTime;

        if ((EXPECTED_RESPONSE_TIME_CONFIG != ActualResponseTime) && (RETCODE_OK != Retcode))
        {
            Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("Fail: API response is not received within max timeout", CountValue));
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, NULL);
}

/* The description is in the private header */

static void RunLoadSocketOpenCloseTestCase(CCMsg_T* MessagePtr)
{

    CellularUdpSocket_T SocketId = -1;
    uint32_t CountValue = LOAD_TEST_COUNTER_INIT_VAL_0;

    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = 0; CountValue < LOAD_TEST_ITERATION_VALUE; CountValue++)
    {
        Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Open", CountValue));
            return;
        }

        Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", CountValue));
            return;
        }

    }

    Tests_SendReport(RETCODE_OK, NULL);

}

/* The description is in the private header */

static void RunLoadOpenSendCloseTestCase(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    CellularUdpSocket_T SocketId = -1;
    uint32_t CountValue = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t PacketRxFailCount = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t UdpEventDataNotReceived = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SendFailCount = 0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = 0; CountValue < LOAD_TEST_ITERATION_VALUE; CountValue++)
    {
        Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Open", CountValue));
            return;
        }

        Retcode = CellularSupport_UdpSendReqBytes(MessagePtr, SocketId, DATA_SEND_LEN);

        if (RETCODE_OK != Retcode)
        {
            uint32_t ErrorCode = Retcode_GetCode(Retcode);

            if (RETCODE_SEMAPHORE_ERROR == ErrorCode)
            {
                PacketRxFailCount++;
            }
            else if (CELLULAR_TESTS_PACKET_TX_FAIL == ErrorCode)
            {
                SendFailCount++;
            }

            else if (RETCODE_FAILURE == ErrorCode)
            {
                UdpEventDataNotReceived++;

            }

        }

        Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", CountValue));
            return;
        }

        if (UDPSENDFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < SendFailCount)
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Packet Send Fail Count", SendFailCount));
            return;
        }

        if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < PacketRxFailCount)
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Packet Receive Fail Count", PacketRxFailCount));
            return;
        }

        if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < UdpEventDataNotReceived)
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            Tests_SendReport(Retcode, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));
            return;
        }

    }

    Tests_SendReport(RETCODE_OK, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));

}

/* The description is in the private header */

static void RunLoadThreeOpenCloseTestCase(CCMsg_T* MessagePtr)
{
    uint32_t count = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SocketOpen = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t CountValue = LOAD_TEST_COUNTER_INIT_VAL_0;

    CellularUdpSocket_T SocketIdReceived[CELLULAR_SOCKET_COUNT]; /* Upto 3 received Socket Id be kept in this variable   */

    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = LOAD_TEST_COUNTER_INIT_VAL_0; CountValue < LOAD_TEST_ITERATION_VALUE; CountValue++)
    {
        for (count = LOAD_TEST_COUNTER_INIT_VAL_0; count < CELLULAR_SOCKET_COUNT; count++)
        {
            /* socket open */
            SocketIdReceived[count] = -1;
            Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketIdReceived[count]);

            if (RETCODE_OK != Retcode)
            {

                for (SocketOpen = LOAD_TEST_COUNTER_INIT_VAL_0; SocketOpen < count; SocketOpen++)
                {
                    Retcode = CellularSupport_SocketClose(MessagePtr, SocketIdReceived[SocketOpen]);

                    if (RETCODE_OK != Retcode)
                    {
                        Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Fail", CountValue));
                        return;
                    }

                }
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Open", CountValue));
                return;
            }
        }

        for (count = LOAD_TEST_COUNTER_INIT_VAL_0; count < CELLULAR_SOCKET_COUNT; count++)
        {
            /* Socket Close */
            Retcode = CellularSupport_SocketClose(MessagePtr, SocketIdReceived[count]);

            if (RETCODE_OK != Retcode)
            {
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", CountValue));
                return;
            }

        }

    }

    Tests_SendReport(RETCODE_OK, NULL);

}

/* The description is in the above  */

static void RunLoadOpenSendMultiCloseTestCase(CCMsg_T* MessagePtr)
{

    Retcode_T Retcode = RETCODE_OK;

    CellularUdpSocket_T SocketId = -1;
    uint32_t CountValue = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SendValue = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t PacketRxFailCount = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t UdpEventDataNotReceived = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SendFailCount = 0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Open", CountValue));
        return;
    }

    for (SendValue = 0; SendValue < LOAD_TEST_ITERATION_VALUE; SendValue++)
    {
        Retcode = CellularSupport_UdpSendReqBytes(MessagePtr, SocketId, DATA_SEND_LEN);

        if (RETCODE_OK != Retcode)
        {
            uint32_t ErrorCode = Retcode_GetCode(Retcode);

            if (RETCODE_SEMAPHORE_ERROR == ErrorCode)
            {
                PacketRxFailCount++;
            }
            else if (CELLULAR_TESTS_PACKET_TX_FAIL == ErrorCode)
            {
                SendFailCount++;
            }

            else if (RETCODE_FAILURE == ErrorCode)
            {
                UdpEventDataNotReceived++;

            }

        }

        if (UDPSENDFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < SendFailCount)
        {
            Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
            if (RETCODE_OK != Retcode)
            {
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", SendFailCount));
                return;
            }

            else
            {
                Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Packet Send Fail Count", SendFailCount));
                return;
            }
        }

        if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < UdpEventDataNotReceived)
        {
            Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
            if (RETCODE_OK != Retcode)
            {
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", SendFailCount));
                return;
            }

            else
            {
                Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                Tests_SendReport(Retcode, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));
                return;
            }
        }

        if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < PacketRxFailCount)
        {
            Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
            if (RETCODE_OK != Retcode)
            {
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", PacketRxFailCount));
                return;
            }
            else
            {
                Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Packet Receive Fail Count", PacketRxFailCount));
                return;
            }
        }

        vTaskDelay(PACKETSEND_DELAY_500_MILLISECS / portTICK_RATE_MS);

    }

    Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", SendFailCount));
        return;
    }

    Tests_SendReport(RETCODE_OK, CellularSupport_StringAttach("Packet Receive Fail Count", PacketRxFailCount));

}

/* The description is in the above  */
static void RunLoadOpenSendThreeThenCloseTestCase(CCMsg_T* MessagePtr)
{

    Retcode_T Retcode = RETCODE_OK;
    uint32_t count = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SocketOpen = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t CountValue = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SendValue = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t UdpEventDataNotReceived = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t PacketRxFailCount = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SendFailCount = 0;

    CellularUdpSocket_T SocketIdReceived[CELLULAR_SOCKET_COUNT]; /* Upto 3 received Socket Id be kept in this variable   */

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = 0; CountValue < LOAD_TEST_ITERATION_THREE_SCKT; CountValue++)
    {

        for (count = LOAD_TEST_COUNTER_INIT_VAL_0; count < CELLULAR_SOCKET_COUNT; count++)
        {
            /* socket open */
            SocketIdReceived[count] = -1;
            Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketIdReceived[count]);

            if (RETCODE_OK != Retcode)
            {
                for (SocketOpen = LOAD_TEST_COUNTER_INIT_VAL_0; SocketOpen < count; SocketOpen++)
                {
                    Retcode = CellularSupport_SocketClose(MessagePtr, SocketIdReceived[SocketOpen]);
                    if (RETCODE_OK != Retcode)
                    {
                        Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", CountValue));
                        return;
                    }
                }

                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Open", CountValue));
                return;
            }

            for (SendValue = 0; SendValue < LOAD_TEST_ITERATION_VALUE; SendValue++)
            {

                Retcode = CellularSupport_UdpSendReqBytes(MessagePtr, SocketIdReceived[count], DATA_SEND_LEN);

                if (RETCODE_OK != Retcode)
                {

                    uint32_t ErrorCode = Retcode_GetCode(Retcode);

                    if (RETCODE_SEMAPHORE_ERROR == ErrorCode)
                    {
                        PacketRxFailCount++;
                    }
                    else if (CELLULAR_TESTS_PACKET_TX_FAIL == ErrorCode)
                    {
                        SendFailCount++;
                        if (UDPSENDFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < SendFailCount)
                        {
                            break;
                        }
                    }

                    else if (RETCODE_FAILURE == ErrorCode)
                    {
                        UdpEventDataNotReceived++;

                    }

                }

                vTaskDelay(PACKETSEND_DELAY_500_MILLISECS / portTICK_RATE_MS);
            }

        }

        for (count = LOAD_TEST_COUNTER_INIT_VAL_0; count < CELLULAR_SOCKET_COUNT; count++)
        {
            /* Socket Close */
            Retcode = CellularSupport_SocketClose(MessagePtr, SocketIdReceived[count]);
            if (RETCODE_OK != Retcode)
            {
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", CountValue));
                return;
            }

        }

    }
    /* in this test case, we test whether the sending is successful. So, don't care if the receive is failing(for now, @todo - Do it properly after analysis)  */

    if ((UDPSENDFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) * CELLULAR_SOCKET_COUNT) < SendFailCount)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        Tests_SendReport(Retcode, CellularSupport_StringAttach("No of packets that are not transmitted", SendFailCount));
        return;

    }

    if (((LOAD_TEST_ITERATION_VALUE * CELLULAR_SOCKET_COUNT) / 5) < PacketRxFailCount)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        Tests_SendReport(Retcode, CellularSupport_StringAttach("Packet Receive Fail Count", PacketRxFailCount));
        return;
    }

    if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < UdpEventDataNotReceived)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        Tests_SendReport(Retcode, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));
        return;
    }

    Tests_SendReport(RETCODE_OK, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));

}

/* The description is in the above  */
static void RunLoadOpenSendCloseSimulTestCase(CCMsg_T* MessagePtr)
{

    Retcode_T Retcode = RETCODE_OK;
    uint32_t count = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SocketOpen = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t CountValue = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SendValue = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t PacketRxFailCount = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t UdpEventDataNotReceived = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SendFailCount = 0;

    CellularUdpSocket_T SocketIdReceived[CELLULAR_SOCKET_COUNT]; /* Upto 3 received Socket Id be kept in this variable   */

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = 0; CountValue < LOAD_TEST_ITERATION_THREE_SCKT; CountValue++)
    {

        for (count = LOAD_TEST_COUNTER_INIT_VAL_0; count < CELLULAR_SOCKET_COUNT; count++)
        {
            /* socket open */
            SocketIdReceived[count] = -1;
            Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketIdReceived[count]);

            if (RETCODE_OK != Retcode)
            {

                for (SocketOpen = LOAD_TEST_COUNTER_INIT_VAL_0; SocketOpen < count; SocketOpen++)
                {
                    Retcode = CellularSupport_SocketClose(MessagePtr, SocketIdReceived[SocketOpen]);

                    if (RETCODE_OK != Retcode)
                    {

                        Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close after open", CountValue));
                        return;
                    }
                }
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Open", CountValue));
                return;
            }
        }

        for (SocketOpen = LOAD_TEST_COUNTER_INIT_VAL_0; SocketOpen < CELLULAR_SOCKET_COUNT; SocketOpen++)
        {

            for (SendValue = 0; SendValue < LOAD_TEST_ITERATION_VALUE; SendValue++)
            {

                Retcode = CellularSupport_UdpSendReqBytes(MessagePtr, SocketIdReceived[SocketOpen], DATA_SEND_LEN);

                if (RETCODE_OK != Retcode)
                {
                    uint32_t ErrorCode = Retcode_GetCode(Retcode);

                    if (RETCODE_SEMAPHORE_ERROR == ErrorCode)
                    {
                        PacketRxFailCount++;
                    }
                    else if (CELLULAR_TESTS_PACKET_TX_FAIL == ErrorCode)
                    {
                        SendFailCount++;

                    }

                    else if (RETCODE_FAILURE == ErrorCode)
                    {
                        UdpEventDataNotReceived++;

                    }

                }
                vTaskDelay(PACKETSEND_DELAY_500_MILLISECS / portTICK_RATE_MS);

            }
        }

        for (count = LOAD_TEST_COUNTER_INIT_VAL_0; count < CELLULAR_SOCKET_COUNT; count++)
        {
            /* Socket Close */
            Retcode = CellularSupport_SocketClose(MessagePtr, SocketIdReceived[count]);

            if (RETCODE_OK != Retcode)
            {
                Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", CountValue));
                return;
            }

        }
    }

    if (UDPSENDFAILURE_TOLERANCE((CELLULAR_SOCKET_COUNT*LOAD_TEST_ITERATION_VALUE)) < SendFailCount)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKETS_LOST_DURING_TX);
        Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Packet Send Fail Count", SendFailCount));
        return;
    }

    if (UDPRECEIVEFAILURE_TOLERANCE(CELLULAR_SOCKET_COUNT*LOAD_TEST_ITERATION_VALUE) < PacketRxFailCount)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKETS_LOST_DURING_RX);
        Tests_SendReport(Retcode, CellularSupport_StringAttach("Packet Receive Fail Count", PacketRxFailCount));
        return;
    }

    if (UDPRECEIVEFAILURE_TOLERANCE(CELLULAR_SOCKET_COUNT*LOAD_TEST_ITERATION_VALUE) < UdpEventDataNotReceived)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        Tests_SendReport(Retcode, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));
        return;
    }

    Tests_SendReport(RETCODE_OK, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));

}

/* The description is in the above  */
static void RunLoadSocketOperation(CCMsg_T* MessagePtr)
{

    Retcode_T Retcode = RETCODE_OK;

    CellularUdpSocket_T SocketId = -1;
    uint32_t CountValue = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t PacketRxFailCount = LOAD_TEST_COUNTER_INIT_VAL_0;
    uint32_t SendFailCount = 0;
    uint32_t UdpEventDataNotReceived = LOAD_TEST_COUNTER_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (CountValue = 0; CountValue < LOAD_TEST_ITERATION_VALUE; CountValue++)
    {

        Retcode = CellularSupport_TurnOn(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Power ON", CountValue));
            return;
        }

        Retcode = CellularSupport_NetworkInit(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: NetworkInit", CountValue));
            return;
        }

        Retcode = CellularSupport_NetworkConfig(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Network Config", CountValue));
            return;
        }

        Retcode = CellularSupport_Connect(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Connect", CountValue));
            return;
        }

        Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Open", CountValue));
            return;
        }

        Retcode = CellularSupport_UdpSendReqBytes(MessagePtr, SocketId, DATA_SEND_LEN);

        if (RETCODE_OK != Retcode)
        {
            uint32_t ErrorCode = Retcode_GetCode(Retcode);

            if (RETCODE_SEMAPHORE_ERROR == ErrorCode)
            {
                PacketRxFailCount++;
            }
            else if (CELLULAR_TESTS_PACKET_TX_FAIL == ErrorCode)
            {
                SendFailCount++;
            }

            else if (RETCODE_FAILURE == ErrorCode)
            {
                UdpEventDataNotReceived++;

            }

        }
        if (UDPSENDFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < SendFailCount)
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Packet Send Fail Count", SendFailCount));
            return;
        }

        if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < PacketRxFailCount)
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Packet Receive Fail Count", PacketRxFailCount));
            return;
        }

        if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < UdpEventDataNotReceived)
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            Tests_SendReport(Retcode, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));
            return;
        }

        Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Socket Close", CountValue));
            return;
        }

        Retcode = CellularSupport_DisConnect(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Disconnect", CountValue));
            return;
        }

        Retcode = CellularSupport_TurnOff(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, CellularSupport_StringAttach("Fail: Poweroff", CountValue));
            return;
        }

    }

    Tests_SendReport(RETCODE_OK, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));

}

static void RunLoadSendDataEvery20Sec(CCMsg_T* MessagePtr)
{
    Retcode_T retcode = (Retcode_T) RETCODE_OK;
    uint32_t FailedToSendCount = 0;
    uint32_t FailedToReceiveCount = 0;
    uint32_t UdpEventDataNotReceived = 0;
    uint32_t ErrorCode = 0;
    CellularUdpSocket_T SocketId = -1;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "Fail: Socket Open");
        return;
    }

    for (uint32_t index = 0; index < LOAD_TEST_ITERATION_VALUE; index++)
    {
        retcode = CellularSupport_UdpSendReceive(MessagePtr, SocketId);

        if (RETCODE_OK != retcode)
        {
            ErrorCode = Retcode_GetCode(retcode);

            if (CELLULAR_TESTS_PACKET_TX_FAIL == ErrorCode)
            {
                FailedToSendCount++;
            }
            else if (RETCODE_SEMAPHORE_ERROR == ErrorCode)
            {
                FailedToReceiveCount++;
            }
            else
            {
                UdpEventDataNotReceived++;
            }
        }
        vTaskDelay(TESTCASE14_DELAY_20_SECONDS / portTICK_RATE_MS);
    }

    if (UDPSENDFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < FailedToSendCount)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKETS_LOST_DURING_TX);
        Tests_SendReport(retcode, CellularSupport_StringAttach("No. of packets that are not transmitted from device", FailedToSendCount));
    }
    else if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < FailedToReceiveCount)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKETS_LOST_DURING_RX);
        Tests_SendReport(retcode, CellularSupport_StringAttach("No. of packets that are not received by device", FailedToReceiveCount));
    }
    else if (UDPRECEIVEFAILURE_TOLERANCE(LOAD_TEST_ITERATION_VALUE) < UdpEventDataNotReceived)
    {
        Tests_SendReport(retcode, CellularSupport_StringAttach("UDP data event not received", UdpEventDataNotReceived));
    }

    else
    {
        retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "Fail: Socket Close");
            return;
        }

        Tests_SendReport(RETCODE_OK, CellularSupport_StringAttach("UDP Event Data Not Received", UdpEventDataNotReceived));
    }
}

static void RunLoadSendSmsSocketSend(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = UINT8_C(0);
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[161];

    memset(TextMessage, '\0', sizeof(TextMessage));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_160; Index++)
    {
        TextMessage[Index] = '$';

    }


    for (Index = 0; Index < 10; Index++)
    {
        Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: SMS send fail");
            return;
        }

        Retcode = CellularSupport_UdpSendReqBytes(MessagePtr, SocketId, DATA_SEND_LEN);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: socket send fail");
            return;
        }
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

/* global functions ********************************************************* */

Retcode_T LoadTestSuite_Init()
{

    Retcode_T Retcode = RETCODE_OK;

    /* cellular on connect test section register */
    Retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.1 Loading Cellular driver with Power ON API for 100 times*/
        Retcode = TEST_REGISTER_TC(RunLoadPowerOn);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.2 Loading Cellular driver with Reset API for 100 times*/
        Retcode = TEST_REGISTER_TC(RunLoadReset);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.3 Loading Cellular driver with Init API for 100 times */
        Retcode = TEST_REGISTER_TC(RunLoadInit);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.4 Loading Cellular driver with Configure API for 100 times */
        Retcode = TEST_REGISTER_TC(RunLoadConfigure);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.5 Loading Cellular driver with Connect API for 100 times*/
        Retcode = TEST_REGISTER_TC(RunLoadConnectDisconnect);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.6: Socket open and close Load Test for 100 Times */
        Retcode = TEST_REGISTER_TC(RunLoadSocketOpenCloseTestCase);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.7: Socket open, Packet Send and close Load Test for 100 Times */
        Retcode = TEST_REGISTER_TC(RunLoadOpenSendCloseTestCase);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.8: Open Three Socket Simultaneously  and close Three Socket Simultaneously for 100 Times */
        Retcode = TEST_REGISTER_TC(RunLoadThreeOpenCloseTestCase);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.9: To verify that after calling the Socket Send API, it returns response within an expected threshold time limit. for 100 Times */
        Retcode = TEST_REGISTER_TC(RunLoadOpenSendMultiCloseTestCase);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.10: To verify that after Opening the Socket Send and receive the Data for Multiple Time for all the Valid three Socket for 100 Times */
        Retcode = TEST_REGISTER_TC(RunLoadOpenSendThreeThenCloseTestCase);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.11: To verify that after Opening the Socket Send and receive the Data for Multiple Time for all the Valid three Socket
         for 100 Times */
        Retcode = TEST_REGISTER_TC(RunLoadOpenSendCloseSimulTestCase);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.12: To verify Init,Config,Connect,Socket Open,Send Data,Close,Disconnect and Poweroff for 100 Times */
        Retcode = TEST_REGISTER_TC(RunLoadSocketOperation);
    }

    if (RETCODE_OK == Retcode)
    {
    	/* TC 1.6.13:send socket data for every 20 seconds*/
    	Retcode = TEST_REGISTER_TC(RunLoadSendDataEvery20Sec);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.6.14:Send SMS and send socket data continuously for 10 times*/
        Retcode = TEST_REGISTER_TC(RunLoadSendSmsSocketSend);
    }

    return (Retcode);
}
/** ************************************************************************* */
