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

#ifndef BCDS_BSP_Extension_Gpio_th_HH
#define BCDS_BSP_Extension_Gpio_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Extension_Gpio.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_EXTENSION_GPIO

FAKE_VALUE_FUNC(Retcode_T, BSP_Extension_Gpio_Connect,uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_Extension_Gpio_Disconnect, uint32_t);
FAKE_VALUE_FUNC(GPIO_T, BSP_Extension_Gpio_GetGpioById, uint32_t);

#endif /* BCDS_FEATURE_BSP_EXTENSION_GPIO */
#endif /* BCDS_BSP_Extension_Gpio_th_HH */
