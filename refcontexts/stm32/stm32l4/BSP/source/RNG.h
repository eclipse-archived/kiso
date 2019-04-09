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
