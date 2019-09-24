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
 * @ingroup UTILS
 *
 * @defgroup XPROTOCOL XProtocol
 * @{
 *
 * @brief
 *      Mockup implementation for the Kiso_XProtocol_th.hh module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_XPROTOCOL_TH_HH_
#define KISO_XPROTOCOL_TH_HH_

/* Include KISO basics header files */
#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/* Include gtest header file */
#include "gtest.h"

/* Wrap Kiso_XProtocol interface header */
#include "Kiso_XProtocol.h"

/**
 * Mock-ups for the provided interfaces
 * Deprecated functions will be removed as part of PFM-659
 * Deprecated since vXXX (\todo: replace XXX with next version)
 */
FAKE_VALUE_FUNC(Retcode_T, xProtocol_Init)
FAKE_VALUE_FUNC(Retcode_T, xProtocol_EncodeFrame, const uint8_t *, uint32_t,
                uint32_t, uint8_t *, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, xProtocol_DecodeFrame, const uint8_t *, uint32_t,
                uint32_t, uint8_t *, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, xProtocol_IsCompleteFrame, const uint8_t *, uint32_t,
                const uint8_t **);
FAKE_VALUE_FUNC(Retcode_T, xProtocol_GetPayloadLength, const uint8_t *, uint32_t,
                uint32_t *);

/* Faked variables needs to be initialized by the test fixture */
FAKE_VALUE_FUNC(Retcode_T, XProtocol_Init)
FAKE_VALUE_FUNC(Retcode_T, XProtocol_EncodeFrame, const uint8_t *, uint32_t,
                uint32_t, uint8_t *, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, XProtocol_DecodeFrame, const uint8_t *, uint32_t,
                uint32_t, uint8_t *, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, XProtocol_IsCompleteFrame, const uint8_t *, uint32_t,
                const uint8_t **);
FAKE_VALUE_FUNC(Retcode_T, XProtocol_GetPayloadLength, const uint8_t *, uint32_t,
                uint32_t *);

#endif /* KISO_XPROTOCOL_TH_HH_*/

/*************************************************************************** */
