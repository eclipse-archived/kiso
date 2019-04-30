/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

#ifndef CELLULAR_UTILS_TH_HH
#define CELLULAR_UTILS_TH_HH

/* Include all headers which are needed by this file. */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "AtResponseParser_th.hh"
#include "CellularUtils.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CellularUtils_ConvertAtResponseRetcodeToCellularRetcode, Retcode_T);
FAKE_VALUE_FUNC(Retcode_T, CellularUtils_ConvertCellularResponseCodeToCellularRetcode, AtResponseCode_T);
FAKE_VALUE_FUNC(Retcode_T, CellularUtils_Strtol, const uint8_t*, uint32_t, int32_t *);
FAKE_VALUE_FUNC(Retcode_T, CellularUtils_StrtolBounds, const uint8_t*, uint32_t, int32_t*, int32_t, int32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularUtils_ParseIpAddress, uint8_t*, uint32_t, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularUtils_WaitForAndHandleResponseCode, uint32_t, Retcode_T);
FAKE_VALUE_FUNC(Retcode_T, CellularUtils_HexToBin, const uint8_t *, uint8_t *, uint32_t);
FAKE_VOID_FUNC(CellularUtils_BinToHex, const uint8_t *, char *, uint32_t);
FAKE_VALUE_FUNC(uint8_t *, CellularUtils_TrimFlukeCharacters, uint8_t *, uint32_t, uint32_t *);

#endif /* CELLULAR_UTILS_TH_HH */
