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
 
#ifndef BCDS_MCU_FLASHINTERN_TH_HH_
#define BCDS_MCU_FLASHINTERN_TH_HH_

/* Include or ban the real interface header */
#include "BCDS_MCU_FlashIntern.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_Initialize, MCU_FlashIntern_T);

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_Erase, uint32_t, uint32_t);

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_Write, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_Read, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_WriteProtect, uint32_t, uint32_t, bool);

FAKE_VALUE_FUNC(uint32_t ,MCU_FlashIntern_GetMinRWSize);
#endif /* BCDS_MCU_FLASHINTERN_TH_HH_ */
