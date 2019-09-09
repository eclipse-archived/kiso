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

#ifndef KISO_BSP_NVM_TH_HH
#define KISO_BSP_NVM_TH_HH

/* Include the needed headers */
//#include "Kiso_HAL_th.hh"

#if KISO_FEATURE_BSP_NVM

//#include "BSP_Board_th.hh"

#endif /* KISO_FEATURE_BSP_NVM */

/* wrap the real interface header */
#include "Kiso_BSP_NVM.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_NVM

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

#endif /* KISO_FEATURE_BSP_NVM */
#endif /* KISO_BSP_NVM_TH_HH */
