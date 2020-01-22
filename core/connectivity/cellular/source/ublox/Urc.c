/**********************************************************************************************************************
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
 **********************************************************************************************************************/

/**
 * @file
 *
 * @brief Implements various URC handling routines.
 */

/*###################### INCLUDED HEADERS ############################################################################*/

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_URC

#include "AtUrc.h"

#include "Kiso_Cellular.h"
#include "AtResponseQueue.h"
#include "At3Gpp27007.h"
#include "AT_UBlox.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

#include "Kiso_Logging.h"
/*###################### MACROS DEFINITION ###########################################################################*/
/**
 * @brief       The maximum number of subsequent URC handler chain executions. If this number
 *              is too low, some URCs might not be handled. If it's too high we waste time
 *              doing nothing. When in dought, err on the side of making this number too high.
 */
#define CELLULAR_MAX_URC_HANDLER_RUNS UINT8_C(1)

/**
 * @brief       The maximum time to wait for the URC argument to be received.
 */
#define CELLULAR_URC_ARG_WAIT_TIME (UINT32_C(100) / portTICK_PERIOD_MS) //todo implicit dependency to freertos here

/*###################### LOCAL FUNCTIONS DECLARATION #################################################################*/

static Retcode_T HandleMiscellaneousUrc(void);

/*###################### VARIABLES DECLARATION #######################################################################*/

/*###################### EXPOSED FUNCTIONS IMPLEMENTATION ############################################################*/

Retcode_T Urc_HandleResponses(void)
{
    Retcode_T retcode;
    bool OneHandlerCared = false;

    for (uint8_t i = 0; (i < CELLULAR_MAX_URC_HANDLER_RUNS); i++)
    {
        retcode = HandleMiscellaneousUrc();
        if (RETCODE_OK == retcode)
        {
            OneHandlerCared = true;
        }

        retcode = At_HandleUrc_CGREG();
        if (RETCODE_OK == retcode)
        {
            OneHandlerCared = true;
        }

        retcode = At_HandleUrc_CEREG();
        if (RETCODE_OK == retcode)
        {
            OneHandlerCared = true;
        }

        retcode = At_HandleUrc_UUSOCL();
        if (RETCODE_OK == retcode)
        {
            OneHandlerCared = true;
        }

        retcode = At_HandleUrc_UUSOLI();
        if (RETCODE_OK == retcode)
        {
            OneHandlerCared = true;
        }

        retcode = At_HandleUrc_UUSORD();
        if (RETCODE_OK == retcode)
        {
            OneHandlerCared = true;
        }

        retcode = At_HandleUrc_UUSORF();
        if (RETCODE_OK == retcode)
        {
            OneHandlerCared = true;
        }

        retcode = At_HandleUrc_UUHTTPCR();
        if (RETCODE_OK == retcode)
        {
            OneHandlerCared = true;
        }

        if (OneHandlerCared)
        {
            retcode = RETCODE_OK;
            continue;
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_URC_NOT_PRESENT);
            break;
        }
    }
    return retcode;
}

/*###################### LOCAL FUNCTIONS IMPLEMENTATION ##############################################################*/

/**
 * @brief       Handle u-blox variant specific URCs that do not need proper interpretation. URCs will mostly just be 
 *              thrown out of the queue.
 *
 * @return      A #Retcode_T following the URC-handler chaining scheme described in #CallHandler.
 */
static Retcode_T HandleMiscellaneousUrc(void)
{
    Retcode_T result = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    Retcode_T retcode;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)"PACSP0", strlen("PACSP0"));
    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("URC for PACSP0"); //LCOV_EXCL_BR_LINE
        result = RETCODE_OK;
    }

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)"PACSP1", strlen("PACSP1"));
    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("URC for PACSP1"); //LCOV_EXCL_BR_LINE
        result = RETCODE_OK;
    }

    retcode = AtResponseQueue_WaitForNamedCmd(0, (const uint8_t *)"UMWI", strlen("UMWI"));
    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("URC for UMWI"); //LCOV_EXCL_BR_LINE
        result = RETCODE_OK;

        // ignore arg 0
        (void)AtResponseQueue_IgnoreEvent(0);

        // ignore arg 1
        (void)AtResponseQueue_IgnoreEvent(0);
    }

    return result;
}
