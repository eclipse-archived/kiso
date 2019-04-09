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
 * @brief Board configuration settings
 */
#ifndef __BOARD_H__
#define __BOARD_H__

/* public interface declaration */

/*
*   This is a simple gateway to the real Board.h file
*   Based on your include path to Board.h, the corresponding Board.c will also be included
*
*/

#if defined(R01) || defined(R00)
    #include "R01/Board_R01.h"
#else
    #error "Please set include path to point to appropriate CC2650EM device"
#endif

#endif /* __BOARD_H__ */
