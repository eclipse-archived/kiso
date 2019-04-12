/****************************************************************************/
/**
 *  Copyright (C) Bosch Connected Devices and Solutions.
 * All Rights Reserved. Confidential.
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.

 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_BMP180_TH_HH_
#define BCDS_BMP180_TH_HH_


#include "bmp180.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(BMP180_RETURN_FUNCTION_TYPE, bmp180_init, struct bmp180_t *);
FAKE_VALUE_FUNC(s16, bmp180_get_temperature, u32);
FAKE_VALUE_FUNC(s32, bmp180_get_pressure, u32);
FAKE_VALUE_FUNC(u16, bmp180_get_uncomp_temperature);
FAKE_VALUE_FUNC(u32, bmp180_get_uncomp_pressure);
FAKE_VALUE_FUNC(BMP180_RETURN_FUNCTION_TYPE, bmp180_get_calib_param);

#endif /* BCDS_BMP180_TH_HH_ */

/*************************************************************************** */
