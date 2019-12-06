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
 * Mock-up implementation for the stm32f7xx_ll_sdmmc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_SDMMC_TH_HH
#define STM32F7XX_LL_SDMMC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_ll_sdmmc.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, SDMMC_Init, SDMMC_TypeDef*, SDMMC_InitTypeDef);

FAKE_VALUE_FUNC(uint32_t, SDMMC_ReadFIFO, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, SDMMC_WriteFIFO, SDMMC_TypeDef*, uint32_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, SDMMC_PowerState_ON, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, SDMMC_PowerState_OFF, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_GetPowerState, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, SDMMC_SendCommand, SDMMC_TypeDef*, SDMMC_CmdInitTypeDef*);

FAKE_VALUE_FUNC(uint8_t, SDMMC_GetCommandResponse, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_GetResponse, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, SDMMC_ConfigData, SDMMC_TypeDef*, SDMMC_DataInitTypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_GetDataCounter, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_GetFIFOCount, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, SDMMC_SetSDMMCReadWaitMode, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdBlockLength, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdReadSingleBlock, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdReadMultiBlock, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdWriteSingleBlock, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdWriteMultiBlock, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdEraseStartAdd, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSDEraseStartAdd, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdEraseEndAdd, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSDEraseEndAdd, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdErase, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdStopTransfer, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSelDesel, SDMMC_TypeDef*, uint64_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdGoIdleState, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdOperCond, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdAppCommand, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdAppOperCommand, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdBusWidth, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSendSCR, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSendCID, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSendCSD, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSetRelAdd, SDMMC_TypeDef*, uint16_t*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSendStatus, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdStatusRegister, SDMMC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdOpCondition, SDMMC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, SDMMC_CmdSwitch, SDMMC_TypeDef*, uint32_t);

# endif /* STM32F7XX_LL_SDMMC_TH_HH */