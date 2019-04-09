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

#ifndef BCDS_BSP_H_
#define BCDS_BSP_H_

#include "BCDS_Retcode.h"
#include "BCDS_Essentials.h"

/**
 * @brief BCDS_MODULE_ID for each BSP module
 * @info  usage:
 *      #undef BCDS_MODULE_ID
 *      #define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_xxx
 */
enum BCDS_BSP_ModuleID_E
{
    BCDS_BSP_MODULE_ID_BT_EM9301 = 1,
    BCDS_BSP_MODULE_ID_BUTTON,
    BCDS_BSP_MODULE_ID_CAN_SN65HVD234,
    BCDS_BSP_MODULE_ID_GNSS_G7020,
    BCDS_BSP_MODULE_ID_PWM_BUZZER,
    BCDS_BSP_MODULE_ID_PWM_LED,
    BCDS_BSP_MODULE_ID_GSM_SIM800H,
    BCDS_BSP_MODULE_ID_MEM_W25Q256FV,
    BCDS_BSP_MODULE_ID_SENSOR_NODE,
    BCDS_BSP_MODULE_ID_CHARGER_BQ2407X,
    BCDS_BSP_MODULE_ID_TEST_INTERFACE,
    BCDS_BSP_MODULE_ID_IRQHANDLER,
    BCDS_BSP_MODULE_ID_SYSTEM_STM32,
    BCDS_BSP_MODULE_ID_BOARD,
    BCDS_BSP_MODULE_ID_WIFI_CC3100MOD,
    BCDS_BSP_MODULE_ID_SD_CARD,
    BCDS_BSP_MODULE_ID_LED,
    BCDS_BSP_MODULE_ID_UMTS_LISAU2,
    BCDS_BSP_MODULE_ID_GPS_NEO7N,
    BCDS_BSP_MODULE_ID_LORA_RN2XX3,
    BCDS_BSP_MODULE_ID_LED_LM2755,
    BCDS_BSP_MODULE_ID_DELAY_US,
    BCDS_BSP_MODULE_ID_UMTS_QUECTELUC20,
    BCDS_BSP_MODULE_ID_EXTENSION_GPIO,
    BCDS_BSP_MODULE_ID_MAX31865,
    BCDS_BSP_MODULE_ID_CHARGER_BQ2405X,
    BCDS_BSP_MODULE_ID_MIC_AKU340,
    BCDS_BSP_MODULE_ID_USB,
    BCDS_BSP_MODULE_ID_EXTENSION_PORT,
};

#endif /* BCDS_BSP_H_ */

