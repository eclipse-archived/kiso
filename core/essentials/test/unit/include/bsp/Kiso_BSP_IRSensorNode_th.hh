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

#ifndef KISO_BSP_IRSENSORNODE_TH_HH
#define KISO_BSP_IRSENSORNODE_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_IRSensorNode.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_IRSENSOR_NODE

FAKE_VALUE_FUNC(Retcode_T, BSP_IRSensorNode_Connect )
FAKE_VALUE_FUNC(Retcode_T, BSP_IRSensorNode_Enable)
FAKE_VALUE_FUNC(Retcode_T, BSP_IRSensorNode_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_IRSensorNode_Disconnect )
FAKE_VALUE_FUNC(HWHandle_T, BSP_IRSensorNode_GetI2CHandle )

#endif /* KISO_FEATURE_BSP_IRSENSOR_NODE */
#endif /* KISO_BSP_IRSENSORNODE_TH_HH */
