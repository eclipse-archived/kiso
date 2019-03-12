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
