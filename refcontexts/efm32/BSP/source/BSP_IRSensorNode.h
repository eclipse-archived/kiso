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
