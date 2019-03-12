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

#ifndef BCDS_BCDS_BSP_Memory_S25FL116K_th_HH
#define BCDS_BCDS_BSP_Memory_S25FL116K_th_HH

/* Include the needed headers */
//#include "BCDS_HAL_th.hh"

#if BCDS_FEATURE_BSP_MEMORY_S25FL116K

//#include "BSP_Board_th.hh"

#endif /* BCDS_FEATURE_BSP_MEMORY_S25FL116K */

/* wrap the real interface header */
#include "BCDS_BSP_Memory_S25FL116K.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_MEMORY_S25FL116K

FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_Enable );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_Disable );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_Disconnect );
FAKE_VALUE_FUNC(HWHandle_T, BSP_Mem_S25FL116K_GetSPIHandle );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_SetCSHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_SetCSLow );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_SetHoldHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_SetHoldLow );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_SetWPHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_Mem_S25FL116K_SetWPLow );

#endif /* BCDS_FEATURE_BSP_MEMORY_S25FL116K */
#endif /* BCDS_BCDS_BSP_Memory_S25FL116K_th_HH */
