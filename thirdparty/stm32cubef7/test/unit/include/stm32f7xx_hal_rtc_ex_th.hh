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
 * Mock-up implementation for the stm32f7xx_hal_rtc_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_RTC_EX_TH_HH
#define STM32F7XX_HAL_RTC_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_rtc_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_rtc_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetTimeStamp, RTC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetTimeStamp_IT, RTC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_DeactivateTimeStamp, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetInternalTimeStamp, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_DeactivateInternalTimeStamp, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_GetTimeStamp, RTC_HandleTypeDef*, RTC_TimeTypeDef*, RTC_DateTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetTamper, RTC_HandleTypeDef*, RTC_TamperTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetTamper_IT, RTC_HandleTypeDef*, RTC_TamperTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_DeactivateTamper, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_PollForTimeStampEvent, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_PollForTamper1Event, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_PollForTamper2Event, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_PollForTamper3Event, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetWakeUpTimer, RTC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetWakeUpTimer_IT, RTC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_RTCEx_DeactivateWakeUpTimer, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_RTCEx_GetWakeUpTimer, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_PollForWakeUpTimerEvent, RTC_HandleTypeDef*, uint32_t);

FAKE_VOID_FUNC(HAL_RTCEx_BKUPWrite, RTC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_RTCEx_BKUPRead, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetSmoothCalib, RTC_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetSynchroShift, RTC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetCalibrationOutPut, RTC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_DeactivateCalibrationOutPut, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_SetRefClock, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_DeactivateRefClock, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_EnableBypassShadow, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_DisableBypassShadow, RTC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RTCEx_PollForAlarmBEvent, RTC_HandleTypeDef*, uint32_t);

# endif /* STM32F7XX_HAL_RTC_EX_TH_HH */