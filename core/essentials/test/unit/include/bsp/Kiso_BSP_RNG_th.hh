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

#ifndef KISO_BSP_RNG_TH_HH_
#define KISO_BSP_RNG_TH_HH_

/* include or ban the real interface header */
#include "Kiso_BSP_RNG.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if KISO_FEATURE_BSP_RNG

FAKE_VALUE_FUNC(Retcode_T, BSP_RNG_Connect)
FAKE_VALUE_FUNC(Retcode_T, BSP_RNG_Enable)
FAKE_VALUE_FUNC(Retcode_T, BSP_RNG_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_RNG_Disconnect)
FAKE_VALUE_FUNC(HWHandle_T, BSP_RNG_GetHandle)

#endif /* KISO_FEATURE_BSP_RNG */

#endif /* KISO_BSP_RNG_TH_HH_ */
