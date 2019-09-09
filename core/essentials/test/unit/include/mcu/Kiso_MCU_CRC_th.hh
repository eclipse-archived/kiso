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
 
#ifndef KISO_MCU_CRC_TH_HH_
#define KISO_MCU_CRC_TH_HH_

#include "gtest.h"

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* Include or ban the real interface header */
#include "Kiso_MCU_CRC.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

#if KISO_FEATURE_MCU_CRC

FAKE_VALUE_FUNC(Retcode_T, MCU_CRC8, struct MCU_CRC8_S *, uint8_t*, uint32_t ,uint8_t *);
FAKE_VALUE_FUNC(Retcode_T, MCU_CRC16, struct MCU_CRC16_S *, uint16_t*, uint32_t ,uint16_t *);
FAKE_VALUE_FUNC(Retcode_T, MCU_CRC32, struct MCU_CRC32_S *, uint32_t*, uint32_t ,uint32_t *);

#endif /* KISO_FEATURE_MCU_CRC */
#endif /* KISO_MCU_CRC_TH_HH_*/
