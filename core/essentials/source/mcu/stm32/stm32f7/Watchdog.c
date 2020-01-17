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
 * @brief Contains the realization of the MCU watchdog interface for STM32
 */

#include "Kiso_MCU_Watchdog.h"

#if KISO_FEATURE_WATCHDOG

#include "stm32f7xx.h"
#include "stm32f7xx_hal_iwdg.h"
#include "stm32f7xx_hal_rcc.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_WATCHDOG

static IWDG_HandleTypeDef *WdgHandle_ptr = NULL;

/**
 * @brief init the hardware watchdog
 * @details The watchdog is automatically configured with a call to Board_Initialize().
 * After that it can be init using this function call.
 * @param[in] Watchdog instance handle created by and received from HAL/BSP
 * @retval  RETCODE_OK upon successful init of the watchdog.
 * @retval  RETCODE_INVALID_PARAM Error incase of failure
 */
Retcode_T MCU_Watchdog_Init(WdgHandle_T wdgHandle)
{
    Retcode_T retcode = RETCODE_OK;
    WdgHandle_ptr = (IWDG_HandleTypeDef *)wdgHandle;

    if (NULL == WdgHandle_ptr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM);
    }

    return retcode;
}

/**
 * @brief Enables the hardware watchdog
 * @details The watchdog is automatically configured with a call to Board_Initialize().
 * After that it can be enabled using this function call.
 * @retval  RETCODE_OK upon successful enabling of the watchdog.
 * @retval  RETCODE_UNINITIALIZED in case of failure
 * @see MCU_Watchdog_Feed()
 */
Retcode_T MCU_Watchdog_Enable(void)
{
    /*With new STM32Cube Lib 1.5.1 , watchdog will be started by
	 * Call to HAL_IWDG_Init API itself*/
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);

    if (NULL == WdgHandle_ptr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_UNINITIALIZED);
    }
    else
    {
        if (HAL_OK == HAL_IWDG_Init(WdgHandle_ptr))
        {
            retcode = RETCODE_OK;
        }
    }

    return retcode;
}

/**
 * @brief Feeds an enabled watchdog.
 * @details After the watchdog has been initialized and enabled this function
 * is used to feed the watchdog within a certain interval to prevent the
 * watchdog from firing.
 * @retval  RETCODE_OK upon successful feeding
 * @retval  RETCODE_UNINITIALIZED in case of failure
 */
Retcode_T MCU_Watchdog_Feed(void)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);

    if (NULL == WdgHandle_ptr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_UNINITIALIZED);
    }
    else
    {
        if (HAL_OK == HAL_IWDG_Refresh(WdgHandle_ptr))
        {
            retcode = RETCODE_OK;
        }
    }
    return retcode;
}

/**
 * @brief Checks if the last system reset was caused by the watchdog.
 * @details Use the function to check whether the last system reset
 * was caused by the watchdog. This may be necessary e.g. for a bootloader
 * to check if a watchdog reset has been occured in order to execute special
 * functions.
 * @retval true if the last system reset was caused by the watchdog. In that
 * case the function will also clear all reset flags.
 * @retval false if the last system reset was not caused by the watchdog.
 */
bool MCU_Watchdog_IsResetCausedByWatchdog(void)
{
    bool retcode = false;
    uint32_t WatchDogResetFlag;
    WatchDogResetFlag = (uint32_t)__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST);

    /* Check if the system has resumed from WWDG reset */
    if ((uint32_t)RESET != WatchDogResetFlag)
    {
        /* clear the reset flags */
        __HAL_RCC_CLEAR_RESET_FLAGS();
        retcode = true;
    }
    return retcode;
}

#endif //-- KISO_FEATURE_WATCHDOG
