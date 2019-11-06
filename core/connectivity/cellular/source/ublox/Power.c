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

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_POWER

#include "Power.h"
#include "Kiso_Cellular.h"
#include "Kiso_CellularConfig.h"
#include "Engine.h"
#include "AtResponseQueue.h"
#include "Hardware.h"
#include "At3Gpp27007.h"
#include "AT_UBlox.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

#include "Kiso_Logging.h"

#include "FreeRTOS.h"
#include "task.h"

#define CELLULAR_POWER_SHORT_ENQUEUE_TIMEOUT (UINT32_C(1000))
#define CELLULAR_POWER_STARTUP_DELAY (pdMS_TO_TICKS(UINT32_C(5000)))
#define CELLULAR_POWER_STARTUP_MAX_RETRY_COUNT (UINT32_C(3))
#define CELLULAR_POWER_STARTUP_RETRY_PERIOD (UINT32_C(3000))
#define CELLULAR_POWER_SOFT_RESET_PERIOD (UINT32_C(3000))

#define CELLULAR_POWER_RESET_MAX_RETRY_COUNT (UINT32_C(4))
#define CELLULAR_POWER_RESET_RETRY_PERIOD (UINT32_C(1000))

#define CELLULAR_POWER_SIM_UNLOCK_DELAY (pdMS_TO_TICKS(UINT32_C(1000)))

#define CELLULAR_POWER_USE_HEX_MODE (true)

#define UBLOX_CFUN_FUN_SILENTRESET ((AT_CFUN_Fun_T)15)

/**
 * @brief Check if the modem responds and is in AT mode.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T ProbeForResponsiveness(void);

/**
 * @brief Startup the Cellular hardware and bring into operating mode.
 *
 * To be encapsulated by via #Engine_Dispatch().
 *
 * @param parameter Ignored.
 * @param parameterLength Ignored.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T StartupCellular(void *parameter, uint32_t parameterLength);
/**
 * @brief Gracefully power down the Cellular hardware.
 *
 * To be encapsulated via #Engine_Dispatch().
 *
 * @param parameter Ignored.
 * @param parameterLength Ignored.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T ShutdownCellular(void *parameter, uint32_t parameterLength);

/**
 * @brief Reset the Cellular hardware and bring back into operating mode.
 *
 * To be encapsulated via #Engine_Dispatch().
 *
 * @param parameter Ignored.
 * @param parameterLength Ignored.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T ResetCellular(void *parameter, uint32_t parameterLength);

static bool IsPoweredOn = false;
static char Pin[UBLOX_MAX_PIN_LENGTH + 1];

Retcode_T Power_SetupModem(void)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = At_Set_UTEST(0);

    if ((0 < strlen(Pin)) && (retcode == RETCODE_OK))
    {
        LOG_DEBUG("Unlocking SIM with PIN '%s' after startup.", Pin);
        retcode = At_Set_CPIN(Pin);
        if (RETCODE_OK != retcode)
        {
            LOG_ERROR("Unable to unlock SIM with PIN '%s'.", Pin);
        }

        /** \todo: Of course a better way would be to check if the SIM has been
         * unlocked instead of just relying on an arbitrary delay. Perhaps use
         * AT+CPIN? to wait for SIM-READY code. */
        /* Give the modem some time to unlock the SIM card. CPIN appears to be
         * non-blocking. */
        vTaskDelay(CELLULAR_POWER_SIM_UNLOCK_DELAY);
    }

    if (RETCODE_OK == retcode)
    {
        /* Keep the modem from automatically registering to the network.
         * This is done, so we can control the registration procedure
         * ourselves. The fact that we have to manually do this for SARA-R4/N4
         * is quite annoying. The modem will always boot-up with +COPS=0 and
         * does not allow to change this behavior.
         * The reason we would want to prevent the modem from auto-registering
         * is that we don't know what settings are currently present on the NVM
         * Perhaps the register could happen on the wrong APN because that's
         * what was stored on modem NVM. */
        AT_COPS_Param_T cops;
        cops.Mode = AT_COPS_MODE_DEREGISTER;
        cops.Format = AT_COPS_FORMAT_INVALID;
        cops.Oper.Numeric = 0;
        cops.AcT = AT_COPS_ACT_INVALID;
        LOG_DEBUG("Deregistering from network after startup.");
        retcode = At_Set_COPS(&cops); //LCOV_EXCL_BR_LINE
        if (RETCODE_OK != retcode)
        {
            LOG_ERROR("Failed to deregister from network.");
        }
    }

    if (RETCODE_OK == retcode)
    {
        /* Enable CEREG URC so we get notified in case the network status
         * changes. */
        LOG_DEBUG("Enabling CEREG URC after startup.");
        retcode = At_Set_CEREG(AT_CXREG_N_URC);
        if (RETCODE_OK != retcode)
        {
            LOG_ERROR("Failed to enable CEREG URC.");
        }
    }

    if (RETCODE_OK == retcode)
    {
        /* Enable CGREG URC so we get notified in case the network status
         * changes. */
        LOG_DEBUG("Enabling CGREG URC after startup.");
        retcode = At_Set_CGREG(AT_CXREG_N_URC);
        if (RETCODE_OK != retcode)
        {
            LOG_ERROR("Failed to enable CGREG URC.");
        }
    }

    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("Setting CMEE after startup.");
        retcode = At_Set_CMEE(0);
    }

    if (RETCODE_OK == retcode)
    {
        /* Enable HEX-mode for socket-service */
        LOG_DEBUG("Enabling HEX-mode after startup.");
        bool useHexMode = CELLULAR_POWER_USE_HEX_MODE;
        AT_UDCONF_Param_T udconf;
        udconf.Config = AT_UDCONF_CONFIG_HEXMODE;
        udconf.Value = &useHexMode;
        retcode = At_Set_UDCONF(&udconf); //LCOV_EXCL_BR_LINE
        if (RETCODE_OK != retcode)
        {
            LOG_ERROR("Failed to enable HEX-mode.");
        }
    }

    return retcode;
}

Retcode_T Power_SoftReset(void)
{
    Retcode_T retcode = RETCODE_OK;

    AT_CFUN_Param_T cfun;
    cfun.Fun = UBLOX_CFUN_FUN_SILENTRESET;
    cfun.Rst = AT_CFUN_RST_INVALID;
    retcode = At_Set_CFUN(&cfun); //LCOV_EXCL_BR_LINE

    Engine_SetFlukeCharFilterEnabled(true); //LCOV_EXCL_BR_LINE

    /* The modem takes some time to actually power down */
    vTaskDelay(pdMS_TO_TICKS(CELLULAR_POWER_SOFT_RESET_PERIOD)); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        uint32_t attempt = 0;
        do
        {
            retcode = ProbeForResponsiveness();                             //LCOV_EXCL_BR_LINE
            vTaskDelay(pdMS_TO_TICKS(CELLULAR_POWER_STARTUP_RETRY_PERIOD)); //LCOV_EXCL_BR_LINE
        } while (RETCODE_OK != retcode && attempt++ < CELLULAR_POWER_STARTUP_MAX_RETRY_COUNT);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Power_SetupModem(); //LCOV_EXCL_BR_LINE
    }

    /* It appears that during soft-reset two '\0's are received on the UART. We
     * want to skip these. */
    Engine_SetFlukeCharFilterEnabled(false); //LCOV_EXCL_BR_LINE

    return retcode;
}

static Retcode_T ProbeForResponsiveness(void)
{
    return At_Test_AT();
}

static Retcode_T StartupCellular(void *parameter, uint32_t parameterLength)
{
    KISO_UNUSED(parameterLength);
    assert(NULL != parameter);
    assert(sizeof(Cellular_PowerUpParameters_T) == parameterLength);
    const Cellular_PowerUpParameters_T *powerUpParam = (const Cellular_PowerUpParameters_T *)parameter;

    LOG_DEBUG("Powering up Cellular.");

    AtResponseQueue_Reset();

    /* During startup, we ignore any fluke characters (as they may occur and
     * destroy our parser) */
    Engine_SetFlukeCharFilterEnabled(true);

    Retcode_T retcode = Hardware_Startup();
    if (RETCODE_OK == retcode)
    {
        /* without this wait time each 30rd get ICCID failed */
        vTaskDelay(CELLULAR_POWER_STARTUP_DELAY);

        /* Cellular has now received its POWER_ON command and should start
         * booting. We will probe the modem several times until we receive a
         * valid answer ... or timeout. */
        LOG_DEBUG("Hardware-POWER_ON command sent!");

        Retcode_T cellularIsResponsive = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        for (uint32_t i = 0; RETCODE_OK != cellularIsResponsive && i < CELLULAR_POWER_STARTUP_MAX_RETRY_COUNT; i++)
        {
            cellularIsResponsive = ProbeForResponsiveness();

            if (RETCODE_OK != cellularIsResponsive)
                vTaskDelay(CELLULAR_POWER_STARTUP_RETRY_PERIOD / portTICK_PERIOD_MS);
        }

        if (RETCODE_OK == cellularIsResponsive)
        {
            assert(sizeof(Pin) > 0);
            if (NULL != powerUpParam->SimPin)
            {
                strncpy(Pin, powerUpParam->SimPin, sizeof(Pin) - 1);
                Pin[sizeof(Pin) - 1] = '\0';
            }
            else
            {
                Pin[0] = '\0';
            }
            retcode = Power_SetupModem();

            if (RETCODE_OK == retcode)
            {
                /* Everything fine, modem is responsive! Exit gracefully */
                LOG_INFO("Cellular hardware powered up!");
                IsPoweredOn = true;
                Engine_NotifyNewState(CELLULAR_STATE_POWERON, NULL, 0);
            }
            else
            {
                LOG_ERROR("Cellular power-on-setup failed")
            }
        }
        else
        {
            /* OK, modem is still not responding ... try resetting */
            LOG_WARNING("Cellular not responding, trying reset...");
            IsPoweredOn = false;
            (void)ResetCellular(NULL, 0);
        }
    }
    else
    {
        /* Something's wrong with the BSP?! */
        LOG_ERROR("Unexpected error from BSP!");
        IsPoweredOn = false;
        Engine_NotifyNewState(CELLULAR_STATE_POWEROFF, NULL, 0);
    }

    Engine_SetFlukeCharFilterEnabled(false);

    return retcode;
}

static Retcode_T CheckCellular(void *parameter, uint32_t parameterLength)
{
    KISO_UNUSED(parameterLength);
    assert(sizeof(bool *) == parameterLength);
    assert(NULL != parameter);
    bool *isPoweredOn = (bool *)parameter;

    Retcode_T retcode = RETCODE_OK;

    if (IsPoweredOn)
    {
        retcode = ProbeForResponsiveness();
        *isPoweredOn = (RETCODE_OK == retcode);
    }
    else
    {
        *isPoweredOn = true;
    }

    return retcode;
}

static Retcode_T ShutdownCellular(void *parameter, uint32_t parameterLength)
{
    KISO_UNUSED(parameter);
    KISO_UNUSED(parameterLength);

    LOG_DEBUG("Powering down Cellular.");

    Retcode_T retcode = Hardware_Shutdown();

    if (RETCODE_OK == retcode)
    {
        IsPoweredOn = false;
        Engine_NotifyNewState(CELLULAR_STATE_POWEROFF, NULL, 0);
    }
    else
    {
        LOG_ERROR("Hardware-POWER_OFF failed!");
    }

    return retcode;
}

static Retcode_T ResetCellular(void *parameter, uint32_t parameterLength)
{
    KISO_UNUSED(parameterLength);
    assert(NULL != parameter);
    assert(sizeof(Cellular_PowerUpParameters_T) == parameterLength);
    const Cellular_PowerUpParameters_T *powerUpParam = (const Cellular_PowerUpParameters_T *)parameter;

    LOG_DEBUG("Resetting Cellular.");

    /* During reset, we ignore any fluke characters (as they may occur and destroy our parser) */
    Engine_SetFlukeCharFilterEnabled(true);

    Retcode_T retcode = Hardware_Reset();

    if (RETCODE_OK == retcode)
    {
        /* Cellular has now received its RESET command and should start booting.
         * We will probe the modem several times until we receive a valid answer ... or timeout. */
        LOG_INFO("Hardware-RESET command sent!");
        vTaskDelay(CELLULAR_POWER_STARTUP_DELAY / portTICK_PERIOD_MS);

        Retcode_T cellularIsResponsive = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_NOT_RESPONSIVE);
        for (uint32_t i = 0; RETCODE_OK != cellularIsResponsive && i < CELLULAR_POWER_RESET_MAX_RETRY_COUNT; i++)
        {
            cellularIsResponsive = ProbeForResponsiveness();

            if (RETCODE_OK != cellularIsResponsive)
                vTaskDelay(CELLULAR_POWER_RESET_RETRY_PERIOD / portTICK_PERIOD_MS);
        }

        if (RETCODE_OK == cellularIsResponsive)
        {
            /* Everything fine, modem is responsive! Exit gracefully */
            LOG_INFO("Cellular hardware powered up!");
            IsPoweredOn = true;
            Engine_NotifyNewState(CELLULAR_STATE_POWERON, NULL, 0);

            assert(sizeof(Pin) > 0);
            if (NULL != powerUpParam->SimPin)
            {
                strncpy(Pin, powerUpParam->SimPin, sizeof(Pin) - 1);
                Pin[sizeof(Pin) - 1] = '\0';
            }
            else
            {
                Pin[0] = '\0';
            }
            retcode = Power_SetupModem();
        }
        else
        {
            /* OK, modem is still not responding ... we give up! */
            LOG_ERROR("Cellular not responding after reset!");
            retcode = cellularIsResponsive;
            IsPoweredOn = false;
            Engine_NotifyNewState(CELLULAR_STATE_POWEROFF, NULL, 0);
        }
    }
    else
    {
        /* Something's wrong with the BSP?! */
        LOG_ERROR("Unexpected error from BSP!");
        IsPoweredOn = false;
        Engine_NotifyNewState(CELLULAR_STATE_POWEROFF, NULL, 0);
    }

    Engine_SetFlukeCharFilterEnabled(false);

    return retcode;
}

Retcode_T Cellular_PowerOn(const Cellular_PowerUpParameters_T *parameters)
{
    if (NULL == parameters)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        Cellular_PowerUpParameters_T tempParams = *parameters;
        return Engine_Dispatch(StartupCellular, CELLULAR_POWER_SHORT_ENQUEUE_TIMEOUT, (void *)&tempParams, sizeof(Cellular_PowerUpParameters_T)); //LCOV_EXL_BR_LINE
    }
}

Retcode_T Cellular_PowerOff(void)
{
    return Engine_Dispatch(ShutdownCellular, CELLULAR_POWER_SHORT_ENQUEUE_TIMEOUT, NULL, 0); //LCOV_EXL_BR_LINE
}

Retcode_T Cellular_Reset(const Cellular_PowerUpParameters_T *parameters)
{
    if (NULL == parameters)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        Cellular_PowerUpParameters_T tempParams = *parameters;
        return Engine_Dispatch(ResetCellular, CELLULAR_POWER_SHORT_ENQUEUE_TIMEOUT, (void *)&tempParams, sizeof(Cellular_PowerUpParameters_T)); //LCOV_EXL_BR_LINE
    }
}

Retcode_T Cellular_IsPoweredOn(bool *isPoweredOn)
{
    if (NULL == isPoweredOn)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        return Engine_Dispatch(CheckCellular, CELLULAR_POWER_SHORT_ENQUEUE_TIMEOUT, isPoweredOn, sizeof(isPoweredOn)); //LCOV_EXL_BR_LINE
    }
}
