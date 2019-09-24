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

/**
 *
 * @brief
 *      Module unit test for Uarttransceiver__unittest.cc module.
 * 
 * @details
 *      The unit test file template follows the Four-Phase test pattern.
 * 
 * @file
 **/

/* Setup testing framework ************************************************** */

/* Include gtest interface */
#include <gtest.h>
#include <string.h>
#include <unistd.h>

/* Start of global scope symbol and fake definitions section */
extern "C"
{
/* Setup compile time configuration defines */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_UART_TRANSCEIVER

#if KISO_FEATURE_UARTTRANSCEIVER

/* Include faked interfaces */
#include "Kiso_Assert_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_RingBuffer_th.hh"
#include "Kiso_MCU_UART_th.hh"
#include "FreeRTOS_th.hh"
#include "semphr_th.hh"
#include "task_th.hh"

/* Include module under test */
#include "UartTransceiver.c"

    static bool frameEndCheckFunc_FAKE(uint8_t value)
    {
        KISO_UNUSED(value);
        return true;
    }

#if KISO_FEATURE_UART
    static void frameEndNotifyFunc_FAKE(struct MCU_UART_Event_S event)
#elif KISO_FEATURE_LEUART
    static void frameEndNotifyFunc_FAKE(struct MCU_LEUART_Event_S event)
#endif
    {
        KISO_UNUSED(event);
        return;
    }
    static UARTTransceiver_T transceiver;

    /* End of global scope symbol and fake definitions section */
}

#if KISO_FEATURE_UART
#define UART_Receive_fake MCU_UART_Receive_fake
#define UART_TRANSCEIVER_UART_TYPE UART_TRANSCEIVER_UART_TYPE_UART
#elif KISO_FEATURE_LEUART
FAKE_VALUE_FUNC(Retcode_T, MCU_LEUART_Receive, LEUART_T, uint8_t *, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, MCU_LEUART_Send, LEUART_T, uint8_t *, uint32_t)
#define UART_Receive_fake MCU_LEUART_Receive_fake
#define UART_TRANSCEIVER_UART_TYPE UART_TRANSCEIVER_UART_TYPE_LEUART
#endif

/* Create test fixture initializing all variables automatically */

class UARTTransceiverTest : public testing::Test
{
public:
protected:
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {

        transceiver.handle = (HWHandle_T)NULL;
        transceiver.UartType = UART_TRANSCEIVER_UART_TYPE_NONE;
        transceiver.Mode = UART_TRANSCEIVER_MODE_NONE;
        transceiver.State = UART_TRANSCEIVER_STATE_RESET;
        transceiver.EndOfFrameCheck = NULL;
        transceiver.callback = NULL;
        transceiver.UartRxBufDescr.Base = NULL;
        transceiver.UartRxBufDescr.Rptr = NULL;
        transceiver.UartRxBufDescr.Size = 0;
        transceiver.UartRxBufDescr.Wptr = NULL;

        transceiver.RawRxBuffer = NULL;
        transceiver.RawRxBufferSize = 0;
        transceiver.LastByte = 0;
        transceiver.TxSemaphore = (SemaphoreHandle_t)NULL;
        transceiver.RxSemaphore = (SemaphoreHandle_t)NULL;
        transceiver.AsyncEvent.registerValue = 0;
        transceiver.errorCode = RETCODE_SUCCESS;

        RESET_FAKE(xSemaphoreCreateBinary);
#if KISO_FEATURE_UART
        RESET_FAKE(MCU_UART_Receive);
#elif KISO_FEATURE_LEUART
        RESET_FAKE(MCU_LEUART_Receive);
#endif
        FFF_RESET_HISTORY();
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* Nothing to do if clean up is not required */
    }
};
bool EndOfFrameCheckFlag = TRUE;
bool EndOfFrameCheck(uint8_t lastByte)
{
    KISO_UNUSED(lastByte);
    if (TRUE == EndOfFrameCheckFlag)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}
void UartCallback(struct MCU_UART_Event_S event)
{
    KISO_UNUSED(event);
}

/* Specify test cases ******************************************************* */

/**
 *  @brief
 *   
 */
TEST_F(UARTTransceiverTest, UartTransceiverInitialize_SUCCESS)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;

    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, rawRxBufferSize, type);
    EXPECT_EQ(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverInitialize_FAILURE_INVALID_PARAM)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)NULL;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;

    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, rawRxBufferSize, type);
    EXPECT_NE(RETCODE_OK, retcode);

    handle = (HWHandle_T)123;
    retcode = UARTTransceiver_Initialize(NULL, handle, rawRxBuffer, rawRxBufferSize, type);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, rawRxBufferSize, UART_TRANSCEIVER_UART_TYPE_NONE);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, 0, type);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    retcode = UARTTransceiver_Initialize(&transceiver, handle, NULL, rawRxBufferSize, type);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    transceiver.State = UART_TRANSCEIVER_STATE_IDLE;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, rawRxBufferSize, type);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_DOPPLE_INITIALIZATION), retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverInitialize_FAILURE_SEMAPHORE)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)NULL;

    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, rawRxBufferSize, type);
    EXPECT_NE(RETCODE_OK, retcode);
}
/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStart_SUCCESS)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, rawRxBufferSize, type);

    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);
    EXPECT_EQ(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStart_FAILURE_INVALID_PARAM)
{
    Retcode_T retcode;
    uint8_t rawRxBuffer[512];
    HWHandle_T handle = (HWHandle_T)123;
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, rawRxBufferSize, type);

    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Start((UARTTransceiver_T *)NULL, frameEndCheckFunc_FAKE);
    EXPECT_NE(RETCODE_OK, retcode);

    transceiver.UartType = UART_TRANSCEIVER_UART_TYPE_NONE;
    retcode = UARTTransceiver_Start(&transceiver, NULL);
    EXPECT_EQ(RETCODE_OK, retcode);

    transceiver.State = UART_TRANSCEIVER_STATE_IDLE;
    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStart_FAILURE_RECEIVE_ERROR)
{
    Retcode_T retcode;
    uint8_t rawRxBuffer[512];
    HWHandle_T handle = (HWHandle_T)123;
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer, rawRxBufferSize, type);

    UART_Receive_fake.return_val = RETCODE_FAILURE;
    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);
    EXPECT_NE(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStop_SUCCESS)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);

    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Stop(&transceiver);
    EXPECT_EQ(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStop_FAILURE_INVALID_PARAM)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);

    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Stop((UARTTransceiver_T *)NULL);
    EXPECT_NE(RETCODE_OK, retcode);

#if KISO_FEATURE_UART
    transceiver.UartType = UART_TRANSCEIVER_UART_TYPE_NONE;
    transceiver.State = UART_TRANSCEIVER_STATE_ACTIVE;
    retcode = UARTTransceiver_Stop(&transceiver);
    EXPECT_EQ(RETCODE_OK, retcode);

    transceiver.State = UART_TRANSCEIVER_STATE_SUSPENDED;
    retcode = UARTTransceiver_Stop(&transceiver);
    EXPECT_EQ(RETCODE_OK, retcode);
#endif
    transceiver.State = UART_TRANSCEIVER_STATE_STOPPED;
    retcode = UARTTransceiver_Stop(&transceiver);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStop_FAILURE_RECEIVE_ERROR)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);

    UART_Receive_fake.return_val = RETCODE_FAILURE;
    retcode = UARTTransceiver_Stop(&transceiver);
    EXPECT_NE(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStartInAsyncMode_SUCCESS)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_StartInAsyncMode(&transceiver, frameEndCheckFunc_FAKE, frameEndNotifyFunc_FAKE);
    EXPECT_EQ(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStartInAsyncMode_FAILURE_INVALID_PARAM)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_StartInAsyncMode(&transceiver, frameEndCheckFunc_FAKE, (UARTransceiver_Callback_T)NULL);
    EXPECT_NE(RETCODE_OK, retcode);

    retcode = UARTTransceiver_StartInAsyncMode(NULL, frameEndCheckFunc_FAKE, frameEndNotifyFunc_FAKE);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

#if KISO_FEATURE_UART
    transceiver.UartType = UART_TRANSCEIVER_UART_TYPE_NONE;
    retcode = UARTTransceiver_StartInAsyncMode(&transceiver, NULL, frameEndNotifyFunc_FAKE);
    EXPECT_EQ(RETCODE_OK, retcode);

    transceiver.State = UART_TRANSCEIVER_STATE_IDLE;
    retcode = UARTTransceiver_StartInAsyncMode(&transceiver, frameEndCheckFunc_FAKE, frameEndNotifyFunc_FAKE);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
#endif
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverStartInAsyncMode_FAILURE_RECEIVE_ERROR)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_FAILURE;
    retcode = UARTTransceiver_StartInAsyncMode(&transceiver, frameEndCheckFunc_FAKE, (UARTransceiver_Callback_T)NULL);
    EXPECT_NE(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverSuspend_SUCCESS)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Suspend(&transceiver);
    EXPECT_EQ(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverSuspend_FAILURE_INVALID_PARAM)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Suspend((UARTTransceiver_T *)NULL);
    EXPECT_NE(RETCODE_OK, retcode);

#if KISO_FEATURE_UART
    transceiver.UartType = UART_TRANSCEIVER_UART_TYPE_NONE;
    retcode = UARTTransceiver_Suspend(&transceiver);
    EXPECT_EQ(RETCODE_OK, retcode);

    transceiver.State = UART_TRANSCEIVER_STATE_IDLE;
    retcode = UARTTransceiver_Suspend(&transceiver);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
#endif
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverSuspend_FAILURE_RECEIVE_ERROR)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);
    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);
    UART_Receive_fake.return_val = RETCODE_FAILURE;
    retcode = UARTTransceiver_Suspend(&transceiver);
    EXPECT_NE(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverResume_SUCCESS)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);

    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);

    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Suspend(&transceiver);

    UART_Receive_fake.return_val = RETCODE_OK;

    retcode = UARTTransceiver_Resume(&transceiver);

    EXPECT_EQ(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverResume_FAILURE_INVALID_PARAM)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);

    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);

    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Suspend(&transceiver);

    UART_Receive_fake.return_val = RETCODE_OK;

    retcode = UARTTransceiver_Resume((UARTTransceiver_T *)NULL);

    EXPECT_NE(RETCODE_OK, retcode);
#if KISO_FEATURE_UART
    transceiver.UartType = UART_TRANSCEIVER_UART_TYPE_NONE;
    retcode = UARTTransceiver_Resume(&transceiver);
    EXPECT_EQ(RETCODE_OK, retcode);
#endif
    transceiver.State = UART_TRANSCEIVER_STATE_IDLE;
    retcode = UARTTransceiver_Resume(&transceiver);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverResume_FAILURE_RECEIVE_ERROR)
{
    Retcode_T retcode;
    HWHandle_T handle = (HWHandle_T)123;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    enum UARTTransceiver_UartType_E type = UART_TRANSCEIVER_UART_TYPE;
    xSemaphoreCreateBinary_fake.return_val = (SemaphoreHandle_t)0x02020202;
    retcode = UARTTransceiver_Initialize(&transceiver, handle, rawRxBuffer,
                                         rawRxBufferSize, type);

    retcode = UARTTransceiver_Start(&transceiver, frameEndCheckFunc_FAKE);

    UART_Receive_fake.return_val = RETCODE_OK;
    retcode = UARTTransceiver_Suspend(&transceiver);

    UART_Receive_fake.return_val = RETCODE_FAILURE;

    retcode = UARTTransceiver_Resume(&transceiver);

    EXPECT_NE(RETCODE_OK, retcode);
}
/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverDeinitialize_SUCCESS)
{
    Retcode_T retcode;
    retcode = UARTTransceiver_Deinitialize(&transceiver);
    EXPECT_EQ(RETCODE_OK, retcode);
}

/**
 *  @brief
 *
 */
TEST_F(UARTTransceiverTest, UartTransceiverDeinitialize_FAILURE_INVALID_PARAM)
{
    Retcode_T retcode;
    retcode = UARTTransceiver_Deinitialize((UARTTransceiver_T *)NULL);
    EXPECT_NE(RETCODE_OK, retcode);
}

TEST_F(UARTTransceiverTest, UARTTransceiverReadDataTest)
{
    Retcode_T retcode;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    uint32_t timeout_ms = 0;
    uint32_t length = 0;

    retcode = UARTTransceiver_ReadData(NULL, rawRxBuffer, rawRxBufferSize, &length, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    retcode = UARTTransceiver_ReadData(&transceiver, NULL, rawRxBufferSize, &length, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    retcode = UARTTransceiver_ReadData(&transceiver, rawRxBuffer, rawRxBufferSize, NULL, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    retcode = UARTTransceiver_ReadData(&transceiver, rawRxBuffer, 0, &length, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    transceiver.State = UART_TRANSCEIVER_STATE_ACTIVE;
    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.errorCode = RETCODE_SUCCESS;
    xSemaphoreTake_fake.return_val = pdTRUE;
    RingBuffer_Read_fake.return_val = 10U;
    retcode = UARTTransceiver_ReadData(&transceiver, rawRxBuffer, rawRxBufferSize, &length, timeout_ms);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(10U, length);

    transceiver.errorCode = RETCODE_FAILURE;
    retcode = UARTTransceiver_ReadData(&transceiver, rawRxBuffer, rawRxBufferSize, &length, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, transceiver.errorCode), retcode);
    EXPECT_EQ(10U, length);

    xSemaphoreTake_fake.return_val = pdFAIL;
    retcode = UARTTransceiver_ReadData(&transceiver, rawRxBuffer, rawRxBufferSize, &length, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR), retcode);
    EXPECT_EQ(10U, length);

    transceiver.Mode = UART_TRANSCEIVER_MODE_NONE;
    retcode = UARTTransceiver_ReadData(&transceiver, rawRxBuffer, rawRxBufferSize, &length, timeout_ms);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(10U, length);

    transceiver.State = UART_TRANSCEIVER_STATE_INITIALIZED;
    retcode = UARTTransceiver_ReadData(&transceiver, rawRxBuffer, rawRxBufferSize, &length, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}

TEST_F(UARTTransceiverTest, UARTTransceiverWriteDataTest)
{
    Retcode_T retcode;
    uint8_t rawRxBuffer[512];
    uint32_t rawRxBufferSize = 512;
    uint32_t timeout_ms = 0;

    retcode = UARTTransceiver_WriteData(NULL, rawRxBuffer, rawRxBufferSize, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    retcode = UARTTransceiver_WriteData(&transceiver, NULL, rawRxBufferSize, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    retcode = UARTTransceiver_WriteData(&transceiver, rawRxBuffer, 0, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);

    transceiver.State = UART_TRANSCEIVER_STATE_ACTIVE;
    transceiver.UartType = UART_TRANSCEIVER_UART_TYPE_UART;
    transceiver.Mode = UART_TRANSCEIVER_MODE_ASYNCH;
    xSemaphoreTake_fake.return_val = pdTRUE;
    RingBuffer_Read_fake.return_val = 10U;
    retcode = UARTTransceiver_WriteData(&transceiver, rawRxBuffer, rawRxBufferSize, timeout_ms);
    EXPECT_EQ(RETCODE_OK, retcode);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    xSemaphoreTake_fake.return_val = pdFAIL;
    retcode = UARTTransceiver_WriteData(&transceiver, rawRxBuffer, rawRxBufferSize, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR), retcode);

    xSemaphoreTake_fake.return_val = pdTRUE;
    retcode = UARTTransceiver_WriteData(&transceiver, rawRxBuffer, rawRxBufferSize, timeout_ms);
    EXPECT_EQ(RETCODE_OK, retcode);

    MCU_UART_Send_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    retcode = UARTTransceiver_WriteData(&transceiver, rawRxBuffer, rawRxBufferSize, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retcode);

#if KISO_FEATURE_UART
    transceiver.UartType = UART_TRANSCEIVER_UART_TYPE_NONE;
    retcode = UARTTransceiver_WriteData(&transceiver, rawRxBuffer, rawRxBufferSize, timeout_ms);
    EXPECT_EQ(RETCODE_OK, retcode);
#endif

    transceiver.State = UART_TRANSCEIVER_STATE_IDLE;
    retcode = UARTTransceiver_WriteData(&transceiver, rawRxBuffer, rawRxBufferSize, timeout_ms);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), retcode);
}
#if KISO_FEATURE_UART
TEST_F(UARTTransceiverTest, UARTTransceiverLoopCallbackReceiveTest)
{
    struct MCU_UART_Event_S event;
    event.RxComplete = 1;
    event.TxComplete = 0;
    event.RxError = 0;
    transceiver.AsyncEvent.registerValue = 0;
    RingBuffer_Write_fake.return_val = 1;
    transceiver.EndOfFrameCheck = EndOfFrameCheck;
    transceiver.callback = UartCallback;
    transceiver.Mode = UART_TRANSCEIVER_MODE_ASYNCH;

    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(1U, transceiver.AsyncEvent.bitfield.RxComplete);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.AsyncEvent.bitfield.RxComplete = 0;
    xSemaphoreGiveFromISR_fake.return_val = pdTRUE;
    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxComplete);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.AsyncEvent.bitfield.RxComplete = 0;
    xSemaphoreGiveFromISR_fake.return_val = pdFAIL;
    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxComplete);

    EndOfFrameCheckFlag = FALSE;
    transceiver.AsyncEvent.bitfield.RxComplete = 0;
    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxComplete);
    EndOfFrameCheckFlag = TRUE;

    transceiver.AsyncEvent.bitfield.RxComplete = 0;
    RingBuffer_Write_fake.return_val = 0;
    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxComplete);
}
TEST_F(UARTTransceiverTest, UARTTransceiverLoopCallbackTxTest)
{
    struct MCU_UART_Event_S event;
    event.RxComplete = 0;
    event.TxComplete = 1;
    event.RxError = 0;
    transceiver.Mode = UART_TRANSCEIVER_MODE_ASYNCH;
    transceiver.AsyncEvent.registerValue = 0;
    transceiver.callback = UartCallback;

    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(1U, transceiver.AsyncEvent.bitfield.TxComplete);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.AsyncEvent.bitfield.TxComplete = 0;
    xSemaphoreGiveFromISR_fake.return_val = pdTRUE;
    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.TxComplete);

    xSemaphoreGiveFromISR_fake.return_val = pdFAIL;
    transceiver.AsyncEvent.bitfield.TxComplete = 0;
    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.TxComplete);
}

TEST_F(UARTTransceiverTest, LoopCallbackRxErrorTest)
{
    struct MCU_UART_Event_S event;
    event.RxComplete = 0;
    event.TxComplete = 0;
    event.RxError = 1;
    transceiver.Mode = UART_TRANSCEIVER_MODE_ASYNCH;
    transceiver.AsyncEvent.registerValue = 0;
    transceiver.callback = UartCallback;

    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(1U, transceiver.AsyncEvent.bitfield.RxError);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.AsyncEvent.bitfield.RxError = 0;
    xSemaphoreGiveFromISR_fake.return_val = pdTRUE;
    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxError);
    EXPECT_EQ(RETCODE_FAILURE, transceiver.errorCode);

    xSemaphoreGiveFromISR_fake.return_val = pdFAIL;
    transceiver.AsyncEvent.bitfield.RxError = 0;
    UARTTransceiver_LoopCallback(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxError);
    EXPECT_EQ(RETCODE_FAILURE, transceiver.errorCode);
}

#elif KISO_FEATURE_LEUART
TEST_F(UARTTransceiverTest, UARTLELoopCallbackReceiveTest)
{
    struct MCU_LEUART_Event_S event;
    event.RxComplete = 1;
    event.TxComplete = 0;
    event.RxError = 0;
    transceiver.AsyncEvent.registerValue = 0;
    RingBuffer_Write_fake.return_val = 1;
    transceiver.EndOfFrameCheck = EndOfFrameCheck;
    transceiver.callback = UartCallback;
    transceiver.Mode = UART_TRANSCEIVER_MODE_ASYNCH;

    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(1U, transceiver.AsyncEvent.bitfield.RxComplete);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.AsyncEvent.bitfield.RxComplete = 0;
    xSemaphoreGiveFromISR_fake.return_val = pdTRUE;
    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxComplete);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.AsyncEvent.bitfield.RxComplete = 0;
    xSemaphoreGiveFromISR_fake.return_val = pdFAIL;
    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxComplete);

    EndOfFrameCheckFlag = FALSE;
    transceiver.AsyncEvent.bitfield.RxComplete = 0;
    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxComplete);
    EndOfFrameCheckFlag = TRUE;

    transceiver.AsyncEvent.bitfield.RxComplete = 0;
    RingBuffer_Write_fake.return_val = 0;
    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxComplete);
}
TEST_F(UARTTransceiverTest, UARTLELoopCallbackTxTest)
{
    struct MCU_LEUART_Event_S event;
    event.RxComplete = 0;
    event.TxComplete = 1;
    event.RxError = 0;
    transceiver.Mode = UART_TRANSCEIVER_MODE_ASYNCH;
    transceiver.AsyncEvent.registerValue = 0;
    transceiver.callback = UartCallback;

    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(1U, transceiver.AsyncEvent.bitfield.TxComplete);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.AsyncEvent.bitfield.TxComplete = 0;
    xSemaphoreGiveFromISR_fake.return_val = pdTRUE;
    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.TxComplete);

    xSemaphoreGiveFromISR_fake.return_val = pdFAIL;
    transceiver.AsyncEvent.bitfield.TxComplete = 0;
    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.TxComplete);
}

TEST_F(UARTTransceiverTest, LELoopCallbackRxErrorTest)
{
    struct MCU_LEUART_Event_S event;
    event.RxComplete = 0;
    event.TxComplete = 0;
    event.RxError = 1;
    transceiver.Mode = UART_TRANSCEIVER_MODE_ASYNCH;
    transceiver.AsyncEvent.registerValue = 0;
    transceiver.callback = UartCallback;

    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(1U, transceiver.AsyncEvent.bitfield.RxError);

    transceiver.Mode = UART_TRANSCEIVER_MODE_SYNCH;
    transceiver.AsyncEvent.bitfield.RxError = 0;
    xSemaphoreGiveFromISR_fake.return_val = pdTRUE;
    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxError);
    EXPECT_EQ(RETCODE_FAILURE, transceiver.errorCode);

    xSemaphoreGiveFromISR_fake.return_val = pdFAIL;
    transceiver.AsyncEvent.bitfield.RxError = 0;
    UARTTransceiver_LoopCallbackLE(&transceiver, event);
    EXPECT_EQ(0U, transceiver.AsyncEvent.bitfield.RxError);
    EXPECT_EQ(RETCODE_FAILURE, transceiver.errorCode);
}
#endif /* KISO_FEATURE_LEUART */
TEST_F(UARTTransceiverTest, DummyFrameEndCheckFuncTest)
{
    bool status;
    status = dummyFrameEndCheckFunc(0);
    EXPECT_EQ(false, status);
}
#else
}

#endif //KISO_FEATURE_UARTTRANSCEIVER
