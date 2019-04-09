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
 * @ingroup BSP_BOARD
 * @{
 * @file
 * @brief  Implementation of board BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_LDO_TLV70233

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BoardSettings.h"

#if BSP_FEATURE_LDO_TLV70233

#include "BSP_HwConfig.h"
#include "LDO_tlv70233.h"
#include "BSP_ModuleState.h"
#include "GNSS_g7020.h"
#include "Memory_w25q256fv.h"
#include "SensorNode.h"
#include "CAN_sn65hvd234.h"
#include "PWM_Buzzer.h"
#include "LED_lm2755.h"


static volatile enum BSP_ModuleState_E ModuleState_LDO_TLV70233 = BSP_MODULE_STATE_DISCONNECTED;
static volatile uint8_t EnableCount;

enum BSP_ModuleState_E LDO_TLV70233_getState(void)
{
    return ModuleState_LDO_TLV70233;
}

static Retcode_T Discharge_3V3PowerLine(void)
{
#if BCDS_FEATURE_BSP_LED_LM2755
    /* It is assumed that the I2C2 pins are not used now */

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    BOARD_GPIO_CLOCK_ENABLE();

    HAL_GPIO_DeInit(I2C2_CLK_PORT, I2C2_CLK_PIN);
    HAL_GPIO_DeInit(I2C2_DATA_PORT, I2C2_DATA_PIN);

    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStruct.Pin = I2C2_CLK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = I2C2_CLK_PULL;
    GPIO_InitStruct.Speed = I2C2_CLK_SPEED;
    HAL_GPIO_Init(I2C2_CLK_PORT, &GPIO_InitStruct);

    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStruct.Pin = I2C2_DATA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = I2C2_DATA_PULL;
    GPIO_InitStruct.Speed = I2C2_DATA_SPEED;
    HAL_GPIO_Init(I2C2_DATA_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(I2C2_DATA_PORT, I2C2_DATA_PIN, BSP_IO_PIN_LOW);
    HAL_GPIO_WritePin(I2C2_CLK_PORT, I2C2_CLK_PIN, BSP_IO_PIN_LOW);
    HAL_Delay(HW_3V3_DISCHARGE_TIME);

    HAL_GPIO_DeInit(I2C2_CLK_PORT, I2C2_CLK_PIN);
    HAL_GPIO_DeInit(I2C2_DATA_PORT, I2C2_DATA_PIN);

#endif /* BCDS_FEATURE_BSP_LED_LM2755 */
    return RETCODE_OK;
}

Retcode_T BSP_LDO_TLV70233_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    GPIO_InitTypeDef GPIO_InitStruct;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_LDO_TLV70233)
    {
    case BSP_MODULE_STATE_DISCONNECTED:
    case BSP_MODULE_STATE_ERROR: /*In case of error state we try to connect again to go back in a defined state, will always succeed*/

        /* GPIO Ports Clock Enable */
        BOARD_GPIO_CLOCK_ENABLE();

        GPIO_InitStruct.Pin = PWR3V3_EN_PIN;
        GPIO_InitStruct.Mode = PWR3V3_EN_MODE;
        GPIO_InitStruct.Pull = PWR3V3_EN_PULL;
        GPIO_InitStruct.Speed = PWR3V3_EN_SPEED;
        HAL_GPIO_Init(PWR3V3_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(PWR3V3_EN_PORT, PWR3V3_EN_PIN, BSP_IO_PIN_LOW);

        EnableCount = 0;
        ModuleState_LDO_TLV70233 = BSP_MODULE_STATE_CONNECTED;
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        retcode = RETCODE_OK;
        break;
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }
    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_LDO_TLV70233_Enable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    switch(ModuleState_LDO_TLV70233)
    {
    case BSP_MODULE_STATE_CONNECTED:

        HAL_GPIO_WritePin(PWR3V3_EN_PORT, PWR3V3_EN_PIN, BSP_IO_PIN_HIGH);

        HAL_Delay(HW_LDO_TLV70233_STARTUP_TIME);

        ModuleState_LDO_TLV70233 = BSP_MODULE_STATE_ENABLED;
        /* FALL-THROUGH */
    case BSP_MODULE_STATE_ENABLED: /* FALL-THROUGH */
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

Retcode_T BSP_LDO_TLV70233_Disable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    switch(ModuleState_LDO_TLV70233)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {
            if(
#if BCDS_FEATURE_BSP_SENSOR_NODE
                (BSP_MODULE_STATE_ENABLED == SensorNode_BMM150_getState()) ||
                (BSP_MODULE_STATE_ENABLED == SensorNode_BMP280_getState()) ||
#endif
#if BCDS_FEATURE_BSP_CAN_SN65HVD234
                (BSP_MODULE_STATE_ENABLED == CAN_SN65HVD234_getState()) ||
#endif
#if BCDS_FEATURE_BSP_PWM_BUZZER
                (BSP_MODULE_STATE_ENABLED == PWM_Buzzer_getState()) ||
#endif
#if BCDS_FEATURE_BSP_GNSS_G7020
                (BSP_MODULE_STATE_ENABLED == GNSS_G7020_getState()) ||
#endif
#if BCDS_FEATURE_BSP_LED_LM2755
                (BSP_MODULE_STATE_ENABLED == LED_LM2755_getState()) ||
#endif
#if BCDS_FEATURE_BSP_MEMORY_W25Q256FV
                (BSP_MODULE_STATE_ENABLED == Mem_W25Q256FV_getState()) ||
#endif
                0)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
            }
            else
            {
                HAL_GPIO_WritePin(PWR3V3_EN_PORT, PWR3V3_EN_PIN, BSP_IO_PIN_LOW);
                retcode = Discharge_3V3PowerLine();
            }
            if (RETCODE_OK == retcode)
            {
                ModuleState_LDO_TLV70233 = BSP_MODULE_STATE_CONNECTED;
            }
            else
            {
                ModuleState_LDO_TLV70233 = BSP_MODULE_STATE_ERROR;
            }
        }
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_LDO_TLV70233_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    switch(ModuleState_LDO_TLV70233)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /* GPIO Ports Clock Enable */
        BOARD_GPIO_CLOCK_ENABLE();

        HAL_GPIO_DeInit(PWR3V3_EN_PORT, PWR3V3_EN_PIN);

        ModuleState_LDO_TLV70233 = BSP_MODULE_STATE_DISCONNECTED;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
        break;
    case BSP_MODULE_STATE_ENABLED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

#endif /* BSP_FEATURE_LDO_TLV70233 */
/**@}*//* @ingroup BSP_BOARD */
