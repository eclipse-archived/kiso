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
#ifndef KISO_BMP280_TH_HH_
#define KISO_BMP280_TH_HH_

#include "bmp280.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_init, struct bmp280_t *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_read_uncomp_temperature, s32 *)
FAKE_VALUE_FUNC(s32, bmp280_compensate_temperature_int32, s32)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_read_uncomp_pressure, s32 *)
FAKE_VALUE_FUNC(u32, bmp280_compensate_pressure_int32, s32)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_read_uncomp_pressure_temperature, s32 *, s32 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_read_pressure_temperature, u32 *, s32 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_get_calib_param)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_get_oversamp_temperature, u8 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_set_oversamp_temperature, u8)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_get_oversamp_pressure, u8 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_set_oversamp_pressure, u8)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_get_power_mode, u8 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_set_power_mode, u8)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_set_soft_rst)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_get_spi3, u8 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_set_spi3, u8)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_get_filter, u8 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_set_filter, u8)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_get_standby_durn, u8 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_set_standby_durn, u8)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_set_work_mode, u8)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_get_forced_uncomp_pressure_temperature, s32 *, s32 *)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_write_register, u8, u8 *, u8)
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_read_register, u8, u8 *, u8)
#ifdef BMP280_ENABLE_FLOAT
FAKE_VALUE_FUNC(double, bmp280_compensate_temperature_double, s32)
FAKE_VALUE_FUNC(double, bmp280_compensate_pressure_double, s32)
#endif
#if defined(BMP280_ENABLE_INT64) && defined(BMP280_64BITSUPPORT_PRESENT)
FAKE_VALUE_FUNC(u32, bmp280_compensate_pressure_int64, s32)
#endif
FAKE_VALUE_FUNC(BMP280_RETURN_FUNCTION_TYPE, bmp280_compute_wait_time, u8 *)

#endif /* KISO_BMP280_TH_HH_ */

/*************************************************************************** */
