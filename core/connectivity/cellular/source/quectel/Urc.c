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
 *
 * @brief Implements various URC handling routines.
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_URC

#include "CellularModule.h"
#include "CellularBsp.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include "CellularResponseQueue.h"
#include "Command.h"
#include "CellularUtils.h"
#include "CellularEngine.h"
#include "Network.h"
#include "Socket.h"
#include "BCDS_Logging.h"
#include "CellularUrc.h"

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
#define CELLULAR_REGISTER_URC_HANDLER(x)   (result = CellularUrc_CallHandler((x), result, &OneHandlerCared))

/**
 * @brief The maximum time to wait for the URC argument to be received.
 */
#define CELLULAR_URC_ARG_WAIT_TIME         (UINT32_C(100) / portTICK_PERIOD_MS)

/**
 * @brief Supported miscellaneous response arguments.
 */
#define CPIN_ARG_READY      "READY"
#define QIND_ARG_SMS_DONE   "SMS DONE"
#define QIND_ARG_PB_DONE    "PB DONE"

/**
 * @brief Calls a single URC handler and updates the result code/OneHandlerWasInterested flags accordingly.
 *
 * @warning This function is meant to be called from CellularEngine_HandleUrcResponses() ONLY.
 *
 * @param[in] handler 				The handler to call
 * @param[in] TotalResult 			The result codes returned from previous calls of this function
 * @param[out] HandlerWasInterested True if this handler was interested in the queue content
 */
static Retcode_T CellularUrc_CallHandler(CellularUrcHandler_T handler, Retcode_T TotalResult, bool *HandlerWasInterested)
{
    // check arguments
    if (NULL == handler || NULL == HandlerWasInterested)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    // call the handler
    Retcode_T HandlerResult = handler();

    // do something with the handler result
    Retcode_T result = TotalResult;
    if (RETCODE_OK == HandlerResult)
    {
        *HandlerWasInterested = true;
    }
    else if (Retcode_GetCode(HandlerResult) == (uint32_t) RETCODE_CELLULAR_URC_NOT_PRESENT)
    {
        // handler is still ok, but simply wasn't interested. Nothing to do
    }
    else
    {
        result = HandlerResult;
    }

    return result;
}

Retcode_T CellularUrc_HandleResponses(void)
{
    Retcode_T result = RETCODE_OK;
    bool OneHandlerCared = false;
    bool OneHandlerWasEverInterested = false;

    for (uint8_t i = 0; 0 == i || (OneHandlerCared && i < CELLULAR_MAX_URC_HANDLER_RUNS); i++, OneHandlerCared = false)
    {
        /*
         * START OF HANDLER LIST
         */
        CELLULAR_REGISTER_URC_HANDLER(CellularUrc_Miscellaneous);
        CELLULAR_REGISTER_URC_HANDLER(CellularNetworkUrc_CREG);
        CELLULAR_REGISTER_URC_HANDLER(CellularSocketUrc_QIURC);
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

Retcode_T CellularUrc_Miscellaneous(void)
{
    bool IsUrc = false;

    Retcode_T retcode;
    uint8_t *arg;
    uint32_t len;
    int32_t value;

    /* check RDY */
    retcode = AtResponseQueue_WaitForMiscContent(0, &arg, &len);
    if (RETCODE_OK == retcode)
    {
        if (0 == memcmp(arg, ATCMD_MODULE_ON_RESP, strlen(ATCMD_MODULE_ON_RESP)))
        {
            AtResponseQueue_MarkBufferAsUnused();
            CellularInitStatus.MeInitSuccessful = 1;
            IsUrc = true;
            LOG_DEBUG("URC: RDY");
        }
    }

    /* check POWER DOWN */
    retcode = AtResponseQueue_WaitForMiscContent(0, &arg, &len);
    if (RETCODE_OK == retcode)
    {
        if (0 == memcmp(arg, ATCMD_MODULE_NORMAL_OFF_RESP, strlen(ATCMD_MODULE_NORMAL_OFF_RESP))
                || 0 == memcmp(arg, ATCMD_MODULE_OFF_RESP, strlen(ATCMD_MODULE_OFF_RESP)))
        {
            AtResponseQueue_MarkBufferAsUnused();
            CellularInitStatus.MePoweredDown = 1;
            IsUrc = true;
            LOG_DEBUG("URC: POWER DOWN");
        }
    }

    /* check +CFUN */
    retcode = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) ATCMD_CFUN_RESP, strlen(ATCMD_CFUN_RESP));
    if (RETCODE_OK == retcode)
    {
        IsUrc = true;
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_URC_ARG_WAIT_TIME, &arg, &len);
        if (RETCODE_OK == retcode)
        {
            if (CellularUtils_StrtolBounds(arg, len, &value, 0, 1) == RETCODE_OK)
            {
                CellularInitStatus.MeAllFuncAvailable = value;
                LOG_DEBUG("URC: +CFUN %d", (int) value);
            }
            AtResponseQueue_MarkBufferAsUnused();
        }
    }

    /* check +CPIN */
    retcode = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) ATCMD_CPIN_RESP, strlen(ATCMD_CPIN_RESP));
    if (RETCODE_OK == retcode)
    {
        IsUrc = true;
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_URC_ARG_WAIT_TIME, &arg, &len);
        if (RETCODE_OK == retcode)
        {
            if (0 == memcmp(arg, CPIN_ARG_READY, strlen(CPIN_ARG_READY)))
            {
                CellularInitStatus.SimCardPinReady = 1;
                LOG_DEBUG("URC: +CPIN READY");
            }
            else
            {
                CellularInitStatus.SimCardPinReady = 0;
            }
            AtResponseQueue_MarkBufferAsUnused();
        }
    }

    /* check +QUSIM */
    retcode = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) ATCMD_USIM_RESP, strlen(ATCMD_USIM_RESP));
    if (RETCODE_OK == retcode)
    {
        IsUrc = true;
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_URC_ARG_WAIT_TIME, &arg, &len);
        if (RETCODE_OK == retcode)
        {
            if (CellularUtils_StrtolBounds(arg, len, &value, 0, 1) == RETCODE_OK)
            {
                CellularInitStatus.UseUsimCard = value;
                LOG_DEBUG("URC: +QUSIM %d", (int) value);
            }
            AtResponseQueue_MarkBufferAsUnused();
        }
    }

    /* check +QIND */
    retcode = AtResponseQueue_WaitForNamedCmd(0, (uint8_t *) ATCMD_IND_RESP, strlen(ATCMD_IND_RESP));
    if (RETCODE_OK == retcode)
    {
        IsUrc = true;
        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_URC_ARG_WAIT_TIME, &arg, &len);
        if (RETCODE_OK == retcode)
        {
            if (0 == memcmp(arg, QIND_ARG_SMS_DONE, strlen(QIND_ARG_SMS_DONE)))
            {
                CellularInitStatus.SmsInitDone = 1;
                LOG_DEBUG("URC: +QIND: SMS DONE");
            }
            else if (0 == memcmp(arg, QIND_ARG_PB_DONE, strlen(QIND_ARG_PB_DONE)))
            {
                CellularInitStatus.PhonebookInitDone = 1;
                LOG_DEBUG("URC: +QIND: PB DONE");
            }
            AtResponseQueue_MarkBufferAsUnused();
        }
    }

    return IsUrc ? RETCODE_OK : RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CELLULAR_URC_NOT_PRESENT);
}
