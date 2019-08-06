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
 * @file 
 *
 *
 *  Mockup implementation for the BCDS_CRC_th module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_CRC_TH_HH_
#define BCDS_CRC_TH_HH_

/* include system header files */
#include "gtest.h"
/*wrap the real BCDS_ComoCRC.h interface header */
#include"BCDS_CRC.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CRC_8,uint8_t,uint8_t * ,uint8_t *,uint16_t);
FAKE_VALUE_FUNC(Retcode_T, CRC_16,uint16_t,uint16_t * ,uint8_t *,uint16_t);
FAKE_VALUE_FUNC(Retcode_T, CRC_32,uint32_t,uint32_t * ,uint8_t *,uint16_t);
FAKE_VALUE_FUNC(Retcode_T, CRC_32_Reverse,uint32_t,uint32_t * ,uint8_t *,uint16_t);

#endif /* BCDS_CRC_TH_HH_*/

/*************************************************************************** */
