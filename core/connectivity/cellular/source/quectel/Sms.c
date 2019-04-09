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

/**
 * @file
 *
 * @brief Provision to send text message to the given mobile number by the user.
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_SMS

#include "CellularModule.h"
#include "BCDS_Assert.h"
#include "BCDS_CellularSms.h"
#include "Sms.h"

void SmsEventNotifyCB(uint8_t Type, uint8_t* Payload_Ptr)
{
    BCDS_UNUSED(Type);
    BCDS_UNUSED(Payload_Ptr);
}

Retcode_T CellularSms_Initialize(Sms_CallbackFunction_T EventCallback)
{
    BCDS_UNUSED(EventCallback);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T CellularSms_Deinitialize(void)
{
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T CellularSms_SendText(uint8_t *SmsNumber, uint8_t *TextData, uint32_t TextDataLength)
{
    BCDS_UNUSED(SmsNumber);
    BCDS_UNUSED(TextData);
    BCDS_UNUSED(TextDataLength);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T CellularSms_SetServiceCenter(uint8_t *ServiceNumber)
{
    BCDS_UNUSED(ServiceNumber);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T CellularSms_GetServiceCenter(uint8_t *ServiceNumber, uint8_t *NumLength)
{
    BCDS_UNUSED(ServiceNumber);
    BCDS_UNUSED(NumLength);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}
