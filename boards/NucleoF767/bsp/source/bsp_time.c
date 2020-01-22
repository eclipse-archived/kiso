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

#include "protected/time.h"
#include "BSP_NucleoF767.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_lptim.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_TIME

#define TIME_PERIOD (uint32_t)(65535)
#define TIME_TIMEOUT (uint32_t)(65535)
#define TIME_DIVIDE_BY_65536(x) ((x) >> 16)
#define TIME_REMAINDER_65536(x) ((x) & (65536 - 1))

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void LPTIM1_IRQHandler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static bool timeStarted = false;
static volatile uint64_t overflow = 0;
static volatile uint16_t offset = 0;

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 *  See API interface for function documentation
 */
uint64_t Time_GetTimeStamp(void)
{
    uint32_t temp1 = LPTIM1->CNT;      /* Read LPTIM counter*/
    uint64_t temp2 = overflow << 16;   /* Read overflow counter*/
    return (temp2 + (temp1 + offset)); /* combine time value todo check if the result is correct because of multiple types addition*/
}

/**
 *  See API interface for function documentation
 */
Retcode_T Time_StartTime(uint64_t initialValue)
{
    Retcode_T retcode = RETCODE_OK;

    if (!timeStarted)
    {
        LPTIM_HandleTypeDef LptimHandle =
            {
                .Instance = LPTIM1,
                .Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC,
                .Init.Clock.Prescaler = LPTIM_PRESCALER_DIV16,
                .Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE,
                .Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE,
                .Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL,
            };

        __HAL_RCC_LPTIM1_CLK_ENABLE();
        __HAL_RCC_LPTIM1_FORCE_RESET();
        __HAL_RCC_LPTIM1_RELEASE_RESET();
        /* Enable and set LPTIM Interrupt to high priority */
        HAL_NVIC_SetPriority(LPTIM1_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(LPTIM1_IRQn);

        /* Initialize LPTIM peripheral according to the passed parameters */
        if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_TIME_INIT_FAILED);
        }
        if (RETCODE_OK == retcode)
        {
            overflow = 0;
            offset = 0;
            Time_AdjustTime(initialValue);
        }
        if (RETCODE_OK == retcode)
        {
            if (HAL_LPTIM_Counter_Start_IT(&LptimHandle, TIME_PERIOD) != HAL_OK)
            {
                retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_BSP_TIME_START_FAILED);
            }
        }
        if (RETCODE_OK == retcode)
        {
            timeStarted = true;
        }
    }

    return retcode;
}

/**
 *  See API interface for function documentation
 */
void Time_AdjustTime(uint64_t timeOffset)
{
    overflow += TIME_DIVIDE_BY_65536(timeOffset + offset);
    offset = TIME_REMAINDER_65536(timeOffset + offset);
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Interrupt service routine for LPTIM1 IRQ. Implemented using bit-banding to avoid HAL library overhead
 */
void LPTIM1_IRQHandler(void)
{
    /* Compare Autoreload match interrupt */
    if (LPTIM_ISR_ARRM == (LPTIM1->ISR & LPTIM_ISR_ARRM))
    {
        overflow++;
    }

    /* Clear all LPTIM flags */
    LPTIM1->ICR = LPTIM_ICR_CMPMCF | LPTIM_ICR_ARRMCF | LPTIM_ICR_EXTTRIGCF | LPTIM_ICR_CMPOKCF | LPTIM_ICR_ARROKCF | LPTIM_ICR_UPCF | LPTIM_ICR_DOWNCF;

    /* Clear Interrupt pending flag */
    EXTI->PR = LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT;
}
