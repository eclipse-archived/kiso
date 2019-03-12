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

#ifndef BCDS_MCU_I2C_th_HH
#define BCDS_MCU_I2C_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_MCU_I2C.h"

/* Faked variables needs to be initialized by the test fixture */

#if BCDS_FEATURE_I2C

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_Initialize, I2C_T, MCU_I2C_Callback_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_Deinitialize, I2C_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_Send, I2C_T, uint16_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_Receive, I2C_T, uint16_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_ReadRegister, I2C_T, uint16_t, uint8_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_WriteRegister, I2C_T, uint16_t, uint8_t, uint8_t*, uint32_t);

#endif /* BCDS_FEATURE_I2C */
#endif /* BCDS_MCU_I2C_th_HH */
