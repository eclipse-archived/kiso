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
 
#ifndef KISO_KISO_UMTS_QECTELUC20_TH_HH
#define KISO_KISO_UMTS_QECTELUC20_TH_HH

#include "Kiso_HAL_th.hh"
#include "Kiso_BSP_UMTS_QuectelUC20.h"


/* Mock-ups for the provided interfaces */
#if KISO_FEATURE_BSP_UMTS_QUECTELUC20

FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_QuectelUC20_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_QuectelUC20_Disable);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_PowerOn);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_PowerOff);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_Reset);
FAKE_VALUE_FUNC(HWHandle_T, BSP_UMTS_QuectelUC20_GetUARTHandle);

#endif /* KISO_FEATURE_BSP_UMTS_QUECTELUC20 */
#endif /* KISO_KISO_UMTS_QECTELUC20_TH_HH */
