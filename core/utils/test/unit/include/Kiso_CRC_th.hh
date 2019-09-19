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
 * @defgroup CRC CRC
 * @{
 *
 * @brief
 *      Mockup implementation for the Kiso_CRC_th.hh module
 *
 * @details
 *
 * @file
 **/
 
/* Header definition */
#ifndef KISO_CRC_TH_HH_
#define KISO_CRC_TH_HH_

/* Include gtest header file */
#include "gtest.h"

/* Wrap Kiso_CRC interface header */
#include "Kiso_CRC.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CRC_8, uint8_t, uint8_t*, const uint8_t*, uint16_t)
FAKE_VALUE_FUNC(Retcode_T, CRC_16, uint16_t, uint16_t*, const uint8_t*, uint16_t)
FAKE_VALUE_FUNC(Retcode_T, CRC_32, uint32_t, uint32_t*, const uint8_t*, uint16_t)
FAKE_VALUE_FUNC(Retcode_T, CRC_32_Reverse, uint32_t, uint32_t*, const uint8_t*, uint16_t)

#endif /* KISO_CRC_TH_HH_*/

/*************************************************************************** */
