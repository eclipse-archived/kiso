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
/**
 * @file
 *
 * @brief Handles MCU STM32 UART driver encapsulation
 */

#include "BCDS_MCU_UART.h"

#if BCDS_FEATURE_UART

#include "BCDS_MCU_UART_Handle.h"
#include "stm32f7xx_hal_uart.h"

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_MODULE_ID_UART

/**
 * @brief      Wrapper for HAL sending function in blocking mode
 * @param[in]  uartHandle : UART handle.
 *
 * @param[in]  data : Pointer to the buffer which contains the data to be transmitted.
 *
 * @param[in]  length : The length of the data to be transmitted.
 * @retval      RETCODE_OK in the case of success or
 * @retval      An error code in case of failure
 */
static Retcode_T UART_SendBlocking(struct MCU_UART_Handle_S* uartHandle, uint8_t * data, uint32_t length);
/**
 * @brief     Wrapper for HAL Receiving function in blocking mode
 * @param[in] uart : UART handle.
 * @param[in] buffer : Pointer to the buffer in which the received data is to be
 *                   stored.
 * @param[in]  size : The length of the data to be received. If 0, then any
 *                   previous receive process is cancelled.
 */
static Retcode_T UART_ReceiveBlocking(struct MCU_UART_Handle_S* uartHandle, uint8_t * data, uint32_t length);

/**
 * @brief Mapper for HAL function return values
 * @param Vendor driver return code
 */
static Retcode_T MapHalRetToMcuRet(HAL_StatusTypeDef halRet);

/** See public interface function description in BCDS_MCU_UART.h */
Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback)
{
    if (NULL == uart)
    {
        /* Check if handle valid pointer failure */
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    struct MCU_UART_Handle_S* uartHandle = (struct MCU_UART_Handle_S*) uart;

    if (uartHandle->TxState != MCU_UART_STATE_INIT || uartHandle->RxState != MCU_UART_STATE_INIT)
    {
        /* Check valid state failure */
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }

    if (BCDS_HAL_TRANSFER_MODE_BLOCKING == uartHandle->TransferMode)
    {
        uartHandle->RxFunc = UART_ReceiveBlocking;
        uartHandle->TxFunc = UART_SendBlocking;
        uartHandle->RxState = MCU_UART_STATE_READY;
        uartHandle->TxState = MCU_UART_STATE_READY;
        uartHandle->AppCallback = NULL;
        uartHandle->RxUserSize = 0;
        uartHandle->RxUserData_ptr = NULL;
        return RETCODE_OK;
    }
    else
    {
        BCDS_UNUSED(callback);
        /* Other transfer modes currently not supported */
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED);
    }
}

/** See public interface function description in BCDS_MCU_UART.h */
Retcode_T MCU_UART_Deinitialize(UART_T uart)
{
    if (NULL == uart)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = RETCODE_OK;
    struct MCU_UART_Handle_S* uartHandle = (struct MCU_UART_Handle_S*) uart;

    if ((uartHandle->RxState == MCU_UART_STATE_READY) && (uartHandle->TxState == MCU_UART_STATE_READY))
    {
        uartHandle->IrqCallback = NULL;
        uartHandle->DmaRxCallback = NULL;
        uartHandle->DmaTxCallback = NULL;
        uartHandle->RxFunc = NULL;
        uartHandle->TxFunc = NULL;
        uartHandle->AppCallback = NULL;
        uartHandle->RxUserSize = 0;
        uartHandle->RxUserData_ptr = NULL;
        uartHandle->RxState = MCU_UART_STATE_INIT;
        uartHandle->TxState = MCU_UART_STATE_INIT;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);

    }
    return retcode;
}
/** See public interface function description in BCDS_MCU_UART.h */
Retcode_T MCU_UART_Send(UART_T uart, uint8_t * data, uint32_t length)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_UART_Handle_S* uartHandle = (struct MCU_UART_Handle_S*) uart;

    if ((NULL == uartHandle) || (NULL == data) || (UINT16_MAX <= length))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (MCU_UART_STATE_READY == uartHandle->TxState)
    {
        retcode = uartHandle->TxFunc(uartHandle, data, length);
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }

    return retcode;
}

/** See public interface function description in BCDS_MCU_UART.h */
Retcode_T MCU_UART_Receive(UART_T uart, uint8_t * data, uint32_t length)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_UART_Handle_S* uartHandle = (struct MCU_UART_Handle_S*) uart;

    if ((NULL == uartHandle) || (NULL == data) || (UINT16_MAX <= length))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (length != 0)
    {
        if (MCU_UART_STATE_READY == uartHandle->RxState)
        {
            retcode = uartHandle->RxFunc(uartHandle, data, length);
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    else
    {
        if (MCU_UART_STATE_RUNNING == uartHandle->RxState)
        {
            /* application wants to cancel ongoing reception which is signaled by length == 0 */
            /* HAL_UART_AbortReceive_IT not supported in stm32f7 HAL */
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED);
        }
        else
        {
            /* Receive was never successfully triggered prior */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }

    return retcode;
}

/** See public interface function description in BCDS_MCU_UART.h */
Retcode_T MCU_UART_GetRxCount(UART_T uart, uint32_t* count)
{
    BCDS_UNUSED(uart);
    BCDS_UNUSED(count);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

static Retcode_T UART_SendBlocking(struct MCU_UART_Handle_S* uartHandle, uint8_t * data, uint32_t length)
{
    uartHandle->TxState = MCU_UART_STATE_RUNNING;
    HAL_StatusTypeDef status = HAL_UART_Transmit(&uartHandle->huart, data, length, uartHandle->TxBlockingModeTimeout);
    uartHandle->TxState = MCU_UART_STATE_READY;
    return MapHalRetToMcuRet(status);
}

Retcode_T UART_ReceiveBlocking(struct MCU_UART_Handle_S* uartHandle, uint8_t * data, uint32_t length)
{
    BCDS_UNUSED(uartHandle);
    BCDS_UNUSED(data);
    BCDS_UNUSED(length);

    return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED);
}

static Retcode_T MapHalRetToMcuRet(HAL_StatusTypeDef halRet)
{
    Retcode_T ret;
    switch (halRet)
    {
    case HAL_OK:
        ret = RETCODE_OK;
        break;
    case HAL_BUSY:
        ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UART_HAL_BUSY);
        break;
    case HAL_TIMEOUT:
        ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UART_HAL_TIMEOUT);
        break;
    case HAL_ERROR:
        /* Fall Through */
    default:
        ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UART_HAL_ERROR);
        break;
    }
    return ret;
}

#endif /* BCDS_FEATURE_UART && UART_COUNT */
