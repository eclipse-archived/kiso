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
 * @ingroup BSP_RNG
 * @{
 * @file
 * @brief  Implementation of RNG functions
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_RNG

#include "BCDS_BSP.h"
#include "BCDS_BSP_RNG.h"

#if BCDS_FEATURE_BSP_RNG

#include "RNG.h"
#include "BoardSettings.h"
#include "BSP_ModuleState.h"
#include "stm32l4xx_hal_rng.h"

static RNG_HandleTypeDef BSP_RNGHandle;

static volatile enum BSP_ModuleState_E ModuleState_RNG = BSP_MODULE_STATE_DISCONNECTED;
static volatile uint8_t EnableCount;

enum BSP_ModuleState_E RNG_getState(void)
{
    return ModuleState_RNG;
}

Retcode_T BSP_RNG_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;
    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_RNG)
    {
    case BSP_MODULE_STATE_DISCONNECTED:
    case BSP_MODULE_STATE_ERROR: /*In case of error state we try to connect again to go back in a defined state, will always succeed*/

        BSP_RNGHandle.Instance = RNG;

        EnableCount = 0;
        ModuleState_RNG = BSP_MODULE_STATE_CONNECTED;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_RNG_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_RNG)
    {
    case BSP_MODULE_STATE_CONNECTED:

        __HAL_RCC_RNG_CLK_ENABLE();

        if (RETCODE_OK == retcode)
        {
            ModuleState_RNG = BSP_MODULE_STATE_ENABLED;
        }
        else
        {
            ModuleState_RNG = BSP_MODULE_STATE_ERROR;
        }
        break;
    case BSP_MODULE_STATE_ENABLED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    if(RETCODE_OK == retcode)
    {
        EnableCount++;
        assert(EnableCount);
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_RNG_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;

    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_RNG)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {

            __HAL_RCC_RNG_CLK_DISABLE();

            if (RETCODE_OK == retcode)
            {
                ModuleState_RNG = BSP_MODULE_STATE_CONNECTED;
            }
            else
            {
                ModuleState_RNG = BSP_MODULE_STATE_ERROR;
            }
        }
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_RNG_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;
    BSP_MODULE_TRY_LOCK;
    switch(ModuleState_RNG)
    {
    case BSP_MODULE_STATE_CONNECTED:

        /* PUT CODE HERE */

        ModuleState_RNG = BSP_MODULE_STATE_DISCONNECTED;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
        break;
    case BSP_MODULE_STATE_ENABLED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }
    BSP_MODULE_UNLOCK;

    return retcode;
}


HWHandle_T BSP_RNG_GetHandle(void)
{
    return (HWHandle_T) &BSP_RNGHandle;
}

void HAL_RNG_MspInit(RNG_HandleTypeDef * RngHandle)
{
    BCDS_UNUSED(RngHandle);
}

void HAL_RNG_MspDeInit(RNG_HandleTypeDef * RngHandle)
{
    BCDS_UNUSED(RngHandle);
}

#endif /* BCDS_FEATURE_BSP_RNG */

/**@}*//* @ingroup BSP_RNG */
