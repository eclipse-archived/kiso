/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) 2015-2018 Bosch Connected Devices and Solutions GmbH. 
 * Copyright (C) 2019 Robert Bosch GmbH.
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
 * @brief
 * Defines MCU I2C handle
 *
 * @details
 * MCU functions do always have at least an integer parameter of type I2C_T.
 * This integer is casted by MCU to a pointer to MCU_I2C_handle_T.
 */

#ifndef BCDS_MCU_I2C_HANDLE_H_
#define BCDS_MCU_I2C_HANDLE_H_

#if BCDS_FEATURE_I2C

#include "BCDS_MCU_I2C.h"

/* TI I2C abstraction layer.  BLE SDK 3.0 C:\ti\simplelink_cc2640r2_sdk_1_00_00_22\source\ti\drivers */
#include "drivers/I2C.h"

enum MCU_I2CState_E
{
    MCU_I2C_STATE_INIT,
    MCU_I2C_STATE_READY,
    MCU_I2C_STATE_ABORTING,
    MCU_I2C_STATE_RUNNING
};

/**
 * @brief   Structure which is used as I2C handle.
 *
 * @detail  The handle is usually created by the BSP and fetched by the
 * application layer to use the I2C interface.
 */
struct MCU_I2C_Handle_S;

struct MCU_I2C_Handle_S
{
    enum MCU_I2CState_E volatile state; /**< state of the peripheral using the I2C resource*/
    I2C_Handle volatile handle; /**< Handle expected by TI I2C driver */
    enum BCDS_HAL_TransferMode_E volatile transferMode; /**< transfer mode used */
    enum BCDS_HAL_CallMode_E volatile callMode; /**< call mode used */
    void(*transferCallback)(I2C_T i2c); /**< function that will be called upon transfer completion */
    MCU_I2C_Callback_T volatile appCallback; /**< function registered by the application to receive the I2C transfer events( transfer_complete or transfer_error )*/
    Retcode_T (*readFunc)(struct MCU_I2C_Handle_S* i2c, uint16_t slaveAddr, uint8_t * data, uint32_t length); /**< SPI transfer function pointer */
    Retcode_T (*writeFunc)(struct MCU_I2C_Handle_S* i2c, uint16_t slaveAddr, uint8_t * data, uint32_t length);
    Retcode_T (*readRegisterFunc)(struct MCU_I2C_Handle_S* i2c, uint16_t slaveAddr,uint8_t registerAddr, uint8_t * data, uint32_t length);
    Retcode_T (*writeRegisterFunc)(struct MCU_I2C_Handle_S* i2c, uint16_t slaveAddr, uint8_t registerAddr,uint8_t * data, uint32_t length);
    Retcode_T (*abortFunc)(struct MCU_I2C_Handle_S* i2c);
    I2C_Transaction transaction;
    bool transferStatus;
};

#endif /* BCDS_FEATURE_I2C && BCDS_I2C_COUNT */
#endif /* BCDS_MCU_I2C_HANDLE_H_ */

