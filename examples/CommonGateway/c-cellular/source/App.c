/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     Robert Bosch GmbH - initial contribution
 */
#include "AppModules.h"
#define KISO_MODULE_ID APP_MODULE_ID_APP

#include "App.h"
#include "AppConfig.h"
#include "AppRetcodes.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Cellular.h"
#include "Kiso_CellularDnsService.h"
#include "Kiso_CellularSocketService.h"
#include "Kiso_CellularHttpService.h"
#include "Kiso_CmdProcessor.h"
#include "Kiso_Logging.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

struct App_DemoStage_S
{
    const char *Description;
    Retcode_T (*Run)(void);
    struct App_DemoStage_S *Next;
};

static void HandleStateChanged(Cellular_State_T oldState, Cellular_State_T newState, void *param, uint32_t len);
static void HandleSocketClosed(CellularSocket_Handle_T closedSocket);
static void HandleSocketDataReady(CellularSocket_Handle_T socket, uint32_t numBytesAvailable);
static void HandleHttpResultAvailable(CellularHttp_Method_T method, CellularHttp_Result_T result);
static Retcode_T InitializeCellular(void);
static Retcode_T PowerOnCellular(void);
static Retcode_T RegisterCellular(void);
static Retcode_T DnsLookup(void);
static Retcode_T UdpEcho(void);
static Retcode_T TcpEcho(void);
static Retcode_T HttpGet(void);
static Retcode_T PowerOffCellular(void);

struct App_DemoStage_S PowerOffStage =
    {
        "power-off",
        PowerOffCellular,
        NULL};
struct App_DemoStage_S HttpGetStage =
    {
        "http-get",
        HttpGet,
        &PowerOffStage};
struct App_DemoStage_S TcpEchoStage =
    {
        "tcp-echo",
        TcpEcho,
        &HttpGetStage};
struct App_DemoStage_S UdpEchoStage =
    {
        "udp-echo",
        UdpEcho,
        &TcpEchoStage};
struct App_DemoStage_S DnsStage =
    {
        "dns",
        DnsLookup,
        &UdpEchoStage};
struct App_DemoStage_S RegisterStage =
    {
        "register",
        RegisterCellular,
        &UdpEchoStage};
struct App_DemoStage_S PowerOnStage =
    {
        "power-on",
        PowerOnCellular,
        &RegisterStage};
struct App_DemoStage_S InitStage =
    {
        "init",
        InitializeCellular,
        &PowerOnStage};

static struct App_DemoStage_S *CurrentStage;
static Cellular_State_T CurrentCellularState = CELLULAR_STATE_MAX;
static CmdProcessor_T *AppCmdProcessor;
static StaticTask_t AppTcb;
static StackType_t AppStack[APP_TASK_APP_STACKSIZE];
static TaskHandle_t AppTask;
static StaticSemaphore_t CellularStateChangedBlock;
static SemaphoreHandle_t CellularStateChangedSignal;
static StaticSemaphore_t SocketStateChangedBlock;
static SemaphoreHandle_t SocketStateChangedSignal;
static StaticSemaphore_t HttpStateChangedBlock;
static SemaphoreHandle_t HttpStateChangedSignal;
static const Cellular_DataContext_T *DataContext;
static uint32_t NumCurrentAttempt = 0;

static void HandleStateChanged(Cellular_State_T oldState, Cellular_State_T newState, void *param, uint32_t len)
{
    KISO_UNUSED(oldState);
    KISO_UNUSED(param);
    KISO_UNUSED(len);

    CurrentCellularState = newState;

    (void)xSemaphoreGive(CellularStateChangedSignal);
}

static void HandleSocketClosed(CellularSocket_Handle_T closedSocket)
{
    KISO_UNUSED(closedSocket);
    LOG_INFO("Socket closed: %u", (unsigned int)closedSocket);

    (void)xSemaphoreGive(SocketStateChangedSignal);
}

static void HandleSocketDataReady(CellularSocket_Handle_T socket, uint32_t numBytesAvailable)
{
    KISO_UNUSED(socket);
    KISO_UNUSED(numBytesAvailable);
    LOG_INFO("Received %u bytes on socket %u", (unsigned int)numBytesAvailable, (unsigned int)socket);

    (void)xSemaphoreGive(SocketStateChangedSignal);
}
#ifdef KISO_LOGGING
static const char *HttpMethodToStr(CellularHttp_Method_T method)
{
    switch (method)
    {
    case CELLULAR_HTTP_METHOD_HEAD:
        return "HEAD";
    case CELLULAR_HTTP_METHOD_GET:
        return "GET";
    case CELLULAR_HTTP_METHOD_POST:
        return "POST";
    default:
        return "<INVALID>";
    }
}
#endif

static void HandleHttpResultAvailable(CellularHttp_Method_T method, CellularHttp_Result_T result)
{
    KISO_UNUSED(method);
    KISO_UNUSED(result);

    LOG_INFO("Received HTTP-%s with result %u", HttpMethodToStr(method), (unsigned int)result);
}

static Retcode_T InitializeCellular(void)
{
    Retcode_T rc = Cellular_Initialize(HandleStateChanged);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during cellular initialization: %x", rc);
        return rc;
    }

    /* FIXME: should be obtained via Cellular_GetState() or state-callback. */
    CurrentCellularState = CELLULAR_STATE_POWEROFF;

    return rc;
}

static Retcode_T PowerOnCellular(void)
{
    Cellular_PowerUpParameters_T powerUpParam;
    powerUpParam.SimPin = APP_SIM_PIN;
    LOG_DEBUG("Using SIM PIN: '%s'", NULL == powerUpParam.SimPin ? "<NO PIN>" : powerUpParam.SimPin);
    Retcode_T rc = Cellular_PowerOn(&powerUpParam);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during cellular power-up: %x", rc);
        return rc;
    }

    (void)xSemaphoreTake(CellularStateChangedSignal, portMAX_DELAY);
    assert(CELLULAR_STATE_POWERON == CurrentCellularState);

    /* This delay is completely arbitrary and only servers to clear the
     * logging-queue to prevent losing log messages */
    vTaskDelay(2000);

    return rc;
}

static Retcode_T RegisterCellular(void)
{
    Cellular_DataContextParameters_T ctxParam;
    ctxParam.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
    ctxParam.ApnSettings.ApnName = APP_APN_NAME;
    ctxParam.ApnSettings.AuthMethod = APP_APN_AUTHMETHOD;
    ctxParam.ApnSettings.Username = APP_APN_USER;
    ctxParam.ApnSettings.Password = APP_APN_PASSWORD;
    Retcode_T rc = Cellular_ConfigureDataContext(0, &ctxParam);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error while configuring data-context: %x", rc);
        return rc;
    }

    Cellular_NetworkParameters_T nwParam;
    nwParam.AcT = APP_RAT;
    nwParam.FallbackAcT = CELLULAR_RAT_DEFAULT;
    rc = Cellular_RegisterOnNetwork(&nwParam);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during network-registration: %x", rc);
        return rc;
    }

    while (CELLULAR_STATE_REGISTERED != CurrentCellularState)
    {
        LOG_DEBUG("Waiting for network-registration...");
        (void)xSemaphoreTake(CellularStateChangedSignal, portMAX_DELAY);
        LOG_DEBUG(" -> State change; new state: %d", (int)CurrentCellularState)
    }

    rc = Cellular_ActivateDataContext(0, &DataContext);

    if (RETCODE_OK == rc)
    {
        (void)xSemaphoreTake(CellularStateChangedSignal, portMAX_DELAY);
        assert(CELLULAR_STATE_DATAACTIVE == CurrentCellularState);
    }

    /* This delay is completely arbitrary and only servers to clear the
     * logging-queue to prevent losing log messages */
    vTaskDelay(5000);

    return rc;
}

static Retcode_T DnsLookup(void)
{
    Cellular_IpAddress_T ip;
    Retcode_T rc = CellularDns_ResolveDomain(DataContext, "telekom.de", &ip);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during DNS lookup: %x", rc);
        return rc;
    }

    return rc;
}

static Retcode_T UdpEcho(void)
{
    Retcode_T cleanup_rc;

    CellularSocket_Handle_T socket;
    Retcode_T rc = CellularSocket_CreateAndBind(&socket, DataContext, 0, CELLULAR_SOCKET_PROTOCOL_UDP, HandleSocketClosed, HandleSocketDataReady);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during socket creation: %x", rc);
        goto cleanup;
    }

    /* This delay is completely arbitrary and only servers to clear the
     * logging-queue to prevent losing log messages */
    vTaskDelay(1000);

    const char *dataToSend = APP_UDP_TEST_DATA;
    Cellular_IpAddress_T remoteIp = APP_UDP_TEST_IP;
    uint16_t remotePort = APP_UDP_TEST_PORT;
    rc = CellularSocket_SendTo(socket, (const uint8_t *)dataToSend, strlen(dataToSend), &remoteIp, remotePort);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during socket sending: %x", rc);
        goto cleanup;
    }

    (void)xSemaphoreTake(SocketStateChangedSignal, portMAX_DELAY);

    /* This delay is completely arbitrary and only servers to clear the
     * logging-queue to prevent losing log messages */
    vTaskDelay(1000);

    uint32_t numBytesAvailable = 0;
    rc = CellularSocket_QueryBytesAvailable(socket, &numBytesAvailable);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during socket data-available query: %x", rc);
        goto cleanup;
    }

    /* This delay is completely arbitrary and only servers to clear the
     * logging-queue to prevent losing log messages */
    vTaskDelay(1000);

    uint32_t rxBufLen = sizeof(APP_UDP_TEST_DATA);
    uint8_t rxBuf[sizeof(APP_UDP_TEST_DATA)];
    uint32_t bytesReceived = 0;
    rc = CellularSocket_ReceiveFrom(socket, rxBuf, rxBufLen, &bytesReceived, &remoteIp, &remotePort);
    if (RETCODE_OK == rc)
    {
        if (remoteIp.Type == CELLULAR_IPADDRESSTYPE_IPV4)
        {
            LOG_INFO("Received %u bytes from %u.%u.%u.%u:%u: %.*s",
                     bytesReceived,
                     remoteIp.Address.IPv4[3],
                     remoteIp.Address.IPv4[2],
                     remoteIp.Address.IPv4[1],
                     remoteIp.Address.IPv4[0],
                     remotePort,
                     bytesReceived, rxBuf);
        }
        else
        {
            LOG_INFO("Received %u bytes from [%x:%x:%x:%x]:%u: %.*s",
                     bytesReceived,
                     ((uint16_t *)remoteIp.Address.IPv6)[7],
                     ((uint16_t *)remoteIp.Address.IPv6)[6],
                     ((uint16_t *)remoteIp.Address.IPv6)[5],
                     ((uint16_t *)remoteIp.Address.IPv6)[4],
                     ((uint16_t *)remoteIp.Address.IPv6)[3],
                     ((uint16_t *)remoteIp.Address.IPv6)[2],
                     ((uint16_t *)remoteIp.Address.IPv6)[1],
                     ((uint16_t *)remoteIp.Address.IPv6)[0],
                     remotePort,
                     bytesReceived, rxBuf);
        }
    }
    else
    {
        LOG_FATAL("Error during socket receive: %x", rc);
        goto cleanup;
    }

cleanup:
    cleanup_rc = CellularSocket_Close(socket);
    if (RETCODE_OK != cleanup_rc)
    {
        LOG_FATAL("Error during socket closing: %x", cleanup_rc);
        /* cleanup error will be supressed */
    }

    return rc;
}

static Retcode_T TcpEcho(void)
{
    Retcode_T cleanup_rc;

    CellularSocket_Handle_T socket;
    Retcode_T rc = CellularSocket_CreateAndBind(&socket, DataContext, 0, CELLULAR_SOCKET_PROTOCOL_TCP, HandleSocketClosed, HandleSocketDataReady);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during socket creation: %x", rc);
        goto cleanup;
    }

    /* This delay is completely arbitrary and only servers to clear the
     * logging-queue to prevent losing log messages */
    vTaskDelay(1000);

    Cellular_IpAddress_T remoteIp = APP_TCP_TEST_IP;
    uint16_t remotePort = APP_TCP_TEST_PORT;
    rc = CellularSocket_Connect(socket, &remoteIp, remotePort);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during socket connect: %x", rc);
        goto cleanup;
    }

    /* This delay is completely arbitrary and only servers to clear the
     * logging-queue to prevent losing log messages */
    vTaskDelay(1000);

    const char *dataToSend = APP_TCP_TEST_DATA;
    rc = CellularSocket_Send(socket, (const uint8_t *)dataToSend, strlen(dataToSend));
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during socket sending: %x", rc);
        goto cleanup;
    }

    (void)xSemaphoreTake(SocketStateChangedSignal, portMAX_DELAY);

    /* This delay is completely arbitrary and only servers to clear the
     * logging-queue to prevent losing log messages */
    vTaskDelay(1000);

    uint32_t numBytesAvailable = 0;
    rc = CellularSocket_QueryBytesAvailable(socket, &numBytesAvailable);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during socket data-available query: %x", rc);
        goto cleanup;
    }

    while (numBytesAvailable > 0)
    {
        uint32_t rxBufLen = sizeof(APP_TCP_TEST_DATA);
        uint8_t rxBuf[sizeof(APP_UDP_TEST_DATA)];
        uint32_t bytesReceived = 0;
        rc = CellularSocket_Receive(socket, rxBuf, rxBufLen, &bytesReceived);
        if (RETCODE_OK == rc)
        {
            numBytesAvailable -= bytesReceived;
            if (remoteIp.Type == CELLULAR_IPADDRESSTYPE_IPV4)
            {
                LOG_INFO("Received %u bytes: %.*s",
                         bytesReceived,
                         bytesReceived, rxBuf);
            }
            else
            {
                LOG_INFO("Received %u bytes: %.*s",
                         bytesReceived,
                         bytesReceived, rxBuf);
            }
        }
        else
        {
            LOG_FATAL("Error during socket receive: %x", rc);
            goto cleanup;
        }
    }

cleanup:
    cleanup_rc = CellularSocket_Close(socket);
    if (RETCODE_OK != cleanup_rc)
    {
        LOG_FATAL("Error during socket closing: %x", cleanup_rc);
        /* cleanup error will be supressed */
    }

    return rc;
}

static Retcode_T HttpGet(void)
{
    Retcode_T rc = CellularHttp_Initialize(HandleHttpResultAvailable);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during http init: %x", rc);
        return rc;
    }

    CellularHttp_Request_T req;
    req.Method = CELLULAR_HTTP_METHOD_GET;
    req.Server = "httpbin.org";
    req.Path = "/get";
    req.Port = 80;
    req.ContentType = CELLULAR_HTTP_CONTENTTYPE_TEXT_PLAIN;
    req.Data = NULL;
    req.IsSecure = false;

    rc = CellularHttp_SendRequest(&req);
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during http get: %x", rc);
        return rc;
    }

    uint32_t respSize = 0;
    rc = CellularHttp_GetResponseSize(&respSize);
    if (RETCODE_OK == rc)
    {
        LOG_INFO("Obtained %u byte(s) long HTTP response.", respSize);
    }
    else
    {
        LOG_FATAL("Error while reading http response size: %x", rc);
        return rc;
    }

    uint8_t buf[703 + 1];
    buf[sizeof(buf) - 1] = '\0';
    CellularHttp_Data_T resp;
    resp.Buffer = buf;
    resp.BufferLength = 0;
    resp.BufferSize = sizeof(buf) - 1;
    rc = CellularHttp_GetResponse(&resp);
    if (RETCODE_OK == rc)
    {
        LOG_INFO("First %u bytes of HTTP response: %.*s", resp.BufferLength, (int)resp.BufferLength, resp.Buffer);
    }
    else
    {
        LOG_FATAL("Error while reading http response: %x", rc);
        return rc;
    }

    return RETCODE_OK;
}

static Retcode_T PowerOffCellular(void)
{
    Retcode_T rc = Cellular_PowerOff();
    if (RETCODE_OK != rc)
    {
        LOG_FATAL("Error during cellular power-off: %x", rc);
        return rc;
    }

    (void)xSemaphoreTake(CellularStateChangedSignal, portMAX_DELAY);
    assert(CELLULAR_STATE_POWEROFF == CurrentCellularState);

    return rc;
}

static void RunAppTask(void *param)
{
    KISO_UNUSED(param);

    for (;;)
    {
        NumCurrentAttempt++;
        assert(NULL != CurrentStage);

        Retcode_T rc;
        if (NumCurrentAttempt <= APP_NUMBER_OF_RETRIES)
        {
            LOG_INFO("Performing operation stage: %s... (%u/%u)", CurrentStage->Description, NumCurrentAttempt, APP_NUMBER_OF_RETRIES);
            rc = CurrentStage->Run();
#if INCLUDE_uxTaskGetStackHighWaterMark
            LOG_DEBUG("STACK-WATERMARK: %s@%u words", pcTaskGetName(xTaskGetCurrentTaskHandle()),
                      uxTaskGetStackHighWaterMark(xTaskGetCurrentTaskHandle()));
#endif
        }
        else
        {
            LOG_FATAL("To many failed attempts of stage: %s. App will now halt!", CurrentStage->Description);
            /* Halt execution because of too many errors. */
            vTaskSuspend(xTaskGetCurrentTaskHandle());
            /* Just in case we get resumed by someone... */
            CurrentStage = &InitStage;
            NumCurrentAttempt = 0;
        }

        /* This delay is completely arbitrary and only servers to clear the
         * logging-queue to prevent losing log messages */
        vTaskDelay(1000);

        if (RETCODE_OK == rc)
        {
            if (NULL != CurrentStage->Next)
            {
                LOG_DEBUG("Transitioning to stage: %s...", CurrentStage->Next->Description);
                CurrentStage = CurrentStage->Next;
                NumCurrentAttempt = 0;
            }
            else
            {
                LOG_INFO("Final demo stage completed. App will now halt!");
                /* Halt execution once end of demo is reached. */
                vTaskSuspend(xTaskGetCurrentTaskHandle());
                /* Just in case we get resumed by someone... */
                CurrentStage = &InitStage;
                NumCurrentAttempt = 0;
            }
        }

        if (RETCODE_OK != rc)
        {
            Retcode_RaiseError(rc);
        }
    }
}

void App_InitSystem(void *param, uint32_t len)
{
    LOG_DEBUG("Initializing app...");

    KISO_UNUSED(len);
    assert(NULL != param);
    AppCmdProcessor = (CmdProcessor_T *)param;

    CurrentStage = &InitStage;

    CellularStateChangedSignal = xSemaphoreCreateBinaryStatic(&CellularStateChangedBlock);
    assert(NULL != CellularStateChangedSignal);

    SocketStateChangedSignal = xSemaphoreCreateBinaryStatic(&SocketStateChangedBlock);
    assert(NULL != SocketStateChangedSignal);

    HttpStateChangedSignal = xSemaphoreCreateBinaryStatic(&HttpStateChangedBlock);
    assert(NULL != HttpStateChangedSignal);

    AppTask = xTaskCreateStatic(RunAppTask, "App Task", APP_TASK_APP_STACKSIZE, NULL, APP_TASK_APP_PRIORITY, AppStack, &AppTcb);
    assert(NULL != AppTask);

    LOG_INFO("App initialization complete!");
}
