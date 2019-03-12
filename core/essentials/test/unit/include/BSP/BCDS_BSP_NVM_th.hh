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

#ifndef BCDS_BCDS_BSP_NVM_th_HH
#define BCDS_BCDS_BSP_NVM_th_HH

/* Include the needed headers */
//#include "BCDS_HAL_th.hh"

#if BCDS_FEATURE_BSP_NVM

//#include "BSP_Board_th.hh"

#endif /* BCDS_FEATURE_BSP_NVM */

/* wrap the real interface header */
#include "BCDS_BSP_NVM.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_NVM

FAKE_VOID_FUNC(BSP_NVM_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_NVM_Enable );
FAKE_VALUE_FUNC(Retcode_T, BSP_NVM_Disable );
FAKE_VOID_FUNC(BSP_NVM_Disconnect );
FAKE_VALUE_FUNC(HWHandle_T, BSP_NVM_GetSPIHandle );
FAKE_VALUE_FUNC(Retcode_T, BSP_NVM_SetCSHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_NVM_SetCSLow );
FAKE_VALUE_FUNC(Retcode_T, BSP_NVM_SetHoldHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_NVM_SetHoldLow );
FAKE_VALUE_FUNC(Retcode_T, BSP_NVM_SetWPHigh );
FAKE_VALUE_FUNC(Retcode_T, BSP_NVM_SetWPLow );

#endif /* BCDS_FEATURE_BSP_NVM */
#endif /* BCDS_BCDS_BSP_NVM_th_HH */
