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

#ifndef BSP_MAX31865_H_
#define BSP_MAX31865_H_

#include "BCDS_BSP_Max31865.h"

#if BCDS_FEATURE_BSP_MAX31865

#define MAX31865_SPI_BAUDRATE                                                   UINT32_C(1000000)
#define MAX31865_CS_PIN                                                         BSP_EXTENSIONPORT_GPIO_PD8
#endif /* BCDS_FEATURE_BSP_MAX31865 */

#endif /* BSP_MAX31865_H_ */
