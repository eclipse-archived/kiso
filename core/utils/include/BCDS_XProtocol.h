/*----------------------------------------------------------------------------*/
/*
 *  Copyright (C) Bosch Connected Devices and Solutions.
 *  All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @ingroup UTILS
 *
 * @defgroup XPROTOCOL XProtocol
 * @{
 *
 * @brief API to encode and decode xProtocol frames.
 *
 *
 * @details Frame format
 *
 * | SD | CS | Payload                 | ED |
 * |----|----|-------------------------|----|
 * | 1B | 2B | N Bytes                 | 1B |
 *
 * SD - Start Delimiter
 *
 * CS - Checksum
 *
 * ED - End Delimiter
 *
 * Any occurrence of SD or ED in CS or payload will be escaped (after
 * the checksum calculation). The special character to escape (ESC)
 * SDs and EDs is escaped as well.
 *
 * The following table shows how escaping is done:
 *
 * | Symbol | Hex Value | Escape Symbol | Hex Value |
 * |--------|-----------|---------------|-----------|
 * | SD     | 0xC0      | ESC_SD        | 0xDBDC    |
 * | ED     | 0xC9      | ESC_ED        | 0xDBDE    |
 * | ESC    | 0xDB      | ESC_ESC       | 0xDBDD    |
 *
 * The complete protocol definition can be found here:
 * https://bcds01.de.bosch.com/wiki/display/BSA/xProtocol
 *
 * @file
 */

#ifndef BCDS_XPROTOCOL_H_
#define BCDS_XPROTOCOL_H_

#include "BCDS_Utils.h"

#if BCDS_FEATURE_XPROTOCOL

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

/**
 * @brief Initializes the xProtocol module
 */
Retcode_T XProtocol_Init(void);

/**
 * @brief Encodes an xProtocol frame.
 * @param[in] data pointer to buffer which contains data from higher level
 *                 protocols, which must not be NULL
 * @param dataLength length of data buffer
 * @param maxFrameLength maximum number of bytes the frame buffer can hold
 * @param[out] frame pointer to buffer which is supposed to hold the encoded
 *                   frame, which must not be NULL
 * @param[out] frameLength length of the encoded frame
 * @retval RETCODE_SUCCESS data encoding to frame successfull
 * @retval RETCODE_NULL_POINTER if data, frame or frameLength is NULL
 * @retval RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL encoded frame does not fit
 *         in frame buffer
 * @retval RETCODE_XPROTOCOL_DATA_EXCEEDES_MAX_PAYLOAD_LENGTH payload length
 *         would exceed the maximum length
 */
Retcode_T XProtocol_EncodeFrame(const uint8_t *data, uint32_t dataLength,
        uint32_t maxFrameLength, uint8_t *frame, uint32_t *frameLength);

/**
 * @brief Decodes an xProtocol frame.
 * @param[in] frame pointer to buffer which contains an encoded frame, which
 *            must not be NULL.
 * @param frameLength length of frame buffer
 * @param maxDataLength maximum number of bytes the data buffer can hold
 * @param[out] data pointer to buffer which is supposed to hold the decoded
 *                  data, which must not be NULL
 * @param[out] dataLength length of data buffer
 * @retval RETCODE_SUCCESS frame decoding successfull
 * @retval RETCODE_NULL_POINTER if data, frame or dataLength is NULL
 * @retval RETCODE_XPROTOCOL_DATA_BUFFER_TOO_SMALL decoded frame does not fit in
 *         data buffer
 * @retval RETCODE_XPROTOCOL_INTEGRITY_FAILED the frame's checksum is wrong
 * @retval RETCODE_XPROTOCOL_START_DELIMITER_MISSING frame does not contain a
 *                                                   start delimiter
 * @retval RETCODE_XPROTOCOL_END_DELIMITER_MISSING frame does not contain an
 *                                                 end delimiter
 */
Retcode_T XProtocol_DecodeFrame(const uint8_t *frame, uint32_t frameLength,
        uint32_t maxDataLength,
        uint8_t *data, uint32_t *dataLength);

/**
 * @brief Check if an xProtocol frame is complete.
 * @param[in] frame pointer to buffer which contains an, possibly incomplete,
 *            encoded frame, which must not be NULL
 * @param frameLength length of frame buffer
 * @param[in, out] lastCheckPosition last position within the frame which has
 *                 been checked. If this function is called the first time on a
 *                 frame, lastCheckPosition must be initialized to `frame'.
 *                 Subsequent calls to XProtocol_IsCompleteFrame for the same
 *                 frame will use and update that parameter, to avoid checking
 *                 the bytes of the frame over and over again.
 *                 This is only meant as a performance optimization. If
 *                 performance is not an issue, then that field may be set to
 *                 NULL.
 * @retval RETCODE_SUCCESS frame buffer contains complete xProtocol frame
 * @retval RETCODE_NULL_POINTER if frame pointer is NULL
 * @retval RETCODE_XPROTOCOL_FRAME_NOT_COMPLETE_YET xProtocol is not complete yet
 * @retval RETCODE_XPROTOCOL_DATA_EXCEEDES_MAX_PAYLOAD_LENGTH payload length
 *         would exceed the maximum length
 */
Retcode_T XProtocol_IsCompleteFrame(const uint8_t *frame, uint32_t frameLength,
        const uint8_t **lastCheckPosition);

/**
 * @brief Determine the payload length
 * @param[in] frame pointer to buffer which contains an, possibly incomplete,
 *            encoded frame, which must not be NULL
 * @param frameLength length of frame buffer
 * @param[out] payloadLength length of payload, must not be NULL
 * @retval RETCODE_SUCCESS payloadLength contains payload length
 * @retval RETCODE_NULL_POINTER if frame pointer or payloadLength is NULL
 */
Retcode_T XProtocol_GetPayloadLength(const uint8_t *frame, uint32_t frameLength,
        uint32_t *payloadLength);

/**
 * @deprecated since version vxx.xx.xx, replaced by #XProtocol_Init
 * @todo replace vxx.xx.xx with next version
 *
 * @brief Initializes the xProtocol module
 */
BCDS_DEPRECATED(Retcode_T xProtocol_Init(void));

/**
 * @deprecated since version vxx.xx.xx, replaced by #XProtocol_EncodeFrame
 * @todo replace vxx.xx.xx with next version
 *
 * @brief Encodes an xProtocol frame.
 * @param[in] data pointer to buffer which contains data from higher level
 *                 protocols, which must not be NULL
 * @param dataLength length of data buffer
 * @param maxFrameLength maximum number of bytes the frame buffer can hold
 * @param[out] frame pointer to buffer which is supposed to hold the encoded
 *                   frame, which must not be NULL
 * @param[out] frameLength length of the encoded frame
 * @retval RETCODE_SUCCESS data encoding to frame successfull
 * @retval RETCODE_NULL_POINTER if data, frame or frameLength is NULL
 * @retval RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL encoded frame does not fit
 *         in frame buffer
 * @retval RETCODE_XPROTOCOL_DATA_EXCEEDES_MAX_PAYLOAD_LENGTH payload length
 *         would exceed the maximum length
 */
BCDS_DEPRECATED(Retcode_T xProtocol_EncodeFrame(const uint8_t *data, uint32_t dataLength,
        uint32_t maxFrameLength, uint8_t *frame, uint32_t *frameLength));

/**
 * @deprecated since version vxx.xx.xx, replaced by #XProtocol_DecodeFrame
 * @todo replace vxx.xx.xx with next version
 *
 * @brief Decodes an xProtocol frame.
 * @param[in] frame pointer to buffer which contains an encoded frame, which
 *            must not be NULL.
 * @param frameLength length of frame buffer
 * @param maxDataLength maximum number of bytes the data buffer can hold
 * @param[out] data pointer to buffer which is supposed to hold the decoded
 *                  data, which must not be NULL
 * @param[out] dataLength length of data buffer
 * @retval RETCODE_SUCCESS frame decoding successfull
 * @retval RETCODE_NULL_POINTER if data, frame or dataLength is NULL
 * @retval RETCODE_XPROTOCOL_DATA_BUFFER_TOO_SMALL decoded frame does not fit in
 *         data buffer
 * @retval RETCODE_XPROTOCOL_INTEGRITY_FAILED the frame's checksum is wrong
 * @retval RETCODE_XPROTOCOL_START_DELIMITER_MISSING frame does not contain a
 *                                                   start delimiter
 * @retval RETCODE_XPROTOCOL_END_DELIMITER_MISSING frame does not contain an
 *                                                 end delimiter
 */
BCDS_DEPRECATED(Retcode_T xProtocol_DecodeFrame(const uint8_t *frame, uint32_t frameLength,
        uint32_t maxDataLength,
        uint8_t *data, uint32_t *dataLength));

/**
 * @deprecated since version vxx.xx.xx, replaced by #XProtocol_IsCompleteFrame
 * @todo replace vxx.xx.xx with next version
 *
 * @brief Check if an xProtocol frame is complete.
 * @param[in] frame pointer to buffer which contains an, possibly incomplete,
 *            encoded frame, which must not be NULL
 * @param frameLength length of frame buffer
 * @param[in, out] lastCheckPosition last position within the frame which has
 *                 been checked. If this function is called the first time on a
 *                 frame, lastCheckPosition must be initialized to `frame'.
 *                 Subsequent calls to XProtocol_IsCompleteFrame for the same
 *                 frame will use and update that parameter, to avoid checking
 *                 the bytes of the frame over and over again.
 *                 This is only meant as a performance optimization. If
 *                 performance is not an issue, then that field may be set to
 *                 NULL.
 * @retval RETCODE_SUCCESS frame buffer contains complete xProtocol frame
 * @retval RETCODE_NULL_POINTER if frame pointer is NULL
 * @retval RETCODE_XPROTOCOL_FRAME_NOT_COMPLETE_YET xProtocol is not complete yet
 * @retval RETCODE_XPROTOCOL_DATA_EXCEEDES_MAX_PAYLOAD_LENGTH payload length
 *         would exceed the maximum length
 */
BCDS_DEPRECATED(Retcode_T xProtocol_IsCompleteFrame(const uint8_t *frame, uint32_t frameLength,
        const uint8_t **lastCheckPosition));

/**
 * @deprecated since version vxx.xx.xx, replaced by #XProtocol_GetPayloadLength
 * @todo replace vxx.xx.xx with next version
 *
 * @brief Determine the payload length
 * @param[in] frame pointer to buffer which contains an, possibly incomplete,
 *            encoded frame, which must not be NULL
 * @param frameLength length of frame buffer
 * @param[out] payloadLength length of payload, must not be NULL
 * @retval RETCODE_SUCCESS payloadLength contains payload length
 * @retval RETCODE_NULL_POINTER if frame pointer or payloadLength is NULL
 */
BCDS_DEPRECATED(Retcode_T xProtocol_GetPayloadLength(const uint8_t *frame, uint32_t frameLength,
        uint32_t *payloadLength));

#endif /* if BCDS_FEATURE_XPROTOCOL */

#endif /* BCDS_XPROTOCOL_H_ */
/**@} */
