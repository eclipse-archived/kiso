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
 * @ingroup BSP_XDK110_SENSOR_NODE
 * @{
 */
/**
 *
 *  @file
 *
 *  @brief Handling routines provided for sensor node related interrupts
 *
 */

#ifndef IRSensorNode_H
#define IRSensorNode_H

#include "BCDS_BSP_IRSensorNode.h"

#if BCDS_FEATURE_BSP_IRSENSOR_NODE

/**
 * @brief I2C interrupt service routine used to capture events at the I2C
 * resource and to forward them to the MCU driver for processing.
 */
void IRSensorNode_I2CISR(void);

void I2C1_IRQHandler(void);

#endif /* BCDS_FEATURE_BSP_IRSENSOR_NODE */

#endif /* IRSensorNode_H */

/**@}*/
