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

#ifndef BCDS_I2CTRANSCEIVER_TH_HH_
#define BCDS_I2CTRANSCEIVER_TH_HH_

/* Include the real interface header */
#include "BCDS_I2CTransceiver.h"

/* Include the needed headers */

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Init,I2cTranceiverHandlePtr_T,I2C_T);
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Read,I2cTranceiverHandlePtr_T,uint8_t,uint8_t,uint8_t*,uint8_t);
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Write,I2cTranceiverHandlePtr_T,uint8_t,uint8_t,uint8_t*,uint8_t);
FAKE_VOID_FUNC(I2CTransceiver_LoopCallback, I2cTranceiverHandlePtr_T,struct MCU_I2C_Event_S );
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Deinit,I2cTranceiverHandlePtr_T);
#endif /* BCDS_I2CTRANSCEIVER_TH_HH_ */
