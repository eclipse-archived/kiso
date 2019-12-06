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
 * Mock-up implementation for the stm32f7xx_ll_dma module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_DMA_TH_HH
#define STM32F7XX_LL_DMA_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_DMA_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/** @defgroup DMA_LL_ES_INIT DMA Exported Init structure
  * @{
  */
typedef struct
{
  uint32_t PeriphOrM2MSrcAddress;  /*!< Specifies the peripheral base address for DMA transfer
                                        or as Source base address in case of memory to memory transfer direction.

                                        This parameter must be a value between Min_Data = 0 and Max_Data = 0xFFFFFFFF. */

  uint32_t MemoryOrM2MDstAddress;  /*!< Specifies the memory base address for DMA transfer
                                        or as Destination base address in case of memory to memory transfer direction.

                                        This parameter must be a value between Min_Data = 0 and Max_Data = 0xFFFFFFFF. */

  uint32_t Direction;              /*!< Specifies if the data will be transferred from memory to peripheral,
                                        from memory to memory or from peripheral to memory.
                                        This parameter can be a value of @ref DMA_LL_EC_DIRECTION

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetDataTransferDirection(). */

  uint32_t Mode;                   /*!< Specifies the normal or circular operation mode.
                                        This parameter can be a value of @ref DMA_LL_EC_MODE
                                        @note The circular buffer mode cannot be used if the memory to memory
                                              data transfer direction is configured on the selected Stream

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetMode(). */

  uint32_t PeriphOrM2MSrcIncMode;  /*!< Specifies whether the Peripheral address or Source address in case of memory to memory transfer direction
                                        is incremented or not.
                                        This parameter can be a value of @ref DMA_LL_EC_PERIPH

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetPeriphIncMode(). */

  uint32_t MemoryOrM2MDstIncMode;  /*!< Specifies whether the Memory address or Destination address in case of memory to memory transfer direction
                                        is incremented or not.
                                        This parameter can be a value of @ref DMA_LL_EC_MEMORY

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetMemoryIncMode(). */

  uint32_t PeriphOrM2MSrcDataSize; /*!< Specifies the Peripheral data size alignment or Source data size alignment (byte, half word, word)
                                        in case of memory to memory transfer direction.
                                        This parameter can be a value of @ref DMA_LL_EC_PDATAALIGN

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetPeriphSize(). */

  uint32_t MemoryOrM2MDstDataSize; /*!< Specifies the Memory data size alignment or Destination data size alignment (byte, half word, word)
                                        in case of memory to memory transfer direction.
                                        This parameter can be a value of @ref DMA_LL_EC_MDATAALIGN

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetMemorySize(). */

  uint32_t NbData;                 /*!< Specifies the number of data to transfer, in data unit.
                                        The data unit is equal to the source buffer configuration set in PeripheralSize
                                        or MemorySize parameters depending in the transfer direction.
                                        This parameter must be a value between Min_Data = 0 and Max_Data = 0x0000FFFF

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetDataLength(). */

  uint32_t Channel;                /*!< Specifies the peripheral channel.
                                        This parameter can be a value of @ref DMA_LL_EC_CHANNEL

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetChannelSelection(). */

  uint32_t Priority;               /*!< Specifies the channel priority level.
                                        This parameter can be a value of @ref DMA_LL_EC_PRIORITY

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetStreamPriorityLevel(). */

  uint32_t FIFOMode;               /*!< Specifies if the FIFO mode or Direct mode will be used for the specified stream.
                                        This parameter can be a value of @ref DMA_LL_FIFOMODE
                                        @note The Direct mode (FIFO mode disabled) cannot be used if the
                                        memory-to-memory data transfer is configured on the selected stream

                                        This feature can be modified afterwards using unitary functions @ref LL_DMA_EnableFifoMode() or @ref LL_DMA_EnableFifoMode() . */

  uint32_t FIFOThreshold;          /*!< Specifies the FIFO threshold level.
                                        This parameter can be a value of @ref DMA_LL_EC_FIFOTHRESHOLD

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetFIFOThreshold(). */

  uint32_t MemBurst;               /*!< Specifies the Burst transfer configuration for the memory transfers.
                                        It specifies the amount of data to be transferred in a single non interruptible
                                        transaction.
                                        This parameter can be a value of @ref DMA_LL_EC_MBURST
                                        @note The burst mode is possible only if the address Increment mode is enabled.

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetMemoryBurstxfer(). */

  uint32_t PeriphBurst;            /*!< Specifies the Burst transfer configuration for the peripheral transfers.
                                        It specifies the amount of data to be transferred in a single non interruptible
                                        transaction.
                                        This parameter can be a value of @ref DMA_LL_EC_PBURST
                                        @note The burst mode is possible only if the address Increment mode is enabled.

                                        This feature can be modified afterwards using unitary function @ref LL_DMA_SetPeriphBurstxfer(). */

} LL_DMA_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_DMA_EnableStream, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_DisableStream, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsEnabledStream, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_ConfigTransfer, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetDataTransferDirection, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetDataTransferDirection, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetMode, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetMode, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetPeriphIncMode, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetPeriphIncMode, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetMemoryIncMode, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetMemoryIncMode, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetPeriphSize, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetPeriphSize, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetMemorySize, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetMemorySize, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetIncOffsetSize, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetIncOffsetSize, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetStreamPriorityLevel, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetStreamPriorityLevel, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetDataLength, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetDataLength, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetChannelSelection, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetChannelSelection, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetMemoryBurstxfer, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetMemoryBurstxfer, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetPeriphBurstxfer, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetPeriphBurstxfer, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetCurrentTargetMem, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetCurrentTargetMem, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_EnableDoubleBufferMode, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_DisableDoubleBufferMode, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetFIFOStatus, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_DisableFifoMode, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_EnableFifoMode, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetFIFOThreshold, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetFIFOThreshold, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_ConfigFifo, DMA_TypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DMA_ConfigAddresses, DMA_TypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetMemoryAddress, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetPeriphAddress, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetMemoryAddress, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetPeriphAddress, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetM2MSrcAddress, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetM2MDstAddress, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetM2MSrcAddress, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetM2MDstAddress, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_SetMemory1Address, DMA_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_GetMemory1Address, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_HT0, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_HT1, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_HT2, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_HT3, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_HT4, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_HT5, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_HT6, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_HT7, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TC0, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TC1, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TC2, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TC3, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TC4, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TC5, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TC6, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TC7, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TE0, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TE1, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TE2, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TE3, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TE4, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TE5, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TE6, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_TE7, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_DME0, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_DME1, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_DME2, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_DME3, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_DME4, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_DME5, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_DME6, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_DME7, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_FE0, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_FE1, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_FE2, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_FE3, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_FE4, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_FE5, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_FE6, DMA_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsActiveFlag_FE7, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_HT0, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_HT1, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_HT2, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_HT3, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_HT4, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_HT5, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_HT6, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_HT7, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TC0, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TC1, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TC2, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TC3, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TC4, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TC5, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TC6, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TC7, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TE0, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TE1, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TE2, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TE3, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TE4, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TE5, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TE6, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_TE7, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_DME0, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_DME1, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_DME2, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_DME3, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_DME4, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_DME5, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_DME6, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_DME7, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_FE0, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_FE1, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_FE2, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_FE3, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_FE4, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_FE5, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_FE6, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_ClearFlag_FE7, DMA_TypeDef*);

FAKE_VOID_FUNC(LL_DMA_EnableIT_HT, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_EnableIT_TE, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_EnableIT_TC, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_EnableIT_DME, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_EnableIT_FE, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_DisableIT_HT, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_DisableIT_TE, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_DisableIT_TC, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_DisableIT_DME, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_DisableIT_FE, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsEnabledIT_HT, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsEnabledIT_TE, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsEnabledIT_TC, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsEnabledIT_DME, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_IsEnabledIT_FE, DMA_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_Init, DMA_TypeDef*, uint32_t, LL_DMA_InitTypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA_DeInit, DMA_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA_StructInit, LL_DMA_InitTypeDef*);

# endif /* STM32F7XX_LL_DMA_TH_HH */
