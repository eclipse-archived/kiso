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
 * @brief Handles GSM connectivity by using other functions of the module
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_GSM

#include "CellularModule.h"
#include "CellularBsp.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Cellular.h"
#include "BCDS_CellularUdpSocket.h"
#include "CellularCommon.h"
#include "Control.h"
#include "Socket.h"
#include "Network.h"
#include "CellularEngine.h"
#include "Dns.h"

Retcode_T Cellular_Initialize(Cellular_CallbackFunction_T EventCallback)
{
    return CellularEngine_Init(EventCallback);
}

Retcode_T Cellular_Uninitialize(void)
{
    return CellularEngine_Deinit();
}

Cellular_DeviceStatus_T Cellular_GetDeviceStatus(void)
{
    return CellularEngine_GetDeviceStatus();
}

Retcode_T Cellular_PowerControl(Cellular_PowerState_T state)
{
    switch (state)
    {
        case CELLULAR_POWER_STATE_OFF:
        case CELLULAR_POWER_STATE_OFF_EAT:
        case CELLULAR_POWER_STATE_SLEEP:
        {
            return Cellular_Shutdown();
        }

        case CELLULAR_POWER_STATE_WAKEUP:
        case CELLULAR_POWER_STATE_ON:
        {
            return Cellular_Startup();
        }

    default:
        break;
    }

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T Cellular_QueryModem(Cellular_QueryInfo_T info, void* value)
{
    Retcode_T retcode;

    switch (info)
    {
    case CELLULAR_QUERYINFO_NETWORKINFO:
        {
        CellularNetwork_RegistrationStatus_T data;
        retcode = CellularNetwork_QueryRegistrationStatus(&data);

        if (RETCODE_OK == retcode)
        {
            Cellular_NetworkInfo_T *p = (Cellular_NetworkInfo_T *) value;
            memcpy(p->LocationCode, &data.Lac, CELLULAR_LOCATION_CODE_LENGTH);
            memcpy(p->CellId, &data.Ci, CELLULAR_LOCATION_CODE_LENGTH);
            p->NetworkCodeLength = CELLULAR_NETWORK_CODE_LENGTH;
        }
        return retcode;
    }

    case CELLULAR_QUERYINFO_ERROR:
        {
            *(Retcode_T *)value = CellularEngine_GetDeviceError();
            return RETCODE_OK;
    }

    case CELLULAR_QUERYINFO_STATUS:
        {
            *(Cellular_DeviceStatus_T *)value = CellularEngine_GetDeviceStatus();
        return RETCODE_OK;
    }

    default:
        break;
    }

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T Cellular_Control(Cellular_Control_T control, void* value)
{
    BCDS_UNUSED(value);

    switch (control)
    {
    case CELLULAR_RESET:
        {
        return Cellular_Reset();
    }

    case CELLULAR_CONFIG:
        {
        Cellular_ConfigurationSettings_T *p = (Cellular_ConfigurationSettings_T *) value;
        if (p->ApnName && strlen((char *) p->ApnName) > CELLULAR_NETWORK_APN_LENGTH_MAX)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        else if (p->ApnUserName && strlen((char *) p->ApnUserName) > CELLULAR_NETWORK_USERNAME_LENGTH_MAX)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        else if (p->ApnPassword && strlen((char *) p->ApnPassword) > CELLULAR_NETWORK_PASSWORD_LENGTH_MAX)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }

        Cellular_PsdProfile_T profile =
                {
                        .Id = CELLULAR_NETWORK_PROFILE_ID_DEFAULT,
                        .Protocol = CELLULAR_PSD_PROTOCOL_IPV4,
                        .Apn = (char *) p->ApnName,
                        .Username = (char *) p->ApnUserName,
                        .Password = (char *) p->ApnPassword,
                        .Authentication = CELLULAR_PSD_AUTH_NONE
                };

        return CellularNetwork_SetPsdProfile(&profile);
    }

    case CELLULAR_TEST:
        default:
        break;
    }

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T Cellular_NetworkInit(void)
{
    return CellularNetwork_Init();
}

Retcode_T Cellular_Connect(void)
{
    return CellularNetwork_ActivatePdpContext();
}

Retcode_T Cellular_Disconnect(void)
{
    return CellularNetwork_DeactivatePdpContext();
}

Retcode_T CellularUdpSocket_Open(CellularUdpSocket_T *socket, CellularUdpSocket_Callback_T callback)
{
    if (NULL == socket || NULL == callback)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    CellularSocket_OpenRequest_T r;

    r.ContextId = CELLULAR_NETWORK_PROFILE_ID_DEFAULT;
    r.SocketId = socket;
    r.StatusCallback = callback;
    /* To comply with GSM API force configuration to UDP Service */
    r.SocketType = CELLULAR_SOCKET_TYPE_UDP_SERVICE;
    r.DomainName = "127.0.0.1";
    r.RemotePort = 0;
    r.LocalPort = 0;

    return CellularSocket_Open(&r);
}

Retcode_T CellularUdpSocket_Close(CellularUdpSocket_T socket)
{
    return CellularSocket_Close(socket);
}

Retcode_T CellularUdpSocket_Listen(CellularUdpSocket_T socket, uint16_t port)
{
    BCDS_UNUSED(socket);
    BCDS_UNUSED(port);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

Retcode_T CellularUdpSocket_Receive(CellularUdpSocket_T socket, uint32_t * IpAddress,
        uint16_t * port, uint8_t **buffer, uint32_t * length)
{
    return CellularSocket_ReceiveFrom(socket, IpAddress, port, buffer, length);
}

Retcode_T CellularUdpSocket_Send(CellularUdpSocket_T socket, const uint32_t IpAddress,
        uint16_t port, uint8_t *buffer, uint32_t length)
{
    return CellularSocket_SendTo(socket, IpAddress, port, buffer, length);
}

Retcode_T CellularUdpSocket_ResolveIp(uint8_t *DomainName, uint32_t DomainNameLength, uint32_t *IpAddress)
{
    BCDS_UNUSED(DomainName);
    BCDS_UNUSED(DomainNameLength);
    BCDS_UNUSED(IpAddress);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/* Deprecated function. See header file for more information */
Retcode_T CellularSocketUdp_Open(CellularUdpSocket_T *socket, CellularUdpSocket_Callback_T callback)
{
    return CellularUdpSocket_Open(socket, callback);
}

/* Deprecated function. See header file for more information */
Retcode_T CellularSocketUdp_Close(CellularUdpSocket_T socket)
{
    return CellularUdpSocket_Close(socket);
}

/* Deprecated function. See header file for more information */
Retcode_T CellularSocketUdp_Listen(CellularUdpSocket_T socket, uint16_t port)
{
	return CellularUdpSocket_Listen(socket, port);
}

/* Deprecated function. See header file for more information */
Retcode_T CellularSocketUdp_Receive(CellularUdpSocket_T socket, uint32_t * IpAddress,
        uint16_t * port, uint8_t **buffer, uint32_t * length)
{
    return CellularUdpSocket_Receive(socket, IpAddress, port, buffer, length);
}

/* Deprecated function. See header file for more information */
Retcode_T CellularSocketUdp_Send(CellularUdpSocket_T socket, const uint32_t IpAddress,
        uint16_t port, uint8_t *buffer, uint32_t length)
{
    return CellularUdpSocket_Send(socket, IpAddress, port, buffer, length);
}

/* Deprecated function. See header file for more information */
Retcode_T Cellular_ResolveName(uint8_t *DomainName, uint32_t DomainNameLength, uint32_t *IpAddress)
{
    return CellularUdpSocket_ResolveIp(DomainName, DomainNameLength, IpAddress);
}
