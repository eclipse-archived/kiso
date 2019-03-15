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
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_SMS_TEST_SUITE

#include "BCDS_TestConfig.h"
#include "BCDS_TestSuite.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include "CellularSupport.h"
#include "SmsTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_SMS_ID

static CellularUdpSocket_T SocketId = -1;

/**
 * @brief   The function checks the functionality of sending the text message of 1 byte for 5 times
 *  and check the functionality
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendWithOneByteFiveTimes(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality of sending the text message with 160bytes (UCF2) for 5 times
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendwith160BytesFiveTimes(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality of sending the text message with extended ASCII
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendExtendedAsciiFiveTimes(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality of sending the text message to two valid numbers consecutively
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendTo2Numbers(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality of sending the message with incremental text length
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendIncrementalSms(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality of deinitialize sms for two times after sms initialize.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendDeinitTwiceSms(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to set the service number twice.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSetServiceNumberTwice(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms when network connected, disconnected and while opening the socket
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendWithDiffCond(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms between two socket send operations.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendSocketSend(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms with an incremental delay from 50 sec to 300 sec.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendwithIncDelay(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms and turn off the modem without deinitialize the modem.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendTurnoffwithoutDeinitSms(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms after get and configure the service number.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendGetSetServiceNum(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms after deinitialize the sms interface.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendAfterDenitSms(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms with NULL data.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendWithNullData(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms with mobile number 20 digits and text length 160.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendWithExceedNum(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms and power off modem immediately.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendAndPoweroff(CCMsg_T* MessagePtr);

/**
 * @brief   The function checks the functionality to send sms with mobile number 6 digits and text length 160.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmsSendWith6DigitNo(CCMsg_T* MessagePtr);

static void RunSmsSendWithOneByteFiveTimes(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[2] = "A";

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (Index = SMS_TEST_INDEX_VALUE_5; Index > SMS_TEST_INDEX_VALUE_0; Index--)
    {
        Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: SMS send fail");
            return;
        }
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendwith160BytesFiveTimes(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Retcode = RETCODE_FAILURE;
    uint32_t Index;
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[161] =
            {

            0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x07, 0x09, 0x0A,
                    0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c,
                    0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
                    0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
                    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
                    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
                    0x6b, 0x6c, 0x6d, 0x6e, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74,
                    0x75, 0x76, 0x77, 0x77, 0x78, 0x79, 0x7A, 0x33, 0x34, 0x35,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x07, 0x09, 0x0A,
                    0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c,
                    0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
                    0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
                    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
                    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
                    0x6b, 0x6c, 0x6d, 0x6e, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74,
                    0x75, 0x76, 0x77, 0x77, 0x78, 0x79, 0x7A, 0x33, 0x34, 0x35,
                    0x00
            };

    for (Index = 0; Index < SMS_TEST_INDEX_VALUE_5; Index++)
    {
        Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: SMS send fail");
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendExtendedAsciiFiveTimes(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(1);
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[21];
    memset(TextMessage, '\0', sizeof(TextMessage));
    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_20; Index++)
    {
        TextMessage[Index] = DataValue++;
    }

    for (Index = SMS_TEST_INDEX_VALUE_5; Index > 0; Index--)
    {
        Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: SMS send fail");
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendTo2Numbers(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t TextMessage[161];
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t SecondNumber[16] = "+491735205096";

    memset(TextMessage, '\0', sizeof(TextMessage));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_160; Index++)
    {
        TextMessage[Index] = 'a';
    }
    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send-1 fail");
        return;
    }

    Retcode = CellularSupport_SmsSendText(SecondNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send-2 fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendIncrementalSms(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(0x30);
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
        if (SMS_TEST_LENGTH_VALUE_90 < DataValue)
        {
            DataValue = UINT8_C(0x30);
        }
    }

    for (Index = SMS_TEST_INDEX_VALUE_1; Index < SMS_TEST_INDEX_VALUE_5; Index++)
    {
        Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, (Index * SMS_TEST_ITIRATIVE_VALUE_40));
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: SMS send fail");
            return;
        }
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendDeinitTwiceSms(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSms_Deinitialize();
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS deinit fail");
        return;
    }
    Retcode = CellularSms_Deinitialize();
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS deinit fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");

}
static void RunSmsSetServiceNumberTwice(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint8_t ServiceNumber[16];
    uint8_t ServiceNumberLength = 16;
    memset(ServiceNumber, '\0', sizeof(ServiceNumber));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }

    Retcode = CellularSms_GetServiceCenter(ServiceNumber, &ServiceNumberLength);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Get service num fail");
        return;
    }
    Retcode = CellularSms_SetServiceCenter(ServiceNumber);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Set service num -1 fail");
        return;
    }
    Retcode = CellularSms_SetServiceCenter(ServiceNumber);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Set service num -2 fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendWithDiffCond(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(0x41);
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[21];
    memset(TextMessage, '\0', sizeof(TextMessage));
    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_20; Index++)
    {
        TextMessage[Index] = DataValue++;
    }

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}
static void RunSmsSendSocketSend(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(0x41);
    uint8_t Data[SMS_TEST_LENGTH_VALUE_100];
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[21];
    memset(Data, UINT32_C(0), SMS_TEST_LENGTH_VALUE_100);
    memset(TextMessage, '\0', sizeof(TextMessage));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_20; Index++)
    {
        TextMessage[Index] = DataValue++;
    }

    for (Index = 0; Index < SMS_TEST_LENGTH_VALUE_100; Index++)
    {
        Data[Index] = '1';
    }
    Retcode = CellularUdpSocket_Send(SocketId, (const uint32_t) SOCKET_SERVER_IP,
    SOCKET_SERVER_PORT, Data, SMS_TEST_LENGTH_VALUE_100);
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: socket send fail");
        return;
    }
    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
        return;
    }
    for (Index = 0; Index < SMS_TEST_LENGTH_VALUE_50; Index++)
    {
        Data[Index] = '2';
    }
    Retcode = CellularUdpSocket_Send(SocketId, (const uint32_t) SOCKET_SERVER_IP,
    SOCKET_SERVER_PORT, Data, SMS_TEST_LENGTH_VALUE_50);
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: socket send fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendwithIncDelay(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(0x41);
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[21];
    memset(TextMessage, '\0', sizeof(TextMessage));
    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_20; Index++)
    {
        TextMessage[Index] = DataValue++;
    }

    for (Index = 0; Index < SMS_TEST_INDEX_VALUE_6; Index++)
    {
        Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
        if (RETCODE_OK != Retcode)
        {
            Tests_SendReport(Retcode, "Fail: SMS send fail");
            return;
        }
        vTaskDelay(Index * SMS_SEND_ITERATION_DELAY_MILLI_SECS / portTICK_RATE_MS);
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendTurnoffwithoutDeinitSms(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(1);
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[21];
    memset(TextMessage, '\0', sizeof(TextMessage));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_20; Index++)
    {
        TextMessage[Index] = DataValue++;
    }

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
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
        Tests_SendReport(Retcode, "Fail: network init fail");
        return;
    }

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendGetSetServiceNum(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint8_t ServiceNumber[16];
    uint8_t ServiceNumberLength = 16;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(1);
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[21];
    memset(TextMessage, '\0', sizeof(TextMessage));
    memset(ServiceNumber, '\0', sizeof(ServiceNumber));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_20; Index++)
    {
        TextMessage[Index] = DataValue++;
    }

    Retcode = CellularSms_GetServiceCenter(ServiceNumber, &ServiceNumberLength);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Get service num fail");
        return;
    }
    Retcode = CellularSms_SetServiceCenter(ServiceNumber);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Set service num fail");
        return;
    }

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendAfterDenitSms(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(0x41);
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[21];
    memset(TextMessage, '\0', sizeof(TextMessage));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_20; Index++)
    {
        TextMessage[Index] = DataValue++;
    }

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
        return;
    }
    Retcode = CellularSms_Deinitialize();
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: sms deinit fail");
        return;
    }
    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendWithNullData(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[2];
    TextMessage[0] = '\0';
    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send pass");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendWithExceedNum(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(1);
    uint8_t MobileNumber[21] = "+9182200154971234567";
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

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send Pass");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunSmsSendAndPoweroff(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t DataValue = UINT8_C(1);
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[21];
    memset(TextMessage, '\0', sizeof(TextMessage));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for (Index = SMS_TEST_INDEX_VALUE_0; Index < SMS_TEST_LENGTH_VALUE_20; Index++)
    {
        TextMessage[Index] = DataValue++;
    }

    Retcode = CellularSms_SendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: SMS send fail");
        return;
    }
    Retcode = CellularSupport_TurnOff(MessagePtr);
    if (RETCODE_OK != Retcode)
    {
        Tests_SendReport(Retcode, "Fail: Power Off Fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}
static void RunSmsSendWith6DigitNo(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_FAILURE;
    uint32_t Index;
    uint8_t MobileNumber[7] = "+91822";
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
        TextMessage[Index] = 'r';

    }

    Retcode = CellularSupport_SmsSendText(MobileNumber, TextMessage, strlen((const char*) TextMessage));
    if (RETCODE_OK == Retcode)
    {
        Tests_SendReport(RETCODE_FAILURE, "Fail: SMS send fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

Retcode_T SmsTestSuite_Init(void)
{
    Retcode_T Retcode = RETCODE_OK;

    Retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.1 Sending 5 SMS to a valid mobile number with min data limit(1 byte) */
        Retcode = TEST_REGISTER_TC(RunSmsSendWithOneByteFiveTimes);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.2 Sending 5 SMS to a valid mobile number with max data limit(160 bytes) */
        Retcode = TEST_REGISTER_TC(RunSmsSendwith160BytesFiveTimes);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.3 Sending 5 SMS to a valid mobile number with extended ascii characters */
        Retcode = TEST_REGISTER_TC(RunSmsSendExtendedAsciiFiveTimes);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.4 Sending a SMS to 2 different valid mobile number consecutively */
        Retcode = TEST_REGISTER_TC(RunSmsSendTo2Numbers);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.5 Sending incremental SMS to a valid mobile number */
        Retcode = TEST_REGISTER_TC(RunSmsSendIncrementalSms);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.6 SMS De-Initialize twice */
        Retcode = TEST_REGISTER_TC(RunSmsSendDeinitTwiceSms);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.7 Configuring the Service Center Number twice */
        Retcode = TEST_REGISTER_TC(RunSmsSetServiceNumberTwice);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.8 Send SMS in Network connected state */
        /* TC 1.10.9 Send SMS in Network disconnected state */
        /* TC 1.10.10 Send SMS with socket open */
        Retcode = TEST_REGISTER_TC(RunSmsSendWithDiffCond);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.11 Send SMS between socket send */
        Retcode = TEST_REGISTER_TC(RunSmsSendSocketSend);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.12 Sending SMS to a valid mobile number after incremental delay */
        Retcode = TEST_REGISTER_TC(RunSmsSendwithIncDelay);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.13  Power Off the modem without de-initializing the SMS */
        Retcode = TEST_REGISTER_TC(RunSmsSendTurnoffwithoutDeinitSms);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.14 Get and Configure the Service center Number to validate the SMS send */
        Retcode = TEST_REGISTER_TC(RunSmsSendGetSetServiceNum);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.15 De-initializing the SMS so event should not received */
        Retcode = TEST_REGISTER_TC(RunSmsSendAfterDenitSms);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.16 SMS send with message contain NULL character so the data after NULL character should not received */
        Retcode = TEST_REGISTER_TC(RunSmsSendWithNullData);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.17 SMS send with mobile number of 20 digits and message of 160 byte */
        Retcode = TEST_REGISTER_TC(RunSmsSendWithExceedNum);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.18 Power Off the modem immediately after sending SMS */
        Retcode = TEST_REGISTER_TC(RunSmsSendAndPoweroff);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.10.19 Sending SMS to a mobile number of length 6 digit with max data limit(160 bytes) */
        Retcode = TEST_REGISTER_TC(RunSmsSendWith6DigitNo);
    }

    return (Retcode);
}
