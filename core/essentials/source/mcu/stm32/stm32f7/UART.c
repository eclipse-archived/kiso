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
 * @brief       Handles MCU STM32F7 UART driver encapsulation
 */

/*---------------------- INCLUDED HEADERS ---------------------------------------------------------------------------*/

#include "Kiso_MCU_UART.h"
#if KISO_FEATURE_UART
#include "Kiso_MCU_STM32F7_UART_Handle.h"
/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_UART

#define UART_MIN_TIMEOUT_MS UINT32_C(100)       /* Minimal timeout in ms*/
#define UART_SECOND_TO_MILLI_MS UINT32_C(1000)  /* Convert one second to millisecond*/
#define UART_SAFETY_FACTOR UINT32_C(2)          /* factor to multiply with the reulting time value for safety */
#define UART_CLOCK_CYCLES_PER_BYTE UINT32_C(12) /* a 9 bits value + one start bit* 2 stop bits */

#define UART_HAL_ERROR_RECOVERABLE (HAL_UART_ERROR_PE | HAL_UART_ERROR_NE | HAL_UART_ERROR_FE) /* recoverable error flags combined */
#define UART_HAL_ERROR_NON_RECOVERABLE (HAL_UART_ERROR_ORE | HAL_UART_ERROR_DMA)               /* non recoverable error flags combined */

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

static void UART_IRQHandler(UART_T uart);
static void UART_DMARxHandler(UART_T uart);
static void UART_DMATxHandler(UART_T uart);

static Retcode_T UART_SendPolling(struct MCU_UART_S *uart_ptr);
static Retcode_T UART_ReceivePolling(struct MCU_UART_S *uart_ptr);

static Retcode_T UART_SendInt(struct MCU_UART_S *uart_ptr);
static Retcode_T UART_ReceiveInt(struct MCU_UART_S *uart_ptr);

static Retcode_T UART_SendDMA(struct MCU_UART_S *uart_ptr);
static Retcode_T UART_ReceiveDMA(struct MCU_UART_S *uart_ptr);

static void UART_AbortSend(struct MCU_UART_S *uart_ptr);
static void UART_AbortReceive(struct MCU_UART_S *uart_ptr);

static Retcode_T MapHalRetToMcuRet(HAL_StatusTypeDef halRet);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/** @brief See public interface function description in Kiso_MCU_UART.h */
Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)uart;

    if (NULL == uart_ptr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if ((uart_ptr->TxState != UART_STATE_INIT) || (uart_ptr->RxState != UART_STATE_INIT))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        switch (uart_ptr->TxMode)
        {
        case KISO_HAL_TRANSFER_MODE_POLLING:
            uart_ptr->SendFunc = UART_SendPolling;
            uart_ptr->AbortSendFunc = UART_AbortSend;
            uart_ptr->TxState = UART_STATE_READY;
            break;

        case KISO_HAL_TRANSFER_MODE_INTERRUPT:
            if (callback)
            {
                uart_ptr->AppCallback = callback;
                uart_ptr->SendFunc = UART_SendInt;
                uart_ptr->AbortSendFunc = UART_AbortSend;
                uart_ptr->IrqCallback = UART_IRQHandler;
                uart_ptr->TxState = UART_STATE_READY;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            }
            break;

        case KISO_HAL_TRANSFER_MODE_DMA:
            if (callback)
            {
                uart_ptr->AppCallback = callback;
                uart_ptr->SendFunc = UART_SendDMA;
                uart_ptr->AbortSendFunc = UART_AbortSend;
                uart_ptr->IrqCallback = UART_IRQHandler;
                uart_ptr->DmaTxCallback = UART_DMATxHandler;
                uart_ptr->TxState = UART_STATE_READY;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            }
            break;

        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        switch (uart_ptr->RxMode)
        {
        case KISO_HAL_TRANSFER_MODE_POLLING:
            uart_ptr->ReceiveFunc = UART_ReceivePolling;
            uart_ptr->AbortReceiveFunc = UART_AbortReceive;

            uart_ptr->RxState = UART_STATE_READY;
            break;

        case KISO_HAL_TRANSFER_MODE_INTERRUPT:
            if (callback)
            {
                uart_ptr->AppCallback = callback;
                uart_ptr->ReceiveFunc = UART_ReceiveInt;
                uart_ptr->AbortReceiveFunc = UART_AbortReceive;
                uart_ptr->IrqCallback = UART_IRQHandler;
                uart_ptr->RxState = UART_STATE_READY;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            }
            break;

        case KISO_HAL_TRANSFER_MODE_DMA:
            if (callback)
            {
                uart_ptr->AppCallback = callback;
                uart_ptr->ReceiveFunc = UART_ReceiveDMA;
                uart_ptr->AbortReceiveFunc = UART_AbortReceive;
                uart_ptr->IrqCallback = UART_IRQHandler;
                uart_ptr->DmaRxCallback = UART_DMARxHandler;
                uart_ptr->RxState = UART_STATE_READY;
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            }
            break;

        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        }
    }
    return retcode;
}

/** @brief See public interface function description in Kiso_MCU_UART.h */
Retcode_T MCU_UART_Deinitialize(UART_T uart)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)uart;
    if (NULL == uart_ptr)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if ((uart_ptr->TxState != UART_STATE_READY) || (uart_ptr->RxState != UART_STATE_READY))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        uart_ptr->RxState = UART_STATE_INIT;
        uart_ptr->TxState = UART_STATE_INIT;
    }
    return retcode;
}

/** @brief See public interface function description in Kiso_MCU_UART.h */
Retcode_T MCU_UART_Send(UART_T uart, const uint8_t *data, uint32_t len)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)uart;

    if (NULL == uart_ptr || NULL == data)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (len > UINT16_MAX)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (len > 0)
        {
            if (uart_ptr->TxState == UART_STATE_READY)
            {
                uart_ptr->Transaction.pTransmitBuffer = data;
                uart_ptr->Transaction.TransmitSize = (uint16_t)len;
                retcode = uart_ptr->SendFunc(uart_ptr);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (uart_ptr->TxState == UART_STATE_TX)
            {
                uart_ptr->AbortSendFunc(uart_ptr);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
    }
    return retcode;
}

/** @brief See public interface function description in Kiso_MCU_UART.h */
Retcode_T MCU_UART_Receive(UART_T uart, uint8_t *buffer, uint32_t size)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)uart;

    if (NULL == uart_ptr || NULL == buffer || size > UINT16_MAX)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (size > 0)
        {
            if (uart_ptr->RxState == UART_STATE_READY)
            {
                uart_ptr->Transaction.pReceiveBuffer = buffer;
                uart_ptr->Transaction.ReceivetSize = (uint16_t)size;
                retcode = uart_ptr->ReceiveFunc(uart_ptr);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (uart_ptr->RxState == UART_STATE_RX)
            {
                uart_ptr->AbortReceiveFunc(uart_ptr);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
    }
    return retcode;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * @brief       Sends Data in polling mode
 * @details     If KISO_HAL_TRANSFER_MODE_POLLING is configured for TxMode this function will be called from
 *              MCU_UART_Send() to process the sending in polling mode i.e. no interrupts will be enabled and the
 *              function will actively wait for status change.
 * @param[in]   uart reference to the UART control block structure.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_INCONSISTENT_STATE if an ongoing process is running on the STM32 HAL library level.
 * @retval      RETCODE_TIMEOUT if a timeout occurred.
 */
static Retcode_T UART_SendPolling(struct MCU_UART_S *uart)
{
    uart->TxState = UART_STATE_TX;
    uint32_t timeout = UART_MIN_TIMEOUT_MS;
    if (uart->Datarate)
    {
        timeout += ((uart->Transaction.TransmitSize * UART_CLOCK_CYCLES_PER_BYTE * UART_SAFETY_FACTOR) / (1 + uart->Datarate / UART_SECOND_TO_MILLI_MS));
    }
    HAL_StatusTypeDef status = HAL_ERROR;
// stm32CubeL4 does not consider transmit buffer as const and therefore we have to discard the const qualifier here
#pragma GCC diagnostic push
#ifdef __cplusplus
#pragma GCC diagnostic ignored "-Wcast-qual"
#else
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
#pragma GCC diagnostic ignored "-Wcast-qual"

#endif
    status = HAL_UART_Transmit(&uart->huart, (uint8_t *)uart->Transaction.pTransmitBuffer, uart->Transaction.TransmitSize, timeout);

#ifndef __cplusplus
#pragma GCC diagnostic pop
#endif
    uart->TxState = UART_STATE_READY;

    return MapHalRetToMcuRet(status);
}

/**
 * @brief       Receives data in polling mode.
 * @details     If KISO_HAL_TRANSFER_MODE_POLLING is configured for RxMode this function will be called from
 *              MCU_UART_Receive() to process the receiving in polling mode. It will block until reaching the count of
 *              data to be received.
 * @param[in]   uart reference to the UART control block structure.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_INCONSISTENT_STATE if an ongoing process is running on the STM32 HAL library level.
 * @retval      RETCODE_TIMEOUT if a timeout occurred.
 */
static Retcode_T UART_ReceivePolling(struct MCU_UART_S *uart)
{
    uart->RxState = UART_STATE_RX;

    uint32_t timeout = UART_MIN_TIMEOUT_MS;
    if (uart->Datarate)
    {
        timeout += ((uart->Transaction.ReceivetSize * UART_CLOCK_CYCLES_PER_BYTE * UART_SAFETY_FACTOR) / (1 + uart->Datarate / UART_SECOND_TO_MILLI_MS));
    }
    HAL_StatusTypeDef status =
        HAL_UART_Receive(&uart->huart, uart->Transaction.pReceiveBuffer, uart->Transaction.ReceivetSize, timeout);
    uart->RxState = UART_STATE_READY;
    return MapHalRetToMcuRet(status);
}

/**
 * @brief       Sends data in interrupt mode.
 * @details     If KISO_HAL_TRANSFER_MODE_INTERRUPT is configured for TxMode this function will be called from
 *              MCU_UART_Send() to process the receiving in interrupt mode. It will trigger the sending process
 *              and return, the process will then be continued in background and the application will be informed
 *              through callbacks.
 * @param[in]   uart reference to the UART control block structure.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_INCONSISTENT_STATE if an ongoing process is running on the STM32 HAL library level.
 * @retval      RETCODE_FAILURE if an other error occurred.
 */
Retcode_T UART_SendInt(struct MCU_UART_S *uart)
{
    uart->TxState = UART_STATE_TX;
// stm32CubeL4 does not consider transmit buffer as const and therefore we have to discard the const qualifier here
#pragma GCC diagnostic push
#ifdef __cplusplus
#pragma GCC diagnostic ignored "-Wcast-qual"
#else
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
#pragma GCC diagnostic ignored "-Wcast-qual"

#endif
    HAL_StatusTypeDef status =
        HAL_UART_Transmit_IT(&uart->huart, (uint8_t *)uart->Transaction.pTransmitBuffer, uart->Transaction.TransmitSize);
#ifndef __cplusplus
#pragma GCC diagnostic pop
#endif
    if (HAL_OK != status)
    {
        uart->TxState = UART_STATE_READY;
    }
    return MapHalRetToMcuRet(status);
}

/**
 * @brief       Receives data in interrupt mode.
 * @details     If KISO_HAL_TRANSFER_MODE_INTERRUPT is configured for RxMode this function will be called from
 *              MCU_UART_Receive() to process the receiving in interrupt mode. It will trigger the reception process
 *              and return, the process will then be continued in background and the application will be informed
 *              through callbacks.
 * @param[in]   uart reference to the UART control block structure.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_INCONSISTENT_STATE if an ongoing process is running on the STM32 HAL library level.
 * @retval      RETCODE_FAILURE if an other error occurred.
 */
static Retcode_T UART_ReceiveInt(struct MCU_UART_S *uart)
{
    uart->RxState = UART_STATE_RX;
    HAL_StatusTypeDef status =
        HAL_UART_Receive_IT(&uart->huart, uart->Transaction.pReceiveBuffer, uart->Transaction.ReceivetSize);
    if (HAL_OK != status)
    {
        uart->RxState = UART_STATE_READY;
    }
    return MapHalRetToMcuRet(status);
}

/**
 * @brief       Sends data in interrupt mode.
 * @details     If KISO_HAL_TRANSFER_MODE_DMA is configured for TxMode this function will be called from
 *              MCU_UART_Send() to process the receiving in DMA mode. It will trigger the sending process
 *              and return, the process will then be continued in background and the application will be informed
 *              through callbacks.
 * @param[in]   uart reference to the UART control block structure.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_INCONSISTENT_STATE if an ongoing process is running on the STM32 HAL library level.
 * @retval      RETCODE_FAILURE if an other error occurred.
 */
static Retcode_T UART_SendDMA(struct MCU_UART_S *uart)
{
    uart->TxState = UART_STATE_TX;
// stm32CubeL4 does not consider transmit buffer as const and therefore we have to discard the const qualifier here
#pragma GCC diagnostic push
#ifdef __cplusplus
#pragma GCC diagnostic ignored "-Wcast-qual"
#else
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
#pragma GCC diagnostic ignored "-Wcast-qual"

#endif
    HAL_StatusTypeDef status =
        HAL_UART_Transmit_DMA(&uart->huart, (uint8_t *)uart->Transaction.pTransmitBuffer, uart->Transaction.TransmitSize);

#ifndef __cplusplus
#pragma GCC diagnostic pop
#endif
    if (HAL_OK != status)
    {
        uart->TxState = UART_STATE_READY;
    }
    return MapHalRetToMcuRet(status);
}

/**
 * @brief       Receives data in interrupt mode.
 * @details     If KISO_HAL_TRANSFER_MODE_DMA is configured for RxMode this function will be called from
 *              MCU_UART_Receive() to process the receiving in DMA mode. It will trigger the reception process
 *              and return, the process will then be continued in background and the application will be informed
 *              through callbacks.
 * @param[in]   uart reference to the UART control block structure.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_INCONSISTENT_STATE if an ongoing process is running on the STM32 HAL library level.
 * @retval      RETCODE_FAILURE if an other error occurred.
 */
static Retcode_T UART_ReceiveDMA(struct MCU_UART_S *uart)
{
    uart->RxState = UART_STATE_RX;
    HAL_StatusTypeDef status =
        HAL_UART_Receive_DMA(&uart->huart, uart->Transaction.pReceiveBuffer, uart->Transaction.ReceivetSize);
    if (HAL_OK != status)
    {
        uart->RxState = UART_STATE_READY;
    }
    return MapHalRetToMcuRet(status);
}

/**
 * @brief       Aborts sending operation.
 * @param[in]   uart reference to the UART control block structure.
 */
void UART_AbortSend(struct MCU_UART_S *uart)
{
    (void)HAL_UART_AbortTransmit(&uart->huart);
    uart->TxState = UART_STATE_READY;
}

/**
 * @brief       Aborts receiving operation.
 * @param[in]   uart reference to the UART control block structure.
 */
void UART_AbortReceive(struct MCU_UART_S *uart)
{
    (void)HAL_UART_AbortReceive(&uart->huart);
    uart->RxState = UART_STATE_READY;
}

/**
 * @brief       Mapper for HAL function return values
 * @param[in]   Vendor driver return code
 */
static Retcode_T MapHalRetToMcuRet(HAL_StatusTypeDef halRet)
{
    Retcode_T retcode;
    switch (halRet)
    {
    case HAL_OK:
        retcode = RETCODE_OK;
        break;
    case HAL_BUSY:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        break;
    case HAL_TIMEOUT:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT);
        break;
    case HAL_ERROR:
        /* Fall Through */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        break;
    }
    return retcode;
}

/**
 * @brief       Callback invoked by BSP in case UART IRQ
 * @param[in]   reference to the UART control block structure
 */
static void UART_IRQHandler(UART_T uart)
{
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)uart;
    HAL_UART_IRQHandler(&uart_ptr->huart);
}

/**
 * @brief       IRQ handler for DMA RX called by BSP
 * @param[in]   reference to the UART control block structure
 */
static void UART_DMARxHandler(UART_T uart)
{
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)uart;
    HAL_DMA_IRQHandler(uart_ptr->huart.hdmarx);
}

/**
 * @brief       IRQ handler for DMA TX called by BSP
 * @param[in]   reference to the UART control block structure
 */
static void UART_DMATxHandler(UART_T uart)
{
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)uart;
    HAL_DMA_IRQHandler(uart_ptr->huart.hdmatx);
}

/**
 * @brief       Tx Transfer completed callback.
 * @note        MCU UART implementation of drivers (STM32Cube) weak callback
 * @param[in]   *huart reference to the STM32 HAL library UART handle
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* cast UART_HandleTypeDef pointer to MCU_UART_S pointer because the first member in the MCU_UART_S structure is
     * actually the UART_HandleTypeDef structure which results in an equal pointer (start address)
     */
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)huart;
    union MCU_UART_Event_U event;
    event.registerValue = 0;
    event.bitfield.TxComplete = 1;
    uart_ptr->AppCallback((UART_T)uart_ptr, event.bitfield);
    uart_ptr->TxState = UART_STATE_READY;
}

/**
 * @brief       This is the RX callback function which is invoked by the stm32 uart driver.
 * @note        MCU UART implementation of drivers (STM32Cube) weak callback
 * @param[in]    *huart reference to the STM32 HAL library UART handle
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* cast UART_HandleTypeDef pointer to MCU_UART_S pointer because the first member in the MCU_UART_S structure is
     * actually the UART_HandleTypeDef structure which results in an equal pointer (start address)
     */
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)huart;
    union MCU_UART_Event_U event;

    event.registerValue = 0;
    event.bitfield.RxComplete = 1;
    /* call app layer event handler */
    uart_ptr->AppCallback((UART_T)uart_ptr, event.bitfield);
    if (uart_ptr->RxState == UART_STATE_RX)
    {
        /* recall receive if we are still in receive mode */
        Retcode_T retcode = uart_ptr->ReceiveFunc(uart_ptr);
        if (RETCODE_OK != retcode)
        {
            Retcode_RaiseErrorFromIsr(retcode);
        }
    }
}

/**
 * @brief       UART error callback
 * @note        MCU UART implementation of STM32Cube library weak callback
 * @param[in]   *huart reference to the STM32 HAL library UART handle
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* cast UART_HandleTypeDef pointer to MCU_UART_S pointer because the first member in the MCU_UART_S structure is
     * actually the UART_HandleTypeDef structure which results in an equal pointer (start address)
     */
    struct MCU_UART_S *uart_ptr = (struct MCU_UART_S *)huart;
    union MCU_UART_Event_U event;

    event.registerValue = 0;
    event.bitfield.RxError = 1;
    if ((UART_HAL_ERROR_NON_RECOVERABLE & huart->ErrorCode))
    {
        /* non recoverable error occurred, transfer cannot go on, the application is informed and the transfer is aborted */
        uart_ptr->AbortReceiveFunc(uart_ptr);
        event.bitfield.RxAborted = 1;
    }
    /* call app layer event handler */
    uart_ptr->AppCallback((UART_T)uart_ptr, event.bitfield);
}

#endif /* KISO_FEATURE_UART */
