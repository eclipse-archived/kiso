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
 * Mock-up implementation for the stm32f7xx_ll_usb module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_USB_TH_HH
#define STM32F7XX_LL_USB_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_ll_usb.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_CoreInit, USB_OTG_GlobalTypeDef*, USB_OTG_CfgTypeDef);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_DevInit, USB_OTG_GlobalTypeDef*, USB_OTG_CfgTypeDef);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_EnableGlobalInt, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_DisableGlobalInt, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_SetCurrentMode, USB_OTG_GlobalTypeDef*, USB_OTG_ModeTypeDef);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_SetDevSpeed, USB_OTG_GlobalTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_FlushRxFifo, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_FlushTxFifo, USB_OTG_GlobalTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_ActivateEndpoint, USB_OTG_GlobalTypeDef*, USB_OTG_EPTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_DeactivateEndpoint, USB_OTG_GlobalTypeDef*, USB_OTG_EPTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_ActivateDedicatedEndpoint, USB_OTG_GlobalTypeDef*, USB_OTG_EPTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_DeactivateDedicatedEndpoint, USB_OTG_GlobalTypeDef*, USB_OTG_EPTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_EPStartXfer, USB_OTG_GlobalTypeDef*, USB_OTG_EPTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_EP0StartXfer, USB_OTG_GlobalTypeDef*, USB_OTG_EPTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_WritePacket, USB_OTG_GlobalTypeDef*, uint8_t*, uint8_t, uint16_t, uint8_t);

FAKE_VALUE_FUNC(void*, USB_ReadPacket, USB_OTG_GlobalTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_EPSetStall, USB_OTG_GlobalTypeDef*, USB_OTG_EPTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_EPClearStall, USB_OTG_GlobalTypeDef*, USB_OTG_EPTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_SetDevAddress, USB_OTG_GlobalTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_DevConnect, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_DevDisconnect, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_StopDevice, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_ActivateSetup, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_EP0_OutStart, USB_OTG_GlobalTypeDef*, uint8_t, uint8_t*);

FAKE_VALUE_FUNC(uint8_t, USB_GetDevSpeed, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(uint32_t, USB_GetMode, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(uint32_t, USB_ReadInterrupts, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(uint32_t, USB_ReadDevAllOutEpInterrupt, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(uint32_t, USB_ReadDevOutEPInterrupt, USB_OTG_GlobalTypeDef*, uint8_t);

FAKE_VALUE_FUNC(uint32_t, USB_ReadDevAllInEpInterrupt, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(uint32_t, USB_ReadDevInEPInterrupt, USB_OTG_GlobalTypeDef*, uint8_t);

FAKE_VOID_FUNC(USB_ClearInterrupts, USB_OTG_GlobalTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_HostInit, USB_OTG_GlobalTypeDef*, USB_OTG_CfgTypeDef);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_InitFSLSPClkSel, USB_OTG_GlobalTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_ResetPort, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_DriveVbus, USB_OTG_GlobalTypeDef*, uint8_t);

FAKE_VALUE_FUNC(uint32_t, USB_GetHostSpeed, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(uint32_t, USB_GetCurrentFrame, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_HC_Init, USB_OTG_GlobalTypeDef*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_HC_StartXfer, USB_OTG_GlobalTypeDef*, USB_OTG_HCTypeDef*, uint8_t);

FAKE_VALUE_FUNC(uint32_t, USB_HC_ReadInterrupt, USB_OTG_GlobalTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_HC_Halt, USB_OTG_GlobalTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_DoPing, USB_OTG_GlobalTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, USB_StopHost, USB_OTG_GlobalTypeDef*);

# endif /* STM32F7XX_LL_USB_TH_HH */
