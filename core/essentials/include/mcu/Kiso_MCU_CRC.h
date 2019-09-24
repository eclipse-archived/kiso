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
 * @defgroup    KISO_HAL_MCU_CRC MCU CRC
 * @ingroup     KISO_HAL_MCU_IF
 * @{
 *
 * @brief       MCU CRC Peripheral Driver
 *
 * @details     The CRC Peripheral Driver will be initialized internally when particular CRC APIs are called for
 *              calculating CRC.
 */

#ifndef KISO_MCU_CRC_H
#define KISO_MCU_CRC_H

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_MCU_CRC

#define MCU_CRC32_POLY_ETHERNET 0x04C11DB7      /**< @def Default polynomial for 32-Bit CRC */
#define MCU_CRC32_INIT_VALUE_DEFAULT 0XFFFFFFFF /**< @def Default initial Value for 32-Bit CRC */

/**
 * @brief       Defines input data format
 */
enum MCU_CRC_DataType_E
{
    MCU_CRC_DATA_8BIT,
    MCU_CRC_DATA_16BIT,
    MCU_CRC_DATA_32BIT,
};

typedef enum MCU_CRC_DataType_E MCU_CRC_DataType_T;

/**
 * @brief       Typedef for the 8-Bit CRC generating polynomial  and initial value to initiate CRC computation
 */
struct MCU_CRC8_S
{
    uint8_t GeneratePolynomial;
    uint8_t InitVal;
};

/**
 * @brief       Typedef for the 16-Bit CRC generating polynomial  and initial value to initiate CRC computation
 */
struct MCU_CRC16_S
{
    uint16_t GeneratePolynomial;
    uint16_t InitVal;
};

/**
 * @brief       Typedef for the 32-Bit CRC generating polynomial  and initial value to initiate CRC computation based
 *              on the Input data format.
 */
struct MCU_CRC32_S
{
    uint32_t GeneratePolynomial;
    uint32_t InitVal;
    MCU_CRC_DataType_T DataFormat;
};

/**
 * @brief       Computes the 8-bit CRC value of an 8-bit data buffer
 *
 * @details     User should pass the value of polynomial and initialization values to calculate the CRC
 *
 * @param[in]   initParms: Input parameters for calculating CRC.
 *
 * @param[in]   data_in: Pointer to the input data buffer.
 *
 * @param[in]   dataLength: Input data buffer length (number of bytes for data_in type is * uint8_t).
 *
 * @param[out]  crc: Pointer to store the calculated CRC.
 *
 * @retval      RETCODE_OK on success, or an error code otherwise.
 */
Retcode_T MCU_CRC8(struct MCU_CRC8_S *initParms, uint8_t *data_in, uint32_t dataLength, uint8_t *crc);

/**
 * @brief       Compute the 16-bit CRC value of an 16-bit data buffer
 *
 * @details     User should pass the value of polynomial and initialization values to calculate the CRC.
 *
 * @param[in]   initParms: Input parameters for calculating CRC.
 *
 * @param[in]   data_in: Pointer to the input data buffer.
 *
 * @param[in]   dataLength: Input data buffer length (number of half-words for data_in type * uint16_t).
 *
 * @param[out]  crc: Pointer to store the calculated CRC.
 *
 * @retval      RETCODE_OK on success, or an error code otherwise.
 */
Retcode_T MCU_CRC16(struct MCU_CRC16_S *initParms, uint16_t *data_in, uint32_t dataLength, uint16_t *crc);

/**
 * @brief       Compute the 32-bit CRC value of an 32-bit data buffer
 *
 * @details     User should pass the value of polynomial,input data format and initialization values to calculate the CRC.
 *
 * @param[in]   initParms: Input parameters for calculating CRC.
 *
 * @param[in]   data_in: Pointer to the input data buffer.
 *
 * @param[in]   dataLength: Input data buffer length: depending on DataFormat specified in initParms structure
 *                  - count of bytes if DataFormat is MCU_CRC_DATA_8BIT
 *                  - count of 2-bytes if DataFormat is MCU_CRC_DATA_16BIT
 *                  - count of 4-bytes if DataFormat is MCU_CRC_DATA_32BIT
 *
 * @param[out]  crc: Pointer to store the calculated CRC.
 *
 * @retval      RETCODE_OK on success, or an error code otherwise.
 *
 * @note        If user wants to accumulate CRC,use below API with previously computed CRC as initialization value
 */
Retcode_T MCU_CRC32(struct MCU_CRC32_S *initParms, uint32_t *data_in, uint32_t dataLength, uint32_t *crc);

#endif /* KISO_FEATURE_MCU_CRC  */
#endif /* KISO_MCU_CRC_H */
/**@} */
