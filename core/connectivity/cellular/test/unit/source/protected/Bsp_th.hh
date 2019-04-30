/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

#ifndef CELLULAR_BSP_TH_HH
#define CELLULAR_BSP_TH_HH

/* Include the needed headers */
#include <gtest.h>
#include "BCDS_MCU_UART_th.hh"
#include "BCDS_BSP_UMTS_LisaU2_th.hh"

#include "CellularBsp.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CellularBsp_Init, MCU_UART_Callback_T, uint8_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularBsp_Deinit);
FAKE_VALUE_FUNC(Retcode_T, CellularBsp_Startup);
FAKE_VALUE_FUNC(Retcode_T, CellularBsp_Shutdown);
FAKE_VALUE_FUNC(Retcode_T, CellularBsp_Reset);
FAKE_VALUE_FUNC(Retcode_T, CellularBsp_GetCommunicationChannel, UART_T*);

#endif /* CELLULAR_BSP_TH_HH */
