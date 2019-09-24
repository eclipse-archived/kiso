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
 * @file
 *
 *
 *  Mockup implementation for the Kiso MCU SPI module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef KISO_MCU_SPI_TH_HH_
#define KISO_MCU_SPI_TH_HH_

#include "gtest.h"

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_MCU_SPI.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

#if KISO_FEATURE_SPI

FAKE_VALUE_FUNC(Retcode_T, MCU_SPI_Initialize, SPI_T, MCU_SPI_Callback_T)
FAKE_VALUE_FUNC(Retcode_T, MCU_SPI_Deinitialize, SPI_T)
FAKE_VALUE_FUNC(Retcode_T, MCU_SPI_Send, SPI_T, uint8_t *, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, MCU_SPI_Receive, SPI_T, uint8_t *, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, MCU_SPI_Transfer, SPI_T, uint8_t *, uint8_t *, uint32_t)
FAKE_VALUE_FUNC(uint32_t, MCU_SPI_GetDataCount, SPI_T)

#endif /* KISO_FEATURE_SPI */
#endif /* KISO_MCU_SPI_TH_HH_*/
