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
