/*----------------------------------------------------------------------------*/
/**
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 *
 * @file
 * 
 */
/*----------------------------------------------------------------------------*/

/**
 * @file
 * @brief BCDS Cellular Sms Interface
 */

/**
 * @defgroup CellularSms
 * @ingroup Cellular CellularSms
 * @{
 *
 * @brief This module is used to sms send, set service center and get service center.
 */

#ifndef BCDS_CELLULARSMS_H
#define BCDS_CELLULARSMS_H

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_CellularConfig.h"

/**
 * @brief Typedef to represent the SMS event.
 */
typedef enum
{
    SMS_EVENT_SEND, /** SMS event after Text sent successfully */
} Sms_Event_T;

/**
 * @brief Enumeration to represent the SMS status.
 */
typedef enum
{
    SMS_STATUS_SUCCESS, /** SMS status for success */
    SMS_STATUS_FAILURE  /** SMS status for failure */
} Sms_Status_T;

/**
 * @brief Callback function signature to be called on SMS event
 *
 * @param [in]  Event : event to be send by the modem after sending the text message.
 * @param [in]  Status : Status of text message which sent successful/failure
 *
 */
typedef void (*Sms_CallbackFunction_T)(Sms_Event_T Event, void* Status);

/**
 * @brief   Initialization of SMS interface.
 *
 * @note    This API will not be successful if called more than once without de-initializing.
 *
 * @param[in]   EventCallback : Registration of callback function for notification of SMS event.
 *
 * @retval  RETCODE_NOT_SUPPORTED Sms is not supported
 *
 */
Retcode_T CellularSms_Initialize(Sms_CallbackFunction_T EventCallback);

/**
 * @brief   Deinitialization of SMS Interface.
 *
 * @retval  RETCODE_NOT_SUPPORTED Sms is not supported
 *
 */
Retcode_T CellularSms_Deinitialize(void);

/**
 * @brief   Send the text message to the given mobile number by the user.
 *
 * @note
 * 1. The text message can send only after successful cellular initialization.
 * 2. For sending the text message, maximum number of bytes are SMS_TEXT_MESSAGE_SIZE(160Bytes)
 * 3. For mobile number, maximum no of digits SMS_MOBILE_NUMBER_SIZE(15 digits) and minimum 1 digit.
 * 4. The payload data type corresponds to text format (UCS2 data type). All the standard ASCII character
 *    set is supported through its corresponding 8-bit Unicode scalar value
 *
 * Ex: In order to send "sei" and  "gl�cklich", equivalent bytes to be send are "0x73 0x65 0x69" and
 *      "0x67 0x6C 0xFC 0x63 0x6B 0x6C 0x69 0x63 0x68" (Remove spaces)
 *
 * @param[in]   SmsNumber       : Holds the mobile number to send text message in string format
 *
 * @param [in]  TextData        : Holds the text message
 *
 * @param [in]  TextDataLength  : Holds the length of text message. Max Length of text message is depends on macro SMS_TEXT_MESSAGE_SIZE.
 *
 * @retval  RETCODE_NOT_SUPPORTED Sms is not supported
 *
 */
Retcode_T CellularSms_SendText(uint8_t *SmsNumber, uint8_t *TextData, uint32_t TextDataLength);

/**
 * @brief   Provision to configure the SMS interface by providing the network service number
 *
 * @note
 * 1. This API shall be called while initializing the SMS interface
 *
 * @param[in]   ServiceNumber   : Holds the network service number in string format
 *
 * @retval  RETCODE_NOT_SUPPORTED Sms is not supported
 *
 */
Retcode_T CellularSms_SetServiceCenter(uint8_t *ServiceNumber);

/**
* @brief    Provision to configure the SMS interface by receiving the network service number
*
* @note
* 1. This API shall be called to get the network service number
*
* @param[OUT]   ServiceNumber   : Received network service number will store in string format
* @param[IN & OUT]   NumLength  :  Input: Provides the length of the service number
*                                  Output: Holds the length of Service number.
*
* @retval  RETCODE_NOT_SUPPORTED Sms is not supported
*
*/
Retcode_T CellularSms_GetServiceCenter(uint8_t *ServiceNumber,uint8_t *NumLength);

#endif /* BCDS_CELLULARSMS_H */
/**@} */
