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
 * @defgroup LEANB2CAP LeanB2CAP
 * @{
 *
 * @brief
 *      LeanB2CAP module
 *
 * @details
 *
 * @file
 **/

#ifndef LEANB2CAP_H_
#define LEANB2CAP_H_

#define LEANB2CAP_START_DELIMITER			(0x55U)            	/**< Unique LeanB2CAP start delimiter value */
#define LEANB2CAP_SUPPORTED_CMD_SIZE        (11UL)				/**< Number of command types supported by Lean B2CAP protocol */
#define LEANB2CAP_CRC_POLYNOMIAL			(2UL)               /**< Polynomial value for CRC (must be same at the end of encoder and decoder) */
#define LEANB2CAP_NUM_DATA_PROCESSED        (1UL)         		/**< Number of Payload parsed per LeanB2CAP_RxDataProcessor state machine entry */
#define LEANB2CAP_BYTES_FOR_PAYLOAD_END     (1UL)         		/**< Number of bytes to remaining in current Lean B2CAP frame for last payload byte reception */
#define LEANB2CAP_NUM_OF_BITS_PER_BYTE     	(8UL)         		/**< Number of bits per byte */
#define LEANB2CAP_PROTOCOL_OVERHEAD     	(6UL)         		/**< Number of Lean B2CAP protocol overhead for the raw payload */
#define LEANB2CAP_MIN_DATA_TO_RX_4M_LEN     (4UL)         		/**< Minimum number of Bytes to receive since length */
#define LEANB2CAP_CT_CS1_CS2_OVERHEAD     	(3UL)         		/**< Size overhead to include command type, checksum 1 and checksum 2 fields for raw data */
#define LEANB2CAP_SD_LEN_CS1_OVERHEAD     	(4UL)         		/**< Size overhead to include start delimiter, length and checksum 1 fields for raw data */
#define LEANB2CAP_CS1_OVERHEAD     			(1UL)         		/**< Size overhead to include checksum 1 field for raw data */
#define LEANB2CAP_SD_INDEX_4_RAW_DATA     	(0U)         		/**< Index of start delimiter for raw data */
#define LEANB2CAP_CS1_INDEX_4_RAW_DATA     	(4U)         		/**< Index of checksum 1 for raw data */
#define LEANB2CAP_UNSIGNED_LONG_ZERO     	(0UL)         		/**< Unsigned long representation for numeric value 0. When truncated, will remain the same. */

/**
 * @brief   Enum to represent Lean B2CAP frame processor states.
 */
enum LeanB2CAP_FrameProcStates_E
{
    LEAN_B2CAP_FRMPRC_SD, /**< Processing start delimiter */
    LEAN_B2CAP_FRMPRC_LEN0, /**< Processing data length byte 0 */
    LEAN_B2CAP_FRMPRC_LEN1, /**< Processing data length byte 1 */
    LEAN_B2CAP_FRMPRC_CMD_TYPE, /**< Processing command type */
    LEAN_B2CAP_FRMPRC_CHECKSUM1, /**< Processing check sum 1 */
    LEAN_B2CAP_FRMPRC_PAYLOAD_BYTE, /**< Processing payload */
    LEAN_B2CAP_FRMPRC_CHECKSUM2, /**< Processing check sum 1 */
    LEAN_B2CAP_FRMPRC_ERROR, /**< In error state */
};

/**
 * @brief   Type definition to represent Lean B2CAP frame processor state.
 */
typedef enum LeanB2CAP_FrameProcStates_E LeanB2CAP_FrameProcStates_T;

/**
 * @brief   Enum to represent Lean B2CAP handle descriptors.
 */
struct LeanB2CAP_HandleDescriptor_S
{
    bool HandleOccupance; /**< Is the handle in use or not */
    LeanB2CAP_FrameProcStates_T FrameProcState; /**< Lean B2CAP frame processor state */
    uint8_t CRC1OfRxFrameInProgress; /**< Lean B2CAP frame under process check sum 1 */
    uint8_t CRC2OfRxFrameInProgress; /**< Lean B2CAP frame under process check sum 2 */
    LeanB2CAP_CommandType_T CmdTypeOfRxFrameInProgress; /**< Lean B2CAP frame under process command type */
    uint8_t* RxFrameOutputDataPtr; /**< Lean B2CAP frame valid data storage output buffer pointer */
    uint16_t RxFrameOutputDataBufferSize; /**< Lean B2CAP frame valid data storage output buffer size */
    uint16_t RxFrameIndex; /**< Number of bytes copied to the output buffer */
    uint16_t RxFramePendingProcessSize; /**< Number of bytes being processed in Lean B2CAP frame under process */
    LeanB2CAPCallback Callback;
};

/**
 * @brief   Type definition to represent Lean B2CAP handle descriptor.
 */
typedef struct LeanB2CAP_HandleDescriptor_S LeanB2CAP_HandleDescriptor_T;

/**
 * @brief   LeanB2CAP command type validity checker.
 *
 * @param[in]  type
 * Input command.
 *
 * @return  returns RETCODE_OK on success or an error code otherwise.
 */
static Retcode_T LeanB2CAPIsCommandTypeValid(LeanB2CAP_CommandType_T Type);

#endif /* LEANB2CAP_H_ */
/** @} */
