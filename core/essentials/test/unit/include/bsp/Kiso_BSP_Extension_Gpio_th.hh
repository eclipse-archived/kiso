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

#ifndef KISO_BSP_EXTENSION_GPIO_TH_HH
#define KISO_BSP_EXTENSION_GPIO_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_Extension_Gpio.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_EXTENSION_GPIO

FAKE_VALUE_FUNC(Retcode_T, BSP_Extension_Gpio_Connect, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_Extension_Gpio_Disconnect, uint32_t)
FAKE_VALUE_FUNC(GPIO_T, BSP_Extension_Gpio_GetGpioById, uint32_t)

#endif /* KISO_FEATURE_BSP_EXTENSION_GPIO */
#endif /* KISO_BSP_EXTENSION_GPIO_TH_HH */
