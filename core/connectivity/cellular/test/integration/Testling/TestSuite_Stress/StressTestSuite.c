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
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_STRESS_TEST_SUITE

#include "BCDS_TestConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "CellularSupport.h"
#include "SmsTestSuite.h"
#include "StressTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_STRESS_ID

extern SemaphoreHandle_t SemaphoreHandle;
extern Cellular_CallbackEvent_T ParseEvent;
static CellularUdpSocket_T SocketId = -1;

/**
 * @brief   The function Check for the reliability of turn ON/Off the modem
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunStressTurnOnOffCellular(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for the reliability of turn ON/Reset the modem
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunStressTurnOnResetCellular(CCMsg_T* MessagePtr);

/**
 * @brief   This test runs the config settings with APN name 100Bytes, User name and password for 30 bytes
 * for 100 times without expecting response
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunStressConfigParaWithMaxLength(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for Network information from modem
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunStressNetworkInfoFromModem(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for Cellular network connection for 100 times
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunStressCellularNetworkConnect(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for Cellular network disconnect for 100 times
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunStressCellularNetworkDisconnect(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for to open the socket for 100 times
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunStressSocketOpen(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for SMS feature by continuous sending SMS 10 times without waiting for Event
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */

static void RunStressSmsSendContinuously(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for Socket Open and UDP packet send after SMS Sending
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */

static void RunStressSmsSendThenSocketOpen(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for SMS and UDP packet Sending without waiting for received Event
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunStressSmsSendUdpSend(CCMsg_T* MessagePtr);

static void RunStressTurnOnOffCellular(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = STRESS_TEST_INDEX_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* Test case */
        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < STRESS_TEST_INDEX_VALUE; Index++)
        {
            Cellular_PowerControl(CELLULAR_POWER_STATE_ON);
            Cellular_PowerControl(CELLULAR_POWER_STATE_OFF);
        }

        Retcode = CellularSupport_TurnOn(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "fail");
            return;
        }

        Retcode = CellularSupport_TurnOff(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "fail");
            return;
        }
    }

    Tests_SendReport(Retcode, NULL);
    return;
}

static void RunStressTurnOnResetCellular(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = STRESS_TEST_INDEX_INIT_VAL_0;
    int8_t ErrorCode;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* Test case */
        Retcode = CellularSupport_TurnOn(MessagePtr);

        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: Turn ON fail");
            return;
        }

        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < STRESS_TEST_INDEX_VALUE; Index++)
        {
            Cellular_Control(CELLULAR_RESET, NULL);
        }

        Retcode = CellularSupport_Reset(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "fail");
            return;
        }

        ErrorCode = xSemaphoreTake(SemaphoreHandle,
                (NETWORK_INIT_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));

        if (pdPASS != ErrorCode)
        {
            Tests_SendReport(Retcode, "fail");
            return;
        }

        if ((CELLULAR_EVENT_INITIALIZED == ParseEvent) || (CELLULAR_EVENT_INITIALIZING == ParseEvent))
        {
            Retcode = RETCODE_OK;
        }
        else
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }

        Retcode = CellularSupport_TurnOff(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "fail");
            return;
        }
    }

    Tests_SendReport(Retcode, NULL);
    return;
}

static void RunStressConfigParaWithMaxLength(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = STRESS_TEST_INDEX_INIT_VAL_0;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* Test case */
        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index <= STRESS_TEST_INDEX_VALUE_100; Index++)
        {
            ConfigSetting.ApnName[Index] = 'n';
        }
        ConfigSetting.ApnName[100] = '\0';

        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index <= STRESS_TEST_INDEX_VALUE_30; Index++)
        {
            ConfigSetting.ApnUserName[Index] = 'g';
        }
        ConfigSetting.ApnUserName[30] = '\0';

        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index <= STRESS_TEST_INDEX_VALUE_30; Index++)
        {
            ConfigSetting.ApnPassword[Index] = 't';
        }
        ConfigSetting.ApnPassword[30] = '\0';

        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < STRESS_TEST_INDEX_VALUE; Index++)
        {
            Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);
        }

        Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: NW config fail");
            return;
        }
    }

    Tests_SendReport(Retcode, NULL);
    return;
}

static void RunStressNetworkInfoFromModem(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = STRESS_TEST_INDEX_INIT_VAL_0;
    Cellular_NetworkInfo_T NetworkInfo;
    uint8_t ErrorInfo;
    Cellular_DeviceStatus_T Value;
    const uint8_t NetworkCode[] = STRESS_TEST_NETWORK_CODE;
    const uint8_t CountryCode[] = STRESS_TEST_COUNTRY_CODE;
    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < STRESS_TEST_INDEX_VALUE; Index++)
        {
            Cellular_QueryModem(CELLULAR_QUERYINFO_NETWORKINFO, &NetworkInfo);
            Cellular_QueryModem(CELLULAR_QUERYINFO_STATUS, &Value);
            Cellular_QueryModem(CELLULAR_QUERYINFO_ERROR, &ErrorInfo);
        }

        Retcode = Cellular_QueryModem(CELLULAR_QUERYINFO_NETWORKINFO, &NetworkInfo);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: NW info fail");
            return;
        }

        /* country code info */
        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < sizeof(NetworkInfo.CountryCode); Index++)
        {
            if (CountryCode[Index] != NetworkInfo.CountryCode[Index])
            {
                Tests_SendReport(RETCODE_FAILURE, "Fail: Cellular nwinfo fail");
                return;
            }
        }

        /* network code info */
        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < NetworkInfo.NetworkCodeLength; Index++)
        {
            if (NetworkCode[Index] != NetworkInfo.NetworkCode[Index])
            {
                Tests_SendReport(RETCODE_FAILURE, "Fail: Cellular nwinfo fail");
                return;
            }
        }
        Retcode = Cellular_QueryModem(CELLULAR_QUERYINFO_STATUS, &Value);
        if (RETCODE_OK != Retcode || CELLULAR_DEVICESTATUS_INIT != Value)
        {
            Tests_SendReport(Retcode, "Fail: Cellular Status fail");
            return;
        }

        Retcode = Cellular_QueryModem(CELLULAR_QUERYINFO_ERROR, &ErrorInfo);
        if (RETCODE_OK != Retcode || (STRESS_TEST_NO_ERROR != ErrorInfo))
        {
            Tests_SendReport(Retcode, "Fail: Error info fail");
            return;
        }
    }

    Tests_SendReport(Retcode, NULL);
    return;
}

static void RunStressCellularNetworkConnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = STRESS_TEST_INDEX_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < STRESS_TEST_INDEX_VALUE; Index++)
        {
            Cellular_Connect();
        }

        Retcode = CellularSupport_DisConnect(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Disconnect Fail");
            return;
        }

        Retcode = CellularSupport_NetworkInit(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Network Init Fail");
            return;
        }

        Retcode = CellularSupport_NetworkConfig(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Network Config Fail");
            return;
        }

        Retcode = CellularSupport_Connect(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Cellular connect Fail");
            return;
        }
    }

    Tests_SendReport(Retcode, NULL);
    return;
}

static void RunStressCellularNetworkDisconnect(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = STRESS_TEST_INDEX_INIT_VAL_0;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < STRESS_TEST_INDEX_VALUE; Index++)
        {
            CellularSupport_DisConnect(MessagePtr);
        }

        Retcode = CellularSupport_NetworkInit(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: Cellular init fail");
            return;
        }

        Retcode = CellularSupport_NetworkConfig(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: Cellular nwConfig fail");
            return;
        }

        Retcode = CellularSupport_Connect(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: Cellular connect fail");
            return;
        }

        Retcode = CellularSupport_DisConnect(MessagePtr);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: Cellular disconnect fail");
            return;
        }
    }

    Tests_SendReport(Retcode, NULL);
    return;
}

static void RunStressSocketOpen(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = STRESS_TEST_INDEX_INIT_VAL_0;
    CellularUdpSocket_T SocketIdReceived[CELLULAR_SOCKET_COUNT];

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < CELLULAR_SOCKET_COUNT; Index++)
        {
            SocketIdReceived[Index] = -1;
            CellularSupport_SocketOpen(MessagePtr, &SocketIdReceived[Index]);
        }

        for (Index = CELLULAR_SOCKET_COUNT; Index < STRESS_TEST_INDEX_VALUE; Index++)
        {
            CellularSupport_SocketOpen(MessagePtr, &SocketId);
        }

        for (Index = STRESS_TEST_INDEX_INIT_VAL_0; Index < CELLULAR_SOCKET_COUNT; Index++)
        {
            Retcode = CellularSupport_SocketClose(MessagePtr, SocketIdReceived[Index]);
            if (RETCODE_OK != Retcode)
            {
                Tests_SendReport(Retcode, "Fail: socket close in loop fail");
                return;
            }
        }

        Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: socket open fail");
            return;
        }

        Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: socket close fail");
            return;
        }
    }

    Tests_SendReport(Retcode, NULL);
    return;
}

static void RunStressConfigBeforeInitEvent(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        return;
    }
    Retcode = Cellular_PowerControl(CELLULAR_POWER_STATE_ON);

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Power ON");
        return;
    }

    Retcode = CellularSupport_NetworkConfig(MessagePtr);
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: NW config fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, NULL);
    return;
}

static void RunStressSocketOpenBeforeConnEvent(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        return;
    }
    Retcode = Cellular_Connect();

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail:Connect fail");
        return;
    }

    Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(RETCODE_FAILURE, "Fail: socket open fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, NULL);
    return;
}

static void RunStressSocketOpenBeforeDisconnEvent(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        return;
    }
    Retcode = Cellular_Disconnect();

    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail:Disconnect fail");
        return;
    }

    Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(RETCODE_FAILURE, "Fail: socket open ");
        return;
    }

    Tests_SendReport(RETCODE_OK, NULL);
    return;
}

static void RunStressSmsSendContinuously(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index = UINT8_C(0);
    uint32_t SMSSendFailCount = UINT8_C(0);
    uint8_t DataValue = UINT8_C(1);
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
        TextMessage[Index] = DataValue++;
        if (SMS_TEST_LENGTH_USC2_127 < DataValue)
        {
            DataValue = UINT8_C(1);
        }
    }

    for (Index = 0; Index < 10; Index++)
    {
        Retcode = CellularSms_SendText(MobileNumber, TextMessage, SMS_TEST_LENGTH_VALUE_160);
        if (RETCODE_OK != Retcode)
        {
            SMSSendFailCount++;

        }

    }

    if (STRESS_TEST_SMS_SEND_SUCCESSFUL < SMSSendFailCount)
    {
        Tests_SendReport(RETCODE_FAILURE, CellularSupport_StringAttach("SMS Send Fail", SMSSendFailCount));
        return;

    }

    Tests_SendReport(RETCODE_OK, CellularSupport_StringAttach("SMS Send PASS", SMSSendFailCount));

}

static void RunStressSmsSendThenSocketOpen(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_FAILURE;
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
        TextMessage[Index] = '%';

    }

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
        return;
    }

    Retcode = CellularSupport_SocketOpen(MessagePtr, &SocketId);

    if (RETCODE_OK != Retcode)
    {
        Retcode = CellularSupport_SocketClose(MessagePtr, SocketId);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: Socket Close");
            return;
        }
        Tests_SendReport(Retcode, "Fail: Socket Close");
        return;
    }

    if (RETCODE_OK != CellularSupport_UdpSendReceive(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Packet Send fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunStressSmsSendUdpSend(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_FAILURE;
    uint32_t Index = UINT8_C(0);
    uint8_t Data[STRESS_TEST_UDP_DATA_SEND_LEN];
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
        TextMessage[Index] = '*';

    }

    for (Index = 0; Index < STRESS_TEST_UDP_DATA_SEND_LEN; Index++)
    {
        Data[Index] = '1';
    }

    for (Index = 0; Index < 5; Index++)
    {

        /* udp data send */
        Retcode = CellularUdpSocket_Send(SocketId, (const uint32_t) SOCKET_SERVER_IP,
        SOCKET_SERVER_PORT, Data, (uint32_t) STRESS_TEST_UDP_DATA_SEND_LEN);

        if (RETCODE_OK != Retcode)
        {
            Retcode = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKET_TX_FAIL);
            return;
        }

        Retcode = CellularSms_SendText(MobileNumber, TextMessage, SMS_TEST_LENGTH_VALUE_160);
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: SMS send fail");
            return;
        }

    }

    vTaskDelay(STRESS_TEST_DELAY_10_MIN / portTICK_RATE_MS);

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS -2 send fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

Retcode_T StressTestSuite_Init(void)
{
    Retcode_T Retcode = RETCODE_FAILURE;

    Retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.1 Turn modem ON */
        Retcode = TEST_REGISTER_TC(RunStressTurnOnOffCellular);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.2 Turn modem ON and RESET */
        Retcode = TEST_REGISTER_TC(RunStressTurnOnResetCellular);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.3 Cellular network Config settings with maximum parameter length */
        Retcode = TEST_REGISTER_TC(RunStressConfigParaWithMaxLength);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.4 Query network info */
        Retcode = TEST_REGISTER_TC(RunStressNetworkInfoFromModem);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.5 Cellular network connection */
        Retcode = TEST_REGISTER_TC(RunStressCellularNetworkConnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.6 Cellular network disconnection */
        Retcode = TEST_REGISTER_TC(RunStressCellularNetworkDisconnect);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.7 Socket open */
        Retcode = TEST_REGISTER_TC(RunStressSocketOpen);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.8 Send connect API before init event will appear */
        Retcode = TEST_REGISTER_TC(RunStressConfigBeforeInitEvent);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.9 Send Socket open API before connected event will appear */
        Retcode = TEST_REGISTER_TC(RunStressSocketOpenBeforeConnEvent);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.10 Send Socket open API before Disconnected event will appear */
        Retcode = TEST_REGISTER_TC(RunStressSocketOpenBeforeDisconnEvent);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.11 Send SMS Continuously without waiting for Event for 10 Times */
        Retcode = TEST_REGISTER_TC(RunStressSmsSendContinuously);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.12 Send SMS then Socket Open and afterwards send UPD PAcket and validate the received event*/
        Retcode = TEST_REGISTER_TC(RunStressSmsSendThenSocketOpen);
    }
    if (RETCODE_OK == Retcode)
    {
        /* TC 1.7.13 Send SMS and UDP Data PAcket without waiting for Event */
        Retcode = TEST_REGISTER_TC(RunStressSmsSendUdpSend);
    }
    return (Retcode);
}

