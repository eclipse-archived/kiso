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
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief       communication channel handling APIs.
 * @details
 *      This file is part of the test executor software component in the integration testing framework. it provides
 * internally used API functions managing the communication protocol between the test executor and the test controller.
 *
 */

#ifndef KISO_CCHANNEL_H_
#define KISO_CCHANNEL_H_

#include "Kiso_Testing.h"

#define CCMSG_VERSION 1

#define CCMSG_TYPE_MASK 0x30
#define CCMSG_VERSION_MASK 0xC0
#define CCMSG_VERSION_SHIFT (6)

#define CCMSG_GET_VERSION(ccmsg) (((ccmsg->header.messageInfo & CCMSG_VERSION_MASK) >> 6) & 0x03)
#define CCMSG_GET_TYPE(ccmsg) (((ccmsg->header.messageInfo & CCMSG_TYPE_MASK) >> 4) & 0x03)

#define CCMSG_CREATE_TYPE(type) ((((type << 4) & 0x30) + ((CCMSG_VERSION << 6) & CCMSG_VERSION_MASK)) & 0xFF)

#define CCHANNEL_MSG_TYPE_COMMAND 0
#define CCHANNEL_MSG_TYPE_REPORT 1
#define CCHANNEL_MSG_TYPE_ACK 2

#define CCHANNEL_MESSAGE_MAX_LENGTH 256
#define CCHANNEL_HEADER_LENGTH 8
#define CCHANNEL_PAYLOAD_MAX_SIZE CCHANNEL_MESSAGE_MAX_LENGTH - CCHANNEL_HEADER_LENGTH

#define NO_REBOOT 0

/**
 * @brief           Initializes the CChannel
 *
 * @return          RETCODE_OK in case of success error code otherwise.
 */
Retcode_T CChannel_Initialize(void);

/**
 * @brief           Frees the message after having been processed
 *
 * @param[in]       ccmsg The reference to the message to free
 */
void CChannel_FreeCCMsg(CCMsg_T *ccmsg);

/**
 * @brief           Prepares the Ack corresponding to the message
 *
 * @param[in]       ccmsg The reference to the message we are preparing the ack for
 */
void CChannel_PrepareAck(CCMsg_T *ccmsg);

/**
 * @brief           Sends the ack previously prepared
 *
 * @param[in]       result error code to set in the ack before sending it
 *
 * @retcode         RETCODE_OK (=0) on success. Any other value means a failure
 */
Retcode_T CChannel_SendAck(uint8_t result);

/**
 * @brief           Resends the last ack. This is used in case we receive twice the same command
 *
 * @return          RETCODE_OK on success. Any other value means a failure
 */
Retcode_T CChannel_ResendAck(void);

/**
 * @brief           Prepares the report corresponding to the message
 *
 * @param[in]       ccmsg The message we are preparing the report for
 */
void CChannel_PrepareReport(CCMsg_T *ccmsg);

/**
 * @brief           Sends a Report. Function to be called within the run function implementation
 *
 * @param[in]       result The test result code (0: success / otherwise: failure)
 * @param[in]       reason 0-terminating string stating a reason. It can be NULL, if no reason should be sent.
 *
 * @reurn           RETCODE_OK on success. Any other value means a failure
 */
Retcode_T CChannel_SendReport(uint8_t result, char *reason);

/**
 * @brief           Resends the last report. This is used in case we did not receive an ack for our report
 *
 * @return          RETCODE_OK on success. Any other value means a failure
 */
Retcode_T CChannel_ResendReport(void);

/**
 * @brief           Gets a tlv element using the type of the tlvElement input
 *
 * @param[in]       ccmsg         -   message in which to look for the element
 * @param[in, out]  tlvElement    -   to store the element if found
 *                                    tlvElement->type used as input to find the element's value
 *
 * @return          RETCODE_OK if found the TLV element, error code otherwise.
 */
Retcode_T CChannel_GetTlvElement(CCMsg_T *ccmsg, TlvElt_T *tlvElement);

/**
 * @brief           Checks if the ack we received corresponds to the report we just sent
 *
 * @param[in]       ccack ack message we received
 *
 * @return          true if match, false if no match
 */
bool CChannel_DoesAckMatchReport(CCMsg_T *ccack);

/**
 * @brief           Stops the CChannel
 *
 * @return          RETCODE_OK on success. Any other value means a failure
 */
Retcode_T CChannel_Deinitialize(void);

/**
 * @brief           Method triggered when a new message in received
 *
 * @param[in]       buffer pointer to the message as a raw buffer
 * @param[in]       length length of the received message
 */
void CChannel_ReceiveEventHandler(uint8_t *buffer, uint8_t length);

/**
 * @brief           Adds the tlv Element to the message
 *
 * @param[in]       ccmsg Message to add the element to
 * @param[in]       tlvElement TLV element to add
 *
 * @return          RETCODE_OK if successful error code otherwise
 */
Retcode_T CChannel_AddTlvElement(CCMsg_T *ccmsg, TlvElt_T *tlvElement);

#endif /* KISO_CCHANNEL_H_ */
