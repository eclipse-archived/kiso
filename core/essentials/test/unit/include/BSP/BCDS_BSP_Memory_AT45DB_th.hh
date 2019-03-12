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

#ifndef BCDS_BCDS_BSP_Memory_AT45DB_th_HH
#define BCDS_BCDS_BSP_Memory_AT45DB_th_HH

/* Include the needed headers */

/* wrap the real interface header */
#include "BCDS_BSP_Memory_AT45DB.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_MEMORY_AT45DB

FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_AT45DB_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_AT45DB_Enable );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_AT45DB_Disable );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_AT45DB_Disconnect );
FAKE_VALUE_FUNC(HWHandle_T, BSP_Mem_AT45DB_GetSPIHandle );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_AT45DB_SetCSHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_AT45DB_SetCSLow );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_AT45DB_HardReset );

#endif /* BCDS_FEATURE_BSP_MEMORY_S25FL116K */
#endif /* BCDS_BCDS_BSP_Memory_AT45DB_th_HH */
