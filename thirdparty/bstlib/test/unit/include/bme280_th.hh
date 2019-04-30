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
#ifndef BCDS_BME280_TH_HH_
#define BCDS_BME280_TH_HH_


#include "bme280.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_compute_wait_time,u8*);
FAKE_VALUE_FUNC(u32,bme280_compensate_pressure_int64_twentyfour_bit_output,s32);
FAKE_VALUE_FUNC(u32,bme280_compensate_pressure_int64,s32);
FAKE_VALUE_FUNC(double,bme280_compensate_humidity_double,s32);
FAKE_VALUE_FUNC(double,bme280_compensate_pressure_double,s32);
FAKE_VALUE_FUNC(double,bme280_compensate_temperature_double,s32);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_read_register,u8,u8*,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_write_register,u8,u8*,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_get_forced_uncomp_pressure_temperature_humidity,s32*,s32*,s32*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_set_standby_durn,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_get_standby_durn,u8*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_set_filter,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_get_filter,u8*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_set_spi3,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_get_spi3,u8*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_set_soft_rst);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_set_power_mode,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_get_power_mode,u8*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_set_oversamp_humidity,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_set_oversamp_pressure,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_get_oversamp_pressure,u8*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_set_oversamp_temperature,u8);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_get_oversamp_temperature,u8*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_get_calib_param);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_read_pressure_temperature_humidity,u32*,s32*,u32*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_read_uncomp_pressure_temperature_humidity,s32*,s32*,s32*);
FAKE_VALUE_FUNC(u16,bme280_compensate_humidity_int32_sixteen_bit_output,s32);
FAKE_VALUE_FUNC(u32,bme280_compensate_humidity_int32,s32);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_read_uncomp_humidity,s32*);
FAKE_VALUE_FUNC(u32,bme280_compensate_pressure_int32,s32);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_read_uncomp_pressure,s32*);
FAKE_VALUE_FUNC(s16,bme280_compensate_temperature_int32_sixteen_bit_output,s32);
FAKE_VALUE_FUNC(s32,bme280_compensate_temperature_int32,s32);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_read_uncomp_temperature,s32*);
FAKE_VALUE_FUNC(BME280_RETURN_FUNCTION_TYPE,bme280_init,struct bme280_t *);
#endif /* BCDS_BME280_TH_HH_ */

/*************************************************************************** */
