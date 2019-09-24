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

/**
 * @ingroup UTILS
 *
 * @defgroup I2CTRANCEIVER I2cTranceiver
 * @{
 *
 * @brief
 *      Mockup implementation for the Kiso_I2CTransceiver_th.hh module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_I2CTRANSCEIVER_TH_HH_
#define KISO_I2CTRANSCEIVER_TH_HH_

/* Include Kiso_I2CTransceiver interface header */
#include "Kiso_I2CTransceiver.h"

/* Include gtest header file */
#include "gtest.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Init, I2cTranceiverHandlePtr_T, I2C_T)
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Read, I2cTranceiverHandlePtr_T, uint8_t, uint8_t, uint8_t *, uint8_t)
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Write, I2cTranceiverHandlePtr_T, uint8_t, uint8_t, uint8_t *, uint8_t)
FAKE_VOID_FUNC(I2CTransceiver_LoopCallback, I2cTranceiverHandlePtr_T, struct MCU_I2C_Event_S)
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Deinit, I2cTranceiverHandlePtr_T)
#endif /* KISO_I2CTRANSCEIVER_TH_HH_ */
