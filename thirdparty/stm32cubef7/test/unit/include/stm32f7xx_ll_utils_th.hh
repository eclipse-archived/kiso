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
 * Mock-up implementation for the stm32f7xx_ll_utils module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_UTILS_TH_HH
#define STM32F7XX_LL_UTILS_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_UTILS_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief  UTILS PLL structure definition
  */
typedef struct
{
  uint32_t PLLM;   /*!< Division factor for PLL VCO input clock.
                        This parameter can be a value of @ref RCC_LL_EC_PLLM_DIV

                        This feature can be modified afterwards using unitary function
                        @ref LL_RCC_PLL_ConfigDomain_SYS(). */

  uint32_t PLLN;   /*!< Multiplication factor for PLL VCO output clock.
                        This parameter must be a number between Min_Data = 50 and Max_Data = 432

                        This feature can be modified afterwards using unitary function
                        @ref LL_RCC_PLL_ConfigDomain_SYS(). */

  uint32_t PLLP;   /*!< Division for the main system clock.
                        This parameter can be a value of @ref RCC_LL_EC_PLLP_DIV

                        This feature can be modified afterwards using unitary function
                        @ref LL_RCC_PLL_ConfigDomain_SYS(). */
} LL_UTILS_PLLInitTypeDef;

/**
  * @brief  UTILS System, AHB and APB buses clock configuration structure definition
  */
typedef struct
{
  uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                       This parameter can be a value of @ref RCC_LL_EC_SYSCLK_DIV

                                       This feature can be modified afterwards using unitary function
                                       @ref LL_RCC_SetAHBPrescaler(). */

  uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_LL_EC_APB1_DIV

                                       This feature can be modified afterwards using unitary function
                                       @ref LL_RCC_SetAPB1Prescaler(). */

  uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_LL_EC_APB2_DIV

                                       This feature can be modified afterwards using unitary function
                                       @ref LL_RCC_SetAPB2Prescaler(). */

} LL_UTILS_ClkInitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(uint32_t, LL_GetUID_Word0);

FAKE_VALUE_FUNC(uint32_t, LL_GetUID_Word1);

FAKE_VALUE_FUNC(uint32_t, LL_GetUID_Word2);

FAKE_VALUE_FUNC(uint32_t, LL_GetFlashSize);

FAKE_VALUE_FUNC(uint32_t, LL_GetPackageType);

FAKE_VOID_FUNC(LL_InitTick, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_SetSystemCoreClock, uint32_t);

FAKE_VALUE_FUNC(ErrorStatus, LL_PLL_ConfigSystemClock_HSI, LL_UTILS_PLLInitTypeDef*, LL_UTILS_ClkInitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_PLL_ConfigSystemClock_HSE, uint32_t, uint32_t, LL_UTILS_PLLInitTypeDef*, LL_UTILS_ClkInitTypeDef*);

FAKE_VOID_FUNC(LL_Init1msTick, uint32_t);

FAKE_VOID_FUNC(LL_mDelay, uint32_t);

# endif /* STM32F7XX_LL_UTILS_TH_HH */
