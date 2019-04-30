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

/* header definition ******************************************************** */
#ifndef BCDS_XPROTOCOL_TH_HH_
#define BCDS_XPROTOCOL_TH_HH_

/* include system header files */
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

/* wrap the real BCDS_XProtocol.h interface header */
#include "BCDS_XProtocol.h"

/**
 * mock-ups for the provided interfaces
 * Deprecated functions will be removed as part of PFM-659
 * Deprecated since vXXX (TODO: replace XXX with next version)
 */
FAKE_VALUE_FUNC(Retcode_T, xProtocol_Init);
FAKE_VALUE_FUNC(Retcode_T, xProtocol_EncodeFrame, const uint8_t *, uint32_t,
        uint32_t, uint8_t *, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, xProtocol_DecodeFrame, const uint8_t *, uint32_t,
        uint32_t, uint8_t *, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, xProtocol_IsCompleteFrame, const uint8_t *, uint32_t,
        const uint8_t **);
FAKE_VALUE_FUNC(Retcode_T, xProtocol_GetPayloadLength, const uint8_t *, uint32_t,
        uint32_t *);

/* faked variables needs to be initialized by the test fixture */
FAKE_VALUE_FUNC(Retcode_T, XProtocol_Init);
FAKE_VALUE_FUNC(Retcode_T, XProtocol_EncodeFrame, const uint8_t *, uint32_t,
        uint32_t, uint8_t *, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, XProtocol_DecodeFrame, const uint8_t *, uint32_t,
        uint32_t, uint8_t *, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, XProtocol_IsCompleteFrame, const uint8_t *, uint32_t,
        const uint8_t **);
FAKE_VALUE_FUNC(Retcode_T, XProtocol_GetPayloadLength, const uint8_t *, uint32_t,
        uint32_t *);

#endif /* BCDS_XPROTOCOL_TH_HH_*/

/*************************************************************************** */
