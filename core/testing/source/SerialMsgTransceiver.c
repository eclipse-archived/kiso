/*******************************************************************************
 * Copyright (c) 2010-2020 Robert Bosch GmbH
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

/**
 * @file
 *
 * @brief
 * Implements the following functionalities specified in template.h
 */
#include "Kiso_Testing.h"
#include "SerialMsgTransceiver.h"
#include "SerialCChannel.h"
#include "CChannel.h"
#include "Kiso_GuardedTask.h"
#include <limits.h>

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_MODULE_ID_TESTING_SERIALMSGTRANSCEIVER

#define PAYLOAD_LENGTH_INDEX 7
#define RECEIVE_BUFFER_SIZE 256

#define START 0xC0
#define ESC 0xDB
#define ESC_START 0xDC
#define ESC_ESC 0xDD

#define CRC_DEFAULT UINT8_C(0)
#define CRC_BIT_SHIFT_4 UINT16_C(4)   /** 4 bit shifting for CRC calculation */
#define CRC_BIT_SHIFT_5 UINT16_C(5)   /** 5 bit shifting for CRC calculation */
#define CRC_BIT_SHIFT_8 UINT16_C(8)   /** 8 bit shifting for CRC calculation */
#define CRC_BIT_SHIFT_12 UINT16_C(12) /** 12 bit shifting for CRC calculation */
#define CRC_INIT_VALUE UINT16_C(0)    /** Default Initialize value for flag */
#define CRC_BYTE_MASK UINT16_C(0xFF)

enum Receiver_State_E
{
    WAITING_FOR_START,
    RECEIVING_HEADER,
    RECEIVING_PAYLOAD,
    RECEIVED_DONE
};

static uint16_t calculateCRC(const void *buffer, uint8_t length);

static uint8_t receiveBuffer[RECEIVE_BUFFER_SIZE];
static uint32_t serialConsumedCnt = 0; /* number of characters received */
static uint32_t msgCnt = 0;            /* diagnostic: number of messages received */
static uint32_t msgOK = 0;             /* diagnostic: number of messages received */
static uint32_t msgNOK = 0;            /* diagnostic: number of messages received */

Retcode_T SerialMsgTransceiver_Send(uint8_t *message, uint8_t length)
{
    Retcode_T ReturnValue;
    uint8_t sendBuffer[length * 2]; //todo variable length array dangerous
    uint8_t j = 0;

    uint16_t crc = calculateCRC(message, length);

    sendBuffer[j++] = START;

    if (((crc >> CHAR_BIT) & CRC_BYTE_MASK) == START)
    {
        sendBuffer[j++] = ESC;
        sendBuffer[j++] = ESC_START;
    }
    else if (((crc >> CHAR_BIT) & CRC_BYTE_MASK) == ESC)
    {
        sendBuffer[j++] = ESC;
        sendBuffer[j++] = ESC_ESC;
    }
    else
    {
        sendBuffer[j++] = (crc >> CHAR_BIT) & CRC_BYTE_MASK;
    }

    if ((crc & CRC_BYTE_MASK) == START)
    {
        sendBuffer[j++] = ESC;
        sendBuffer[j++] = ESC_START;
    }
    else if ((crc & CRC_BYTE_MASK) == ESC)
    {
        sendBuffer[j++] = ESC;
        sendBuffer[j++] = ESC_ESC;
    }
    else
    {
        sendBuffer[j++] = crc & CRC_BYTE_MASK;
    }

    for (uint32_t i = 0; i < length; i++)
    {
        if (message[i] == START)
        {
            sendBuffer[j++] = ESC;
            sendBuffer[j++] = ESC_START;
        }
        else if (message[i] == ESC)
        {
            sendBuffer[j++] = ESC;
            sendBuffer[j++] = ESC_ESC;
        }
        else
        {
            sendBuffer[j++] = message[i];
        }
    }

    ReturnValue = Serial_Send(sendBuffer, j);

    return (ReturnValue);
}

/** @todo Verify total length, limited to 256! what to do if exceeded?
  * The description is defined at function declaration */
void SerialMsgTransceiver_Receive(void)
{
    static uint8_t receivingState = WAITING_FOR_START;
    static uint8_t receivedByte;
    static uint8_t nReceivedByte = 0;
    static uint8_t payloadLength = 0;
    static bool receivedEsc = false;
    static uint8_t testCnt = 0;

    /* The serialReceivedCnt is incremented with every received byte in the ISR
	   The difference betweeb the serialReceivedCnt and serialConsumedCnt repflects the number of bytes available in the buffer
	   We consume characters while the difference is > 0
	   There is no need to check overflows since both numbers are of uint32_t and the result will always state the difference.
	*/
    while ((serialReceivedCnt - serialConsumedCnt) > 0)
    {
        if (RETCODE_OK != Serial_Receive((uint8_t *)&receivedByte, UINT32_C(1)))
        {
            Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_FAILURE));
            return;
        }
        serialConsumedCnt++;

        if (WAITING_FOR_START == receivingState)
        {
            if (START == receivedByte)
            {
                nReceivedByte = 0;
                (void)memset(receiveBuffer, 0, sizeof(receiveBuffer));
                receivingState = RECEIVING_HEADER;
            }
        }
        else
        {
            if (START == receivedByte)
            {
                Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_TESTING_INCOMPLETE_MESSAGE_RECEIVED));
                nReceivedByte = 0;
                (void)memset(receiveBuffer, 0, sizeof(receiveBuffer));
                msgNOK++;
                receivingState = RECEIVING_HEADER;
            }
            else if (receivedEsc)
            {
                receivedEsc = false;
                if (ESC_START == receivedByte)
                {
                    receiveBuffer[nReceivedByte++] = START;
                }
                else if (ESC_ESC == receivedByte)
                {
                    receiveBuffer[nReceivedByte++] = ESC;
                }
                else
                {
                    // error should not happen
                    Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_INCONSITENT_STATE));
                    receivingState = WAITING_FOR_START;
                }
            }
            else if (ESC == receivedByte)
            {
                receivedEsc = true;
            }
            else
            {
                receiveBuffer[nReceivedByte++] = receivedByte;
            }

            if (RECEIVING_HEADER == receivingState)
            {
                if (CCHANNEL_HEADER_LENGTH + 2 == nReceivedByte)
                {
                    payloadLength = receiveBuffer[PAYLOAD_LENGTH_INDEX + 2];

                    if (0 == payloadLength)
                    {
                        receivingState = RECEIVED_DONE;
                    }
                    else
                    {
                        receivingState = RECEIVING_PAYLOAD;
                    }
                }
            }
            else if (RECEIVING_PAYLOAD == receivingState)
            {
                if (payloadLength + CCHANNEL_HEADER_LENGTH + 2 == nReceivedByte)
                {
                    receivingState = RECEIVED_DONE;
                }
            }
        }
    }

    if (RECEIVED_DONE == receivingState)
    {
        uint16_t calculatedCRC = calculateCRC(&receiveBuffer[2], CCHANNEL_HEADER_LENGTH + payloadLength);
        uint16_t expectedCRC = ((receiveBuffer[0] & CRC_BYTE_MASK) << CHAR_BIT) + (receiveBuffer[1] & CRC_BYTE_MASK);

        msgCnt++;
        if (calculatedCRC == expectedCRC)
        {
            CChannel_ReceiveEventHandler(&receiveBuffer[2], CCHANNEL_HEADER_LENGTH + payloadLength);
            receivingState = WAITING_FOR_START;
            msgOK++;
        }
        else
        {
            Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_TESTING_CRC_MISMATCH));
            receivingState = WAITING_FOR_START;
            msgNOK++;
        }
        testCnt++;
    }
}

static uint16_t calculateCRC(const void *buffer, uint8_t length)
{
    uint16_t crc = CRC_DEFAULT;

    for (uint8_t i = 0; i < length; i++)
    {
        crc = (crc >> (CRC_BIT_SHIFT_8)) | (crc << (CRC_BIT_SHIFT_8));
        crc ^= ((const uint8_t *)buffer)[i];
        crc ^= (crc & (CRC_BYTE_MASK)) >> (CRC_BIT_SHIFT_4);
        crc ^= crc << (CRC_BIT_SHIFT_12);
        crc ^= (crc & (CRC_BYTE_MASK)) << (CRC_BIT_SHIFT_5);
    }
    return crc;
}
