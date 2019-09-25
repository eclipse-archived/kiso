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

/* include gtest interface */
#include <gtest.h>

/* include faked interfaces */
extern "C"
{
#include "em_i2c_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_HAL_th.hh"

    /**
 * Kiso_HALConfig.h file which breaks the unit test implementation.
 *
 * Below four lines hot-fix needs to be replaced by proper implementation.
 */

#if KISO_FEATURE_I2C

/* include module under test */
#include "I2C.c"
}
/* end of global scope symbol and fake definitions section */

class KISO_I2Ctest : public testing::Test
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
    static uint32_t m_i2c_cbf;
    static struct MCU_I2C_Event_S m_event_cbf;
};

const struct MCU_I2C_Event_S KISO_I2Ctest::m_EventInit = {0, 0, 0, 0};
bool KISO_I2Ctest::m_isCalled_cbf = false;

struct MCU_I2C_Event_S KISO_I2Ctest::m_event_cbf = m_EventInit;
uint32_t KISO_I2Ctest::m_i2c_cbf = 0;

//----------------------------------------------------------------------

class I2CDevice
{
public:
    I2CDevice(enum KISO_HAL_TransferMode_E TransferMode);
    virtual ~I2CDevice();

    uint32_t m_I2CInterfaceParam; /**< user interface handle, simple integer */

    struct MCU_I2C_Handle_S m_I2C; /**< handle of MCU whose address is given to user interface in form UART_T m_uart */

    I2C_TypeDef m_Instance = {0};

    uint32_t getAppInterfaceHandle();
};

I2CDevice::I2CDevice(enum KISO_HAL_TransferMode_E TransferMode)
{
    m_I2C.TransferMode = TransferMode;

    m_I2C.instance = I2C_TEST;
    m_I2C._DriverCtx.Initialized = false;

    m_I2C.IRQCallback = MCU_I2C_IRQ_Callback;
    m_I2C.DmaRxCallback = NULL;
    m_I2C.DmaTxCallback = NULL;

    m_I2CInterfaceParam = (intptr_t)&m_I2C;
}

I2CDevice::~I2CDevice()
{
    ;
}

uint32_t I2CDevice::getAppInterfaceHandle()
{
    return m_I2CInterfaceParam; // ((UART_T)&m_mcuHandle);
}

/**
 * @brief   Test public I2C MCU interface
 * @detail  callback signature: typedef void (*MCU_I2C_Callback_T)(struct MCU_I2C_Event_S event);
 */
void AppTestCallbackFunction(I2C_T i2c, struct MCU_I2C_Event_S event)
{
    KISO_I2Ctest::m_i2c_cbf = (intptr_t)i2c;
    KISO_I2Ctest::m_event_cbf = event;
    /*
     std::cout << "     --- Hello from AppTestCallbackFunction ---" << std::endl;
     std::cout << "     --- uart: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uint32_t) uart << std::endl;
     std::cout << "     --- event: 0x" << std::hex << std::setw(8) << std::setfill('0')
     << (uint32_t) *(uint32_t*) (struct MCU_UART_Event_S *) &event << std::endl;
     std::cout << "     --- TransferError: " << std::dec << event.TransferError << std::endl;
     std::cout << "     --- TransferComplete: " << std::dec << event.TransferComplete << std::endl;
     std::cout << "     --- Cts: " << std::dec << event.Cts << std::endl;
     std::cout << "     --- Dsr: " << std::dec << event.Dsr << std::endl;
     std::cout << "     --- Dcd: " << std::dec << event.Dcd << std::endl;
     std::cout << "     --- Ri: " << std::dec << event.Ri << std::endl;
     */
    KISO_I2Ctest::m_isCalled_cbf = true;
}

/*************************** I2C TESTS *****************************/
/**
 * @brief   Test public MCU_I2C_Initialize
 * @detail  Retcode_T MCU_I2C_Initialize(I2C_T i2c, MCU_I2C_Callback_T callback);
 */

TEST_F(KISO_I2Ctest, test_MCU_I2C_Initialize_ok)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Initialize_nullI2C)
{
    Retcode_T rc;
    I2C_T i2c01 = 0;
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Initialize_nullCallbackandI2C)
{
    Retcode_T rc;
    I2C_T i2c01 = 0;
    rc = MCU_I2C_Initialize(i2c01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Initialize_nullCallback)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Initialize_wrongTransferMode)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_BLOCKING);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

/**
 * @brief   Test public I2C MCU interface MCU_I2C_Deinitialize
 * @detail  Retcode_TMCU_I2C_Deinitialize(I2C_T i2c);
 * Dependent on Test public MCU_I2C_Initialize.
 */

TEST_F(KISO_I2Ctest, test_MCU_I2C_Deinitialize_ok)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Deinitialize_notInitialized)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_UNINITIALIZED), rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Deinitialize_nullI2C)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Deinitialize(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
    rc = MCU_I2C_Deinitialize(i2c01);
}

/**
 * @brief   Test public I2C MCU interface MCU_I2C_Send
 * @detail  MCU_I2C_Send(I2C_T i2c, uint8_t slaveAddr, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_I2C_Initialize and MCU_I2C_Deinitialize
 */

TEST_F(KISO_I2Ctest, test_MCU_I2C_Send_ok)
{
    uint16_t addr = 0x01;
    const uint16_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, (Retcode_T)rc);
    I2C_TransferInit_fake.return_val = i2cTransferInProgress;
    rc = MCU_I2C_Send(i2c01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T)rc);

    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Send_notInitialized)
{
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Send(i2c01, addr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Send_nullBuffer)
{
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Send(i2c01, addr, NULL, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Send_sendCancel)
{
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Send(i2c01, addr, buffer, 0); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Send_wrongTransferMode)
{
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_BLOCKING);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    rc = MCU_I2C_Send(i2c01, addr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), rc);
}

/**
 * @brief   Test public I2C MCU interface MCU_I2C_Receive
 * @detail  Retcode_T MCU_I2C_Receive(I2C_T i2c, uint8_t slaveAddr, uint8_t * buffer, uint32_t len);
 * Dependent on Test public MCU_I2C_Initialize and MCU_I2C_Deinitialize
 */

TEST_F(KISO_I2Ctest, test_MCU_I2C_Receive_ok)
{
    uint8_t addr = 0x01;
    const uint32_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    I2C_TransferInit_fake.return_val = i2cTransferInProgress;
    rc = MCU_I2C_Receive(i2c01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Receive_nullBuffer)
{
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Receive(i2c01, addr, 0, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Receive_receiveCancel)
{
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Receive(i2c01, addr, buffer, 0); /* receive cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Receive_notInitialized)
{
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Receive(i2c01, addr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_Receive_wrongTransferMode)
{
    uint8_t addr = 0x01;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_BLOCKING);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    rc = MCU_I2C_Receive(i2c01, addr, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), rc);
}

/**
 * @brief   Test public I2C MCU interface MCU_I2C_ReadRegister
 * @detail   Retcode_T MCU_I2C_ReadRegister(I2C_T i2c, uint8_t slaveAddr, uint8_t registerAddr, uint8_t * rxBuffer, uint32_t rxLen);
 * Dependent on Test public MCU_I2C_Initialize and MCU_I2C_Deinitialize
 */

TEST_F(KISO_I2Ctest, test_MCU_I2C_ReadRegister_ok)
{
    uint8_t addr = 0x01;
    uint16_t addr2 = 0x02;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x10};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    I2C_TransferInit_fake.return_val = i2cTransferInProgress;
    rc = MCU_I2C_ReadRegister(i2c01, addr, addr2, buffer, size);
    EXPECT_EQ(RETCODE_OK, (Retcode_T)rc);
    I2C_TransferInit_fake.return_val = i2cTransferDone;
    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_ReadRegister_nullBuff)
{
    uint8_t addr = 0x01;
    uint16_t addr2 = 0x02;
    const uint32_t size = 10;
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_ReadRegister(i2c01, addr, addr2, 0, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_ReadRegister_nullLength)
{
    uint8_t addr = 0x01;
    uint16_t addr2 = 0x02;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_ReadRegister(i2c01, addr, addr2, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_ReadRegister_notInitialized)
{
    uint8_t addr = 0x01;
    uint16_t addr2 = 0x01;
    const uint32_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_ReadRegister(i2c01, addr, addr2, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), rc);
}

/**
 * @brief   Test public I2C MCU interface MCU_I2C_ReadRegister
 * @detail   Retcode_T MCU_I2C_WriteRegister(I2C_T i2c, uint8_t slaveAddr,uint8_t registerAddr, uint8_t * txdata, uint32_t txLen);
 * Dependent on Test public MCU_I2C_Initialize and MCU_I2C_Deinitialize
 */

TEST_F(KISO_I2Ctest, test_MCU_I2C_WriteRegister_ok)
{
    uint8_t addr = 0x01;
    uint16_t addr2 = 0x01;
    const uint32_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    I2C_TransferInit_fake.return_val = i2cTransferInProgress;
    rc = MCU_I2C_WriteRegister(i2c01, addr, addr2, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);

    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_WriteRegister_nullBuff)
{
    uint8_t addr = 0x01;
    uint8_t addr2 = 0x01;
    const uint32_t size = 10;
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_WriteRegister(i2c01, addr, addr2, 0, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(KISO_I2Ctest, test_MCU_I2C_WriteRegister_nullLength)
{
    uint8_t addr = 0x01;
    uint8_t addr2 = 0x01;
    const uint32_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    I2C_T i2c01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_I2C_Initialize(i2c01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_I2C_WriteRegister(i2c01, addr, addr2, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_I2C_Deinitialize(i2c01);
    EXPECT_EQ(RETCODE_OK, rc);
}

#else
}
#endif // KISO_FEATURE_I2C
