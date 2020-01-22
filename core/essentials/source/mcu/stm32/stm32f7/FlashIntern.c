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
 * @brief Contains an implementation of a non-volatile memory interface for STM32f7.
 */

#include "Kiso_MCU_FlashIntern.h"

#if KISO_FEATURE_FLASH_INTERN

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_flash.h"
#include "CDDK_Stm32f7MemSectorMap.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_FLASH_INTERN

#define FLASH_INTERN_OB_NB_BANK_MASK OB_NDBANK_SINGLE_BANK
#define FLASH_INTERN_OB_WRP_SECTOR_ENABLE OB_WRP_SECTOR_0
#define FLASH_INTERN_OB_WRP_SECTOR_MASK OB_WRP_SECTOR_All
/**
 *@details Macro to configure the minimum read/write-able size of FlashIntern.
 *         Read/write addresses need to be aligned to this size boundaries.
 *         This is a result of the use of FLASH_TYPEPROGRAM_BYTE
 */
#define FLASH_INTERN_MIN_RW_SIZE sizeof(uint8_t)

/** @brief Private function to convert error from the stm32f7 HAL flash driver into CDDK return codes.
 *
 * @details It calls the API HAL_FLASH_GetError() to retrieve the last error and assigns it a
 * corresponding CDDK internal flash memory module return code. @sa RETCODE_MCU_FLASH_INTERN_E.
 *
 * @retval RETCODE_OK if no error occurred and HAL_FLASH_GetError() returned HAL_FLASH_ERROR_NONE.
 * @retval RETCODE_MCU_FLASH_INTERN_ERASE_ERROR if HAL_FLASH_GetError() returned FLASH_ERROR_ERS.
 * @retval RETCODE_MCU_FLASH_INTERN_PROG_PARALLELISM_ERROR if HAL_FLASH_GetError() returned HAL_FLASH_ERROR_PGP.
 * @retval RETCODE_MCU_FLASH_INTERN_PROG_ALIGMENT_ERROR if HAL_FLASH_GetError() returned HAL_FLASH_ERROR_PGA.
 * @retval RETCODE_MCU_FLASH_INTERN_WRITE_PROTECTED_ERROR if HAL_FLASH_GetError() returned HAL_FLASH_ERROR_WRP.
 * @retval RETCODE_MCU_FLASH_INTERN_FLASH_OPERATION_ERROR if HAL_FLASH_GetError() returned HAL_FLASH_ERROR_OPERATION.
 * @retval RETCODE_MCU_FLASH_INTERN_READ_PROTECTED_ERROR if HAL_FLASH_GetError() returned HAL_FLASH_ERROR_RD.
 * @retval RETCODE_INCONSITENT_STATE if HAL_FLASH_GetError() returned an unknown return code.
 */
static Retcode_T getFlashError(void);

/** @brief This API determines the targeted internal flash memory sector(s) from given start and end address.
 *
 * @details The sector start and end addresses depend on the user's configuration of number of bank. Thus this
 * API first reads out the user's configuration, determines if it is single or dual bank and, then calls the
 * corresponding macro to retrieve the corresponding start and end sector index. @sa FLASH_OBProgramInitTypeDef.
 *
 * @param [in] startAddress : start address of the targeted memory section.
 * @param [in] endAddress : end address of the targeted memory section.
 * @param [out] pStartSector : pointer of type uint32_t where the first sector index is written.
 * @param [out] pStartSector : pointer of type uint32_t where the last sector index is written.
 *
 * @retval RETCODE_OK if all operation run successfully.
 * @retval RETCODE_INVALID_PARAM if the provided start and end address as invalid.
 * @retval RETCODE_NULL_POINTER if the provided parameter pStartSector or pEndSector is null .
 * @retval RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR if the start and end address are not aligned to a sectors's address.
 */
static Retcode_T getCorrespondingSectors(uint32_t startAddress, uint32_t endAddress,
                                         uint32_t *pStartSector, uint32_t *pEndSector);

/** @brief Private function to populate the control block needed by the stm32f7 HAL flash driver to conduct erase operation.
 *
 * @details It verifies if the provided addresses are not out of bound. Get the start sector to erase and the number of
 * sectors to erase. If the given start address doesn't match a sector's start address as well if the end address doesn't
 * not match a sectors end address, it returns an error.
 *
 * @param [out] pEraseInitStruct : pointer of type FLASH_EraseInitTypeDef, where the resulting erase parameters are wrote.
 * @param [in] startAddress : start address of the first sector to be erase.
 * @param [in] endAddress : end address of the last sector to be erase.
 *
 * @retval RETCODE_OK if all operation run successfully.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND if the given addresses are out of the memory's boundaries.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR if the start and end address are not aligned to a sectors's address.
 */
static Retcode_T getEraseParameters(FLASH_EraseInitTypeDef *pEraseInitStruct,
                                    uint32_t startAddress, uint32_t endAddress);

/** @brief Private function to verify if given start and end address are in the non-volatile memory's boundaries.
 *
 * @details It verify if the start address is not smaller than the internal flash memory's base address. Verify as well if the end
 * address is not greater than the internal flash memory's end address or equal/smaller to the start address.
 *
 * @param [in] startAddress : desired internal flash memory's start address.
 * @param [in] endAddress : desired internal flash memory's end address.
 *
 * @retval RETCODE_OK if the provided address are within the internal flash memory's boundaries.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND if the provided address are out of the internal flash memory's boundaries.
 */
static Retcode_T checkAddressBounderies(uint32_t startAddress, uint32_t endAddress);

/* Put function implementations here */

Retcode_T MCU_FlashIntern_Initialize(MCU_FlashIntern_T flashInternInitStruct)
{
    KISO_UNUSED(flashInternInitStruct);

    return RETCODE_OK;
}

/* API documentation is in the interface header. */
Retcode_T MCU_FlashIntern_Erase(uint32_t startAddress, uint32_t endAddress)
{
    FLASH_EraseInitTypeDef eraseInitStruct;

    /* Verify if the start address are in bound and matches a page's or a sector's start address. */
    Retcode_T retcode = getEraseParameters(&eraseInitStruct, startAddress, endAddress);

    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_FLASH_Unlock())
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED);
        }
    }

    if (RETCODE_OK == retcode)
    {
        uint32_t sectorIndexWithError = 0;

        if (HAL_OK != HAL_FLASHEx_Erase(&eraseInitStruct, &sectorIndexWithError))
        {
            retcode = getFlashError();
        }
    }

    /* Regardless of the result of the previous operation lock the internal flash.
     * No need to verify the return code, the API HAL_FLASH_Lock() always returns HAL_OK. */
    (void)HAL_FLASH_Lock();

    return retcode;
}

/* API documentation is in the interface header. */
Retcode_T MCU_FlashIntern_Write(uint32_t startAddress, uint8_t *buffer, uint32_t length)
{
    Retcode_T retcode = RETCODE_OK;

    if (NULL == buffer)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (RETCODE_OK == retcode)
    {
        if (0 == length)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
    }

    if (RETCODE_OK == retcode)
    {
        uint32_t endAddress = startAddress + length;

        retcode = checkAddressBounderies(startAddress, endAddress);
    }

    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_FLASH_Unlock())
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED);
        }
    }

    if (RETCODE_OK == retcode)
    {
        for (uint32_t byteIndex = UINT32_C(0); byteIndex < length; byteIndex++)
        {
            if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, startAddress + byteIndex, (uint64_t)buffer[byteIndex]))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_PROG_ERROR);

                break;
            }
        }
    }

    /* Regardless of the result of the previous operation lock the internal flash.
     * No need to verify the return code, the API HAL_FLASH_Lock() always returns HAL_OK. */
    (void)HAL_FLASH_Lock();

    return retcode;
}

/* API documentation is in the interface header. */
Retcode_T MCU_FlashIntern_Read(uint32_t startAddress, uint8_t *buffer, uint32_t length)
{
    Retcode_T retcode = RETCODE_OK;

    if (NULL == buffer)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (RETCODE_OK == retcode)
    {
        if (0 == length)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
    }

    if (RETCODE_OK == retcode)
    {
        uint32_t endAddress = startAddress + length;

        /* Check whether the Address is within the OTP memory range of information block*/
        if ((FLASH_OTP_BASE <= startAddress) && (FLASH_OTP_END >= endAddress))
        {
            if (startAddress >= endAddress)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            }
        }
        /* Check whether the Address is within the range of main memory block */
        else
        {
            retcode = checkAddressBounderies(startAddress, endAddress);
        }
    }

    if (RETCODE_OK == retcode)
    {
        /* Copy memory into buffer */
        uint8_t *addressToRead = (uint8_t *)startAddress;

        for (uint32_t byteIndex = 0; byteIndex < length; byteIndex++)
        {
            buffer[byteIndex] = addressToRead[byteIndex];
        }
    }

    return retcode;
}

/* API documentation is in the interface header. */
Retcode_T MCU_FlashIntern_WriteProtect(uint32_t startAddress, uint32_t endAddress, bool enable)
{
    FLASH_OBProgramInitTypeDef optionByte = {
        .OptionType = 0,
        .WRPState = 0,
        .WRPSector = 0,
        .RDPLevel = 0,
        .BORLevel = 0,
        .USERConfig = 0,
        .BootAddr0 = 0,
        .BootAddr1 = 0,
#if defined(FLASH_OPTCR2_PCROP)
        .PCROPSector = 0,
        .PCROPRdp = 0
#endif /* FLASH_OPTCR2_PCROP */
    };

    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;

    Retcode_T retcode = checkAddressBounderies(startAddress, endAddress);

    if (RETCODE_OK == retcode)
    {
        retcode = getCorrespondingSectors(startAddress, endAddress, &startSector, &endSector);
    }

    if (RETCODE_OK == retcode)
    {
        /* Retrieve the current configuration. */
        HAL_FLASHEx_OBGetConfig(&optionByte);

        /* Unlock the option byte feature. */
        if (HAL_OK != HAL_FLASH_OB_Unlock())
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_FAILED_TO_CONFIGURE_WRITE_PROTECTION);
        }
    }

    if (RETCODE_OK == retcode)
    {
        uint32_t writeProtectFlag = UINT32_C(0);

        for (uint8_t sectorIndex = startSector; sectorIndex < endSector; sectorIndex++)
        {
            if ((optionByte.USERConfig & FLASH_INTERN_OB_NB_BANK_MASK) == OB_NDBANK_SINGLE_BANK)
            {
                writeProtectFlag |= (FLASH_INTERN_OB_WRP_SECTOR_ENABLE << sectorIndex);
            }
            else
            {
                writeProtectFlag |= (FLASH_INTERN_OB_WRP_SECTOR_ENABLE << (sectorIndex / 2));
            }
        }

        optionByte.WRPSector = writeProtectFlag;
        optionByte.OptionType = OPTIONBYTE_WRP;

        if (enable)
        {
            optionByte.WRPState = OB_WRPSTATE_ENABLE;
        }
        else
        {
            optionByte.WRPState = OB_WRPSTATE_DISABLE;
        }

        if (HAL_OK != HAL_FLASHEx_OBProgram(&optionByte))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_FAILED_TO_CONFIGURE_WRITE_PROTECTION);
        }
    }

    /* Regardless of the result of the previous operation lock the option byte feature.
     * No need to verify the return code, the API HAL_FLASH_OB_Lock() always returns HAL_OK. */
    (void)HAL_FLASH_OB_Lock();

    return retcode;
}

static Retcode_T checkAddressBounderies(uint32_t startAddress, uint32_t endAddress)
{
    Retcode_T retcode = RETCODE_OK;

    if ((FLASH_BASE > startAddress) || (startAddress >= endAddress) ||
        (endAddress > FLASH_END))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND);
    }

    return retcode;
}

static Retcode_T getCorrespondingSectors(uint32_t startAddress, uint32_t endAddress,
                                         uint32_t *pStartSector, uint32_t *pEndSector)
{
    Retcode_T retcode = RETCODE_OK;
    FLASH_OBProgramInitTypeDef optionByte;

    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;

    if ((NULL == pStartSector) || (NULL == pEndSector))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (RETCODE_OK == retcode)
    {
        /* Retrieve the current configuration. */
        HAL_FLASHEx_OBGetConfig(&optionByte);

        if ((optionByte.USERConfig & FLASH_INTERN_OB_NB_BANK_MASK) == OB_NDBANK_SINGLE_BANK)
        {
            startSector = GET_SB_START_SECTOR(startAddress);
            endSector = GET_SB_END_SECTOR(endAddress);
        }
        else
        {
            startSector = GET_DB_START_SECTOR(startAddress);
            endSector = GET_DB_END_SECTOR(endAddress);
        }

        if (startSector == endSector)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }

        if ((startSector == CDDK_FLASH_INTERN_SECTOR_ERROR) || (CDDK_FLASH_INTERN_SECTOR_ERROR == endSector))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDR_UNALIGNED_WITH_SECTOR_ADDR);
        }
    }

    if (RETCODE_OK == retcode)
    {
        *pStartSector = startSector;
        *pEndSector = endSector;
    }

    return retcode;
}

static Retcode_T getEraseParameters(FLASH_EraseInitTypeDef *pEraseInitStruct,
                                    uint32_t startAddress, uint32_t endAddress)
{
    uint32_t startSector = CDDK_FLASH_INTERN_SECTOR_ERROR;
    uint32_t endSector = CDDK_FLASH_INTERN_SECTOR_ERROR;

    /* Verify if the provided addresses are not out of bound. */
    Retcode_T retcode = checkAddressBounderies(startAddress, endAddress);

    if (RETCODE_OK == retcode)
    {
        retcode = getCorrespondingSectors(startAddress, endAddress, &startSector, &endSector);
    }

    if (RETCODE_OK == retcode)
    {
        uint32_t nbSector = endSector - startSector;

        pEraseInitStruct->Sector = startSector;
        pEraseInitStruct->NbSectors = nbSector;
        pEraseInitStruct->VoltageRange = FLASH_VOLTAGE_RANGE_3;

        if (nbSector == NB_SB_SECTOR)
        {
            pEraseInitStruct->TypeErase = FLASH_TYPEERASE_MASSERASE;
        }
        else
        {
            pEraseInitStruct->TypeErase = FLASH_TYPEERASE_SECTORS;
        }
    }

    return retcode;
}

static Retcode_T getFlashError(void)
{
    Retcode_T retcode = RETCODE_OK;

    switch (HAL_FLASH_GetError())
    {
    /* No error to report. Return RETCODE_OK. */
    case HAL_FLASH_ERROR_NONE:
        break;

    /* Programming Sequence error. */
    case FLASH_ERROR_ERS:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ERASE_ERROR);
        break;

    /* Programming Parallelism error.*/
    case HAL_FLASH_ERROR_PGP:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_PROG_PARALLELISM_ERROR);
        break;

    /* Programming Alignment error.*/
    case HAL_FLASH_ERROR_PGA:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_PROG_ALIGMENT_ERROR);
        break;

    /* Write protection error.*/
    case HAL_FLASH_ERROR_WRP:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_WRITE_PROTECTED_ERROR);
        break;

    /* Operation Error.*/
    case HAL_FLASH_ERROR_OPERATION:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_FLASH_OPERATION_ERROR);
        break;

    /* Read Protection Error.*/
    case HAL_FLASH_ERROR_RD:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_READ_PROTECTED_ERROR);
        break;

    /* This point should be reached!*/
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    return retcode;
}
uint32_t MCU_FlashIntern_GetMinRWSize(void)
{
    return FLASH_INTERN_MIN_RW_SIZE;
}

#endif /* KISO_FEATURE_FLASH_INTERN */
