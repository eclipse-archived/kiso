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
 *
 * @file
 *
 * Mock-up implementation for the stm32f7xx_hal_nand module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_NAND_TH_HH
#define STM32F7XX_HAL_NAND_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_nand.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Init, NAND_HandleTypeDef*, FMC_NAND_PCC_TimingTypeDef*, FMC_NAND_PCC_TimingTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_DeInit, NAND_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_ConfigDevice, NAND_HandleTypeDef*, NAND_DeviceConfigTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Read_ID, NAND_HandleTypeDef*, NAND_IDTypeDef*);

FAKE_VOID_FUNC(HAL_NAND_MspInit, NAND_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_NAND_MspDeInit, NAND_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Reset, NAND_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Read_Page_8b, NAND_HandleTypeDef*, NAND_AddressTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Write_Page_8b, NAND_HandleTypeDef*, NAND_AddressTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Read_SpareArea_8b, NAND_HandleTypeDef*, NAND_AddressTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Write_SpareArea_8b, NAND_HandleTypeDef*, NAND_AddressTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Read_Page_16b, NAND_HandleTypeDef*, NAND_AddressTypeDef*, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Write_Page_16b, NAND_HandleTypeDef*, NAND_AddressTypeDef*, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Read_SpareArea_16b, NAND_HandleTypeDef*, NAND_AddressTypeDef*, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Write_SpareArea_16b, NAND_HandleTypeDef*, NAND_AddressTypeDef*, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_Erase_Block, NAND_HandleTypeDef*, NAND_AddressTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_NAND_Address_Inc, NAND_HandleTypeDef*, NAND_AddressTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_ECC_Enable, NAND_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_ECC_Disable, NAND_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NAND_GetECC, NAND_HandleTypeDef*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_NAND_StateTypeDef, HAL_NAND_GetState, NAND_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_NAND_Read_Status, NAND_HandleTypeDef*);

# endif /* STM32F7XX_HAL_NAND_TH_HH */