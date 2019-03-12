/******************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  Module test specification for the SPI_unittest module.
 *
 * The unit test file template follows the Four-Phase test pattern.
 *
 * ****************************************************************************/

/* include gtest interface */
#include "gtest.h"

FFF_DEFINITION_BLOCK_START

/* include faked interface */
extern "C"
{
#include "BCDS_Basics.h"
#include "stm32l4xx_th.hh"
#include "stm32l4xx_hal_dma_th.hh"
#include "stm32l4xx_hal_i2c_th.hh"
#include "BCDS_HAL_CriticalSection_th.hh"

#undef BCDS_FEATURE_I2C
#define BCDS_FEATURE_I2C 1

/* Include unit under test */
#include "I2C.c"

} /* extern "C"*/

FFF_DEFINITION_BLOCK_END

// Macro is used to manipulate the Status values of the fake stm32 handle
#define I2C_FLAG_MASK  ((uint32_t)0x0001FFFF)
#define __HAL_I2C_SET_FLAG(__HANDLE__, __FLAG__)                ((((__HANDLE__)->Instance->ISR) = ((__FLAG__) & I2C_FLAG_MASK)))

/* This class is used as context for the unit test. This means that an
 * instance of this class is created at the beginning of the test and the
 * instance lives during all tests that are running. It also provides
 * the callback that is passed to the I2C driver in order to be notified by
 * the driver upon any event.
 */
class I2CTestContext
{
public:
    // Constructor, ties the fake STM Handler to the BSP Handler
    I2CTestContext()
        : mTestAppCallbackCount(0)
    {
        memset(&mLastEvent,0,sizeof(mLastEvent));
        mSTMHandleI2C.Instance = &mI2C;
        mBSPHandleI2C.hi2c.Instance = &mI2C;
        // Configure DMA mode by default
        mBSPHandleI2C.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;
        I2CTestContext::TestContext = this;
    }
    // A static pointer that will be set upon setup to be able to access
    // the test instance from within static functions
    static I2CTestContext* TestContext;

    // Declare a static function used as application callback for I2C
    static void TestAppCallback(I2C_T i2c, struct MCU_I2C_Event_S event);

    // Counter to count the number of callbacks from the driver
    uint32_t mTestAppCallbackCount;
    // the fake I2C instance used by the STM32 handler to simulate registers
    I2C_TypeDef mI2C;
    // the fake handler that is used to call STM32 HAL functions
    I2C_HandleTypeDef mSTMHandleI2C;
    // the fake handler that is normally created by the BSP.
    struct MCU_I2C_S mBSPHandleI2C;
    // An event structure into which the last event from the driver is stored
    struct MCU_I2C_Event_S mLastEvent;

};

// static member variable
I2CTestContext* I2CTestContext::TestContext = NULL;
// static member function
void I2CTestContext::TestAppCallback(I2C_T i2c, struct MCU_I2C_Event_S event)
{
    if (TestContext)
    {
        UNUSED(i2c);
        TestContext->mLastEvent = event;
        TestContext->mTestAppCallbackCount++;
    }
}

uint32_t Fake_HAL_I2C_GetError(I2C_HandleTypeDef *hi2c)
{
  return hi2c->ErrorCode;
}

bool Fake___HAL_I2C_GET_FLAG(I2C_HandleTypeDef* hi2c, uint32_t flag)
{
    return (((((hi2c)->Instance->ISR) & (flag)) == (flag)) ? SET : RESET);
}

// Create an instance for the test context
I2CTestContext testContext;

class BCDS_I2Ctest: public testing::Test
{
public:
    protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }

    virtual void TearDown()
    {
        RESET_FAKE(HAL_I2C_Init);
        RESET_FAKE(HAL_I2C_GetState);
        RESET_FAKE(HAL_I2C_DeInit);
        RESET_FAKE(HAL_I2C_Master_Transmit_IT);
        RESET_FAKE(HAL_I2C_Master_Receive_IT);
        RESET_FAKE(HAL_I2C_Slave_Transmit_IT);
        RESET_FAKE(HAL_I2C_Slave_Receive_IT);
        RESET_FAKE(HAL_I2C_GetError);
        RESET_FAKE(__HAL_I2C_GET_FLAG);
        RESET_FAKE(HAL_I2C_EV_IRQHandler);
        RESET_FAKE(HAL_I2C_ER_IRQHandler);

    }
};

TEST_F(BCDS_I2Ctest, testI2C_initialize)
{
    /* testing I2C_initialize function */

    /* call initialize function with missing handler  */
    Retcode_T rc = MCU_I2C_Initialize(0, I2CTestContext::TestAppCallback);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM),rc);

    /* call initialize function with missing callback function  */
    rc = MCU_I2C_Initialize((I2C_T)&testContext.mBSPHandleI2C, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM),rc);

    /* call initialize function with correct parameters but unsupported transfer modes */
    testContext.mBSPHandleI2C.TransferMode = BCDS_HAL_TRANSFER_MODE_BLOCKING;
    rc = MCU_I2C_Initialize((I2C_T)&testContext.mBSPHandleI2C, &I2CTestContext::TestAppCallback);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED),rc);

    testContext.mBSPHandleI2C.TransferMode = BCDS_HAL_TRANSFER_MODE_DMA;
    rc = MCU_I2C_Initialize((I2C_T)&testContext.mBSPHandleI2C, &I2CTestContext::TestAppCallback);
    EXPECT_EQ(RETCODE_OK,rc);

    rc = MCU_I2C_Deinitialize((I2C_T)&testContext.mBSPHandleI2C);
    EXPECT_EQ(RETCODE_OK, rc);

    testContext.mBSPHandleI2C.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;
    /* call initialize function with correct parameters  */
    rc = MCU_I2C_Initialize((I2C_T)&testContext.mBSPHandleI2C, &I2CTestContext::TestAppCallback);
    EXPECT_EQ(RETCODE_OK,rc);

}


TEST_F(BCDS_I2Ctest, testI2C_Send)
{
    uint8_t addr = 0x01;
    uint32_t len = 1024;
    uint8_t buffer[len];
    // First try with wrong handle */
    Retcode_T rc = MCU_I2C_Send(0,addr,buffer,len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM),rc);
    // Then use a valid handle, but invalid buffer
    rc = MCU_I2C_Send((I2C_T)&testContext.mBSPHandleI2C,addr,NULL,len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM),rc);
    // Then use a valid handle, valid buffer but invalid len
    rc = MCU_I2C_Send((I2C_T)&testContext.mBSPHandleI2C,addr,buffer,UINT16_MAX+1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED),rc);
    // Set fake return value for
    switch (testContext.mBSPHandleI2C.TransferMode)
    {
    case BCDS_HAL_TRANSFER_MODE_BLOCKING:
        break;
    case BCDS_HAL_TRANSFER_MODE_DMA:
        HAL_I2C_Master_Transmit_DMA_fake.return_val = HAL_OK;
        break;
    case BCDS_HAL_TRANSFER_MODE_INTERRUPT:
        HAL_I2C_Master_Transmit_IT_fake.return_val = HAL_OK;
        break;
    default:
        break;
    }
    // Then us correct parameters
    rc = MCU_I2C_Send((I2C_T)&testContext.mBSPHandleI2C,addr,buffer,256);
    EXPECT_EQ(RETCODE_OK,rc);
    // Set fake return value for
    switch (testContext.mBSPHandleI2C.TransferMode)
    {
    case BCDS_HAL_TRANSFER_MODE_BLOCKING:
        break;
    case BCDS_HAL_TRANSFER_MODE_DMA:
        HAL_I2C_Master_Transmit_DMA_fake.return_val = HAL_ERROR;
        break;
    case BCDS_HAL_TRANSFER_MODE_INTERRUPT:
        HAL_I2C_Master_Transmit_IT_fake.return_val = HAL_ERROR;
        break;
    default:
        break;
    }
    rc = MCU_I2C_Send((I2C_T)&testContext.mBSPHandleI2C,addr,buffer,256);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE),rc);
    // Try to cancel an active transfer, however we have not implemented this
    testContext.mBSPHandleI2C.hi2c.State = HAL_I2C_STATE_BUSY;
    rc = MCU_I2C_Send((I2C_T)&testContext.mBSPHandleI2C,addr,NULL,0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED),rc);
    testContext.mBSPHandleI2C.hi2c.State = HAL_I2C_STATE_RESET;
}

TEST_F(BCDS_I2Ctest, testI2C_Receive)
{
    uint8_t addr = 0x01;
    uint32_t len = 1024;
    uint8_t buffer[len];

    // First try with wrong handle */
    Retcode_T rc = MCU_I2C_Receive(0,addr,buffer,len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM),rc);
    // Then use a valid handle, but invalid buffer
    rc = MCU_I2C_Receive((I2C_T)&testContext.mBSPHandleI2C,addr,NULL,len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM),rc);
    // Then use a valid handle, valid buffer but invalid len
    rc = MCU_I2C_Receive((I2C_T)&testContext.mBSPHandleI2C,addr,buffer,UINT16_MAX+1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED),rc);
    // Set fake return value for
    switch (testContext.mBSPHandleI2C.TransferMode)
    {
    case BCDS_HAL_TRANSFER_MODE_BLOCKING:
        break;
    case BCDS_HAL_TRANSFER_MODE_DMA:
        HAL_I2C_Master_Receive_DMA_fake.return_val = HAL_OK;
        break;
    case BCDS_HAL_TRANSFER_MODE_INTERRUPT:
        HAL_I2C_Master_Receive_IT_fake.return_val = HAL_OK;
        break;
    default:
        break;
    }
    // Then us correct parameters
    rc = MCU_I2C_Receive((I2C_T)&testContext.mBSPHandleI2C,addr,buffer,256);
    EXPECT_EQ(RETCODE_OK,rc);
    // Set fake return value for
    switch (testContext.mBSPHandleI2C.TransferMode)
    {
    case BCDS_HAL_TRANSFER_MODE_BLOCKING:
        break;
    case BCDS_HAL_TRANSFER_MODE_DMA:
        HAL_I2C_Master_Receive_DMA_fake.return_val = HAL_ERROR;
        break;
    case BCDS_HAL_TRANSFER_MODE_INTERRUPT:
        HAL_I2C_Master_Receive_IT_fake.return_val = HAL_ERROR;
        break;
    default:
        break;
    }
    rc = MCU_I2C_Receive((I2C_T)&testContext.mBSPHandleI2C,addr,buffer,256);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE),rc);
    // Try to cancel an active transfer, however we have not implemented this
    testContext.mBSPHandleI2C.hi2c.State = HAL_I2C_STATE_BUSY;
    rc = MCU_I2C_Receive((I2C_T)&testContext.mBSPHandleI2C,addr,NULL,0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED),rc);
    testContext.mBSPHandleI2C.hi2c.State = HAL_I2C_STATE_RESET;

}

TEST_F(BCDS_I2Ctest, testI2C_ReadRegister)
{

    /* First try with wrong handle */
    Retcode_T rc = MCU_I2C_ReadRegister(0,0x01,0x0,NULL,0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM),rc);

    /*  Then use a valid handle and try to abort, however until now the function is not supported */
    testContext.mBSPHandleI2C.hi2c.State = HAL_I2C_STATE_BUSY;
    rc = MCU_I2C_ReadRegister((I2C_T)&testContext.mBSPHandleI2C,0x01,0x0,NULL,0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED),rc);
}

TEST_F(BCDS_I2Ctest, testI2C_WriteRegister)
{

    // First try with wrong handle */
    Retcode_T rc = MCU_I2C_WriteRegister(0,0x01,0x0,NULL,0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM),rc);

    /*  Then use a valid handle and try to abort, however until now the function is not supported */
    testContext.mBSPHandleI2C.hi2c.State = HAL_I2C_STATE_BUSY;
    rc = MCU_I2C_WriteRegister((I2C_T)&testContext.mBSPHandleI2C,0x01,0x0,NULL,0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED),rc);
}

/**
 * This test is currently just used to call the I2C event callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_I2Ctest, testI2C_MCU_BSP_I2C_EV_Callback)
{
    __HAL_I2C_GET_FLAG_fake.custom_fake = Fake___HAL_I2C_GET_FLAG;
    /* First try with totally wrong parameters */
    MCU_BSP_I2C_EV_Callback(0);
    /* Then use a valid handle. */
    MCU_BSP_I2C_EV_Callback((I2C_T)&testContext.mBSPHandleI2C);
    /* Then use a valid handle, but set the status like the drivers receive
     * function has never been called
     */
    __HAL_I2C_SET_FLAG(&testContext.mBSPHandleI2C.hi2c,I2C_FLAG_RXNE);
    testContext.mBSPHandleI2C.hi2c.State = HAL_I2C_STATE_READY;
    // Call the function. This should generate a callback event in the context.
    // where the RXReady event is set.
    MCU_BSP_I2C_EV_Callback((I2C_T)&testContext.mBSPHandleI2C);
    // Now the callback should be called onece
    EXPECT_EQ(1,testContext.mTestAppCallbackCount);
    // RxReady should be set, all others 0
    EXPECT_EQ(1,testContext.mLastEvent.RxReady);
    EXPECT_EQ(0,testContext.mLastEvent.RxComplete);
    EXPECT_EQ(0,testContext.mLastEvent.TxComplete);
    EXPECT_EQ(0,testContext.mLastEvent.TransferError);
    // reset the counter
    testContext.mTestAppCallbackCount = 0;
}

/**
 * This test is currently just used to call the I2C error event callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_I2Ctest, testI2C_MCU_BSP_I2C_ER_Callback)
{
    /* First try with totally wrong parameters */
	RESET_FAKE(HAL_I2C_ER_IRQHandler);
    MCU_BSP_I2C_ER_Callback(0);
    EXPECT_EQ(0, HAL_I2C_ER_IRQHandler_fake.call_count);
    /* Then use a valid handle */
    MCU_BSP_I2C_ER_Callback((I2C_T)&testContext.mBSPHandleI2C);
    EXPECT_EQ(1, HAL_I2C_ER_IRQHandler_fake.call_count);
    RESET_FAKE(HAL_I2C_ER_IRQHandler);
}

/**
 * This test is currently just used to call the I2C DMA RX callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_I2Ctest, testI2C_MCU_BSP_I2C_DMA_RX_Callback)
{
    /* First try with totally wrong parameters */
	RESET_FAKE(HAL_DMA_IRQHandler);
    MCU_BSP_I2C_DMA_RX_Callback(0);
    EXPECT_EQ(0, HAL_DMA_IRQHandler_fake.call_count);
    /* Then use a valid handle */
    MCU_BSP_I2C_DMA_RX_Callback((I2C_T)&testContext.mBSPHandleI2C);
    EXPECT_EQ(1, HAL_DMA_IRQHandler_fake.call_count);
    RESET_FAKE(HAL_DMA_IRQHandler);
}

/**
 * This test is currently just used to call the I2C DMA TX callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_I2Ctest, testI2C_MCU_BSP_I2C_DMA_TX_Callback)
{
    /* First try with totally wrong parameters */
	RESET_FAKE(HAL_DMA_IRQHandler);
    MCU_BSP_I2C_DMA_TX_Callback(0);
    EXPECT_EQ(0, HAL_DMA_IRQHandler_fake.call_count);
    /* Then use a valid handle */
    MCU_BSP_I2C_DMA_TX_Callback((I2C_T)&testContext.mBSPHandleI2C);
    EXPECT_EQ(1, HAL_DMA_IRQHandler_fake.call_count);
    RESET_FAKE(HAL_DMA_IRQHandler);
}

/**
 * This test is currently just used to call the I2C error callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_I2Ctest, testI2C_HAL_I2C_ErrorCallback)
{
    HAL_I2C_GetError_fake.custom_fake = Fake_HAL_I2C_GetError;

    /* First try with totally wrong parameters */
    HAL_I2C_ErrorCallback(NULL);
    /* Then use a valid handle but do not set the error flag in the handle.
     * This should not call the application callback
     */
    HAL_I2C_ErrorCallback(&testContext.mBSPHandleI2C.hi2c);
    // Now the callback should not be called
    EXPECT_EQ(0,testContext.mTestAppCallbackCount);
    // Then set an error value for the handle. Possible codes are:
    // HAL_I2C_ERROR_NONE      ((uint32_t)0x00000000)    /*!< No error              */
    // HAL_I2C_ERROR_BERR      ((uint32_t)0x00000001)    /*!< BERR error            */
    // HAL_I2C_ERROR_ARLO      ((uint32_t)0x00000002)    /*!< ARLO error            */
    // HAL_I2C_ERROR_AF        ((uint32_t)0x00000004)    /*!< ACKF error            */
    // HAL_I2C_ERROR_OVR       ((uint32_t)0x00000008)    /*!< OVR error             */
    // HAL_I2C_ERROR_DMA       ((uint32_t)0x00000010)    /*!< DMA transfer error    */
    // HAL_I2C_ERROR_TIMEOUT   ((uint32_t)0x00000020)    /*!< Timeout error         */
    // HAL_I2C_ERROR_SIZE      ((uint32_t)0x00000040)    /*!< Size Management error */
    testContext.mBSPHandleI2C.hi2c.ErrorCode = HAL_I2C_ERROR_BERR;
    // then call the function. This should result in an call back for the app
    HAL_I2C_ErrorCallback(&testContext.mBSPHandleI2C.hi2c);
    EXPECT_EQ(1,testContext.mTestAppCallbackCount);
    EXPECT_EQ(0,testContext.mLastEvent.RxReady);
    EXPECT_EQ(0,testContext.mLastEvent.TxComplete);
    EXPECT_EQ(0,testContext.mLastEvent.RxComplete);
    EXPECT_EQ(1,testContext.mLastEvent.TransferError);
    testContext.mTestAppCallbackCount = 0;
}

/**
 * This test is currently just used to call the I2C MasterTxCpltCallback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_I2Ctest, testI2C_HAL_I2C_MasterTxCpltCallback)
{
    uint32_t appcallbackcount = testContext.mTestAppCallbackCount;
    testContext.mLastEvent.TxComplete = 0;
	/* First try with totally wrong parameters */
    HAL_I2C_MasterTxCpltCallback(NULL);
    EXPECT_EQ(appcallbackcount, testContext.mTestAppCallbackCount);
    /* Then use a valid handle */
    HAL_I2C_MasterTxCpltCallback(&testContext.mBSPHandleI2C.hi2c);
    appcallbackcount += 1;
    EXPECT_EQ(appcallbackcount,testContext.mTestAppCallbackCount);
    EXPECT_EQ(1, testContext.mLastEvent.TxComplete);
}

/**
 * This test is currently just used to call the I2C MasterRxCpltCallback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_I2Ctest, testI2C_HAL_I2C_MasterRxCpltCallback)
{
    uint32_t appcallbackcount = testContext.mTestAppCallbackCount;
    testContext.mLastEvent.RxComplete = 0;
	/* First try with totally wrong parameters */
    HAL_I2C_MasterRxCpltCallback(NULL);
    EXPECT_EQ(appcallbackcount, testContext.mTestAppCallbackCount);
    /* Then use a valid handle */
    HAL_I2C_MasterRxCpltCallback(&testContext.mBSPHandleI2C.hi2c);
    appcallbackcount += 1;
    EXPECT_EQ(appcallbackcount,testContext.mTestAppCallbackCount);
    EXPECT_EQ(1, testContext.mLastEvent.RxComplete);
}


/**
 * @attention This should be the last test case executed because it deintializes
 * the local I2C Handle of the testContext.
 * After that it is necessary to call MCU_I2C_Initialize again if more test
 * are run after this one.
 */
TEST_F(BCDS_I2Ctest, testI2C_DeInitializer)
{
    /* First try with totally wrong parameters */
    Retcode_T rc = MCU_I2C_Deinitialize(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_INVALID_PARAM),rc);
    /* Then use the correct handle */
    rc = MCU_I2C_Deinitialize((I2C_T)&testContext.mBSPHandleI2C);
    EXPECT_EQ(RETCODE_OK,rc);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleI2C.AppLayerCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleI2C.IRQCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleI2C.ERRCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleI2C.DMARxCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleI2C.DMATxCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleI2C.RxFunPtr,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleI2C.TxFunPtr,NULL);
    /* After this any call except to initalize should fail */
    rc = MCU_I2C_Send((I2C_T)&testContext.mBSPHandleI2C,0x01,NULL,0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED),rc);
}

TEST_F(BCDS_I2Ctest, test_MCU_I2C_ReadRegister)
{
    struct MCU_I2C_S BspHandle;
    BspHandle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    I2C_T i2cU = (HWHandle_T) &BspHandle;
    uint8_t slaveAddrU = 0;
    uint8_t registerAddrU = 0;
    uint8_t * rxBufferU = NULL;
    uint32_t rxLenU = 0;

    Retcode_T rc = MCU_I2C_Initialize(i2cU, I2CTestContext::TestAppCallback);
    EXPECT_EQ(RETCODE_OK, rc);

    rc = MCU_I2C_ReadRegister(i2cU, slaveAddrU, registerAddrU, rxBufferU, rxLenU);
    EXPECT_NE(RETCODE_OK, rc);
}
