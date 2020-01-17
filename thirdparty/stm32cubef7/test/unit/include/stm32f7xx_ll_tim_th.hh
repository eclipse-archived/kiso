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
 * Mock-up implementation for the stm32f7xx_ll_tim module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_TIM_TH_HH
#define STM32F7XX_LL_TIM_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_TIM_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief  TIM Time Base configuration structure definition.
  */
typedef struct
{
  uint16_t Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
                                   This parameter can be a number between Min_Data=0x0000 and Max_Data=0xFFFF.

                                   This feature can be modified afterwards using unitary function @ref LL_TIM_SetPrescaler().*/

  uint32_t CounterMode;       /*!< Specifies the counter mode.
                                   This parameter can be a value of @ref TIM_LL_EC_COUNTERMODE.

                                   This feature can be modified afterwards using unitary function @ref LL_TIM_SetCounterMode().*/

  uint32_t Autoreload;        /*!< Specifies the auto reload value to be loaded into the active
                                   Auto-Reload Register at the next update event.
                                   This parameter must be a number between Min_Data=0x0000 and Max_Data=0xFFFF.
                                   Some timer instances may support 32 bits counters. In that case this parameter must be a number between 0x0000 and 0xFFFFFFFF.

                                   This feature can be modified afterwards using unitary function @ref LL_TIM_SetAutoReload().*/

  uint32_t ClockDivision;     /*!< Specifies the clock division.
                                   This parameter can be a value of @ref TIM_LL_EC_CLOCKDIVISION.

                                   This feature can be modified afterwards using unitary function @ref LL_TIM_SetClockDivision().*/

  uint8_t RepetitionCounter;  /*!< Specifies the repetition counter value. Each time the RCR downcounter
                                   reaches zero, an update event is generated and counting restarts
                                   from the RCR value (N).
                                   This means in PWM mode that (N+1) corresponds to:
                                      - the number of PWM periods in edge-aligned mode
                                      - the number of half PWM period in center-aligned mode
                                   This parameter must be a number between 0x00 and 0xFF.

                                   This feature can be modified afterwards using unitary function @ref LL_TIM_SetRepetitionCounter().*/
} LL_TIM_InitTypeDef;

/**
  * @brief  TIM Output Compare configuration structure definition.
  */
typedef struct
{
  uint32_t OCMode;        /*!< Specifies the output mode.
                               This parameter can be a value of @ref TIM_LL_EC_OCMODE.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_OC_SetMode().*/

  uint32_t OCState;       /*!< Specifies the TIM Output Compare state.
                               This parameter can be a value of @ref TIM_LL_EC_OCSTATE.

                               This feature can be modified afterwards using unitary functions @ref LL_TIM_CC_EnableChannel() or @ref LL_TIM_CC_DisableChannel().*/

  uint32_t OCNState;      /*!< Specifies the TIM complementary Output Compare state.
                               This parameter can be a value of @ref TIM_LL_EC_OCSTATE.

                               This feature can be modified afterwards using unitary functions @ref LL_TIM_CC_EnableChannel() or @ref LL_TIM_CC_DisableChannel().*/

  uint32_t CompareValue;  /*!< Specifies the Compare value to be loaded into the Capture Compare Register.
                               This parameter can be a number between Min_Data=0x0000 and Max_Data=0xFFFF.

                               This feature can be modified afterwards using unitary function LL_TIM_OC_SetCompareCHx (x=1..6).*/

  uint32_t OCPolarity;    /*!< Specifies the output polarity.
                               This parameter can be a value of @ref TIM_LL_EC_OCPOLARITY.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_OC_SetPolarity().*/

  uint32_t OCNPolarity;   /*!< Specifies the complementary output polarity.
                               This parameter can be a value of @ref TIM_LL_EC_OCPOLARITY.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_OC_SetPolarity().*/


  uint32_t OCIdleState;   /*!< Specifies the TIM Output Compare pin state during Idle state.
                               This parameter can be a value of @ref TIM_LL_EC_OCIDLESTATE.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_OC_SetIdleState().*/

  uint32_t OCNIdleState;  /*!< Specifies the TIM Output Compare pin state during Idle state.
                               This parameter can be a value of @ref TIM_LL_EC_OCIDLESTATE.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_OC_SetIdleState().*/
} LL_TIM_OC_InitTypeDef;

/**
  * @brief  TIM Input Capture configuration structure definition.
  */

typedef struct
{

  uint32_t ICPolarity;    /*!< Specifies the active edge of the input signal.
                               This parameter can be a value of @ref TIM_LL_EC_IC_POLARITY.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetPolarity().*/

  uint32_t ICActiveInput; /*!< Specifies the input.
                               This parameter can be a value of @ref TIM_LL_EC_ACTIVEINPUT.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetActiveInput().*/

  uint32_t ICPrescaler;   /*!< Specifies the Input Capture Prescaler.
                               This parameter can be a value of @ref TIM_LL_EC_ICPSC.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetPrescaler().*/

  uint32_t ICFilter;      /*!< Specifies the input capture filter.
                               This parameter can be a value of @ref TIM_LL_EC_IC_FILTER.

                               This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetFilter().*/
} LL_TIM_IC_InitTypeDef;


/**
  * @brief  TIM Encoder interface configuration structure definition.
  */
typedef struct
{
  uint32_t EncoderMode;     /*!< Specifies the encoder resolution (x2 or x4).
                                 This parameter can be a value of @ref TIM_LL_EC_ENCODERMODE.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_SetEncoderMode().*/

  uint32_t IC1Polarity;     /*!< Specifies the active edge of TI1 input.
                                 This parameter can be a value of @ref TIM_LL_EC_IC_POLARITY.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetPolarity().*/

  uint32_t IC1ActiveInput;  /*!< Specifies the TI1 input source
                                 This parameter can be a value of @ref TIM_LL_EC_ACTIVEINPUT.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetActiveInput().*/

  uint32_t IC1Prescaler;    /*!< Specifies the TI1 input prescaler value.
                                 This parameter can be a value of @ref TIM_LL_EC_ICPSC.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetPrescaler().*/

  uint32_t IC1Filter;       /*!< Specifies the TI1 input filter.
                                 This parameter can be a value of @ref TIM_LL_EC_IC_FILTER.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetFilter().*/

  uint32_t IC2Polarity;      /*!< Specifies the active edge of TI2 input.
                                 This parameter can be a value of @ref TIM_LL_EC_IC_POLARITY.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetPolarity().*/

  uint32_t IC2ActiveInput;  /*!< Specifies the TI2 input source
                                 This parameter can be a value of @ref TIM_LL_EC_ACTIVEINPUT.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetActiveInput().*/

  uint32_t IC2Prescaler;    /*!< Specifies the TI2 input prescaler value.
                                 This parameter can be a value of @ref TIM_LL_EC_ICPSC.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetPrescaler().*/

  uint32_t IC2Filter;       /*!< Specifies the TI2 input filter.
                                 This parameter can be a value of @ref TIM_LL_EC_IC_FILTER.

                                 This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetFilter().*/

} LL_TIM_ENCODER_InitTypeDef;

/**
  * @brief  TIM Hall sensor interface configuration structure definition.
  */
typedef struct
{

  uint32_t IC1Polarity;        /*!< Specifies the active edge of TI1 input.
                                    This parameter can be a value of @ref TIM_LL_EC_IC_POLARITY.

                                    This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetPolarity().*/

  uint32_t IC1Prescaler;       /*!< Specifies the TI1 input prescaler value.
                                    Prescaler must be set to get a maximum counter period longer than the
                                    time interval between 2 consecutive changes on the Hall inputs.
                                    This parameter can be a value of @ref TIM_LL_EC_ICPSC.

                                    This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetPrescaler().*/

  uint32_t IC1Filter;          /*!< Specifies the TI1 input filter.
                                    This parameter can be a value of @ref TIM_LL_EC_IC_FILTER.

                                    This feature can be modified afterwards using unitary function @ref LL_TIM_IC_SetFilter().*/

  uint32_t CommutationDelay;   /*!< Specifies the compare value to be loaded into the Capture Compare Register.
                                    A positive pulse (TRGO event) is generated with a programmable delay every time
                                    a change occurs on the Hall inputs.
                                    This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF.

                                    This feature can be modified afterwards using unitary function @ref LL_TIM_OC_SetCompareCH2().*/
} LL_TIM_HALLSENSOR_InitTypeDef;

/**
  * @brief  BDTR (Break and Dead Time) structure definition
  */
typedef struct
{
  uint32_t OSSRState;            /*!< Specifies the Off-State selection used in Run mode.
                                      This parameter can be a value of @ref TIM_LL_EC_OSSR

                                      This feature can be modified afterwards using unitary function @ref LL_TIM_SetOffStates()

                                      @note This bit-field cannot be modified as long as LOCK level 2 has been programmed. */

  uint32_t OSSIState;            /*!< Specifies the Off-State used in Idle state.
                                      This parameter can be a value of @ref TIM_LL_EC_OSSI

                                      This feature can be modified afterwards using unitary function @ref LL_TIM_SetOffStates()

                                      @note This bit-field cannot be modified as long as LOCK level 2 has been programmed. */

  uint32_t LockLevel;            /*!< Specifies the LOCK level parameters.
                                      This parameter can be a value of @ref TIM_LL_EC_LOCKLEVEL

                                      @note The LOCK bits can be written only once after the reset. Once the TIMx_BDTR register
                                            has been written, their content is frozen until the next reset.*/

  uint8_t DeadTime;              /*!< Specifies the delay time between the switching-off and the
                                      switching-on of the outputs.
                                      This parameter can be a number between Min_Data = 0x00 and Max_Data = 0xFF.

                                      This feature can be modified afterwards using unitary function @ref LL_TIM_OC_SetDeadTime()

                                      @note This bit-field can not be modified as long as LOCK level 1, 2 or 3 has been programmed. */

  uint16_t BreakState;           /*!< Specifies whether the TIM Break input is enabled or not.
                                      This parameter can be a value of @ref TIM_LL_EC_BREAK_ENABLE

                                      This feature can be modified afterwards using unitary functions @ref LL_TIM_EnableBRK() or @ref LL_TIM_DisableBRK()

                                      @note This bit-field can not be modified as long as LOCK level 1 has been programmed. */

  uint32_t BreakPolarity;        /*!< Specifies the TIM Break Input pin polarity.
                                      This parameter can be a value of @ref TIM_LL_EC_BREAK_POLARITY

                                      This feature can be modified afterwards using unitary function @ref LL_TIM_ConfigBRK()

                                      @note This bit-field can not be modified as long as LOCK level 1 has been programmed. */

  uint32_t BreakFilter;          /*!< Specifies the TIM Break Filter.
                                      This parameter can be a value of @ref TIM_LL_EC_BREAK_FILTER

                                      This feature can be modified afterwards using unitary function @ref LL_TIM_ConfigBRK()

                                      @note This bit-field can not be modified as long as LOCK level 1 has been programmed. */

  uint32_t Break2State;          /*!< Specifies whether the TIM Break2 input is enabled or not.
                                      This parameter can be a value of @ref TIM_LL_EC_BREAK2_ENABLE

                                      This feature can be modified afterwards using unitary functions @ref LL_TIM_EnableBRK2() or @ref LL_TIM_DisableBRK2()

                                      @note This bit-field can not be modified as long as LOCK level 1 has been programmed. */

  uint32_t Break2Polarity;        /*!< Specifies the TIM Break2 Input pin polarity.
                                      This parameter can be a value of @ref TIM_LL_EC_BREAK2_POLARITY

                                      This feature can be modified afterwards using unitary function @ref LL_TIM_ConfigBRK2()

                                      @note This bit-field can not be modified as long as LOCK level 1 has been programmed. */

  uint32_t Break2Filter;          /*!< Specifies the TIM Break2 Filter.
                                      This parameter can be a value of @ref TIM_LL_EC_BREAK2_FILTER

                                      This feature can be modified afterwards using unitary function @ref LL_TIM_ConfigBRK2()

                                      @note This bit-field can not be modified as long as LOCK level 1 has been programmed. */

  uint32_t AutomaticOutput;      /*!< Specifies whether the TIM Automatic Output feature is enabled or not.
                                      This parameter can be a value of @ref TIM_LL_EC_AUTOMATICOUTPUT_ENABLE

                                      This feature can be modified afterwards using unitary functions @ref LL_TIM_EnableAutomaticOutput() or @ref LL_TIM_DisableAutomaticOutput()

                                      @note This bit-field can not be modified as long as LOCK level 1 has been programmed. */
} LL_TIM_BDTR_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_TIM_EnableCounter, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableCounter, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledCounter, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableUpdateEvent, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableUpdateEvent, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledUpdateEvent, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetUpdateSource, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetUpdateSource, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetOnePulseMode, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetOnePulseMode, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetCounterMode, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetCounterMode, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableARRPreload, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableARRPreload, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledARRPreload, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetClockDivision, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetClockDivision, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetCounter, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetCounter, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetDirection, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetPrescaler, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetPrescaler, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetAutoReload, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetAutoReload, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetRepetitionCounter, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_GetRepetitionCounter, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableUIFRemap, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableUIFRemap, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_CC_EnablePreload, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_CC_DisablePreload, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_CC_SetUpdate, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_CC_SetDMAReqTrigger, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_CC_GetDMAReqTrigger, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_CC_SetLockLevel, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_CC_EnableChannel, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_CC_DisableChannel, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_CC_IsEnabledChannel, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_ConfigOutput, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetMode, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetMode, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetPolarity, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetPolarity, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetIdleState, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetIdleState, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_EnableFast, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_DisableFast, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_IsEnabledFast, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_EnablePreload, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_DisablePreload, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_IsEnabledPreload, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_EnableClear, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_DisableClear, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_IsEnabledClear, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetDeadTime, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetCompareCH1, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetCompareCH2, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetCompareCH3, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetCompareCH4, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetCompareCH5, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_OC_SetCompareCH6, TIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetCompareCH1, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetCompareCH2, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetCompareCH3, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetCompareCH4, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetCompareCH5, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_OC_GetCompareCH6, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetCH5CombinedChannels, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_IC_Config, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_TIM_IC_SetActiveInput, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_GetActiveInput, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_IC_SetPrescaler, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_GetPrescaler, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_IC_SetFilter, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_GetFilter, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_IC_SetPolarity, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_GetPolarity, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_IC_EnableXORCombination, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_IC_DisableXORCombination, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_IsEnabledXORCombination, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_GetCaptureCH1, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_GetCaptureCH2, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_GetCaptureCH3, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IC_GetCaptureCH4, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableExternalClock, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableExternalClock, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledExternalClock, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_SetClockSource, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_SetEncoderMode, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_SetTriggerOutput, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_SetTriggerOutput2, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_SetSlaveMode, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_SetTriggerInput, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_EnableMasterSlaveMode, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableMasterSlaveMode, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledMasterSlaveMode, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableBRK, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableBRK, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ConfigBRK, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_TIM_EnableBRK2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableBRK2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ConfigBRK2, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_TIM_SetOffStates, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_TIM_EnableAutomaticOutput, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableAutomaticOutput, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledAutomaticOutput, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableAllOutputs, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableAllOutputs, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledAllOutputs, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableBreakInputSource, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_TIM_DisableBreakInputSource, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_TIM_ConfigDMABurst, TIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_TIM_SetRemap, TIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_UPDATE, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_UPDATE, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC1, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC1, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC2, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC3, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC3, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC4, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC4, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC5, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC5, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC6, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC6, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_COM, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_COM, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_TRIG, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_TRIG, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_BRK, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_BRK, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_BRK2, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_BRK2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC1OVR, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC1OVR, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC2OVR, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC2OVR, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC3OVR, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC3OVR, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_CC4OVR, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_CC4OVR, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_ClearFlag_SYSBRK, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsActiveFlag_SYSBRK, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableIT_UPDATE, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableIT_UPDATE, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledIT_UPDATE, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableIT_CC1, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableIT_CC1, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledIT_CC1, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableIT_CC2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableIT_CC2, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledIT_CC2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableIT_CC3, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableIT_CC3, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledIT_CC3, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableIT_CC4, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableIT_CC4, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledIT_CC4, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableIT_COM, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableIT_COM, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledIT_COM, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableIT_TRIG, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableIT_TRIG, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledIT_TRIG, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableIT_BRK, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableIT_BRK, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledIT_BRK, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableDMAReq_UPDATE, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableDMAReq_UPDATE, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledDMAReq_UPDATE, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableDMAReq_CC1, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableDMAReq_CC1, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledDMAReq_CC1, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableDMAReq_CC2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableDMAReq_CC2, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledDMAReq_CC2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableDMAReq_CC3, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableDMAReq_CC3, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledDMAReq_CC3, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableDMAReq_CC4, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableDMAReq_CC4, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledDMAReq_CC4, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableDMAReq_COM, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableDMAReq_COM, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledDMAReq_COM, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_EnableDMAReq_TRIG, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_DisableDMAReq_TRIG, TIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_TIM_IsEnabledDMAReq_TRIG, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_UPDATE, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_CC1, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_CC2, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_CC3, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_CC4, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_COM, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_TRIG, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_BRK, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_GenerateEvent_BRK2, TIM_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_TIM_DeInit, TIM_TypeDef*);

FAKE_VOID_FUNC(LL_TIM_StructInit, LL_TIM_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_TIM_Init, TIM_TypeDef*, LL_TIM_InitTypeDef*);

FAKE_VOID_FUNC(LL_TIM_OC_StructInit, LL_TIM_OC_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_TIM_OC_Init, TIM_TypeDef*, uint32_t, LL_TIM_OC_InitTypeDef*);

FAKE_VOID_FUNC(LL_TIM_IC_StructInit, LL_TIM_IC_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_TIM_IC_Init, TIM_TypeDef*, uint32_t, LL_TIM_IC_InitTypeDef*);

FAKE_VOID_FUNC(LL_TIM_ENCODER_StructInit, LL_TIM_ENCODER_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_TIM_ENCODER_Init, TIM_TypeDef*, LL_TIM_ENCODER_InitTypeDef*);

FAKE_VOID_FUNC(LL_TIM_HALLSENSOR_StructInit, LL_TIM_HALLSENSOR_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_TIM_HALLSENSOR_Init, TIM_TypeDef*, LL_TIM_HALLSENSOR_InitTypeDef*);

FAKE_VOID_FUNC(LL_TIM_BDTR_StructInit, LL_TIM_BDTR_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_TIM_BDTR_Init, TIM_TypeDef*, LL_TIM_BDTR_InitTypeDef*);

# endif /* STM32F7XX_LL_TIM_TH_HH */
