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
 * @ingroup UTILS
 *
 * @defgroup CRC CRC
 * @{
 *
 * @brief routines for CRC8, CRC16 and CRC32
 *
 * @details General desciption of how CRC is calculated:
 * source: https://en.wikipedia.org/wiki/Cyclic_redundancy_check (2017-08-14)
 *
 * @code{.unparsed}
 * Always start with a common '1'
 * Shifter set to zero (or initial value)
 *  as long as bits remain in data stream:
 *      if leftmost bit (that is set to 1) in shifter a is not equal to next bit in data stream
 *          shifter := (left shift shifter by one) xor crc polynomial
 *      else
 *          shifter := (left shift shifter by one)
 *  next bit in data stream
 * result in shifter
 * @endcode
 *
 * @file
 **/

#ifndef KISO_CRC_H_
#define KISO_CRC_H_

/* additional interface header files */
#include "Kiso_Utils.h"

#if KISO_FEATURE_CRC

#include "Kiso_Retcode.h"

/* public interface declaration */

/* public type and macro definitions */
#define CRC32_EATH_STD_INIT(x)                  (x = (~0UL))
#define CRC32_INVERSE(x)                        (x = x ^ (~0UL))
#define CRC32_ETHERNET_REVERSE_POLYNOMIAL       UINT32_C(0xEDB88320)      /**< CRC32 polynomial for Ethernet standard IEEE 802.3 */

/* public function prototype declarations */

/**
 * @brief           Calculates CRC8 for a given message, or data, using a a given polynomial and initial value. No frills: no bit-reversing, no final XOR.
 *
 * @param[in]       poly    - polynomial to be used
 * @param[in,out]   shifter - initial value when called; calculated CRC8 when done
 * @param[in]       data_p  - pointer to start of data CRC8 shall be calculated for
 * @param[in]       len     - number of octets in data
 *
 * @retval          #RETCODE_OK when successful
 * @retval          #RETCODE_FAILURE when any of the input pointers is NULL
 */
Retcode_T CRC_8(uint8_t poly, uint8_t * shifter, uint8_t * data_p, uint16_t len);

/**
 * @brief           Calculates CRC16 for a given message, or data, using a a given polynomial and initial value. No frills: no bit-reversing, no final XOR.
 *
 * @param[in]       poly    - polynomial to be used
 * @param[in,out]   shifter - initial value when called; calculated CRC16 when done
 * @param[in]       data_p  - pointer to start of data CRC16 shall be calculated for
 * @param[in]       len     - number of octets in data
 *
 *
 * @retval          #RETCODE_OK when successful
 * @retval          #RETCODE_FAILURE when any of the input pointers is NULL
 */
Retcode_T CRC_16(uint16_t poly, uint16_t * shifter, uint8_t * data_p, uint16_t len);

/**
 * @brief           Calculates CRC32 for a given message, or data, using a a given polynomial and initial value. No frills: no bit-reversing, no final XOR.
 *
 * @param[in]       poly    - polynomial to be used
 * @param[in,out]   shifter - initial value when called; calculated CRC32 when done
 * @param[in]       data_p  - pointer to start of data CRC32 shall be calculated for
 * @param[in]       len     - number of octets in data
 *
 *
 * @retval          #RETCODE_OK when successful
 * @retval          #RETCODE_FAILURE when any of the input pointers is NULL
 */
Retcode_T CRC_32(uint32_t poly, uint32_t * shifter, uint8_t * data_p, uint16_t len);

/**
 * @brief           Calculates CRC32 with reverse polynomial for a given message, or data and initial value.
 *
 * @param[in]       poly    - polynomial to be used
 * @param[in,out]   shifter - initial value when called; calculated CRC32 when done
 * @param[in]       data_p  - pointer to start of data CRC32 shall be calculated for
 * @param[in]       len     - number of octets in data
 *
 *
 * @retval          #RETCODE_OK when successful
 * @retval          #RETCODE_FAILURE when any of the input pointers is NULL
 *
 * @note            For the first time shifter value will initialize with 0xFFFFFFFF, Call CRC32_EATH_STD_INIT(shifter) macro for initialize
 *                  This api won't support final CRC32 inversion, manually to do the final CRC32 inversion in application.
 *                  Call CRC32_INVERSE(shifter) macro for invert the final CRC32 value.
 */
Retcode_T CRC_32_Reverse(uint32_t poly, uint32_t * shifter, uint8_t * data_p, uint16_t len);

#endif /* if KISO_FEATURE_CRC */

#endif /* KISO_CRC_H_ */
/**@} */
