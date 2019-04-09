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
 * @defgroup BSP_XDK110_LED LED
 * @ingroup BCDS_HAL_BSP_IMP
 * @{
 */
/**
 * @file
 * @brief  Implementation of PWM LED BSP functions
 */

/* This is used to define the correct module ID for generation of module
 * error codes
 */

#include "BCDS_BSP.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_LED

#include "BCDS_HAL.h"
#if BCDS_FEATURE_BSP_LED

#include "BSP_BoardSettings.h"
#include "BSP_BoardShared.h"
#include "BSP_BoardType.h"
#include "BCDS_BSP_LED.h"

enum LED_Status_E
{
    LED_DISABLED = 0,
    LED_ENABLED,
};

static enum LED_Status_E LED_O_Status;
static enum LED_Status_E LED_R_Status;
static enum LED_Status_E LED_Y_Status;
/*
 * Refer to interface for description
 */
Retcode_T BSP_LED_Connect(void)
{
    GPIO_PinModeSet(LED_ORANGE_PWM_PORT, LED_ORANGE_PWM_PIN, gpioModePushPullDrive, 0);
    GPIO_PinModeSet(LED_YELLOW_PWM_PORT, LED_YELLOW_PWM_PIN, gpioModePushPullDrive, 0);
    GPIO_PinModeSet(LED_RED_PWM_PORT, LED_RED_PWM_PIN, gpioModePushPullDrive, 0);
    return RETCODE_OK;
}

/*
 * Refer to interface for description
 */
Retcode_T BSP_LED_Enable(uint32_t id)
{
    Retcode_T retcode = RETCODE_OK;
    switch (id)
    {
        case BSP_XDK_LED_R:
            LED_R_Status = LED_ENABLED;
            break;
        case BSP_XDK_LED_Y:
            LED_Y_Status = LED_ENABLED;
            break;
        case BSP_XDK_LED_O:
            LED_O_Status = LED_ENABLED;
            break;
        default:
        	retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    return retcode;
}

/*
 * Refer to interface for description
 */
Retcode_T BSP_LED_EnableAll(void)
{
    LED_R_Status = LED_ENABLED;
    LED_Y_Status = LED_ENABLED;
    LED_O_Status = LED_ENABLED;
    return RETCODE_OK;
}
/*
 * Refer to interface for description
 */
Retcode_T BSP_LED_Switch(uint32_t id, uint32_t command)
{
    Retcode_T retcode = RETCODE_OK;
    switch (id)
    {
        case BSP_XDK_LED_R:
            if (LED_R_Status == LED_ENABLED)
            {
                if ((uint32_t)BSP_LED_COMMAND_ON == command)
                {
                    GPIO_PinOutSet(LED_RED_PWM_PORT, LED_RED_PWM_PIN);
                }
                else if ((uint32_t)BSP_LED_COMMAND_OFF == command)
                {
                    GPIO_PinOutClear(LED_RED_PWM_PORT, LED_RED_PWM_PIN);
                }
                else if((uint32_t)BSP_LED_COMMAND_TOGGLE == command)
                {
                    GPIO_PinOutToggle(LED_RED_PWM_PORT, LED_RED_PWM_PIN);
                }
                else
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                }
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LED_DISABLED);
            }
            break;
        case BSP_XDK_LED_O:
            if (LED_O_Status == LED_ENABLED)
            {
                if ((uint32_t)BSP_LED_COMMAND_ON == command)
                {
                    GPIO_PinOutSet(LED_ORANGE_PWM_PORT, LED_ORANGE_PWM_PIN);
                }
                else if ((uint32_t)BSP_LED_COMMAND_OFF == command)
                {
                    GPIO_PinOutClear(LED_ORANGE_PWM_PORT, LED_ORANGE_PWM_PIN);
                }
                else if ((uint32_t)BSP_LED_COMMAND_TOGGLE == command)
                {
                    GPIO_PinOutToggle(LED_ORANGE_PWM_PORT, LED_ORANGE_PWM_PIN);
                }
                else
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                }
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LED_DISABLED);
            }
            break;
        case BSP_XDK_LED_Y:
            if (LED_Y_Status == LED_ENABLED)
            {
                if ((uint32_t)BSP_LED_COMMAND_ON == command)
                {
                    GPIO_PinOutSet(LED_YELLOW_PWM_PORT, LED_YELLOW_PWM_PIN);
                }
                else if ((uint32_t)BSP_LED_COMMAND_OFF == command)
                {
                    GPIO_PinOutClear(LED_YELLOW_PWM_PORT, LED_YELLOW_PWM_PIN);
                }
                else if ((uint32_t)BSP_LED_COMMAND_TOGGLE == command)
                {
                    GPIO_PinOutToggle(LED_YELLOW_PWM_PORT, LED_YELLOW_PWM_PIN);
                }
                else
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                }
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LED_DISABLED);
            }
            break;
        default:
        	retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    return retcode;
}

/*
 * Refer to interface for description
 */
Retcode_T BSP_LED_SwitchAll(uint32_t command)
{
    Retcode_T retcode = RETCODE_OK;
    if ((uint32_t)BSP_LED_COMMAND_ON == command)
    {
        if (LED_ENABLED == LED_R_Status)
        {
            GPIO_PinOutSet(LED_RED_PWM_PORT, LED_RED_PWM_PIN);
        }

        if (LED_ENABLED == LED_O_Status)
        {
            GPIO_PinOutSet(LED_ORANGE_PWM_PORT, LED_ORANGE_PWM_PIN);
        }

        if (LED_ENABLED == LED_Y_Status)
        {
            GPIO_PinOutSet(LED_YELLOW_PWM_PORT, LED_YELLOW_PWM_PIN);
        }
    }
    else if ((uint32_t)BSP_LED_COMMAND_OFF == command)
    {
        if (LED_ENABLED == LED_R_Status)
        {
            GPIO_PinOutClear(LED_RED_PWM_PORT, LED_RED_PWM_PIN);
        }
        if (LED_ENABLED == LED_O_Status)
        {
            GPIO_PinOutClear(LED_ORANGE_PWM_PORT, LED_ORANGE_PWM_PIN);
        }
        if (LED_ENABLED == LED_Y_Status)
        {
            GPIO_PinOutClear(LED_YELLOW_PWM_PORT, LED_YELLOW_PWM_PIN);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    return retcode;
}
/*
 * Refer to interface for description
 */
Retcode_T BSP_LED_Disable(uint32_t id)
{
    Retcode_T retcode = RETCODE_OK;
    switch (id)
    {
        case BSP_XDK_LED_R:
            LED_R_Status = LED_DISABLED;
            break;
        case BSP_XDK_LED_Y:
            LED_Y_Status = LED_DISABLED;
            break;
        case BSP_XDK_LED_O:
            LED_O_Status = LED_DISABLED;
            break;
        default:
        	retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    return retcode;
}

/*
 * Refer to interface for description
 */
Retcode_T BSP_LED_DisableAll(void)
{
    LED_O_Status = LED_DISABLED;
    LED_Y_Status = LED_DISABLED;
    LED_R_Status = LED_DISABLED;
    return RETCODE_OK;
}

/*
 * Refer to interface for description
 */
Retcode_T BSP_LED_Disconnect(void)
{
    GPIO_PinModeSet(LED_ORANGE_PWM_PORT, LED_ORANGE_PWM_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(LED_YELLOW_PWM_PORT, LED_YELLOW_PWM_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(LED_RED_PWM_PORT, LED_RED_PWM_PIN, gpioModeDisabled, 0);
    return RETCODE_OK;
}

#endif /* BCDS_FEATURE_BSP_LED */

/**@}*/
