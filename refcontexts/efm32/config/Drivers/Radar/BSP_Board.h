/*
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

#ifndef BSP_BOARD_H
#define BSP_BOARD_H

/* Fake interface so that the compiler is happy ==> File not to copy to a project!! */

/***************************************************************************//**
 * @brief Enable 3V3 domain.
 * @return None.
  ******************************************************************************/
void BSP_enable3V3Domain(void);

/***************************************************************************//**
 * @brief Disable 3V3 domain.
 * @details Decrement keep active counter and disables 3V3 domain if counter
 * reached zero.
 * @return None.
 ******************************************************************************/
void BSP_disable3V3Domain(void);

#endif /* BSP_BOARD_H */

