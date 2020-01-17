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

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_UTILS

#include "AtUtils.h"
#include "AtResponseParser.h"
#include "AtResponseQueue.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

#define IPV4_ADDR_BYTE_COUNT 4
#define LONG_INT_STR_LENGTH 11

Retcode_T Utils_ConvertAtResponseRetcodeToCellularRetcode(Retcode_T retcode)
{
    Retcode_T result = retcode;

    switch (Retcode_GetCode(retcode))
    {
    case RETCODE_AT_RESPONSE_QUEUE_TIMEOUT:
        result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
        break;
    case RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT:
    case RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT:
    case RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE:
        result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
        break;
    default:
        result = retcode;
        break;
    }

    return result;
}

Retcode_T Utils_ConvertCellularResponseCodeToCellularRetcode(AtResponseCode_T response)
{
    Retcode_T result;

    switch (response)
    {
    case AT_RESPONSE_CODE_OK:
        result = RETCODE_OK;
        break;
    case AT_RESPONSE_CODE_ABORTED:
        result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONDED_ABORTED);
        break;
    case AT_RESPONSE_CODE_ERROR:
        result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONDED_ERROR);
        break;
    default:
        result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONDED_OTHER);
        break;
    }

    return result;
}

Retcode_T Utils_Strtol(const uint8_t *buffer, uint32_t BufferLength, int32_t *number)
{
    Retcode_T result = RETCODE_OK;

    if (NULL == buffer || NULL == number)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else if (BufferLength > LONG_INT_STR_LENGTH)
    {
        // no number with more than 10 digits (11 if the first char is the minus sign) will ever fit in an int32
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    bool IsSigned = false;
    uint64_t ResultNumber = 0;
    if (RETCODE_OK == result)
    {
        for (uint32_t i = 0; i < BufferLength; i++)
        {
            uint8_t CurrentChar = buffer[i];
            if (0 == i && '-' == CurrentChar)
            {
                IsSigned = true;
            }
            else if ('0' <= CurrentChar && CurrentChar <= '9')
            {
                ResultNumber *= 10;
                ResultNumber += (CurrentChar - '0');
            }
            else
            {
                result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                break;
            }

            if (ResultNumber > (INT32_MAX - 1))
            {
                // we had an overflow
                result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                break;
            }
        }
    }

    if (RETCODE_OK == result)
    {
        if (ResultNumber > (INT32_MAX - 1))
        {
            result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        else
        {
            *number = (IsSigned ? -1 : 1) * ((int32_t)ResultNumber);
        }
    }

    return result;
}

Retcode_T Utils_StrtolBounds(const uint8_t *buffer, uint32_t BufferLength, int32_t *number, int32_t LowerBound, int32_t UpperBound)
{
    Retcode_T result = Utils_Strtol(buffer, BufferLength, number);

    if (RETCODE_OK == result)
    {
        if (LowerBound <= *number && *number <= UpperBound)
        {
            result = RETCODE_OK;
        }
        else if (*number < LowerBound)
        {
            *number = LowerBound;
            result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        else if (*number > UpperBound)
        {
            *number = UpperBound;
            result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
    }

    return result;
}

Retcode_T Utils_WaitForAndHandleResponseCode(uint32_t timeout, Retcode_T retcode)
{
    AtResponseCode_T ResponseCode = AT_RESPONSE_CODE_ERROR;
    Retcode_T ResponseCodeRetcode = AtResponseQueue_WaitForArbitraryResponseCode(timeout, &ResponseCode);

    if (RETCODE_OK == ResponseCodeRetcode)
    {
        // we received a response code ... now turn it into a retcode
        ResponseCodeRetcode = Utils_ConvertCellularResponseCodeToCellularRetcode(ResponseCode);

        if (RETCODE_OK == ResponseCodeRetcode) /* modem resp. OK */
        {
            /* leave retcode as is */
            retcode = Utils_ConvertAtResponseRetcodeToCellularRetcode(retcode);
        }
        else /* modem resp. nOK */
        {
            /* overwrite retcode with modem response */
            retcode = ResponseCodeRetcode;
        }
    }
    else if (Retcode_GetCode(ResponseCodeRetcode) == (uint32_t)RETCODE_AT_RESPONSE_QUEUE_TIMEOUT) /* modem resp. timeout */
    {
        /* overwrite with not-response-error */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
    }
    else /* other WaitFor-error */
    {
        /* overwrite with unexpected-error */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_RESPONSE_UNEXPECTED);
    }

    return retcode;
}

const uint8_t *Utils_TrimFlukeCharacters(const uint8_t *buffer, uint32_t BufferLength, uint32_t *NewBufferLength)
{
    uint32_t ResultBufferLength = BufferLength;
    const uint8_t *ResultBuffer = buffer;

    while (ResultBufferLength > 0 && ((*ResultBuffer) < AT_SPACE_CHARACTER || (*ResultBuffer) > AT_TILDE_CHARACTER) && (AT_DEFAULT_S4_CHARACTER != (*ResultBuffer) && AT_DEFAULT_S3_CHARACTER != (*ResultBuffer)))
    {
        ++ResultBuffer;
        --ResultBufferLength;
    }

    *NewBufferLength = ResultBufferLength;
    return ResultBuffer;
}
