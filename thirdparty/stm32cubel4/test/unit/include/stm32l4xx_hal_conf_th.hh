/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
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
