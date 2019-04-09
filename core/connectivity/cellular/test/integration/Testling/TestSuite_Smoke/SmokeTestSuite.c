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

#include "BCDS_TestlingInfo.h"
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_SMOKE_TEST_SUITE

/* Include required Header files */
#include "BCDS_Tests.h"
#include "BCDS_Basics.h"
#include "BCDS_TestConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "CellularSupport.h"
#include "SmokeTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_SMOKE_ID

static CellularUdpSocket_T SocketId = -1;
extern SemaphoreHandle_t SemaphoreHandle;
extern Cellular_CallbackEvent_T ParseEvent;

/**
 * @brief   The function Check for the reliability of Power on and OFF operation.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmokeOnOff(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for the reliability of Power reset operation.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmokeReset(CCMsg_T* MessagePtr);

/**
* @brief   The function tries to run the GPRS Connection and sends status
* by checking Network_Init(),GPRSConnect() return values
*
* @param[in] MessagePtr
* A pointer to hold the TLV element structure
*
*
*/
static void RunSmokeGPRSConnect(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for the reliability of udp data receive operation.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmokeUdpSocketSend(CCMsg_T* MessagePtr);

/**
 * @brief  The function checks the functionality of sending the message
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmokeSmsSendText(CCMsg_T* MessagePtr);


/**
 * @brief   The function Check for the reliability of sending and receiving
 * an extended ASCII character.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSmokeUdpSendReceiveExtendedAscii(CCMsg_T* MessagePtr);

/** The API is described while declaring the function(i.e., above)  */
static void RunSmokeOnOff(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, "MessagePtr is null");
        return;
    }
    else
    {
        retcode = CellularSupport_TurnOn(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "ON fail");
            return;
        }
        retcode = CellularSupport_TurnOff(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "OFF fail");
            return;
        }
    }

    Tests_SendReport(retcode, "PASS");
    return;
}

/** The API is described while declaring the function(i.e., above)  */
static void RunSmokeReset(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T retcode = RETCODE_OK;
    int8_t ErrorCode;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, "MessagePtr is null");
        return;
    }
    else
    {
        retcode = CellularSupport_TurnOn(MessagePtr);
        if (RETCODE_OK != retcode)
        {
            Tests_SendReport(retcode, "ON fail");
            return;
        }

        retcode = CellularSupport_Reset(MessagePtr);
		if (RETCODE_OK != retcode)
		{
			Tests_SendReport(retcode, "Reset fail");
			return;
		}

		ErrorCode = xSemaphoreTake(SemaphoreHandle,
				(NETWORK_INIT_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));

		if (pdPASS != ErrorCode) {
			Tests_SendReport(retcode, "Event fail");
			return;
		}

		if ((CELLULAR_EVENT_INITIALIZED == ParseEvent) || (CELLULAR_EVENT_INITIALIZING == ParseEvent))
	    {
			retcode = RETCODE_OK;
	    }
		else
		{
			retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
			Tests_SendReport(retcode, "Init event not received");
		}
    }

    Tests_SendReport(retcode, "PASS");

    return;
}

/** The API is described while declaring the function(i.e., above)  */
static void RunSmokeGPRSConnect(CCMsg_T* MessagePtr)
{

    Retcode_T retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, "MessagePtr is null");
        return;
    }

    retcode = CellularSupport_NetworkInit(MessagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "Network Init Fail");
        return;
    }

    retcode = CellularSupport_NetworkConfig(MessagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "Network Config Fail");
        return;
    }

    retcode = CellularSupport_Connect(MessagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "Network Connect Fail");
        return;
    }

    retcode = CellularSupport_DisConnect(MessagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "Network disconnect Fail");
        return;
    }

    Tests_SendReport(retcode, "PASS");
    return;
}

/** The API is described while declaring the function(i.e., above)  */
static void RunSmokeUdpSocketSend(CCMsg_T* MessagePtr)
{
    Retcode_T retcode = (Retcode_T) RETCODE_OK;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    if (RETCODE_OK != CellularSupport_SocketOpen(MessagePtr, &SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Open fail");
        return;
    }

    if (RETCODE_OK != CellularSupport_UdpSendReceive(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Packet Send fail");
        return;
    }

    if (RETCODE_OK != CellularSupport_SocketClose(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

static void RunSmokeSmsSendText(CCMsg_T* MessagePtr)
{
    Retcode_T Retcode = RETCODE_OK;
    uint32_t Index;
    uint8_t MobileNumber[16] = "+918220015497";
    uint8_t TextMessage[161];
    memset(TextMessage,'\0',sizeof(TextMessage));

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(Retcode, " input parameter null ");
        return;
    }
    for(Index = SMOKE_TEST_INDEX_VALUE_0; Index < SMOKE_TEST_LENGTH_VALUE_160; Index++)
    {
        TextMessage[Index] = 'a';
    }

	Retcode = CellularSupport_SmsSendText(MobileNumber,TextMessage,strlen((const char*) TextMessage));
	if (RETCODE_OK != Retcode)
	{
		Tests_SendReport(Retcode, "Fail: SMS send fail");
		return;
	}

	Tests_SendReport(RETCODE_OK, "PASS");
}


static void RunSmokeUdpSendReceiveExtendedAscii(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    uint8_t DataToSend[4] = {'D', 'o', 'j', 'o'};
    uint16_t ECHO_SERVER_PORT = UINT16_C(13370);

    Rc = CellularSupport_SocketOpen(MessagePtr, &SocketId);
    if(RETCODE_OK != Rc)
    {
        Tests_SendReport(Rc, "socket open failed");
        return;
    }

    /* udp data send */
    Rc = CellularUdpSocket_Send(SocketId, (const uint32_t) SOCKET_SERVER_IP,
            ECHO_SERVER_PORT, DataToSend, (uint32_t) SOCKET_UDP_DATA_LENGTH);

    if (RETCODE_OK != Rc)
    {
        Tests_SendReport(Rc, "Socket Send fail");
        return;
    }

    Rc = CellularSupport_CheckForReceivedUdpData(SocketId,DataToSend, 0);
    if (RETCODE_OK != Rc)
    {
        Tests_SendReport(Rc, "check for received udp data failed");
        return;
    }

    /* socket close */
    Rc = CellularSupport_SocketClose(MessagePtr, SocketId);
    if (RETCODE_OK != Rc)
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    Tests_SendReport(Rc, NULL);
}

/** The description is in the public header */
Retcode_T SmokeTestSuite_Init(void)
{
	Retcode_T Retcode = RETCODE_OK;

	Retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == Retcode)
    {
    	/*TC 1.1.1 Turn modem ON and OFF */
    	Retcode = TEST_REGISTER_TC(RunSmokeOnOff);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.1.2 Reset modem, after Modem ON */
    	Retcode = TEST_REGISTER_TC(RunSmokeReset);
    }

    if (RETCODE_OK == Retcode)
    {
        /* TC 1.1.3 Init, Configure, Connect, Disconnect the Cellular network */
    	Retcode = TEST_REGISTER_TC(RunSmokeGPRSConnect);
    }

	if (RETCODE_OK == Retcode)
	{
		 /* TC 1.1.4 Socket open, send, receive, close */
		Retcode = TEST_REGISTER_TC(RunSmokeUdpSocketSend);
	}

	if (RETCODE_OK == Retcode)
	{
		 /* TC 1.1.5 Send SMS to a valid mobile number */
		Retcode = TEST_REGISTER_TC(RunSmokeSmsSendText);
	}

    if (RETCODE_OK == Retcode)
    {
        Retcode = TEST_REGISTER_TC(RunSmokeUdpSendReceiveExtendedAscii);
    }

	return Retcode;
}
