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
 * @ingroup BSP_BSE_TEST_IF
 * @{
 */
/**
 *
 *  @file
 *
 *  @brief Handling routines provided for test interface related interrupts
 *
 */

#ifndef TestInterface_H
#define TestInterface_H

#include "BCDS_BSP_TestInterface.h"

#if BCDS_FEATURE_BSP_TEST_INTERFACE

void BSP_LEUART1_IRQHandler(void);

#endif /* BCDS_FEATURE_BSP_TEST_INTERFACE */

#endif /* TestInterface_H */

/**@}*/
