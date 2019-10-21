/*******************************************************************************
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
 ******************************************************************************/

#include <gtest.h>

extern "C"
{
#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_HARDWARE

#include "Kiso_BSP_Cellular_SaraR4N4_th.hh"
#include "Kiso_BSP_UMTS_LisaU2_th.hh"

#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "Kiso_MCU_UART_th.hh"

#undef KISO_MODULE_ID
#include "Hardware.c"
}

class TS_Hardware : public testing::Test
{
protected:
    virtual void SetUp()
    {
        RESET_FAKE(BSP_Cellular_SaraR4N4_Connect);
        RESET_FAKE(BSP_Cellular_SaraR4N4_Enable);
        RESET_FAKE(BSP_Cellular_SaraR4N4_Reset);
        RESET_FAKE(BSP_Cellular_SaraR4N4_Disable);
        RESET_FAKE(BSP_Cellular_SaraR4N4_Disconnect);
        RESET_FAKE(BSP_Cellular_SaraR4N4_GetUARTHandle);
        RESET_FAKE(MCU_UART_Receive);
        RESET_FAKE(MCU_UART_Initialize);
        RESET_FAKE(MCU_UART_Deinitialize);
        FFF_RESET_HISTORY();
    }

    UART_T uartHandle = (UART_T)0;
    uint8_t dummyUartRxByte;
    static void dummyUartCallback(UART_T uart, struct MCU_UART_Event_S event)
    {
        KISO_UNUSED(uart);
        KISO_UNUSED(event);
    }
};

TEST_F(TS_Hardware, Hardware_Initialize_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    MCU_UART_Initialize_fake.return_val = RETCODE_OK;
    BSP_Cellular_SaraR4N4_Connect_fake.return_val = RETCODE_OK;
    rc = Hardware_Initialize(dummyUartCallback, &dummyUartRxByte);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
    EXPECT_EQ(true, Initialized);
}

TEST_F(TS_Hardware, Hardware_Double_Initialize)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    MCU_UART_Initialize_fake.return_val = RETCODE_OK;
    BSP_Cellular_SaraR4N4_Connect_fake.return_val = RETCODE_OK;
    rc = Hardware_Initialize(dummyUartCallback, &dummyUartRxByte);
    EXPECT_EQ(RETCODE_DOUBLE_INITIALIZATION, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Initialize_BSP_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    BSP_Cellular_SaraR4N4_Connect_fake.return_val = RETCODE_INCONSISTENT_STATE;
    MCU_UART_Initialize_fake.return_val = RETCODE_OK;
    rc = Hardware_Initialize(dummyUartCallback, &dummyUartRxByte);
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(false, Initialized);
}

TEST_F(TS_Hardware, Hardware_Initialize_Callback_param_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    BSP_Cellular_SaraR4N4_Connect_fake.return_val = RETCODE_OK;
    MCU_UART_Initialize_fake.return_val = RETCODE_NULL_POINTER;
    rc = Hardware_Initialize(NULL, NULL);
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(rc));
    EXPECT_EQ(false, Initialized);
}

TEST_F(TS_Hardware, Hardware_Startup_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    BSP_Cellular_SaraR4N4_Enable_fake.return_val = RETCODE_OK;
    MCU_UART_Receive_fake.return_val = RETCODE_OK;
    rc = Hardware_Startup();
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Startup_BSP_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    BSP_Cellular_SaraR4N4_Enable_fake.return_val = RETCODE_INCONSISTENT_STATE;
    MCU_UART_Receive_fake.return_val = RETCODE_OK;
    rc = Hardware_Startup();
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Startup_NonInit_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    BSP_Cellular_SaraR4N4_Enable_fake.return_val = RETCODE_OK;
    MCU_UART_Receive_fake.return_val = RETCODE_OK;
    rc = Hardware_Startup();
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Startup_Receive_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    BSP_Cellular_SaraR4N4_Enable_fake.return_val = RETCODE_OK;
    MCU_UART_Receive_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    rc = Hardware_Startup();
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Shutdown_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    MCU_UART_Receive_fake.return_val = RETCODE_OK;
    BSP_Cellular_SaraR4N4_Disable_fake.return_val = RETCODE_OK;
    rc = Hardware_Shutdown();
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Shutdown_NonInit_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    MCU_UART_Receive_fake.return_val = RETCODE_OK;
    BSP_Cellular_SaraR4N4_Disable_fake.return_val = RETCODE_OK;
    rc = Hardware_Shutdown();
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Shutdown_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    MCU_UART_Receive_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    BSP_Cellular_SaraR4N4_Disable_fake.return_val = RETCODE_OK;
    rc = Hardware_Shutdown();
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Reset_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    BSP_Cellular_SaraR4N4_Reset_fake.return_val = RETCODE_OK;
    rc = Hardware_Reset();
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Reset_NonInit_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    BSP_Cellular_SaraR4N4_Reset_fake.return_val = RETCODE_OK;
    rc = Hardware_Reset();
    EXPECT_EQ(BSP_Cellular_SaraR4N4_Reset_fake.call_count, 0U);
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Reset_Module_Reset_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    BSP_Cellular_SaraR4N4_Reset_fake.return_val = RETCODE_FAILURE;
    rc = Hardware_Reset();
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_Deinitialize_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    MCU_UART_Deinitialize_fake.return_val = RETCODE_OK;
    BSP_Cellular_SaraR4N4_Disconnect_fake.return_val = RETCODE_OK;
    rc = Hardware_Deinitialize();
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
    EXPECT_EQ(false, Initialized);
}

TEST_F(TS_Hardware, Hardware_Deinitialize_NonInit_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    MCU_UART_Deinitialize_fake.return_val = RETCODE_OK;
    BSP_Cellular_SaraR4N4_Disconnect_fake.return_val = RETCODE_OK;
    rc = Hardware_Deinitialize();
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(rc));
    EXPECT_EQ(false, Initialized);
}

TEST_F(TS_Hardware, Hardware_Deinitialize_MCU_Uart_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    MCU_UART_Deinitialize_fake.return_val = RETCODE_FAILURE;
    BSP_Cellular_SaraR4N4_Disconnect_fake.return_val = RETCODE_OK;
    rc = Hardware_Deinitialize();
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(true, Initialized);
}

TEST_F(TS_Hardware, Hardware_GetCommunicationChannel_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = true;
    rc = Hardware_GetCommunicationChannel(&uartHandle);
    EXPECT_NE((UART_T)NULL, &uartHandle);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_GetCommunicationChannel_NonInit_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    rc = Hardware_GetCommunicationChannel(&uartHandle);
    EXPECT_EQ(RETCODE_UNINITIALIZED, Retcode_GetCode(rc));
}

TEST_F(TS_Hardware, Hardware_GetCommunicationChannel_NoUartHandle_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Initialized = false;
    rc = Hardware_GetCommunicationChannel(NULL);
    EXPECT_EQ(RETCODE_NULL_POINTER, Retcode_GetCode(rc));
}