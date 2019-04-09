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
 * @ingroup BSP_CAN_MODULE
 * @{
 * @file
 * @brief  Implementation of CAN BSP function.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_CAN_SN65HVD234

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_CAN_SN65HVD234.h"

#if BCDS_FEATURE_BSP_CAN_SN65HVD234

#include "BoardSettings.h"
#include "CAN_sn65hvd234.h"
#include "LDO_tlv70233.h"
#include "BSP_ModuleState.h"

static volatile enum BSP_ModuleState_E ModuleState_CAN_SN65HVD234 = BSP_MODULE_STATE_DISCONNECTED;
static volatile uint8_t EnableCount;

enum BSP_ModuleState_E CAN_SN65HVD234_getState(void)
{
    return ModuleState_CAN_SN65HVD234;
}


/* Refer to interface header for description */
Retcode_T BSP_CAN_SN65HVD234_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    GPIO_InitTypeDef GPIO_InitStruct;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_CAN_SN65HVD234)
    {
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH: */
    case BSP_MODULE_STATE_DISCONNECTED:
        /* Set the priorities for the CAN communication */
        HAL_NVIC_SetPriority(CAN1_RX1_IRQn, CAN1_RX1_PREEMPT_PRIORITY, CAN1_RX1_SUBPRIORITY);
        HAL_NVIC_SetPriority(CAN1_TX_IRQn, CAN1_TX_PREEMPT_PRIORITY, CAN1_TX_SUBPRIORITY);
        HAL_NVIC_SetPriority(CAN1_SCE_IRQn, CAN1_SCE_PREEMPT_PRIORITY, CAN1_SCE_SUBPRIORITY);

        /* Enabling GPIO clocks required by the CAN module */
        CAN_GPIO_CLOCK_ENABLE();

        GPIO_InitStruct.Pin = CAN_EN_PIN;
        GPIO_InitStruct.Mode = CAN_EN_MODE;
        GPIO_InitStruct.Pull = CAN_EN_PULL;
        GPIO_InitStruct.Speed = CAN_EN_SPEED;
        HAL_GPIO_Init(CAN_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(CAN_EN_PORT, CAN_EN_PIN, GPIO_PIN_RESET);

        GPIO_InitStruct.Pin = CAN_RX_PIN;
        GPIO_InitStruct.Mode = CAN_RX_MODE;
        GPIO_InitStruct.Pull = CAN_RX_PULL;
        GPIO_InitStruct.Speed = CAN_RX_SPEED;
        GPIO_InitStruct.Alternate = CAN_RX_ALTERNATE;
        HAL_GPIO_Init(CAN_RX_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = CAN_TX_PIN;
        GPIO_InitStruct.Mode = CAN_TX_MODE;
        GPIO_InitStruct.Pull = CAN_TX_PULL;
        GPIO_InitStruct.Speed = CAN_TX_SPEED;
        GPIO_InitStruct.Alternate = CAN_TX_ALTERNATE;
        HAL_GPIO_Init(CAN_TX_PORT, &GPIO_InitStruct);

        EnableCount = 0;
        ModuleState_CAN_SN65HVD234 = BSP_MODULE_STATE_CONNECTED;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH: */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_CAN_SN65HVD234_Enable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_CAN_SN65HVD234)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = BSP_LDO_TLV70233_Enable();
        if (RETCODE_OK == retcode)
        {
            /* Enable the transceiver */
            HAL_Delay(CAN_POWER_ON_DELAY);
            HAL_GPIO_WritePin(CAN_EN_PORT, CAN_EN_PIN, GPIO_PIN_SET);
            HAL_Delay(CAN_ENABLE_DELAY);
            __HAL_RCC_CAN1_CLK_ENABLE();
        }

        if (RETCODE_OK == retcode)
        {
            ModuleState_CAN_SN65HVD234 = BSP_MODULE_STATE_ENABLED;
        }
        else
        {
            ModuleState_CAN_SN65HVD234 = BSP_MODULE_STATE_ERROR;
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

Retcode_T BSP_CAN_SN65HVD234_EnableIRQ(void)
{
    BSP_MODULE_TRY_LOCK;
    /* Enable the interrupt requests for the CAN */
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

    BSP_MODULE_UNLOCK;
    return RETCODE_OK;
}

Retcode_T BSP_CAN_SN65HVD234_DisableIRQ(void)
{
    BSP_MODULE_TRY_LOCK;
    /* Disable the interrupt requests for the CAN */
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);

    BSP_MODULE_UNLOCK;
    return RETCODE_OK;
}

Retcode_T BSP_CAN_SN65HVD234_Disable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_CAN_SN65HVD234)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {
            HAL_GPIO_WritePin(CAN_EN_PORT, CAN_EN_PIN, GPIO_PIN_RESET);
            __HAL_RCC_CAN1_CLK_DISABLE();

            if (RETCODE_OK == retcode)
            {
                ModuleState_CAN_SN65HVD234 = BSP_MODULE_STATE_CONNECTED;
                retcode = BSP_LDO_TLV70233_Disable();
            }

            if (RETCODE_OK != retcode)
            {
                ModuleState_CAN_SN65HVD234 = BSP_MODULE_STATE_ERROR;
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

Retcode_T BSP_CAN_SN65HVD234_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_CAN_SN65HVD234)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /* Enabling GPIO clocks required by the CAN module */
        CAN_GPIO_CLOCK_ENABLE();

        /* Put the CAN related pins into analog mode */
        HAL_GPIO_DeInit(CAN_EN_PORT, CAN_EN_PIN);
        HAL_GPIO_DeInit(CAN_RX_PORT, CAN_RX_PIN);
        HAL_GPIO_DeInit(CAN_TX_PORT, CAN_TX_PIN);

        ModuleState_CAN_SN65HVD234 = BSP_MODULE_STATE_DISCONNECTED;
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
        retcode = RETCODE_OK;
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

#endif /* if BCDS_FEATURE_BSP_CAN_SN65HVD234 */
/**@}*//* @ingroup BSP_CAN_MODULE */
