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

#include <gtest.h>

FFF_DEFINITION_BLOCK_START

extern "C"
{

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID  BCDS_UTILS_MODULE_ID_LOGGING

#if BCDS_FEATURE_LOGGING

#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "portmacro_th.hh"
#include "BCDS_Queue_th.hh"
#include "semphr_th.hh"
#include "LogConfig.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_dma.h"

FAKE_VOID_FUNC(GPIO_PinModeSet, GPIO_Port_TypeDef, unsigned int, GPIO_Mode_TypeDef,
        unsigned int );
FAKE_VOID_FUNC(CMU_ClockEnable, CMU_Clock_TypeDef, bool );
FAKE_VOID_FUNC(USART_Reset, USART_TypeDef * );
FAKE_VOID_FUNC(USART_InitAsync, USART_TypeDef *, const USART_InitAsync_TypeDef * );
FAKE_VOID_FUNC(USART_Enable, USART_TypeDef *, USART_Enable_TypeDef );
FAKE_VOID_FUNC(DMA_Init, DMA_Init_TypeDef *);
FAKE_VOID_FUNC(DMA_CfgChannel, unsigned int, DMA_CfgChannel_TypeDef *);
FAKE_VOID_FUNC(DMA_CfgDescr, unsigned int, bool, DMA_CfgDescr_TypeDef *);
FAKE_VOID_FUNC(DMA_ActivateBasic, unsigned int, bool, bool, void *, void *, unsigned int );
FAKE_VALUE_FUNC(uint32_t, SystemHFXOClockGet);

USART_TypeDef USART_2;
#undef LOG_SER_PORT
#define LOG_SER_PORT    (&USART_2)

#undef RETCODE
#define RETCODE(severity,code) ((Retcode_T) code)

/* As the coverage should be calculated based on the release build source code,
 * the debug asserts are excluded from the source code coverage basis
 */
#undef assert
#define assert(expression)  ((void)0)

#undef BCDS_MODULE_ID
#include "UartAppender.c"
#define Queue_Purge(x) BCDS_UNUSED(x);break;

static SemaphoreHandle_t FakeSemaphoreHandle = NULL;

static SemaphoreHandle_t xSemaphoreCreateBinaryFake(void)
{
    return FakeSemaphoreHandle;
}

static Retcode_T QueueGetResult = RETCODE_FAILURE;

static Retcode_T Queue_GetFake(Queue_T *arg1, void **arg2, uint32_t*arg3, uint32_t arg4)
{
    BCDS_UNUSED(arg1);
    BCDS_UNUSED(arg2);
    BCDS_UNUSED(arg3);
    BCDS_UNUSED(arg4);
    if (RETCODE_FAILURE == QueueGetResult)
    {
        QueueGetResult = RETCODE_OK;
        return RETCODE_FAILURE;
    }
    else
    {
        *arg3 = 1UL; /* Random Number */
    }
    return QueueGetResult;
}

static BaseType_t xTaskPassed = pdFAIL;

static BaseType_t xTaskCreateFake(TaskHookFunction_t TaskFun, const char * arg1,
        unsigned short arg2, void * arg3, UBaseType_t arg4, TaskHandle_t* arg5)
{
    BCDS_UNUSED(TaskFun);
    BCDS_UNUSED(arg1);
    BCDS_UNUSED(arg2);
    BCDS_UNUSED(arg3);
    BCDS_UNUSED(arg4);
    BCDS_UNUSED(arg5);

    return xTaskPassed;
}

static void DMA_CfgChannelFake(unsigned int channel, DMA_CfgChannel_TypeDef *cfg)
{
    cfg->cb->cbFunc(channel, cfg->cb->primary, cfg->cb->userPtr);
    cfg->cb->cbFunc(1, cfg->cb->primary, cfg->cb->userPtr);
    cfg->cb->cbFunc(channel, cfg->cb->primary, NULL);
}

} //-- extern "C"

FFF_DEFINITION_BLOCK_END

class UartAppender: public testing::Test
{
protected:

    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        RESET_FAKE(xSemaphoreCreateBinary);
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(Queue_Get);
        RESET_FAKE(Queue_Create);
        RESET_FAKE(DMA_CfgChannel);

        xSemaphoreCreateBinary_fake.custom_fake = xSemaphoreCreateBinaryFake;
        xTaskCreate_fake.custom_fake = xTaskCreateFake;
        Queue_Get_fake.custom_fake = Queue_GetFake;
        QueueGetResult = RETCODE_FAILURE;
        DMA_CfgChannel_fake.custom_fake = DMA_CfgChannelFake;
    }

    virtual void TearDown()
    {
    }
};

TEST_F(UartAppender, UartAppender_Init)
{
    /** @testcase{ UartAppender::UartAppender_Init: }
     *
     * API used to test the UartAppender_Init functionality
     */
    EXPECT_EQ(RETCODE_OK, Logging_UARTAppender->Init(NULL));
}

TEST_F(UartAppender, UartAppender_Write)
{
    /** @testcase{ UartAppender::UartAppender_Write: }
     *
     * API used to test the UartAppender_Write functionality
     */
    const char message[] = "UartAppenderWriteMessage";
    {
        EXPECT_EQ(RETCODE_OK, Logging_UARTAppender->Write(NULL,0UL));
    }
    {
        EXPECT_EQ(RETCODE_OK, Logging_UARTAppender->Write(message, 0UL));
    }
    {
        EXPECT_EQ(RETCODE_OK, Logging_UARTAppender->Write(message, 5UL));
    }
}
#else
}
#endif /* if BCDS_FEATURE_LOGGING */
