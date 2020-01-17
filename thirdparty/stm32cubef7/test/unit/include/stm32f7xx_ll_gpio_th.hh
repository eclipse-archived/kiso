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
 * Mock-up implementation for the stm32f7xx_ll_gpio module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_GPIO_TH_HH
#define STM32F7XX_LL_GPIO_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_GPIO_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief LL GPIO Init Structure definition
  */
typedef struct
{
  uint32_t Pin;          /*!< Specifies the GPIO pins to be configured.
                              This parameter can be any value of @ref GPIO_LL_EC_PIN */

  uint32_t Mode;         /*!< Specifies the operating mode for the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_MODE.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinMode().*/

  uint32_t Speed;        /*!< Specifies the speed for the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_SPEED.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinSpeed().*/

  uint32_t OutputType;   /*!< Specifies the operating output type for the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_OUTPUT.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinOutputType().*/

  uint32_t Pull;         /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_PULL.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinPull().*/

  uint32_t Alternate;    /*!< Specifies the Peripheral to be connected to the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_AF.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetAFPin_0_7() and LL_GPIO_SetAFPin_8_15().*/
} LL_GPIO_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_GPIO_SetPinMode, GPIO_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_GetPinMode, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_SetPinOutputType, GPIO_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_GetPinOutputType, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_SetPinSpeed, GPIO_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_GetPinSpeed, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_SetPinPull, GPIO_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_GetPinPull, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_SetAFPin_0_7, GPIO_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_GetAFPin_0_7, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_SetAFPin_8_15, GPIO_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_GetAFPin_8_15, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_LockPin, GPIO_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_IsPinLocked, GPIO_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_IsAnyPinLocked, GPIO_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_ReadInputPort, GPIO_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_IsInputPinSet, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_WriteOutputPort, GPIO_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_ReadOutputPort, GPIO_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_GPIO_IsOutputPinSet, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_SetOutputPin, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_ResetOutputPin, GPIO_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_GPIO_TogglePin, GPIO_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(ErrorStatus, LL_GPIO_DeInit, GPIO_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_GPIO_Init, GPIO_TypeDef*, LL_GPIO_InitTypeDef*);

FAKE_VOID_FUNC(LL_GPIO_StructInit, LL_GPIO_InitTypeDef*);

# endif /* STM32F7XX_LL_GPIO_TH_HH */
