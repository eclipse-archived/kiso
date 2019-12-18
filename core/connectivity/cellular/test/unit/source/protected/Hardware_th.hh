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

#ifndef HARDWARE_TH_HH_
#define HARDWARE_TH_HH_

#include <gtest.h>

#include "Hardware.h"

FAKE_VALUE_FUNC(Retcode_T, Hardware_Initialize, MCU_UART_Callback_T, uint8_t *)
FAKE_VALUE_FUNC(Retcode_T, Hardware_Deinitialize)
FAKE_VALUE_FUNC(Retcode_T, Hardware_Startup)
FAKE_VALUE_FUNC(Retcode_T, Hardware_Shutdown)
FAKE_VALUE_FUNC(Retcode_T, Hardware_Reset)
FAKE_VALUE_FUNC(Retcode_T, Hardware_GetCommunicationChannel, UART_T *)

#endif /* HARDWARE_TH_HH_ */
