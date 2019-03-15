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
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_UDP_TEST_SUITE

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Tests.h"
#include "BCDS_TestConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <projdefs.h>
#include "BCDS_TestSuite.h"
#include "CellularSupport.h"
#include "LoadTestSuite.h"
#include "UdpTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_UDP_ID

static CellularUdpSocket_T SocketId = -1;
CellularUdpSocket_T SocketIdNum[CELLULAR_SOCKET_COUNT] = {-1,-1,-1}; /* Upto 3 received Socket Id be kept in this variable   */
static uint8_t Data[CELLULAR_TX_BUFFER_SIZE] = {0};

/**
 * @brief   The function Check for the reliability of udp data receive operation.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunUdpSocketReceive(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for the reliability of open the one socket and data send
 * to the opened socket for 100 thousand times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunUdpSocketSend100KTimes(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for the reliability of open the one socket and send
 * with 185 bytes (Max no of bytes) of data to the opened socket for 100 thousand times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunUdpSendReceive100KTimes(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for the reliability of open the all three socket at a time and send
 * with 185 bytes (Max no of bytes) of data to all 3 sockets one by one for thousand times
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunUdp3SocketOpenSend1KTimes(CCMsg_T* MessagePtr);

/**
 * @brief   The function Check for the reliability of open the socket and send
 * with more than 185 bytes of data for 100 times.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunUdpSendExceedDataLength(CCMsg_T* MessagePtr);

static void RunUdpSocketReceive(CCMsg_T* MessagePtr)
{
    Retcode_T Rc = RETCODE_OK;
    uint8_t ReceiveFailCount = 0;
    uint8_t PacketMismatchCount = 0;
    uint32_t ErrorCode;

    BCDS_UNUSED(MessagePtr);

    for(uint8_t i = 0; i < NUMBER_OF_UDP_RECEIVE; i++)
    {
    	Rc = CellularSupport_CheckForReceivedUdpData(SocketId, (uint8_t*)"COSP_RX", 0);
    	if(RETCODE_OK != Rc)
    	{
            ErrorCode = Retcode_GetCode(Rc);
    		if(RETCODE_SEMAPHORE_ERROR == ErrorCode)
    		{
    			ReceiveFailCount++;
    		}
    		else
    		{
    			PacketMismatchCount++;
    		}
    	}
    	/* we validate based on tolerance here.
    	 * Hence the status of the last trigger
    	 * is of no benefit. */
    	Rc = RETCODE_OK;
    }

    if(UDPRECEIVEFAILURE_TOLERANCE(NUMBER_OF_UDP_RECEIVE) < ReceiveFailCount)
    {
    	Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKETS_LOST_DURING_RX);
    	Tests_SendReport(Rc, "One or more packets are not received by the device");
    }
    else if(UDPMISMATCHFAILURE_TOLERANCE(NUMBER_OF_UDP_RECEIVE) < PacketMismatchCount)
    {
    	Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKET_MISMATCH);
    	Tests_SendReport(Rc, "The received packet does not match the transmitted packet");
    }
    else
    {
    	Tests_SendReport(Rc, "Packet received successfully");
    }
}

static void RunUdpSocketReceiveAfterFourMinDelay(CCMsg_T* MessagePtr)
{
    Retcode_T Rc = RETCODE_OK;
    uint32_t ErrorCode = 0;

    BCDS_UNUSED(MessagePtr);

    Rc = CellularSupport_CheckForReceivedUdpData(SocketId, (uint8_t*)"COSP_RX after delay of 4 minutes", FOUR_MIN_DELAY_PERIOD_IN_MS);

    ErrorCode = Retcode_GetCode(Rc);

    /* We are expecting semaphore error for this test case as an error code. Hence updating the return code accordingly. */
    if(RETCODE_SEMAPHORE_ERROR != ErrorCode)
    {
        Rc = RETCODE_FAILURE;
        Tests_SendReport(Rc, "Failure to meet the expectation");
        return;
    }

    Tests_SendReport(RETCODE_OK, "Success");
}

static void RunUdpSocketReceiveAfter30SecDelay(CCMsg_T* MessagePtr)
{
    Retcode_T Rc = RETCODE_OK;

    BCDS_UNUSED(MessagePtr);

    Rc = CellularSupport_CheckForReceivedUdpData(SocketId, (uint8_t*)"COSP_RX after delay of 30 sec", THIRTY_SEC_DELAY_PERIOD_IN_MS );

    if(RETCODE_OK != Rc)
	{
		Tests_SendReport(Rc, "Packet receive fail");
		return;
	}

    Tests_SendReport(Rc, "Packet received");
}

static void RunUdpSocketSend100KTimes(CCMsg_T* MessagePtr)
{
    uint32_t Index = UINT32_C(0);
    uint32_t DataIndex = UINT32_C(0);
    uint32_t SendDataLength= UINT32_C(14);
    Retcode_T Rc = RETCODE_OK;
    uint32_t ReceiveFailCount = 0;
    uint32_t PacketMismatchCount = 0;
    uint32_t ErrorCode;
    uint8_t DataValue = UINT32_C(1);

    memset(Data,UINT32_C(0),CELLULAR_UDP_SOCKET_SIZE);

    Rc = CellularSupport_SocketOpen(MessagePtr, &SocketId);
    if(RETCODE_OK != Rc)
    {
        Tests_SendReport(Rc, "socket open failed");
        return;
    }

	for(uint32_t Count = UDP_SCKT_SEND_REPEAT_COUNT; Count > UINT32_C(0); Count--)
	{

		for(Index = DataIndex; Index < (DataIndex + SendDataLength); Index ++)
		{
			Data[Index] = DataValue;
		}

		DataIndex = Index;

		/* Send incremental data to the udp server */
		Rc = CellularUdpSocket_Send(SocketId, (const uint32_t) SOCKET_SERVER_IP,SOCKET_SERVER_PORT, Data, (uint32_t) Index);

		if(RETCODE_OK != Rc)
		{
			Tests_SendReport(Rc, CellularSupport_StringAttach("UDP socket send failed", Rc));
		}

		/* Receive echoed data from the UDP server */
		Rc = CellularSupport_CheckForReceivedUdpData(SocketId,Data, 0);

    	if(RETCODE_OK != Rc)
    	{
            ErrorCode = Retcode_GetCode(Rc);
    		if(RETCODE_SEMAPHORE_ERROR == ErrorCode)
    		{
    			ReceiveFailCount++;
    		}
    		else
    		{
    			PacketMismatchCount++;
    		}
    	}
	}

    /* socket close */
    Rc = CellularSupport_SocketClose(MessagePtr, SocketId);
    if (RETCODE_OK != Rc)
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    /* Failure has a tolerance of 10 percentage, after that it will be treated as a failure */
    if(UDPRECEIVEFAILURE_TOLERANCE(UDP_SCKT_SEND_REPEAT_COUNT) < ReceiveFailCount )
    {
    	Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKETS_LOST_DURING_RX);
    	Tests_SendReport(Rc, "Some packets are not received by the device");
    }
    else if(UDPMISMATCHFAILURE_TOLERANCE(UDP_SCKT_SEND_REPEAT_COUNT) < PacketMismatchCount)
    {
    	Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKET_MISMATCH);
    	Tests_SendReport(Rc, "The received packet does not match the transmitted packet");
    }
    else
    {
    	Tests_SendReport(Rc, "Packet received successfully");
    }

}

static void RunUdpSendReceive100KTimes(CCMsg_T* MessagePtr)
{
    uint32_t Count = UINT32_C(0);
    Retcode_T Rc = RETCODE_OK;
    uint32_t ReceiveFailCount = 0;
    uint32_t PacketMismatchCount = 0;
    uint32_t ErrorCode;
    uint8_t Datavalue = UINT32_C(0);

    Rc = CellularSupport_SocketOpen(MessagePtr, &SocketId);
    if(RETCODE_OK != Rc)
    {
        Tests_SendReport(Rc, "socket open failed");
        return;
    }

    for(Count = UDP_SCKT_SEND_REPEAT_COUNT; Count > 0; Count--)
    {
        for (uint32_t Dataindex =  UINT32_C(0); Dataindex < CELLULAR_UDP_SOCKET_SIZE; Dataindex++ )
        {
            if(Datavalue >127)
            	Datavalue =  UINT32_C(0);

            Data[Dataindex] = Datavalue;
            Datavalue ++;

        }

		/* Send constant 1472 bytes of data to the udp server */
		Rc = CellularUdpSocket_Send(SocketId, (const uint32_t) SOCKET_SERVER_IP,SOCKET_SERVER_PORT, Data, (uint32_t) CELLULAR_UDP_SOCKET_SIZE);

		if(RETCODE_OK != Rc)
		{
			Tests_SendReport(Rc, CellularSupport_StringAttach("UDP socket send failed", Rc));
		}

		/* Receive echoed data from the UDP server */
		Rc = CellularSupport_CheckForReceivedUdpData(SocketId,Data, 0);

    	if(RETCODE_OK != Rc)
    	{
            ErrorCode = Retcode_GetCode(Rc);
    		if(RETCODE_SEMAPHORE_ERROR == ErrorCode)
    		{
    			ReceiveFailCount++;
    		}
    		else
    		{
    			PacketMismatchCount++;
    		}
    	}
    }

    /* socket close */
    Rc = CellularSupport_SocketClose(MessagePtr, SocketId);
    if (RETCODE_OK != Rc)
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    /* Failure has a tolerance of 10 percentage, after that it will be treated as a failure */
    if(UDPRECEIVEFAILURE_TOLERANCE(UDP_SCKT_SEND_REPEAT_COUNT) < ReceiveFailCount)
    {
    	Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKETS_LOST_DURING_RX);
    	Tests_SendReport(Rc, CellularSupport_StringAttach("Some packets are not received by the device",ReceiveFailCount));
    }
    else if(UDPMISMATCHFAILURE_TOLERANCE(UDP_SCKT_SEND_REPEAT_COUNT) < PacketMismatchCount)
    {
    	Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKET_MISMATCH);
    	Tests_SendReport(Rc, CellularSupport_StringAttach("The received packet does not match the transmitted packet",PacketMismatchCount));
    }
    else
    {
    	Tests_SendReport(Rc, "Packet received successfully");
    }
}

static void RunUdp3SocketOpenSend1KTimes(CCMsg_T* MessagePtr)
{
	Retcode_T Rc = RETCODE_OK;
	uint32_t SendCount = UINT32_C(0);
	uint32_t SocketOpenCount = UINT32_C(0);
    uint32_t PacketSendFailCount = UINT32_C(0);

    BCDS_UNUSED(MessagePtr);

	for (SocketOpenCount = 0; SocketOpenCount < CELLULAR_SOCKET_COUNT; SocketOpenCount++)
	{
		for (SendCount = 0; SendCount < UDP_SCKT_SEND_REPEAT_COUNT; SendCount++)
		{
			Rc = CellularSupport_UdpSendReqBytes(MessagePtr,SocketIdNum[SocketOpenCount],CELLULAR_UDP_SOCKET_SIZE);

			if (RETCODE_OK != Rc)
			{
				PacketSendFailCount++;
			}
			vTaskDelay(UDP_SEND_DELAY_50_MILLISECS / portTICK_RATE_MS);
		}
	}

	if(UDP_PACKET_FAILURE_COUNT <= PacketSendFailCount)
	{
		Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKET_TX_FAIL);
		Tests_SendReport(Rc, CellularSupport_StringAttach("Packet Send Fail Count", PacketSendFailCount));
		return;
	}

	Tests_SendReport(RETCODE_OK,CellularSupport_StringAttach("Packet sent successfully", PacketSendFailCount));
}

static void RunUdpSendExceedDataLength(CCMsg_T* MessagePtr)
{
	uint32_t Count = UINT32_C(0);
	Retcode_T Rc = RETCODE_OK;
	bool PacketSendSuccessFlag = false;
	uint8_t DataValue = UINT32_C(1);
	memset(Data,UINT32_C(0),UDP_SOCKET_SEND_LENGTH_EXCEED);

	Rc = CellularSupport_SocketOpen(MessagePtr, &SocketId);
    if(RETCODE_OK != Rc)
    {
        Tests_SendReport(Rc, "socket open failed");
        return;
    }

	for (uint32_t Index = 0; Index < UDP_SOCKET_SEND_LENGTH_EXCEED; Index++ )
	{
		Data[Index] = DataValue;
	}

	for(Count = UDP_SCKT_SEND_REPEAT_COUNT; Count > 0; Count--)
	{
		/* udp Data send */
		Rc = CellularUdpSocket_Send(SocketId, (const uint32_t) SOCKET_SERVER_IP,
		        		SOCKET_SERVER_PORT, Data, UDP_SOCKET_SEND_LENGTH_EXCEED);

		if (RETCODE_OK == Rc)
		{
		    PacketSendSuccessFlag = true;
		    break;
		}
		vTaskDelay(UDP_SEND_DELAY_50_MILLISECS / portTICK_RATE_MS);
	}

    /* socket close */
    Rc = CellularSupport_SocketClose(MessagePtr, SocketId);
    if (RETCODE_OK != Rc)
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    if(true == PacketSendSuccessFlag)
	{
		Rc = RETCODE(RETCODE_SEVERITY_ERROR, CELLULAR_TESTS_PACKET_TX_FAIL);
		Tests_SendReport(Rc, "Packet Send was successful for 1473 bytes");
		return;
	}

	Tests_SendReport(RETCODE_OK, "Success");
}

Retcode_T UdpTestSuite_Init(void)
{
	Retcode_T Rc = RETCODE_OK;

	Rc = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == Rc)
    {
    	Rc = TEST_REGISTER_TC(RunUdpSocketReceive);
    }

    if (RETCODE_OK == Rc)
	{
		Rc = TEST_REGISTER_TC(RunUdpSocketReceiveAfterFourMinDelay);
	}

	if (RETCODE_OK == Rc)
	{
		Rc = TEST_REGISTER_TC(RunUdpSocketReceiveAfter30SecDelay);
	}

    if (RETCODE_OK == Rc)
    {
        Rc = TEST_REGISTER_TC(RunUdpSocketSend100KTimes);
    }

    if (RETCODE_OK == Rc)
	{
		Rc = TEST_REGISTER_TC(RunUdpSendReceive100KTimes);
	}

    if (RETCODE_OK == Rc)
	{
		Rc = TEST_REGISTER_TC(RunUdp3SocketOpenSend1KTimes);
	}

    if (RETCODE_OK == Rc)
	{
		Rc = TEST_REGISTER_TC(RunUdpSendExceedDataLength);
	}

	return Rc;
}
