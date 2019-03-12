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
