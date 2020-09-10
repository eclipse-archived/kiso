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
 * This module is responsible for the transmission and reception of data through
 * the communication medium, which can be UART / UDP / BLE and so on.
 */

#include "Kiso_Testing.h"
#include "CChannel.h"
#include "TestRunner.h"
#include "SerialCChannel.h"
#include "SerialMsgTransceiver.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_MODULE_ID_TESTING_CCHANNEL

#define MAX_NUMBER_OF_INCOMING_MESSAGE 2

#define CCHANNEL_TLV_TYPE_REASON 112

#define CCHANNEL_REPORT_TYPE 110

#define CCHANNEL_NUM_OF_SEND_RETRIES 4

#define MSG_INIT_OK ("CCHANNEL INIT OK\r\n")

static CCMsg_T *allocCCMessage(void);
static Retcode_T sendMessage(CCMsg_T *ccmsg);
static void parseTlvElements(CCMsg_T *ccmsg);
static void freeAllCCMsg(void);

static CCMsg_T msgPool[MAX_NUMBER_OF_INCOMING_MESSAGE];
static CCMsg_T ackMessage;
static CCMsg_T reportMessage;

Retcode_T CChannel_Initialize(void)
{
    freeAllCCMsg();
    Retcode_T retcode = Serial_Initialize();

    if (RETCODE_OK == retcode)
    {
        char msg[sizeof(MSG_INIT_OK)] = MSG_INIT_OK;
        retcode = Serial_Send((void *)msg, strlen(msg));
    }
    return retcode;
}

Retcode_T CChannel_Deinitialize(void)
{
    return Serial_Deinitialize();
}

void CChannel_FreeCCMsg(CCMsg_T *ccmsg)
{
    (void)memset(ccmsg, 0, sizeof(CCMsg_T));

    ccmsg->isFree = true;
}

void CChannel_ReceiveEventHandler(uint8_t *buffer, uint8_t length)
{
    if (CCMSG_VERSION != ((buffer[0] & CCMSG_VERSION_MASK) >> CCMSG_VERSION_SHIFT))
    {
        //if the framework version does not match we just ignore this message
        return;
    }

    CCMsg_T *ccmsg = allocCCMessage();

    if (NULL == ccmsg)
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES));
        return;
    }

    (void)memcpy(ccmsg, buffer, length);

    parseTlvElements(ccmsg);

    TestRunner_ProcessMessage(ccmsg);
}

void CChannel_PrepareAck(CCMsg_T *ccmsg)
{
    (void)memset(&ackMessage, 0, sizeof(CCMsg_T));

    (void)memcpy(&ackMessage, ccmsg, CCHANNEL_HEADER_LENGTH);

    ackMessage.header.messageInfo = CCMSG_CREATE_TYPE(CCHANNEL_MSG_TYPE_ACK);
    ackMessage.header.payloadLength = 0;
}

Retcode_T CChannel_SendAck(uint8_t result)
{
    ackMessage.header.messageType = result == 0 ? 0 : 1;
    ackMessage.header.errorCode = result;

    assert(ackMessage.header.messageInfo == CCMSG_CREATE_TYPE(CCHANNEL_MSG_TYPE_ACK));
    assert(ackMessage.header.payloadLength == 0);

    return sendMessage(&ackMessage);
}

Retcode_T CChannel_ResendAck(void)
{
    return sendMessage(&ackMessage);
}

void CChannel_PrepareReport(CCMsg_T *ccmsg)
{
    (void)memset(&reportMessage, 0, sizeof(CCMsg_T));

    (void)memcpy(&reportMessage, ccmsg, CCHANNEL_HEADER_LENGTH);

    reportMessage.header.messageType = 0;
    reportMessage.header.messageInfo = CCMSG_CREATE_TYPE(CCHANNEL_MSG_TYPE_REPORT);
    reportMessage.header.payloadLength = 0;
}

Retcode_T CChannel_SendReport(uint8_t result, char *reason)
{
    Retcode_T retcode = RETCODE_OK;

    reportMessage.header.errorCode = result;
    reportMessage.header.messageType = result == 0 ? 0 : 1;

    assert(reportMessage.header.payloadLength == 0);

    if (NULL != reason)
    {
        TlvElt_T reasonTlv;
        reasonTlv.type = CCHANNEL_TLV_TYPE_REASON;
        reasonTlv.length = strlen(reason);
        reasonTlv.value = reason;

        retcode = CChannel_AddTlvElement(&reportMessage, &reasonTlv);

        if ((strlen(reason) + 2) != reportMessage.header.payloadLength)
        {
            return RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_UNEXPECTED_BEHAVIOR);
        }
    }

    assert(reportMessage.header.messageInfo == CCMSG_CREATE_TYPE(CCHANNEL_MSG_TYPE_REPORT));

    if (RETCODE_OK == retcode)
    {
        retcode = sendMessage(&reportMessage);
    }

    return retcode;
}

Retcode_T CChannel_ResendReport(void)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = sendMessage(&reportMessage);

    return retcode;
}

bool CChannel_DoesAckMatchReport(CCMsg_T *ccack)
{
    bool result = true;
    MsgHdr_T msgHdr = reportMessage.header;
    MsgHdr_T ackHdr = ccack->header;

    if ((ackHdr.messageToken != msgHdr.messageToken) || (ackHdr.testEntry != msgHdr.testEntry) || (ackHdr.testSuite != msgHdr.testSuite) || (ackHdr.testCase != msgHdr.testCase))
    {
        result = false;
    }

    return result;
}

Retcode_T CChannel_GetTlvElement(CCMsg_T *ccmsg, TlvElt_T *tlvElement)
{
    if ((NULL == ccmsg) || (NULL == tlvElement))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        for (uint32_t i = 0; i < ccmsg->numberOfTlvElements; i++)
        {
            if (tlvElement->type == ccmsg->tlvArray[i].type)
            {
                if (NULL == ccmsg->tlvArray[i].value)
                {
                    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
                }

                tlvElement->value = ccmsg->tlvArray[i].value;
                tlvElement->length = ccmsg->tlvArray[i].length;
                return RETCODE_OK;
            }
        }
    }
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TESTING_TLVELEMENT_NOT_FOUND);
}

Retcode_T CChannel_AddTlvElement(CCMsg_T *ccmsg, TlvElt_T *tlvElement)
{
    if ((NULL == ccmsg) || (NULL == tlvElement) || (NULL == tlvElement->value))
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_NULL_POINTER));
    }
    else
    {

        if (CCHANNEL_MAX_NUMBER_OF_TLV_ELEMENTS == ccmsg->numberOfTlvElements)
        {
            //we allow only 20 TLV elements in a message
            return RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_OUT_OF_RESOURCES);
        }

        if (CCHANNEL_PAYLOAD_MAX_SIZE <= (ccmsg->payloadIndex + 2 + tlvElement->length))
        {
            //we allow only 248 as the maximum paylaod size
            return RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_OUT_OF_RESOURCES);
        }

        TlvElt_T *currentTlvElement = &ccmsg->tlvArray[ccmsg->numberOfTlvElements];

        currentTlvElement->type = tlvElement->type;
        ccmsg->payload[ccmsg->payloadIndex++] = tlvElement->type;
        currentTlvElement->length = tlvElement->length;
        ccmsg->payload[ccmsg->payloadIndex++] = tlvElement->length;

        currentTlvElement->value = (char *)&(ccmsg->payload[ccmsg->payloadIndex]);
        (void)memcpy(&ccmsg->payload[ccmsg->payloadIndex], tlvElement->value, tlvElement->length);

        ccmsg->payloadIndex += tlvElement->length;

        ccmsg->header.payloadLength += tlvElement->length + 2;
        ccmsg->numberOfTlvElements++;

        return RETCODE_OK;
    }
}

static CCMsg_T *allocCCMessage(void)
{
    CCMsg_T *ccmsg = NULL;

    for (uint8_t i = 0; i < MAX_NUMBER_OF_INCOMING_MESSAGE; i++)
    {
        if (msgPool[i].isFree)
        {
            ccmsg = &msgPool[i];
            ccmsg->isFree = false;
            break;
        }
    }

    return ccmsg;
}

static Retcode_T sendMessage(CCMsg_T *ccmsg)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t retries = CCHANNEL_NUM_OF_SEND_RETRIES;
    do
    {
        retcode = SerialMsgTransceiver_Send((uint8_t *)ccmsg, CCHANNEL_HEADER_LENGTH + ccmsg->header.payloadLength);
        retries--;
    } while (retries > 0 && RETCODE_OK != retcode);

    return retcode;
}

static void parseTlvElements(CCMsg_T *ccmsg)
{
    for (uint8_t i = 0; i < ccmsg->header.payloadLength;)
    {
        if (ccmsg->numberOfTlvElements == CCHANNEL_MAX_NUMBER_OF_TLV_ELEMENTS)
        {
            return;
        }

        ccmsg->tlvArray[ccmsg->numberOfTlvElements].type = ccmsg->payload[i++];
        ccmsg->tlvArray[ccmsg->numberOfTlvElements].length = ccmsg->payload[i++];
        ccmsg->tlvArray[ccmsg->numberOfTlvElements].value = (char *)&(ccmsg->payload[i]);
        i += ccmsg->tlvArray[ccmsg->numberOfTlvElements].length;
        ccmsg->numberOfTlvElements++;
    }
}

static void freeAllCCMsg(void)
{
    (void)memset(msgPool, 0, sizeof(msgPool));

    for (uint32_t i = 0; i < MAX_NUMBER_OF_INCOMING_MESSAGE; i++)
    {
        msgPool[i].isFree = true;
    }
}
