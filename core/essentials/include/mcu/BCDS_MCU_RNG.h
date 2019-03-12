/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) 2015-2018 Bosch Connected Devices and Solutions GmbH. 
 * Copyright (C) 2019 Robert Bosch GmbH.
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
 * @defgroup    BCDS_HAL_MCU_RNG MCU RNG
 * @ingroup     BCDS_HAL_MCU_IF
 * @{
 *
 * @brief Unified RNG (Random Number Generator) API Inteface
 *
 * @details
 * The randomness of this data may depend on the hardware implementation. No guarantees can be made by the MCU layer
 * about that.
 */

#ifndef BCDS_MCU_RNG_H_
#define BCDS_MCU_RNG_H_

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_RNG

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
Retcode_T MCU_RNG_Generate(RNG_T rng, uint8_t* buffer, uint32_t length);

#endif /* BCDS_FEATURE_RNG */
#endif /* BCDS_MCU_RNG_H_ */

/**  @} */
