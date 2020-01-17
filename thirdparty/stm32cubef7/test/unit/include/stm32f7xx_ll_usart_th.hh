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
 * Mock-up implementation for the stm32f7xx_ll_usart module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_USART_TH_HH
#define STM32F7XX_LL_USART_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_USART_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief LL USART Init Structure definition
  */
typedef struct
{
  uint32_t BaudRate;                  /*!< This field defines expected Usart communication baud rate.

                                           This feature can be modified afterwards using unitary function @ref LL_USART_SetBaudRate().*/

  uint32_t DataWidth;                 /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref USART_LL_EC_DATAWIDTH.

                                           This feature can be modified afterwards using unitary function @ref LL_USART_SetDataWidth().*/

  uint32_t StopBits;                  /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref USART_LL_EC_STOPBITS.

                                           This feature can be modified afterwards using unitary function @ref LL_USART_SetStopBitsLength().*/

  uint32_t Parity;                    /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref USART_LL_EC_PARITY.

                                           This feature can be modified afterwards using unitary function @ref LL_USART_SetParity().*/

  uint32_t TransferDirection;         /*!< Specifies whether the Receive and/or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref USART_LL_EC_DIRECTION.

                                           This feature can be modified afterwards using unitary function @ref LL_USART_SetTransferDirection().*/

  uint32_t HardwareFlowControl;       /*!< Specifies whether the hardware flow control mode is enabled or disabled.
                                           This parameter can be a value of @ref USART_LL_EC_HWCONTROL.

                                           This feature can be modified afterwards using unitary function @ref LL_USART_SetHWFlowCtrl().*/

  uint32_t OverSampling;              /*!< Specifies whether USART oversampling mode is 16 or 8.
                                           This parameter can be a value of @ref USART_LL_EC_OVERSAMPLING.

                                           This feature can be modified afterwards using unitary function @ref LL_USART_SetOverSampling().*/

} LL_USART_InitTypeDef;

/**
  * @brief LL USART Clock Init Structure definition
  */
typedef struct
{
  uint32_t ClockOutput;               /*!< Specifies whether the USART clock is enabled or disabled.
                                           This parameter can be a value of @ref USART_LL_EC_CLOCK.

                                           USART HW configuration can be modified afterwards using unitary functions
                                           @ref LL_USART_EnableSCLKOutput() or @ref LL_USART_DisableSCLKOutput().
                                           For more details, refer to description of this function. */

  uint32_t ClockPolarity;             /*!< Specifies the steady state of the serial clock.
                                           This parameter can be a value of @ref USART_LL_EC_POLARITY.

                                           USART HW configuration can be modified afterwards using unitary functions @ref LL_USART_SetClockPolarity().
                                           For more details, refer to description of this function. */

  uint32_t ClockPhase;                /*!< Specifies the clock transition on which the bit capture is made.
                                           This parameter can be a value of @ref USART_LL_EC_PHASE.

                                           USART HW configuration can be modified afterwards using unitary functions @ref LL_USART_SetClockPhase().
                                           For more details, refer to description of this function. */

  uint32_t LastBitClockPulse;         /*!< Specifies whether the clock pulse corresponding to the last transmitted
                                           data bit (MSB) has to be output on the SCLK pin in synchronous mode.
                                           This parameter can be a value of @ref USART_LL_EC_LASTCLKPULSE.

                                           USART HW configuration can be modified afterwards using unitary functions @ref LL_USART_SetLastClkPulseOutput().
                                           For more details, refer to description of this function. */

} LL_USART_ClockInitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_USART_Enable, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_Disable, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabled, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableDirectionRx, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableDirectionRx, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableDirectionTx, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableDirectionTx, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetTransferDirection, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetTransferDirection, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetParity, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetParity, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetWakeUpMethod, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetWakeUpMethod, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetDataWidth, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetDataWidth, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableMuteMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableMuteMode, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledMuteMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetOverSampling, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetOverSampling, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetLastClkPulseOutput, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetLastClkPulseOutput, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetClockPhase, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetClockPhase, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetClockPolarity, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetClockPolarity, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigClock, USART_TypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_USART_EnableSCLKOutput, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableSCLKOutput, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledSCLKOutput, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetStopBitsLength, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetStopBitsLength, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetTXRXSwap, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetTXRXSwap, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetRXPinLevel, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetRXPinLevel, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetTXPinLevel, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetTXPinLevel, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetBinaryDataLogic, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetBinaryDataLogic, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetTransferBitOrder, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetTransferBitOrder, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableAutoBaudRate, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableAutoBaudRate, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledAutoBaud, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetAutoBaudRateMode, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetAutoBaudRateMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableRxTimeout, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableRxTimeout, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledRxTimeout, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigNodeAddress, USART_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetNodeAddress, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetNodeAddressLen, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableRTSHWFlowCtrl, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableRTSHWFlowCtrl, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableCTSHWFlowCtrl, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableCTSHWFlowCtrl, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetHWFlowCtrl, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetHWFlowCtrl, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableOneBitSamp, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableOneBitSamp, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledOneBitSamp, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableOverrunDetect, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableOverrunDetect, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledOverrunDetect, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetBaudRate, USART_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_USART_SetRxTimeout, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetRxTimeout, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetBlockLength, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetBlockLength, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIrda, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIrda, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIrda, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetIrdaPowerMode, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetIrdaPowerMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetIrdaPrescaler, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetIrdaPrescaler, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableSmartcardNACK, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableSmartcardNACK, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledSmartcardNACK, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableSmartcard, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableSmartcard, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledSmartcard, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetSmartcardAutoRetryCount, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetSmartcardAutoRetryCount, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetSmartcardPrescaler, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetSmartcardPrescaler, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetSmartcardGuardTime, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetSmartcardGuardTime, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableHalfDuplex, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableHalfDuplex, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledHalfDuplex, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetLINBrkDetectionLen, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetLINBrkDetectionLen, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableLIN, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableLIN, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledLIN, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetDEDeassertionTime, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetDEDeassertionTime, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetDEAssertionTime, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetDEAssertionTime, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableDEMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableDEMode, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledDEMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_SetDESignalPolarity, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_USART_GetDESignalPolarity, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigAsyncMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigSyncMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigLINMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigHalfDuplexMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigSmartcardMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigIrdaMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ConfigMultiProcessMode, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_PE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_FE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_NE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_ORE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_IDLE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_RXNE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_TC, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_TXE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_LBD, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_nCTS, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_CTS, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_RTO, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_EOB, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_ABRE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_ABR, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_BUSY, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_CM, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_SBK, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_RWU, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_TEACK, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsActiveFlag_TCBGT, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_PE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_FE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_NE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_ORE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_IDLE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_TC, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_TCBGT, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_LBD, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_nCTS, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_RTO, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_EOB, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_ClearFlag_CM, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_IDLE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_RXNE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_TC, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_TXE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_PE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_CM, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_RTO, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_EOB, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_LBD, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_ERROR, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_CTS, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableIT_TCBGT, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_IDLE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_RXNE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_TC, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_TXE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_PE, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_CM, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_RTO, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_EOB, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_LBD, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_ERROR, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_CTS, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableIT_TCBGT, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_IDLE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_RXNE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_TC, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_TXE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_PE, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_CM, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_RTO, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_EOB, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_LBD, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_ERROR, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_CTS, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledIT_TCBGT, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableDMAReq_RX, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableDMAReq_RX, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledDMAReq_RX, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableDMAReq_TX, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableDMAReq_TX, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledDMAReq_TX, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_EnableDMADeactOnRxErr, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_DisableDMADeactOnRxErr, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_IsEnabledDMADeactOnRxErr, USART_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_USART_DMA_GetRegAddr, USART_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint8_t, LL_USART_ReceiveData8, USART_TypeDef*);

FAKE_VALUE_FUNC(uint16_t, LL_USART_ReceiveData9, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_TransmitData8, USART_TypeDef*, uint8_t);

FAKE_VOID_FUNC(LL_USART_TransmitData9, USART_TypeDef*, uint16_t);

FAKE_VOID_FUNC(LL_USART_RequestAutoBaudRate, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_RequestBreakSending, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_RequestEnterMuteMode, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_RequestRxDataFlush, USART_TypeDef*);

FAKE_VOID_FUNC(LL_USART_RequestTxDataFlush, USART_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_USART_DeInit, USART_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_USART_Init, USART_TypeDef*, LL_USART_InitTypeDef*);

FAKE_VOID_FUNC(LL_USART_StructInit, LL_USART_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_USART_ClockInit, USART_TypeDef*, LL_USART_ClockInitTypeDef*);

FAKE_VOID_FUNC(LL_USART_ClockStructInit, LL_USART_ClockInitTypeDef*);

# endif /* STM32F7XX_LL_USART_TH_HH */
