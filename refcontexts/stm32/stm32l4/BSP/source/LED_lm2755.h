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
 * @ingroup BSP_LED_LM2755
 * @{
 *  @file
 *  @brief Handling routines provided for Charge Pump LED Controller Module LM2755.
 */

#ifndef LED_LM2755_H_
#define LED_LM2755_H_

#include "BCDS_BSP_LED_LM2755.h"

#if BCDS_FEATURE_BSP_LED_LM2755

/**
 * @brief I2C interrupt service routine used to capture events at the I2C
 * resource and to forward them to the MCU driver for processing.
 */
void LED_LM2755_I2CEvISR(void);
/**
 * @brief I2C interrupt service routine used to capture errors at the I2C
 * resource and to forward them to the MCU driver for processing.
 */
void LED_LM2755_I2CErISR(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E LED_LM2755_getState(void);

#endif /* BCDS_FEATURE_BSP_LED_LM2755 */
#endif /* LED_LM2755_H_ */
/**@}*//* @ingroup BSP_LED_LM2755 */
