/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/


/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START

/* setup compile time configuration defines */
//-- instead of BCDS_BSP_BoardConfig.h mock
/* include faked interfaces */
extern "C"
{
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"

#include "drivers\I2C.h"

/* Add vendor specific fake functions needed */
FAKE_VALUE_FUNC(bool, I2C_transfer,I2C_Handle , I2C_Transaction *)
;
FAKE_VOID_FUNC(I2C_cancel, I2C_Handle)
;

#undef BCDS_FEATURE_I2C
#define BCDS_FEATURE_I2C 1

/* include module under test */
#include "BCDS_MCU_I2C.c"

/* Fake error handler */

}
/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

class BCDS_I2Ctest: public testing::Test
{
protected:

    virtual void SetUp()
    {
        DeInit();

        FFF_RESET_HISTORY()
    }

    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }

    void DeInit()
    {

    }

public:
    static const struct MCU_I2C_Event_S m_EventInit;
    static bool m_isCalled_cbf;
    static uint32_t m_I2C_cbf;
    static struct MCU_I2C_Event_S m_event_cbf;
};


const struct MCU_I2C_Event_S BCDS_I2Ctest::m_EventInit = { 0, 0, 0, 0 };
bool BCDS_I2Ctest::m_isCalled_cbf = false;

struct MCU_I2C_Event_S BCDS_I2Ctest::m_event_cbf = m_EventInit;
uint32_t BCDS_I2Ctest::m_I2C_cbf = 0;

//----------------------------------------------------------------------

class I2CDevice
{
public:
    I2CDevice(enum BCDS_HAL_CallMode_E CallMode);
    virtual ~I2CDevice();

    uint32_t m_I2CInterfaceParam; /**< user interface handle, simple integer */

    struct MCU_I2C_Handle_S m_I2C; /**< handle of MCU whose address is given to user interface in form UART_T m_uart */

    uint32_t getAppInterfaceHandle();
    void setDriveHandle(I2C_Handle handle);
    void setTransactionStatus(bool status);
};

I2CDevice::I2CDevice(enum BCDS_HAL_CallMode_E CallMode)
{
    m_I2C.handle = NULL;
    m_I2C.callMode = CallMode;
    m_I2C.state = MCU_I2C_STATE_INIT;
    m_I2C.readFunc = NULL;
    m_I2C.writeFunc = NULL;

    m_I2C.readRegisterFunc = NULL;
    m_I2C.writeRegisterFunc = NULL;

    m_I2C.abortFunc = NULL;

    m_I2C.appCallback = NULL;

    m_I2C.transferCallback = NULL;

    m_I2C.transaction.writeCount = 0;
    m_I2C.transaction.writeBuf = NULL;
    m_I2C.transaction.writeCount = 0;
    m_I2C.transaction.readBuf = NULL;
    m_I2C.transaction.arg = NULL;
    m_I2C.transaction.nextPtr = NULL;
    m_I2C.transaction.slaveAddress = 0;

    m_I2C.transferStatus = false;

    m_I2CInterfaceParam = (intptr_t) &m_I2C;
}

I2CDevice::~I2CDevice()
{
    ;
}

uint32_t I2CDevice::getAppInterfaceHandle()
{
    return m_I2CInterfaceParam; // ((UART_T)&m_mcuHandle);
}

void I2CDevice::setDriveHandle(I2C_Handle handle)
{
    m_I2C.handle = handle;
}

void I2CDevice::setTransactionStatus(bool status)
{
    m_I2C.transferStatus = status;
}
/**
 * @brief   Test public I2C MCU interface
 * @detail  callback signature: typedef void (*MCU_I2C_Callback_T)(struct MCU_I2C_Event_S event);
 */
void AppTestCallbackFunction(I2C_T I2C, struct MCU_I2C_Event_S event)
{
    BCDS_I2Ctest::m_I2C_cbf = (intptr_t) I2C;
    BCDS_I2Ctest::m_event_cbf = event;
    std::cout << "     --- Hello from AppTestCallbackFunction ---" << std::endl;
    std::cout << "     --- i2c: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uint32_t) I2C << std::endl;
    std::cout << "     --- event: 0x" << std::hex << std::setw(8) << std::setfill('0')
            << (uint32_t) *(uint32_t*) (struct MCU_UART_Event_S *) &event << std::endl;
    std::cout << "     --- TransferError: " << std::dec << event.TransferError << std::endl;
    std::cout << "     --- TxComplete: " << std::dec << event.TxComplete << std::endl;
    BCDS_I2Ctest::m_isCalled_cbf = true;
}

TEST_F(BCDS_I2Ctest, dummyTest)
{
    /** @testcase{ <FIXTURE_NAME>::<TEST_NAME>: }
     *
     */

    /* SETUP: Declare and initialize local variables required only by this
     * test case */

    /* EXECISE: call relevant production code Interface with appropriate test
     * inputs  */

    /* VERIFY : Compare the expected with actual */

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */

}

/*************************** I2C TESTS *****************************/

/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public MCU_I2C_Initialize
 * @detail  Retcode_T MCU_I2C_Initialize(I2C_T I2C, MCU_I2C_Callback_T callback);
 */

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Initialize_ok)
{
    Retcode_T rc;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Initialize_dopple)
{
    Retcode_T rc;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Initialize_nullI2C)
{
    Retcode_T rc;
    I2C_T I2C01 = 0;
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Initialize_nullCallback_blocking)
{
    Retcode_T rc;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Initialize_nullCallback_non_blocking)
{
    Retcode_T rc;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Initialize_wrongCallMode)
{
    Retcode_T rc;
    I2CDevice Device01((enum BCDS_HAL_CallMode_E)0);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public I2C MCU interface MCU_I2C_Deinitialize
 * @detail  Retcode_TMCU_I2C_Deinitialize(I2C_T I2C);
 * Dependent on Test public MCU_I2C_Initialize.
 */

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Deinitialize_ok)
{
    Retcode_T rc;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Deinitialize_notInitialized)
{
    Retcode_T rc;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Deinitialize_nullI2C)
{
    Retcode_T rc;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Deinitialize(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
    rc = MCU_I2C_Deinitialize(I2C01);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public I2C MCU interface MCU_I2C_Send
 * @detail  MCU_I2C_Send(I2C_T I2C, uint8_t slaveAddr, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_I2C_Initialize and MCU_I2C_Deinitialize
 */

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_ok_non_blocking)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setTransactionStatus(true);
    MCU_I2CCallback(I2C01);
    EXPECT_TRUE(m_isCalled_cbf);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_ok_blocking)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_transfer_fail)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = false;
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_notInitialized)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_nullBuffer)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(I2C01, addr, NULL, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);

}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_null_handle)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(NULL, addr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_null_Dirve_Handle)
{
    I2C_Handle I2CHandle = (I2C_Handle) NULL;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_inappropriate_length)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint32_t size = 0x0001FFFF;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_sendCancel)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Send(I2C01, addr, buffer, 0);
    Device01.setTransactionStatus(false);
    MCU_I2CCallback(I2C01);
    EXPECT_FALSE(m_isCalled_cbf);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_sencCancel_fail)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setTransactionStatus(true);
    MCU_I2CCallback(I2C01);
    EXPECT_TRUE(m_isCalled_cbf);
    rc = MCU_I2C_Send(I2C01, addr, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Send_wrongCallMode)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01((enum BCDS_HAL_CallMode_E)0);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public I2C MCU interface MCU_I2C_Receive
 * @detail  MCU_I2C_Receive(I2C_T I2C, uint8_t slaveAddr, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_I2C_Initialize and MCU_I2C_Deinitialize
 */

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_ok_non_blocking)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setTransactionStatus(true);
    MCU_I2CCallback(I2C01);
    EXPECT_TRUE(m_isCalled_cbf);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_ok_blocking)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_transfer_fail)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = false;
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_notInitialized)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_nullBuffer)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(I2C01, addr, NULL, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);

}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_null_handle)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(NULL, addr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_null_Dirve_Handle)
{
    I2C_Handle I2CHandle = (I2C_Handle) NULL;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_inappropriate_length)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint32_t size = 0x0001FFFF;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_sendCancel)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Receive(I2C01, addr, buffer, 0);
    Device01.setTransactionStatus(false);
    MCU_I2CCallback(I2C01);
    EXPECT_FALSE(m_isCalled_cbf);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_sendCancel_fail)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setTransactionStatus(true);
    MCU_I2CCallback(I2C01);
    EXPECT_TRUE(m_isCalled_cbf);
    rc = MCU_I2C_Receive(I2C01, addr, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_Receive_wrongCallMode)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01((enum BCDS_HAL_CallMode_E)0);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
}

/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public I2C MCU interface MCU_I2C_ReadRegister
 * @detail  MCU_I2C_ReadRegister(I2C_T I2C, uint8_t slaveAddr, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_I2C_Initialize and MCU_I2C_Deinitialize
 */

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_ok_non_blocking)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setTransactionStatus(true);
    MCU_I2CCallback(I2C01);
    EXPECT_TRUE(m_isCalled_cbf);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_ok_blocking)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_transfer_fail)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = false;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_notInitialized)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint8_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_nullBuffer)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, NULL, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);

}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_null_handle)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(NULL, addr, regAddr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_null_Dirve_Handle)
{
    I2C_Handle I2CHandle = (I2C_Handle) NULL;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_inappropriate_length)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint32_t size = 0x0001FFFF;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_sendCancel)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, 0);
    Device01.setTransactionStatus(false);
    MCU_I2CCallback(I2C01);
    EXPECT_FALSE(m_isCalled_cbf);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_sendCancel_fail)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setTransactionStatus(true);
    MCU_I2CCallback(I2C01);
    EXPECT_TRUE(m_isCalled_cbf);
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister_wrongCallMode)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01((enum BCDS_HAL_CallMode_E)0);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_ReadRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
}

/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public I2C MCU interface MCU_I2C_WriteRegister
 * @detail  MCU_I2C_WriteRegister(I2C_T I2C, uint8_t slaveAddr, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_I2C_Initialize and MCU_I2C_Deinitialize
 */

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_ok_non_blocking)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setTransactionStatus(true);
    MCU_I2CCallback(I2C01);
    EXPECT_TRUE(m_isCalled_cbf);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_ok_blocking)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_transfer_fail)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = false;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_notInitialized)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint8_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_nullBuffer)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, NULL, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_null_handle)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(NULL, addr, regAddr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_null_Dirve_Handle)
{
    I2C_Handle I2CHandle = (I2C_Handle) NULL;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_inappropriate_length)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint32_t size = 0x0001FFFF;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_sendCancel)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, 0);
    Device01.setTransactionStatus(false);
    MCU_I2CCallback(I2C01);
    EXPECT_FALSE(m_isCalled_cbf);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_sendCancel_fail)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T )rc);
    Device01.setTransactionStatus(true);
    MCU_I2CCallback(I2C01);
    EXPECT_TRUE(m_isCalled_cbf);
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), (Retcode_T )rc);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_WriteRegister_wrongCallMode)
{
    I2C_Handle I2CHandle = (I2C_Handle) 0xC0DE;
    uint16_t addr = 0x01;
    uint8_t regAddr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = { 0x01 };
    m_isCalled_cbf = false;
    I2CDevice Device01((enum BCDS_HAL_CallMode_E)0);
    I2C_T I2C01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
    Device01.setDriveHandle(I2CHandle);
    I2C_transfer_fake.return_val = true;
    rc = MCU_I2C_WriteRegister(I2C01, addr, regAddr, buffer, size);
    EXPECT_NE(RETCODE_OK, (Retcode_T )rc);
}
