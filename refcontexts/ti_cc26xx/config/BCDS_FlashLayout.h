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

#ifndef BCDS_FLASHLAYOUT_H
#define BCDS_FLASHLAYOUT_H

#include <stdint.h>

/**
 * @brief
 *      Size of a page in the internal flash
 */
#define FLASHLAYOUT_INTFLASH_PAGE_SIZE UINT32_C(0x1000)

/**
 * @brief
 *      Size of a page in the internal flash
 */
#define FOTA_INTFLASH_PAGE_SIZE UINT32_C(0x1000)

/**
 * @brief
 *      Size of a sector in the external flash
 */
#define FOTA_EXTFLASH_SECTOR_SIZE UINT32_C(0x10000)

/**
 * @brief
 *      Size of a page in the internal flash
 */
#define FLASHLAYOUT_EXTFLASH_PAGE_SIZE UINT32_C(0x100)

/**
 * @brief
 *      Size of a page in the internal flash
 */
#define FOTA_EXTFLASH_PAGE_SIZE                    UINT32_C(0x100)

#if FLASHLAYOUT_INTFLASH_PAGE_SIZE <= FLASHLAYOUT_EXTFLASH_PAGE_SIZE || FLASHLAYOUT_INTFLASH_PAGE_SIZE % FLASHLAYOUT_EXTFLASH_PAGE_SIZE
#error "Santity check: FLASHLAYOUT_INTFLASH_PAGE_SIZE must be bigger than and a multiple of FLASHLAYOUT_EXTFLASH_SECTOR_SIZE."
#endif

/**
 * @brief
 *      Size of a page in the external flash.
 */
#define FOTA_INTFLASH_SIZE UINT32_C(0x1D000)

/**
 * @brief
 *      Id of page in which firmware starts
 */
#define FOTA_INTFLASH_FIRMWARE_FIRST_PAGE UINT8_C(1)

/**
 * @brief
 *      Absolute address in external flash where the download should be stored.
 */

#define FLASHLAYOUT_EXTFLASH_DOWNLOAD_ADDR UINT32_C(0x0000)


/**
 * @brief
 *      Absolute address in external flash where the backup should be stored.
 */
#define FLASHLAYOUT_EXTFLASH_BACKUP_ADDR   UINT32_C(0x20000)


/**
 * @brief
 *      Size of the signature at the end of the internal flash. Should not be changed!
 */
#undef FLASHLAYOUT_SIGNATURE_SIZE
#define FLASHLAYOUT_SIGNATURE_SIZE UINT8_C(0x40)

#define FLASHLAYOUT_FIRMWARE_FIRST_PAGE UINT8_C(1)
#define FLASHLAYOUT_FIRMWARE_LAST_PAGE  UINT8_C(29)

/* public type and macro definitions */
#define CRC_INIT_VALUE                             0xAAAAU
#define CRC_POLY                                   0x1021U

#endif /* BCDS_FLASHLAYOUT_H */

