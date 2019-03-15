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

#ifndef STM32L4XX_HAL_RCC_TH_HH_
#define STM32L4XX_HAL_RCC_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_rcc.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VOID_FUNC(HAL_RCC_DeInit);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCC_OscConfig, RCC_OscInitTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCC_ClockConfig, RCC_ClkInitTypeDef*, uint32_t);

FAKE_VOID_FUNC(HAL_RCC_MCOConfig, uint32_t, uint32_t, uint32_t);
FAKE_VOID_FUNC(HAL_RCC_EnableCSS);
FAKE_VALUE_FUNC(uint32_t, HAL_RCC_GetSysClockFreq);
FAKE_VALUE_FUNC(uint32_t, HAL_RCC_GetHCLKFreq);
FAKE_VALUE_FUNC(uint32_t, HAL_RCC_GetPCLK1Freq);
FAKE_VALUE_FUNC(uint32_t, HAL_RCC_GetPCLK2Freq);
FAKE_VOID_FUNC(HAL_RCC_GetOscConfig, RCC_OscInitTypeDef*);
FAKE_VOID_FUNC(HAL_RCC_GetClockConfig, RCC_ClkInitTypeDef*, uint32_t*);
FAKE_VOID_FUNC(HAL_RCC_NMI_IRQHandler);

#endif /* STM32L4XX_HAL_RCC_TH_HH_ */
