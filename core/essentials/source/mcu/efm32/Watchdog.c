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
 * @file
 * @brief Contains the realization of the MCU watchdog interface for EFM32
 */

#include "BCDS_MCU_Watchdog.h"

#if BCDS_FEATURE_WATCHDOG

#include "em_wdog.h"
#include "em_rmu.h"

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_MODULE_ID_WATCHDOG

static bool wdgInitFlag = false, wdgEnableFlag = false;

#define WATCHDOG_PERIOD_SIZE 16

const WDOG_PeriodSel_TypeDef wdgTimeoutPeriod[WATCHDOG_PERIOD_SIZE] =
        {
                wdogPeriod_9,
                wdogPeriod_17,
                wdogPeriod_33,
                wdogPeriod_65,
                wdogPeriod_129,
                wdogPeriod_257,
                wdogPeriod_513,
                wdogPeriod_1k,
                wdogPeriod_2k,
                wdogPeriod_4k,
                wdogPeriod_8k,
                wdogPeriod_16k,
                wdogPeriod_32k,
                wdogPeriod_64k,
                wdogPeriod_128k,
                wdogPeriod_256k
        };

const uint32_t clockPeriod[WATCHDOG_PERIOD_SIZE] = {9,17,33,65,129,257,513,1000,2000,4000,8000,16000,32000,64000,128000,256000};


/**
 * @brief initializes the hardware watchdog
 * @details After that it can be enabled using this function call.
 * @return  RETCODE_OK upon successful initializing of the watchdog error code otherwise.
 */
Retcode_T MCU_Watchdog_Init(WdgHandle_T wdgHandle)
{
    Retcode_T retcode = RETCODE_OK;
    WDOG_Init_TypeDef wdgConfig;
    uint32_t wdgTimeout = 0;

    MCU_Watchdog_Init_T *wdgInitStruct = (MCU_Watchdog_Init_T *)wdgHandle;

    if (NULL == wdgInitStruct)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }
    else if ((NULL != wdgInitStruct->WdgCallback) || (MCU_WATCHDOG_RESET_OFF == wdgInitStruct->ResetMode))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
    }
    else
    {
        /* For efm32 mcu, only watchdog timeout is configured for Internal 1 kHz RC oscillator */
        wdgTimeout = wdgInitStruct->Timeout;
        if ((UINT32_C(0) >= wdgTimeout) || (UINT32_C(256000) < wdgTimeout))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
    }

    if (RETCODE_OK == retcode)
    {
        /* Map the watchdog timeout to clock period range supported by emlib */
        for (uint8_t i = 0; i < WATCHDOG_PERIOD_SIZE-1; i++)
        {
            if ((wdgTimeout >= clockPeriod[i]) && (wdgTimeout <= clockPeriod[i+1]))
            {
                uint32_t clockPerMidPoint = (clockPeriod[i] + clockPeriod[i+1])/2;
                if (wdgTimeout <= clockPerMidPoint)
                {
                    wdgConfig.perSel = wdgTimeoutPeriod[i];
                    wdgInitStruct->Timeout = clockPeriod[i];
                    break;
                }
                else
                {
                    wdgConfig.perSel = wdgTimeoutPeriod[i+1];
                    wdgInitStruct->Timeout = clockPeriod[i+1];
                    break;
                }
            }
            else
            {
                wdgConfig.perSel = wdgTimeoutPeriod[0];
                wdgInitStruct->Timeout = clockPeriod[0];
            }
        }
        wdgConfig.enable = true;
        wdgConfig.debugRun = wdgInitStruct->RunOnCpuHalt;
        wdgConfig.em2Run = false;
        wdgConfig.em3Run = false;
        wdgConfig.em4Block = false;
        wdgConfig.swoscBlock = false;
        wdgConfig.lock = false;
        wdgConfig.clkSel = wdogClkSelULFRCO;

        WDOG_Init(&wdgConfig);

        wdgInitFlag = true;
        wdgEnableFlag = false;
    }

    return retcode;
}

/**
 * @brief Enables the hardware watchdog
 * @details After that it can be enabled using this function call.
 * @retval  RETCODE_OK upon successful enabling of the watchdog.
 * @retval  RETCODE_UNINITIALIZED in case of failure
 * @see MCU_Watchdog_Feed()
 */
Retcode_T MCU_Watchdog_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (true == wdgInitFlag)
    {
        WDOG_Enable(true);
        wdgEnableFlag = true;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }
    return retcode;
}

/**
 * @brief Feeds an enabled watchdog.
 * @details This function is used to feed the watchdog within a certain
 * interval to prevent the watchdog from firing.
 * @retval  RETCODE_OK upon successful feeding
 * @retval  RETCODE_UNINITIALIZED in case of failure
 */
Retcode_T MCU_Watchdog_Feed(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (true == wdgEnableFlag)
    {
        WDOG_Feed();
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }
    return retcode;
}

/**
 * @brief Checks if the last system reset was caused by the watchdog.
 * @details Use the function to check whether the last system reset
 * was caused by the watchdog. If reset was due to watchdog, reset cause is cleared.
 * @retval true if the last system reset was caused by the watchdog.
 * @retval false if the last system reset was not caused by the watchdog.
 */
bool MCU_Watchdog_IsResetCausedByWatchdog(void)
{
    bool wdgResetFlag = false;
    uint32_t rflags = RMU_ResetCauseGet();

    if (rflags & RMU_RSTCAUSE_WDOGRST)
    {
        wdgResetFlag = true;
        RMU_ResetCauseClear();
    }
    return wdgResetFlag;
}

#endif //-- BCDS_FEATURE_WATCHDOG

