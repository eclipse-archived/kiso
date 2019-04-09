/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

/**
 * @ingroup BSP_BOARD
 * @{
 * @file
 * @brief  Implementation of board BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_SYSTEMCLOCK

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_Essentials.h"

#if BCDS_FEATURE_HAL_TICK_HANDLER

#include "BoardSettings.h"
#include "SystemClock.h"

BSP_Systick_Callback_T BSP_SysTickOsPreProcessing;
BSP_Systick_Callback_T BSP_SysTickOsPostProcessing;

Retcode_T SystemClock_Config(void)
{
    Retcode_T retcode = RETCODE_OK;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE
            | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 8;
    RCC_OscInitStruct.PLL.PLLP = 7;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    if (HAL_OK != HAL_RCC_OscConfig(&RCC_OscInitStruct))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    if (RETCODE_OK == retcode)
    {

        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
        if (HAL_OK != HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == retcode)
    {
        PeriphClkInit.PeriphClockSelection = BT_UART_CLOCK_SELECT |
        GSM_UART_CLOCK_SELECT |
        TEST_UART_CLOCK_SELECT |
        GNSS_UART_CLOCK_SELECT |
        RCC_PERIPHCLK_I2C1 |
        RCC_PERIPHCLK_ADC |
        RCC_PERIPHCLK_RNG;
        PeriphClkInit.Usart1ClockSelection = BT_UART_CLOCK_SOURCE_STOP1;
        PeriphClkInit.Usart2ClockSelection = GSM_UART_CLOCK_SOURCE;
        PeriphClkInit.Usart3ClockSelection = TEST_UART_CLOCK_SOURCE;
        PeriphClkInit.Lpuart1ClockSelection = GNSS_UART_CLOCK_SOURCE;
        PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
        PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
        PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
        if (HAL_OK != HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        /** @todo 2016-11-29, BCDS/ENG1: Clarify whether it is safe to enable CSS when power management is in place */
        HAL_RCC_EnableCSS();
        __PWR_CLK_ENABLE();
    }

    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (0 != HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
        else
        {
            HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

            /* SysTick_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
        }

    }

    return retcode;
}

void SystemClock_ISR(void)
{
    if (NULL != BSP_SysTickOsPreProcessing)
    {
        BSP_SysTickOsPreProcessing();
    }

    HAL_IncTick();

    if (NULL != BSP_SysTickOsPostProcessing)
    {
        BSP_SysTickOsPostProcessing();
    }
}

#endif /* if BCDS_FEATURE_HAL_TICK_HANDLER */
/**@}*//* @ingroup BSP_BOARD */
