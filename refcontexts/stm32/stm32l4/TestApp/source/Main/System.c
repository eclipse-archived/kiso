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
#define BCDS_MODULE_ID BCDS_TEST_APP_MODULE_ID_SYSTEM

/* own header files */
#include "SystemConfig.h"
#include "System.h"

/* system header files */

/* additional interface header files */
#include "BCDS_BSP_Charger_BQ2407X.h"
#include "stm32l4xx_hal.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_TestSection.h"
#include "BSP_BoardType.h"
#include "Led.h"
#include <FreeRTOS.h>
#include <semphr.h>

/* constant definitions ***************************************************** */

/* function prototypes ****************************************************** */
/**
 * @brief This function will enable/disable the internal Battery
 * to source the device.
 *
 * @param [in]  controlValue
 * Boolean defining enable / disable.
 *
 * @return  RETCODE_OK on success or an error code otherwise.
 */
static Retcode_T SysOffControl(bool controlValue);

/**
 * @brief This function will enable the below signal lines.
 * - SysOff Control
 * - Peripheral Supply
 *
 * @return  RETCODE_OK on success or an error code otherwise.
 */
static Retcode_T SystemEnable(void);

/**
 * @brief This function will configure and enable the below signal lines.
 * - SysOff Control
 * - Peripheral Supply
 *
 * @return  RETCODE_OK on success or an error code otherwise.
 */
static Retcode_T SystemSetup(void);

/**
 * @brief This function will initialize the following System Components.
 * - User Interface
 * - CAN
 * - BLE
 * - Serval/GSM
 *
 * @note System_Initialize must have been called prior to this.
 *
 * @param [in] data
 * Pointer to any data which if needed shall be passed while enqueuing
 * this function. Currently this is not used.
 *
 * @param [in] value
 * Data value which if needed shall be passed while enqueuing this
 * function. Currently this is not used.
 */
static void SystemInitialize(void *data, uint32_t value);

/**
 * @brief This function will enable the below signal lines.
 * - Battery Charging
 *
 */
static Retcode_T BatteryChargingConfigure(void);

/**
 * @brief This function will configure and enable the below signal lines.
 * - Battery Charging
 *
 * @return  RETCODE_OK on success or an error code otherwise.
 */
static Retcode_T BatteryChargingSetup(void);
/* local variables ********************************************************** */

static CmdProcessor_T * systemCmdHandle;


/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/* The description is in the function prototype definition */
static Retcode_T SysOffControl(bool controlValue)
{
    if (true == controlValue)
    {
        BSP_Charger_BQ2407X_SetSysoffHigh();
    }
    else
    {
        BSP_Charger_BQ2407X_SetSysoffLow();
    }

    return (RETCODE_OK);
}

/* The description is in the function prototype definition */
static Retcode_T SystemEnable(void)
{
    Retcode_T Rc;

    Rc = SysOffControl(true);

    return (Rc);
}

/* The description is in the function prototype definition */
static Retcode_T SystemSetup(void)
{
    Retcode_T Rc;

    Rc = SystemEnable();

    return (Rc);
}

static void SystemInitialize(void *data, uint32_t value)
{
    BCDS_UNUSED(data);
    BCDS_UNUSED(value);

    Retcode_T Rc;
    Rc = Led_Indication(LED_BLUE);

    if (RETCODE_OK == Rc)
    {
        Rc = TestSection_Initialize();
    }

    if (RETCODE_OK != Rc)
    {
        Retcode_RaiseError(Rc);
    }
}

/* The description is in the function prototype definition */
static Retcode_T BatteryChargingConfigure(void)
{
    return BSP_Charger_BQ2407X_Connect();
}

/* The description is in the function prototype definition */
static Retcode_T BatteryChargingSetup(void)
{
    Retcode_T Rc;

    Rc = BatteryChargingConfigure();
    if ( RETCODE_OK == Rc)
    {
    	Rc = BSP_Charger_BQ2407X_Enable((uint32_t)BSP_CHARGING_LEVEL_3);
    }

    return (Rc);
}
/* global functions ********************************************************* */

Retcode_T System_Initialize(CmdProcessor_T * cmdHandle)
{
    Retcode_T retcode;

    if (NULL != cmdHandle)
    {
        systemCmdHandle = cmdHandle;

        retcode = SystemSetup();

        if (RETCODE_OK == retcode)
        {
            retcode = Led_Initialize();
        }
        if(RETCODE_OK == retcode)
        {
        	retcode = BatteryChargingSetup();
        }

        if (RETCODE_OK == retcode)
        {
            retcode = CmdProcessor_Enqueue(systemCmdHandle, SystemInitialize, NULL, 0);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t )RETCODE_INVALID_PARAM);
    }

    return (retcode);
}
/** ************************************************************************* */
