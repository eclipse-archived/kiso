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
