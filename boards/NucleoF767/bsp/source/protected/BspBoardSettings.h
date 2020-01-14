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
 * @file    BspBoardSettings.h
 * @brief   This header file will carry the common board and MCU configurations for entire project
 *
 * @details Basically this file will carry the static configurations for the MCU. Hence this will
 *          act as the one and only interface header between the stm32 HAL library files and the
 *          BSP module.
 */

#ifndef BSPBOARDSETTINGS_H_
#define BSPBOARDSETTINGS_H_

/* Include all headers which are needed by this file. */
#include "stm32f7xx_hal.h"

/* Put the type and macro definitions here */

#define BSP_MEMSET_ZERO UINT32_C(0) ///<@details Macro to hold memory initialization for memset()

#define BSP_IO_PIN_HIGH GPIO_PIN_SET  ///<@details Macro to hold the value for pin high of stm32 devices
#define BSP_IO_PIN_LOW GPIO_PIN_RESET ///<@details Macro to hold the value for pin low of stm32 devices

#endif /* BSPBOARDSETTINGS_H_ */

/**
  * @}
  */
