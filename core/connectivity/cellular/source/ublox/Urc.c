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
 *
 * @brief Implements various URC handling routines.
 */

#include "BCDS_CellularModules.h"
#define BCDS_MODULE_ID BCDS_CELLULAR_MODULE_ID_URC

#include "AtUrc.h"

#include "BCDS_Cellular.h"
#include "AtResponseQueue.h"
#include "At3Gpp27007.h"
#include "AT_UBlox.h"

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"

#include "BCDS_Logging.h"

/**
 * @brief The maximum number of subsequent URC handler chain executions. If this number
 *        is too low, some URCs might not be handled. If it's too high we waste time
 *        doing nothing. When in dought, err on the side of making this number too high.
 */
#define CELLULAR_MAX_URC_HANDLER_RUNS      UINT8_C(100)

/**
 * @brief Macro used exclusively in CellularEngine_HandleUrcResponses to register URC handlers.
 *
 * @param x The URC handler to register
 */
#define CELLULAR_REGISTER_URC_HANDLER(x)   (result = CallHandler((x), result, &OneHandlerCared))

/**
 * @brief The maximum time to wait for the URC argument to be received.
 */
#define CELLULAR_URC_ARG_WAIT_TIME         (UINT32_C(100) / portTICK_PERIOD_MS)

/**
 * @brief Calls a single URC handler and updates the
 * result-code/OneHandlerWasInterested flags accordingly.
 *
 * @warning This function is meant to be called from
 * Engine_HandleUrcResponses() ONLY.
 *
 * @param[in] handler
 * The handler to call
 *
 * @param[in] totalResult
 * The result codes returned from previous calls of this function
 *
 * @param[out] handlerWasInterested
 * True if this handler was interested in the queue content
 *
 * @return A #Retcode_T that is equal to the parameter totalResult, in case the
 * handler was not interested and did not encounter and error. If the handler
 * did encounter an error, that #Retcode_T will be returned.
 */
static Retcode_T CallHandler(CellularUrcHandler_T handler, Retcode_T totalResult, bool* handlerWasInterested);

/**
 * @brief Handle u-blox variant specific URCs that do not need proper
 * interpretation. URCs will mostly just be thrown out of the queue.
 *
 * @return A #Retcode_T following the URC-handler chaining scheme described in
 * #CallHandler.
 */
static Retcode_T HandleMiscellaneousUrc(void);

static Retcode_T CallHandler(CellularUrcHandler_T handler, Retcode_T totalResult, bool* handlerWasInterested)
{
    if (NULL == handler || NULL == handlerWasInterested)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T handlerResult = handler();

    Retcode_T result = totalResult;
    if (RETCODE_OK == handlerResult)
    {
        *handlerWasInterested = true;
    }
    else if (Retcode_GetCode(handlerResult) == (uint32_t) RETCODE_CELLULAR_URC_NOT_PRESENT)
    {
        /* Handler is still ok, but simply wasn't interested. Nothing to do. */
    }
    else
    {
        result = handlerResult;
    }

    return result;
}

static Retcode_T HandleMiscellaneousUrc(void)
{
    Retcode_T result = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
    Retcode_T retcode;

    retcode = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) "PACSP0", strlen("PACSP0"));
    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("URC for PACSP0");
        result = RETCODE_OK;
    }

    retcode = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) "PACSP1", strlen("PACSP1"));
    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("URC for PACSP1");
        result = RETCODE_OK;
    }

    retcode = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) "UMWI", strlen("UMWI"));
    if (RETCODE_OK == retcode)
    {
        LOG_DEBUG("URC for UMWI");
        result = RETCODE_OK;

        // ignore arg 0
        (void) AtResponseQueue_IgnoreEvent(0);

        // ignore arg 1
        (void) AtResponseQueue_IgnoreEvent(0);
    }

    return result;
}

Retcode_T Urc_HandleResponses(void)
{
    Retcode_T result = RETCODE_OK;
    bool OneHandlerCared = false;
    bool OneHandlerWasEverInterested = false;

    for (uint8_t i = 0; 0 == i || (OneHandlerCared && i < CELLULAR_MAX_URC_HANDLER_RUNS); i++, OneHandlerCared = false)
    {
        /*
         * START OF HANDLER LIST
         */
        CELLULAR_REGISTER_URC_HANDLER(HandleMiscellaneousUrc);
        CELLULAR_REGISTER_URC_HANDLER(At_HandleUrc_CGREG);
        CELLULAR_REGISTER_URC_HANDLER(At_HandleUrc_CEREG);
        CELLULAR_REGISTER_URC_HANDLER(At_HandleUrc_UUSOCL);
        CELLULAR_REGISTER_URC_HANDLER(At_HandleUrc_UUSOLI);
        CELLULAR_REGISTER_URC_HANDLER(At_HandleUrc_UUSORD);
        CELLULAR_REGISTER_URC_HANDLER(At_HandleUrc_UUSORF);
        CELLULAR_REGISTER_URC_HANDLER(At_HandleUrc_UUHTTPCR);
        /* END OF HANDLER LIST */

        if (OneHandlerCared)
        {
            OneHandlerWasEverInterested = true;
        }
    }

    if (!OneHandlerWasEverInterested)
    {
        result = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }

    return result;
}
