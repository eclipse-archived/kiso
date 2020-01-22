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

#ifndef ATUTILS_TH_HH_
#define ATUTILS_TH_HH_

#include <gtest.h>

#include "AtUtils.h"

FAKE_VALUE_FUNC(Retcode_T, Utils_ConvertAtResponseRetcodeToCellularRetcode, Retcode_T)
FAKE_VALUE_FUNC(Retcode_T, Utils_ConvertCellularResponseCodeToCellularRetcode, AtResponseCode_T)
FAKE_VALUE_FUNC(Retcode_T, Utils_Strtol, const uint8_t *, uint32_t, int32_t *)
FAKE_VALUE_FUNC(Retcode_T, Utils_StrtolBounds, const uint8_t *, uint32_t, int32_t *, int32_t, int32_t)
FAKE_VALUE_FUNC(Retcode_T, Utils_WaitForAndHandleResponseCode, uint32_t, Retcode_T)
FAKE_VALUE_FUNC(const uint8_t *, Utils_TrimFlukeCharacters, const uint8_t *, uint32_t, uint32_t *)

#endif /* ATUTILS_TH_HH_ */
