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
 * @brief Resolves a domain name to an IP address via the network provided DNS server.
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_DNS

#include "CellularModule.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "portmacro.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "CellularCommon.h"
#include "CellularAtResponseParser.h"
#include "CellularResponseQueue.h"
#include "CellularEngine.h"
#include "CellularUtils.h"
#include "Dns.h"

#define CELLULAR_DNS_ENQUEUE_TIMEOUT            (UINT32_C(1000))

/**
 * @brief The time to wait for the modem to respond to a DNS resolve request.
 *        We set this to 80 seconds, as the datasheet specifices a response within 70 seconds (providing some tolerance here).
 *        See u-Blox AT Commands Manual, Pg. 479, Ch. 24.1, Resolve name / IP number through DNS +UDNSRN.
 */
#define CELLULAR_DNS_RESPONSE_TIMEOUT           (UINT32_C(80000))

/**
 * @brief The maximum length of a domain name we can resolve
 *        (choose so that our request AT command doesn't get longer than 250 chars).
 */
#define CELLULAR_DNS_MAX_DOMAIN_NAME_LENGTH     (UINT32_C(200))

/**
 * @brief The parameter structure for Cellular_ResolveDomainName
 */
struct CellularDns_Parameter_S
{
    uint8_t *DomainName;
    uint32_t DomainNameLength;
    uint32_t ResolvedIpAddress;
};
typedef struct CellularDns_Parameter_S CellularDns_Parameter_T;

static Retcode_T ResolveDomainName(void *parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(ParameterLength);

    // check our parameter and unpack it
    Retcode_T retcode = RETCODE_OK;
    CellularDns_Parameter_T *DnsRequestParameter = (CellularDns_Parameter_T *) parameter;

    // prepare the AT command (we've checked that DomainNameLength < CELLULAR_DNS_MAX_DOMAIN_NAME_LENGTH in Cellular_ResolveDomainName)
    const char *AtCommandTemplate = "AT+UDNSRN=0,\"%.*s\"\r\n";
    uint8_t AtCommand[DnsRequestParameter->DomainNameLength + strlen(AtCommandTemplate)];
    int SnprintResult = snprintf((char *) AtCommand, sizeof(AtCommand), AtCommandTemplate, (int) DnsRequestParameter->DomainNameLength, DnsRequestParameter->DomainName);
    if (SnprintResult <= 0)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    uint32_t AtCommandLength = strlen((const char *) AtCommand);

    // off we go -> send the AT command
    if (RETCODE_OK == retcode)
    {
        retcode = CellularEngine_SendAtCommand(AtCommand, AtCommandLength);
    }

    // let's wait for answers
    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmdEcho(CELLULAR_DNS_RESPONSE_TIMEOUT, AtCommand, AtCommandLength - 2);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = AtResponseQueue_WaitForNamedCmd(CELLULAR_DNS_RESPONSE_TIMEOUT, (const uint8_t *) "UDNSRN", strlen("UDNSRN"));
    }

    if (RETCODE_OK == retcode)
    {
        uint8_t *IpAddressBuffer;
        uint32_t IpAddressBufferLen;

        retcode = AtResponseQueue_WaitForArbitraryCmdArg(CELLULAR_DNS_RESPONSE_TIMEOUT, &IpAddressBuffer, &IpAddressBufferLen);

        if (RETCODE_OK == retcode)
        {
            if (IpAddressBufferLen > 2)
            {
                /*
                 * The string has to be longer than two characters so that we can trim the " chars away (hence the +1/-2 below).
                 */
                retcode = CellularUtils_ParseIpAddress(IpAddressBuffer + 1, IpAddressBufferLen - 2, &(DnsRequestParameter->ResolvedIpAddress));
                AtResponseQueue_MarkBufferAsUnused();
            }
            else
            {
                AtResponseQueue_MarkBufferAsUnused();
                return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
    }

    return CellularUtils_WaitForAndHandleResponseCode(CELLULAR_DNS_RESPONSE_TIMEOUT, retcode);
}

Retcode_T Cellular_ResolveDomainName(uint8_t *DomainName, uint32_t DomainNameLength, uint32_t *IpAddress)
{
    if (NULL == DomainName || NULL == IpAddress || 0 == DomainNameLength
            || DomainNameLength > CELLULAR_DNS_MAX_DOMAIN_NAME_LENGTH)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    // setup the parameter structure
    CellularDns_Parameter_T parameter =
            {
                    .DomainName = DomainName,
                    .DomainNameLength = DomainNameLength,
                    .ResolvedIpAddress = 0
            };

    // call the thing
    Retcode_T retcode = CellularRequest_Dispatch(CELLULAR_DNS_ENQUEUE_TIMEOUT, ResolveDomainName, (void*) &parameter, sizeof(parameter));

    if (RETCODE_OK == retcode)
    {
        *IpAddress = parameter.ResolvedIpAddress;
    }

    return retcode;
}

