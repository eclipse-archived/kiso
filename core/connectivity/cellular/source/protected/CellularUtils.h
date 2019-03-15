/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @file
 *
 * @brief Defines utility functions shared within the driver
 *
 * @details Enables such functionality as:
 * * converting string to long
 * * converting hex to binary and vice versa
 * * trim fluke characters
 * * handling of normal and AT response codes
 */

#ifndef CELLULAR_UTILS_H
#define CELLULAR_UTILS_H

/* Include all headers which are needed by this file. */
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

/* Put the type and macro definitions here */
/**
 * @brief The maximum length of a string representing an IPv4 address
 */
#define CELLULAR_UTILS_MAX_IP_STR_LENGTH          (UINT32_C(15))

/* Put the function declarations here */

/**
 * @brief Converts an AT response queue Retcode to a Cellular retcode, and passes through Cellular retcodes.
 *
 * @param[in] retcode The AT response queue retcode to convert
 *
 * @retval RETCODE_OK                               If the retcode was RETCODE_OK
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE       If the retcode was RETCODE_AT_RESPONSE_QUEUE_TIMEOUT
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED  If the retcode was RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT or RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT
 */
Retcode_T CellularUtils_ConvertAtResponseRetcodeToCellularRetcode(Retcode_T retcode);

/**
 * @brief Converts an AT response code to a Retcode to comply with the driver retcode behavior
 *        specified in BLUCOW-176.
 *
 * @param[in] response The AT response code to convert to a retcode
 *
 * @retval RETCODE_OK                               If the response code was AT_RESPONSE_CODE_OK
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED    If the response code was AT_RESPONSE_CODE_ABORTED
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR      If the response code was AT_RESPONSE_CODE_ERROR
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER      If the response code was any other
 */
Retcode_T CellularUtils_ConvertCellularResponseCodeToCellularRetcode(AtResponseCode_T response);

/**
 * @brief Parses a string to a signed long number
 *
 * @param[in] buffer        The buffer holding the string to parser
 * @param[in] BufferLength  The length of the buffer
 * @param[out] number       The parsed number
 *
 * @retval RETCODE_OK       The number was parsed correctly
 * @retval RETCODE_FAILURE  The string did not represent a number
 * @retval RETCODE_NULL_POINTER    The buffer or number param was a NULL pointer
 * @retval RETCODE_INVALID_PARAM   The number did exceed the int32_t max value
 */
Retcode_T CellularUtils_Strtol(const uint8_t *buffer, uint32_t BufferLength, int32_t *number);

/**
 * @brief Parses a string to a signed long number and checks if the number is within given bounds.
 *        If the parsed number exceeds the bounds, it is clamped to the respective bound and RETCODE_INVALID_PARAM
 *        is returned.
 *
 * @param[in] buffer        The buffer holding the string to parser
 * @param[in] BufferLength  The length of the buffer
 * @param[out] number       The parsed number
 *
 * @retval RETCODE_OK       The number was parsed correctly
 * @retval RETCODE_FAILURE  The string did not represent a number
 * @retval RETCODE_NULL_POINTER    The buffer or number param was a NULL pointer
 * @retval RETCODE_INVALID_PARAM   The number did exceed the int32_t max value or was not within bounds
 */
Retcode_T CellularUtils_StrtolBounds(const uint8_t *buffer, uint32_t BufferLength, int32_t *number, int32_t LowerBound, int32_t UpperBound);

/**
 * @brief Parses an IP address string to its integer representation.
 *
 * @param[in] IpAddressBuffer       The IP address string to parse
 * @param[in] IpAddressBufferLength The length of the IP address string
 * @param[out] ParsedIpAddress      The parsed IP address
 *
 * @retval RETCODE_INVALID_PARAM    If any of the pointers were NULL or the IP address string was too long
 * @retval RETCODE_FAILURE          If the string was not a valid IPv4 address
 * @retval RETCODE_OK               The IP address was parsed correctly
 */
Retcode_T CellularUtils_ParseIpAddress(uint8_t *IpAddressBuffer, uint32_t IpAddressBufferLength, uint32_t *ParsedIpAddress);

/**
 * @brief Waits for a AT response code from the modem and merges it with the passed retcode to generate a unified
 * retcode that can be passed to the application-layer.
 *
 * @param[in] timeout The timeout that this function
 * @param[in] retcode The retcode from previous driver and AT-queue calls. This will be merged with the response code
 * from the modem
 *
 * @return A retcode that merged the retcode passed as parameter with the response gotten from the modem.
 *
 * See Table:
 * Cellular response    | Retcode OK                        | Retcode nOK
 * ---------------------|-----------------------------------|-------------
 * modem resp. OK:      | Retcode OK (â†‘)                    | â†‘ (converted)
 * modem resp. nOK:     | â†�                                 | â†�
 * modem resp. timeout: | Retcode modem not responsive      | <- see
 * other WaitFor-error: | Retcode modem resp. not expected  | <- see
 */
Retcode_T CellularUtils_WaitForAndHandleResponseCode(uint32_t timeout, Retcode_T retcode);

/**
 * @brief Converts hexadecimal characters to binary array.
 *
 * @param[in] hex                   hexadecimal characters source
 * @param[out] bin                  binary destination
 * @param[in] BinLength            length of the binary destination
 *
 * @retval RETCODE_INVALID_PARAM    If any of the hexadecimal character conversion failed
 * @retval RETCODE_OK               If the conversion accomplished successfully
 */
Retcode_T CellularUtils_HexToBin(const uint8_t *hex, uint8_t *bin, uint32_t BinLength);

/**
 * @brief Converts binary array to hexadecimal characters.
 *
 * @param[in] bin                   binary source
 * @param[out] hex                  hexadecimal characters destination
 * @param[in] BinLength            length of the binary source
 */
void CellularUtils_BinToHex(const uint8_t *bin, char *hex, uint32_t BinLength);

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
uint8_t *CellularUtils_TrimFlukeCharacters(uint8_t *buffer, uint32_t BufferLength, uint32_t *NewBufferLength);

#endif /* CELLULAR_UTILS_H */
