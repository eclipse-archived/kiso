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
 * @brief Contains an implementation of a non-volatile memory interface for STM32l4.
 */

#include "Kiso_MCU_FlashIntern.h"

#if KISO_FEATURE_FLASH_INTERN

#include "Kiso_Retcode.h"

#include "stm32l4xx.h"

#include "stm32l4xx_hal.h"

#include "stm32l4xx_hal_flash.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_FLASH_INTERN

/**
 *@details Macro to configure the minimum read/write-able size of FlashIntern.
 *         Read/write addresses need to be aligned to this size boundaries.
 *         This is a result of the use of FLASH_TYPEPROGRAM_DOUBLEWORD in the
 *         stm32 HAL.
 */
#define FLASH_INTERN_MIN_RW_SIZE sizeof(uint64_t)

/* Put private function declarations here */

/** @brief Private function to verify if given start and end length are aligned to the uint64 size.
 *
 * @details It verify if the start address and the length are a multiple of FLASH_INTERN_MIN_RW_SIZE
 *
 * @param [in] startAddress : desired internal flash memory's start address.
 * @param [in] length : desired length
 *
 * @retval RETCODE_OK if the provided address are within the internal flash memory's boundaries.
 * @retval RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND if the provided address are out of the internal flash memory's boundaries.
 */
static Retcode_T checkParamAlignment(uint32_t startAddress, uint32_t length)
{
    Retcode_T retcode = RETCODE_OK;

    /* Check length is a multiple of FLASH_INTERN_MIN_RW_SIZE  */
    if (UINT32_C(0) != (length % FLASH_INTERN_MIN_RW_SIZE))
    {
        retcode = (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM));
    }

    /* Check startAddress is aligned to FLASH_INTERN_MIN_RW_SIZE Bytes */
    if (UINT32_C(0) != (startAddress % FLASH_INTERN_MIN_RW_SIZE))
    {
        retcode = (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_INVALID_ADDRESS));
    }

    return retcode;
}

/** @brief Private function to verify if given start and end address are in the non-volatile memory's boundaries.
 *
 * @details It verify if the start address is not smaller than the internal flash memory's base address. Verify as well if the end
 * address is not greater than the internal flash memory's end address or equal/smaller to the start address.
 *
 * @param [in] startAddress : desired internal flash memory's start address.
 * @param [in] endAddress : desired internal flash memory's end address.
 *
 * @retval RETCODE_OK if the provided address are within the internal flash memory's boundaries.
 * @retval RETCODE_MCU_FLASH_INTERN_INVALID_ADDRESS if the provided address is not aligned and
 * RETCODE_INVALID_PARAM if the provided length is not aligned.
 */
static Retcode_T checkAddressBounderies(uint32_t startAddress, uint32_t endAddress)
{
    Retcode_T retcode = RETCODE_OK;

    if ((FLASH_BASE > startAddress) || (startAddress >= endAddress) ||
        (endAddress > FLASH_BASE + FLASH_SIZE))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ADDRESS_OUT_OF_BOUND);
    }

    return retcode;
}

static uint32_t GetBank(uint32_t Address)
{
    if ((FLASH_BASE + FLASH_BANK_SIZE) > Address)
    {
        return (FLASH_BANK_1);
    }
    else
    {
        return (FLASH_BANK_2);
    }
}

static uint32_t GetPage(uint32_t Address)
{
    if (FLASH_BANK_1 == GetBank(Address))
    {
        return ((Address - FLASH_BASE) / FLASH_PAGE_SIZE);
    }
    else
    {
        return ((Address - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE);
    }
}

static void ClearFlashFlag(uint32_t FlashFlagToClear)
{
    __HAL_FLASH_CLEAR_FLAG(FlashFlagToClear);
}

/* Put function implementations here */
/* API documentation is in the interface header. */
Retcode_T MCU_FlashIntern_Initialize(MCU_FlashIntern_T flashInternInitStruct)
{
    KISO_UNUSED(flashInternInitStruct);

    return RETCODE_OK;
}

/* API documentation is in the interface header. */
Retcode_T MCU_FlashIntern_Erase(uint32_t startAddress, uint32_t endAddress)
{
    Retcode_T Ret;

    uint32_t RegistryPageStart = 0;
    uint32_t RegistryBankStart = 0;
    uint32_t RegistryPageEnd = 0;
    uint32_t RegistryBankEnd = 0;
    uint32_t PageError = 0;

    FLASH_EraseInitTypeDef EraseInitStruct;

    /* Check Alignment */
    /* No need to check the length here, therefore passing 0 */
    Ret = checkParamAlignment(startAddress, 0);
    if (RETCODE_OK != Ret)
        return Ret;
    /* Check bounds */
    Ret = checkAddressBounderies(startAddress, endAddress);
    if (RETCODE_OK != Ret)
        return Ret;

    /* Unlock flash */
    if (HAL_OK != HAL_FLASH_Unlock())
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED));
    }

    /* Clear error flag */
    ClearFlashFlag(FLASH_FLAG_ALL_ERRORS);

    RegistryPageStart = GetPage(startAddress);
    RegistryBankStart = GetBank(startAddress);

    RegistryPageEnd = GetPage(endAddress - 1);
    RegistryBankEnd = GetBank(endAddress - 1);

    /* If area is on the 2 banks */
    if (RegistryBankEnd != RegistryBankStart)
    {
        /* We first erase bank 1 */
        EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
        EraseInitStruct.Banks = FLASH_BANK_1;
        EraseInitStruct.Page = RegistryPageStart;
        EraseInitStruct.NbPages = GetPage(FLASH_BASE + FLASH_BANK_SIZE - 1) - RegistryPageStart + 1;

        if (HAL_OK != HAL_FLASHEx_Erase(&EraseInitStruct, &PageError))
        {
            KISO_UNUSED(PageError);

            Ret = (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ERASE_ERROR));
        }
        else
        {
            /* Then we erase bank 2 */
            EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
            EraseInitStruct.Banks = FLASH_BANK_2;
            EraseInitStruct.Page = GetPage(FLASH_BASE + FLASH_BANK_SIZE);
            EraseInitStruct.NbPages = RegistryPageEnd - GetPage(FLASH_BASE + FLASH_BANK_SIZE) + 1;

            if (HAL_OK != HAL_FLASHEx_Erase(&EraseInitStruct, &PageError))
            {
                KISO_UNUSED(PageError);

                Ret = (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ERASE_ERROR));
            }
            else
            {
                Ret = RETCODE_OK;
            }
        }
    }
    else
    {
        /* Only one bank to erase */
        EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
        EraseInitStruct.Banks = RegistryBankStart;
        EraseInitStruct.Page = RegistryPageStart;
        EraseInitStruct.NbPages = RegistryPageEnd - RegistryPageStart + 1;

        if (HAL_OK != HAL_FLASHEx_Erase(&EraseInitStruct, &PageError))
        {
            KISO_UNUSED(PageError);

            Ret = (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_ERASE_ERROR));
        }
        else
        {
            Ret = RETCODE_OK;
        }
    }

    /* Lock the flash again */
    if (HAL_OK != HAL_FLASH_Lock())
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_LOCK_FAILED));
    }

    return Ret;
}

/* API documentation is in the interface header. */
Retcode_T MCU_FlashIntern_Write(uint32_t startAddress, uint8_t *buffer, uint32_t length)
{
    Retcode_T Ret;

    uint64_t *DoubleWordBuffer = (uint64_t *)buffer;
    uint32_t DoubleWordLength = length / FLASH_INTERN_MIN_RW_SIZE;
    uint32_t endAddress = startAddress + (DoubleWordLength * FLASH_INTERN_MIN_RW_SIZE);

    /* Check Alignment */
    Ret = checkParamAlignment(startAddress, length);
    if (RETCODE_OK != Ret)
        return Ret;
    /* Check bounds */
    Ret = checkAddressBounderies(startAddress, endAddress);
    if (RETCODE_OK != Ret)
        return Ret;

    /* Check buffer not null */
    if (NULL == DoubleWordBuffer)
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER));
    }

    /* Unlock flash */
    if (HAL_OK != HAL_FLASH_Unlock())
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_UNLOCK_FAILED));
    }

    /* Clear error flag */
    ClearFlashFlag(FLASH_FLAG_ALL_ERRORS);

    for (uint32_t i = 0; i < DoubleWordLength; i++)
    {
        if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, startAddress + (FLASH_INTERN_MIN_RW_SIZE * i), DoubleWordBuffer[i]))
        {
            Ret = (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_PROG_ERROR));

            break;
        }
    }

    /* Lock the flash again */
    if (HAL_OK != HAL_FLASH_Lock())
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_FLASH_INTERN_LOCK_FAILED));
    }

    return Ret;
}

/* API documentation is in the interface header. */
Retcode_T MCU_FlashIntern_Read(uint32_t startAddress, uint8_t *buffer, uint32_t length)
{
    Retcode_T Ret;
    uint64_t *FlashArray;

    uint64_t *DoubleWordBuffer = (uint64_t *)buffer;
    uint32_t DoubleWordLength = length / FLASH_INTERN_MIN_RW_SIZE;
    uint32_t endAddress = startAddress + (DoubleWordLength * FLASH_INTERN_MIN_RW_SIZE);

    /* Check Alignment */
    Ret = checkParamAlignment(startAddress, length);
    if (RETCODE_OK != Ret)
        return Ret;
    /* Check bounds */
    Ret = checkAddressBounderies(startAddress, endAddress);
    if (RETCODE_OK != Ret)
        return Ret;

    /* Check buffer not null */
    if (NULL == DoubleWordBuffer)
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER));
    }

    /* Copy memory into buffer */
    FlashArray = (uint64_t *)startAddress;

    for (uint32_t i = 0; i < DoubleWordLength; i++)
    {
        DoubleWordBuffer[i] = FlashArray[i];
    }

    return RETCODE_OK;
}

uint32_t MCU_FlashIntern_GetMinRWSize(void)
{
    return FLASH_INTERN_MIN_RW_SIZE;
}
#endif /* KISO_FEATURE_FLASH_INTERN */
