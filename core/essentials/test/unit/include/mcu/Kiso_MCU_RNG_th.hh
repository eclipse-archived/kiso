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
 
#ifndef KISO_MCU_RNG_TH_HH_
#define KISO_MCU_RNG_TH_HH_

#include "gtest.h"

/* include or ban the real interface header */
#include "Kiso_MCU_RNG.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if KISO_FEATURE_RNG

FAKE_VALUE_FUNC(Retcode_T, MCU_RNG_Initialize, RNG_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_RNG_Deinitialize, RNG_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_RNG_Generate, RNG_T, uint8_t*, uint32_t);

#endif /* if KISO_FEATURE_RNG */


#endif /* KISO_MCU_RNG_TH_HH_ */
