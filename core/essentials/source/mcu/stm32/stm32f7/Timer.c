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

#include "Kiso_MCU_Timer.h"

#if KISO_FEATURE_TIMER

#include "Kiso_MCU_Timer_Handle.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_TIMER

void MCU_BSP_TIMER_IRQ_Callback(Timer_T timer);

/* Refer to API header for documentation */
Retcode_T MCU_Timer_Initialize(Timer_T timer, MCU_TIMER_Callback_T Callback)
{
    /* Test Handle structure for validity */
    if (NULL == timer)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* Test callback pointer for validity */
    if ((NULL == Callback))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    Retcode_T retcode = RETCODE_OK;

    struct MCU_Timer_Handle_S *timerHandle = (struct MCU_Timer_Handle_S *)timer;

    if ((MCU_TIMER_STATUS_INITIALIZED == timerHandle->Status) || (MCU_TIMER_STATUS_ACTIVE == timerHandle->Status) || (MCU_TIMER_STATUS_IDLE == timerHandle->Status))
    {
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_ALREADY_INITIALIZED);
    }

    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_TIM_Base_Init(&timerHandle->TimerHandle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_INIT_FAIL);
        }
    }

    /* Register Callback based  on the mode */
    if (RETCODE_OK == retcode)
    {
        if (STM_TIMER_MODE_INTERRUPT == timerHandle->TimMode)
        {
            timerHandle->TimerStartFunPtr = HAL_TIM_Base_Start_IT;
            timerHandle->TimerStopFunPtr = HAL_TIM_Base_Stop_IT;
            timerHandle->IRQCallback = MCU_BSP_TIMER_IRQ_Callback;
            timerHandle->Status = MCU_TIMER_STATUS_INITIALIZED;
            timerHandle->AppCallback = Callback;
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        }
    }

    return retcode;
}

/* Refer to API header for documentation */
Retcode_T MCU_Timer_Deinitialize(Timer_T timer)
{
    /* Test Handle structure for validity */
    if (NULL == timer)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = RETCODE_OK;

    struct MCU_Timer_Handle_S *timerHandle = (struct MCU_Timer_Handle_S *)timer;

    if (timerHandle->Status == MCU_TIMER_STATUS_ACTIVE)
    {
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_IS_ACTIVE);
    }

    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_TIM_Base_DeInit(&timerHandle->TimerHandle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_DEINIT_FAIL);
        }
    }

    if (RETCODE_OK == retcode)
    {
        timerHandle->TimerStartFunPtr = NULL;
        timerHandle->TimerStopFunPtr = NULL;
        timerHandle->IRQCallback = NULL;
        timerHandle->AppCallback = NULL;
        timerHandle->Status = MCU_TIMER_STATUS_DEINITIALIZED;
    }

    return retcode;
}

/* Refer to API header for documentation */
Retcode_T MCU_Timer_Start(Timer_T timer)
{
    /* Test Handle structure for validity */
    if (NULL == timer)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = RETCODE_OK;
    HAL_StatusTypeDef halStatus;

    struct MCU_Timer_Handle_S *timerHandle = (struct MCU_Timer_Handle_S *)timer;

    if ((MCU_TIMER_STATUS_DEINITIALIZED == timerHandle->Status))
    {
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_NOINIT);
    }

    if ((MCU_TIMER_STATUS_ACTIVE == timerHandle->Status))
    {
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_IS_ACTIVE);
    }

    if (NULL == timerHandle->TimerStartFunPtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (RETCODE_OK == retcode)
    {
        halStatus = timerHandle->TimerStartFunPtr(&timerHandle->TimerHandle);

        if (HAL_OK != halStatus)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_START_FAIL);
        }
    }

    if (RETCODE_OK == retcode)
    {
        /* Timer was started and should be active */
        timerHandle->Status = MCU_TIMER_STATUS_ACTIVE;
    }

    return retcode;
}

/* Refer to API header for documentation */
Retcode_T MCU_Timer_Stop(Timer_T timer)
{
    /* Test Handle structure for validity */
    if (NULL == timer)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = RETCODE_OK;

    HAL_StatusTypeDef halStatus;

    struct MCU_Timer_Handle_S *timerHandle = (struct MCU_Timer_Handle_S *)timer;

    if (MCU_TIMER_STATUS_ACTIVE != timerHandle->Status)
    {
        retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_TIMER_IS_NOT_ACTIVE);
    }

    if (NULL == timerHandle->TimerStopFunPtr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (RETCODE_OK == retcode)
    {

        halStatus = timerHandle->TimerStopFunPtr(&timerHandle->TimerHandle);

        if (HAL_OK != halStatus)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMER_STOP_FAIL);
        }
    }
    if (RETCODE_OK == retcode)
    {
        /* Timer was stoped and should be idle */
        timerHandle->Status = MCU_TIMER_STATUS_IDLE;
    }

    return retcode;
}

/* Active or Idle */
MCU_Timer_Status_T MCU_Timer_GetStatus(Timer_T timer)
{
    MCU_Timer_Status_T Status = MCU_TIMER_STATUS_ERROR;

    struct MCU_Timer_Handle_S *timerHandle = (struct MCU_Timer_Handle_S *)timer;

    if (NULL != timerHandle)
    {
        Status = timerHandle->Status;
    }

    return Status;
}

/** Timer IRQ callback handler which is called back
 * by the BSP Timer IRQ handler
 * @param timer is the BSP handle for the interface
 */
void MCU_BSP_TIMER_IRQ_Callback(Timer_T timer)
{
    struct MCU_Timer_Handle_S *timerHandle = (struct MCU_Timer_Handle_S *)timer;

    if (timerHandle)
    {
        HAL_TIM_IRQHandler(&timerHandle->TimerHandle);
    }
}

/**
 * @brief Callback function for Timer elapsed events.
 * @details This function overrides the weak definition provided by the STM32
 * HAL and is called when a periodElapsed is completed.
 * @param htim is the STM32 HAL handle of the interface that reports the event
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    struct MCU_Timer_Handle_S *timerHandle = (struct MCU_Timer_Handle_S *)htim;

    struct MCU_Timer_Event_S event;

    event.CompareChannel = UINT32_C(0);
    event.CompareMatch = UINT8_C(0);
    event.ErrorCode = UINT32_C(0);
    event.Overflow = UINT8_C(0);
    event.reserved = UINT8_C(0);
    event.Underflow = UINT8_C(0);

    if (timerHandle && ((timerHandle->AppCallback) != NULL))
    {
        /* Has a valid handle, now process the event */
        event.CompareMatch = 1;
        timerHandle->AppCallback((Timer_T)timerHandle, event);
    }
}

Retcode_T MCU_Timer_SetCompareValue(Timer_T timer, uint32_t channel, uint32_t value)
{
    KISO_UNUSED(timer);
    KISO_UNUSED(channel);
    KISO_UNUSED(value);

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T MCU_Timer_GetCompareValue(Timer_T timer, uint32_t channel, uint32_t *value)
{
    KISO_UNUSED(timer);
    KISO_UNUSED(channel);
    KISO_UNUSED(value);

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T MCU_Timer_GetCountValue(Timer_T timer, uint32_t *countValue)
{
    KISO_UNUSED(timer);
    KISO_UNUSED(countValue);

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T MCU_Timer_GetMaxCount(Timer_T timer, uint32_t *maxCount)
{
    KISO_UNUSED(timer);
    KISO_UNUSED(maxCount);

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T MCU_Timer_GetClockFrequency(Timer_T timer, uint32_t *frequency)
{
    KISO_UNUSED(timer);
    KISO_UNUSED(frequency);

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T MCU_Timer_EnableOverflow(Timer_T timer, bool enable)
{
    KISO_UNUSED(timer);
    KISO_UNUSED(enable);

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}
#endif
