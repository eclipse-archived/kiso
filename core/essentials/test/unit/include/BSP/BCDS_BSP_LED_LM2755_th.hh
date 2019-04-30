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

#ifndef BCDS_BSP_LED_LM2755_TH_HH_
#define BCDS_BSP_LED_LM2755_TH_HH_

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_LED_LM2755.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_LED_LM2755

FAKE_VALUE_FUNC(Retcode_T, BSP_LED_LM2755_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_LM2755_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_LM2755_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_LM2755_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_LED_LM2755_GetI2CHandle );

#endif /* BCDS_FEATURE_BSP_LED_LM2755 */

#endif /* BCDS_BSP_LED_LM2755_TH_HH_ */
