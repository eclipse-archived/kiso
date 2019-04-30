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
 
#ifndef BCDS_BCDS_UMTS_QECTELUC20_TH_HH
#define BCDS_BCDS_UMTS_QECTELUC20_TH_HH

#include "BCDS_HAL_th.hh"
#include "BCDS_BSP_UMTS_QuectelUC20.h"


/* Mock-ups for the provided interfaces */
#if BCDS_FEATURE_BSP_UMTS_QUECTELUC20

FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_QuectelUC20_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_QuectelUC20_Disable);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_PowerOn);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_PowerOff);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_Reset);
FAKE_VALUE_FUNC(HWHandle_T, BSP_UMTS_QuectelUC20_GetUARTHandle);

#endif /* BCDS_FEATURE_BSP_UMTS_QUECTELUC20 */
#endif /* BCDS_BCDS_UMTS_QECTELUC20_TH_HH */
