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

#include "BCDS_BSP_Board.h"

#include "BCDS_HAL_Delay.h"
#include "BSP_Sensgate.h"
#include "protected/board.h"
#include "protected/time.h"
#include "protected/gpio.h"
#include <string.h>
#include "core_cm4.h"
#include "stdio.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID MODULE_BSP_API_BOARD

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

static Retcode_T Board_InterruptsInit(void);

static Retcode_T Board_AMBAClockInit(void);

static Retcode_T Board_CacheInit(void);

static Retcode_T Board_PowerInit(void);

static Retcode_T Board_OscillatorsInit(void);

static Retcode_T Board_PeripheralsClockInit(void);

static Retcode_T Board_SysTickInit(void);

static Retcode_T Board_GPIOInit(void);

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
Retcode_T BSP_Board_Initialize(uint32_t param1, void* param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    Retcode_T retcode = RETCODE_OK;

    retcode = Board_InterruptsInit();

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
        retcode = Board_SysTickInit();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = Board_GPIOInit();
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
void BSP_Board_Delay(uint32_t delayMs)
{
    uint64_t timeStart = Time_GetTimeStamp();
    while ((Time_GetTimeStamp() - timeStart) < delayMs * 1000)
    {
    }
}

/**
 * See API interface for function documentation
 */
void BSP_Board_DelayUs(uint32_t delayUs)
{
    uint64_t timeStart = Time_GetTimeStamp();
    while ((Time_GetTimeStamp() - timeStart) < delayUs)
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
Retcode_T Board_InterruptsInit(void)
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    __enable_irq();
    /* set Interrupt Group Priority */
    return RETCODE_OK;
}

/**
 * Initializes internal flash memory data and instruction caching and instruction prefetching.
 * @retval RETCODE_OK
 */
Retcode_T Board_CacheInit(void)
{
    /* caching instructions enabled */
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    /* caching data Enabled*/
    __HAL_FLASH_DATA_CACHE_ENABLE();
    /* prefetching disabled due to timing non-determinism*/
    __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
    return RETCODE_OK;
}

/**
 * Initializes internal MCU power module.
 * @retval RETCODE_OK in case of success
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_PWR_VOLTAGE_SCALING_FAILED) in case failure
 *
 */
Retcode_T Board_PowerInit(void)
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
Retcode_T Board_OscillatorsInit(void)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_OSCILLATORS_INIT_FAILED);

    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSI
            | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6; /* 4MHz */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
    {
        /* Enable MSI in PLL mode */
        HAL_RCCEx_EnableMSIPLLMode();
        retcode = RETCODE_OK;
    }
    return retcode;
}

/**
 * Initializes ASB, AHB and APB bus clock sourcing.
 * @retval RETCODE_OK In case of success.
 * @retval RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_OSCILLATORS_INIT_FAILED) In case of failure
 */
Retcode_T Board_AMBAClockInit(void)
{
    Retcode_T retcode = RETCODE_OK;

    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
            | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
Retcode_T Board_PeripheralsClockInit(void)
{
    Retcode_T retcode = RETCODE_OK;
    RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2
            | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_I2C3 | RCC_PERIPHCLK_ADC
            | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_USB;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
    PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
    PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_HSI;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_BOARD_PERIPHERALS_CLOCK_INIT_FAILED);
    }
    return retcode;
}

/**
 * Initializes the CPU core SysTick source
 * @retval RETCODE_OK
 */
Retcode_T Board_SysTickInit(void)
{
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    return RETCODE_OK;
}

/**
 * Initializes the Board GPIOs
 * @retval
 */
Retcode_T Board_GPIOInit(void)
{
    GPIO_InitTypeDef BSP_GPIOInitStruct;
    memset(&BSP_GPIOInitStruct, 0, sizeof(GPIO_InitTypeDef));

    GPIO_OpenClockGate(GPIO_PORT_A, PINA_EN_POW_SENS | PINA_EN_POW_GPS | PINA_VUSBM | PINA_EN_POW_GSM);
    BSP_GPIOInitStruct.Pin = PINA_EN_POW_SENS | PINA_EN_POW_GPS | PINA_EN_POW_GSM;
    BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
    BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &BSP_GPIOInitStruct);

    BSP_GPIOInitStruct.Pin = PINA_VUSBM;
    BSP_GPIOInitStruct.Mode = GPIO_MODE_ANALOG;
    BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
    BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &BSP_GPIOInitStruct);
    GPIO_CloseClockGate(GPIO_PORT_A, PINA_EN_POW_SENS | PINA_EN_POW_GPS | PINA_VUSBM | PINA_EN_POW_GSM);

    GPIO_OpenClockGate(GPIO_PORT_B, PINB_EN_POW_BLE | PINB_EN_POW_MEM);
    BSP_GPIOInitStruct.Pin = PINB_EN_POW_BLE | PINB_EN_POW_MEM;
    BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
    BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &BSP_GPIOInitStruct);
    GPIO_CloseClockGate(GPIO_PORT_B, PINB_EN_POW_BLE | PINB_EN_POW_MEM);

    GPIO_OpenClockGate(GPIO_PORT_C, PINC_VBATM | PINC_EN_POW_5V_B | PINC_EN_POW_5V_A);
    BSP_GPIOInitStruct.Pin = PINC_EN_POW_5V_B | PINC_EN_POW_5V_A;
    BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
    BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &BSP_GPIOInitStruct);

    BSP_GPIOInitStruct.Pin = PINC_VBATM;
    BSP_GPIOInitStruct.Mode = GPIO_MODE_ANALOG;
    BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
    BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &BSP_GPIOInitStruct);
    GPIO_CloseClockGate(GPIO_PORT_C, PINC_VBATM | PINC_EN_POW_5V_B | PINC_EN_POW_5V_A);

    GPIO_OpenClockGate(GPIO_PORT_D, PIND_PG_5V0);
    BSP_GPIOInitStruct.Pin = PIND_PG_5V0;
    BSP_GPIOInitStruct.Mode = GPIO_MODE_INPUT;
    BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
    BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &BSP_GPIOInitStruct);
    GPIO_CloseClockGate(GPIO_PORT_D, PIND_PG_5V0);

    GPIO_OpenClockGate(GPIO_PORT_E, PINE_EN_POW_CAN | PINE_EN_VBATM);
    BSP_GPIOInitStruct.Pin = PINE_EN_POW_CAN | PINE_EN_VBATM;
    BSP_GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    BSP_GPIOInitStruct.Pull = GPIO_NOPULL;
    BSP_GPIOInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &BSP_GPIOInitStruct);
    GPIO_CloseClockGate(GPIO_PORT_E, PINE_EN_POW_CAN | PINE_EN_VBATM);
    return RETCODE_OK;
}

void Error_Handler(void)
{
    for(;;);
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
