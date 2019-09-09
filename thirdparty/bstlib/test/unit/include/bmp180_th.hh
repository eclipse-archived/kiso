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
 
/* header definition ******************************************************** */
#ifndef KISO_BMP180_TH_HH_
#define KISO_BMP180_TH_HH_


#include "bmp180.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(BMP180_RETURN_FUNCTION_TYPE, bmp180_init, struct bmp180_t *);
FAKE_VALUE_FUNC(s16, bmp180_get_temperature, u32);
FAKE_VALUE_FUNC(s32, bmp180_get_pressure, u32);
FAKE_VALUE_FUNC(u16, bmp180_get_uncomp_temperature);
FAKE_VALUE_FUNC(u32, bmp180_get_uncomp_pressure);
FAKE_VALUE_FUNC(BMP180_RETURN_FUNCTION_TYPE, bmp180_get_calib_param);

#endif /* KISO_BMP180_TH_HH_ */

/*************************************************************************** */
