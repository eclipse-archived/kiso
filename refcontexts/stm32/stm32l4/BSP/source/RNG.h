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
 * @ingroup BSP_RNG
 * @{
 *  @file
 *  @brief Handling routines provided for RNG.
 */

#ifndef RNG_H
#define RNG_H

#include "BCDS_BSP_RNG.h"

#if BCDS_FEATURE_BSP_RNG

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E RNG_getState(void);

#endif /* BCDS_FEATURE_BSP_RNG */
#endif /* RNG_H */
/**@}*//* @ingroup BSP_RNG */
