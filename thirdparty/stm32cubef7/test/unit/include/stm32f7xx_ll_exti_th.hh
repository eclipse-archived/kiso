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
 * Mock-up implementation for the stm32f7xx_ll_exti module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_EXTI_TH_HH
#define STM32F7XX_LL_EXTI_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_EXTI_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/** @defgroup EXTI_LL_ES_INIT EXTI Exported Init structure
  * @{
  */
typedef struct
{

  uint32_t Line_0_31;           /*!< Specifies the EXTI lines to be enabled or disabled for Lines in range 0 to 31
                                     This parameter can be any combination of @ref EXTI_LL_EC_LINE */

  FunctionalState LineCommand;  /*!< Specifies the new state of the selected EXTI lines.
                                     This parameter can be set either to ENABLE or DISABLE */

  uint8_t Mode;                 /*!< Specifies the mode for the EXTI lines.
                                     This parameter can be a value of @ref EXTI_LL_EC_MODE. */

  uint8_t Trigger;              /*!< Specifies the trigger signal active edge for the EXTI lines.
                                     This parameter can be a value of @ref EXTI_LL_EC_TRIGGER. */
} LL_EXTI_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_EXTI_EnableIT_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_DisableIT_0_31, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_EXTI_IsEnabledIT_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_EnableEvent_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_DisableEvent_0_31, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_EXTI_IsEnabledEvent_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_EnableRisingTrig_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_DisableRisingTrig_0_31, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_EXTI_IsEnabledRisingTrig_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_EnableFallingTrig_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_DisableFallingTrig_0_31, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_EXTI_IsEnabledFallingTrig_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_GenerateSWI_0_31, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_EXTI_IsActiveFlag_0_31, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_EXTI_ReadFlag_0_31, uint32_t);

FAKE_VOID_FUNC(LL_EXTI_ClearFlag_0_31, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_EXTI_Init, LL_EXTI_InitTypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_EXTI_DeInit);

FAKE_VOID_FUNC(LL_EXTI_StructInit, LL_EXTI_InitTypeDef*);

# endif /* STM32F7XX_LL_EXTI_TH_HH */
