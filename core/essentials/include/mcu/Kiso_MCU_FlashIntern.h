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
 * @defgroup    KISO_HAL_MCU_FLASH_INTERN  MCU Internal Flash
 * @ingroup     KISO_HAL_MCU_IF
 * @{
 *
 * @brief       Non-volatile memory interface
 *
 * @details
 */

#ifndef KISO_MCU_FLASH_INTERN_H_
#define KISO_MCU_FLASH_INTERN_H_

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_FLASH_INTERN

/* Include all headers which are needed by this file. */
#include "Kiso_Essentials.h"

/* Put the type and macro definitions here */

/**
 * @brief enumeration of special internal flash memory interface error return codes
 */
enum RETCODE_MCU_FLASH_INTERN_ERROR_E
{
    RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND = RETCODE_FIRST_CUSTOM_CODE, /**< Erase sequence error */
    RETCODE_MCU_FLASH_INTERN_INVALID_ADDRESS,                                  /**< Invalid address. Write to an address that is not within the internal flash memory. */
    RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_PAGE_ADDR,                    /**< The given address is not aligned to any of the pages. */
    RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR,                  /**< The given address is not aligned to any of the sectors. */
    RETCODE_MCU_FLASH_INTERN_ADDRESS_LOCKED,                                   /**< Flash address is locked. */
    RETCODE_MCU_FLASH_INTERN_TIMEOUT,                                          /**< Timeout while writing to internal flash memory. */
    RETCODE_MCU_FLASH_INTERN_LOCK_FAILED,                                      /**< Failed to lock the internal flash memory. */
    RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED,                                    /**< Failed to unlock the internal flash memory. */
    RETCODE_MCU_FLASH_INTERN_ERASE_ERROR,                                      /**< Programming Sequence error */
    RETCODE_MCU_FLASH_INTERN_PROG_ERROR,                                       /**< Programming Alignment error */
    RETCODE_MCU_FLASH_INTERN_PROG_PARALLELISM_ERROR,                           /**< Programming Parallelism error  */
    RETCODE_MCU_FLASH_INTERN_PROG_ALIGMENT_ERROR,                              /**< Programming Alignment error */
    RETCODE_MCU_FLASH_INTERN_WRITE_PROTECTED_ERROR,                            /**< Write protection error  */
    RETCODE_MCU_FLASH_INTERN_FLASH_OPERATION_ERROR,                            /**< Flash operation Error  */
    RETCODE_MCU_FLASH_INTERN_READ_PROTECTED_ERROR,                             /**< Read Protection Error */
    RETCODE_MCU_FLASH_INTERN_FAILED_TO_CONFIGURE_WRITE_PROTECTION,             /**< Write protection error  */
};

/**
 *  @brief  Internal flash callback pointer.
 *
 *  This is the typedef that will allow a callback function to be specified in the
 *  MCU_FlashIntern_S structure which provides interrupt to the application.
 */
typedef void (*FlashIntern_Callback)(uint32_t, void *);

/**
 * Internal flash interface driver initialization structure passed as argument to the API
 * MCU_FlashIntern_Initialize().
 */
typedef struct MCU_FlashIntern_S
{
    enum KISO_HAL_CallMode_E CallMode; /**< Blocking or interrupt mode selection for the internal flash operations. */
    FlashIntern_Callback Callback;     /**< Pointer to callback upon an interrupt from the internal flash in interrupt mode.
     A NULL parameter must be provided if blocking mode was selected. */
} MCU_FlashIntern_T;

/* Put the function declarations here */

/** @brief API to initialize the internal flash interface driver.
 *
 * @details Prior to any other call to any of the API exposed by the internal flash interface driver a call
 * to the API MCU_FlashIntern_Initialize() must be made. In this API the operation mode is configured (blocking or
 * interrupt mode). And if interrupt mode was selected the application callback for the internal flash driver is
 * registered.
 *
 * @param [in] flashInternInitStruct : internal flash interface driver initialization structure.
 *
 * @return RETCODE_OK if all operation run successfully.
 */
Retcode_T MCU_FlashIntern_Initialize(MCU_FlashIntern_T flashInternInitStruct);

/** @brief This API can be used to erase the content of a given non-volatile memory.
 *
 * @details It verifies if the provided start and end address are within the boundaries of
 * the non-volatile memory. The provided address must match at least the start and end address
 * of the smallest erasable memory  (page, sector, block,...). If the parameter are valid
 * the API calls lower level MCU API from the vendor to unlock the internal flash memory, erase the desired
 * memory section and lock it back again.
 *
 * @param [in] startAddress : start address of the memory section to erase.
 * @param [in] endAddress : end address of the memory section to erase.
 *
 * @retval RETCODE_OK if all operation run successfully.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND if the given addresses are out of the memory's boundaries.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_PAGE_ADDR if the start and end address are not aligned to a page's address.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR if the start and end address are not aligned to a sectors's address.
 * @retval RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED if unlocking the internal flash memory failed.
 * @retval RETCODE_MCU_FLASH_INTERN_ERASE_ERROR if an error occurred during erase.
 * @retval RETCODE_MCU_FLASH_INTERN_WRITE_PROTECTED_ERROR if a or more section are write protected.
 * @retval RETCODE_MCU_FLASH_INTERN_LOCK_FAILED if locking the internal flash memory failed.
 */
Retcode_T MCU_FlashIntern_Erase(uint32_t startAddress, uint32_t endAddress);

/** @brief This API writes to a given address in the non-volatile memory.
 *
 * @details It verifies if the provided start address is within the boundaries of the non-volatile memory.
 * Unlocks the internal flash memory, writes the desired value and locks back the internal flash memory.
 *
 * @param [in] startAddress : start address of the memory section to write to.
 * @param [in] buffer : pointer to a buffer of type uint8_t containing the bytes to write.
 * @param [in] length : number of bytes to write.
 *
 * @retval RETCODE_OK if all operation run successfully.
 * @retval RETCODE_NULL_POINTER if a null pointer was provided for parameter buffer.
 * @retval RETCODE_INVALID_PARAM if length is inferior/equal to 0.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND if the given addresses are out of the memory's boundaries.
 * @retval RETCODE_MCU_FLASH_INTERN_PROG_ERROR if an error while writing to the internal flash memory.
 * @retval RETCODE_MCU_FLASH_INTERN_WRITE_PROTECTED_ERROR if a or more section are write protected.
 * @retval RETCODE_MCU_FLASH_INTERN_LOCK_FAILED if locking the internal flash memory failed.
 */
Retcode_T MCU_FlashIntern_Write(uint32_t StartAddress, uint8_t *buffer, uint32_t length);

/** @brief This API read from a given address in the non-volatile memory.
 *
 * @details It verifies if the provided start address is within the boundaries of the non-volatile memory.
 * Copies the internal flash memory's content into the provided buffer.
 *
 * @param [in] startAddress : start address of the memory section to read from.
 * @param [out] buffer : pointer to a buffer of type uint8_t where the internal flash memory's content is copied to.
 * @param [in] length : number of bytes to read.
 *
 * @retval RETCODE_OK if all operation run successfully.
 * @retval RETCODE_NULL_POINTER if a null pointer was provided for parameter buffer.
 * @retval RETCODE_INVALID_PARAM if length is inferior/equal to 0.
 */
Retcode_T MCU_FlashIntern_Read(uint32_t startAddress, uint8_t *buffer, uint32_t length);

/** @brief This API enables/disables protecting against writing to a given section of the internal memory.
 *
 * @details It determines the blocks, sectors or pages included in the given memory section,
 * and call the corresponding third-party interface driver.
 *
 * @param [in] startAddress : start address of the memory section affected by the write protection.
 * @param [in] endAddress : end address of the memory section affected by the write protection.
 * @param [in] enable : enable/disable write protection flag.
 *
 * @retval RETCODE_OK if all operation run successfully.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_PAGE_ADDR if the start and end address are not aligned to a page's address.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR if the start and end address are not aligned to a sectors's address.
 */
Retcode_T MCU_FlashIntern_WriteProtect(uint32_t startAddress, uint32_t endAddress, bool enable);

/** @brief This API returns the the minimum read/write-able size of FlashIntern
 *
 * @details Read/write addresses need to be aligned to this size boundaries. Read/write lengths
 *          need to be a multiple of this size.
 *
 * @retval uint32_t Minimum read/write-able size of FlashIntern
 */
uint32_t MCU_FlashIntern_GetMinRWSize(void);

#endif /* KISO_FEATURE_FLASH_INTERN */
#endif /* KISO_MCU_FLASH_INTERN_H_ */
