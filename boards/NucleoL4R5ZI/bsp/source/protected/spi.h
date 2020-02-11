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
 * @brief provides sharing of SPI resource between different devices.
 *
 *
 * @details
 */
#ifndef PROTECTED_SPI_H_
#define PROTECTED_SPI_H_

#include "Kiso_HAL.h"
#include "BSP_NucleoL4R5ZI.h"
#include "stm32/stm32l4/Kiso_MCU_STM32L4_SPI_Handle.h"

struct SPIBus_S
{
    enum BSP_SPI_Devices OwnerId;
    SPI_T BusHandle;
};

extern struct MCU_SPI_S nucleoSpiStruct;

/**
* Configures the I2C GPIOs to the desired mode
* @param id of the device requesting the function.
* @return RETCODE_OK in case of success error code otherwise.
*/
Retcode_T SPI_Connect(enum BSP_SPI_Devices id);

/**
* Enables the I2C bus according to the configuration needed from the requesting device.
* @param id of the device requesting the function.
* @return RETCODE_OK in case of success error code otherwise.
*/
Retcode_T SPI_Enable(enum BSP_SPI_Devices id);

/**
* Disables the I2C bus.
* @param id of the device requesting the function.
* @return RETCODE_OK in case of success error code otherwise.
*/
Retcode_T SPI_Disable(enum BSP_SPI_Devices id);

/**
* Configures the I2C GPIOs in the lowest power consumption safest mode.
* @param id of the device requesting the function.
* @return RETCODE_OK in case of success error code otherwise.
*/
Retcode_T SPI_Disconnect(enum BSP_SPI_Devices id);

HWHandle_T BSP_TestInterface_GetSPIHandle(void);

#endif /* PROTECTED_SPI_H_ */
