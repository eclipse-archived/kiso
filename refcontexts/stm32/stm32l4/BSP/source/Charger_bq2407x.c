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
 */
/*----------------------------------------------------------------------------*/

/**
 * @ingroup BSP_Charger_MODULE
 * @{
 * @file
 * @brief  Implementation of Charger BSP functionality.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_CHARGER_BQ2407X

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_Charger_BQ2407X.h"

#if BCDS_FEATURE_BSP_CHARGER_BQ2407X

#include "Charger_bq2407x.h"

#include "BoardSettings.h"
#include "BSP_BoardType.h"

#include "AnalogSensor.h"
#include "BSP_ModuleState.h"

static volatile enum BSP_ModuleState_E ModuleState_Charger_bq2407x = BSP_MODULE_STATE_UNINITIALIZED;
static BSP_AnalogSensor_NotifyObject_T AnalogSensorNotifyObject;
static int32_t ScaledAnalogSignals[BSP_ANALOG_SIGNAL_COUNT] = { INT32_C(0) };

static TIM_HandleTypeDef BSP_ChargerTIMHandle;

static enum ChargingStatus_E ChargingStatus = BSP_STATUS_CHARGING_OFF;

static Retcode_T AnalogSensorNotifyFunction(uint32_t size, int32_t buffer[]);

Retcode_T Board_ChargerTIM_Init(void);

Retcode_T AnalogSensorNotifyFunction(uint32_t size, int32_t buffer[])
{
    memcpy((void*) ScaledAnalogSignals, buffer, size * sizeof(int32_t));

    return RETCODE_OK;
}

enum BSP_ModuleState_E Charger_bq2407x_getState(void)
{
    return ModuleState_Charger_bq2407x;
}

Retcode_T Board_ChargerTIM_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    CHARGING_TIMER_CLOCK_ENABLE();

    BSP_ChargerTIMHandle.Instance = CHARGING_TIMER;
    BSP_ChargerTIMHandle.Init.Prescaler = CHARGING_TIMER_PRESCALER;
    BSP_ChargerTIMHandle.Init.CounterMode = CHARGING_TIMER_COUNTER_MODE;
    BSP_ChargerTIMHandle.Init.Period = CHARGING_ERROR_TIMEOUT_TIME_MS;
    BSP_ChargerTIMHandle.Init.ClockDivision = CHARGING_TIMER_CLK_DIV;
    if (HAL_OK != HAL_TIM_Base_Init(&BSP_ChargerTIMHandle))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    if (RETCODE_OK == retcode)
    {
        CLEAR_BIT(CHARGING_TIMER->SR, TIM_IT_UPDATE);
    }
    /* Disable the Clock */
    CHARGING_TIMER_CLOCK_DISABLE();
    if (RETCODE_OK == retcode)
    {
        ModuleState_Charger_bq2407x = BSP_MODULE_STATE_DISCONNECTED;
    }

    return retcode;
}

Retcode_T BSP_Charger_BQ2407X_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    GPIO_InitTypeDef GPIO_InitStruct;

    switch(ModuleState_Charger_bq2407x)
    {
    case BSP_MODULE_STATE_DISCONNECTED:
    case BSP_MODULE_STATE_ERROR: /*In case of error state we try to connect again to go back in a defined state, will always succeed */
        /* Enabling GPIO clocks required by the Charger module */
            CHARGER_GPIO_CLOCK_ENABLE();

            memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
            GPIO_InitStruct.Pin = CHARGING_EN_PIN;
            GPIO_InitStruct.Mode = CHARGING_EN_MODE;
            GPIO_InitStruct.Pull = CHARGING_EN_PULL;
            GPIO_InitStruct.Speed = CHARGING_EN_SPEED;
            HAL_GPIO_Init(CHARGING_EN_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(CHARGING_EN_PORT, CHARGING_EN_PIN, BSP_IO_PIN_HIGH);

            memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
            GPIO_InitStruct.Pin = CHARGING_IN_EN1_PIN;
            GPIO_InitStruct.Mode = CHARGING_IN_EN1_MODE;
            GPIO_InitStruct.Pull = CHARGING_IN_EN1_PULL;
            GPIO_InitStruct.Speed = CHARGING_IN_EN1_SPEED;
            HAL_GPIO_Init(CHARGING_IN_EN1_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(CHARGING_IN_EN1_PORT, CHARGING_IN_EN1_PIN, BSP_IO_PIN_LOW);

            memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
            GPIO_InitStruct.Pin = CHARGING_IN_EN2_PIN;
            GPIO_InitStruct.Mode = CHARGING_IN_EN2_MODE;
            GPIO_InitStruct.Pull = CHARGING_IN_EN2_PULL;
            GPIO_InitStruct.Speed = CHARGING_IN_EN2_SPEED;
            HAL_GPIO_Init(CHARGING_IN_EN2_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(CHARGING_IN_EN2_PORT, CHARGING_IN_EN2_PIN, BSP_IO_PIN_LOW);

            memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
            GPIO_InitStruct.Pin = CHARGING_PWR_GOOD_PIN;
            GPIO_InitStruct.Mode = CHARGING_PWR_GOOD_MODE;
            GPIO_InitStruct.Pull = CHARGING_PWR_GOOD_PULL;
            HAL_GPIO_Init(CHARGING_PWR_GOOD_PORT, &GPIO_InitStruct);

            memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
            GPIO_InitStruct.Pin = CHARGING_STAT_PIN;
            GPIO_InitStruct.Mode = CHARGING_STAT_MODE;
            GPIO_InitStruct.Pull = CHARGING_STAT_PULL;
            HAL_GPIO_Init(CHARGING_STAT_PORT, &GPIO_InitStruct);

            /* connect analog sensor interface, always return OK */
            retcode = BSP_AnalogSensor_Connect();

            if(RETCODE_OK == retcode)
            {
                ModuleState_Charger_bq2407x = BSP_MODULE_STATE_CONNECTED;
            }
            else
            {
                //not possible as connect will always return OK
                ModuleState_Charger_bq2407x = BSP_MODULE_STATE_ERROR;
            }
        break;
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        retcode = RETCODE_OK;
        break;
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }
    BSP_MODULE_UNLOCK;

    return retcode;
}

/*
 * refer to interface for description
 */
Retcode_T BSP_Charger_BQ2407X_Enable(uint32_t control)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    assert(FALSE != IS_CHARGING_LEVEL((BSP_ChargingLevel_T )control));

    switch(ModuleState_Charger_bq2407x)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /* Enable the Timer for level duration measurement */
        CHARGING_TIMER_CLOCK_ENABLE();

        /* enable analog sensor interface, always return OK*/
        retcode = BSP_AnalogSensor_RegisterNotify(&AnalogSensorNotifyObject, AnalogSensorNotifyFunction);
        if (RETCODE_OK == retcode)
        {
            retcode = BSP_AnalogSensor_Enable(0);
        }
        if (RETCODE_OK == retcode)
        {
            /* Configure the external interrupt line 7 used for CHARGING_STAT pin */
            HAL_NVIC_SetPriority(CHARGING_STAT_EXTI_IRQN, CHARGING_STAT_EXTI_PREEMPT_PRIORITY, CHARGING_STAT_EXTI_SUBPRIORITY);
            /* Configure the NVIC for button timer */
            HAL_NVIC_SetPriority(CHARGING_TIMER_IRQN, CHARGING_TIMER_PREEMPT_PRIORITY, CHARGING_TIMER_SUBPRIORITY);
            /* Enable the External Interrupt Interrupt  for the CHARGING_STAT pin */
            HAL_NVIC_EnableIRQ(CHARGING_STAT_EXTI_IRQN);
            HAL_NVIC_EnableIRQ(CHARGING_TIMER_IRQN);

            ModuleState_Charger_bq2407x = BSP_MODULE_STATE_ENABLED;
        }
        else
        {
            ModuleState_Charger_bq2407x = BSP_MODULE_STATE_ERROR;
            break;
        } /* FALL-THROUGH */
    case BSP_MODULE_STATE_ENABLED: /* FALL-THROUGH: */
        switch (control)
        {
        case BSP_CHARGING_LEVEL_0:
            HAL_GPIO_WritePin(CHARGING_EN_PORT, CHARGING_EN_PIN, BSP_IO_PIN_HIGH);
            break;
        case BSP_CHARGING_LEVEL_1:
            HAL_GPIO_WritePin(CHARGING_IN_EN1_PORT, CHARGING_IN_EN1_PIN, BSP_IO_PIN_LOW);
            HAL_GPIO_WritePin(CHARGING_IN_EN2_PORT, CHARGING_IN_EN2_PIN, BSP_IO_PIN_LOW);
            HAL_GPIO_WritePin(CHARGING_EN_PORT, CHARGING_EN_PIN, BSP_IO_PIN_LOW);
            break;
        case BSP_CHARGING_LEVEL_2:
            HAL_GPIO_WritePin(CHARGING_IN_EN1_PORT, CHARGING_IN_EN1_PIN, BSP_IO_PIN_HIGH);
            HAL_GPIO_WritePin(CHARGING_IN_EN2_PORT, CHARGING_IN_EN2_PIN, BSP_IO_PIN_LOW);
            HAL_GPIO_WritePin(CHARGING_EN_PORT, CHARGING_EN_PIN, BSP_IO_PIN_LOW);
            break;
        case BSP_CHARGING_LEVEL_3:
            HAL_GPIO_WritePin(CHARGING_IN_EN1_PORT, CHARGING_IN_EN1_PIN, BSP_IO_PIN_LOW);
            HAL_GPIO_WritePin(CHARGING_IN_EN2_PORT, CHARGING_IN_EN2_PIN, BSP_IO_PIN_HIGH);
            HAL_GPIO_WritePin(CHARGING_EN_PORT, CHARGING_EN_PIN, BSP_IO_PIN_LOW);
            break;
        }
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_Charger_BQ2407X_Disable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_Charger_bq2407x)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* Disable the Timer */
        CHARGING_TIMER_CLOCK_DISABLE();
        /* Disable the interrupt for the CHARGING_STAT pin */
        HAL_NVIC_DisableIRQ(CHARGING_STAT_EXTI_IRQN);
        HAL_NVIC_DisableIRQ(CHARGING_TIMER_IRQN);
        ChargingStatus = BSP_STATUS_CHARGING_OFF;
        /* Disable the charger */
        HAL_GPIO_WritePin(CHARGING_EN_PORT, CHARGING_EN_PIN, BSP_IO_PIN_HIGH);

        /* enable analog sensor interface */
        retcode = BSP_AnalogSensor_DeregisterNotify(&AnalogSensorNotifyObject);

        if (RETCODE_OK == retcode)
        {
            retcode = BSP_AnalogSensor_Disable(0);
        }

        if (RETCODE_OK == retcode)
        {
            ModuleState_Charger_bq2407x = BSP_MODULE_STATE_CONNECTED;
        }
        else
        {
            ModuleState_Charger_bq2407x = BSP_MODULE_STATE_ERROR;
        }
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Charger_BQ2407X_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_Charger_bq2407x)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /* Enabling GPIO clocks required by the Charger module */
        CHARGER_GPIO_CLOCK_ENABLE();

        HAL_GPIO_DeInit(CHARGING_EN_PORT, CHARGING_EN_PIN);
        HAL_GPIO_DeInit(CHARGING_IN_EN1_PORT, CHARGING_IN_EN1_PIN);
        HAL_GPIO_DeInit(CHARGING_IN_EN2_PORT, CHARGING_IN_EN2_PIN);

        HAL_GPIO_DeInit(CHARGING_PWR_GOOD_PORT, CHARGING_PWR_GOOD_PIN);

        HAL_GPIO_DeInit(CHARGING_STAT_PORT, CHARGING_STAT_PIN);

        ModuleState_Charger_bq2407x = BSP_MODULE_STATE_DISCONNECTED;
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}


void ChargerTIM_ISR(void)
{
    /* Check if TIM Update event */
    if ((CHARGING_TIMER->DIER & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE))
    {
        /* Checkout interrupt */
        if ((CHARGING_TIMER->SR & (TIM_IT_UPDATE)) == (TIM_IT_UPDATE))
        {
            /* Reset The interrupt flag */
            CHARGING_TIMER->SR &= ~(TIM_IT_UPDATE);
            if (BSP_MODULE_STATE_ENABLED == ModuleState_Charger_bq2407x)
            {
                if (BSP_IO_PIN_LOW == HAL_GPIO_ReadPin(CHARGING_STAT_PORT, CHARGING_STAT_PIN))
                {
                    ChargingStatus = BSP_STATUS_CHARGING_ON;
                }
                else
                {
                    ChargingStatus = BSP_STATUS_CHARGING_OFF;
                }
            }
            /* Disable the Timer Peripheral */
            CHARGING_TIMER->CR1 &= ~(TIM_CR1_CEN);
        }
    }
}

void Charging_Stat_EXTI7ISR(void)
{
    /* Check if the external interrupt comes from CHARGING_STAT pin */
    if (__HAL_GPIO_EXTI_GET_IT(CHARGING_STAT_PIN) != (uint32_t) RESET)
    {
        /* Clear the interrupt flag */
        __HAL_GPIO_EXTI_CLEAR_IT(CHARGING_STAT_PIN);
        if (BSP_MODULE_STATE_ENABLED == ModuleState_Charger_bq2407x)
        {
            if(CHARGING_TIMER->CR1 & TIM_CR1_CEN)
            { /* Timer is already running */
                ChargingStatus = BSP_STATUS_CHARGING_ERROR;
            }
            else
            {
                if (BSP_IO_PIN_LOW == HAL_GPIO_ReadPin(CHARGING_STAT_PORT, CHARGING_STAT_PIN))
                {
                    ChargingStatus = BSP_STATUS_CHARGING_ON;
                }
                else
                {
                    ChargingStatus = BSP_STATUS_CHARGING_OFF;
                }
            }
            /* Disable the Timer Peripheral */
            CHARGING_TIMER->CR1 &= ~(TIM_CR1_CEN);
            /* Start timer */
            /* Update the Prescaler for 1ms timer Tick */
            SystemCoreClockUpdate();
            CHARGING_TIMER->PSC = (SystemCoreClock / 1000) - 1;
            /* Reset the counter to zero */
            CHARGING_TIMER->CNT &= 0x0000;
            /* Enable the TIM Update interrupt */
            CHARGING_TIMER->DIER |= TIM_IT_UPDATE;
            /* Enable the Timer Peripheral */
            CHARGING_TIMER->CR1 |= TIM_CR1_CEN;
            /* Prepare register for the next update event */
            CHARGING_TIMER->ARR = CHARGING_ERROR_TIMEOUT_TIME_MS;

            /* Call the service routine for the CHARGING_STAT pin */
        }
    }
}


Retcode_T BSP_Charger_BQ2407X_GetStatus(BSP_ChargeState_T * state)
{
    Retcode_T rc = RETCODE_OK;

    if(NULL != state )
    {
        if (BSP_MODULE_STATE_ENABLED != ModuleState_Charger_bq2407x)
        {
            ChargingStatus = BSP_STATUS_CHARGING_OFF;
        }

        *state = ChargingStatus;
    }
    else
    {
        rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    return rc;
}

Retcode_T BSP_Charger_BQ2407X_MeasureSignal(uint32_t signal, uint32_t* output)
{
    Retcode_T retval = RETCODE_OK;

    if(NULL == output)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (BSP_ANALOG_SIGNAL_COUNT <= (BSP_AnalogSignal_T) signal)
    {
        retval = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else if (BSP_MODULE_STATE_ENABLED == ModuleState_Charger_bq2407x)
    {

        retval = BSP_AnalogSensor_PollSignals();
        if (RETCODE_OK == retval)
        {
            /*lint -e{661,662} The value of signal is tested against BSP_ANALOG_SIGNAL_COUNT */
            *output = (uint32_t)ScaledAnalogSignals[signal];
        }
    }

    return retval;
}

Retcode_T BSP_Charger_BQ2407X_SetSysoffLow(void)
{
    return RETCODE(RETCODE_SEVERITY_NONE, RETCODE_NOT_SUPPORTED);
}

Retcode_T BSP_Charger_BQ2407X_SetSysoffHigh(void)
{
    return RETCODE(RETCODE_SEVERITY_NONE, RETCODE_NOT_SUPPORTED);
}


#endif /* if BCDS_FEATURE_BSP_CHARGER_BQ2407X */
/**@}*//* @ingroup BSP_Charger_MODULE */
