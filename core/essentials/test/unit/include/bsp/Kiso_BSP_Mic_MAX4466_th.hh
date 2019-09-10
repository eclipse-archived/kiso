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

#ifndef KISO_BSP_MIC_MAX4466_TH_HH
#define KISO_BSP_MIC_MAX4466_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_Mic_MAX4466.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_MIC_MAX4466

FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_MAX4466_Connect)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_MAX4466_Enable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_MAX4466_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_MAX4466_Disconnect)

#endif /* KISO_FEATURE_BSP_MIC_MAX4466 */

#endif /* KISO_BSP_MIC_MAX4466_TH_HH */
