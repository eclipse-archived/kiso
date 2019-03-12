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
