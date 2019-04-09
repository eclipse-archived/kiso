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

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_WATCHDOG

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_MCU_WATCHDOG.h"

#if BCDS_FEATURE_WATCHDOG

#include "BoardSettings.h"

/**
 * @ingroup BSP_BOARD
 * @{
 * @file
 * @brief  Implementation of board BSP functions
 */

static IWDG_HandleTypeDef BSP_IWDGHandle;

/*
 * @brief Initialize the independent watchdog.
 * @details This function initializes the independent watchdog timer.
 *
 * @note this api also starts the watchdog, there is no need for seperate call to enable/start the watchdog
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_IWDG_Init(void);

/*
 * @brief Configure the behavior of the watchdog during the Stop and Standby modes
 * @details The watchdog can be freeze or kept working during the stop mode of the MCU; this function is used to set the behavior
 * It will be configured to freeze in Standby mode; in Stop mode the configuration can be set by the parameter.
 *
 * @param[in] flag :
 * OB_IWDG_STOP_FREEZE      Independent watchdog counter is frozen in Stop mode
 * OB_IWDG_STOP_RUN         Independent watchdog counter is running in Stop mode
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T ConfigureWatchdogInStopMode(uint32_t flag);

/*
 * @brief Returns a handle to the configured MCU peripheral object.
 * @return HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_Watchdog_GetWdtHandle(void);

/*
 * In this function, all operations are performed even if something return Error to avoid Timing attack.
 * Indeed, if we return immediately after the first failure (example flash unlock), it could be easy for an attacker to
 * detect when if the flash control register are unlocked or not by just checking the execution time
 * Description can be find in the declaration
 */
Retcode_T ConfigureWatchdogInStopMode(uint32_t flag)
{
    Retcode_T retcode = RETCODE_OK;
    HAL_StatusTypeDef halRetcode = HAL_OK;
    /*Variable used to handle the Options Bytes*/
    static FLASH_OBProgramInitTypeDef OptionsBytesStruct;

    /* Unlock the Flash to enable the flash control register access */
    if(HAL_OK != HAL_FLASH_Unlock())
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }

    /* Clear OPTVERR bit set on virgin samples */
    /*lint -e{506} The function is provided by STM32Cube library */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    /* Unlock the Options Bytes */
    if(HAL_OK != HAL_FLASH_OB_Unlock())
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

    /* IWDG will be frozen in standby mode */
    if( (OptionsBytesStruct.USERConfig & FLASH_OPTR_IWDG_STOP) != flag ||
        (OptionsBytesStruct.USERConfig & FLASH_OPTR_IWDG_STDBY))
    {
        OptionsBytesStruct.OptionType = OPTIONBYTE_USER;
        OptionsBytesStruct.USERType = OB_USER_IWDG_STOP | OB_USER_IWDG_STDBY;
        OptionsBytesStruct.USERConfig = (flag ? FLASH_OPTR_IWDG_STOP : UINT32_C(0));
        halRetcode = HAL_FLASHEx_OBProgram(&OptionsBytesStruct);
        if(halRetcode == HAL_OK)
        {
            halRetcode = HAL_FLASH_OB_Launch();
        }

        if(halRetcode != HAL_OK)
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }

    }

    if(HAL_OK != HAL_FLASH_OB_Lock())
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }

    if(HAL_OK != HAL_FLASH_Lock())
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }

    /* Use to freeze the Watchdog during debug mode*/
    __HAL_DBGMCU_FREEZE_IWDG();

    return retcode;
}

Retcode_T Board_IWDG_Init(void)
{
    Retcode_T retcode = RETCODE_OK;
    /* Configure the watchdog timer. The timer uses LSI (32KHz) */
    /**
     * @note This api does not starts the watchdog. A call to MCU_Watchdog_Enable() is required.
     * @note For the BSE board the independent watchdog timer period is set to 32,76 seconds nominal.
     */
    BSP_IWDGHandle.Instance = IWDG;
    /* Set prescaler to 256 which results in 0.008 s/tick (32KHz/256).*/
    BSP_IWDGHandle.Init.Prescaler = IWDG_PRESCALER_256;
    /* Disable window watchdog */
    BSP_IWDGHandle.Init.Window = 4095;
    /* Set reload value to maximum. This results in 0.008 * 4095 = 32,76 s
     * for the watchdog interval.
     */
    BSP_IWDGHandle.Init.Reload = 4095;

    /** @todo 2017-02-09, BCDS/ENG1: STMicro needs to clarify why the expected
     * functional behaviour of the watchdog does not work in given low power
     * modes. The support request had been sent, analysis on their side is
     * ongoing.
     */
    retcode = ConfigureWatchdogInStopMode(OB_IWDG_STOP_FREEZE);

    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_PWR_EnableBkUpAccess();

    __HAL_DBGMCU_FREEZE_IWDG();

    return retcode;
}

HWHandle_T BSP_Watchdog_GetWdtHandle(void)
{
    return &BSP_IWDGHandle;
}

/**@}*//* @ingroup BSP_BOARD */
#endif /* if BCDS_FEATURE_WATCHDOG */
