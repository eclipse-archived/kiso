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


/* module includes ********************************************************** */

#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_TEST_APP_MODULE_ID_LED

#include "LedConfig.h"
#include "Led.h"
#include <stdint.h>
#include <stdbool.h>
#include "BCDS_BSP_Board.h"
#include "BCDS_BSP_PWM_LED.h"

static Retcode_T ControlRGB_GPIOs(bool red, bool green, bool blue)
{
    BCDS_UNUSED(red);
    BCDS_UNUSED(green);
    BCDS_UNUSED(blue);
    return RETCODE_OK;
    /* @todo - upon addressing BSE-2045, integrate the latest LED. */
#if 0
    Retcode_T retcode = RETCODE_OK;

    if (RETCODE_OK == retcode)
    {
        retcode = BSP_PWM_LED_SwitchAll(0);
    }

    if (RETCODE_OK == retcode)
    {
        if (red)
        {
            retcode = BSP_PWM_LED_Switch(BSP_BSE_LED_R_PWM, 33);
        }
    }

    if (RETCODE_OK == retcode)
    {
        if (green)
        {
            retcode = BSP_PWM_LED_Switch(BSP_BSE_LED_G_PWM, 33);
        }
    }

    if (RETCODE_OK == retcode)
    {
        if (blue)
        {
            retcode = BSP_PWM_LED_Switch(BSP_BSE_LED_B_PWM, 33);
        }
    }

    return retcode;

#endif /* 0 */
}


Retcode_T Led_Initialize(void)
{
    return RETCODE_OK;
    /* @todo - upon addressing BSE-2045, integrate the latest LED. */
#if 0

    BSP_PWM_LED_Connect();

    return BSP_PWM_LED_EnableAll();
#endif /* 0 */
}

Retcode_T Led_Indication(LED_Type_T Type)
{
    Retcode_T Rc = RETCODE_OK;

	switch (Type)
	{
	case LED_GREEN:
		Rc = ControlRGB_GPIOs(false, true, false);
		break;
	case LED_RED:
		Rc = ControlRGB_GPIOs(true, false, false);
		break;
	case LED_BLUE:
		Rc = ControlRGB_GPIOs(false, false, true);
		break;
	default:
		/* LED_WHITE (all LEDs lit) is not allowed */
		Rc = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t )RETCODE_INVALID_PARAM);
		break;
	}

    return Rc;
}

/* Deprecated functions will be removed as part of PFM-659. Deprecated since vXXX (@todo: replace xxx with next version) */
Retcode_T Led_initialize(void)
{
    return (Led_Initialize());
}

/* Deprecated functions will be removed as part of PFM-659. Deprecated since vXXX (@todo: replace xxx with next version) */
Retcode_T Led_indication(LED_Type_T Type)
{
    return (Led_Indication(Type));
}
/* *************************************************************************** */
