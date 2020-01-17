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
 * Mock-up implementation for the stm32f7xx_ll_pwr module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_PWR_TH_HH
#define STM32F7XX_LL_PWR_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_PWR_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_PWR_EnableUnderDriveMode);

FAKE_VOID_FUNC(LL_PWR_DisableUnderDriveMode);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledUnderDriveMode);

FAKE_VOID_FUNC(LL_PWR_EnableOverDriveSwitching);

FAKE_VOID_FUNC(LL_PWR_DisableOverDriveSwitching);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledOverDriveSwitching);

FAKE_VOID_FUNC(LL_PWR_EnableOverDriveMode);

FAKE_VOID_FUNC(LL_PWR_DisableOverDriveMode);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledOverDriveMode);

FAKE_VOID_FUNC(LL_PWR_SetRegulVoltageScaling, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_GetRegulVoltageScaling);

FAKE_VOID_FUNC(LL_PWR_EnableMainRegulatorDeepSleepUDMode);

FAKE_VOID_FUNC(LL_PWR_DisableMainRegulatorDeepSleepUDMode);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledMainRegulatorDeepSleepUDMode);

FAKE_VOID_FUNC(LL_PWR_EnableLowPowerRegulatorDeepSleepUDMode);

FAKE_VOID_FUNC(LL_PWR_DisableLowPowerRegulatorDeepSleepUDMode);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledLowPowerRegulatorDeepSleepUDMode);

FAKE_VOID_FUNC(LL_PWR_EnableFlashPowerDown);

FAKE_VOID_FUNC(LL_PWR_DisableFlashPowerDown);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledFlashPowerDown);

FAKE_VOID_FUNC(LL_PWR_EnableBkUpAccess);

FAKE_VOID_FUNC(LL_PWR_DisableBkUpAccess);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledBkUpAccess);

FAKE_VOID_FUNC(LL_PWR_EnableBkUpRegulator);

FAKE_VOID_FUNC(LL_PWR_DisableBkUpRegulator);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledBkUpRegulator);

FAKE_VOID_FUNC(LL_PWR_SetRegulModeDS, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_GetRegulModeDS);

FAKE_VOID_FUNC(LL_PWR_SetPowerMode, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_GetPowerMode);

FAKE_VOID_FUNC(LL_PWR_SetPVDLevel, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_GetPVDLevel);

FAKE_VOID_FUNC(LL_PWR_EnablePVD);

FAKE_VOID_FUNC(LL_PWR_DisablePVD);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledPVD);

FAKE_VOID_FUNC(LL_PWR_EnableWakeUpPin, uint32_t);

FAKE_VOID_FUNC(LL_PWR_DisableWakeUpPin, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledWakeUpPin, uint32_t);

FAKE_VOID_FUNC(LL_PWR_SetWakeUpPinPolarityLow, uint32_t);

FAKE_VOID_FUNC(LL_PWR_SetWakeUpPinPolarityHigh, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsWakeUpPinPolarityLow, uint32_t);

FAKE_VOID_FUNC(LL_PWR_EnableInternalWakeUp);

FAKE_VOID_FUNC(LL_PWR_DisableInternalWakeUp);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsEnabledInternalWakeUp);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_WU6);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_WU5);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_WU4);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_WU3);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_WU2);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_WU1);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_SB);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_PVDO);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_BRR);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_VOS);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_OD);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_ODSW);

FAKE_VALUE_FUNC(uint32_t, LL_PWR_IsActiveFlag_UD);

FAKE_VOID_FUNC(LL_PWR_ClearFlag_SB);

FAKE_VOID_FUNC(LL_PWR_ClearFlag_WU6);

FAKE_VOID_FUNC(LL_PWR_ClearFlag_WU5);

FAKE_VOID_FUNC(LL_PWR_ClearFlag_WU4);

FAKE_VOID_FUNC(LL_PWR_ClearFlag_WU3);

FAKE_VOID_FUNC(LL_PWR_ClearFlag_WU2);

FAKE_VOID_FUNC(LL_PWR_ClearFlag_WU1);

FAKE_VOID_FUNC(LL_PWR_ClearFlag_UD);

FAKE_VALUE_FUNC(ErrorStatus, LL_PWR_DeInit);

# endif /* STM32F7XX_LL_PWR_TH_HH */
