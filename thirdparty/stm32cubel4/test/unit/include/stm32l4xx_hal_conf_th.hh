/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

#ifndef STM32L4XX_HAL_CONF_TH_HH_
#define STM32L4XX_HAL_CONF_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_conf.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */
#ifdef USE_FULL_ASSERT
FAKE_VOID_FUNC(assert_failed, uint8_t*, uint32_t);
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

/* Mock-ups for the provided interfaces */

#endif /* STM32L4XX_HAL_CONF_TH_HH_ */
