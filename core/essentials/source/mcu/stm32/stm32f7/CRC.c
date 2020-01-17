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
 * @brief Contains the realization of the MCU CRC interface for STM32
 */

#include "Kiso_MCU_CRC.h"

#if KISO_FEATURE_MCU_CRC

#include "stm32f7xx.h"
#include "stm32f7xx_hal_crc.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_CRC

/** See description in the interface declaration */
Retcode_T MCU_CRC8(struct MCU_CRC8_S *initParms, uint8_t *data_in, uint32_t dataLength, uint8_t *crc)
{
    Retcode_T retcode = RETCODE_OK;

    if ((NULL == initParms) || (NULL == data_in) || (NULL == crc))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    CRC_HandleTypeDef handle;

    handle.Instance = CRC;
    handle.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
    handle.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
    handle.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
    handle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
    handle.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
    handle.Init.CRCLength = CRC_POLYLENGTH_8B;

    handle.Init.GeneratingPolynomial = (uint32_t)initParms->GeneratePolynomial;
    handle.Init.InitValue = (uint32_t)initParms->InitVal;

    __HAL_RCC_CRC_CLK_ENABLE();

    /* CRC module initialization by passing the populated CRC handle */
    if (HAL_OK != HAL_CRC_Init(&handle))
    {
        /* TODO: Use a more meaningful error code */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    if (RETCODE_OK == retcode)
    {
        *crc = HAL_CRC_Calculate(&handle, (uint32_t *)data_in, dataLength);

        if (HAL_OK != HAL_CRC_DeInit(&handle))
        {
            /* TODO: Use a more meaningful error code */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    __HAL_RCC_CRC_CLK_DISABLE();

    return retcode;
}

Retcode_T MCU_CRC16(struct MCU_CRC16_S *initParms, uint16_t *data_in, uint32_t dataLength, uint16_t *crc)
{
    Retcode_T retcode = RETCODE_OK;

    if ((NULL == initParms) || (NULL == data_in) || (NULL == crc))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    CRC_HandleTypeDef handle;

    handle.Instance = CRC;
    handle.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
    handle.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
    handle.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
    handle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
    handle.InputDataFormat = CRC_INPUTDATA_FORMAT_HALFWORDS;
    handle.Init.CRCLength = CRC_POLYLENGTH_16B;

    handle.Init.GeneratingPolynomial = (uint32_t)initParms->GeneratePolynomial;
    handle.Init.InitValue = (uint32_t)initParms->InitVal;

    __HAL_RCC_CRC_CLK_ENABLE();

    /* CRC module initialization by passing the populated CRC handle */
    if (HAL_OK != HAL_CRC_Init(&handle))
    {
        /* TODO: Use a more meaningful error code */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    if (RETCODE_OK == retcode)
    {
        *crc = HAL_CRC_Calculate(&handle, (uint32_t *)data_in, dataLength);

        if (HAL_OK != HAL_CRC_DeInit(&handle))
        {
            /* TODO: Use a more meaningful error code */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    __HAL_RCC_CRC_CLK_DISABLE();

    return retcode;
}

/** See description in the interface declaration */
Retcode_T MCU_CRC32(struct MCU_CRC32_S *initParms, uint32_t *data_in, uint32_t dataLength, uint32_t *crc)
{
    Retcode_T retcode = RETCODE_OK;

    if ((NULL == initParms) || (NULL == data_in) || (NULL == crc))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    CRC_HandleTypeDef handle;

    handle.Instance = CRC;
    handle.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
    handle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
    handle.Init.CRCLength = CRC_POLYLENGTH_32B;

    if (MCU_CRC32_POLY_ETHERNET == initParms->GeneratePolynomial)
    {
        handle.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
    }
    else
    {
        handle.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
        handle.Init.GeneratingPolynomial = initParms->GeneratePolynomial;
    }
    if (MCU_CRC32_INIT_VALUE_DEFAULT == initParms->InitVal)
    {
        handle.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
    }
    else
    {
        handle.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
        handle.Init.InitValue = initParms->InitVal;
    }

    switch (initParms->DataFormat)
    {
    case MCU_CRC_DATA_8BIT:
        handle.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
        break;

    case MCU_CRC_DATA_16BIT:
        handle.InputDataFormat = CRC_INPUTDATA_FORMAT_HALFWORDS;
        break;

    case MCU_CRC_DATA_32BIT:
        handle.InputDataFormat = CRC_INPUTDATA_FORMAT_WORDS;
        break;

    default:
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    __HAL_RCC_CRC_CLK_ENABLE();

    /* CRC module initialization by passing the populated CRC handle */
    if (HAL_OK != HAL_CRC_Init(&handle))
    {
        /* TODO: Use a more meaningful error code */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    if (RETCODE_OK == retcode)
    {
        *crc = HAL_CRC_Calculate(&handle, data_in, dataLength);

        if (HAL_OK != HAL_CRC_DeInit(&handle))
        {
            /* TODO: Use a more meaningful error code */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    __HAL_RCC_CRC_CLK_DISABLE();

    return retcode;
}

#endif /* KISO_FEATURE_MCU_CRC */
