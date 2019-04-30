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

#ifndef BCDS_MCU_FLASH_TH_HH
#define BCDS_MCU_FLASH_TH_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"


/* wrap the real interface header */
#include "BCDS_MCU_Flash.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_FLASH

uint8_t targetAddress = 0x00;
uint8_t sourceAddress = 0x01;

const uint8_t *targetAddress_ptr = &targetAddress;
const uint8_t *sourceAddress_ptr = &sourceAddress;

FAKE_VALUE_FUNC(Retcode_T, MCU_Flash_Write, const uint8_t*, const uint8_t*, uint32_t);
FAKE_VALUE_FUNC(uint32_t, MCU_Flash_GetPageSize );
FAKE_VALUE_FUNC(Retcode_T, MCU_Flash_ErasePage, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, MCU_Flash_Erase, uint32_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_Flash_Read, uint8_t*, uint8_t*, uint32_t);


#endif /* BCDS_FEATURE_FLASH */
#endif /* BCDS_MCU_Flash_th_HH */
