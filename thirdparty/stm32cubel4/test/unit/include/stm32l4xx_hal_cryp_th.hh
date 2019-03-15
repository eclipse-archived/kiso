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
 */
/*----------------------------------------------------------------------------*/

#ifndef STM32L4XX_HAL_CRYP_TH_HH_
#define STM32L4XX_HAL_CRYP_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_cryp.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */
#if defined (STM32L442xx) || defined (STM32L443xx) || defined(STM32L485xx) || defined(STM32L486xx)

#undef __HAL_CRYP_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_CRYP_RESET_HANDLE_STATE, CRYP_HandleTypeDef*);
#undef __HAL_CRYP_ENABLE
FAKE_VOID_FUNC(__HAL_CRYP_ENABLE);
#undef __HAL_CRYP_DISABLE
FAKE_VOID_FUNC(__HAL_CRYP_DISABLE);
#undef __HAL_CRYP_SET_OPERATINGMODE
FAKE_VOID_FUNC(__HAL_CRYP_SET_OPERATINGMODE, uint32_t);
#undef __HAL_CRYP_SET_CHAININGMODE
FAKE_VOID_FUNC(__HAL_CRYP_SET_CHAININGMODE, uint32_t);
#undef __HAL_CRYP_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_CRYP_GET_FLAG, uint32_t);
#undef __HAL_CRYP_CLEAR_FLAG
FAKE_VOID_FUNC(__HAL_CRYP_CLEAR_FLAG, uint32_t);
#undef __HAL_CRYP_GET_IT_SOURCE
FAKE_VALUE_FUNC(bool, __HAL_CRYP_GET_IT_SOURCE, uint32_t);
#undef __HAL_CRYP_GET_IT
FAKE_VALUE_FUNC(bool, __HAL_CRYP_GET_IT, uint32_t);
#undef __HAL_CRYP_CLEAR_IT
FAKE_VOID_FUNC(__HAL_CRYP_CLEAR_IT, uint32_t);
#undef __HAL_CRYP_ENABLE_IT
FAKE_VOID_FUNC(__HAL_CRYP_ENABLE_IT, uint32_t);
#undef __HAL_CRYP_DISABLE_IT
FAKE_VOID_FUNC(__HAL_CRYP_DISABLE_IT, uint32_t);

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

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESECB_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCBC_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Encrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRYP_AESCTR_Decrypt_DMA, CRYP_HandleTypeDef*, uint8_t*, uint16_t, uint8_t *);

FAKE_VOID_FUNC(HAL_CRYP_IRQHandler, CRYP_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_CRYP_STATETypeDef, HAL_CRYP_GetState, CRYP_HandleTypeDef*);
FAKE_VALUE_FUNC(uint32_t, HAL_CRYP_GetError, CRYP_HandleTypeDef*);

#endif /* defined (STM32L442xx) || defined (STM32L443xx) || defined(STM32L485xx) || defined(STM32L486xx) */

/* Mock-ups for the provided interfaces */

#endif /* STM32L4XX_HAL_CRYP_TH_HH_ */
