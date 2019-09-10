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
 * @file SEGGER_RTT_th.hh
 *
 *
 *  Mockup implementation for SEGGER_RTT
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef KISO_SEGGER_RTT_TH_HH_
#define KISO_SEGGER_RTT_TH_HH_

/* include system header files */
#include <gtest.h>

/* wrap the real event_groups.h header */
#include "SEGGER_RTT.h"

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(SEGGER_RTT_Init)
FAKE_VALUE_FUNC(unsigned, SEGGER_RTT_Read, unsigned , void* , unsigned)
FAKE_VALUE_FUNC(unsigned, SEGGER_RTT_Write, unsigned , const void* , unsigned)
/* @todo: add missing headers */



#endif /* KISO_SEGGER_RTT_TH_HH_ */

/** ************************************************************************* */
