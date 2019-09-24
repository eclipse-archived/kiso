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
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief Provides functions internally exposed from board module to other BSP modules.
 *
 * @details
 */
#ifndef PROTECTED_BOARD_H_
#define PROTECTED_BOARD_H_

#include "Kiso_HAL.h"
/*
 * Returns true if BSP_Board_Initialize() successfully executed.
 */
bool Board_IsInitDone(void);

#endif /* PROTECTED_BOARD_H_ */
