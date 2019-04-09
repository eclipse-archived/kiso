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
 * @brief Implements utility functions shared within the driver
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_UTILS

#include "CellularModule.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "CellularAtResponseParser.h"
#include "CellularResponseQueue.h"
#include "CellularCommon.h"
#include "CellularUtils.h"

#define IPV4_ADDR_BYTE_COUNT    4
#define LONG_INT_STR_LENGTH     11

/**
 * @brief Converts a number to a single character representing a hexadecimal number.
 */
static uint8_t toHexChar(uint8_t QuintetIndex, uint8_t byte)
{
    const uint8_t *HexChars = (const uint8_t *) "0123456789ABCDEF";
    uint8_t mask = (0 == QuintetIndex) ? 0x0F : 0xF0;
    uint8_t shift = (0 == QuintetIndex) ? 0 : 4;
    uint8_t quintet = (byte & mask) >> shift;

    return HexChars[quintet];
}

/**
 * @brief Converts a single character representing a hexadecimal number to the number.
 *        If the character is not a valid hex char, 0xFF is returned.
 */
static uint8_t FromHexChar(uint8_t HexChar)
{
    uint8_t result;

    if ('0' <= HexChar && HexChar <= '9')
    {
        result = HexChar - '0';
    }
    else if ('A' <= HexChar && HexChar <= 'F')
    {
        result = 10 + (HexChar - 'A');
    }
    else if ('a' <= HexChar && HexChar <= 'f')
    {
        result = 10 + (HexChar - 'a');
    }
    else
    {
        result = 0xFF;
    }

    return result;
}

Retcode_T CellularUtils_HexToBin(const uint8_t *hex, uint8_t *bin, uint32_t BinLength)
{
    for (uint32_t i = 0; i < BinLength; i++)
    {
        uint8_t high = FromHexChar(hex[2 * i]);
        uint8_t low = FromHexChar(hex[2 * i + 1]);

        if (0xFF == high || 0xFF == low)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        bin[i] = (high << 4) | low;
    }

    return RETCODE_OK;
}

void CellularUtils_BinToHex(const uint8_t *bin, char *hex, uint32_t BinLength)
{
    for (uint32_t i = 0; i < BinLength; i++)
    {
        hex[2 * i] = toHexChar(1, bin[i]);
        hex[2 * i + 1] = toHexChar(0, bin[i]);
    }
}

Retcode_T CellularUtils_ConvertAtResponseRetcodeToCellularRetcode(Retcode_T retcode)
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

Retcode_T CellularUtils_ConvertCellularResponseCodeToCellularRetcode(AtResponseCode_T response)
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

Retcode_T CellularUtils_Strtol(const uint8_t *buffer, uint32_t BufferLength, int32_t *number)
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
            *number = (IsSigned ? -1 : 1) * ((int32_t) ResultNumber);
        }
    }

    return result;
}

Retcode_T CellularUtils_StrtolBounds(const uint8_t *buffer, uint32_t BufferLength, int32_t *number, int32_t LowerBound, int32_t UpperBound)
{
    Retcode_T result = CellularUtils_Strtol(buffer, BufferLength, number);

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

Retcode_T CellularUtils_ParseIpAddress(uint8_t *IpAddressBuffer, uint32_t IpAddressBufferLength, uint32_t *ParsedIpAddress)
{
    if (NULL == IpAddressBuffer || NULL == ParsedIpAddress || 0 == IpAddressBufferLength
            || IpAddressBufferLength > CELLULAR_UTILS_MAX_IP_STR_LENGTH)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T result = RETCODE_OK;
    uint32_t IpAddressResult = 0;

    uint32_t CurrentByte = 0;
    uint8_t PositionInCurrentByte = 0;
    uint8_t IpAddressBytesParsed = 0;
    for (uint32_t i = 0; i < IpAddressBufferLength; i++)
    {
        uint8_t CurrentChar = IpAddressBuffer[i];

        if ('.' == CurrentChar)
        {
            IpAddressResult = IpAddressResult << 8;
            IpAddressResult |= (CurrentByte & 0xFF);

            IpAddressBytesParsed++;
            CurrentByte = 0;
            PositionInCurrentByte = 0;
        }
        else if ('0' <= CurrentChar && CurrentChar <= '9')
        {
            CurrentByte *= 10;
            CurrentByte += (CurrentChar - '0');
            PositionInCurrentByte++;

            if (CurrentByte > 0xFF || PositionInCurrentByte > 3)
            {
                result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                break;
            }
        }
        else
        {
            result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            break;
        }

        if ((IpAddressBufferLength - 1) == i)
        {
            // end of IP address
            IpAddressResult = IpAddressResult << 8;
            IpAddressResult |= (CurrentByte & 0xFF);
            IpAddressBytesParsed++;
        }
    }

    if (IPV4_ADDR_BYTE_COUNT == IpAddressBytesParsed)
    {
        *ParsedIpAddress = IpAddressResult;
        result = RETCODE_OK;
    }
    else
    {
        result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    return result;
}

Retcode_T CellularUtils_WaitForAndHandleResponseCode(uint32_t timeout, Retcode_T retcode)
{
    /*
     *                      | Retcode OK        | Retcode nOK
     * --------------------------------------------------------
     * modem resp. OK:      | Retcode OK (â†‘)    | â†‘ (converted)
     * --------------------------------------------------------
     * modem resp. nOK:     | â†�                 | â†�
     * --------------------------------------------------------
     * modem resp. timeout: | Retcode modem not responsive
     * --------------------------------------------------------
     * other WaitFor-error: | Retcode modem resp. not expected
     */

    AtResponseCode_T ResponseCode = AT_RESPONSE_CODE_ERROR;
    Retcode_T ResponseCodeRetcode = AtResponseQueue_WaitForArbitraryResponseCode(timeout, &ResponseCode);

    if (RETCODE_OK == ResponseCodeRetcode)
    {
        // we received a response code ... now turn it into a retcode
        ResponseCodeRetcode = CellularUtils_ConvertCellularResponseCodeToCellularRetcode(ResponseCode);

        if (RETCODE_OK == ResponseCodeRetcode) /* modem resp. OK */
        {
            /* leave retcode as is (â†‘) */
            retcode = CellularUtils_ConvertAtResponseRetcodeToCellularRetcode(retcode);
        }
        else /* modem resp. nOK */
        {
            /* overwrite retcode with modem response (â†�) */
            retcode = ResponseCodeRetcode;
        }
    }
    else if (Retcode_GetCode(ResponseCodeRetcode) == (uint32_t) RETCODE_AT_RESPONSE_QUEUE_TIMEOUT) /* modem resp. timeout */
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

uint8_t *CellularUtils_TrimFlukeCharacters(uint8_t *buffer, uint32_t BufferLength, uint32_t *NewBufferLength)
{
    uint32_t ResultBufferLength = BufferLength;
    uint8_t *ResultBuffer = buffer;

    while (ResultBufferLength > 0
            && ((*ResultBuffer) < AT_SPACE_CHARACTER || (*ResultBuffer) > AT_TILDE_CHARACTER)
            && (AT_DEFAULT_S4_CHARACTER != (*ResultBuffer) && AT_DEFAULT_S3_CHARACTER != (*ResultBuffer)))
    {
        ++ResultBuffer;
        --ResultBufferLength;
    }

    *NewBufferLength = ResultBufferLength;
    return ResultBuffer;
}
