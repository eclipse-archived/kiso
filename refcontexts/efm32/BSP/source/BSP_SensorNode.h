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
 * @defgroup BSP_XDK110_SENSOR_NODE SENSOR_NODE
 * @ingroup BCDS_HAL_BSP_IMP
 * @{
 */
/**
 *
 *  @file
 *
 *  @brief Handling routines provided for sensor node related interrupts
 *
 */

#ifndef SensorNode_H
#define SensorNode_H

#include "BCDS_BSP_SensorNode.h"

#if BCDS_FEATURE_BSP_SENSOR_NODE

/**
 * @brief I2C interrupt service routine used to capture events at the I2C
 * resource and to forward them to the MCU driver for processing.
 */
void SensorNode_I2CISR(void);
/**
 * @brief Interrupt service routine used to capture interrupts at the DMA
 * receive channel and to forward them to the MCU driver for processing.
 */
void SensorNode_DMARxISR(void);

/**
 * @brief Interrupt service routine used to capture interrupts at the DMA
 * transmit channel and to forward them to the MCU driver for processing.
 */
void SensorNode_DMATxISR(void);

#endif /* BCDS_FEATURE_BSP_SENSOR_NODE */

#endif /* SensorNode_H */

/**@}*/
