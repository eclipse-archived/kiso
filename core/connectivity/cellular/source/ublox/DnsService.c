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

#include "Kiso_CellularDnsService.h"
#include "Kiso_Cellular.h"
#include "Kiso_CellularConfig.h"
#include "UBloxUtils.h"

#include "Engine.h"
#include "AT_UBlox.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

#define CELLULAR_DNS_SHORT_ENQUEUE_TIMEOUT   (UINT32_C(1000))

struct CellularDns_ResolveDomainParam_S
{
    const Cellular_DataContext_T* DataContext;
    const char* Fqdn;
    Cellular_IpAddress_T* Result;
};

/**
 * @brief Perform the domain name lookup.
 *
 * To be encapsulated by via #Engine_Dispatch().
 *
 * @param[in,out] param
 * A valid pointer to a #CellularDns_ResolveDomainParam_S structure.
 *
 * @param[in] paramLen
 * Length of #CellularDns_ResolveDomainParam_S structure.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
static Retcode_T ResolveDomain(void* param, uint32_t paramLen);

static Retcode_T ResolveDomain(void* param, uint32_t paramLen)
{
    KISO_UNUSED(paramLen);
    assert(NULL != param);
    assert(sizeof(struct CellularDns_ResolveDomainParam_S) == paramLen);

    struct CellularDns_ResolveDomainParam_S* rslvParam = (struct CellularDns_ResolveDomainParam_S*) param;
    Retcode_T retcode = RETCODE_OK;
    AT_UDNSRN_Param_T udnsrnParam;
    udnsrnParam.ResolutionType = AT_UDNSRN_RESOLUTIONTYPE_DOMAINTOIP;
    udnsrnParam.DomainIpString.Domain = rslvParam->Fqdn;
    AT_UDNSRN_Resp_T udnsrnResp;

    retcode = At_Set_UDNSRN(&udnsrnParam, &udnsrnResp);

    if (RETCODE_OK == retcode)
    {
        retcode = UbloxToCelAddr(&udnsrnResp.DomainIpString.Ip, rslvParam->Result);
    }

    return retcode;
}

Retcode_T CellularDns_ResolveDomain(
        const Cellular_DataContext_T* dataContext,
        const char* fqdn,
        Cellular_IpAddress_T* result)
{
    if (NULL == dataContext
            || NULL == fqdn
            || NULL == result)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        struct CellularDns_ResolveDomainParam_S param;
        param.DataContext = dataContext;
        param.Fqdn = fqdn;
        param.Result = result;
        return Engine_Dispatch(ResolveDomain, CELLULAR_DNS_SHORT_ENQUEUE_TIMEOUT, &param, sizeof(param));
    }

}
