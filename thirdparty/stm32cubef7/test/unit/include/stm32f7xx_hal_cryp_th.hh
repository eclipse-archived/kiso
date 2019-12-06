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
 * Mock-up implementation for the stm32f7xx_hal_cryp module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_CRYP_TH_HH
#define STM32F7XX_HAL_CRYP_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_cryp.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if defined (CRYP)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_Init, CRYP_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DeInit, CRYP_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CRYP_MspInit, CRYP_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CRYP_MspDeInit, CRYP_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESECB_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESECB_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESCBC_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESCBC_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESECB_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESECB_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESCBC_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESCBC_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESECB_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESECB_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESCBC_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DESCBC_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESECB_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESCBC_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESECB_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESCBC_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESECB_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESECB_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESCBC_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESCBC_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESECB_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESECB_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESCBC_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_TDESCBC_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_CRYP_STATETypeDef, HAL_CRYP_GetState, CRYP_HandleTypeDef*);

#endif /* CRYP */

#if defined (AES)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_Init, CRYP_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_DeInit, CRYP_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CRYP_MspInit, CRYP_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CRYP_MspDeInit, CRYP_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Encrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Decrypt, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Encrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Decrypt_IT, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_CRYP_STATETypeDef, HAL_CRYP_GetState, CRYP_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_CRYP_GetError, CRYP_HandleTypeDef*);

#endif /* AES */

# endif /* STM32F7XX_HAL_CRYP_TH_HH */
