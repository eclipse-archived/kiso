/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) 2015-2018 Bosch Connected Devices and Solutions GmbH. 
 * Copyright (C) 2019 Robert Bosch GmbH.
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
 * @file
 * @brief Add a brief description here.
 *
 * @details Put here the documentation of this header file. Explain the interface exposed
 * by this header, e.g. what is the purpose of use, how to use it, etc.
 */
#ifndef KISO_MCU_STM32F7_TIMER_HANDLE_H_
#define KISO_MCU_STM32F7_TIMER_HANDLE_H_

/* Include all headers which are needed by this file. */
#if KISO_FEATURE_TIMER

/* Include all headers which are needed by this file. */

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_tim.h"

/* Put the type and macro definitions here */

/**
 * @brief This data type represents a function pointer which is used between
 * BSP and MCU SPI as a callback whenever an IRQ event is to be notified from
 * the BSP to the Timer.
 *
 * @param [in] timer : Handle of the TIMER whose IRQ event should be handled.
 */
typedef void (*MCU_BSP_TIMER_IRQ_Callback_T)(Timer_T timer);

/**
 * STM32 Specific Timer Mode
 */

enum MCU_TimerMode_E
{
    STM_TIMER_MODE_BLOCKING,  /**< Standard Base Timer */
    STM_TIMER_MODE_INTERRUPT, /**< INTERRUPT Timer  */
    STM_TIMER_MODE_DMA,       /**< DMA Timer  */
};

/**
 * STM32 Specific Timer Handle
 */
struct MCU_Timer_Handle_S
{
    TIM_HandleTypeDef TimerHandle;
    MCU_BSP_TIMER_IRQ_Callback_T IRQCallback;
    enum MCU_TimerMode_E TimMode;
    enum MCU_Timer_Status_E Status;   /**< Current state of the timer as tracked by the SW */
    MCU_TIMER_Callback_T AppCallback; /**< Callback for timer */

    /** Function pointer to the vendor library Timer Start function. Will be set
     *  upon initialization. */
    HAL_StatusTypeDef (*TimerStartFunPtr)(TIM_HandleTypeDef *htim);

    /** Function pointer to the vendor library Timer Stop function. Will be set upon
     *  initialization.*/
    HAL_StatusTypeDef (*TimerStopFunPtr)(TIM_HandleTypeDef *htim);
};

/* Put the function declarations here */

#endif /* KISO_FEATURE_TIMER */

#endif /* KISO_MCU_STM32F7_TIMER_HANDLE_H_ */
