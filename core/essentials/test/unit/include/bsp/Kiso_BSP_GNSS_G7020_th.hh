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

#ifndef KISO_BSP_GNSS_G7020_TH_HH
#define KISO_BSP_GNSS_G7020_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_GNSS_G7020.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_GNSS_G7020

FAKE_VALUE_FUNC(Retcode_T, BSP_GNSS_G7020_Connect)
FAKE_VALUE_FUNC(Retcode_T, BSP_GNSS_G7020_Enable)
FAKE_VALUE_FUNC(Retcode_T, BSP_GNSS_G7020_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_GNSS_G7020_Disconnect)
FAKE_VALUE_FUNC(Retcode_T, BSP_GNSS_G7020_Reset)
FAKE_VALUE_FUNC(HWHandle_T, BSP_GNSS_G7020_GetUARTHandle)

#endif /* KISO_FEATURE_BSP_GNSS_G7020 */
#endif /* KISO_BSP_GNSS_G7020_TH_HH */
