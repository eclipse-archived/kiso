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
 * @brief
 * Unittest for MCU STM32L4 SPI
 *
 */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START
extern "C"
{

/* setup compile time configuration defines */
/* include faked interfaces */
#include "Kiso_HAL_th.hh"

#if KISO_FEATURE_SPI

#include "stm32l4xx_th.hh"
#include "stm32l4xx_hal_th.hh"
#include "stm32l4xx_hal_spi_th.hh"
#include "stm32l4xx_hal_dma_th.hh"

/* include module under test */
#include "SPI.c"

} /* extern "C" */

/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END
SPI_T global_spi;
struct MCU_SPI_Event_S global_event;
bool global_isCalled_cbf = false;
class STM32L4_SPI_Test : public testing::Test
{
public:
    STM32L4_SPI_Test();

protected:
    void SetUp()
    {
        RESET_FAKE(HAL_SPI_IRQHandler);
        RESET_FAKE(HAL_DMA_IRQHandler);
        RESET_FAKE(HAL_SPI_Transmit);
        RESET_FAKE(HAL_SPI_Transmit_IT);
        RESET_FAKE(HAL_SPI_Transmit_DMA);
        RESET_FAKE(HAL_SPI_Receive);
        RESET_FAKE(HAL_SPI_Receive_IT);
        RESET_FAKE(HAL_SPI_Receive_DMA);
        RESET_FAKE(HAL_SPI_Abort);
        RESET_FAKE(HAL_SPI_GetError);
        FFF_RESET_HISTORY();
        global_isCalled_cbf = false;
        global_event = {0};
        global_spi = {0};
    }

    /* TearDown() is invoked immediately after a test finishes. */
    void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

STM32L4_SPI_Test::STM32L4_SPI_Test()
{
    ;
}

//----------------------------------------------------------------------

class SpiDevice
{
public:
    SpiDevice(enum KISO_HAL_TransferMode_E TransferMode);
    ~SpiDevice();

    void *m_SpiInterfaceParam; /**< user interface handle, simple integer */

    struct MCU_SPI_S m_Spi; /**< handle of MUC which address is given to user interface in form SPI_T m_spi */

    SPI_TypeDef m_Instance;
    DMA_HandleTypeDef m_hdmatx;
    DMA_HandleTypeDef m_hdmarx;

    void *getAppInterfaceHandle();
};

SpiDevice::SpiDevice(enum KISO_HAL_TransferMode_E TransferMode)
{
    m_Spi.TransferMode = TransferMode;

    m_Spi.hspi.Instance = &m_Instance;

    // m_Spi.RxUserSize = 0;
    m_Spi.State = SPI_STATE_INIT;
    m_Spi.AppCallback = NULL;
    m_Spi.IRQCallback = NULL;

    m_Spi.hspi.hdmatx = NULL;
    m_Spi.hspi.hdmarx = NULL;
    m_Spi.DmaRxCallback = NULL;
    m_Spi.DmaTxCallback = NULL;

    m_SpiInterfaceParam = (void *)&m_Spi;
}

SpiDevice::~SpiDevice()
{
    ;
}

void *SpiDevice::getAppInterfaceHandle()
{
    return m_SpiInterfaceParam;
}

static void SPI_Callback(SPI_T spi, struct MCU_SPI_Event_S event)
{
    global_spi = spi;
    global_event = event;
    global_isCalled_cbf = true;
}
/* specify test cases ******************************************************* */

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_Invalid_Handle)
{
    Retcode_T rc;
    rc = MCU_SPI_Initialize(NULL, (MCU_SPI_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_InvalidInstance)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);

    Device01.m_Spi.hspi.Instance = (SPI_TypeDef *)NULL;
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();

    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SPI_INVALID_PERIPHERAL), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_InvalidState)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);

    Device01.m_Spi.State = SPI_STATE_READY;
    Device01.m_Spi.hspi.Instance = SPI1;
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();

    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    Device01.m_Spi.State = SPI_STATE_TX;

    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    Device01.m_Spi.State = SPI_STATE_RX;

    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_InvalidCallback_int)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.m_Spi.hspi.Instance = SPI1;
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_InvalidCallback_dma)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_InvalidMode)
{
    Retcode_T rc;
    SpiDevice Device01((enum KISO_HAL_TransferMode_E)5);
    Device01.m_Spi.hspi.Instance = SPI1;
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_Polling_Ok)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_Int_Ok)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI2;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Initialize_DMA_Ok)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    Device01.m_Spi.hspi.Instance = SPI3;
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Deinitialize_Invalid_Handle)
{
    Retcode_T rc;
    rc = MCU_SPI_Deinitialize((SPI_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Deinitialize_OK)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Deinitialize_Invalid_State)
{
    Retcode_T rc;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();

    Device01.m_Spi.State = SPI_STATE_TX;

    rc = MCU_SPI_Deinitialize(spi01);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    Device01.m_Spi.State = SPI_STATE_INIT;

    rc = MCU_SPI_Deinitialize(spi01);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    Device01.m_Spi.State = SPI_STATE_RX;

    rc = MCU_SPI_Deinitialize(spi01);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Transfer_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer_in[size];
    uint8_t buffer_out[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);

    EXPECT_EQ(RETCODE_OK, rc);

    rc = MCU_SPI_Transfer((SPI_T)NULL, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_SPI_Transfer(spi01, (uint8_t *)NULL, buffer_in, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_SPI_Transfer(spi01, buffer_out, (uint8_t *)NULL, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, UINT16_MAX + 1);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);

    Device01.m_Spi.State = SPI_STATE_TX_RX;

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Transfer_Polling_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer_in[size];
    uint8_t buffer_out[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);

    HAL_SPI_TransmitReceive_fake.return_val = HAL_ERROR;

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Transfer_Int_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer_in[size];
    uint8_t buffer_out[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);

    HAL_SPI_TransmitReceive_IT_fake.return_val = HAL_ERROR;

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Transfer_DMA_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer_in[size];
    uint8_t buffer_out[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);

    HAL_SPI_TransmitReceive_DMA_fake.return_val = HAL_ERROR;

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Transfer_Polling_OK)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer_in[size];
    uint8_t buffer_out[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_TransmitReceive_fake.return_val = HAL_OK;

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Transfer_Int_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer_in[size];
    uint8_t buffer_out[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_TransmitReceive_IT_fake.return_val = HAL_OK;

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_TX_RX);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Transfer_And_Abort_Int_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer_in[size];
    uint8_t buffer_out[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_TransmitReceive_IT_fake.return_val = HAL_OK;

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_TX_RX);

    // Abort transfering by transfering zero
    HAL_SPI_Abort_fake.return_val = HAL_OK;
    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Transfer_And_Abort_DMA_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer_in[size];
    uint8_t buffer_out[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_TransmitReceive_DMA_fake.return_val = HAL_OK;

    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_TX_RX);

    // Abort transfering by transfering zero
    HAL_SPI_Abort_fake.return_val = HAL_OK;
    rc = MCU_SPI_Transfer(spi01, buffer_out, buffer_in, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Send_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);

    EXPECT_EQ(RETCODE_OK, rc);

    rc = MCU_SPI_Send((SPI_T)NULL, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_SPI_Send(spi01, (uint8_t *)NULL, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_SPI_Send(spi01, buffer, UINT16_MAX + 1);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);

    Device01.m_Spi.State = SPI_STATE_TX;

    rc = MCU_SPI_Send(spi01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Send_Polling_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);

    HAL_SPI_Transmit_fake.return_val = HAL_ERROR;

    rc = MCU_SPI_Send(spi01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Send_Int_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);

    HAL_SPI_Transmit_IT_fake.return_val = HAL_ERROR;

    rc = MCU_SPI_Send(spi01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Send_DMA_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);

    HAL_SPI_Transmit_DMA_fake.return_val = HAL_ERROR;

    rc = MCU_SPI_Send(spi01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Send_Polling_OK)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Transmit_fake.return_val = HAL_OK;

    rc = MCU_SPI_Send(spi01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Send_Int_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Transmit_IT_fake.return_val = HAL_OK;

    rc = MCU_SPI_Send(spi01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_TX);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Send_And_Abort_Int_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Transmit_IT_fake.return_val = HAL_OK;

    rc = MCU_SPI_Send(spi01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_TX);

    // Abort sending by sending zero
    HAL_SPI_Abort_fake.return_val = HAL_OK;
    rc = MCU_SPI_Send(spi01, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Send_And_Abort_DMA_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Transmit_DMA_fake.return_val = HAL_OK;

    rc = MCU_SPI_Send(spi01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_TX);

    // Abort sending by sending zero
    HAL_SPI_Abort_fake.return_val = HAL_OK;
    rc = MCU_SPI_Send(spi01, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Receive_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);

    rc = MCU_SPI_Receive((SPI_T)NULL, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_SPI_Receive(spi01, (uint8_t *)NULL, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_SPI_Receive(spi01, buffer, UINT16_MAX + 1);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Receive_Polling_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, (MCU_SPI_Callback_T)NULL);

    HAL_SPI_Receive_fake.return_val = HAL_BUSY;

    rc = MCU_SPI_Receive(spi01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Receive_Int_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);

    HAL_SPI_Receive_IT_fake.return_val = HAL_BUSY;

    rc = MCU_SPI_Receive(spi01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Receive_DMA_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);

    HAL_SPI_Receive_DMA_fake.return_val = HAL_BUSY;

    rc = MCU_SPI_Receive(spi01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Receive_Polling_OK)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Receive_And_Abort_Int_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_IT_fake.return_val = HAL_OK;

    rc = MCU_SPI_Receive(spi01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_RX);

    // Abort receiving by sending zero
    HAL_SPI_Abort_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_Receive_And_Cancel_DMA_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_DMA_fake.return_val = HAL_OK;

    rc = MCU_SPI_Receive(spi01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_RX);

    // Abort receiving by sending zero
    HAL_SPI_Abort_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Spi.State, SPI_STATE_READY);
}

TEST_F(STM32L4_SPI_Test, test_MCU_SPI_GetDataCount)
{
    uint32_t retval = 0;
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    Device01.m_Spi.hspi.TxXferCount = 100U;
    Device01.m_Spi.hspi.RxXferCount = 200U;
    Device01.m_Spi.State = SPI_STATE_INIT;

    retval = MCU_SPI_GetDataCount(spi01);

    EXPECT_EQ(retval, 0U);

    Device01.m_Spi.State = SPI_STATE_TX;

    retval = MCU_SPI_GetDataCount(spi01);

    EXPECT_EQ(retval, 100U);

    Device01.m_Spi.State = SPI_STATE_RX;

    retval = MCU_SPI_GetDataCount(spi01);
    EXPECT_EQ(retval, 200U);

    Device01.m_Spi.State = SPI_STATE_TX_RX;

    retval = MCU_SPI_GetDataCount(spi01);
    EXPECT_EQ(retval, 200U);
}

TEST_F(STM32L4_SPI_Test, test_SPI_IRQHandler)
{
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    SPI_IRQHandler(spi01);
    EXPECT_EQ(HAL_SPI_IRQHandler_fake.call_count, 1U);
}

TEST_F(STM32L4_SPI_Test, test_SPI_DMARxHandler)
{
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    SPI_DMARxHandler(spi01);
    EXPECT_EQ(HAL_DMA_IRQHandler_fake.call_count, 1U);
}

TEST_F(STM32L4_SPI_Test, test_SPI_DMATxHandler)
{
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    SPI_DMATxHandler(spi01);
    EXPECT_EQ(HAL_DMA_IRQHandler_fake.call_count, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_TxCpltCallback_ok)
{
    MCU_SPI_S spi;
    spi.AppCallback = SPI_Callback;
    spi.State = SPI_STATE_TX;

    HAL_SPI_TxCpltCallback(&spi.hspi);
    EXPECT_TRUE(global_isCalled_cbf);
    EXPECT_EQ(global_event.TxComplete, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_TxCpltCallback_wrongSpi)
{
    HAL_SPI_TxCpltCallback((SPI_HandleTypeDef *)NULL);
    EXPECT_FALSE(global_isCalled_cbf);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_RxCpltCallback_ok)
{
    MCU_SPI_S spi;
    spi.AppCallback = SPI_Callback;
    spi.State = SPI_STATE_RX;

    HAL_SPI_RxCpltCallback(&spi.hspi);
    EXPECT_TRUE(global_isCalled_cbf);
    EXPECT_EQ(global_event.RxComplete, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_RxCpltCallback_wrongSpi)
{
    HAL_SPI_RxCpltCallback((SPI_HandleTypeDef *)NULL);
    EXPECT_FALSE(global_isCalled_cbf);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_TxRxCpltCallback_ok)
{
    MCU_SPI_S spi;
    spi.AppCallback = SPI_Callback;
    spi.State = SPI_STATE_TX_RX;

    HAL_SPI_TxRxCpltCallback(&spi.hspi);
    EXPECT_TRUE(global_isCalled_cbf);
    EXPECT_EQ(global_event.RxComplete, 1U);
    EXPECT_EQ(global_event.TxComplete, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_TxRxCpltCallback_wrongSpi)
{
    HAL_SPI_TxRxCpltCallback((SPI_HandleTypeDef *)NULL);
    EXPECT_FALSE(global_isCalled_cbf);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_ErrorCallback_noError)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, size);

    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_NONE;

    HAL_SPI_ErrorCallback((SPI_HandleTypeDef *)spi01);

    EXPECT_EQ(0U, global_event.RxError);
    EXPECT_EQ(0U, global_event.TxError);
    EXPECT_EQ(0U, global_event.DataLoss);
    EXPECT_TRUE(global_isCalled_cbf);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_ErrorCallback_HAL_SPI_ERROR_MODF)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_MODF;

    HAL_SPI_ErrorCallback((SPI_HandleTypeDef *)spi01);

    EXPECT_EQ(global_event.DataLoss, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_ErrorCallback_HAL_SPI_ERROR_CRC)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_CRC;

    HAL_SPI_ErrorCallback((SPI_HandleTypeDef *)spi01);

    EXPECT_EQ(global_event.DataLoss, 1U);
    EXPECT_EQ(global_event.RxError, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_ErrorCallback_HAL_SPI_ERROR_OVR)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_OVR;

    HAL_SPI_ErrorCallback((SPI_HandleTypeDef *)spi01);

    EXPECT_EQ(global_event.DataLoss, 1U);
    EXPECT_EQ(global_event.RxError, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_ErrorCallback_HAL_SPI_ERROR_FRE)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_FRE;

    HAL_SPI_ErrorCallback((SPI_HandleTypeDef *)spi01);

    EXPECT_EQ(global_event.DataLoss, 1U);
    EXPECT_EQ(global_event.RxError, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_ErrorCallback_HAL_SPI_ERROR_DMA)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_DMA;

    HAL_SPI_ErrorCallback((SPI_HandleTypeDef *)spi01);

    EXPECT_EQ(global_event.DataLoss, 1U);
    EXPECT_EQ(global_event.RxError, 1U);
    EXPECT_EQ(global_event.TxError, 1U);
}

TEST_F(STM32L4_SPI_Test, test_HAL_SPI_ErrorCallback_HAL_SPI_ERROR_FLAG)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Spi.hspi.Instance = SPI1;
    rc = MCU_SPI_Initialize(spi01, SPI_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_SPI_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);

    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_FLAG;

    HAL_SPI_ErrorCallback((SPI_HandleTypeDef *)spi01);

    EXPECT_EQ(global_event.DataLoss, 1U);
    EXPECT_EQ(global_event.RxError, 1U);
    EXPECT_EQ(global_event.TxError, 1U);
}

#endif
