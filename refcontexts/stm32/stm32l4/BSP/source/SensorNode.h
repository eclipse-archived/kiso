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
 * @ingroup BSP_SENSOR_NODE
 * @{
 *  @file
 *  @brief Handling routines provided for sensor node related interrupts.
 */

#ifndef SENSORNODE_H_
#define SENSORNODE_H_

#include "BCDS_BSP_SensorNode.h"

#if BCDS_FEATURE_BSP_SENSOR_NODE

/**
 * @brief Gets status of the BMI160
 */
enum BSP_ModuleState_E SensorNode_BMI160_getState(void);

/**
 * @brief Gets status of the BMM150
 */
enum BSP_ModuleState_E SensorNode_BMM150_getState(void);

/**
 * @brief Gets status of the BMP280
 */
enum BSP_ModuleState_E SensorNode_BMP280_getState(void);

/**
 * @brief I2C interrupt service routine used to capture events at the I2C
 * resource and to forward them to the MCU driver for processing.
 */
void SensorNode_I2CEvISR(void);
/**
 * @brief I2C interrupt service routine used to capture errors at the I2C
 * resource and to forward them to the MCU driver for processing.
 */
void SensorNode_I2CErISR(void);

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

/**
 * @brief Interrupt service routine used to capture interrupts at the external
 * interrupt line 3 and to forward them to the Application processing.
 */
void SensorNode_EXTI3ISR(void);

void SensorNode_EXTI15_10ISR(void);

#endif /* BCDS_FEATURE_BSP_SENSOR_NODE */


#endif /* SENSORNODE_H_ */
/**@}*//* @ingroup BSP_SENSOR_NODE */
