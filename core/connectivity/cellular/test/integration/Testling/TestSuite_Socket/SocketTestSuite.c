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
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_SOCKET_TEST_SUITE

#include "BCDS_Basics.h"
#include "BCDS_Tests.h"
#include "BCDS_Retcode.h"
#include "BCDS_TestSuite.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <projdefs.h>
#include <task.h>
#include "CellularSupport.h"
#include "SocketTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_SOCKET_ID

static CellularUdpSocket_T SocketId = -1;

/**
 * @brief   The function describes the Run for Socket Open Close Test Case
 * This test require Cellular Socket  as Run for Test Case of Socket Open and Close test with framework.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */

static void RunSocketOpenCloseTestCase(CCMsg_T* MessagePtr);

/**
 * @brief   The function describes the Run for Socket Open Close with 260 Seconds Delay Test Case
 * This test require Cellular Socket  as Run for Test Case of Socket Open and Close test with framework.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSocketOpenDly260SecClose(CCMsg_T* MessagePtr);

/**
 * @brief   The function describes the Run for Socket Open for three times and Close for Three Times Test Case
 * This test require Cellular Socket  as Run for Test Case of Socket Open and Close test with framework.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */

static void RunSocket3timesOpenClose(CCMsg_T* MessagePtr);

/**
 * @brief   The function describes the Run for Socket Open ,Send the Packet and Close the Socket
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSocketUdpSend(CCMsg_T* MessagePtr);

/**
 * @brief    The function describes the Run for Socket Open ,Send the Packet and Close the Socket for three times
 *
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */

static void RunSocketUdp3TimesDataSend(CCMsg_T* MessagePtr);

/**
 * @brief    The function describes the Run for Socket Open delay for 260 Secs ,Send the Packet and Close the Socket
 *
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSocketOpenDelayUdpSend(CCMsg_T* MessagePtr);

/**
 * @brief    The function describes the Run for Socket Open ,Send the Packet five times and Close the Socket
 *
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */

static void RunSocketUdpSend5Times(CCMsg_T* MessagePtr);

/**
 * @brief    The function describes the Run for Socket Open ,Send the Packet and Close the Socket after 260 Seconds Delay
 *
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSocketOpenUdpSendDlyClose(CCMsg_T* MessagePtr);

/**
 * @brief    The function describes the Run for Socket open, send data and close the Socket with a delay of 260 Sec
 *
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunSocketOpenUdpSendCloseDly(CCMsg_T* MessagePtr);

static void RunSocketOpenCloseTestCase(CCMsg_T* MessagePtr)
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

    if (RETCODE_OK != CellularSupport_SocketClose(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

/* The description is in the above */
static void RunSocketOpenDly260SecClose(CCMsg_T* MessagePtr)
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

    vTaskDelay(SOCKET_DELAY_260_SECS / portTICK_RATE_MS);

    if (RETCODE_OK != CellularSupport_SocketClose(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }
    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

/* The description is in the above */

static void RunSocket3timesOpenClose(CCMsg_T* MessagePtr)
{

    Retcode_T retcode = (Retcode_T) RETCODE_OK;

    int32_t count;
    int32_t SocketOpen;
    CellularUdpSocket_T SocketIdReceived[CELLULAR_SOCKET_COUNT]; /* Upto 3 received Socket Id be kept in this variable   */

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    for (count = 0; count < 3; count++)
    {
        /* socket open */
        SocketIdReceived[count] = -1;
        retcode = CellularSupport_SocketOpen(MessagePtr, &SocketIdReceived[count]);

        if (RETCODE_OK != retcode)
        {

            for (SocketOpen = 0; SocketOpen < count; SocketOpen++)
            {
                CellularSupport_SocketClose(MessagePtr, SocketIdReceived[SocketOpen]);
            }
            Tests_SendReport(RETCODE_FAILURE, "Socket Open fail");
            return;
        }
    }

    for (count = 0; count < 3; count++)
    {
        /* Socket Close */
        if (RETCODE_OK != CellularSupport_SocketClose(MessagePtr, SocketIdReceived[count]))
        {
            Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
            return;
        }

    }
    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

/* The description is in the above */
static void RunSocketUdpSend(CCMsg_T* MessagePtr)
{
    Retcode_T retcode = (Retcode_T) RETCODE_OK;
    uint8_t Index;
    uint8_t UdpDataLength;
    CCMsg_TlvElement_T GetTlvElement[SOCKET_GET_TLV_ELEMENT_SIZE];
    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    /* Get the tlvelement from TestCoordinator */
    GetTlvElement[0].type = SOCKET_FIRST_TLV_ELEMENT_TYPE;
    Tests_GetTlvElement(MessagePtr, &GetTlvElement[0]);

    for (Index = 0; Index < GetTlvElement[0].length; Index++)
    {
        CellularUdpData[Index] = GetTlvElement[0].value[Index];
    }

    /* Get the tlvelement from TestCoordinator */
    GetTlvElement[1].type = SOCKET_SECOND_TLV_ELEMENT_TYPE;
    Tests_GetTlvElement(MessagePtr, &GetTlvElement[1]);

    UdpDataLength = GetTlvElement[1].value[0];

    UdpDataLength = UdpDataLength - SOCKET_ASCII_TO_DECIMAL_VALUE;

    if (RETCODE_OK != CellularSupport_SocketOpen(MessagePtr, &SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Open fail");
        return;
    }

    if (RETCODE_OK != CellularSupport_CustomDataSend(MessagePtr, SocketId, CellularUdpData, (uint32_t) UdpDataLength))
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

/* The description is in the above */


static void RunSocketUdp3TimesDataSend(CCMsg_T* MessagePtr)
{
    Retcode_T retcode = (Retcode_T) RETCODE_OK;
    uint32_t CountValue = 0;

    if (NULL == MessagePtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(retcode, " input parameter null ");
        return;
    }

    for (CountValue = 0; CountValue < 3; CountValue++)
    {
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
    }
    Tests_SendReport(RETCODE_OK, "PASS");
    return;
}

/* The description is in the above  */
static void RunSocketOpenDelayUdpSend(CCMsg_T* MessagePtr)
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

    /* delay for 260 secs */
    vTaskDelay(SOCKET_DELAY_260_SECS / portTICK_RATE_MS);

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

/* The description is in the above  */

static void RunSocketUdpSend5Times(CCMsg_T* MessagePtr)
{
    Retcode_T retcode = (Retcode_T) RETCODE_OK;
    uint32_t Countvalue;

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

    for (Countvalue = 0; Countvalue < 5; Countvalue++)
    {
        if (RETCODE_OK != CellularSupport_UdpSendReceive(MessagePtr, SocketId))
        {
            Tests_SendReport(RETCODE_FAILURE, "Packet Send fail");
            return;
        }
    }

    if (RETCODE_OK != CellularSupport_SocketClose(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
    return;

}

/* The description is in the above */

static void RunSocketOpenUdpSendDlyClose(CCMsg_T* MessagePtr)
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

    /* delay for 260 secs */
    vTaskDelay(SOCKET_DELAY_260_SECS / portTICK_RATE_MS);

    if (RETCODE_OK != CellularSupport_SocketClose(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
    return;
}

/* The description is in the above  */

static void RunSocketOpenUdpSendCloseDly(CCMsg_T* MessagePtr)
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

    /* delay for 260 secs */
    vTaskDelay(SOCKET_DELAY_260_SECS / portTICK_RATE_MS);

    if (RETCODE_OK != CellularSupport_UdpSendReceive(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Packet Send fail");
        return;
    }

    /* delay for 260 secs */
    vTaskDelay(SOCKET_DELAY_260_SECS / portTICK_RATE_MS);

    if (RETCODE_OK != CellularSupport_SocketClose(MessagePtr, SocketId))
    {
        Tests_SendReport(RETCODE_FAILURE, "Socket Close fail");
        return;
    }

    Tests_SendReport(RETCODE_OK, "PASS");
    return;
}

Retcode_T SocketTestSuite_Init()
{

    Retcode_T retcode = RETCODE_OK;

    /* cellular on connect test section register */
    retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == retcode)
    {
        /* TC 1.3.1: Socket open and close */
        retcode = TEST_REGISTER_TC(RunSocketOpenCloseTestCase);
    }

    if (RETCODE_OK == retcode)
    {

        /* TC 1.3.2: Socket open(3) and close (3)*/
        retcode = TEST_REGISTER_TC(RunSocket3timesOpenClose);
    }

    if (RETCODE_OK == retcode)
    {

        /* TC 1.3.3: Socket open, time delay , close Socket */
        retcode = TEST_REGISTER_TC(RunSocketOpenDly260SecClose);
    }

    if (RETCODE_OK == retcode)
    {

        /* TC 1.3.4: Send data to Socket Opened */
        retcode = TEST_REGISTER_TC(RunSocketUdpSend);
    }

    if (RETCODE_OK == retcode)
    {

        /* TC 1.3.5: Send data to 3 Opened sockets */
        retcode = TEST_REGISTER_TC(RunSocketUdp3TimesDataSend);
    }

    if (RETCODE_OK == retcode)
    {

        /* TC 1.3.6: Send data to Socket Open with time delay */
        retcode = TEST_REGISTER_TC(RunSocketOpenDelayUdpSend);
    }

    if (RETCODE_OK == retcode)
    {

        /* TC 1.3.7: Send multiple (5) datum to Socket Opened */
        retcode = TEST_REGISTER_TC(RunSocketUdpSend5Times);
    }

    if (RETCODE_OK == retcode)
    {

        /* TC 1.3.8: Continuously Socket open, send data and close after delay of 260 Sec */
        retcode = TEST_REGISTER_TC(RunSocketOpenUdpSendDlyClose);
    }

    if (RETCODE_OK == retcode)
    {

        /* TC 1.3.9: Socket open, send data and close with a delay of 260 Sec */
        retcode = TEST_REGISTER_TC(RunSocketOpenUdpSendCloseDly);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunSocketTurnOnConnectId, NULL, RunTurnOnConnect, NULL);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunSocketDisconnectTurnOffId, NULL, RunDisconnectTurnOff, NULL);
    }

    return (retcode);
}
