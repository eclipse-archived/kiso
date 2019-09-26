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

/* start of global scope symbol and fake definitions section */

FFF_DEFINITION_BLOCK_START

extern "C"
{
#include "Kiso_HAL.h"

#if KISO_FEATURE_I2C
/* include faked interface */
#include "stm32l4xx_th.hh"
#include "stm32l4xx_hal_th.hh"
#include "stm32l4xx_hal_dma_th.hh"
#include "stm32l4xx_hal_i2c_th.hh"

/* Include unit under test */
#include "I2C.c"
}
/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

class KISO_I2CTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        DeInit();

        FFF_RESET_HISTORY()
    }

    virtual void TearDown()
    {
        RESET_FAKE(__HAL_I2C_GET_FLAG);
        RESET_FAKE(HAL_I2C_GetState);
        RESET_FAKE(HAL_GetTick);
        RESET_FAKE(HAL_I2C_Master_Transmit);
        RESET_FAKE(HAL_I2C_Master_Transmit_IT);
        RESET_FAKE(HAL_I2C_Master_Transmit_DMA);
        RESET_FAKE(HAL_I2C_Master_Seq_Transmit_IT);
        RESET_FAKE(HAL_I2C_Master_Receive);
        RESET_FAKE(HAL_I2C_Master_Receive_IT);
        RESET_FAKE(HAL_I2C_Master_Receive_DMA);
        RESET_FAKE(HAL_I2C_EV_IRQHandler);
        RESET_FAKE(HAL_I2C_ER_IRQHandler);
        RESET_FAKE(HAL_DMA_IRQHandler);
        RESET_FAKE(HAL_I2C_GetError);
        m_isCalled_cbf = false;
    }

    void DeInit()
    {
        m_testInstance = {};
    }

public:
    static const struct MCU_I2C_Event_S m_EventInit;
    static bool m_isCalled_cbf;
    static uint32_t m_I2C_cbf;
    static struct MCU_I2C_Event_S m_event_cbf;
    I2C_TypeDef m_testInstance = {};
    static uint32_t m_adressingMode;
};

const struct MCU_I2C_Event_S KISO_I2CTest::m_EventInit = {0, 0, 0, 0};
bool KISO_I2CTest::m_isCalled_cbf = false;

struct MCU_I2C_Event_S KISO_I2CTest::m_event_cbf = m_EventInit;
uint32_t KISO_I2CTest::m_I2C_cbf = 0;

class I2CDevice
{
public:
    I2CDevice(enum KISO_HAL_TransferMode_E TransferMode);
    virtual ~I2CDevice();

    HWHandle_T m_I2CInterfaceParam; /**< user interface handle, simple integer */
    struct MCU_I2C_S m_I2C;         /**< handle of MCU whose address is given to user interface in form UART_T m_uart */

    HWHandle_T BSP_Device_GetHandle();

    MCU_I2C_State_T getState();

    void setState(MCU_I2C_State_T state);

    void setSTM32Instance(I2C_TypeDef *instance);

    void setAddressingMode(uint32_t mode);

    void setDataRate(void);
};

I2CDevice::I2CDevice(enum KISO_HAL_TransferMode_E TransferMode)
{
    m_I2C.hi2c.Instance = NULL;
    m_I2C.hi2c.Init.AddressingMode = ~I2C_ADDRESSINGMODE_7BIT;
    m_I2C.TransferMode = TransferMode;
    m_I2C.State = I2C_STATE_INIT;
    m_I2C.DataRate = 0;
}

I2CDevice::~I2CDevice()
{
    ;
}

HWHandle_T I2CDevice::BSP_Device_GetHandle()
{
    return (HWHandle_T)&m_I2C;
}

void I2CDevice::setSTM32Instance(I2C_TypeDef *instance)
{
    m_I2C.hi2c.Instance = instance;
}

void I2CDevice::setAddressingMode(uint32_t mode)
{
    m_I2C.hi2c.Init.AddressingMode = mode;
}

void I2CDevice::setDataRate(void)
{
    m_I2C.DataRate = 100000U;
}

MCU_I2C_State_T I2CDevice::getState()
{
    return m_I2C.State;
}

void I2CDevice::setState(MCU_I2C_State_T state)
{
    m_I2C.State = state;
}

void AppTestCallbackFunction(I2C_T I2C, struct MCU_I2C_Event_S event)
{
    KISO_I2CTest::m_I2C_cbf = (intptr_t)I2C;
    KISO_I2CTest::m_event_cbf = event;
    std::cout << "     --- Hello from AppTestCallbackFunction ---" << std::endl;
    std::cout << "     --- i2c: 0x" << std::hex << std::setw(8) << std::setfill('0') << I2C << std::endl;
    std::cout << "     --- event: 0x" << std::hex << std::setw(8) << std::setfill('0')
              << (uint32_t) * (uint32_t *)(struct MCU_UART_Event_S *)&event << std::endl;
    std::cout << "     --- TransferError: " << std::dec << event.TransferError << std::endl;
    std::cout << "     --- TxComplete: " << std::dec << event.TxComplete << std::endl;
    KISO_I2CTest::m_isCalled_cbf = true;
}

bool Fake___HAL_I2C_GET_FLAG(I2C_HandleTypeDef *, uint32_t flag)
{
    switch (flag)
    {
    case I2C_FLAG_TXIS:
    case I2C_FLAG_STOPF:
    case I2C_FLAG_TCR:
        return SET;
    case I2C_FLAG_AF:
    case I2C_FLAG_BUSY:
        return RESET;
    default:
        return RESET;
    }
}

HAL_I2C_StateTypeDef Fake_HAL_I2C_GetState(I2C_HandleTypeDef *)
{
    return HAL_I2C_STATE_READY;
}

uint32_t Fake_HAL_GetTick()
{
    static uint32_t tick;
    return tick++;
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Success_Polling)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Success_Interrupt)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Success_DMA)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Fail_InvalidI2CStruct)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    rc = MCU_I2C_Initialize(NULL, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Fail_InvalidI2CHWInstance)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance((I2C_TypeDef *)0x12345678);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_MCU_I2C_INVALID_PERIPHERAL, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Fail_InvalidTransferMode)
{
    Retcode_T rc;
    I2CDevice Device01((enum KISO_HAL_TransferMode_E)0);
    Device01.setSTM32Instance(I2C2);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Fail_Interrupt_No_Callback)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C3);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Fail_DMA_No_Callback)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Initialize_Fail_DoppleInit)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());

    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Deinitialize_Null)
{
    Retcode_T rc;
    rc = MCU_I2C_Deinitialize(NULL);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
}

TEST_F(KISO_I2CTest, MCU_I2C_Deinitialize_Success)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Deinitialize_Fail_State_Receiving)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    Device01.setState(I2C_STATE_RX);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Deinitialize_Fail_State_Transmitting)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    Device01.setState(I2C_STATE_TX);
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, MCU_I2C_Deinitialize_Fail_Not_Init)
{
    Retcode_T rc;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Deinitialize(I2C01);
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_ok_polling)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_ok_Interrupt)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Transmit_IT_fake.return_val = HAL_OK;

    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_ok_DMA)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Transmit_DMA_fake.return_val = HAL_OK;

    rc = MCU_I2C_Send(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_Cancel_polling)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;

    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());

    rc = MCU_I2C_Send(I2C01, addr, buffer, 0);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    Device01.setState(I2C_STATE_TX);

    rc = MCU_I2C_Send(I2C01, addr, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_cancel_Interrupt)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setState(I2C_STATE_TX);

    rc = MCU_I2C_Send(I2C01, addr, buffer, 0);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_cancel_DMA)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setState(I2C_STATE_TX);
    rc = MCU_I2C_Send(I2C01, addr, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_Fail_InvalidSize)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;

    uint16_t addr = 0x01234;
    const uint32_t size = UINT16_MAX + 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_Send(I2C01, addr, &buffer, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_Fail_InvalidHandle)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;

    uint16_t addr = 0x01234;
    const uint32_t size = UINT16_MAX + 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_Send(NULL, addr, buffer, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_Fail_InvalidBuffer)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;

    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_Send(I2C01, addr, NULL, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_Fail_Not_Initialized)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;

    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_Send(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_Fail_State_Sending)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_I2C_Master_Transmit_IT_fake.return_val = HAL_OK;

    rc = MCU_I2C_Send(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_ok_polling_size_gt_I2C_MAX_NBYTES)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;
    uint16_t addr = 0x01234;
    const uint32_t size = 600;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);
    rc = MCU_I2C_Send(I2C01, addr, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_Interrupt_Fail_HAL_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_IT_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_Send(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Send_DMA_Fail_HAL_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_DMA_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_Send(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Polling_Success)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_fake.return_val = HAL_OK;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Interrupt_Success)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Receive_IT_fake.return_val = HAL_OK;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_DMA_Success)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Receive_DMA_fake.return_val = HAL_OK;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Cancel_polling)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());

    rc = MCU_I2C_Receive(I2C01, addr, buffer, 0);

    Device01.setState(I2C_STATE_RX);
    rc = MCU_I2C_Receive(I2C01, addr, buffer, 0);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_cancel_Interrupt)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setState(I2C_STATE_RX);
    rc = MCU_I2C_Receive(I2C01, addr, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_cancel_DMA)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setState(I2C_STATE_RX);
    rc = MCU_I2C_Receive(I2C01, addr, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Fail_InvalidHandle)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_fake.return_val = HAL_OK;

    rc = MCU_I2C_Receive(NULL, addr, buffer, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Fail_InvalidBuffer)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_fake.return_val = HAL_OK;

    rc = MCU_I2C_Receive(I2C01, addr, NULL, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Fail_InvalidLength)
{
    uint16_t addr = 0x01234;
    const uint32_t size = UINT16_MAX + 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_fake.return_val = HAL_OK;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Fail_Not_Initialized)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();

    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_fake.return_val = HAL_OK;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Fail_State_Receiveing)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Receive_DMA_fake.return_val = HAL_OK;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Polling_Fail_HAL_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_ERROR;
    HAL_I2C_Master_Receive_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_Interrupt_Fail_HAL_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_I2C_Master_Receive_IT_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_Receive_DMA_Fail_HAL_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_I2C_Master_Receive_DMA_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_Receive(I2C01, addr, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_ok_polling)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;

    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    Device01.setSTM32Instance(&m_testInstance);
    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_ok_Interrupt)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Transmit_IT_fake.return_val = HAL_OK;

    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_ok_DMA)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Transmit_DMA_fake.return_val = HAL_OK;

    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_Cancel_polling)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());

    rc = MCU_I2C_WriteRegister(I2C01, addr, 0, buffer, 0);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    Device01.setState(I2C_STATE_TX);

    rc = MCU_I2C_WriteRegister(I2C01, addr, 0, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_cancel_Interrupt)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setState(I2C_STATE_TX);

    rc = MCU_I2C_WriteRegister(I2C01, addr, 0, buffer, 0);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_cancel_DMA)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setState(I2C_STATE_TX);

    rc = MCU_I2C_WriteRegister(I2C01, addr, 0, buffer, 0);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_Fail_InvalidSize)
{
    uint16_t addr = 0x01234;
    const uint32_t size = UINT16_MAX + 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, &buffer, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_Fail_InvalidHandle)
{
    uint16_t addr = 0x01234;
    const uint32_t size = UINT16_MAX + 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_WriteRegister(NULL, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_Fail_InvalidBuffer)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, NULL, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_Fail_Not_Initialized)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_Fail_State_Sending)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    Device01.setState(I2C_STATE_TX);
    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, buffer, size);
    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_Interrupt_Fail_HAL_Failure_first_call)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Seq_Transmit_IT_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_WriteRegister_Interrupt_Fail_HAL_Failure_second_call)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_StatusTypeDef returnSequence[2] = {HAL_OK, HAL_ERROR};
    SET_RETURN_SEQ(HAL_I2C_Master_Seq_Transmit_IT, returnSequence, 2)

    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_ok_polling)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;

    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    Device01.setSTM32Instance(&m_testInstance);
    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_fake.return_val = HAL_OK;

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_ok_Interrupt)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_IT_fake.return_val = HAL_OK;

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_ok_DMA)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    Device01.setAddressingMode(I2C_ADDRESSINGMODE_7BIT);
    Device01.setDataRate();
    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_DMA_fake.return_val = HAL_OK;

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Cancel_polling_invalid_state)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());

    rc = MCU_I2C_ReadRegister(I2C01, addr, 0, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Cancel_polling)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    Device01.setState(I2C_STATE_RX);

    rc = MCU_I2C_ReadRegister(I2C01, addr, 0, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_cancel_Interrupt)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setState(I2C_STATE_RX);

    rc = MCU_I2C_ReadRegister(I2C01, addr, 0, buffer, 0);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_cancel_DMA)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setState(I2C_STATE_RX);

    rc = MCU_I2C_ReadRegister(I2C01, addr, 0, buffer, 0);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Fail_InvalidSize)
{
    uint16_t addr = 0x01234;
    const uint32_t size = UINT16_MAX + 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, &buffer, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Fail_InvalidHandle)
{
    uint16_t addr = 0x01234;
    const uint32_t size = UINT16_MAX + 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_ReadRegister(NULL, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Fail_InvalidBuffer)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, NULL, size);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Fail_Not_Initialized)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = ~RETCODE_OK;
    Device01.setSTM32Instance(&m_testInstance);

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_INIT, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Fail_State_Receiving)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_IT_fake.return_val = HAL_OK;

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_INCONSISTENT_STATE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Polling_Fail_HAL_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size] = {0x00};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_ERROR;
    HAL_I2C_Master_Receive_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_ReadRegister(I2C01, addr, 0, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Interrupt_Fail_HAL_Master_Receive_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_IT_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_Interrupt_Fail_HAL_Master_Transmit_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_ERROR;
    HAL_I2C_Master_Receive_IT_fake.return_val = HAL_OK;

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_DMA_Fail_HAL_Master_Receive_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_OK;
    HAL_I2C_Master_Receive_DMA_fake.return_val = HAL_ERROR;

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_MCU_I2C_ReadRegister_DMA_Fail_HAL_Master_Transmit_Failure)
{
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_I2C_Master_Transmit_fake.return_val = HAL_ERROR;
    HAL_I2C_Master_Receive_DMA_fake.return_val = HAL_OK;

    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
    EXPECT_EQ(I2C_STATE_READY, Device01.getState());
}

TEST_F(KISO_I2CTest, test_I2C_EventCallback)
{
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    I2C_EventCallback(I2C01);
    EXPECT_EQ(HAL_I2C_EV_IRQHandler_fake.call_count, 1U);
}

TEST_F(KISO_I2CTest, test_I2C_ErrorCallback)
{
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    I2C_ErrorCallback(I2C01);
    EXPECT_EQ(HAL_I2C_ER_IRQHandler_fake.call_count, 1U);
}

TEST_F(KISO_I2CTest, test_I2C_DmaRxHandler)
{
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    I2C_DmaRxHandler(I2C01);
    EXPECT_EQ(HAL_DMA_IRQHandler_fake.call_count, 1U);
}

TEST_F(KISO_I2CTest, test_I2C_DmaTxHandler)
{
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    I2C_DmaTxHandler(I2C01);
    EXPECT_EQ(HAL_DMA_IRQHandler_fake.call_count, 1U);
}

TEST_F(KISO_I2CTest, test_HAL_I2C_ErrorCallback)
{
    Retcode_T rc = RETCODE_OK;
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_I2C_GetError_fake.return_val = ~HAL_I2C_ERROR_NONE;

    HAL_I2C_ErrorCallback((I2C_HandleTypeDef *)I2C01);

    EXPECT_EQ(m_event_cbf.TransferError, 1U);
}

TEST_F(KISO_I2CTest, test_HAL_I2C_MasterTxCpltCallback_PrependRegAddr_true)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_I2C_Master_Transmit_IT_fake.return_val = HAL_OK;
    rc = MCU_I2C_WriteRegister(I2C01, addr, reg, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_TX, Device01.getState());

    HAL_I2C_MasterTxCpltCallback((I2C_HandleTypeDef *)I2C01);
    //first callback will only reset register writing flag no application callback should trigger
    EXPECT_EQ(m_isCalled_cbf, false);
    HAL_I2C_MasterTxCpltCallback((I2C_HandleTypeDef *)I2C01);
    //second callback will only call application
    EXPECT_EQ(m_isCalled_cbf, true);
    EXPECT_EQ(m_event_cbf.TxComplete, 1U);
}

TEST_F(KISO_I2CTest, test_HAL_I2C_MasterRxCpltCallback)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    HAL_I2C_GetState_fake.custom_fake = Fake_HAL_I2C_GetState;
    HAL_GetTick_fake.custom_fake = Fake_HAL_GetTick;
    uint16_t addr = 0x01234;
    const uint32_t size = 1;
    Retcode_T rc = RETCODE_OK;
    uint8_t reg = 0;
    uint8_t buffer[size] = {0x01};
    I2CDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.setSTM32Instance(I2C1);
    I2C_T I2C01 = (I2C_T)Device01.BSP_Device_GetHandle();
    rc = MCU_I2C_Initialize(I2C01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_I2C_Master_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_I2C_ReadRegister(I2C01, addr, reg, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(I2C_STATE_RX, Device01.getState());

    HAL_I2C_MasterRxCpltCallback((I2C_HandleTypeDef *)I2C01);
    EXPECT_EQ(m_isCalled_cbf, true);
    EXPECT_EQ(m_event_cbf.RxComplete, 1U);
}

#endif
