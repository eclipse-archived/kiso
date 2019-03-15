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

/**
 * @file This file provide the wrapper APIs for Cellular driver.
 *
 * This file API will also validate the Cellular driver API functionality and provide the result.
 * Integration tests function should call these function and send the response to Test Manager.
 *
 */

#ifndef CellularSupport_H
#define CellularSupport_H

/* Include all headers which are needed by this file. */
#include <em_device.h>
#include "BCDS_Assert.h"
#include "BCDS_Tests.h"
#include "BCDS_TestSuite.h"
#include "BCDS_CellularConfig.h"
#include "BCDS_Cellular.h"
#include "BCDS_CellularUdpSocket.h"
#include "BCDS_CellularSms.h"

#define NETWORK_INIT_TIMEOUT_MILLI_SECS             UINT32_C(90000)                 /**< Network init time out in milliseconds */
#define NETWORK_CONNECT_TIMEOUT_MILLI_SECS          UINT32_C(80000)                 /**< Network connect time out in milliseconds */
#define NETWORK_DISCONNECT_TIMEOUT_MILLI_SECS       UINT32_C(80000)                 /**< Network disconnect time out in milliseconds */
#define EVENT_DURING_POWER_OFF						UINT32_C(10000)                 /**< Event received during Off (Disconnect and socket close) */
#define SMS_SEND_TIMEOUT_MILLI_SECS           		UINT32_C(30000)                 /**< Timeout for sending the text SMS data */

#if defined(RT_SERVER)
#define SOCKET_SERVER_IP                            UINT32_C(0xD95C216D )           /**< 0xD95C216D --echo serverRT * 223.181.199.214  0xDFB5C7D6  3753232342 ,223.190.139.202 --> CA8BBEDF,0x6ACE5C5D,0xDFBEB8BC */
#define SOCKET_SERVER_PORT                          UINT16_C(6600)                  /**< Port number on which server will listen 40505 ,13370 */
#define SOCKET_UDP_DATA_LENGTH                      UINT32_C(4)                     /**< sending udp Data length */
#define APN_NAME                                    "ppinternet.gdsp"
#else

#ifdef  BCDS_ECHO_SERVER_IP
#define SOCKET_SERVER_IP                            __REV(BCDS_ECHO_SERVER_IP)
#define SOCKET_UDP_DATA                             'T', 'e', 's','t','i','n','g','.'
#define SOCKET_UDP_DATA_LENGTH                      8
#else
#define SOCKET_SERVER_IP                            __REV((193 << 24) | (228 << 16) | (143 << 8) | 24)
#define SOCKET_UDP_DATA_LENGTH                      UINT32_C(48)
#define SOCKET_UDP_DATA                             UINT8_C(0x1B)
#endif /* BCDS_ECHO_SERVER_IP */

#ifdef  BCDS_ECHO_SERVER_PORT
#define SOCKET_SERVER_PORT                          BCDS_ECHO_SERVER_PORT
#else
#define SOCKET_SERVER_PORT                          UINT16_C(123)
#endif /* BCDS_ECHO_SERVER_PORT */

#ifdef  BCDS_NETWORK_APN

#define TOSTRING(x) #x
#define TOSTRING_VALUE_OF(x) TOSTRING(x)

#define APN_NAME                                    TOSTRING_VALUE_OF(BCDS_NETWORK_APN)
#else
#define APN_NAME                                    "4g.tele2.se"
#endif /* BCDS_NETWORK_APN */

#endif /* RT_SERVER */

#define SOCKET_ECHO_SERVER_IP                       SOCKET_SERVER_IP	           /**< Test Echo server IP address i.e., not same always */
#define SOCKET_ECHO_SERVER_PORT                     UINT16_C(13370)	               /**< Test Echo server port number i.e., not same always */

#define SOCKET_TESTCASE_TIMEOUT                     UINT32_C(260000)                /**< Timeout for socket testcases */


/* local type and macro definitions */

#define SOCKET_NWINIT_TIMEOUT_MILLI_SECS            UINT32_C(30000)                 /**< Network init time out in milliseconds */
#define SOCKET_CONNECT_TIMEOUT_MILLI_SECS           UINT32_C(30000)                 /**< Network connect time out in milliseconds */
#define SOCKET_DISCONNECT_TIMEOUT_MILLI_SECS        UINT32_C(30000)                 /**< Network disconnect time out in milliseconds */
#define SOCKET_OPEN_TIMEOUT_MILLI_SECS              UINT32_C(6000)
#define SOCKET_SEND_TIMEOUT_MILLI_SECS              UINT32_C(25000)
#define SOCKET_RECEIVE_TIMEOUT_MILLI_SECS           UINT32_C(25000)                 /**< Timeout for receiving data from server */
#define SOCKET_CLOSE_TIMEOUT_IN_MS              	UINT32_C(25000)                 /**< Timeout for socket close event reception from the modem */

#define TEST_REGISTER_TC(run)   Tests_RegisterTestCase(TEST_SUITE_ID, run ## Id, NULL, run, NULL)

/*lint -e19 -e514 */
static_assert((SOCKET_CLOSE_TIMEOUT_IN_MS != 0),"Socket close timeout must not be zero");
/*lint +e19 +e514 */

/**
 * @brief External reference to the Cellular UDP data packet
 */
extern uint8_t CellularUdpData[CELLULAR_UDP_SOCKET_SIZE];

/**
 * @brief  The function to Turn on the Cellular
 *
 * Wrapper function to turn on the modem. After turning on the modem,
 * It will wait and check for the modem events (Initialing or initialized evets)
 * If anyone the event occurs, modem booted successfully hence returns ok otherwise return failure.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 * @retval Status of the Cellular turn On operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_TurnOn(CCMsg_T* MessagePtr);

/**
 * @brief   The function to Turn off the Cellular
 *
 * Wrapper function to turn off the modem. Cellular driver will check the modem On status.
 * If it is turned on, driver will turn off the modem. If modem is not turned On, it always return Ok.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 * @retval Status of the Cellular turn On operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_TurnOff(CCMsg_T* MessagePtr);

/**
 * @brief  The function to initialize the network
 *
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 * @retval Status of the Cellular turn On operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_NetworkInit(CCMsg_T* MessagePtr);

/**
 * @brief  The function to configure the specific network
 *
 * setup for turn on the modem.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 * @retval Status of the Cellular turn On operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_NetworkConfig(CCMsg_T* MessagePtr);

/**
 * @brief  The function to connect the network
 *
 * setup for turn on the modem.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 * @retval Status of the Cellular turn On operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_Connect(CCMsg_T* MessagePtr);

/**
 * @brief  The function to Disconnect the network connection
 *
 * setup for turn on the modem.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 * @retval Status of the Cellular turn On operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_DisConnect(CCMsg_T* MessagePtr);

/**
 * @brief   The function to Reset the Cellular
 *
 * Wrapper function to Reset the modem. Cellular driver will check the modem Reset status.
 * It is Reset the Cellular, driver will turn on and turn off the Cellular.
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 * @retval Status of the Cellular Reset operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_Reset(CCMsg_T* MessagePtr);

/**
 *
 * @brief The function to receive the incoming data via Cellular UDP socket.
 *
 * When some incoming data arrives, the Cellular driver invokes the callback, which
 * is associated with the corresponding socket. The callback has been passed
 * as a parameter to the CellularUdpSocket_Open(). This receive function must be
 * invoked within the callback to get the incoming packet. The received data
 * must be consumed before the return of the callback. The buffer, which
 * contains the data, will be released after the return of the callback.
 * Wrapper function to Parse the Socket Event . Cellular driver will check the Parse the Socket Event status.
 *
 *
 * @param[in] event
 * the status of Event as  CELLULAR_UDP_EVENT_DATA_RECEIVED
 * 						   CELLULAR_UDP_EVENT_SOCKET_ERROR
 *
 *
 */
void ParseSocketCallback(CellularUdpSocket_T socket, CellularUdpEvent_T event);

/**
 * @brief   The function to Open the Socket
 *
 * Wrapper function to Open the Socket of the modem. Cellular driver will check the modem Socket Open status.
 *
 *
 * @param[in] MessagePtr  A pointer to hold the TLV element structure
 * @param[in] socketid  :-Get the SocketID
 * @param[in] idLength  :- Socket ID Length
 *
 * @retval Status of the UDP Socket Open operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_SocketOpen(CCMsg_T* MessagePtr, CellularUdpSocket_T *socketid);

/**
 * @brief   The function to Close the Socket
 *
 * Wrapper function to Close the Socket of the modem. Cellular driver will check the modem Socket close Status.
 *
 *
 * @param[in] MessagePtr  A pointer to hold the TLV element structure
 *
 * @retval Status of the UDP Socket Close operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_SocketClose(CCMsg_T* MessagePtr, CellularUdpSocket_T socketid);

/**
 * @brief   The function to send the text message
 *
 * Wrapper function to send the text message. Cellular driver will check the send message status.
 *
 * @param[in] SmsNumber	: A pointer to hold the mobile number to send text message in string format
 * @param[in] TextData	: Holds the text message
 * @param[in] TextDataLength: Holds the length of text message. Max Length of text message is depends on macro SMS_TEXT_MESSAGE_SIZE.
 *
 * @retval Status of the sending of text message operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_SmsSendText(uint8_t *SmsNumber, uint8_t *TextData, uint32_t TextDataLength);

/**
 * @brief   The function to Send the packet through UDP and check for status
 *
 * Wrapper function to Send the packet through UDP. Cellular driver will check the modem UDP packet Send status.
 *
 * @param[in] MessagePtr  * A pointer to hold the TLV element structure
 * @param[in] socketid  :-Get the SocketID
 *
 * @retval Status of the UDP packet Send operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_UdpSend(CCMsg_T* MessagePtr, CellularUdpSocket_T Socketid);

/**
 * @brief   The function to Send the packet through UDP and check for status
 *
 * Wrapper function to Send the packet through UDP. Cellular driver will check the modem UDP packet Send status.
 *
 * @param[in] MessagePtr        :* A pointer to hold the TLV element structure
 * @param[in] socketid          :-Get the SocketID
 * @param[in] *UdpDatabuffer    :-Get the data from Testmanager
 * @param[in] DataLength        :-Get the datalength from Testmanager
 *
 * @retval Status of the UDP packet Send operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_CustomDataSend(CCMsg_T* MessagePtr, CellularUdpSocket_T Socketid, uint8_t *UdpDatabuffer, uint32_t DataLength);

/**
 * @brief   The function to Send the packet through UDP and check for receive the Packet
 *
 * Wrapper function to Send the packet through UDP. Cellular driver will check the modem UDP packet Send status.
 *
 * @param[in] MessagePtr  * A pointer to hold the TLV element structure
 * @param[in] socketid  :-Get the SocketID
 *
 * @retval Status of the UDP packet Send operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_UdpSendReceive(CCMsg_T* MessagePtr, CellularUdpSocket_T socketid);

/**
 * @brief   Append the value with string
 *
 * @param[in] header : Header to be appended
 * @param[in] value  : value to be attended
 *
 */
char* CellularSupport_StringAttach(char* header, int value);

/**
 * @brief   The function to Send the packet of required bytes through UDP and check for receive the Packet
 *
 * Wrapper function to Send the packet through UDP. Cellular driver will check the modem UDP packet Send status.
 *
 * @param[in] MessagePtr  * A pointer to hold the TLV element structure
 * @param[in] socketid    :-Get the SocketID
 * @param[in] DataLength  :-Data length to be sent
 * @retval Status of the UDP packet Send operation, RETCODE_OK on success or some
 * error code otherwise.
 *
 */
Retcode_T CellularSupport_UdpSendReqBytes(CCMsg_T* MessagePtr, CellularUdpSocket_T socketid, uint32_t DataLength);


Retcode_T CellularSupport_CheckForReceivedUdpData(CellularUdpSocket_T socketid, uint8_t* referenceData, uint32_t waitTimeInMs);

void RunTurnOn(CCMsg_T* messagePtr);
void RunNetworkInit(CCMsg_T* messagePtr);
void RunNetworkConfigConnect(CCMsg_T* messagePtr);
void RunTurnOnConnect(CCMsg_T* messagePtr);
void RunDisconnect(CCMsg_T* messagePtr);
void RunTurnOff(CCMsg_T* messagePtr);
void RunDisconnectTurnOff(CCMsg_T* messagePtr);

#endif /* CellularSupport_H */

