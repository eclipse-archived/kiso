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

/* own header files */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_XPROTOCOL

#include "Kiso_XProtocol.h"

#if KISO_FEATURE_XPROTOCOL

/* system header files */
#include "Kiso_Retcode.h"

/* additional interface header files */
#include "Kiso_CRC.h"

#define XPROTOCOL_CRC_CCITT_POLY   0x1021U /**< Polynom for function crc16 */
#define XPROTOCOL_SD               0xC0    /**< Start delimitter */
#define XPROTOCOL_ED               0xC9    /**< End delimitter */
#define XPROTOCOL_ESC              0xDB    /**< Escape character */
#define XPROTOCOL_ESCAPED_SD       0xDC    /**< Escaped SD */
#define XPROTOCOL_ESCAPED_ED       0xDE    /**< Escaped ED */
#define XPROTOCOL_ESCAPED_ESC      0xDD    /**< Escaped ESC */


static Retcode_T Escape_SpecialCharacters(uint8_t *frame, uint32_t *indicator, uint32_t maxFrameLength)
{
    if (XPROTOCOL_SD == frame[*indicator])
    {
        frame[*indicator] = XPROTOCOL_ESC;
        (*indicator)++;
        if (maxFrameLength <= *indicator)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
        }
        frame[*indicator] = XPROTOCOL_ESCAPED_SD;
    }
    else if (XPROTOCOL_ED == frame[*indicator])
    {
        frame[*indicator] = XPROTOCOL_ESC;
        (*indicator)++;
        if (maxFrameLength <= *indicator)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
        }
        frame[*indicator] = XPROTOCOL_ESCAPED_ED;
    }
    else if (XPROTOCOL_ESC == frame[*indicator])
    {
        frame[*indicator] = XPROTOCOL_ESC;
        (*indicator)++;
        if (maxFrameLength <= *indicator)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
        }
        frame[*indicator] = XPROTOCOL_ESCAPED_ESC;
    }
    (*indicator)++;
    return RETCODE_OK;
}

static void UnEscape_SpecialCharacters(const uint8_t *frame, uint32_t indicator,
        uint8_t *data, uint32_t counter)
{
    if (XPROTOCOL_ESC == frame[indicator])
    {
        /* check the byte after escape letter */
        if (XPROTOCOL_ESCAPED_SD == frame[indicator + 1])
        {
            data[counter] = XPROTOCOL_SD;
        }
        else if (XPROTOCOL_ESCAPED_ED == frame[indicator + 1])
        {
            data[counter] = XPROTOCOL_ED;
        }
        else if (XPROTOCOL_ESCAPED_ESC == frame[indicator + 1])
        {
            data[counter] = XPROTOCOL_ESC;
        }
    }
    else
    {
        data[counter] = frame[indicator];
    }
}

/*  The description of the function is available in Kiso_XProtocol.h */
Retcode_T XProtocol_Init(void)
{
    return RETCODE_OK;
}

/*  The description of the function is available in Kiso_XProtocol.h */
Retcode_T XProtocol_EncodeFrame(const uint8_t *data, uint32_t dataLength,
        uint32_t maxFrameLength, uint8_t *frame, uint32_t *frameLength)
{
    uint32_t indicator = UINT32_C(0);
    uint16_t checksum = UINT16_C(0x0000);
    Retcode_T retcode = RETCODE_OK;

    if (NULL == data || NULL == frame || NULL == frameLength)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    /* Calculate payload's checksum based on CRC-CCITT (XModem)
     * ignoring the Retcode_T returning value. We know that it's
     * always returning RETCODE_OK because we check for NULL pointers
     * ourselves.
     */
    // TODO: Check for dataLength >= 0xFFFF because of the downcast
    (void) CRC_16(XPROTOCOL_CRC_CCITT_POLY, &checksum, data, (uint16_t) dataLength);

    /* create encoded frame byte per byte */
    /* set start delimiter */
    if (maxFrameLength > indicator)
    {
        frame[indicator] = XPROTOCOL_SD;
        indicator++;
    }
    else
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
    }

    /* set 1st byte of checksum (Network Byte Order) */
    if (maxFrameLength > indicator)
    {
        frame[indicator] = (uint8_t)(checksum >> 8);
    }
    else
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
    }

    /* escape special character of 1st byte of checksum if exists */
    retcode = Escape_SpecialCharacters(frame, &indicator, maxFrameLength);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    /* set 2nd byte of checksum (Network Byte Order) */
    if (maxFrameLength > indicator)
    {
        frame[indicator] = (uint8_t)(checksum & 0xFF);
    }
    else
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
    }

    /* escape special character of 2nd byte of checksum if exists */
    retcode = Escape_SpecialCharacters(frame, &indicator, maxFrameLength);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    for (uint32_t x = UINT32_C(0); x < dataLength; x++)
    {
        if (maxFrameLength > indicator)
        {
            frame[indicator] = data[x];
        }
        else
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
        }
        /* escape special character of payload byte if exists */
        retcode = Escape_SpecialCharacters(frame, &indicator, maxFrameLength);
        if (RETCODE_OK != retcode)
        {
            return retcode;
        }
    }
    if (maxFrameLength > indicator)
    {
        frame[indicator] = XPROTOCOL_ED;
    }
    else
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
    }
    *frameLength = indicator + 1;

    return RETCODE_OK;
}

/*  The description of the function is available in Kiso_XProtocol.h */
Retcode_T XProtocol_GetPayloadLength(const uint8_t *frame, uint32_t frameLength,
        uint32_t *payloadLength)
{
    uint32_t counter = UINT32_C(0);
    if (NULL == payloadLength || NULL == frame)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* check the number of checksum and payload bytes of frame after decoding */
        for (uint32_t x = UINT32_C(0); x < frameLength; x++)
        {
            if (XPROTOCOL_ESC == frame[x])
            {
                x++;
            }
            counter++;
        }
        /* we decrease the counter by 4 (the bytes of SD, ED and the 2
         * bytes of checksum. Result: we get only the number of payload's bytes) */
        if (UINT32_C(4) < counter)
        {
            *payloadLength = counter - 4;
        }
        else
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_INTEGRITY_FAILED);
        }
    }
    return RETCODE_OK;
}

/*  The description of the function is available in Kiso_XProtocol.h */
Retcode_T XProtocol_DecodeFrame(const uint8_t *frame, uint32_t frameLength,
        uint32_t maxDataLength, uint8_t *data, uint32_t *dataLength)
{
    uint16_t checksum = UINT16_C(0x0000);
    uint8_t chksmbuff[2] =
    {   0};

    if (NULL == data || NULL == dataLength || NULL == frame)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (UINT32_C(0) == frameLength)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
    }

    /* check for missing start delimiter */
    if (XPROTOCOL_SD != frame[0])
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_START_DELIMITER_MISSING);
    }

    /* check for missing end delimiter */
    if (XPROTOCOL_ED != frame[frameLength - 1])
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_END_DELIMITER_MISSING);
    }

    /* number of checksum bytes */
    uint32_t checksumbytes = UINT32_C(2);

    uint32_t counter = UINT32_C(0);
    /* separate checksum bytes to a 2bytes buffer (Network Byte Order) */
    for (uint32_t x = UINT32_C(1); x <= checksumbytes; x++)
    {
        /* counter < 2 is always true */
        if (frameLength > x)
        {
            UnEscape_SpecialCharacters(frame, x, chksmbuff, counter);
            if (XPROTOCOL_ESC == frame[x])
            {
                checksumbytes++;
                x++;
            }
            counter++;
        }
        else
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
        }
    }
    counter = UINT32_C(0);

    /* decode the frame's payload */
    for (uint32_t x = checksumbytes + 1; x < frameLength - 1; x++)
    {
        if (maxDataLength <= counter)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_DATA_BUFFER_TOO_SMALL);
        }
        if (frameLength > x)
        {
            UnEscape_SpecialCharacters(frame, x, data, counter);
            if (XPROTOCOL_ESC == frame[x])
            {
                x++;
            }
            counter++;
        }
        else
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL);
        }
    }

    /* retrieve frame's checksum */
    uint16_t cs = ((uint16_t) chksmbuff[0] << 8) | chksmbuff[1];

    /* Calculate payload's checksum based on CRC-CCITT (XModem)
     * ignoring the Retcode_T returning value. We know that it's
     * always returning RETCODE_OK because we check for NULL pointers
     * ourselves.
     */
    (void) CRC_16(XPROTOCOL_CRC_CCITT_POLY, &checksum, data, (uint16_t)(counter));

    /* check if calculated checksum is the same with retrieved (Network Byte Order) */
    if (checksum != cs)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_INTEGRITY_FAILED);
    }

    *dataLength = counter;
    return RETCODE_OK;
}

/*  The description of the function is available in Kiso_XProtocol.h */
Retcode_T XProtocol_IsCompleteFrame(const uint8_t *frame, uint32_t frameLength,
        const uint8_t **lastCheckPosition)
{
    if (NULL == frame)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (UINT32_C(0) < frameLength)
    {
        if (NULL != lastCheckPosition)
        {
            *lastCheckPosition = &frame[frameLength - 1];
        }

        if (XPROTOCOL_ED != frame[frameLength - 1])
        {
            return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_XPROTOCOL_FRAME_NOT_COMPLETE_YET);
        }
    }
    else
    {
        return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_XPROTOCOL_START_DELIMITER_MISSING);
    }

    return RETCODE_OK;
}

#endif /* if KISO_FEATURE_XPROTOCOL */
