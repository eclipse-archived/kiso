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
 * Mock-up implementation for the stm32f7xx_ll_spi module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_SPI_TH_HH
#define STM32F7XX_LL_SPI_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_SPI_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief  SPI Init structures definition
  */
typedef struct
{
  uint32_t TransferDirection;       /*!< Specifies the SPI unidirectional or bidirectional data mode.
                                         This parameter can be a value of @ref SPI_LL_EC_TRANSFER_MODE.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetTransferDirection().*/

  uint32_t Mode;                    /*!< Specifies the SPI mode (Master/Slave).
                                         This parameter can be a value of @ref SPI_LL_EC_MODE.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetMode().*/

  uint32_t DataWidth;               /*!< Specifies the SPI data width.
                                         This parameter can be a value of @ref SPI_LL_EC_DATAWIDTH.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetDataWidth().*/

  uint32_t ClockPolarity;           /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref SPI_LL_EC_POLARITY.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetClockPolarity().*/

  uint32_t ClockPhase;              /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref SPI_LL_EC_PHASE.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetClockPhase().*/

  uint32_t NSS;                     /*!< Specifies whether the NSS signal is managed by hardware (NSS pin) or by software using the SSI bit.
                                         This parameter can be a value of @ref SPI_LL_EC_NSS_MODE.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetNSSMode().*/

  uint32_t BaudRate;                /*!< Specifies the BaudRate prescaler value which will be used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref SPI_LL_EC_BAUDRATEPRESCALER.
                                         @note The communication clock is derived from the master clock. The slave clock does not need to be set.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetBaudRatePrescaler().*/

  uint32_t BitOrder;                /*!< Specifies whether data transfers start from MSB or LSB bit.
                                         This parameter can be a value of @ref SPI_LL_EC_BIT_ORDER.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetTransferBitOrder().*/

  uint32_t CRCCalculation;          /*!< Specifies if the CRC calculation is enabled or not.
                                         This parameter can be a value of @ref SPI_LL_EC_CRC_CALCULATION.

                                         This feature can be modified afterwards using unitary functions @ref LL_SPI_EnableCRC() and @ref LL_SPI_DisableCRC().*/

  uint32_t CRCPoly;                 /*!< Specifies the polynomial used for the CRC calculation.
                                         This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFFFF.

                                         This feature can be modified afterwards using unitary function @ref LL_SPI_SetCRCPolynomial().*/

} LL_SPI_InitTypeDef;

/**
  * @brief  I2S Init structure definition
  */

typedef struct
{
  uint32_t Mode;                    /*!< Specifies the I2S operating mode.
                                         This parameter can be a value of @ref I2S_LL_EC_MODE

                                         This feature can be modified afterwards using unitary function @ref LL_I2S_SetTransferMode().*/

  uint32_t Standard;                /*!< Specifies the standard used for the I2S communication.
                                         This parameter can be a value of @ref I2S_LL_EC_STANDARD

                                         This feature can be modified afterwards using unitary function @ref LL_I2S_SetStandard().*/


  uint32_t DataFormat;              /*!< Specifies the data format for the I2S communication.
                                         This parameter can be a value of @ref I2S_LL_EC_DATA_FORMAT

                                         This feature can be modified afterwards using unitary function @ref LL_I2S_SetDataFormat().*/


  uint32_t MCLKOutput;              /*!< Specifies whether the I2S MCLK output is enabled or not.
                                         This parameter can be a value of @ref I2S_LL_EC_MCLK_OUTPUT

                                         This feature can be modified afterwards using unitary functions @ref LL_I2S_EnableMasterClock() or @ref LL_I2S_DisableMasterClock.*/


  uint32_t AudioFreq;               /*!< Specifies the frequency selected for the I2S communication.
                                         This parameter can be a value of @ref I2S_LL_EC_AUDIO_FREQ

                                         Audio Frequency can be modified afterwards using Reference manual formulas to calculate Prescaler Linear, Parity
                                         and unitary functions @ref LL_I2S_SetPrescalerLinear() and @ref LL_I2S_SetPrescalerParity() to set it.*/


  uint32_t ClockPolarity;           /*!< Specifies the idle state of the I2S clock.
                                         This parameter can be a value of @ref I2S_LL_EC_POLARITY

                                         This feature can be modified afterwards using unitary function @ref LL_I2S_SetClockPolarity().*/

} LL_I2S_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_SPI_Enable, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_Disable, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsEnabled, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetMode, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetMode, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetStandard, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetStandard, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetClockPhase, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetClockPhase, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetClockPolarity, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetClockPolarity, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetBaudRatePrescaler, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetBaudRatePrescaler, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetTransferBitOrder, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetTransferBitOrder, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetTransferDirection, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetTransferDirection, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetDataWidth, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetDataWidth, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetRxFIFOThreshold, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetRxFIFOThreshold, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_EnableCRC, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_DisableCRC, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsEnabledCRC, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetCRCWidth, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetCRCWidth, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetCRCNext, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetCRCPolynomial, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetCRCPolynomial, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetRxCRC, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetTxCRC, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetNSSMode, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetNSSMode, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_EnableNSSPulseMgt, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_DisableNSSPulseMgt, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsEnabledNSSPulse, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsActiveFlag_RXNE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsActiveFlag_TXE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsActiveFlag_CRCERR, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsActiveFlag_MODF, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsActiveFlag_OVR, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsActiveFlag_BSY, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsActiveFlag_FRE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetRxFIFOLevel, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetTxFIFOLevel, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_ClearFlag_CRCERR, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_ClearFlag_MODF, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_ClearFlag_OVR, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_ClearFlag_FRE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_EnableIT_ERR, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_EnableIT_RXNE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_EnableIT_TXE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_DisableIT_ERR, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_DisableIT_RXNE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_DisableIT_TXE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsEnabledIT_ERR, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsEnabledIT_RXNE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsEnabledIT_TXE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_EnableDMAReq_RX, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_DisableDMAReq_RX, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsEnabledDMAReq_RX, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_EnableDMAReq_TX, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_DisableDMAReq_TX, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_IsEnabledDMAReq_TX, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetDMAParity_RX, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetDMAParity_RX, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_SetDMAParity_TX, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_GetDMAParity_TX, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_SPI_DMA_GetRegAddr, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint8_t, LL_SPI_ReceiveData8, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint16_t, LL_SPI_ReceiveData16, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_SPI_TransmitData8, SPI_TypeDef*, uint8_t);

FAKE_VOID_FUNC(LL_SPI_TransmitData16, SPI_TypeDef*, uint16_t);

FAKE_VOID_FUNC(LL_I2S_Enable, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_Disable, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsEnabled, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_SetDataFormat, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_GetDataFormat, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_SetClockPolarity, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_GetClockPolarity, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_SetStandard, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_GetStandard, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_SetTransferMode, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_GetTransferMode, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_SetPrescalerLinear, SPI_TypeDef*, uint8_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_GetPrescalerLinear, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_SetPrescalerParity, SPI_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_GetPrescalerParity, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_EnableMasterClock, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_DisableMasterClock, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsEnabledMasterClock, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_EnableAsyncStart, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_DisableAsyncStart, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsEnabledAsyncStart, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsActiveFlag_RXNE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsActiveFlag_TXE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsActiveFlag_BSY, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsActiveFlag_OVR, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsActiveFlag_UDR, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsActiveFlag_FRE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsActiveFlag_CHSIDE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_ClearFlag_OVR, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_ClearFlag_UDR, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_ClearFlag_FRE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_EnableIT_ERR, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_EnableIT_RXNE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_EnableIT_TXE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_DisableIT_ERR, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_DisableIT_RXNE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_DisableIT_TXE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsEnabledIT_ERR, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsEnabledIT_RXNE, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsEnabledIT_TXE, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_EnableDMAReq_RX, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_DisableDMAReq_RX, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsEnabledDMAReq_RX, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_EnableDMAReq_TX, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_DisableDMAReq_TX, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_I2S_IsEnabledDMAReq_TX, SPI_TypeDef*);

FAKE_VALUE_FUNC(uint16_t, LL_I2S_ReceiveData16, SPI_TypeDef*);

FAKE_VOID_FUNC(LL_I2S_TransmitData16, SPI_TypeDef*, uint16_t);

FAKE_VALUE_FUNC(ErrorStatus, LL_SPI_DeInit, SPI_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_SPI_Init, SPI_TypeDef*, LL_SPI_InitTypeDef*);

FAKE_VOID_FUNC(LL_SPI_StructInit, LL_SPI_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_I2S_DeInit, SPI_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_I2S_Init, SPI_TypeDef*, LL_I2S_InitTypeDef*);

FAKE_VOID_FUNC(LL_I2S_StructInit, LL_I2S_InitTypeDef*);

FAKE_VOID_FUNC(LL_I2S_ConfigPrescaler, SPI_TypeDef*, uint32_t, uint32_t);

# endif /* STM32F7XX_LL_SPI_TH_HH */
