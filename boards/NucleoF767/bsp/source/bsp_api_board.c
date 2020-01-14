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

#include "Kiso_BSP_Board.h"

#include <string.h>

#include "protected/board.h"
#include "protected/gpio.h"
#include "protected/time.h"
#include "BSP_NucleoF767.h"
#include "stm32f7xx_hal.h"
#include "core_cm7.h"
#include "stdio.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_API_BOARD

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

static Retcode_T Board_InterruptsInit(void);

static Retcode_T Board_AMBAClockInit(void);

static Retcode_T Board_CacheInit(void);

static Retcode_T Board_PowerInit(void);

static Retcode_T Board_OscillatorsInit(void);

static Retcode_T Board_PeripheralsClockInit(void);

void SysTick_Handler(void);

void Error_Handler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static bool initDone = false; /**< board initialization status */

static BSP_Systick_Callback_T preTickHandler = NULL; /**< function to be executed prior to incrementing the SYSTICK */

static BSP_Systick_Callback_T postTickHandler = NULL; /**< function to be executed after incrementing the SYSTICK */

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_PWR_VOLTAGE_SCALING_FAILED) in case voltage scaling failed
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_OSCILLATORS_INIT_FAILED) in case oscillators init failed
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_AMBA_CLOCK_INIT_FAILED) in case AMBA bus clock init failed
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_PERIPHERALS_CLOCK_INIT_FAILED) in case peripherals clock init failed
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_TIME_INIT_FAILED) in case time-stamp init failed
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_TIME_INIT_FAILED) in case time-stamp starting failed
 */
Retcode_T BSP_Board_Initialize(uint32_t param1, void *param2)
{
    KISO_UNUSED(param1);
    KISO_UNUSED(param2);

    Retcode_T retcode = RETCODE_OK;

    if (RETCODE_OK == retcode)
    {
        retcode = Board_InterruptsInit();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Board_CacheInit();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Board_PowerInit();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Board_OscillatorsInit();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Board_AMBAClockInit();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Board_PeripheralsClockInit();
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Time_StartTime(0);
    }
    if (RETCODE_OK == retcode)
    {
        initDone = true;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 */
void BSP_Board_Delay(uint32_t delayInMs)
{
    uint64_t timeStart = Time_GetTimeStamp();
    while ((Time_GetTimeStamp() - timeStart) < delayInMs * 1000)
    {
    }
}

/**
 * See API interface for function documentation
 */
void BSP_Board_DelayUs(uint32_t delayInUs)
{
    uint64_t timeStart = Time_GetTimeStamp();
    while ((Time_GetTimeStamp() - timeStart) < delayInUs)
    {
    }
}

/**
 * See API interface for function documentation
 */
void BSP_Board_SoftReset(void)
{
    NVIC_SystemReset();
}

/**
 * See API interface for function documentation
 */
void BSP_Board_OSTickInitialize(BSP_Systick_Callback_T PreCallback, BSP_Systick_Callback_T PostCallback)
{
    preTickHandler = PreCallback;
    postTickHandler = PostCallback;
}
/**
 * See API interface for function documentation
 */
bool Board_IsInitDone(void)
{
    return initDone;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Initializes the NVIC priority grouping and enables general interrupts.
 * @retval RETCODE_OK
 */
static Retcode_T Board_InterruptsInit(void)
{
    Retcode_T retcode = RETCODE_OK;
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    __enable_irq();
    /* set Interrupt Group Priority */
    return retcode;
}

/**
 * Initializes internal flash memory data and instruction caching and instruction prefetching.
 * @retval RETCODE_OK
 */
static Retcode_T Board_CacheInit(void)
{
#if ART_ACCLERATOR_ENABLE
    /*
     * Configure Instruction cache through ART accelerator
     */
    __HAL_FLASH_ART_ENABLE();
#endif /* ART_ACCLERATOR_ENABLE */

#if PREFETCH_ENABLE
    /*
     * Configure Flash prefetch
     */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */
    return RETCODE_OK;
}

/**
 * Initializes internal MCU power module.
 * @retval RETCODE_OK in case of success
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_PWR_VOLTAGE_SCALING_FAILED) in case failure
 *
 */
static Retcode_T Board_PowerInit(void)
{
    Retcode_T retcode = RETCODE_OK;
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_PWR_VOLTAGE_SCALING_FAILED);
    }
    __HAL_RCC_PWR_CLK_DISABLE();
    return retcode;
}

/**
 * Initializes system Oscillators.
 * @retval RETCODE_OK In case of success.
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_OSCILLATORS_INIT_FAILED) In case of failure
 *
 */
static Retcode_T Board_OscillatorsInit(void)
{
    Retcode_T retcode = RETCODE_OK;

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /*Configure the main internal regulator output voltage used to drive internal PLL */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /*Initializes the HSI and PLL clocks*/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 216;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 9;

    if (HAL_OK != HAL_RCC_OscConfig(&RCC_OscInitStruct))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_UNINITIALIZED);
    }

    if (RETCODE_OK == retcode)
    {
        /* Activating the Over-Drive Mode*/
        if (HAL_OK != HAL_PWREx_EnableOverDrive())
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INCONSITENT_STATE);
        }
    }

    return retcode;
}

/**
 * Initializes ASB, AHB and APB bus clock sourcing.
 * @retval RETCODE_OK In case of success.
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_OSCILLATORS_INIT_FAILED) In case of failure
 */

static Retcode_T Board_AMBAClockInit(void)
{
    Retcode_T retcode = RETCODE_OK;

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* pll is selected as system clock as with that we can achieve 216MHz */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_OK != HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_AMBA_CLOCK_INIT_FAILED);
    }
    return retcode;
}

/**
 *  Initializes Core peripherals clock sourcing.
 * @retval RETCODE_OK in case of success
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_PERIPHERALS_CLOCK_INIT_FAILED) in case of failure.
 */
static Retcode_T Board_PeripheralsClockInit(void)
{
    Retcode_T retcode = RETCODE_OK;
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_I2C3 | RCC_PERIPHCLK_LPTIM1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
    PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
    PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_HSI;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_PERIPHERALS_CLOCK_INIT_FAILED);
    }
    return retcode;
}

void Error_Handler(void)
{
    for (;;)
    {
        /*endless loop*/
    }
}

/**
 * Interrupt Service Routine for SysTick interrupts
 */
void SysTick_Handler(void)
{
    if (NULL != preTickHandler)
    {
        preTickHandler();
    }

    HAL_IncTick();

    if (NULL != postTickHandler)
    {
        postTickHandler();
    }
}
