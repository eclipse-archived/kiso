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

#ifndef CELLULAR_URC_TH_HH
#define CELLULAR_URC_TH_HH

/* Include the needed headers */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "CellularUrc.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CellularUrc_Miscellaneous);
FAKE_VALUE_FUNC(Retcode_T, CellularUrc_HandleResponses);

#endif /* CELLULAR_URC_TH_HH */
