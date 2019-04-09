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
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_CELLULAR_SUPPORT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "BCDS_TestSuite.h"
#include "BCDS_TestConfig.h"
#include "BCDS_Tests.h"
#include <FreeRTOS.h>
#include <stdio.h>
#include <semphr.h>
#include <projdefs.h>
#include <task.h>
#include "BCDS_TestConfig.h"
#include "CellularSupport.h"


extern SemaphoreHandle_t SemaphoreHandle; /* Semaphore assigned to a variable of type SemaphoreHandle_t  */
Cellular_CallbackEvent_T ParseEvent = CELLULAR_EVENT_MAX; /* Cellular Callback Event status such as Cellular Initializing,Initialized,Data Received,Connect,Disconnect,Error */
bool ErrorEventOccured=FALSE; /* To capture the error event */
Cellular_CallbackEvent_T ParseSmsEvent; /* Sms Send Event status as text message sent or not */
uint8_t ParseSmsStatus;	/* Sms Send status as text message sent successfully or not */
CellularUdpEvent_T ParseSocketEvent; /* Udp Packet received Event  status as Packet Received or Error*/
uint8_t CellularUdpData[CELLULAR_UDP_SOCKET_SIZE]; /* Cellular UDP data packet */

static char StringAttachLocalBuffer[1024];
static uint8_t *ReceivedData;
static uint32_t Length;
static CellularUdpSocket_T SocketValueOnNotifyCallback;

/* Put private function declarations here */
void ParseEventCallback(Cellular_CallbackEvent_T event, void *info)
{
    (void) info;
    ParseEvent = event;
	if (CELLULAR_EVENT_ERROR == event){
		ErrorEventOccured=TRUE;
	}
    xSemaphoreGive(SemaphoreHandle);
}

void SmsEventCallback(Sms_Event_T Event, void* Info)
{
	int8_t SemTakeStatus;
	if((Sms_Event_T)CELLULAR_EVENT_SMS == Event)
	{
		ParseSmsStatus = *(uint8_t *)Info;
		ParseSmsEvent = Event;
		SemTakeStatus = xSemaphoreGive(SemaphoreHandle);
		if (pdTRUE != SemTakeStatus)
		{
			Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_SEMAPHORE_ERROR));
		}
	}
	else
	{
		ParseSmsEvent = CELLULAR_EVENT_ERROR;
		Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_UNEXPECTED_BEHAVIOR));
	}
}

void ParseSocketCallback(CellularUdpSocket_T socket, CellularUdpEvent_T event)
{
    uint32_t Ipaddress;
    uint16_t Port;
    uint8_t **ReceivedDataPtr;
	SocketValueOnNotifyCallback = socket;
    ParseSocketEvent = event;

    ReceivedDataPtr = &ReceivedData;

    if (CELLULAR_UDP_EVENT_DATA_RECEIVED == ParseSocketEvent)
    {
        CellularUdpSocket_Receive(socket, &Ipaddress, &Port,
                ReceivedDataPtr, &Length);
        xSemaphoreGive(SemaphoreHandle);
    }

    else if (CELLULAR_UDP_EVENT_SOCKET_CLOSE == ParseSocketEvent)
    {
        xSemaphoreGive(SemaphoreHandle);
    }

    else
    {
        ParseSocketEvent = CELLULAR_UDP_EVENT_SOCKET_ERROR;
    }

}

/* Function description is in public header */
Retcode_T CellularSupport_TurnOn(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    int8_t SemTakeStatus;
    Retcode_T Rc = RETCODE_OK;
    uint8_t Index = 0U;
    bool InitStatus = false;

    ParseEvent = CELLULAR_EVENT_MAX;

    Rc = Cellular_PowerControl(CELLULAR_POWER_STATE_ON);

    if (RETCODE_OK == Rc)
    {
        for(Index = 0U; Index < 2U; Index++)
        {
        	SemTakeStatus = xSemaphoreTake(SemaphoreHandle, (NETWORK_INIT_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));
        	if (pdTRUE == SemTakeStatus)
        	{
                if ((CELLULAR_EVENT_INITIALIZED == ParseEvent) || (CELLULAR_EVENT_INITIALIZING == ParseEvent))
                {
                	InitStatus = true;
            		break;
                }
        	}
        }
        if(true != InitStatus)
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
        }
    }

    return (Rc);

}

/* Function description is in public header */
Retcode_T CellularSupport_NetworkInit(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    int8_t SemTakeStatus;
    uint8_t Index = 0U;

    ParseEvent = CELLULAR_EVENT_MAX;

    Rc = Cellular_NetworkInit();

    if (RETCODE_OK == Rc)
    {
        for(Index = 0U; Index < 2U; Index++)
        {
        	SemTakeStatus = xSemaphoreTake(SemaphoreHandle, (NETWORK_INIT_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));
        	if (pdTRUE == SemTakeStatus)
        	{
                if (CELLULAR_EVENT_INITIALIZED == ParseEvent)
                {
            		break;
                }
        	}
        }
        if((CELLULAR_EVENT_INITIALIZED != ParseEvent) || (pdTRUE != SemTakeStatus))
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
        }
    }

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_NetworkConfig(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    memcpy(ConfigSetting.ApnName, APN_NAME, strlen(APN_NAME));
    memset(ConfigSetting.ApnUserName, '\0', sizeof(ConfigSetting.ApnUserName));
    memset(ConfigSetting.ApnPassword, '\0', sizeof(ConfigSetting.ApnPassword));

    Rc = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_Connect(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    int8_t SemTakeStatus;

    /* connect with n/w (gprs connection) */
    Rc = Cellular_Connect();

    if (RETCODE_OK == Rc)
    {
        /* wait for connected event */

        SemTakeStatus = xSemaphoreTake(SemaphoreHandle, (NETWORK_CONNECT_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));

        if (pdTRUE == SemTakeStatus)
        {
            if (CELLULAR_EVENT_CONNECTED != ParseEvent)
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
            }
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        }
    }

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_DisConnect(CCMsg_T* MessagePtr)
{

    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    int8_t SemTakeStatus;

    /* disconnect the gprs connection */
    Rc = Cellular_Disconnect();

    if (RETCODE_OK == Rc)
    {
        SemTakeStatus = xSemaphoreTake(SemaphoreHandle, (NETWORK_DISCONNECT_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));

        if (pdTRUE == SemTakeStatus)
        {
            if (CELLULAR_EVENT_DISCONNECTED != ParseEvent)
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
            }
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        }

    }

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_TurnOff(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    uint8_t Index = 0U;

    Rc = Cellular_PowerControl(CELLULAR_POWER_STATE_OFF);

    /* Disconnection and socket close event */
    for(Index = 0U; Index < 2U; Index++)
    {
        xSemaphoreTake(SemaphoreHandle, (EVENT_DURING_POWER_OFF / portTICK_RATE_MS));
    }

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_Reset(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;

    Rc = Cellular_Control(CELLULAR_RESET,NULL);

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_SocketOpen(CCMsg_T* MessagePtr, CellularUdpSocket_T *socketid)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;

    /* socket open */
    Rc = CellularUdpSocket_Open(socketid, ParseSocketCallback);

    if ((RETCODE_OK == Rc) && (*socketid < 0))
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_SocketClose(CCMsg_T* MessagePtr, CellularUdpSocket_T socketid)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;

    /*socket close*/
    Rc = CellularUdpSocket_Close(socketid);

    return (Rc);
}

Retcode_T CellularSupport_CustomDataSend(CCMsg_T* MessagePtr, CellularUdpSocket_T Socketid,uint8_t *UdpDatabuffer,uint32_t DataLength)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;

    /* udp data send */
    Rc = CellularUdpSocket_Send(Socketid, (const uint32_t) SOCKET_SERVER_IP,
            SOCKET_SERVER_PORT, UdpDatabuffer, (uint32_t) DataLength);

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_UdpSend(CCMsg_T* MessagePtr, CellularUdpSocket_T Socketid)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    uint8_t data[SOCKET_UDP_DATA_LENGTH] = { SOCKET_UDP_DATA };

    /* udp data send */
    Rc = CellularUdpSocket_Send(Socketid, (const uint32_t) SOCKET_SERVER_IP,
            SOCKET_SERVER_PORT, data, (uint32_t) SOCKET_UDP_DATA_LENGTH);

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_UdpSendReceive(CCMsg_T* MessagePtr, CellularUdpSocket_T socketid)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    int8_t SemTakeStatus;
    uint8_t data[SOCKET_UDP_DATA_LENGTH] = { SOCKET_UDP_DATA };

    /* udp data send */
    Rc = CellularUdpSocket_Send(socketid, (const uint32_t) SOCKET_SERVER_IP,
            SOCKET_SERVER_PORT, data, (uint32_t) SOCKET_UDP_DATA_LENGTH);

    if (RETCODE_OK == Rc)
	{
        SemTakeStatus = xSemaphoreTake(SemaphoreHandle, (SOCKET_SEND_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));

        if (pdTRUE == SemTakeStatus)
        {
            if (CELLULAR_UDP_EVENT_DATA_RECEIVED != ParseSocketEvent)
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        }
	}

    return (Rc);
}

/* Function description is in public header */
Retcode_T CellularSupport_UdpSendReqBytes(CCMsg_T* MessagePtr, CellularUdpSocket_T Socketid,uint32_t DataLength)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Rc = RETCODE_OK;
    int8_t SemTakeStatus;

    for (uint32_t Index = 0; Index < DataLength; Index++ )
    {
        CellularUdpData[Index] = Index;
    }

    /* udp data send */
    Rc = CellularUdpSocket_Send(Socketid, (const uint32_t) SOCKET_SERVER_IP,
            SOCKET_SERVER_PORT, CellularUdpData, (uint32_t) DataLength);

    if(RETCODE_OK == Rc)
    {
        SemTakeStatus = xSemaphoreTake(SemaphoreHandle, (SOCKET_SEND_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));
        if (pdTRUE == SemTakeStatus)
        {
            if (CELLULAR_UDP_EVENT_DATA_RECEIVED != ParseSocketEvent)
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        }
    }

    return (Rc);
}

char* CellularSupport_StringAttach(char* header, int value)
{
    memset(StringAttachLocalBuffer, '\0', sizeof(StringAttachLocalBuffer));
	snprintf(StringAttachLocalBuffer, sizeof StringAttachLocalBuffer, "%s:%d:", header, value);
	return (char*)StringAttachLocalBuffer;
}


Retcode_T CellularSupport_CheckForReceivedUdpData(CellularUdpSocket_T socketid, uint8_t* ReferenceData, uint32_t WaitTimeInMs)
{
	BCDS_UNUSED(socketid);
    int8_t SemTakeStatus;
    Retcode_T Rc = RETCODE_OK;
    char *ReceivedDataIndexPtr;

    /* Reset the data pointer, event and length before evaluation */
    ParseSocketEvent = CELLULAR_UDP_EVENT_SOCKET_ERROR;
    Length = 0;
    ReceivedData = (uint8_t*)"\0";

    SemTakeStatus = xSemaphoreTake(SemaphoreHandle, ((SOCKET_RECEIVE_TIMEOUT_MILLI_SECS + WaitTimeInMs) / portTICK_RATE_MS));

    if (pdTRUE == SemTakeStatus)
    {
        if (CELLULAR_UDP_EVENT_DATA_RECEIVED != ParseSocketEvent)
        {
        	Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
        else
        {
        	ReceivedDataIndexPtr = strstr((const char*)ReceivedData, (const char*)"DATA RECEIVED: ");
        	if(ReceivedDataIndexPtr != NULL)
        	{
        		ReceivedData = (uint8_t *) ReceivedDataIndexPtr + 15U;
        	}

        	if(0UL != strncmp((const char*)ReferenceData, (const char*)ReceivedData, Length))
        	{
        		Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKET_MISMATCH);
        	}
        }
    }
    else
    {
    	Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
    }

    return (Rc);
}

Retcode_T CellularSupport_SmsSendText(uint8_t *SmsNumber, uint8_t *TextData, uint32_t TextDataLength)
{
	Retcode_T Rc = RETCODE_OK;
	int8_t SemTakeStatus;

	Rc = CellularSms_SendText(SmsNumber,TextData,TextDataLength);
	if(RETCODE_OK == Rc)
	{
		SemTakeStatus = xSemaphoreTake(SemaphoreHandle, (SMS_SEND_TIMEOUT_MILLI_SECS / portTICK_RATE_MS));
		if (pdTRUE == SemTakeStatus)
		{
			if (CELLULAR_EVENT_SMS != ParseSmsEvent)
			{
				Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
			}
			else
			{
				if(SMS_STATUS_SUCCESS != ParseSmsStatus)
				{
					Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
				}
			}
		}
		else
		{
			Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
		}
	}

	return (Rc);
}


void RunTurnOn(CCMsg_T* messagePtr)
{
    Retcode_T retcode;

    if (NULL == messagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    retcode = CellularSupport_TurnOn(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "TurnOn fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

void RunNetworkInit(CCMsg_T* messagePtr)
{
    Retcode_T retcode;

    if (NULL == messagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    retcode = CellularSupport_NetworkInit(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "NetworkInit fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

void RunNetworkConfigConnect(CCMsg_T* messagePtr)
{
    Retcode_T retcode;

    if (NULL == messagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    retcode = CellularSupport_NetworkConfig(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "NetworkConfig fail");
        return;
    }

    retcode = CellularSupport_Connect(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "Connect fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

void RunTurnOnConnect(CCMsg_T* messagePtr)
{
    Retcode_T retcode;

    if (NULL == messagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    retcode = CellularSupport_TurnOn(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "TurnOn fail");
        return;
    }

    retcode = CellularSupport_NetworkInit(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "NetworkInit fail");
        return;
    }

    retcode = CellularSupport_NetworkConfig(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "NetworkConfig fail");
        return;
    }

    retcode = CellularSupport_Connect(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "Connect fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

void RunDisconnect(CCMsg_T* messagePtr)
{
    Retcode_T retcode;

    if (NULL == messagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    retcode = CellularSupport_DisConnect(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "Disconnect fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

void RunTurnOff(CCMsg_T* messagePtr)
{
    Retcode_T retcode;

    if (NULL == messagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    retcode = CellularSupport_TurnOff(messagePtr);
    if (RETCODE_OK != retcode)
    {
        Tests_SendReport(retcode, "TurnOff fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}

void RunDisconnectTurnOff(CCMsg_T* messagePtr)
{
    Retcode_T retcode1 = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER),
              retcode2 = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);

    if (NULL == messagePtr)
    {
        Tests_SendReport(retcode1, " input parameter null ");
        return;
    }

    retcode1 = CellularSupport_DisConnect(messagePtr);
    retcode2 = CellularSupport_TurnOff(messagePtr);

    if (RETCODE_OK != retcode1)
    {
        Tests_SendReport(retcode1, "Disconnect fail");
        return;
    }
    else if (RETCODE_OK != retcode2)
    {
        Tests_SendReport(retcode2, "TurnOff fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
}
