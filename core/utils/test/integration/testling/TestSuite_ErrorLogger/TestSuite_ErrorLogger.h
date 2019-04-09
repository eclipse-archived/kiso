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

#ifndef ERRORLOGGER_TESTSUITE_H_
#define ERRORLOGGER_TESTSUITE_H_

#include "BCDS_Retcode.h"

/* Constant definition */

#define MAX_ENTRY (ERRORLOGGER_SIZE/SIZEOF_LOGENTRY)

Retcode_T TestSuite_ErrorLogger_Initialize( uint8_t );

#endif /* ERRORLOGGER_TESTSUITE_H_ */
