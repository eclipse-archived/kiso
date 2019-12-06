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
 * Mock-up implementation for the stm32f7xx_ll_i2c module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_I2C_TH_HH
#define STM32F7XX_LL_I2C_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_I2C_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/** @defgroup I2C_LL_ES_INIT I2C Exported Init structure
  * @{
  */
typedef struct
{
  uint32_t PeripheralMode;      /*!< Specifies the peripheral mode.
                                     This parameter can be a value of @ref I2C_LL_EC_PERIPHERAL_MODE

                                     This feature can be modified afterwards using unitary function @ref LL_I2C_SetMode(). */

  uint32_t Timing;              /*!< Specifies the SDA setup, hold time and the SCL high, low period values.
                                     This parameter must be set by referring to the STM32CubeMX Tool and
                                     the helper macro @ref __LL_I2C_CONVERT_TIMINGS()

                                     This feature can be modified afterwards using unitary function @ref LL_I2C_SetTiming(). */

  uint32_t AnalogFilter;        /*!< Enables or disables analog noise filter.
                                     This parameter can be a value of @ref I2C_LL_EC_ANALOGFILTER_SELECTION

                                     This feature can be modified afterwards using unitary functions @ref LL_I2C_EnableAnalogFilter() or LL_I2C_DisableAnalogFilter(). */

  uint32_t DigitalFilter;       /*!< Configures the digital noise filter.
                                     This parameter can be a number between Min_Data = 0x00 and Max_Data = 0x0F

                                     This feature can be modified afterwards using unitary function @ref LL_I2C_SetDigitalFilter(). */

  uint32_t OwnAddress1;         /*!< Specifies the device own address 1.
                                     This parameter must be a value between Min_Data = 0x00 and Max_Data = 0x3FF

                                     This feature can be modified afterwards using unitary function @ref LL_I2C_SetOwnAddress1(). */

  uint32_t TypeAcknowledge;     /*!< Specifies the ACKnowledge or Non ACKnowledge condition after the address receive match code or next received byte.
                                     This parameter can be a value of @ref I2C_LL_EC_I2C_ACKNOWLEDGE

                                     This feature can be modified afterwards using unitary function @ref LL_I2C_AcknowledgeNextData(). */

  uint32_t OwnAddrSize;         /*!< Specifies the device own address 1 size (7-bit or 10-bit).
                                     This parameter can be a value of @ref I2C_LL_EC_OWNADDRESS1

                                     This feature can be modified afterwards using unitary function @ref LL_I2C_SetOwnAddress1(). */
} LL_I2C_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_I2C_Enable, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_Disable, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabled, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ConfigFilters, I2C_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_I2C_SetDigitalFilter, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetDigitalFilter, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableAnalogFilter, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableAnalogFilter, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledAnalogFilter, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableDMAReq_TX, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableDMAReq_TX, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledDMAReq_TX, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableDMAReq_RX, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableDMAReq_RX, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledDMAReq_RX, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_DMA_GetRegAddr, I2C_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_I2C_EnableClockStretching, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableClockStretching, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledClockStretching, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableSlaveByteControl, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableSlaveByteControl, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledSlaveByteControl, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableGeneralCall, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableGeneralCall, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledGeneralCall, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetMasterAddressingMode, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetMasterAddressingMode, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetOwnAddress1, I2C_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_I2C_EnableOwnAddress1, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableOwnAddress1, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledOwnAddress1, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetOwnAddress2, I2C_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_I2C_EnableOwnAddress2, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableOwnAddress2, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledOwnAddress2, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetTiming, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetTimingPrescaler, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetClockLowPeriod, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetClockHighPeriod, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetDataHoldTime, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetDataSetupTime, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetMode, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetMode, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableSMBusAlert, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableSMBusAlert, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledSMBusAlert, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableSMBusPEC, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableSMBusPEC, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledSMBusPEC, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetSMBusTimeoutA, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetSMBusTimeoutA, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetSMBusTimeoutAMode, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetSMBusTimeoutAMode, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetSMBusTimeoutB, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetSMBusTimeoutB, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableSMBusTimeout, I2C_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_I2C_DisableSMBusTimeout, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledSMBusTimeout, I2C_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_I2C_EnableIT_TX, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableIT_TX, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledIT_TX, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableIT_RX, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableIT_RX, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledIT_RX, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableIT_ADDR, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableIT_ADDR, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledIT_ADDR, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableIT_NACK, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableIT_NACK, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledIT_NACK, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableIT_STOP, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableIT_STOP, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledIT_STOP, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableIT_TC, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableIT_TC, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledIT_TC, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableIT_ERR, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableIT_ERR, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledIT_ERR, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_TXE, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_TXIS, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_RXNE, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_ADDR, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_NACK, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_STOP, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_TC, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_TCR, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_BERR, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_ARLO, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_OVR, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveSMBusFlag_PECERR, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveSMBusFlag_TIMEOUT, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveSMBusFlag_ALERT, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsActiveFlag_BUSY, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearFlag_ADDR, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearFlag_NACK, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearFlag_STOP, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearFlag_TXE, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearFlag_BERR, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearFlag_ARLO, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearFlag_OVR, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearSMBusFlag_PECERR, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearSMBusFlag_TIMEOUT, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_ClearSMBusFlag_ALERT, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableAutoEndMode, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableAutoEndMode, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledAutoEndMode, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableReloadMode, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableReloadMode, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledReloadMode, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetTransferSize, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetTransferSize, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_AcknowledgeNextData, I2C_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_I2C_GenerateStartCondition, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_GenerateStopCondition, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableAuto10BitRead, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_DisableAuto10BitRead, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledAuto10BitRead, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetTransferRequest, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetTransferRequest, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_SetSlaveAddr, I2C_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetSlaveAddr, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetTransferDirection, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetAddressMatchCode, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_EnableSMBusPECCompare, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_IsEnabledSMBusPECCompare, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_GetSMBusPEC, I2C_TypeDef*);

FAKE_VALUE_FUNC(uint8_t, LL_I2C_ReceiveData8, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_TransmitData8, I2C_TypeDef*, uint8_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_Init, I2C_TypeDef*, LL_I2C_InitTypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2C_DeInit, I2C_TypeDef*);

FAKE_VOID_FUNC(LL_I2C_StructInit, LL_I2C_InitTypeDef*);

# endif /* STM32F7XX_LL_I2C_TH_HH */
