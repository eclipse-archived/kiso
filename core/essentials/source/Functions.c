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

/**
 * @file Functions.c
 *
 * @brief Diverse useful Functions e.g. Cmsis Functions for upper Modules
 *
 */

#include "Kiso_Essentials.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_HAL_MODULE_ID_CRITICALSECTION

/* additional interface header files */
#include "Kiso_HAL.h"
#include "Cmsis.h"

#ifdef __GNUC__
#ifdef ti_cc26xx
#ifndef __ARM_PROFILE_M__
#define __ARM_PROFILE_M__ 1
#endif /* #ifndef __ARM_PROFILE_M__ */
/* The __get_IPSR is defined in the below header which is supported for __ARM_PROFILE_M__ only.
 * If __ARM_PROFILE_M__ is not enabled, then TI will need a different implementation. */
#include "intrinsics.h"
#endif /* ti_cc26xx */

bool HAL_IsInISR(void)
{
    if (0 != __get_IPSR())
    {
        return true;
    }
    return false;
}

#endif
