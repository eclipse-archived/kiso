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
 
#ifndef KISO_MCU_FLASHINTERN_TH_HH_
#define KISO_MCU_FLASHINTERN_TH_HH_

/* Include or ban the real interface header */
#include "Kiso_MCU_FlashIntern.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_Initialize, MCU_FlashIntern_T)

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_Erase, uint32_t, uint32_t)

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_Write, uint32_t, uint8_t*, uint32_t)

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_Read, uint32_t, uint8_t*, uint32_t)

FAKE_VALUE_FUNC(Retcode_T ,MCU_FlashIntern_WriteProtect, uint32_t, uint32_t, bool)

FAKE_VALUE_FUNC(uint32_t ,MCU_FlashIntern_GetMinRWSize)
#endif /* KISO_MCU_FLASHINTERN_TH_HH_ */
