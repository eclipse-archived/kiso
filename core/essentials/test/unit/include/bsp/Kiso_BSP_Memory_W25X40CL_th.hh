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

#ifndef KISO_BSP_MEMORY_W25X40CL_TH_HH
#define KISO_BSP_MEMORY_W25X40CL_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_Memory_W25X40CL.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_MEMORY_W25X40CL

FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_Connect)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_Enable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_Disconnect)
FAKE_VALUE_FUNC(HWHandle_T, BSP_Mem_W25X40CL_GetSPIHandle)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_SetCSHigh)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_SetCSLow)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_SetHoldHigh)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_SetHoldLow)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_SetWPHigh)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_W25X40CL_SetWPLow)

#endif /* KISO_FEATURE_BSP_MEMORY_W25X40CL */
#endif /* KISO_BSP_MEMORY_W25X40CL_TH_HH */
