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
 * Mock-up implementation for the stm32f7xx_hal_ltdc_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_LTDC_EX_TH_HH
#define STM32F7XX_HAL_LTDC_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include"stm32f7xx_hal_dsi_th.hh"
#include"stm32f7xx_hal_ltdc_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_ltdc_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if defined (STM32F769xx) || defined (STM32F779xx)

#undef HAL_LTDC_StructInitFromVideoConfig

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_StructInitFromVideoConfig, LTDC_HandleTypeDef*, DSI_VidCfgTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_StructInitFromAdaptedCommandConfig, LTDC_HandleTypeDef*, DSI_CmdCfgTypeDef*);

#endif /*STM32F769xx | STM32F779xx */

# endif /* STM32F7XX_HAL_LTDC_EX_TH_HH */
