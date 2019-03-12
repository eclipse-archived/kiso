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

#ifndef BCDS_BCDS_BSP_Charger_BQ2407X_th_HH
#define BCDS_BCDS_BSP_Charger_BQ2407X_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Charger_BQ2407X.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if BCDS_FEATURE_BSP_CHARGER_BQ2407X

FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_Enable, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_GetStatus, BSP_ChargeState_T*);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_MeasureSignal, uint32_t, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_Disconnect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_SetSysoffHigh);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_SetSysoffLow);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_CheckStatus,BSP_ChargeState_T* , uint32_t );
#endif /* BCDS_FEATURE_BSP_CHARGER_BQ2407X */
#endif /* BCDS_BCDS_BSP_Charger_BQ2407X_th_HH */
