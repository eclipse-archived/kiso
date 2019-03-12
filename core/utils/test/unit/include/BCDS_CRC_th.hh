/****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file 
 *
 *
 *  Mockup implementation for the BCDS_CRC_th module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_CRC_TH_HH_
#define BCDS_CRC_TH_HH_

/* include system header files */

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
