/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Robert Bosch GmbH. 
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
