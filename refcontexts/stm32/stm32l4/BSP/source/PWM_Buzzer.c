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
 * @ingroup BSP_PWM_BUZZER
 * @{
 * @file
 * @brief  Implementation of PWM Buzzer BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_PWM_BUZZER

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_PWM_Buzzer.h"

#if BCDS_FEATURE_BSP_PWM_BUZZER

#include "BoardSettings.h"
#include "PWM_Buzzer.h"
#include "LDO_tlv70233.h"
#include "BSP_ModuleState.h"

#include "stm32l4xx_ll_tim.h"

#define TIMER_MAXIMUM_PRESCALER 65535

/*
 * Structure representing the peripherals  using TIM1 resource. It allows to
 * know if it is used or not and which peripheral is using it.
 */
struct TIM1Register_S
{
    uint32_t Buzzer :1;
};
/*
 * Union used to control the TIM1 Timer. It allows to know if a
 * peripheral is using it.
 */
union TIM1Register_U
{
    struct TIM1Register_S Peripheral;
    uint32_t Register;
};

typedef union TIM1Register_U TIM1Register_T;

static TIM_HandleTypeDef BSP_TIM1Handle;
static TIM1Register_T BSP_TIM1;

static volatile enum BSP_ModuleState_E ModuleState_PWM_Buzzer = BSP_MODULE_STATE_UNINITIALIZED;
static volatile uint8_t EnableCount;

enum BSP_ModuleState_E PWM_Buzzer_getState(void)
{
    return ModuleState_PWM_Buzzer;
}

static Retcode_T BuzzerPWM_SetDutyCycle(uint32_t dutyCyclePercent);
static Retcode_T BuzzerPWM_SetFrequency_ScaleChannels(uint32_t newFrequencyHz);

static Retcode_T BuzzerPWM_SetDutyCycle(uint32_t dutyCyclePercent)
{
    Retcode_T retcode = RETCODE_OK;

    uint32_t pulse;
    uint32_t period;

    period = LL_TIM_GetAutoReload(BUZZER_PWM_TIMER) + 1;

    pulse = period * dutyCyclePercent / 100;

    BUZZER_PWM_CHANNEL_SETCOMPARE(BUZZER_PWM_TIMER, pulse);

    if (pulse == BUZZER_PWM_CHANNEL_GETCOMPARE(BUZZER_PWM_TIMER))
    {
        retcode = RETCODE_OK;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    return retcode;
}

static Retcode_T BuzzerPWM_SetFrequency_ScaleChannels(uint32_t newFrequencyHz)
{
    /* The Counter Clock CLK_CNT is the result of the division of the Internal Clock CLK_INT by this value*/
    uint32_t prescaler;

    /* The Counter Period is the result of the multiplication of the Counter Clock by this value */
    uint32_t autoReload_current;
    uint32_t autoReload_target;
    uint32_t pulse;

    /* Check if the New frequency is valid
     */
    if ((newFrequencyHz == 0) || (newFrequencyHz > SystemCoreClock / 100))
    {
        /* Cannot use assert here because frequency is runtime variable. */
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    /* Every time a parameter depending on the system clock is calculated there is a need to update the SystemCoreClock.
     * SystemCoreClockUpdate() evaluates the clock register settings and calculates the current core clock.
     */
    SystemCoreClockUpdate();

    autoReload_current = LL_TIM_GetAutoReload(BUZZER_PWM_TIMER);
    pulse = BUZZER_PWM_CHANNEL_GETCOMPARE(BUZZER_PWM_TIMER);

    /* The ratio between system clock and new frequency */
    /* Calculate the FrequencyRatio */
    uint32_t FrequencyRatio = SystemCoreClock / newFrequencyHz;

    /* Calculate the Prescaler value to be stored in the Prescaler Register TIMx_PSC */
    prescaler = FrequencyRatio / (TIMER_MAXIMUM_PRESCALER + 1);

    /* Calculate the AutoReload value to be stored in the AutoReload Register TIMx_ARR */
    autoReload_target = FrequencyRatio / (prescaler + 1);

    /* If the new requested frequency is higher than the previous frequency the result of the
     * integer division is lower than 0. In order to avoid that we introduce the 1000 factor.*/
    pulse = (((autoReload_target + 1)*1000 / (autoReload_current + 1)) * pulse)/1000;

    /* Low level write access to the registers and update their values.
     * These values will be effective at the next update event.
     * In the scope of this configuration the next update event is defined when the Timer counter reaches
     * the AutoReload Register value.
     * */
    LL_TIM_SetPrescaler(BUZZER_PWM_TIMER, prescaler);
    LL_TIM_SetAutoReload(BUZZER_PWM_TIMER, autoReload_target);
    BUZZER_PWM_CHANNEL_SETCOMPARE(BUZZER_PWM_TIMER, pulse);

    /* Check if the values have been correctly written to the registers
     * todo check if there is a need for a wait before read
     */

    if ((LL_TIM_GetPrescaler(BUZZER_PWM_TIMER) == prescaler) && (LL_TIM_GetAutoReload(BUZZER_PWM_TIMER) == autoReload_target))
    {
        return RETCODE_OK;
    }
    else
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
}

/*
 * @brief Initializes the Timer TIM1
 * @details This function Initializes the TIM1 resource in PWM mode. It is called
 * within the Board_Initialize() function. It stores the desired configuration
 * into the resource registers and puts it into low power consumption
 * configuration until it is requested by the application.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_TIM1_Init(void);

Retcode_T Board_TIM1_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
    TIM_OC_InitTypeDef sConfigOC;

    /* Enable the clock */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* Time base configuration */
    BSP_TIM1Handle.Instance = TIM1;
    BSP_TIM1Handle.Init.Prescaler = 0;
    BSP_TIM1Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    BSP_TIM1Handle.Init.Period = 999;
    BSP_TIM1Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    BSP_TIM1Handle.Init.RepetitionCounter = 0;
    if (HAL_OK != HAL_TIM_Base_Init(&BSP_TIM1Handle))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    /* Clock source configuration */
    if (RETCODE_OK == retcode)
    {
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_OK != HAL_TIM_ConfigClockSource(&BSP_TIM1Handle, &sClockSourceConfig))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }

    /* PWM mode configuration */
    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_TIM_PWM_Init(&BSP_TIM1Handle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    /* Master output configuration */
    if (RETCODE_OK == retcode)
    {
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_OK != HAL_TIMEx_MasterConfigSynchronization(&BSP_TIM1Handle, &sMasterConfig))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    /* Break dead time configuration */
    if (RETCODE_OK == retcode)
    {
        sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
        sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
        sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
        sBreakDeadTimeConfig.DeadTime = 0;
        sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
        sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
        sBreakDeadTimeConfig.BreakFilter = 0;
        sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
        sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
        sBreakDeadTimeConfig.Break2Filter = 0;
        sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
        if (HAL_OK != HAL_TIMEx_ConfigBreakDeadTime(&BSP_TIM1Handle, &sBreakDeadTimeConfig))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    /* Output channel configuration */
    if (RETCODE_OK == retcode)
    {
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
        sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

        /* Configure Buzzer channel */
        if (RETCODE_OK == retcode)
        {
            if (HAL_OK != HAL_TIM_PWM_ConfigChannel(&BSP_TIM1Handle, &sConfigOC, BUZZER_PWM_TIMER_CHANNEL))
            {
                retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
            }
        }
    }
    /* Disable the clock */
    __HAL_RCC_TIM1_CLK_DISABLE();

    if(RETCODE_OK == retcode)
    {
        ModuleState_PWM_Buzzer = BSP_MODULE_STATE_DISCONNECTED;
    }
    return retcode;
}

Retcode_T BSP_PWM_Buzzer_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;
    GPIO_InitTypeDef GPIO_InitStruct;
    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_PWM_Buzzer)
    {
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH: */
    case BSP_MODULE_STATE_DISCONNECTED:
        __HAL_RCC_GPIOE_CLK_ENABLE();

        BSP_TIM1.Register = 0;

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Mode = BUZZER_PWM_MODE;
        GPIO_InitStruct.Pull = BUZZER_PWM_PULL;
        GPIO_InitStruct.Speed = BUZZER_PWM_SPEED;
        GPIO_InitStruct.Alternate = BUZZER_PWM_ALTERNATE;
        GPIO_InitStruct.Pin = BUZZER_PWM_PIN;
        HAL_GPIO_Init(BUZZER_PWM_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Mode = VBUZZER_EN_MODE;
        GPIO_InitStruct.Pull = VBUZZER_EN_PULL;
        GPIO_InitStruct.Speed = VBUZZER_EN_SPEED;
        GPIO_InitStruct.Pin = VBUZZER_EN_PIN;
        HAL_GPIO_Init(VBUZZER_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(VBUZZER_EN_PORT, VBUZZER_EN_PIN, GPIO_PIN_RESET);
        /* @todo 2017.04.13 BCDS/ENG2: non-void return value has to be declared in HAL    BSP_MODULE_UNLOCK;*/
        EnableCount = 0;
        ModuleState_PWM_Buzzer = BSP_MODULE_STATE_CONNECTED;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH: */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_PWM_Buzzer_Enable(uint32_t Id)
{
    BSP_MODULE_TRY_LOCK;
    BCDS_UNUSED(Id);
    Retcode_T retcode;

    switch(ModuleState_PWM_Buzzer)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /* Enable the power supply */
        retcode = BSP_LDO_TLV70233_Enable();
        if (RETCODE_OK == retcode)
        {
            /*
             * Enable the clock for the Timer
             */
            BSP_TIM1.Peripheral.Buzzer = 1;
            __HAL_RCC_TIM1_CLK_ENABLE();

            HAL_GPIO_WritePin(VBUZZER_EN_PORT, VBUZZER_EN_PIN, GPIO_PIN_SET);
            /*
             * the channel will start running with zero duty cycle
             */
            retcode = BuzzerPWM_SetDutyCycle(0);
        }
        if (RETCODE_OK == retcode)
        {
            if(HAL_OK != HAL_TIM_PWM_Start(&BSP_TIM1Handle, BUZZER_PWM_TIMER_CHANNEL))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }

        if (RETCODE_OK == retcode)
        {
            ModuleState_PWM_Buzzer = BSP_MODULE_STATE_ENABLED;
        }
        else
        {
            ModuleState_PWM_Buzzer = BSP_MODULE_STATE_ERROR;
        }
        break;
    case BSP_MODULE_STATE_ENABLED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    if(RETCODE_OK == retcode)
    {
        EnableCount++;
        assert(EnableCount);
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_PWM_Buzzer_Switch(uint32_t Id, uint32_t Frequency_Hz, uint32_t Volume_Percent)
{
    BSP_MODULE_TRY_LOCK;
    BCDS_UNUSED(Id);
    Retcode_T retcode = RETCODE_OK;
    if ((PWM_FREQUENCY_MAX < Frequency_Hz) || (PWM_FREQUENCY_MIN > Frequency_Hz))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if(RETCODE_OK == retcode)
    {
        retcode = BuzzerPWM_SetFrequency_ScaleChannels(Frequency_Hz);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BuzzerPWM_SetDutyCycle(Volume_Percent);
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_PWM_Buzzer_Disable(uint32_t Id)
{
    BSP_MODULE_TRY_LOCK;
    BCDS_UNUSED(Id);
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_PWM_Buzzer)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {
            BSP_TIM1.Register = 0;

            retcode = BuzzerPWM_SetDutyCycle(0);
            if (RETCODE_OK == retcode)
            {
                if(HAL_OK != HAL_TIM_PWM_Stop(&BSP_TIM1Handle, BUZZER_PWM_TIMER_CHANNEL))
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
                }
            }

            /*
             * Disable the shared resources.
             */
            if (RETCODE_OK == retcode)
            {
                BSP_TIM1.Peripheral.Buzzer = 0;
                if (0 == BSP_TIM1.Register)
                {
                    __HAL_RCC_TIM1_CLK_DISABLE();
                }

                HAL_GPIO_WritePin(VBUZZER_EN_PORT, VBUZZER_EN_PIN, GPIO_PIN_RESET);
                ModuleState_PWM_Buzzer = BSP_MODULE_STATE_CONNECTED;
                retcode = BSP_LDO_TLV70233_Disable();
            }

            if (RETCODE_OK != retcode)
            {
                ModuleState_PWM_Buzzer = BSP_MODULE_STATE_ERROR;
            }
        }
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_PWM_Buzzer_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* @todo 2017.04.13 BCDS/ENG2: non-void return value has to be declared in HAL*/
    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_PWM_Buzzer)
    {
    case BSP_MODULE_STATE_CONNECTED:
        __HAL_RCC_GPIOE_CLK_ENABLE();

        HAL_GPIO_DeInit(BUZZER_PWM_PORT, BUZZER_PWM_PIN);
        HAL_GPIO_DeInit(VBUZZER_EN_PORT, VBUZZER_EN_PIN);
         /* @todo 2017.04.13 BCDS/ENG2: non-void return value has to be declared in HAL*/
        ModuleState_PWM_Buzzer = BSP_MODULE_STATE_DISCONNECTED;
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

#endif /* BCDS_FEATURE_BSP_PWM_BUZZER */
/**@}*//* @ingroup BSP_PWM_BUZZER */



