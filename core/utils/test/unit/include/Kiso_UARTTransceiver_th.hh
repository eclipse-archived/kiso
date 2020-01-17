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
 * @defgroup UARTTRANSCEIVER_TESTS UartTransceiver Unit Tests
 * @{
 *
 * @brief
 *      Mockup implementation for the @ref UARTTRANSCEIVER module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_UARTTransceiver_TH_HH_
#define KISO_UARTTransceiver_TH_HH_

/* Include gtest header file */
#include "gtest.h"

/* Wrap Kiso_UARTTransceiver interface header */
#include "Kiso_UARTTransceiver.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_Initialize, UARTTransceiver_T *, HWHandle_T, uint8_t *, uint32_t, enum UARTTransceiver_UartType_E)
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_Deinitialize, UARTTransceiver_T *)
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_Start, UARTTransceiver_T *, UARTTransceiver_EndofFrameCheckFunc_T)
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_StartInAsyncMode, UARTTransceiver_T *, UARTTransceiver_EndofFrameCheckFunc_T, UARTransceiver_Callback_T)
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_Suspend, UARTTransceiver_T *)
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_Resume, UARTTransceiver_T *)
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_Stop, UARTTransceiver_T *)
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_ReadData, UARTTransceiver_T *, uint8_t *, uint32_t, uint32_t *, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, UARTTransceiver_WriteData, UARTTransceiver_T *, const uint8_t *, uint32_t, uint32_t)
FAKE_VOID_FUNC(UARTTransceiver_LoopCallback, UARTTransceiver_T *, struct MCU_UART_Event_S)

#endif /* KISO_UARTTransceiver_TH_HH_ */

/** ************************************************************************* */
