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
 * @brief Defines utility functions shared within the driver
 *
 * @details Enables such functionality as:
 * converting string to long,
 * converting hex to binary and vice versa,
 * trim fluke characters,
 * handling of normal and AT response codes,
 */

#ifndef ATUTILS_H_
#define ATUTILS_H_

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

#include "AtResponseParser.h"

/**
 * @brief Converts an AT response queue Retcode to a Cellular retcode, and passes through Cellular retcodes.
 *
 * @param[in] retcode The AT response queue retcode to convert
 *
 * @retval RETCODE_OK                           If the retcode was RETCODE_OK
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE      If the retcode was RETCODE_AT_RESPONSE_QUEUE_TIMEOUT
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the retcode was RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT or RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT
 */
Retcode_T Utils_ConvertAtResponseRetcodeToCellularRetcode(Retcode_T retcode);

/**
 * @brief Converts an AT response code to a Retcode to comply with the driver retcode behavior
 *        specified in BLUCOW-176.
 *
 * @param[in] response The AT response code to convert to a retcode
 *
 * @retval RETCODE_OK                           If the response code was AT_RESPONSE_CODE_OK
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED   If the response code was AT_RESPONSE_CODE_ABORTED
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR     If the response code was AT_RESPONSE_CODE_ERROR
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER     If the response code was any other
 */
Retcode_T Utils_ConvertCellularResponseCodeToCellularRetcode(AtResponseCode_T response);

/**
 * @brief Parses a string to a signed long number
 *
 * @param[in] buffer        The buffer holding the string to parser
 * @param[in] BufferLength  The length of the buffer
 * @param[out] number       The parsed number
 *
 * @retval RETCODE_OK               The number was parsed correctly
 * @retval RETCODE_FAILURE          The string did not represent a number
 * @retval RETCODE_NULL_POINTER     The buffer or number param was a NULL pointer
 * @retval RETCODE_INVALID_PARAM    The number did exceed the int32_t max value
 */
Retcode_T Utils_Strtol(const uint8_t *buffer, uint32_t bufferLength, int32_t *number);

/**
 * @brief Parses a string to a signed long number and checks if the number is within given bounds.
 *        If the parsed number exceeds the bounds, it is clamped to the respective bound and RETCODE_INVALID_PARAM
 *        is returned.
 *
 * @param[in] buffer        The buffer holding the string to parser
 * @param[in] BufferLength  The length of the buffer
 * @param[out] number       The parsed number
 *
 * @retval RETCODE_OK               The number was parsed correctly
 * @retval RETCODE_FAILURE          The string did not represent a number
 * @retval RETCODE_NULL_POINTER     The buffer or number param was a NULL pointer
 * @retval RETCODE_INVALID_PARAM    The number did exceed the int32_t max value or was not within bounds
 */
Retcode_T Utils_StrtolBounds(const uint8_t *buffer, uint32_t bufferLength, int32_t *number, int32_t lowerBound, int32_t upperBound);

/**
 * @brief Waits for a AT response code from the modem and merges it with the passed retcode to generate a unified
 * retcode that can be passed to the application-layer.
 *
 * See Table:
 * Cellular response    | Retcode OK                        | Retcode nOK
 * ---------------------|-----------------------------------|-------------
 * modem resp. OK:      | Retcode OK                        | ↑
 * modem resp. nOK:     | Retcode from modem response       | ←
 * modem resp. timeout: | Retcode modem not responsive      | ←
 * other WaitFor-error: | Retcode modem resp. not expected  | ←
 *
 * @param[in] timeout The timeout that this function
 * @param[in] retcode The retcode from previous driver and AT-queue calls. This will be merged with the response code
 * from the modem
 *
 * @return A retcode that merged the retcode passed as parameter with the response gotten from the modem.
 */
Retcode_T Utils_WaitForAndHandleResponseCode(uint32_t timeout, Retcode_T retcode);

/**
 * @brief Trims "fluke" characters from the beginning of a buffer. "Fluke characters" are all chars/bytes
 *        less than 0x20 and greater than 0x7E, except 0x0A and 0x0D (CR, LF).
 *        This function does not modify the buffer content.
 *
 * @param[in] buffer The buffer to trim the chars from
 * @param[in] BufferLength The original length of the buffer
 * @param[out] NewBufferLength The length of the result buffer
 *
 * @return The start of the "fluke free" buffer
 */
const uint8_t *Utils_TrimFlukeCharacters(const uint8_t *buffer, uint32_t bufferLength, uint32_t *newBufferLength);

#endif /* ATUTILS_H_ */
