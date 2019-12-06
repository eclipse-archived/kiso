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
 * Mock-up implementation for the stm32f7xx_ll_rtc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_RTC_TH_HH
#define STM32F7XX_LL_RTC_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_RTC_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief  RTC Init structures definition
  */
typedef struct
{
  uint32_t HourFormat;   /*!< Specifies the RTC Hours Format.
                              This parameter can be a value of @ref RTC_LL_EC_HOURFORMAT

                              This feature can be modified afterwards using unitary function
                              @ref LL_RTC_SetHourFormat(). */

  uint32_t AsynchPrescaler; /*!< Specifies the RTC Asynchronous Predivider value.
                              This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7F

                              This feature can be modified afterwards using unitary function
                              @ref LL_RTC_SetAsynchPrescaler(). */

  uint32_t SynchPrescaler;  /*!< Specifies the RTC Synchronous Predivider value.
                              This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7FFF

                              This feature can be modified afterwards using unitary function
                              @ref LL_RTC_SetSynchPrescaler(). */
} LL_RTC_InitTypeDef;

/**
  * @brief  RTC Time structure definition
  */
typedef struct
{
  uint32_t TimeFormat; /*!< Specifies the RTC AM/PM Time.
                            This parameter can be a value of @ref RTC_LL_EC_TIME_FORMAT

                            This feature can be modified afterwards using unitary function @ref LL_RTC_TIME_SetFormat(). */

  uint8_t Hours;       /*!< Specifies the RTC Time Hours.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the @ref LL_RTC_TIME_FORMAT_PM is selected.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the @ref LL_RTC_TIME_FORMAT_AM_OR_24 is selected.

                            This feature can be modified afterwards using unitary function @ref LL_RTC_TIME_SetHour(). */

  uint8_t Minutes;     /*!< Specifies the RTC Time Minutes.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 59

                            This feature can be modified afterwards using unitary function @ref LL_RTC_TIME_SetMinute(). */

  uint8_t Seconds;     /*!< Specifies the RTC Time Seconds.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 59

                            This feature can be modified afterwards using unitary function @ref LL_RTC_TIME_SetSecond(). */
} LL_RTC_TimeTypeDef;

/**
  * @brief  RTC Date structure definition
  */
typedef struct
{
  uint8_t WeekDay;  /*!< Specifies the RTC Date WeekDay.
                         This parameter can be a value of @ref RTC_LL_EC_WEEKDAY

                         This feature can be modified afterwards using unitary function @ref LL_RTC_DATE_SetWeekDay(). */

  uint8_t Month;    /*!< Specifies the RTC Date Month.
                         This parameter can be a value of @ref RTC_LL_EC_MONTH

                         This feature can be modified afterwards using unitary function @ref LL_RTC_DATE_SetMonth(). */

  uint8_t Day;      /*!< Specifies the RTC Date Day.
                         This parameter must be a number between Min_Data = 1 and Max_Data = 31

                         This feature can be modified afterwards using unitary function @ref LL_RTC_DATE_SetDay(). */

  uint8_t Year;     /*!< Specifies the RTC Date Year.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 99

                         This feature can be modified afterwards using unitary function @ref LL_RTC_DATE_SetYear(). */
} LL_RTC_DateTypeDef;

/**
  * @brief  RTC Alarm structure definition
  */
typedef struct
{
  LL_RTC_TimeTypeDef AlarmTime;  /*!< Specifies the RTC Alarm Time members. */

  uint32_t AlarmMask;            /*!< Specifies the RTC Alarm Masks.
                                      This parameter can be a value of @ref RTC_LL_EC_ALMA_MASK for ALARM A or @ref RTC_LL_EC_ALMB_MASK for ALARM B.

                                      This feature can be modified afterwards using unitary function @ref LL_RTC_ALMA_SetMask() for ALARM A
                                      or @ref LL_RTC_ALMB_SetMask() for ALARM B
                                 */

  uint32_t AlarmDateWeekDaySel;  /*!< Specifies the RTC Alarm is on day or WeekDay.
                                      This parameter can be a value of @ref RTC_LL_EC_ALMA_WEEKDAY_SELECTION for ALARM A or @ref RTC_LL_EC_ALMB_WEEKDAY_SELECTION for ALARM B

                                      This feature can be modified afterwards using unitary function @ref LL_RTC_ALMA_EnableWeekday() or @ref LL_RTC_ALMA_DisableWeekday()
                                      for ALARM A or @ref LL_RTC_ALMB_EnableWeekday() or @ref LL_RTC_ALMB_DisableWeekday() for ALARM B
                                 */

  uint8_t AlarmDateWeekDay;      /*!< Specifies the RTC Alarm Day/WeekDay.
                                      If AlarmDateWeekDaySel set to day, this parameter  must be a number between Min_Data = 1 and Max_Data = 31.

                                      This feature can be modified afterwards using unitary function @ref LL_RTC_ALMA_SetDay()
                                      for ALARM A or @ref LL_RTC_ALMB_SetDay() for ALARM B.

                                      If AlarmDateWeekDaySel set to Weekday, this parameter can be a value of @ref RTC_LL_EC_WEEKDAY.

                                      This feature can be modified afterwards using unitary function @ref LL_RTC_ALMA_SetWeekDay()
                                      for ALARM A or @ref LL_RTC_ALMB_SetWeekDay() for ALARM B.
                                 */
} LL_RTC_AlarmTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_RTC_SetHourFormat, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_GetHourFormat, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_SetAlarmOutEvent, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_GetAlarmOutEvent, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_SetAlarmOutputType, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_GetAlarmOutputType, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableInitMode, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableInitMode, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_SetOutputPolarity, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_GetOutputPolarity, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableShadowRegBypass, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableShadowRegBypass, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsShadowRegBypassEnabled, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableRefClock, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableRefClock, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_SetAsynchPrescaler, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_SetSynchPrescaler, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_GetAsynchPrescaler, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_GetSynchPrescaler, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableWriteProtection, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableWriteProtection, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_SetFormat, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TIME_GetFormat, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_SetHour, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TIME_GetHour, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_SetMinute, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TIME_GetMinute, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_SetSecond, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TIME_GetSecond, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_Config, RTC_TypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TIME_Get, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_EnableDayLightStore, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_DisableDayLightStore, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TIME_IsDayLightStoreEnabled, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_DecHour, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_IncHour, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TIME_GetSubSecond, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_Synchronize, RTC_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RTC_DATE_SetYear, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_DATE_GetYear, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DATE_SetWeekDay, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_DATE_GetWeekDay, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DATE_SetMonth, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_DATE_GetMonth, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DATE_SetDay, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_DATE_GetDay, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DATE_Config, RTC_TypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_DATE_Get, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_Enable, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_Disable, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetMask, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetMask, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_EnableWeekday, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_DisableWeekday, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetDay, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetDay, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetWeekDay, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetWeekDay, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetTimeFormat, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetTimeFormat, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetHour, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetHour, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetMinute, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetMinute, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetSecond, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetSecond, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_ConfigTime, RTC_TypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetTime, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetSubSecondMask, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetSubSecondMask, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_SetSubSecond, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMA_GetSubSecond, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_Enable, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_Disable, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetMask, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetMask, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_EnableWeekday, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_DisableWeekday, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetDay, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetDay, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetWeekDay, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetWeekDay, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetTimeFormat, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetTimeFormat, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetHour, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetHour, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetMinute, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetMinute, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetSecond, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetSecond, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_ConfigTime, RTC_TypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetTime, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetSubSecondMask, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetSubSecondMask, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_SetSubSecond, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_ALMB_GetSubSecond, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TS_EnableInternalEvent, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TS_DisableInternalEvent, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TS_Enable, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TS_Disable, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TS_SetActiveEdge, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetActiveEdge, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetTimeFormat, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetHour, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetMinute, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetSecond, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetTime, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetWeekDay, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetMonth, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetDay, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetDate, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TS_GetSubSecond, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TS_EnableOnTamper, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TS_DisableOnTamper, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TAMPER_Enable, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_TAMPER_Disable, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_TAMPER_EnableMask, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_TAMPER_DisableMask, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_TAMPER_EnableEraseBKP, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_TAMPER_DisableEraseBKP, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_TAMPER_DisablePullUp, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TAMPER_EnablePullUp, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TAMPER_SetPrecharge, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TAMPER_GetPrecharge, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TAMPER_SetFilterCount, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TAMPER_GetFilterCount, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TAMPER_SetSamplingFreq, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_TAMPER_GetSamplingFreq, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_TAMPER_EnableActiveLevel, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_TAMPER_DisableActiveLevel, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_WAKEUP_Enable, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_WAKEUP_Disable, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_WAKEUP_IsEnabled, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_WAKEUP_SetClock, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_WAKEUP_GetClock, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_WAKEUP_SetAutoReload, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_WAKEUP_GetAutoReload, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_BAK_SetRegister, RTC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_BAK_GetRegister, RTC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_RTC_CAL_SetOutputFreq, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_CAL_GetOutputFreq, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_CAL_SetPulse, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_CAL_IsPulseInserted, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_CAL_SetPeriod, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_CAL_GetPeriod, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_CAL_SetMinus, RTC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_CAL_GetMinus, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_ITS, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_RECALP, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_TAMP3, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_TAMP2, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_TAMP1, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_TSOV, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_TS, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_WUT, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_ALRB, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_ALRA, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_ITS, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_TAMP3, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_TAMP2, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_TAMP1, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_TSOV, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_TS, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_WUT, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_ALRB, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_ALRA, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_INIT, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_RS, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_ClearFlag_RS, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_INITS, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_SHP, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_WUTW, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_ALRBW, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsActiveFlag_ALRAW, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableIT_TS, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableIT_TS, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableIT_WUT, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableIT_WUT, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableIT_ALRB, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableIT_ALRB, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableIT_ALRA, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableIT_ALRA, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableIT_TAMP3, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableIT_TAMP3, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableIT_TAMP2, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableIT_TAMP2, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableIT_TAMP1, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableIT_TAMP1, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_EnableIT_TAMP, RTC_TypeDef*);

FAKE_VOID_FUNC(LL_RTC_DisableIT_TAMP, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsEnabledIT_TS, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsEnabledIT_WUT, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsEnabledIT_ALRB, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsEnabledIT_ALRA, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsEnabledIT_TAMP3, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsEnabledIT_TAMP2, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsEnabledIT_TAMP1, RTC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RTC_IsEnabledIT_TAMP, RTC_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_DeInit, RTC_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_Init, RTC_TypeDef*, LL_RTC_InitTypeDef*);

FAKE_VOID_FUNC(LL_RTC_StructInit, LL_RTC_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_TIME_Init, RTC_TypeDef*, uint32_t, LL_RTC_TimeTypeDef*);

FAKE_VOID_FUNC(LL_RTC_TIME_StructInit, LL_RTC_TimeTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_DATE_Init, RTC_TypeDef*, uint32_t, LL_RTC_DateTypeDef*);

FAKE_VOID_FUNC(LL_RTC_DATE_StructInit, LL_RTC_DateTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_ALMA_Init, RTC_TypeDef*, uint32_t, LL_RTC_AlarmTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_ALMB_Init, RTC_TypeDef*, uint32_t, LL_RTC_AlarmTypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMA_StructInit, LL_RTC_AlarmTypeDef*);

FAKE_VOID_FUNC(LL_RTC_ALMB_StructInit, LL_RTC_AlarmTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_EnterInitMode, RTC_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_ExitInitMode, RTC_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RTC_WaitForSynchro, RTC_TypeDef*);

# endif /* STM32F7XX_LL_RTC_TH_HH */
