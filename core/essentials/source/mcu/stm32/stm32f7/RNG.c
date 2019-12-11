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

#include "Kiso_MCU_RNG.h"

#if KISO_FEATURE_RNG

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rng.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_RNG

#define AS_CUBE_HANDLE(rng) ((RNG_HandleTypeDef *)rng)

/**
 * @brief Mapper for HAL function return values.
 *
 * @param[in] halRet: Vendor driver return code.
 *
 * @return The Retcode_T mapped to the respective HAL_StatusTypeDef value.
 */
static inline Retcode_T MapHalRetToMcuRet(HAL_StatusTypeDef halRet);

static inline Retcode_T MapHalRetToMcuRet(HAL_StatusTypeDef halRet)
{
    Retcode_T ret;
    switch (halRet)
    {
    case HAL_OK:
        ret = RETCODE_OK;
        break;
    case HAL_BUSY:
        ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        break;
    case HAL_TIMEOUT:
        ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT);
        break;
    case HAL_ERROR:
        /* Fall Through */
    default:
        ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        break;
    }
    return ret;
}

Retcode_T MCU_RNG_Initialize(RNG_T rng)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_HandleTypeDef *cubeHandle = NULL;
    HAL_StatusTypeDef status = HAL_OK;

    if (NULL == rng)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (RETCODE_OK == retcode)
    {
        cubeHandle = AS_CUBE_HANDLE(rng);
        status = HAL_RNG_Init(cubeHandle);
        retcode = MapHalRetToMcuRet(status);
    }

    return retcode;
}

Retcode_T MCU_RNG_Deinitialize(RNG_T rng)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_HandleTypeDef *cubeHandle = NULL;
    HAL_StatusTypeDef status = HAL_OK;

    if (NULL == rng)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (RETCODE_OK == retcode)
    {
        cubeHandle = AS_CUBE_HANDLE(rng);
        status = HAL_RNG_DeInit(cubeHandle);
        retcode = MapHalRetToMcuRet(status);
    }

    return retcode;
}

Retcode_T MCU_RNG_Generate(RNG_T rng, uint8_t *buffer, uint32_t length)
{
    Retcode_T retcode = RETCODE_OK;
    RNG_HandleTypeDef *cubeHandle = NULL;
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t random32 = 0;

    if (NULL == rng || NULL == buffer)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    cubeHandle = AS_CUBE_HANDLE(rng);

    for (uint32_t i = 0; (RETCODE_OK == retcode) && (i < length);
         i += sizeof(uint32_t))
    {
        /* Check if remaining buffer length allows direct write into the
         * buffer and... */
        if (length - i >= sizeof(uint32_t))
        {
            /* ...fill into buffer directly. */
            status = HAL_RNG_GenerateRandomNumber(cubeHandle,
                                                  (uint32_t *)&buffer[i]);
            retcode = MapHalRetToMcuRet(status);
        }
        else
        {
            /* ...or utilize a helper variable... */
            status = HAL_RNG_GenerateRandomNumber(cubeHandle, &random32);
            retcode = MapHalRetToMcuRet(status);

            if (RETCODE_OK == retcode)
            {
                /* ...and copy only the chunk of bytes needed. */
                memcpy(&(buffer[i]), (uint8_t *)&random32, length - i);
            }
        }
    }

    return retcode;
}

#endif /* if KISO_FEATURE_RNG */
