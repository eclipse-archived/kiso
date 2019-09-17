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

#include "Kiso_Utils.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_I2C_TRANSCEIVER

#if KISO_FEATURE_I2CTRANSCEIVER
/* Put the type and macro definitions here */
#include "Kiso_I2CTransceiver.h"
#if KISO_FEATURE_I2C
/* Put constant and variable definitions here */
#define CANCEL_I2C_TRANSMISSION  UINT32_C(0)
#define DATA_TRANSFER_TIMEOUT_MS  UINT32_C(1000)

/*  The description of the function is available in Kiso_I2CTransceiver.h */
void I2CTransceiver_LoopCallback(I2cTranceiverHandlePtr_T i2cTransceiver, struct MCU_I2C_Event_S event)
{
    Retcode_T retcode = RETCODE_OK;
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    if (NULL == i2cTransceiver)
    { /* Handle is not initialized */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    if (RETCODE_OK == retcode)
    {
        if (false == i2cTransceiver->InitializationStatus)
        { /* Transciever is not initialized */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
        }
        else if (NULL == i2cTransceiver->I2CBusSync)
        { /* I2C Bus lock not present */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        }
    }

    if (RETCODE_OK == retcode)
    {
        if (UINT8_C(1) == event.TxComplete)
        {
            i2cTransceiver->I2cTransferStatusFlag = INT8_C(0); // No Error in I2C Transfer Flagged
        }

        else if (UINT8_C(1) == event.TransferError)
        {
            i2cTransceiver->I2cTransferStatusFlag = INT8_C(-1); //  Error in I2C Transfer Flagged
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_I2CTRANSCEIVER_TRANSFER_ERROR);
        }
        /* Give Semaphore even in case of transfer errors or Transfer completed */
        if (pdTRUE == xSemaphoreGiveFromISR(i2cTransceiver->I2CBusSync, &higherPriorityTaskWoken))
        {
            portYIELD_FROM_ISR(higherPriorityTaskWoken);
        }
        else
        {
            /* ignore... semaphore has already been given */
        }
    }
    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseErrorFromIsr(retcode);
    }
}

/*  The description of the function is available in Kiso_I2CTransceiver.h */
Retcode_T I2CTransceiver_Init(I2cTranceiverHandlePtr_T i2cTransceiver, I2C_T i2cHandle)
{
    Retcode_T retcode = RETCODE_OK;
    if ((NULL == i2cTransceiver) || (NULL == i2cHandle))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        if (false == i2cTransceiver->InitializationStatus)
        {

            i2cTransceiver->I2CHandle = i2cHandle;
            i2cTransceiver->I2CBusSync = xSemaphoreCreateBinary();
            i2cTransceiver->I2CMutexLock = xSemaphoreCreateMutex();
            if ((NULL != i2cTransceiver->I2CBusSync) && (NULL != i2cTransceiver->I2CMutexLock))
            {
                i2cTransceiver->InitializationStatus = true;
            }
            else if ((NULL != i2cTransceiver->I2CBusSync) && (NULL == i2cTransceiver->I2CMutexLock))
            {
                vSemaphoreDelete(i2cTransceiver->I2CBusSync);
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }
            else if ((NULL == i2cTransceiver->I2CBusSync) && (NULL != i2cTransceiver->I2CMutexLock))
            {
                vSemaphoreDelete(i2cTransceiver->I2CMutexLock);
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }
        }
        else
        {
            retcode = RETCODE_OK;

        }
    }

    return retcode;
}

/*  The description of the function is available in Kiso_I2CTransceiver.h */
Retcode_T I2CTransceiver_Read(I2cTranceiverHandlePtr_T i2cTransceiver, uint8_t i2cAddr,uint8_t regAddr,uint8_t *buffer,uint8_t bytesToRead)
{
    Retcode_T retcode = RETCODE_OK;

    if (UINT8_C(0) == bytesToRead)
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM));
    }

    if ((NULL == i2cTransceiver) || (NULL == buffer) || (NULL == i2cTransceiver->I2CBusSync) || (NULL == i2cTransceiver->I2CMutexLock) || (NULL == i2cTransceiver->I2CHandle))
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER));
    }

    if (false == i2cTransceiver->InitializationStatus)
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED));
    }
    if (pdTRUE != xSemaphoreTake(i2cTransceiver->I2CMutexLock, (TickType_t) pdMS_TO_TICKS(DATA_TRANSFER_TIMEOUT_MS)))
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR));
    }
    retcode = MCU_I2C_ReadRegister(i2cTransceiver->I2CHandle, (uint16_t) i2cAddr, regAddr, buffer, bytesToRead);
    if (RETCODE_OK == retcode)
    {
        if (pdTRUE != xSemaphoreTake(i2cTransceiver->I2CBusSync, (TickType_t) pdMS_TO_TICKS(DATA_TRANSFER_TIMEOUT_MS)))
        {
            /* Since the I2C transfer time out happened, Abort an ongoing I2C transmission.*/
            retcode = MCU_I2C_Send(i2cTransceiver->I2CHandle, (uint16_t) i2cAddr, buffer, CANCEL_I2C_TRANSMISSION);
            if (RETCODE_OK == retcode)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT);
            }
        }
        else
        {
            if (INT8_C(-1) == i2cTransceiver->I2cTransferStatusFlag)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_I2CTRANSCEIVER_TRANSFER_ERROR);
            }
        }
    }
    if (pdTRUE != xSemaphoreGive(i2cTransceiver->I2CMutexLock))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_SEMAPHORE_ERROR);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_I2CTransceiver.h */
Retcode_T I2CTransceiver_Write(I2cTranceiverHandlePtr_T i2cTransceiver, uint8_t i2cAddr, uint8_t regAddr, uint8_t *buffer, uint8_t bytesToWrite)
{
    Retcode_T retcode = RETCODE_OK;
    if (UINT8_C(0) == bytesToWrite)
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM));
    }

    if ((NULL == i2cTransceiver) || (NULL == buffer) || (NULL == i2cTransceiver->I2CBusSync) || (NULL == i2cTransceiver->I2CMutexLock) || (NULL == i2cTransceiver->I2CHandle))
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER));
    }

    if (false == i2cTransceiver->InitializationStatus)
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED));
    }
    if (pdTRUE != xSemaphoreTake(i2cTransceiver->I2CMutexLock, (TickType_t) pdMS_TO_TICKS(DATA_TRANSFER_TIMEOUT_MS)))
    {
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR));
    }
    retcode = MCU_I2C_WriteRegister(i2cTransceiver->I2CHandle, (uint16_t) i2cAddr, regAddr, buffer, bytesToWrite);
    if (RETCODE_OK == retcode)
    {
        if (pdTRUE != xSemaphoreTake(i2cTransceiver->I2CBusSync, (TickType_t) pdMS_TO_TICKS(DATA_TRANSFER_TIMEOUT_MS)))
        {
            /* Since the I2C transfer time out happened, Abort an ongoing I2C transmission.*/
            retcode = MCU_I2C_Send(i2cTransceiver->I2CHandle, (uint16_t) i2cAddr, buffer, CANCEL_I2C_TRANSMISSION);
            if (RETCODE_OK == retcode)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT);
            }
        }
        else
        {
            if (INT8_C(-1) == i2cTransceiver->I2cTransferStatusFlag)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_I2CTRANSCEIVER_TRANSFER_ERROR);
            }
        }
    }
    if (pdTRUE != xSemaphoreGive(i2cTransceiver->I2CMutexLock))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_SEMAPHORE_ERROR);
    }

    return retcode;
}

/*  The description of the function is available in Kiso_I2CTransceiver.h */
Retcode_T I2CTransceiver_Deinit(I2cTranceiverHandlePtr_T i2cTransceiver)
{
    Retcode_T retcode = RETCODE_OK;

    if ((NULL == i2cTransceiver) || (NULL == i2cTransceiver->I2CBusSync) || (NULL == i2cTransceiver->I2CMutexLock) || (NULL == i2cTransceiver->I2CHandle))
    { /* Handle is not initialized */
        return (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER));
    }
    if (false == i2cTransceiver->InitializationStatus)
    {
        return (RETCODE_OK);
    }

    i2cTransceiver->I2CHandle=NULL;
    vSemaphoreDelete(i2cTransceiver->I2CBusSync);
    i2cTransceiver->I2CBusSync = NULL;
    vSemaphoreDelete(i2cTransceiver->I2CMutexLock);
    i2cTransceiver->I2CMutexLock = NULL;
    i2cTransceiver->InitializationStatus = false;
    return retcode;
}

#endif /* KISO_FEATURE_I2C */

#endif /* if KISO_FEATURE_I2CTRANSCEIVER */
