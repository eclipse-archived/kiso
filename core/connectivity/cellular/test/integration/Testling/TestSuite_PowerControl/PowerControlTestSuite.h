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
 * @file	Tests power ON,OFF and RESET conditions.
 *
 * The positive condition related to Cellular Power control Module(ON,OFF,Reset)
 * and corresponding response is validated.
 *
 */

#ifndef PowerControlTestSuite_H
#define PowerControlTestSuite_H

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

#define DELAY_COUNT_VALUE                       UINT8_C(26)

#define DELAY_15_SECS                           UINT32_C(15000)
#define DELAY_60_SECS                           UINT32_C(60000)

/* Put the function declarations here */

/**
 * @brief  Registers the power control suite and the respective test cases for this suite
 *
 * @retval Status of the Power control test suite operation, RETCODE_OK on success or some
 * error code otherwise.
 */
Retcode_T PowerControlTestSuite_Init(void);

#endif /* PowerControlTestSuite_H */

