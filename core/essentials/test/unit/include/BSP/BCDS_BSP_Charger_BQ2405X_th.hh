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
 
#ifndef BCDS_BCDS_BSP_Charger_BQ2405X_th_HH
#define BCDS_BCDS_BSP_Charger_BQ2405X_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Charger_BQ2405X.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_CHARGER_BQ2405X

FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2405X_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2405X_Enable, uint32_t);
FAKE_VALUE_FUNC(uint32_t, BSP_Charger_BQ2405X_GetStatus);
FAKE_VALUE_FUNC(uint32_t, BSP_Charger_BQ2405X_GetIOState, uint32_t);
FAKE_VALUE_FUNC(int32_t, BSP_Charger_BQ2405X_MeasureSignal, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2405X_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2405X_Disconnect);

#endif /* BCDS_FEATURE_BSP_CHARGER_BQ2405X */

#endif /* BCDS_BCDS_BSP_Charger_BQ2405X_th_HH */
