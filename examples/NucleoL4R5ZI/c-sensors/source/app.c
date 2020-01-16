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
 * @ingroup
 *
 * @defgroup
 * @{
 *
 * @brief Sensors Example
 *
 * @details Demo application demonstrates how to use integrate the sensors with KISO NUCLEOl4R5ZI BSP .
 *
 **/

#include "AppInfo.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID APP_MODULE_ID_APP

/* own header files */
#include "app.h"

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "BSP_NucleoL4R5ZI.h"
#include "Kiso_CmdProcessor.h"
#include "accel_sensor.h"
#include "env_sensor.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

static void Sensors_init(void *param1, uint32_t param2);

/*---------------------- VARIABLES DECLARATIONS ---------------------------------------------------------------------*/

/*Application Command Processor Instance */
CmdProcessor_T *AppCmdProcessor;

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
static void Sensors_init(void *param1, uint32_t param2)
{
    KISO_UNUSED(param1);
    KISO_UNUSED(param2);

    Retcode_T returnVal = RETCODE_OK;

    returnVal = AccelSensor_init();

    if (RETCODE_OK == returnVal)
    {
        returnVal = EnvSensor_init();
    }
    if (RETCODE_OK != returnVal)
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE));
    }
}

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void appInitSystem(void *CmdProcessorHandle, uint32_t param2)
{
    KISO_UNUSED(param2);

    Retcode_T returnVal = RETCODE_OK;

    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    AppCmdProcessor = (CmdProcessor_T *)CmdProcessorHandle;

    if (RETCODE_OK == returnVal)
    {
        CmdProcessor_Enqueue((CmdProcessor_T *)AppCmdProcessor, Sensors_init, (void *)AppCmdProcessor, 0);
    }
}
