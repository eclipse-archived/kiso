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
 * Mock-up implementation for the stm32f7xx_ll_rcc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_RCC_TH_HH
#define STM32F7XX_LL_RCC_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_RCC_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief  RCC Clocks Frequency Structure
  */
typedef struct
{
  uint32_t SYSCLK_Frequency;        /*!< SYSCLK clock frequency */
  uint32_t HCLK_Frequency;          /*!< HCLK clock frequency */
  uint32_t PCLK1_Frequency;         /*!< PCLK1 clock frequency */
  uint32_t PCLK2_Frequency;         /*!< PCLK2 clock frequency */
} LL_RCC_ClocksTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_RCC_HSE_EnableCSS);

FAKE_VOID_FUNC(LL_RCC_HSE_EnableBypass);

FAKE_VOID_FUNC(LL_RCC_HSE_DisableBypass);

FAKE_VOID_FUNC(LL_RCC_HSE_Enable);

FAKE_VOID_FUNC(LL_RCC_HSE_Disable);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_HSE_IsReady);

FAKE_VOID_FUNC(LL_RCC_HSI_Enable);

FAKE_VOID_FUNC(LL_RCC_HSI_Disable);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_HSI_IsReady);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_HSI_GetCalibration);

FAKE_VOID_FUNC(LL_RCC_HSI_SetCalibTrimming, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_HSI_GetCalibTrimming);

FAKE_VOID_FUNC(LL_RCC_LSE_Enable);

FAKE_VOID_FUNC(LL_RCC_LSE_Disable);

FAKE_VOID_FUNC(LL_RCC_LSE_EnableBypass);

FAKE_VOID_FUNC(LL_RCC_LSE_DisableBypass);

FAKE_VOID_FUNC(LL_RCC_LSE_SetDriveCapability, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_LSE_GetDriveCapability);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_LSE_IsReady);

FAKE_VOID_FUNC(LL_RCC_LSI_Enable);

FAKE_VOID_FUNC(LL_RCC_LSI_Disable);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_LSI_IsReady);

FAKE_VOID_FUNC(LL_RCC_SetSysClkSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetSysClkSource);

FAKE_VOID_FUNC(LL_RCC_SetAHBPrescaler, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetAPB1Prescaler, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetAPB2Prescaler, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetAHBPrescaler);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetAPB1Prescaler);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetAPB2Prescaler);

FAKE_VOID_FUNC(LL_RCC_ConfigMCO, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetUSARTClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetUARTClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetI2CClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetLPTIMClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetSAIClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetSDMMCClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetCK48MClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetRNGClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetUSBClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetCECClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetI2SClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetDSIClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetDFSDMAudioClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetDFSDMClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetUSARTClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetUARTClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetI2CClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetLPTIMClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetSAIClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetSDMMCClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetCK48MClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetRNGClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetUSBClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetCECClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetI2SClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetDFSDMAudioClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetDFSDMClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetDSIClockSource, uint32_t);

FAKE_VOID_FUNC(LL_RCC_SetRTCClockSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetRTCClockSource);

FAKE_VOID_FUNC(LL_RCC_EnableRTC);

FAKE_VOID_FUNC(LL_RCC_DisableRTC);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsEnabledRTC);

FAKE_VOID_FUNC(LL_RCC_ForceBackupDomainReset);

FAKE_VOID_FUNC(LL_RCC_ReleaseBackupDomainReset);

FAKE_VOID_FUNC(LL_RCC_SetRTC_HSEPrescaler, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetRTC_HSEPrescaler);

FAKE_VOID_FUNC(LL_RCC_SetTIMPrescaler, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetTIMPrescaler);

FAKE_VOID_FUNC(LL_RCC_PLL_Enable);

FAKE_VOID_FUNC(LL_RCC_PLL_Disable);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_IsReady);

FAKE_VOID_FUNC(LL_RCC_PLL_ConfigDomain_SYS, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RCC_PLL_ConfigDomain_48M, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RCC_PLL_ConfigDomain_DSI, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RCC_PLL_SetMainSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetMainSource);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetN);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetP);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetQ);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetR);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetDivider);

FAKE_VOID_FUNC(LL_RCC_PLL_ConfigSpreadSpectrum, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetPeriodModulation);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetStepIncrementation);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLL_GetSpreadSelection);

FAKE_VOID_FUNC(LL_RCC_PLL_SpreadSpectrum_Enable);

FAKE_VOID_FUNC(LL_RCC_PLL_SpreadSpectrum_Disable);

FAKE_VOID_FUNC(LL_RCC_PLLI2S_Enable);

FAKE_VOID_FUNC(LL_RCC_PLLI2S_Disable);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLI2S_IsReady);

FAKE_VOID_FUNC(LL_RCC_PLLI2S_ConfigDomain_SAI, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RCC_PLLI2S_ConfigDomain_SPDIFRX, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RCC_PLLI2S_ConfigDomain_I2S, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLI2S_GetN);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLI2S_GetQ);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLI2S_GetR);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLI2S_GetP);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLI2S_GetDIVQ);

FAKE_VOID_FUNC(LL_RCC_PLLSAI_Enable);

FAKE_VOID_FUNC(LL_RCC_PLLSAI_Disable);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLSAI_IsReady);

FAKE_VOID_FUNC(LL_RCC_PLLSAI_ConfigDomain_SAI, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RCC_PLLSAI_ConfigDomain_48M, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_RCC_PLLSAI_ConfigDomain_LTDC, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLSAI_GetN);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLSAI_GetQ);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLSAI_GetR);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLSAI_GetP);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLSAI_GetDIVQ);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_PLLSAI_GetDIVR);

FAKE_VOID_FUNC(LL_RCC_ClearFlag_LSIRDY);

FAKE_VOID_FUNC(LL_RCC_ClearFlag_LSERDY);

FAKE_VOID_FUNC(LL_RCC_ClearFlag_HSIRDY);

FAKE_VOID_FUNC(LL_RCC_ClearFlag_HSERDY);

FAKE_VOID_FUNC(LL_RCC_ClearFlag_PLLRDY);

FAKE_VOID_FUNC(LL_RCC_ClearFlag_PLLI2SRDY);

FAKE_VOID_FUNC(LL_RCC_ClearFlag_PLLSAIRDY);

FAKE_VOID_FUNC(LL_RCC_ClearFlag_HSECSS);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_LSIRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_LSERDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_HSIRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_HSERDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_PLLRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_PLLI2SRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_PLLSAIRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_HSECSS);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_IWDGRST);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_LPWRRST);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_PINRST);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_PORRST);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_SFTRST);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_WWDGRST);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsActiveFlag_BORRST);

FAKE_VOID_FUNC(LL_RCC_ClearResetFlags);

FAKE_VOID_FUNC(LL_RCC_EnableIT_LSIRDY);

FAKE_VOID_FUNC(LL_RCC_EnableIT_LSERDY);

FAKE_VOID_FUNC(LL_RCC_EnableIT_HSIRDY);

FAKE_VOID_FUNC(LL_RCC_EnableIT_HSERDY);

FAKE_VOID_FUNC(LL_RCC_EnableIT_PLLRDY);

FAKE_VOID_FUNC(LL_RCC_EnableIT_PLLI2SRDY);

FAKE_VOID_FUNC(LL_RCC_EnableIT_PLLSAIRDY);

FAKE_VOID_FUNC(LL_RCC_DisableIT_LSIRDY);

FAKE_VOID_FUNC(LL_RCC_DisableIT_LSERDY);

FAKE_VOID_FUNC(LL_RCC_DisableIT_HSIRDY);

FAKE_VOID_FUNC(LL_RCC_DisableIT_HSERDY);

FAKE_VOID_FUNC(LL_RCC_DisableIT_PLLRDY);

FAKE_VOID_FUNC(LL_RCC_DisableIT_PLLI2SRDY);

FAKE_VOID_FUNC(LL_RCC_DisableIT_PLLSAIRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsEnabledIT_LSIRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsEnabledIT_LSERDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsEnabledIT_HSIRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsEnabledIT_HSERDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsEnabledIT_PLLRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsEnabledIT_PLLI2SRDY);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_IsEnabledIT_PLLSAIRDY);

FAKE_VALUE_FUNC(ErrorStatus, LL_RCC_DeInit);

FAKE_VOID_FUNC(LL_RCC_GetSystemClocksFreq, LL_RCC_ClocksTypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetUSARTClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetUARTClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetI2CClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetLPTIMClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetSAIClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetSDMMCClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetRNGClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetUSBClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetDFSDMClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetDFSDMAudioClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetI2SClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetCECClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetLTDCClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetSPDIFRXClockFreq, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_RCC_GetDSIClockFreq, uint32_t);

# endif /* STM32F7XX_LL_RCC_TH_HH */
