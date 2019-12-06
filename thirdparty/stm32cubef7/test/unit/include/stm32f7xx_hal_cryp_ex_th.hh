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
 * Mock-up implementation for the stm32f7xx_hal_cryp_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_CRYP_EX_TH_HH
#define STM32F7XX_HAL_CRYP_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_cryp_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_cryp_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if defined (CRYP)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESGCM_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESGCM_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESGCM_Finish, CRYP_HandleTypeDef*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESCCM_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESCCM_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESCCM_Finish, CRYP_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESGCM_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESGCM_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESCCM_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESCCM_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESGCM_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESGCM_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESCCM_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AESCCM_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

#endif /* CRYP */

#if defined (AES)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AES, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AES_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AES_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AES_Auth, CRYP_HandleTypeDef*, uint8_t*, uint64_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AES_Auth_IT, CRYP_HandleTypeDef*, uint8_t*, uint64_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYPEx_AES_Auth_DMA, CRYP_HandleTypeDef*, uint8_t*, uint64_t, uint8_t*);

FAKE_VOID_FUNC(HAL_CRYPEx_Read_IVRegisters, CRYP_HandleTypeDef*, uint8_t*);

FAKE_VOID_FUNC(HAL_CRYPEx_Write_IVRegisters, CRYP_HandleTypeDef*, uint8_t*);

FAKE_VOID_FUNC(HAL_CRYPEx_Read_SuspendRegisters, CRYP_HandleTypeDef*, uint8_t*);

FAKE_VOID_FUNC(HAL_CRYPEx_Write_SuspendRegisters, CRYP_HandleTypeDef*, uint8_t*);

FAKE_VOID_FUNC(HAL_CRYPEx_Read_KeyRegisters, CRYP_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VOID_FUNC(HAL_CRYPEx_Write_KeyRegisters, CRYP_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VOID_FUNC(HAL_CRYPEx_Read_ControlRegister, CRYP_HandleTypeDef*, uint8_t*);

FAKE_VOID_FUNC(HAL_CRYPEx_Write_ControlRegister, CRYP_HandleTypeDef*, uint8_t*);

FAKE_VOID_FUNC(HAL_CRYPEx_ProcessSuspend, CRYP_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, CRYP_AES_Auth_IT, CRYP_HandleTypeDef*);

#endif /* AES */

# endif /* STM32F7XX_HAL_CRYP_EX_TH_HH */
