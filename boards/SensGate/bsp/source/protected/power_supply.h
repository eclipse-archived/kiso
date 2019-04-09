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
 * @file
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief Manages Powersupply rails for different board components.
 *
 * @details 
 */
#ifndef PROTECTED_POWER_SUPPLY_H_
#define PROTECTED_POWER_SUPPLY_H_

#include "BCDS_HAL.h"

enum PowerSupply_Retcode_E
{
    RETCODE_INFO_POWER_SUPPLY_ENABLED,
};

enum PowerSupply_Sensors_E
{
    POWER_SUPPLY_SENSOR_BMA280,
    POWER_SUPPLY_SENSOR_BME280,
    POWER_SUPPLY_SENSOR_COUNT
};

enum PowerSupply_CAN_E
{
    POWER_SUPPLY_LED,
    POWER_SUPPLY_CAN,
    POWER_SUPPLY_5VA,
    POWER_SUPPLY_5VB,
    POWER_SUPPLY_CAN_COUNT
};

enum PowerSupply_5VA_E
{
    POWER_SUPPLY_ONE_WIRE,
    POWER_SUPPLY_5VA_EXTI,
    POWER_SUPPLY_5VA_COUNT
};

enum PowerSupply_5VB_E
{
    POWER_SUPPLY_SDI,
    POWER_SUPPLY_5VB_EXTI,
    POWER_SUPPLY_5VB_COUNT
};

Retcode_T PowerSupply_EnablePower3V9GSM(void);
Retcode_T PowerSupply_EnablePower2V8GPS(void);
Retcode_T PowerSupply_EnablePower2V8BLE(void);
Retcode_T PowerSupply_EnablePower2V8Sensor(enum PowerSupply_Sensors_E peripheral);
Retcode_T PowerSupply_EnablePower2V8Memory(void);
Retcode_T PowerSupply_EnablePower5VCAN(enum PowerSupply_CAN_E peripheral);
Retcode_T PowerSupply_EnablePower5VA(enum PowerSupply_5VA_E peripheral);
Retcode_T PowerSupply_EnablePower5VB(enum PowerSupply_5VB_E peripheral);

Retcode_T PowerSupply_DisablePower3V9GSM(void);
Retcode_T PowerSupply_DisablePower2V8GPS(void);
Retcode_T PowerSupply_DisablePower2V8BLE(void);
Retcode_T PowerSupply_DisablePower2V8Sensor(enum PowerSupply_Sensors_E peripheral);
Retcode_T PowerSupply_DisablePower2V8Memory(void);
Retcode_T PowerSupply_DisablePower5VCAN(enum PowerSupply_CAN_E peripheral);
Retcode_T PowerSupply_DisablePower5VA(enum PowerSupply_5VA_E peripheral);
Retcode_T PowerSupply_DisablePower5VB(enum PowerSupply_5VB_E peripheral);

#endif /* PROTECTED_POWER_SUPPLY_H_ */
