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
 * Mock-up implementation for the stm32f7xx_hal_rtc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_RTC_TH_HH
#define STM32F7XX_HAL_RTC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_rtc.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_Init, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_DeInit, RTC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_RTC_MspInit, RTC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_RTC_MspDeInit, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_SetTime, RTC_HandleTypeDef*, RTC_TimeTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_GetTime, RTC_HandleTypeDef*, RTC_TimeTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_SetDate, RTC_HandleTypeDef*, RTC_DateTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_GetDate, RTC_HandleTypeDef*, RTC_DateTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_SetAlarm, RTC_HandleTypeDef*, RTC_AlarmTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_SetAlarm_IT, RTC_HandleTypeDef*, RTC_AlarmTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_DeactivateAlarm, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_GetAlarm, RTC_HandleTypeDef*, RTC_AlarmTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_PollForAlarmAEvent, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTC_WaitForSynchro, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_RTCStateTypeDef, HAL_RTC_GetState, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, RTC_EnterInitMode, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint8_t, RTC_ByteToBcd2, uint8_t);

FAKE_VALUE_FUNC(uint8_t, RTC_Bcd2ToByte, uint8_t);

# endif /* STM32F7XX_HAL_RTC_TH_HH */