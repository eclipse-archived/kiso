/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
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
