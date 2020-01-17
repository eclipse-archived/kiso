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
 * @brief       Provides functionality for the modem interaction with the network
 */
/*###################### INCLUDED HEADERS ---------------------------------------------------------------------------*/
#include "Kiso_CellularModules.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_NETWORK
#include "Kiso_Cellular.h"

#include "Kiso_CellularConfig.h"
#include "Engine.h"
#include "At3Gpp27007.h"
#include "AT_UBlox.h"
#include "Power.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"
#include "Kiso_Logging.h"

#include <limits.h>
/*###################### MACROS DEFINITION ###########################################################################*/

#define CELLULAR_NETWORK_SHORT_ENQUEUE_TIMEOUT (UINT32_C(1000))
#define UBLOX_NETWORK_ONLY_BIT_SET(val, mask) (!((val) & ~(mask)) && ((val) & (mask)))

/*###################### TYPES DEFINITION  ###########################################################################*/

/**
 * @brief Internal structure for passing arguments through the Engine
 * dispatcher.
 */
struct CellularNetwork_CfgDataCtxParam_S
{
    uint32_t Cid;
    const Cellular_DataContextParameters_T *Parameters;
};

/**
 * @brief Internal structure for passing arguments through the Engine
 * dispatcher.
 */
struct CellularNetwork_ActDataCtxParam_S
{
    uint32_t Cid;
    const Cellular_DataContext_T **Ctx;
};

/**
 * @brief Internal structure for passing arguments through the Engine
 * dispatcher.
 */
struct CellularNetwork_QueryIccidParam_S
{
    char *Iccid;
    uint32_t *IccidLen;
};

/*###################### LOCAL FUNCTIONS DECLARATION #################################################################*/

static Retcode_T SelectLteRat(const AT_URAT_Param_T *urat);
static Retcode_T RegisterOnNetwork(void *param, uint32_t len);
static Retcode_T TranslateToUbloxAcT(const Cellular_NetworkParameters_T *netParam, AT_URAT_Param_T *uRatParam);
static Retcode_T DeregisterFromNetwork(void *param, uint32_t len);
static Retcode_T ConfigureDataContext(void *param, uint32_t len);
static Retcode_T ActivateDataContext(void *param, uint32_t len);
static Retcode_T DeactivateDataContext(void *param, uint32_t len);
static Retcode_T QueryIccid(void *param, uint32_t paramLen);

/*###################### VARIABLES DECLARATION #######################################################################*/

static Cellular_DataContext_T DataContexts[CELLULAR_DATACTX_COUNT];

/*###################### EXPOSED FUNCTIONS IMPLEMENTATION ############################################################*/

Retcode_T Cellular_ConfigureDataContext(uint32_t cid, const Cellular_DataContextParameters_T *parameters)
{
    if (CELLULAR_DATACTX_COUNT <= cid || NULL == parameters)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularNetwork_CfgDataCtxParam_S param;
        param.Cid = cid + 1;
        param.Parameters = parameters;
        return Engine_Dispatch(ConfigureDataContext, CELLULAR_NETWORK_SHORT_ENQUEUE_TIMEOUT, (void *)&param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T Cellular_RegisterOnNetwork(const Cellular_NetworkParameters_T *networkParameters)
{
    if (NULL == networkParameters)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        Cellular_NetworkParameters_T tempParams = *networkParameters;
        return Engine_Dispatch(RegisterOnNetwork, CELLULAR_NETWORK_SHORT_ENQUEUE_TIMEOUT, (void *)&tempParams, sizeof(Cellular_NetworkParameters_T)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T Cellular_ActivateDataContext(uint32_t cid, const Cellular_DataContext_T **ctx)
{
    if (CELLULAR_DATACTX_COUNT <= cid || NULL == ctx)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularNetwork_ActDataCtxParam_S param;
        param.Cid = cid + 1;
        param.Ctx = ctx;
        return Engine_Dispatch(ActivateDataContext, CELLULAR_NETWORK_SHORT_ENQUEUE_TIMEOUT, (void *)&param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T Cellular_DeactivateDataContext(uint32_t cid)
{
    if (CELLULAR_DATACTX_COUNT <= cid)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        assert(cid < UINT8_MAX);
        uint8_t ubloxCid = (uint8_t)cid + 1;
        return Engine_Dispatch(DeactivateDataContext, CELLULAR_NETWORK_SHORT_ENQUEUE_TIMEOUT, (void *)&ubloxCid, sizeof(ubloxCid)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T Cellular_QueryIccid(char *iccid, uint32_t *iccidLen)
{
    if (NULL == iccid || NULL == iccidLen || *iccidLen > AT_CCID_TYPE_20CHAR + 1)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularNetwork_QueryIccidParam_S param;
        param.Iccid = iccid;
        param.IccidLen = iccidLen;
        return Engine_Dispatch(QueryIccid, CELLULAR_NETWORK_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param)); //LCOV_EXCL_BR_LINE
    }
}

Retcode_T Cellular_QueryNetworkInfo(Cellular_NetworkInfo_T *networkInfo)
{
    KISO_UNUSED(networkInfo);
    Retcode_T retcode = RETCODE_OK;

    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);

    return retcode;
}

Retcode_T Cellular_DisconnectFromNetwork(void)
{
    return Engine_Dispatch(DeregisterFromNetwork, CELLULAR_NETWORK_SHORT_ENQUEUE_TIMEOUT, NULL, 0); //LCOV_EXCL_BR_LINE
}

Retcode_T Cellular_SetAirPlaneMode(bool airPlanModeOn)
{
    KISO_UNUSED(airPlanModeOn);
    /** \todo: Implement 3GPP 27.007 commands for AT+CFUN=4 (disable rx/tx) and
     * AT+CFUN=1 (enable rx/tx). */
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*###################### LOCAL FUNCTIONS IMPLEMENTATION ##############################################################*/

/**
 * @brief Perform LTE specific RAT change. This involves checking the
 * MNO-profile, band-mask and perhaps having to soft-reset the modem if one of
 * these has changed.
 *
 * @param[in] urat
 * URAT settings to apply.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T SelectLteRat(const AT_URAT_Param_T *urat)
{
    AT_UBANDMASK_Param_T ubandmask;

    Retcode_T retcode = At_Set_UMNOPROF(CELLULAR_MNO_PROFILE); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        retcode = At_Set_URAT(urat); //LCOV_EXCL_BR_LINE
    }
    if (RETCODE_OK == retcode)
    {
        ubandmask.Rat = AT_UBANDMASK_RAT_LTECATM1;
        ubandmask.Bitmask1 = CELLULAR_LTEM1_BAND_BITMASK1;
        ubandmask.Bitmask2 = CELLULAR_LTEM1_BAND_BITMASK2;
        retcode = At_Set_UBANDMASK(&ubandmask); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        ubandmask.Rat = AT_UBANDMASK_RAT_LTECATNB1;
        ubandmask.Bitmask1 = CELLULAR_LTENBIOT_BAND_BITMASK1;
        ubandmask.Bitmask2 = CELLULAR_LTENBIOT_BAND_BITMASK2;
        retcode = At_Set_UBANDMASK(&ubandmask); //LCOV_EXCL_BR_LINE
    }

    /* To use LTE Cat. M1 and LTE Cat. NB-IoT it is vital to
     * configure UMNOPROF and UBANDMASK on every boot-up. Changing
     * these also requires a soft-reset for the settings to apply.
     * We use AT+CFUN=15 for that. */
    if (RETCODE_OK == retcode)
    {
        retcode = Power_SoftReset(); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

/**
 * @brief Will translate the high level Cellular AcT and FallbackAcT parameters
 * to u-blox specific URAT parameters.
 *
 * @param[in] netParam
 * Pointer to a valid structure containing the high level AcT settings.
 *
 * @param[out] uRatParam
 * Will contain the associated URAT parameters for netParam.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T TranslateToUbloxAcT(const Cellular_NetworkParameters_T *netParam, AT_URAT_Param_T *uRatParam)
{
    assert(NULL != netParam);
    assert(NULL != uRatParam);

    Retcode_T retcode = RETCODE_OK;

    /** \todo: Maybe revisit the PreferredAcT and SecondPreferredAcT choices.
     * Currently best-guess values, but an application may want to chose the
     * metrics instead. */

    switch ((uint32_t)netParam->AcT)
    {
    case CELLULAR_RAT_DEFAULT:
        /* This indicates that the URAT command should be skipped, as the client
         * wants the default/SIM RAT configuration to be applied! */
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_INVALID;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        return RETCODE_OK;
    case CELLULAR_RAT_GSM:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_GPRS_EGPRS; /* SARA-R4/SARA-N4 specific */
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        break;
    case CELLULAR_RAT_GSM | CELLULAR_RAT_UMTS:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_GSM_UMTS;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_UTRAN;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        break;
    case CELLULAR_RAT_UMTS:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_UMTS;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        break;
    case CELLULAR_RAT_LTE:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_LTE;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        break;
    case CELLULAR_RAT_GSM | CELLULAR_RAT_UMTS | CELLULAR_RAT_LTE:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_GSM_UMTS_LTE;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_LTE;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_UTRAN;
        break;
    case CELLULAR_RAT_GSM | CELLULAR_RAT_LTE:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_GSM_LTE;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_LTE;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        break;
    case CELLULAR_RAT_UMTS | CELLULAR_RAT_LTE:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_UMTS_LTE;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_LTE;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        break;
    case CELLULAR_RAT_LTE_CAT_M1:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_LTEM1;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        break;
    case CELLULAR_RAT_LTE_CAT_NB1:
        uRatParam->SelectedAcT = AT_URAT_SELECTEDACT_LTENB1;
        uRatParam->PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
        uRatParam->SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;
        break;
    default:
        /* Seems like the combination of RATs did not fit any of the supported
         * <SelectedAcT> values. Our API can't provide this combination, so we
         * consider it a 'not supported' error. */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        break;
    }

    /* FallbackAcT not supported. The modem will decide the fallback within the
     * selected RAT-mask. */

    return retcode;
}

/**
 * @brief Register to the cellular network.
 *
 * To be encapsulated by via #Engine_Dispatch().
 *
 * @param[in] param
 * @param[in] len
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T RegisterOnNetwork(void *param, uint32_t len)
{
    KISO_UNUSED(len);
    assert(NULL != param);
    assert(sizeof(Cellular_NetworkParameters_T) == len);

    LOG_DEBUG("Enabling network registration.");

    Retcode_T retcode = RETCODE_OK;

    const Cellular_NetworkParameters_T *netParam = (const Cellular_NetworkParameters_T *)param;
    AT_URAT_Param_T urat;
    AT_COPS_Param_T cops;

    retcode = TranslateToUbloxAcT(netParam, &urat);

    /* Only force URAT if <SelectedAcT> is valid! */
    if (RETCODE_OK == retcode && AT_URAT_SELECTEDACT_INVALID != urat.SelectedAcT)
    {
        if (AT_URAT_SELECTEDACT_LTEM1 == urat.SelectedAcT || AT_URAT_SELECTEDACT_LTENB1 == urat.SelectedAcT)
        {
            retcode = SelectLteRat(&urat); //LCOV_EXCL_BR_LINE
        }
        else
        {
            retcode = At_Set_URAT(&urat); //LCOV_EXCL_BR_LINE
        }
    }

    /** \todo: Check current CREG, CGREG and CEREG values before initiating a
     * new join. We might already be connected, in which case we just need to
     * update the state-machine. */

    if (RETCODE_OK == retcode)
    {
        cops.Mode = AT_COPS_MODE_AUTOMATIC;
        cops.Format = AT_COPS_FORMAT_INVALID;
        cops.Oper.Numeric = 0;
        cops.AcT = AT_COPS_ACT_INVALID;

        retcode = At_Set_COPS(&cops); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        Engine_NotifyNewState(CELLULAR_STATE_REGISTERING, NULL, 0); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

/**
 * @brief Configure a given data-context.
 *
 * To be encapsulated by via #Engine_Dispatch().
 *
 * @param[in] param
 * A valid #CellularNetwork_CfgDataCtxParam_S pointer carrying the configuration
 * details.
 *
 * @param[in] len
 * Length of param.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T ConfigureDataContext(void *param, uint32_t len)
{
    KISO_UNUSED(len);
    assert(sizeof(struct CellularNetwork_CfgDataCtxParam_S) == len);
    assert(NULL != param);

    const struct CellularNetwork_CfgDataCtxParam_S *cfg = (const struct CellularNetwork_CfgDataCtxParam_S *)param;

    if (CELLULAR_DATACONTEXTTYPE_INTERNAL != cfg->Parameters->Type)
    {
        /* Currently not supported, sorry! ... maybe your first PR? :) */
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
    }
    else if (CELLULAR_APNAUTHMETHOD_NONE != cfg->Parameters->ApnSettings.AuthMethod)
    {
        /* Currently not supported, sorry! ... maybe your first PR? :) */
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
    }

    AT_CGDCONT_Param_T cgdcont;
    cgdcont.Cid = cfg->Cid;
    /** \todo: Make PDP_Type configurable. */
    cgdcont.PdpType = AT_CGDCONT_PDPTYPE_IP;
    cgdcont.Apn = cfg->Parameters->ApnSettings.ApnName;
    return At_Set_CGDCONT(&cgdcont); //LCOV_EXCL_BR_LINE
}

/**
 * @brief Activate a given data-context.
 *
 * To be encapsulated by via #Engine_Dispatch().
 *
 * @param[in,out] param
 * A valid #CellularNetwork_ActDataCtxParam_S pointer carrying the activation
 * details.
 *
 * @param[in] len
 * Length of param.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T ActivateDataContext(void *param, uint32_t len)
{
    KISO_UNUSED(len);
    assert(NULL != param);
    assert(sizeof(struct CellularNetwork_ActDataCtxParam_S) == len);

    Retcode_T retcode = RETCODE_OK;

    const struct CellularNetwork_ActDataCtxParam_S *act = (const struct CellularNetwork_ActDataCtxParam_S *)param;

    LOG_DEBUG("Activating data-context #%u.", (unsigned int)act->Cid);

    AT_CGACT_Param_T cgact;
    cgact.Cid = act->Cid;
    cgact.State = AT_CGACT_STATE_ACTIVATED;
    retcode = At_Set_CGACT(&cgact); //LCOV_EXCL_BR_LINE

    AT_CGPADDR_Resp_T cgpaddrResp;
    cgpaddrResp.Address1.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(cgpaddrResp.Address1.Address.IPv6, 0, sizeof(cgpaddrResp.Address1.Address.IPv6));
    cgpaddrResp.Address2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(cgpaddrResp.Address2.Address.IPv6, 0, sizeof(cgpaddrResp.Address2.Address.IPv6));
    if (RETCODE_OK == retcode)
    {
        AT_CGPADDR_Param_T cgpaddrParam;
        cgpaddrParam.Cid = act->Cid;

        retcode = At_Set_CGPADDR(&cgpaddrParam, &cgpaddrResp); //LCOV_EXCL_BR_LINE
    }

    if (RETCODE_OK == retcode)
    {
        Cellular_DataContext_T *ctx = &DataContexts[act->Cid];
        ctx->Id = act->Cid;
        ctx->Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
        switch (cgpaddrResp.Address1.Type)
        {
        case AT_CGPADDR_ADDRESSTYPE_IPV4:
            memcpy(ctx->IpAddress.Address.IPv4, cgpaddrResp.Address1.Address.IPv4, sizeof(cgpaddrResp.Address1.Address.IPv4));
            ctx->IpAddress.Type = CELLULAR_IPADDRESSTYPE_IPV4;
            break;
        case AT_CGPADDR_ADDRESSTYPE_IPV6:
            memcpy(ctx->IpAddress.Address.IPv6, cgpaddrResp.Address1.Address.IPv6, sizeof(cgpaddrResp.Address1.Address.IPv6));
            ctx->IpAddress.Type = CELLULAR_IPADDRESSTYPE_IPV6;
            break;
        case AT_CGPADDR_ADDRESSTYPE_INVALID:
            /** \todo: Consider throwing an error if we find the PDP-address
             * being invalid. */
            memset(ctx->IpAddress.Address.IPv6, 0, sizeof(ctx->IpAddress.Address.IPv4));
            ctx->IpAddress.Type = CELLULAR_IPADDRESSTYPE_MAX;
            break;
        }
        *(act->Ctx) = ctx;
        Engine_NotifyNewState(CELLULAR_STATE_DATAACTIVE, NULL, 0); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

/**
 * @brief Deactivate a data-context.
 *
 * To be encapsulated by via #Engine_Dispatch().
 *
 * @param[in] param
 * A valid #uint8_t pointer pointing to the id of the context to deactivate.
 *
 * @param[in] len
 * Length of param.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T DeactivateDataContext(void *param, uint32_t len)
{
    KISO_UNUSED(len);
    assert(NULL != param);
    assert(sizeof(uint8_t) == len);

    Retcode_T retcode = RETCODE_OK;
    uint8_t cid = *(uint8_t *)param;

    LOG_DEBUG("Deactivating data-context #%u.", (unsigned int)cid);

    AT_CGACT_Param_T cgact;
    cgact.Cid = cid;
    cgact.State = AT_CGACT_STATE_DEACTIVATED;

    retcode = At_Set_CGACT(&cgact); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        DataContexts[cid].Id = 0;
        DataContexts[cid].IpAddress.Type = CELLULAR_IPADDRESSTYPE_MAX;
        DataContexts[cid].Type = CELLULAR_DATACONTEXTTYPE_MAX;
        Engine_NotifyNewState(CELLULAR_STATE_REGISTERED, NULL, 0); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}

/**
 * @brief Query the ICCID from the SIM-card.
 *
 * To be encapsulated by via #Engine_Dispatch().
 *
 * @param[in,out] param
 * A valid #CellularNetwork_QueryIccidParam_S pointer to be filled with the
 * query result.
 *
 * @param[in] paramLen
 * Length of #CellularNetwork_QueryIccidParam_S structure.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T QueryIccid(void *param, uint32_t paramLen)
{
    KISO_UNUSED(paramLen);

    assert(NULL != param);
    assert(sizeof(struct CellularNetwork_QueryIccidParam_S) == paramLen);

    struct CellularNetwork_QueryIccidParam_S *query = (struct CellularNetwork_QueryIccidParam_S *)param;

    Retcode_T retcode = RETCODE_OK;

    AT_CCID_Resp_T ccid;
    retcode = At_Get_CCID(&ccid); //LCOV_EXCL_BR_LINE
    if (RETCODE_OK == retcode)
    {
        memset(query->Iccid, 0, *query->IccidLen);

        uint32_t bytesToCopy = *query->IccidLen > (uint32_t)ccid.Type ? (uint32_t)ccid.Type : *query->IccidLen;

        memcpy(query->Iccid, ccid.Iccid, bytesToCopy);

        if (*query->IccidLen < bytesToCopy)
        {
            /* ICCID got cropped. Considered a warning as defined in
             * API-doc. */
            retcode = RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_OUT_OF_RESOURCES);
        }

        *query->IccidLen = bytesToCopy;
    }

    return retcode;
}

/**
 * @brief Tell the modem to deregister from the network. The module will remain
 * idle and will not attempt a reconnect until #RegisterOnNetwork() is being
 * called.
 *
 * To be encapsulated by via #Engine_Dispatch().
 *
 * @param param Ignored.
 * @param len Ignored.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T DeregisterFromNetwork(void *param, uint32_t len)
{
    KISO_UNUSED(param);
    KISO_UNUSED(len);

    LOG_DEBUG("Disabling network registration and deregistering.");

    Retcode_T retcode = RETCODE_OK;
    AT_COPS_Param_T cops;
    cops.Mode = AT_COPS_MODE_DEREGISTER;
    cops.Format = AT_COPS_FORMAT_INVALID;
    cops.Oper.Numeric = 0;
    cops.AcT = AT_COPS_ACT_INVALID;

    retcode = At_Set_COPS(&cops); //LCOV_EXCL_BR_LINE

    if (RETCODE_OK == retcode)
    {
        Engine_NotifyNewState(CELLULAR_STATE_POWERON, NULL, 0); //LCOV_EXCL_BR_LINE
    }

    return retcode;
}
