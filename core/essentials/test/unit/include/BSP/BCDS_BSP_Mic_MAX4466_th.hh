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
 
#ifndef BCDS_BCDS_BSP_Mic_MAX4466_th_HH
#define BCDS_BCDS_BSP_Mic_MAX4466_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Mic_MAX4466.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_MIC_MAX4466

FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_MAX4466_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_MAX4466_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_MAX4466_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_MAX4466_Disconnect);

#endif /* BCDS_FEATURE_BSP_MIC_MAX4466 */

#endif /* BCDS_BCDS_BSP_Mic_MAX4466_th_HH */
