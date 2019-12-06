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
 * Mock-up implementation for the stm32f7xx_hal_i2c module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_I2C_TH_HH
#define STM32F7XX_HAL_I2C_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_i2c.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#undef __HAL_I2C_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_I2C_RESET_HANDLE_STATE, I2C_HandleTypeDef*);

#undef __HAL_I2C_ENABLE_IT
FAKE_VOID_FUNC(__HAL_I2C_ENABLE_IT, I2C_HandleTypeDef*, uint32_t);

#undef __HAL_I2C_DISABLE_IT
FAKE_VOID_FUNC(__HAL_I2C_DISABLE_IT, I2C_HandleTypeDef*, uint32_t);

#undef __HAL_I2C_GET_IT_SOURCE
FAKE_VALUE_FUNC(bool, __HAL_I2C_GET_IT_SOURCE, I2C_HandleTypeDef*, uint32_t);

#undef __HAL_I2C_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_I2C_GET_FLAG, I2C_HandleTypeDef*, uint32_t);

#undef __HAL_I2C_CLEAR_FLAG
FAKE_VOID_FUNC(__HAL_I2C_CLEAR_FLAG, I2C_HandleTypeDef*, uint32_t);

#undef __HAL_I2C_ENABLE
FAKE_VOID_FUNC(__HAL_I2C_ENABLE, I2C_HandleTypeDef*);

#undef __HAL_I2C_DISABLE
FAKE_VOID_FUNC(__HAL_I2C_DISABLE, I2C_HandleTypeDef*);

#undef __HAL_I2C_GENERATE_NACK
FAKE_VOID_FUNC(__HAL_I2C_GENERATE_NACK, I2C_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Init, I2C_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_DeInit, I2C_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_I2C_MspInit, I2C_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_I2C_MspDeInit, I2C_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Transmit, I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Receive, I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Slave_Transmit, I2C_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Slave_Receive, I2C_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Mem_Write, I2C_HandleTypeDef*, uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Mem_Read, I2C_HandleTypeDef*, uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_IsDeviceReady, I2C_HandleTypeDef*, uint16_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Transmit_IT, I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Receive_IT, I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Slave_Transmit_IT, I2C_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Slave_Receive_IT, I2C_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Mem_Write_IT, I2C_HandleTypeDef*, uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Mem_Read_IT, I2C_HandleTypeDef*, uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Sequential_Transmit_IT, I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Sequential_Receive_IT, I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Slave_Sequential_Transmit_IT, I2C_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Slave_Sequential_Receive_IT, I2C_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_EnableListen_IT, I2C_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_DisableListen_IT, I2C_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Abort_IT, I2C_HandleTypeDef*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Transmit_DMA, I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Master_Receive_DMA, I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Slave_Transmit_DMA, I2C_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Slave_Receive_DMA, I2C_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Mem_Write_DMA, I2C_HandleTypeDef*, uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2C_Mem_Read_DMA, I2C_HandleTypeDef*, uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t);

FAKE_VOID_FUNC(HAL_I2C_EV_IRQHandler, I2C_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_I2C_ER_IRQHandler, I2C_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_I2C_StateTypeDef, HAL_I2C_GetState, I2C_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_I2C_ModeTypeDef, HAL_I2C_GetMode, I2C_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_I2C_GetError, I2C_HandleTypeDef*);

# endif /* STM32F7XX_HAL_I2C_TH_HH */
