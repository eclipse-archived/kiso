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
 * @brief provides sharing of I2C resource between different devices.
 *
 *
 * @details 
 */
#ifndef PROTECTED_I2C_H_
#define PROTECTED_I2C_H_

#include "BCDS_HAL.h"
#include "stm32/stm32l4/BCDS_MCU_STM32L4_I2C_Handle.h"

enum BSP_I2c_Devices
{
    I2C_DEVICE_BMA280_ID = 1,
    I2C_DEVICE_BME280_ID = 2,
};

struct I2CBus_S
{
    enum BSP_I2c_Devices OwnerId;
    I2C_T BusHandle;
};

extern struct MCU_I2C_S sensorsI2CStruct;

/**
 * Configures the I2C GPIOs to the desired mode
 * @param id of the device requesting the function.
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T I2C_Connect(enum BSP_I2c_Devices id);

/**
 * Enables the I2C bus according to the configuration needed from the requesting device.
 * @param id of the device requesting the function.
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T I2C_Enable(enum BSP_I2c_Devices id);

/**
 * Disables the I2C bus.
 * @param id of the device requesting the function.
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T I2C_Disable(enum BSP_I2c_Devices id);

/**
 * Configures the I2C GPIOs in the lowest power consumption safest mode.
 * @param id of the device requesting the function.
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T I2C_Disconnect(enum BSP_I2c_Devices id);

#endif /* PROTECTED_I2C_H_ */
