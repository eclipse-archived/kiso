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
 * @ingroup BSP_BOARD
 * @{
 *  @file
 *  @brief Interfaces to the Analog Sensor module.
 */

#ifndef ANALOGSENSOR_H_
#define ANALOGSENSOR_H_

#include "BoardSettings.h"

#if BSP_FEATURE_ANALOG_SENSOR

#include "BCDS_Essentials.h"

typedef Retcode_T (*BSP_AnalogSensor_NotifyFunction)(uint32_t size, int32_t buffer[]);

typedef struct BSP_AnalogSensor_NotifyObject_S BSP_AnalogSensor_NotifyObject_T;

struct BSP_AnalogSensor_NotifyObject_S
{
    BSP_AnalogSensor_NotifyObject_T* prev;
    BSP_AnalogSensor_NotifyObject_T* next;
    BSP_AnalogSensor_NotifyFunction notifyFunction;
};

void BSP_AnalogSensor_ConversionCompleteISR(void);

Retcode_T BSP_AnalogSensor_Connect(void);
Retcode_T BSP_AnalogSensor_Disconnect(void);
Retcode_T BSP_AnalogSensor_Enable(uint32_t ids);
Retcode_T BSP_AnalogSensor_Disable(uint32_t ids);
Retcode_T BSP_AnalogSensor_PollSignals(void);
Retcode_T BSP_AnalogSensor_RegisterNotify(BSP_AnalogSensor_NotifyObject_T* object, BSP_AnalogSensor_NotifyFunction function);
Retcode_T BSP_AnalogSensor_DeregisterNotify(BSP_AnalogSensor_NotifyObject_T* object);
Retcode_T BSP_AnalogSensor_MeasureSignal(uint32_t signal, int32_t * result);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E AnalogSensor_getState(void);

#endif /* BSP_FEATURE_ANALOG_SENSOR */
#endif /* ANALOGSENSOR_H_ */
/**@}*//* @ingroup BSP_BOARD */
