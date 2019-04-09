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
 * @brief Contains the realization of the MCU I2C interface for EFM32
 */

#include "BCDS_MCU_GPIO.h"

#if BCDS_FEATURE_GPIO

#include "BCDS_MCU_GPIO_Handle.h"

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_MODULE_ID_GPIO

Retcode_T MCU_GPIO_Initialize(GPIO_T gpio)
{
    MCU_GPIO_Handle_T* gpio_ptr = (MCU_GPIO_Handle_T *) gpio;

    GPIO_PinModeSet(gpio_ptr->Port, gpio_ptr->Pin, gpio_ptr->Mode, gpio_ptr->InitialState);
    return RETCODE_OK;
}

Retcode_T MCU_GPIO_DeInitialize(GPIO_T gpio)
{
    BCDS_UNUSED(gpio);
    return RETCODE_OK;
}

Retcode_T MCU_GPIO_WritePin(GPIO_T gpio, MCU_GPIO_PinState_T gpioLevel)
{
    Retcode_T retcode=RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    MCU_GPIO_Handle_T* gpio_ptr = (MCU_GPIO_Handle_T *) gpio;
    if(NULL != gpio_ptr)
    {
        if(MCU_GPIO_PIN_STATE_HIGH == gpioLevel)
        {
            GPIO_PinOutSet(gpio_ptr->Port, gpio_ptr->Pin);
        }
        else if(MCU_GPIO_PIN_STATE_LOW == gpioLevel)
        {
            GPIO_PinOutClear(gpio_ptr->Port, gpio_ptr->Pin);
        }
        else
        {
          return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        }
        retcode = RETCODE_OK;
    }
    return retcode;
}

Retcode_T MCU_GPIO_ReadPin(GPIO_T gpio, MCU_GPIO_PinState_T *gpioLevel)
{
    Retcode_T retcode=RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    uint32_t PinGet;
    MCU_GPIO_Handle_T* gpio_ptr = (MCU_GPIO_Handle_T*) gpio;
    if(NULL != gpio_ptr && NULL!= gpioLevel)
    {
        PinGet = GPIO_PinInGet(gpio_ptr->Port, gpio_ptr->Pin);
        if(PinGet)
        {
            *gpioLevel = MCU_GPIO_PIN_STATE_HIGH;
        }
        else
        {
            *gpioLevel = MCU_GPIO_PIN_STATE_LOW;
        }
        retcode = RETCODE_OK;
    }
    return retcode;
}

Retcode_T MCU_GPIO_TogglePin(GPIO_T gpio)
{
    Retcode_T retcode=RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    MCU_GPIO_Handle_T* gpio_ptr = (MCU_GPIO_Handle_T*) gpio;
    if(NULL != gpio_ptr)
    {
        GPIO_PinOutToggle(gpio_ptr->Port, gpio_ptr->Pin);
        retcode = RETCODE_OK;
    }
    return retcode;
}
#endif /* BCDS_FEATURE_GPIO */
