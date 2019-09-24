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

#ifndef KISO_KISO_BSP_Mic_AKU340_th_HH
#define KISO_KISO_BSP_Mic_AKU340_th_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_Mic_AKU340.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_MIC_AKU340

FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_AKU340_Connect)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_AKU340_Enable)
FAKE_VALUE_FUNC(int32_t, BSP_Mic_AKU340_Sense)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_AKU340_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_AKU340_Disconnect)

#endif /* KISO_FEATURE_BSP_MIC_AKU340 */

#endif /* KISO_KISO_BSP_Mic_AKU340_th_HH */
