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
#include "System.h"

/* system header files */

/* additional interface header files */
#include "BCDS_TestSection.h"
#include "BSP_BoardType.h"
#include "BCDS_BSP_Charger_BQ2407X.h"
#include "BCDS_BSP_Board.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_BSP_TestInterface.h"
#include <FreeRTOS.h>
#include <semphr.h>


#define CMD_PROCESS_TASK_PRI            UINT8_C(3)
#define CMD_PROCESS_TASK_STACK        	UINT8_C(512)
#define CMD_PROCESS_TASK_QUEUE_LEN      UINT8_C(5)

extern void xPortSysTickHandler(void);

static CmdProcessor_T Test_cmdHandle;

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

/* local variables ********************************************************** */

static CmdProcessor_T * systemCmdHandle;


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

static void initialize(void *param1, uint32_t param2)
{
    BCDS_UNUSED(  param1 );
    BCDS_UNUSED(  param2 );
    TestSection_Initialize();
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
        	 retcode = CmdProcessor_Initialize(&Test_cmdHandle, "Integration Test Application", CMD_PROCESS_TASK_PRI, CMD_PROCESS_TASK_STACK, CMD_PROCESS_TASK_QUEUE_LEN);

        	    if(RETCODE_OK != retcode)
        	    {
        	        Retcode_RaiseError(retcode);
        	    }

        	    retcode = CmdProcessor_Enqueue(&Test_cmdHandle, initialize, NULL,  (uint32_t) NULL);

        	    if(RETCODE_OK != retcode)
        	    {
        	        Retcode_RaiseError(retcode);
        	    }

                BSP_Board_OSTickInitialize(xPortSysTickHandler, NULL);
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, (uint32_t )RETCODE_INVALID_PARAM);
    }

    return (retcode);
}


/*************************************************************************** */
