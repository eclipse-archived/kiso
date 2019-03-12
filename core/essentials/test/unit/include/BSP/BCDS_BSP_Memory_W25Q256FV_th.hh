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
