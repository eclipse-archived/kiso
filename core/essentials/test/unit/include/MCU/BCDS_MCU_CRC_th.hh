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
 
#ifndef BCDS_MCU_CRC_TH_HH_
#define BCDS_MCU_CRC_TH_HH_

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* Include or ban the real interface header */
#include "BCDS_MCU_CRC.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

#if BCDS_FEATURE_MCU_CRC

FAKE_VALUE_FUNC(Retcode_T, MCU_CRC8, struct MCU_CRC8_S *, uint8_t*, uint32_t ,uint8_t *);
FAKE_VALUE_FUNC(Retcode_T, MCU_CRC16, struct MCU_CRC16_S *, uint16_t*, uint32_t ,uint16_t *);
FAKE_VALUE_FUNC(Retcode_T, MCU_CRC32, struct MCU_CRC32_S *, uint32_t*, uint32_t ,uint32_t *);

#endif /* BCDS_FEATURE_MCU_CRC */
#endif /* BCDS_MCU_CRC_TH_HH_*/
