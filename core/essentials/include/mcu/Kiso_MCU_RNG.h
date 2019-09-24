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
 * @defgroup    KISO_HAL_MCU_RNG MCU RNG
 * @ingroup     KISO_HAL_MCU_IF
 * @{
 *
 * @brief Unified RNG (Random Number Generator) API Inteface
 *
 * @details
 * The randomness of this data may depend on the hardware implementation. No guarantees can be made by the MCU layer
 * about that.
 */

#ifndef KISO_MCU_RNG_H_
#define KISO_MCU_RNG_H_

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_RNG

/**
 * @brief Data type for the RNG handle type which is used to identify the
 * RNG component to work with.
 */
typedef HWHandle_T RNG_T;

/**
 * @brief Initializes the RNG instance.
 *
 * The initialization must be invoked before any use of the RNG instance.
 * This command will not activate or turn-on the RNG hardware. Instead the BSP
 * shall be utilized for that.
 *
 * @param[in] rng: RNG handle.
 *
 * @retval RETCODE_OK on success, or an error code otherwise.
 */
Retcode_T MCU_RNG_Initialize(RNG_T rng);

/**
 * @brief Deinitializes the RNG instance.
 *
 * After calling this function, the RNG instance must not be used anymore.
 *
 * @param[in] rng: RNG handle.
 *
 * @retval RETCODE_OK on success, or an error code otherwise.
 */
Retcode_T MCU_RNG_Deinitialize(RNG_T rng);

/**
 * @brief Fills the given buffer with the requested amount of random bytes.
 *
 * @note Any amount of bytes may be generated this way. Note however, that the
 * caller is blocked for the entirety of the process, which depending on the
 * hardware implementation and the requested data amount, may take some time.

 * @param[in] rng: RNG handle.
 * @param[in] buffer: Buffer to hold the generated random bytes.
 * @param[in] length: Length of the passed buffer.
 *
 * @retval RETCODE_OK on success, or an error code otherwise.
 */
Retcode_T MCU_RNG_Generate(RNG_T rng, uint8_t *buffer, uint32_t length);

#endif /* KISO_FEATURE_RNG */
#endif /* KISO_MCU_RNG_H_ */

/**  @} */
