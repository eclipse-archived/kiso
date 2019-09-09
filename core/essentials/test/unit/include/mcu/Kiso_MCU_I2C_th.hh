/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

#ifndef KISO_MCU_I2C_TH_HH
#define KISO_MCU_I2C_TH_HH

#include "gtest.h"

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_MCU_I2C.h"

/* Faked variables needs to be initialized by the test fixture */

#if KISO_FEATURE_I2C

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_Initialize, I2C_T, MCU_I2C_Callback_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_Deinitialize, I2C_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_Send, I2C_T, uint16_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_Receive, I2C_T, uint16_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_ReadRegister, I2C_T, uint16_t, uint8_t, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_I2C_WriteRegister, I2C_T, uint16_t, uint8_t, uint8_t*, uint32_t);

#endif /* KISO_FEATURE_I2C */
#endif /* KISO_MCU_I2C_TH_HH */
