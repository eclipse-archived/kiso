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
 * Mock-up implementation for the stm32f7xx_ll_crc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_CRC_TH_HH
#define STM32F7XX_LL_CRC_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_CRC_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_CRC_ResetCRCCalculationUnit, CRC_TypeDef*);

FAKE_VOID_FUNC(LL_CRC_SetPolynomialSize, CRC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_CRC_GetPolynomialSize, CRC_TypeDef*);

FAKE_VOID_FUNC(LL_CRC_SetInputDataReverseMode, CRC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_CRC_GetInputDataReverseMode, CRC_TypeDef*);

FAKE_VOID_FUNC(LL_CRC_SetOutputDataReverseMode, CRC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_CRC_GetOutputDataReverseMode, CRC_TypeDef*);

FAKE_VOID_FUNC(LL_CRC_SetInitialData, CRC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_CRC_GetInitialData, CRC_TypeDef*);

FAKE_VOID_FUNC(LL_CRC_SetPolynomialCoef, CRC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_CRC_GetPolynomialCoef, CRC_TypeDef*);

FAKE_VOID_FUNC(LL_CRC_FeedData32, CRC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_CRC_FeedData16, CRC_TypeDef*, uint16_t);

FAKE_VOID_FUNC(LL_CRC_FeedData8, CRC_TypeDef*, uint8_t);

FAKE_VALUE_FUNC(uint32_t, LL_CRC_ReadData32, CRC_TypeDef*);

FAKE_VALUE_FUNC(uint16_t, LL_CRC_ReadData16, CRC_TypeDef*);

FAKE_VALUE_FUNC(uint8_t, LL_CRC_ReadData8, CRC_TypeDef*);

FAKE_VALUE_FUNC(uint8_t, LL_CRC_ReadData7, CRC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_CRC_Read_IDR, CRC_TypeDef*);

FAKE_VOID_FUNC(LL_CRC_Write_IDR, CRC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(ErrorStatus, LL_CRC_DeInit, CRC_TypeDef*);

# endif /* STM32F7XX_LL_CRC_TH_HH */
