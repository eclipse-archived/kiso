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

#ifndef BCDS_BCDS_BSP_Memory_W25Q256FV_th_HH
#define BCDS_BCDS_BSP_Memory_W25Q256FV_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Memory_W25Q256FV.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_MEMORY_W25Q256FV

FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_Enable );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_Disable );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_Disconnect );
FAKE_VALUE_FUNC(HWHandle_T, BSP_Mem_W25Q256FV_GetSPIHandle );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_SetCSHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_SetCSLow );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_SetHoldHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_SetHoldLow );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_SetWPHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25Q256FV_SetWPLow );

#endif /* BCDS_FEATURE_BSP_MEMORY_W25Q256FV */
#endif /* BCDS_BCDS_BSP_Memory_W25Q256FV_th_HH */
