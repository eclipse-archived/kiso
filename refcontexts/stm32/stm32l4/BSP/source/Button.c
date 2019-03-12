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
 * @ingroup BSP_BUTTON
 * @{
 * @file
 * @brief  Implementation of Button BSP functions
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_BUTTON

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_Button.h"

#if BCDS_FEATURE_BSP_BUTTON

#include "BoardSettings.h"
#include "Button.h"
#include "BSP_BoardType.h"
#include "BSP_ModuleState.h"

typedef union TIM3Register_U TIM3Register_T;

static TIM_HandleTypeDef BSP_TIM3Handle;

static volatile enum BSP_ModuleState_E ModuleState_ResetButton = BSP_MODULE_STATE_UNINITIALIZED;
static volatile enum BSP_ModuleState_E ModuleState_ECallButton = BSP_MODULE_STATE_UNINITIALIZED;
static volatile enum BSP_ModuleState_E ModuleState_Button = BSP_MODULE_STATE_UNINITIALIZED;
static volatile uint8_t EnableCountReset;
static volatile uint8_t EnableCountECall;

/*
 * @brief Initializes the Button Timer
 * @details This function Initializes the timer resource in TimeBase mode. It is
 * called within the Board_Initialize() function. It stores the desired configuration
 * into the resource registers and puts it into low power consumption
 * configuration until it is requested by the application.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_TIM3_Init(void);

enum BSP_ModuleState_E Button_ResetButton_getState(void)
{
    return ModuleState_ResetButton;
}

enum BSP_ModuleState_E Button_ECallButton_getState(void)
{
    return ModuleState_ECallButton;
}

enum BSP_ModuleState_E Button_ModuleButton_getState(void)
{
    return ModuleState_Button;
}

Retcode_T Board_TIM3_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    __HAL_RCC_TIM3_CLK_ENABLE();

    BSP_TIM3Handle.Instance = BUTTON_TIMER;
    BSP_TIM3Handle.Init.Prescaler = BUTTON_TIMER_PRESCALER;
    BSP_TIM3Handle.Init.CounterMode = BUTTON_TIMER_COUNTER_MODE;
    BSP_TIM3Handle.Init.Period = BUTTON_DEBOUNCE_TIME_MS;
    BSP_TIM3Handle.Init.ClockDivision = BUTTON_TIMER_CLK_DIV;
    if (HAL_OK != HAL_TIM_Base_Init(&BSP_TIM3Handle))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    if (RETCODE_OK == retcode)
    {
        CLEAR_BIT(BUTTON_TIMER->SR, TIM_IT_UPDATE);
    }
    /* Disable the Clock */
    __HAL_RCC_TIM3_CLK_DISABLE();
    if (RETCODE_OK == retcode)
    {
        ModuleState_ResetButton = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_ECallButton = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_Button = BSP_MODULE_STATE_DISCONNECTED;
    }

    return retcode;
}

/* Trigger of the button ISR (input line or timer ) */
enum ButtonTrigger_E
{
    EXTI_FALLING, /* Trigger from external interrupt falling edge */
    EXTI_RISING, /* Trigger from external interrupt rising edge */
    TIM_UPDATE /* Trigger from timer update event */
};

static BSP_Button_Callback_T Button_EcallEvent;
static enum ButtonState_E EcallButtonState = BUTTON_NO_TRIGGER;

/**
 * This function checks the button level if low start timer if high stop timer
 * assess timer duration if greater than debounce time then short press
 * if timer overflows then long press.
 */
static void ECallButtonISRCallback(enum ButtonTrigger_E buttonEvent);

Retcode_T BSP_Button_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    GPIO_InitTypeDef GPIO_InitStruct;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_Button)
    {
    case BSP_MODULE_STATE_DISCONNECTED:
    case BSP_MODULE_STATE_ERROR: /*In case of error state we try to connect again to go back in a defined state, will always succeed*/
        /* Enable the clocks for the buttons */
        BUTTON_GPIO_CLOCK_ENABLE();

        /* Connect the reset button to the MCU */
        GPIO_InitStruct.Pin = SYS_RESET_BUTTON_PIN;
        GPIO_InitStruct.Mode = SYS_RESET_BUTTON_MODE;
        GPIO_InitStruct.Pull = SYS_RESET_BUTTON_PULL;
        GPIO_InitStruct.Speed = SYS_RESET_BUTTON_SPEED;
        HAL_GPIO_Init(SYS_RESET_BUTTON_PORT, &GPIO_InitStruct);

        /* Connect the Ecall Button to the MCU */
        GPIO_InitStruct.Pin = SYS_ECALL_BUTTON_PIN;
        GPIO_InitStruct.Mode = SYS_ECALL_BUTTON_MODE;
        GPIO_InitStruct.Pull = SYS_ECALL_BUTTON_PULL;
        GPIO_InitStruct.Speed = SYS_ECALL_BUTTON_SPEED;
        HAL_GPIO_Init(SYS_ECALL_BUTTON_PORT, &GPIO_InitStruct);

        EnableCountReset = 0;
        EnableCountECall = 0;
        ModuleState_Button = BSP_MODULE_STATE_CONNECTED;
        ModuleState_ResetButton = BSP_MODULE_STATE_CONNECTED;
        ModuleState_ECallButton = BSP_MODULE_STATE_CONNECTED;
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Button_Enable(uint32_t ButtonId, BSP_Button_Callback_T callback)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    if (!(IS_BUTTON((BSP_Button_T )ButtonId)))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        /* Enable the Timer for press duration measurement */
        __HAL_RCC_TIM3_CLK_ENABLE();

        switch(ButtonId)
        {
        case BSP_BUTTON_ECALL:
            switch(ModuleState_ECallButton)
            {
            case BSP_MODULE_STATE_CONNECTED:

                if (NULL != callback)
                {
                    /* Assign the callback to the E-call event */
                    Button_EcallEvent = callback;
                    /* Configure the external interrupt line 1 used for E-Call */
                    HAL_NVIC_SetPriority(BUTTON_EXTI_IRQN, BUTTON_EXTI_PREEMPT_PRIORITY, BUTTON_EXTI_SUBPRIORITY);
                    /* Configure the NVIC for button timer */
                    HAL_NVIC_SetPriority(BUTTON_TIMER_IRQN, BUTTON_TIMER_PREEMPT_PRIORITY, BUTTON_TIMER_SUBPRIORITY);
                    /* Enable the External Interrupt Interrupt  for the button */
                    HAL_NVIC_EnableIRQ(BUTTON_EXTI_IRQN);
                    HAL_NVIC_EnableIRQ(BUTTON_TIMER_IRQN);
                }
                EnableCountECall++;
                assert(EnableCountECall);
                retcode = RETCODE_OK;
                ModuleState_ECallButton = BSP_MODULE_STATE_ENABLED;
                ModuleState_Button = BSP_MODULE_STATE_ENABLED;

                break;
            case BSP_MODULE_STATE_ENABLED:
                EnableCountECall++;
                assert(EnableCountECall);
                retcode = RETCODE_OK;
                break;
            case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
            case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
            case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
                break;
            }
            break;
        case BSP_BUTTON_RESET:
            switch(ModuleState_ResetButton)
            {
            case BSP_MODULE_STATE_CONNECTED:
                EnableCountReset++;
                assert(EnableCountReset);
                retcode = RETCODE_OK;
                ModuleState_ResetButton = BSP_MODULE_STATE_ENABLED;
                ModuleState_Button = BSP_MODULE_STATE_ENABLED;
                break;
            case BSP_MODULE_STATE_ENABLED:
                EnableCountReset++;
                assert(EnableCountReset);
                retcode = RETCODE_OK;
                break;
            case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
            case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
            case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
                break;
            }
            break;
        }

    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Button_Disable(uint32_t ButtonId)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    if (!(IS_BUTTON((BSP_Button_T )ButtonId)))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        switch(ButtonId)
        {
        case BSP_BUTTON_ECALL:
            switch(ModuleState_ECallButton)
            {
            case BSP_MODULE_STATE_CONNECTED:
                retcode = RETCODE_OK;
                break;
            case BSP_MODULE_STATE_ENABLED:
                assert(EnableCountECall);

                EnableCountECall--;

                if (0 == EnableCountECall)
                {
                    /* Disable the Timer */
                    __HAL_RCC_TIM3_CLK_DISABLE();
                    /* Enable the Interrupts  for the button */
                    HAL_NVIC_DisableIRQ(BUTTON_EXTI_IRQN);
                    HAL_NVIC_DisableIRQ(BUTTON_TIMER_IRQN);
                    /* Reset the callback pointer */
                    Button_EcallEvent = NULL;

                    retcode = RETCODE_OK;
                    ModuleState_ECallButton = BSP_MODULE_STATE_CONNECTED;
                }

                break;
            case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
            case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
            case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
                break;
            }
            break;
        case BSP_BUTTON_RESET:
            switch(ModuleState_ResetButton)
            {
            case BSP_MODULE_STATE_CONNECTED:
                retcode = RETCODE_OK;
                break;
            case BSP_MODULE_STATE_ENABLED:
                retcode = RETCODE_OK;
                assert(EnableCountReset);

                EnableCountReset--;

                if (0 == EnableCountReset)
                {
                    ModuleState_ResetButton = BSP_MODULE_STATE_CONNECTED;
                }
                break;
            case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
            case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
            case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
                break;
            }
            break;
        }

        if ((ModuleState_ResetButton == BSP_MODULE_STATE_CONNECTED) && (ModuleState_ECallButton == BSP_MODULE_STATE_CONNECTED))
        {
            ModuleState_Button = BSP_MODULE_STATE_CONNECTED;
        }
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_Button_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    switch(ModuleState_Button)
    {
    case BSP_MODULE_STATE_CONNECTED:
        HAL_GPIO_DeInit(SYS_RESET_BUTTON_PORT, SYS_RESET_BUTTON_PIN);
        HAL_GPIO_DeInit(SYS_ECALL_BUTTON_PORT, SYS_ECALL_BUTTON_PIN);

        ModuleState_Button = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_ResetButton = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_ECallButton = BSP_MODULE_STATE_DISCONNECTED;
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

uint32_t BSP_Button_GetState(uint32_t buttonId)
{
    ButtonState_T buttonState = BUTTON_RELEASED;

    assert(IS_BUTTON((BSP_Button_T)buttonId));

    if (BSP_BUTTON_ECALL == (BSP_Button_T) buttonId)
    {
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(SYS_ECALL_BUTTON_PORT, SYS_ECALL_BUTTON_PIN))
        {
            buttonState = BUTTON_PRESSED;
        }
        else
        {
            buttonState = BUTTON_RELEASED;
        }
    }
    else if (BSP_BUTTON_RESET == (BSP_Button_T) buttonId)
    {
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(SYS_RESET_BUTTON_PORT, SYS_RESET_BUTTON_PIN))
        {
            buttonState = BUTTON_PRESSED;
        }
        else
        {
            buttonState = BUTTON_RELEASED;
        }
    }
    return (uint32_t) buttonState;
}

void Button_TIM3ISR(void)
{
    /* Check if TIM Update event */
    if ((BUTTON_TIMER->DIER & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE))
    {
        /* Checkout interrupt */
        if ((BUTTON_TIMER->DIER & (TIM_IT_UPDATE)) == (TIM_IT_UPDATE))
        {
            /* Reset The interrupt flag */
            BUTTON_TIMER->SR &= ~(TIM_IT_UPDATE);
            ECallButtonISRCallback(TIM_UPDATE);

        }
    }
}

void Button_EXTI1ISR(void)
{
    /* Check if the external interrupt comes from ECALL button */
    if (__HAL_GPIO_EXTI_GET_IT(SYS_ECALL_BUTTON_PIN) != (uint32_t) RESET)
    {
        /* Clear the interrupt flag */
        __HAL_GPIO_EXTI_CLEAR_IT(SYS_ECALL_BUTTON_PIN);
        /* Call the service routine for the ECALL button */
        if (HAL_GPIO_ReadPin(SYS_ECALL_BUTTON_PORT, SYS_ECALL_BUTTON_PIN) == GPIO_PIN_RESET)
        {
            ECallButtonISRCallback(EXTI_FALLING);
        }
        else
        {
            ECallButtonISRCallback(EXTI_RISING);
        }
    }
}

static void ECallButtonISRCallback(enum ButtonTrigger_E buttonTrigger)
{

    if ((EXTI_FALLING == buttonTrigger) && (EcallButtonState == BUTTON_NO_TRIGGER))
    {
        EcallButtonState = BUTTON_PRESSED;
    }
    else if ((TIM_UPDATE == buttonTrigger) && (EcallButtonState == BUTTON_PRESSED))
    {
        BUTTON_TIMER->ARR = BUTTON_LONG_PRESS_MS;
        EcallButtonState = BUTTON_DEBOUNCED;
        Button_EcallEvent((uint32_t) BSP_BUTTON_PRESS);

    }
    else if ((TIM_UPDATE == buttonTrigger) && (EcallButtonState == BUTTON_DEBOUNCED))
    {
        BUTTON_TIMER->ARR = BUTTON_DEBOUNCE_TIME_MS;
        EcallButtonState = BUTTON_PRESSED_LONG;
        Button_EcallEvent((uint32_t) BSP_BUTTON_LONG_PRESS);
    }
    else if ((EXTI_RISING == buttonTrigger))
    {
        BUTTON_TIMER->ARR = BUTTON_DEBOUNCE_TIME_MS;
        if (EcallButtonState == BUTTON_DEBOUNCED)
        {
            EcallButtonState = BUTTON_RELEASED;
            Button_EcallEvent((uint32_t) BSP_BUTTON_SHORT_PRESS);
        }
        else
        {
            EcallButtonState = BUTTON_RELEASED;
        }
    }

    switch (EcallButtonState)
    {

    case BUTTON_PRESSED:
        /* Update the Prescaler for 1ms timer Tick */
        SystemCoreClockUpdate();
        BUTTON_TIMER->PSC = (SystemCoreClock / 1000) - 1;
        /* Reset the counter to zero */
        BUTTON_TIMER->CNT &= 0x0000;
        /* Enable the TIM Update interrupt */
        BUTTON_TIMER->DIER |= TIM_IT_UPDATE;
        /* Enable the Timer Peripheral */
        BUTTON_TIMER->CR1 |= TIM_CR1_CEN;
        /* Prepare register for the next update event */
        BUTTON_TIMER->ARR = BUTTON_DEBOUNCE_TIME_MS;
        break;
    case BUTTON_RELEASED:
        /* Disable the Timer Peripheral */
        BUTTON_TIMER->CR1 &= ~(TIM_CR1_CEN);
        EcallButtonState = BUTTON_NO_TRIGGER;
        break;
    case BUTTON_PRESSED_LONG:
        /* Disable the Timer Peripheral */
        BUTTON_TIMER->CR1 &= ~(TIM_CR1_CEN);
        EcallButtonState = BUTTON_NO_TRIGGER;
        break;
    case BUTTON_NO_TRIGGER:
        break;
    case BUTTON_DEBOUNCED:
        break;
    default:
        break;
    }
}

#endif /* BCDS_FEATURE_BSP_BUTTON */
/**@}*//* @ingroup BSP_BUTTON */
