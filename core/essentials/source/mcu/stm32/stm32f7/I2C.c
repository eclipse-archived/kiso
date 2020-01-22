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
 * @file
 * @brief Contains the realization of the MCU I2C interface for STM32L4
 */
/*---------------------- INCLUDED HEADERS ---------------------------------------------------------------------------*/

#include "Kiso_MCU_I2C.h"

#if KISO_FEATURE_I2C

#include "Kiso_MCU_STM32F7_I2C_Handle.h"
#include "stm32f7xx_hal.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_I2C

#define I2C_DEFAULT_TIMEOUT_MS UINT32_C(1000)  /* Default timeout in ms */
#define I2C_MIN_TIMEOUT_MS UINT32_C(100)       /* Minimal timeout in ms*/
#define I2C_SECOND_TO_MILLI_MS UINT32_C(1000)  /* Convert one second to millisecond*/
#define I2C_SAFETY_FACTOR UINT32_C(2)          /* factor to multiply with the reulting time value for safety */
#define I2C_ADDITIONAL_BYTES UINT32_C(2)       /* at max, additionally one byte for slave address and one byte for register address  */
#define I2C_CLOCK_CYCLES_PER_BYTE UINT32_C(10) /* an 8 bit value + 2 bits i2c protocol overhead(start, stop, ack, nack ) */
#define I2C_MAX_NBYTES UINT32_C(255)           /* maximum bytes to send per i2c cycle a reload is necessary if the data to tranfer exceeds this value*/
/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

static void I2C_EventCallback(I2C_T i2c);
static void I2C_ErrorCallback(I2C_T i2c);
static void I2C_DmaTxHandler(I2C_T i2c);
static void I2C_DmaRxHandler(I2C_T i2c);

static Retcode_T I2C_SendPollMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_ReceivePollMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_CancelPollMode(struct MCU_I2C_S *pi2c);

static Retcode_T I2C_SendIntMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_ReceiveIntMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_ReadRegisterIntMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_WriteRegisterIntMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_CancelIntMode(struct MCU_I2C_S *pi2c);

static Retcode_T I2C_SendDmaMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_ReceiveDmaMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_ReadRegisterDmaMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_WriteRegisterDmaMode(struct MCU_I2C_S *pi2c);
static Retcode_T I2C_CancelDmaMode(struct MCU_I2C_S *pi2c);

static void I2C_WriteControlWord(I2C_TypeDef *i2c, uint16_t slaveAddress, uint8_t nBytes, uint32_t mode, uint32_t req);
static Retcode_T I2C_WaitUntilTimeout_ms(struct MCU_I2C_S *pi2c, bool (*waitCondition)(struct MCU_I2C_S *pi2c), uint32_t timeout);
static bool I2C_IsI2CNotReady(struct MCU_I2C_S *pi2c);
static bool I2C_IsTXISReset(struct MCU_I2C_S *pi2c);
static bool I2C_IsNACKSet(struct MCU_I2C_S *pi2c);
static bool I2C_IsSTOPFReset(struct MCU_I2C_S *pi2c);
static bool I2C_IsTCRReset(struct MCU_I2C_S *pi2c);
static bool I2C_IsBUSYSet(struct MCU_I2C_S *pi2c);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/** See description in the interface declaration */
Retcode_T MCU_I2C_Initialize(I2C_T i2c, MCU_I2C_Callback_T callback)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if (NULL == pI2C)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (!IS_I2C_ALL_INSTANCE(pI2C->hi2c.Instance))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_I2C_INVALID_PERIPHERAL);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (pI2C->State == I2C_STATE_INIT)
        {
            /* Now we have to check the configuration of the interface in order to assign the correct transmit and
             * receive functions depending on the configured transfer mode.
             */
            switch (pI2C->TransferMode)
            {
            case KISO_HAL_TRANSFER_MODE_POLLING:
                pI2C->IRQCallback = NULL;
                pI2C->ERRCallback = NULL;
                pI2C->DMATxCallback = NULL;
                pI2C->DMARxCallback = NULL;
                pI2C->AppLayerCallback = NULL;
                pI2C->SendFunPtr = I2C_SendPollMode;
                pI2C->ReceiveFunPtr = I2C_ReceivePollMode;
                pI2C->ReadRegisterFunPtr = I2C_ReceivePollMode;
                pI2C->WriteRegisterFunPtr = I2C_SendPollMode;
                pI2C->CancelFunPtr = I2C_CancelPollMode;
                pI2C->State = I2C_STATE_READY;
                break;

            case KISO_HAL_TRANSFER_MODE_INTERRUPT:
                if (NULL != callback)
                {
                    pI2C->IRQCallback = I2C_EventCallback;
                    pI2C->ERRCallback = I2C_ErrorCallback;
                    pI2C->DMATxCallback = NULL;
                    pI2C->DMARxCallback = NULL;
                    pI2C->AppLayerCallback = callback;
                    pI2C->SendFunPtr = I2C_SendIntMode;
                    pI2C->ReceiveFunPtr = I2C_ReceiveIntMode;
                    pI2C->ReadRegisterFunPtr = I2C_ReadRegisterIntMode;
                    pI2C->WriteRegisterFunPtr = I2C_WriteRegisterIntMode;
                    pI2C->CancelFunPtr = I2C_CancelIntMode;
                    pI2C->State = I2C_STATE_READY;
                }
                else
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                }
                break;

            case KISO_HAL_TRANSFER_MODE_DMA:
                if (NULL != callback)
                {
                    pI2C->IRQCallback = I2C_EventCallback;
                    pI2C->ERRCallback = I2C_ErrorCallback;
                    pI2C->DMATxCallback = I2C_DmaTxHandler;
                    pI2C->DMARxCallback = I2C_DmaRxHandler;
                    pI2C->AppLayerCallback = callback;
                    pI2C->SendFunPtr = I2C_SendDmaMode;
                    pI2C->ReceiveFunPtr = I2C_ReceiveDmaMode;
                    pI2C->ReadRegisterFunPtr = I2C_ReadRegisterDmaMode;
                    pI2C->WriteRegisterFunPtr = I2C_WriteRegisterDmaMode;
                    pI2C->CancelFunPtr = I2C_CancelDmaMode;
                    pI2C->State = I2C_STATE_READY;
                }
                else
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                }
                break;

            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
                break;
            }
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_I2C_Deinitialize(I2C_T i2c)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;

    if (NULL == pI2C)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (pI2C->State != I2C_STATE_READY)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        pI2C->State = I2C_STATE_INIT;
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_I2C_Send(I2C_T i2c, uint16_t slaveAddr, uint8_t *data, uint32_t len)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if ((NULL == pI2C) || (data == NULL) || (len > UINT16_MAX))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (0 != len)
        {
            if (pI2C->State == I2C_STATE_READY)
            {
                pI2C->Transaction.DevAddress = slaveAddr;
                pI2C->Transaction.pDataBuffer = data;
                pI2C->Transaction.Size = (uint16_t)len;
                pI2C->Transaction.PrependRegAddr = false;
                pI2C->Transaction.RegisterAddr = 0;
                retcode = pI2C->SendFunPtr(pI2C);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (pI2C->State == I2C_STATE_TX)
            {
                retcode = pI2C->CancelFunPtr(pI2C);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_I2C_Receive(I2C_T i2c, uint16_t slaveAddr, uint8_t *buffer, uint32_t len)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if ((NULL == pI2C) || (buffer == NULL) || (len > UINT16_MAX))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (0 != len)
        {
            if (pI2C->State == I2C_STATE_READY)
            {
                pI2C->Transaction.DevAddress = slaveAddr;
                pI2C->Transaction.pDataBuffer = buffer;
                pI2C->Transaction.Size = (uint16_t)len;
                pI2C->Transaction.PrependRegAddr = false;
                pI2C->Transaction.RegisterAddr = 0;

                retcode = pI2C->ReceiveFunPtr(pI2C);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (pI2C->State == I2C_STATE_RX)
            {
                retcode = pI2C->CancelFunPtr(pI2C);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_I2C_WriteRegister(I2C_T i2c, uint16_t slaveAddr, uint8_t registerAddr, uint8_t *data, uint32_t len)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if ((NULL == pI2C) || (data == NULL) || (len > UINT16_MAX))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (0 != len)
        {
            if (pI2C->State == I2C_STATE_READY)
            {
                pI2C->Transaction.DevAddress = slaveAddr;
                pI2C->Transaction.pDataBuffer = data;
                pI2C->Transaction.Size = (uint16_t)len;
                pI2C->Transaction.PrependRegAddr = true;
                pI2C->Transaction.RegisterAddr = registerAddr;
                retcode = pI2C->WriteRegisterFunPtr(pI2C);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (pI2C->State == I2C_STATE_TX)
            {
                retcode = pI2C->CancelFunPtr(pI2C);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
    }
    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_I2C_ReadRegister(I2C_T i2c, uint16_t slaveAddr, uint8_t registerAddr, uint8_t *buffer, uint32_t len)
{
    Retcode_T retcode = RETCODE_OK;
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if ((NULL == pI2C) || (buffer == NULL) || (len > UINT16_MAX))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (0 != len)
        {
            if (pI2C->State == I2C_STATE_READY)
            {
                pI2C->Transaction.DevAddress = slaveAddr;
                pI2C->Transaction.pDataBuffer = buffer;
                pI2C->Transaction.Size = (uint16_t)len;
                pI2C->Transaction.PrependRegAddr = true;
                pI2C->Transaction.RegisterAddr = registerAddr;

                retcode = pI2C->ReadRegisterFunPtr(pI2C);
            }
            else
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        else
        {
            if (pI2C->State == I2C_STATE_RX)
            {
                retcode = pI2C->CancelFunPtr(pI2C);
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
 * @details     This function is a custom polling send function which allows prepending a register address before
 *              transmitting the rest of the data
 * @param       pi2c reference to the I2C control structure.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_TIMEOUT in case of timeout
 * @retval      RETCODE_MCU_I2C_NACK in case of unexpected NACK reception
 */
static Retcode_T I2C_SendPollMode(struct MCU_I2C_S *pi2c)
{
    pi2c->State = I2C_STATE_TX;

    Retcode_T retcode;
    uint16_t slaveAddr = pi2c->Transaction.DevAddress;

    uint32_t timeout = I2C_DEFAULT_TIMEOUT_MS;
    if (pi2c->DataRate)
    {
        timeout +=
            (((pi2c->Transaction.Size + I2C_ADDITIONAL_BYTES) * I2C_CLOCK_CYCLES_PER_BYTE * I2C_SAFETY_FACTOR) / (1 + (pi2c->DataRate / I2C_SECOND_TO_MILLI_MS)));
    }
    if (I2C_ADDRESSINGMODE_7BIT == pi2c->hi2c.Init.AddressingMode)
    {
        slaveAddr = slaveAddr << 1; /**< 7bit Adressing Mode */
    }

    /* Wait if the bus is still busy */
    retcode = I2C_WaitUntilTimeout_ms(pi2c, I2C_IsBUSYSet, timeout);

    if (RETCODE_OK == retcode)
    {
        /* Prepare and initiate transfer */
        pi2c->hi2c.pBuffPtr = pi2c->Transaction.pDataBuffer;
        pi2c->hi2c.XferCount = pi2c->Transaction.Size;
        pi2c->hi2c.XferISR = NULL;
        /* add one to the total count if we want to send register address */
        if (pi2c->Transaction.PrependRegAddr)
        {
            pi2c->hi2c.XferCount++;
        }

        /* check if the total size needs reloading to complete to transaction */
        if (pi2c->hi2c.XferCount > I2C_MAX_NBYTES)
        {
            /* reloading is needed, write in 255 byte chunks*/
            pi2c->hi2c.XferSize = I2C_MAX_NBYTES;
            I2C_WriteControlWord(pi2c->hi2c.Instance, slaveAddr, pi2c->hi2c.XferSize, I2C_RELOAD_MODE, I2C_GENERATE_START_WRITE);
        }
        else
        {
            /* No reloading is needed */
            pi2c->hi2c.XferSize = pi2c->hi2c.XferCount;
            I2C_WriteControlWord(pi2c->hi2c.Instance, slaveAddr, pi2c->hi2c.XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_WRITE);
        }

        /* loop sending the data */
        while (pi2c->hi2c.XferCount > 0U)
        {
            /* Wait until TXIS flag is set i.e. the previous byte (including device address) has been sent */
            retcode = I2C_WaitUntilTimeout_ms(pi2c, I2C_IsTXISReset, timeout);
            if (RETCODE_OK != retcode)
            {
                /* check if NACK has been received */
                if (I2C_IsNACKSet(pi2c))
                {
                    /* Wait for STOP to be sent */
                    (void)I2C_WaitUntilTimeout_ms(pi2c, I2C_IsSTOPFReset, timeout);
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_MCU_I2C_NACK);
                }
                /* exit the loop once an error is detected */
                break;
            }
            /* check if prepending a register address is requested */
            if (pi2c->Transaction.PrependRegAddr)
            {
                pi2c->hi2c.Instance->TXDR = pi2c->Transaction.RegisterAddr;
                pi2c->Transaction.PrependRegAddr = false;
            }
            else
            {
                pi2c->hi2c.Instance->TXDR = (*pi2c->hi2c.pBuffPtr++);
            }

            pi2c->hi2c.XferCount--;
            pi2c->hi2c.XferSize--;

            /* check if reloading is neeed */
            if ((pi2c->hi2c.XferSize == 0U) && (pi2c->hi2c.XferCount != 0U))
            {
                /* Wait until TCR flag is set */
                retcode = I2C_WaitUntilTimeout_ms(pi2c, I2C_IsTCRReset, timeout);
                if (RETCODE_OK == retcode)
                {
                    if (pi2c->hi2c.XferCount > I2C_MAX_NBYTES)
                    {
                        pi2c->hi2c.XferSize = I2C_MAX_NBYTES;
                        I2C_WriteControlWord(pi2c->hi2c.Instance, slaveAddr, pi2c->hi2c.XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
                    }
                    else
                    {
                        pi2c->hi2c.XferSize = pi2c->hi2c.XferCount;
                        I2C_WriteControlWord(pi2c->hi2c.Instance, slaveAddr, pi2c->hi2c.XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
                    }
                }
            }
        }
    }
    if (RETCODE_OK == retcode)
    {
        /* Transcation is complete Wait until STOP flag is set */
        retcode = I2C_WaitUntilTimeout_ms(pi2c, I2C_IsSTOPFReset, timeout);
    }
    if (RETCODE_OK == retcode)
    {
        /* Clear STOP Flag */
        __HAL_I2C_CLEAR_FLAG(&pi2c->hi2c, I2C_FLAG_STOPF);
    }
    /* Clear Configuration Register 2 */
    I2C_RESET_CR2(&pi2c->hi2c);

    pi2c->State = I2C_STATE_READY;

    return retcode;
}

/**
 * @details     This function is a polling receive function which allows prepending a register address before
 *              transmitting the rest of the data
 * @param       pi2c reference to the I2C control structure.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_FAILURE in case of STM32 HAL Library failure
 */
Retcode_T I2C_ReceivePollMode(struct MCU_I2C_S *pI2C)
{
    pI2C->State = I2C_STATE_RX;

    Retcode_T retcode = RETCODE_OK;
    uint16_t slaveAddr = pI2C->Transaction.DevAddress;
    uint32_t timeout = I2C_DEFAULT_TIMEOUT_MS;

    if (pI2C->DataRate)
    {
        timeout +=
            (((pI2C->Transaction.Size + I2C_ADDITIONAL_BYTES) * I2C_CLOCK_CYCLES_PER_BYTE * I2C_SAFETY_FACTOR) / (1 + pI2C->DataRate / I2C_SECOND_TO_MILLI_MS));
    }

    if (I2C_ADDRESSINGMODE_7BIT == pI2C->hi2c.Init.AddressingMode)
    {
        slaveAddr = slaveAddr << 1; /**< 7bit Adressing Mode */
    }
    if (pI2C->Transaction.PrependRegAddr)
    {
        if (HAL_OK != HAL_I2C_Master_Transmit(&pI2C->hi2c, slaveAddr, &pI2C->Transaction.RegisterAddr, 1, timeout))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_I2C_Master_Receive(&pI2C->hi2c, slaveAddr, pI2C->Transaction.pDataBuffer, pI2C->Transaction.Size, timeout))
        {
            /* roll back the state and return error*/
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    pI2C->State = I2C_STATE_READY;
    return retcode;
}

/**
 * @brief       An ongoing polling operation cannot be canceled
 * @param       pi2c irrelevant parameter
 * @return      RETCODE_OK always
 */
static Retcode_T I2C_CancelPollMode(struct MCU_I2C_S *pi2c)
{
    KISO_UNUSED(pi2c);
    return RETCODE_OK;
}

/**
 * @brief       Sends data in interrupt mode
 * @details     This function relies on STM32HAL library to send data via I2C in interrupt mode
 * @param       pI2C pointer to the I2C control block (structure)
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_FAILURE in case HAL transmit operation failure
 */
Retcode_T I2C_SendIntMode(struct MCU_I2C_S *pI2C)
{
    pI2C->State = I2C_STATE_TX;

    Retcode_T retcode = RETCODE_OK;
    uint16_t slaveAddress = pI2C->Transaction.DevAddress;

    if (I2C_ADDRESSINGMODE_7BIT == pI2C->hi2c.Init.AddressingMode)
    {
        slaveAddress = slaveAddress << 1; /**< 7bit Addressing Mode */
    }

    if (HAL_OK != HAL_I2C_Master_Transmit_IT(&pI2C->hi2c, slaveAddress, pI2C->Transaction.pDataBuffer, pI2C->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        pI2C->State = I2C_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Receives data in interrupt mode
 * @details     This function relies on STM32HAL library to receive data via I2C in interrupt mode
 * @param       pI2C reference to the I2C control block (structure)
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_FAILURE in case HAL transmit operation failure
 */
Retcode_T I2C_ReceiveIntMode(struct MCU_I2C_S *pI2C)
{
    pI2C->State = I2C_STATE_RX;

    Retcode_T retcode = RETCODE_OK;
    uint16_t slaveAddress = pI2C->Transaction.DevAddress;

    if (I2C_ADDRESSINGMODE_7BIT == pI2C->hi2c.Init.AddressingMode)
    {
        slaveAddress = slaveAddress << 1; /**< 7bit Addressing Mode */
    }

    if (HAL_OK != HAL_I2C_Master_Receive_IT(&pI2C->hi2c, slaveAddress, pI2C->Transaction.pDataBuffer, pI2C->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        pI2C->State = I2C_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Writes to a device register in interrupt mode
 * @details     This function prepends a register address for write operation before wirting a data buffer to that
 *              address.
 * @param       pI2C refernce to the I2C control block (structure).
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_FAILURE in case HAL transmit operation failure
 */
Retcode_T I2C_WriteRegisterIntMode(struct MCU_I2C_S *pI2C)
{
    pI2C->State = I2C_STATE_TX;
    uint32_t timeout = I2C_DEFAULT_TIMEOUT_MS;

    if (pI2C->DataRate)
    {
        timeout +=
            (((I2C_ADDITIONAL_BYTES)*I2C_CLOCK_CYCLES_PER_BYTE * I2C_SAFETY_FACTOR) / (1 + (pI2C->DataRate / I2C_SECOND_TO_MILLI_MS)));
    }
    Retcode_T retcode = RETCODE_OK;
    uint16_t slaveAddr = pI2C->Transaction.DevAddress;

    if (I2C_ADDRESSINGMODE_7BIT == pI2C->hi2c.Init.AddressingMode)
    {
        slaveAddr = slaveAddr << 1; /**< 7bit Adressing Mode */
    }
    if (HAL_OK != HAL_I2C_Master_Seq_Transmit_IT(&pI2C->hi2c, slaveAddr, &pI2C->Transaction.RegisterAddr, 1, I2C_RELOAD_MODE))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    if (RETCODE_OK == retcode)
    {
        /* here we actively wait for the completion of the previous register address sending because we do not have other
         * synchronization option */
        retcode = I2C_WaitUntilTimeout_ms(pI2C, I2C_IsI2CNotReady, timeout);
    }
    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_I2C_Master_Seq_Transmit_IT(&pI2C->hi2c, slaveAddr, pI2C->Transaction.pDataBuffer, pI2C->Transaction.Size, I2C_LAST_FRAME))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    if (RETCODE_OK != retcode)
    {
        pI2C->State = I2C_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Reads a register and returns its value into the provided buffer.
 * @details     The implementation will write the register address in polling mode first and then reads the values from
 *              the register in interrupt mode.
 * @param       pI2C reference to the i2c control block.
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_FAILURE in case of HAL library error
 *
 */
Retcode_T I2C_ReadRegisterIntMode(struct MCU_I2C_S *pI2C)
{
    pI2C->State = I2C_STATE_RX;
    Retcode_T retcode = RETCODE_OK;
    uint16_t slaveAddr = pI2C->Transaction.DevAddress;
    uint32_t timeout = I2C_DEFAULT_TIMEOUT_MS;

    if (pI2C->DataRate)
    {
        timeout +=
            (((pI2C->Transaction.Size + I2C_ADDITIONAL_BYTES) * I2C_CLOCK_CYCLES_PER_BYTE * I2C_SAFETY_FACTOR) / (1 + (pI2C->DataRate / I2C_SECOND_TO_MILLI_MS)));
    }

    if (I2C_ADDRESSINGMODE_7BIT == pI2C->hi2c.Init.AddressingMode)
    {
        slaveAddr = slaveAddr << 1; /**< 7bit Adressing Mode */
    }
    /* write first the register address in polling mode.*/
    if (HAL_OK != HAL_I2C_Master_Transmit(&pI2C->hi2c, slaveAddr, &pI2C->Transaction.RegisterAddr, 1, timeout))
    {
        pI2C->State = I2C_STATE_READY;
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    if (RETCODE_OK == retcode)
    {
        /* Read the requested data in interrupt mode */
        if (HAL_OK != HAL_I2C_Master_Receive_IT(&pI2C->hi2c, slaveAddr, pI2C->Transaction.pDataBuffer, pI2C->Transaction.Size))
        {
            /* roll back the state and return error*/
            pI2C->State = I2C_STATE_READY;
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    return retcode;
}

/**
 * @brief   Cancels ongoing interrupt mode transaction
 * @param   pi2c reference to the I2C control block.
 * @return  RETCODE_NOT_SUPPORTED
 */
static Retcode_T I2C_CancelIntMode(struct MCU_I2C_S *pi2c)
{
    KISO_UNUSED(pi2c);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * @brief       Sends data in interrupt mode
 * @details     This function relies on STM32HAL library to send data via I2C using DMA
 * @param       pI2C pointer to the I2C control block (structure)
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_FAILURE in case HAL transmit operation failure
 */
Retcode_T I2C_SendDmaMode(struct MCU_I2C_S *pI2C)
{
    pI2C->State = I2C_STATE_TX;
    Retcode_T retcode = RETCODE_OK;
    uint16_t slaveAddress = pI2C->Transaction.DevAddress;

    if (I2C_ADDRESSINGMODE_7BIT == pI2C->hi2c.Init.AddressingMode)
    {
        slaveAddress = slaveAddress << 1; /**< 7bit Addressing Mode */
    }

    if (HAL_OK != HAL_I2C_Master_Transmit_DMA(&pI2C->hi2c, slaveAddress, pI2C->Transaction.pDataBuffer, (pI2C->Transaction).Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        pI2C->State = I2C_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Receives data in interrupt mode
 * @details     This function relies on STM32HAL library to receive data via I2C in dma mode
 * @param       pI2C reference to the I2C control block (structure)
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_FAILURE in case HAL transmit operation failure
 */
Retcode_T I2C_ReceiveDmaMode(struct MCU_I2C_S *pI2C)
{
    pI2C->State = I2C_STATE_RX;
    Retcode_T retcode = RETCODE_OK;
    uint16_t slaveAddress = pI2C->Transaction.DevAddress;

    if (I2C_ADDRESSINGMODE_7BIT == pI2C->hi2c.Init.AddressingMode)
    {
        slaveAddress = slaveAddress << 1; /**< 7bit Addressing Mode */
    }

    if (HAL_OK != HAL_I2C_Master_Receive_DMA(&pI2C->hi2c, slaveAddress, pI2C->Transaction.pDataBuffer, pI2C->Transaction.Size))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        pI2C->State = I2C_STATE_READY;
    }
    return retcode;
}

/**
 * @brief       Writes to a device register in dma mode
 * @details     This function prepends a register address for write operation before wirting a data buffer to that
 *              address.
 * @param       pI2C refernce to the I2C control block (structure).
 * @retval      RETCODE_OK in case of success
 * @retval      RETCODE_FAILURE in case HAL transmit operation failure
 */
Retcode_T I2C_WriteRegisterDmaMode(struct MCU_I2C_S *pI2C)
{
    KISO_UNUSED(pI2C);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 *
 * @param pI2C
 * @return
 */
Retcode_T I2C_ReadRegisterDmaMode(struct MCU_I2C_S *pI2C)
{
    pI2C->State = I2C_STATE_RX;
    Retcode_T retcode = RETCODE_OK;
    uint16_t slaveAddr = pI2C->Transaction.DevAddress;
    uint32_t timeout = I2C_DEFAULT_TIMEOUT_MS;

    if (pI2C->DataRate)
    {
        timeout +=
            (((pI2C->Transaction.Size + I2C_ADDITIONAL_BYTES) * I2C_CLOCK_CYCLES_PER_BYTE * I2C_SAFETY_FACTOR) / (1 + (pI2C->DataRate / I2C_SECOND_TO_MILLI_MS)));
    }

    if (I2C_ADDRESSINGMODE_7BIT == pI2C->hi2c.Init.AddressingMode)
    {
        slaveAddr = slaveAddr << 1; /**< 7bit Adressing Mode */
    }

    if (HAL_OK != HAL_I2C_Master_Transmit(&pI2C->hi2c, slaveAddr, &pI2C->Transaction.RegisterAddr, 1, timeout))
    {
        pI2C->State = I2C_STATE_READY;
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_I2C_Master_Receive_DMA(&pI2C->hi2c, slaveAddr, pI2C->Transaction.pDataBuffer, pI2C->Transaction.Size))
        {
            /* roll back the state and return error*/
            pI2C->State = I2C_STATE_READY;
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
    return retcode;
}

/**
 * @brief   Cancels ongoing DMA mode transaction
 * @param   pi2c reference to the I2C control block.
 * @return  RETCODE_NOT_SUPPORTED
 */
static Retcode_T I2C_CancelDmaMode(struct MCU_I2C_S *pi2c)
{
    KISO_UNUSED(pi2c);
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/**
 * @brief       I2C EV event interrupt service routine
 * @details     This handler is called upon occurrence of any communication event interrupt request on the I2C bus. This
 *              relates to communication event only.
 * @param       i2c is a reference to the I2C control block (structure) generally hosted by the BSP.
 */
void I2C_EventCallback(I2C_T i2c)
{
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if (pI2C)
    {
        HAL_I2C_EV_IRQHandler(&pI2C->hi2c);
    }
}

/**
 * @brief       I2C ER event interrupt service routine
 * @details     This handler is called upon occurrence of an interrupt request on the I2C bus, it relates to error
 *              events only.
 * @param       i2c is a reference to the I2C control block (structure) generally hosted by the BSP.
 */
void I2C_ErrorCallback(I2C_T i2c)
{
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if (pI2C)
    {
        HAL_I2C_ER_IRQHandler(&pI2C->hi2c);
    }
}

/**
 * @brief       I2C DMA Rx Interrupt service routine.
 * @details     in case DMA is chosen as transfer mode this functions gets called by the DMA Rx channel IRQ handler
 *              assigned to the I2C resource in order to process DMA events (transfer complete, error ...)
 * @param       i2c is a reference to the I2C control block (structure) generally hosted by the BSP.
 */
void I2C_DmaRxHandler(I2C_T i2c)
{
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if (pI2C)
    {
        HAL_DMA_IRQHandler(pI2C->hi2c.hdmarx);
    }
}

/**
 * @brief       I2C DMA Tx Interrupt service routine.
 * @details     in case DMA is chosen as transfer mode this functions gets called by the DMA Tx channel IRQ handler
 *              assigned to the I2C resource in order to process DMA events (transfer complete, error ...)
 * @param       i2c is a reference to the I2C control block (structure) generally hosted by the BSP.
 */
void I2C_DmaTxHandler(I2C_T i2c)
{
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)i2c;
    if (pI2C)
    {
        HAL_DMA_IRQHandler(pI2C->hi2c.hdmatx);
    }
}
/**
 * @brief       Callback function for I2C error events
 * @details     This function overrides the weak definition provided by the STM32
 *              HAL and is called upon every error interrupt request occurring on a
 *              particular I2C interface.
 * @param       hi2c is the STM32 HAL handle of the interface that reports the error
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)hi2c;
    if (pI2C)
    {
        /* Has a valid handle, now process the event */
        /* First read the error value from the I2C device. This value is
         * bit coded.
         */
        uint32_t errorValue = HAL_I2C_GetError(hi2c);
        /* Then create the return value for the callback to the application if
         * an callback pointer is assigned.
         */
        if ((HAL_I2C_ERROR_NONE != errorValue) && pI2C->AppLayerCallback)
        {
            struct MCU_I2C_Event_S event = {0, 0, 0, 0};
            /* Set the error bit in the event and signal it to application */
            event.TransferError = 1;
            pI2C->AppLayerCallback((I2C_T)pI2C, event);
        }
    }
}

/**
 * @brief       Callback function for I2C master TX complete events.
 * @details     This function overrides the weak definition provided by the STM32
 *              HAL and is called when a Tx transfer is completed.
 * @param       hi2c is the STM32 HAL handle of the interface that reports the event
 */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)hi2c;
    if (pI2C)
    {
        if (pI2C->State == I2C_STATE_TX)
        {
            if (pI2C->Transaction.PrependRegAddr)
            {
                /* the call is assumed to be concerning register address sending completion */
                pI2C->Transaction.PrependRegAddr = false;
            }
            else
            {
                /* Has a valid handle, now process the event */
                struct MCU_I2C_Event_S event = {0, 0, 0, 0};
                /* Signal that transfer is complete to application */
                event.TxComplete = 1;
                pI2C->AppLayerCallback((I2C_T)pI2C, event);
                pI2C->State = I2C_STATE_READY;
            }
        }
    }
}

/**
 * @brief       Callback function for I2C master RX complete events.
 * @details     This function overrides the weak definition provided by the STM32
 *              HAL and is called when a Rx transfer is completed.
 * @param       hi2c is the STM32 HAL handle of the interface that reports the event
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    struct MCU_I2C_S *pI2C = (struct MCU_I2C_S *)hi2c;
    if (pI2C && pI2C->AppLayerCallback)
    {
        if (pI2C->State == I2C_STATE_RX)
        {
            /* Has a valid handle, now process the event */
            struct MCU_I2C_Event_S event = {0, 0, 0, 0};
            /* Signal that transfer is complete to application */
            event.RxComplete = 1;
            pI2C->AppLayerCallback((I2C_T)pI2C, event);
            pI2C->State = I2C_STATE_READY;
        }
    }
}

/**
 * @brief       Writes the transaction control word to the I2C_CR2 register
 * @param       i2c reference to the I2C base register
 * @param       slaveAddress Value to be written to SADD field
 * @param       nBytes Value to be written to NBYTES field
 * @param       mode Reload, Softend or Autoend
 * @param       req Start, Stop, ACK, NACK
 */
static void I2C_WriteControlWord(I2C_TypeDef *i2c, uint16_t slaveAddress, uint8_t nBytes, uint32_t mode, uint32_t req)
{

    uint32_t tmp = i2c->CR2;

    /* clear CR2 specific bits */
    tmp &= (uint32_t) ~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));

    /* update value */
    tmp |= (uint32_t)(((uint32_t)slaveAddress & I2C_CR2_SADD) | (((uint32_t)nBytes << 16) & I2C_CR2_NBYTES) |
                      (uint32_t)mode | (uint32_t)req);

    /* write to register */
    i2c->CR2 = tmp;
}
/**
 * @brief       Waits until a condition is not valid or the timeout (milliseconds) elapses.
 * @details     A reference to a boolean function of the signature **bool function(struct MCU_I2C_S * pi2c)** called
 *              waitCondition is passed as an argument to this function which will loop while the waitCondition is true
 *              or the timeout elapses.
 * @param       pi2c reference to the i2c control block
 * @param       waitCondition reference to the wait function
 * @param       timeout value in milliseconds of the desired timeout
 * @retval      RETCODE_OK if the condition becomes false before the timeout elapses.
 * @retval      RETCODE_TIMEOUT if the timeout elapses before the condition becomes flase.
 *
 */
static Retcode_T I2C_WaitUntilTimeout_ms(struct MCU_I2C_S *pi2c, bool (*waitCondition)(struct MCU_I2C_S *pi2c), uint32_t timeout)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t start = HAL_GetTick();
    while (waitCondition(pi2c))
    {
        if ((timeout == 0U) || ((HAL_GetTick() - start) > timeout))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT);
            break;
        }
    }
    return retcode;
}

/**
 * @brief       Checks if I2C instance is not ready
 * @param       pi2c reference to the i2c control block
 * @return      true if Flag is reset
 */
static bool I2C_IsI2CNotReady(struct MCU_I2C_S *pi2c)
{
    return (bool)(HAL_I2C_GetState(&pi2c->hi2c) != HAL_I2C_STATE_READY);
}

/**
 * @brief       Checks if TXIS flag is reset
 * @param       pi2c reference to the i2c control block
 * @return      true if Flag is reset
 */
static bool I2C_IsTXISReset(struct MCU_I2C_S *pi2c)
{
    return (bool)(__HAL_I2C_GET_FLAG(&pi2c->hi2c, I2C_FLAG_TXIS) == RESET);
}

/**
 * @brief       Checks if NACK flag is set
 * @param       pi2c reference to the i2c control block
 * @return      true if Flag is set
 */
static bool I2C_IsNACKSet(struct MCU_I2C_S *pi2c)
{
    return (bool)(__HAL_I2C_GET_FLAG(&pi2c->hi2c, I2C_FLAG_AF) == SET);
}

/**
 * @brief       Checks if STOPF flag is reset
 * @param       pi2c reference to the i2c control block
 * @return      true if Flag is reset
 */
static bool I2C_IsSTOPFReset(struct MCU_I2C_S *pi2c)
{
    return (bool)(__HAL_I2C_GET_FLAG(&pi2c->hi2c, I2C_FLAG_STOPF) == RESET);
}

/**
 * @brief       Checks if TCR flag is reset
 * @param       pi2c reference to the i2c control block
 * @return      true if Flag is reset
 */
static bool I2C_IsTCRReset(struct MCU_I2C_S *pi2c)
{
    return (bool)(__HAL_I2C_GET_FLAG(&pi2c->hi2c, I2C_FLAG_TCR) == RESET);
}

/**
 * @brief       Checks if BUSY flag is set
 * @param       pi2c reference to the i2c control block
 * @return      true if Flag is reset
 */
static bool I2C_IsBUSYSet(struct MCU_I2C_S *pi2c)
{
    return (bool)(__HAL_I2C_GET_FLAG(&pi2c->hi2c, I2C_FLAG_BUSY) == SET);
}

#endif //-- KISO_FEATURE_I2C
