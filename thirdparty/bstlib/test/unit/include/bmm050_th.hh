/****************************************************************************/
/**
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution

 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_BMM050_TH_HH_
#define BCDS_BMM050_TH_HH_


#include "bmm050.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_init,struct bmm050_t *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_read_mag_data_XYZ,struct bmm050_mag_data_s16_t *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_read_mag_data_XYZ_s32,struct bmm050_mag_s32_data_t *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_read_mag_data_XYZ_float,struct bmm050_mag_data_float_t *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_read_register,u8,u8*,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_write_register,u8,u8*,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_selftest_XYZ,u8 *);
FAKE_VALUE_FUNC(s16,bmm050_compensate_X,s16,u16);
FAKE_VALUE_FUNC(s32,bmm050_compensate_X_s32,s16,u16);
FAKE_VALUE_FUNC(float,bmm050_compensate_X_float,s16,u16);
FAKE_VALUE_FUNC(s16,bmm050_compensate_Y,s16,u16);
FAKE_VALUE_FUNC(s32,bmm050_compensate_Y_s32,s16,u16);
FAKE_VALUE_FUNC(float,bmm050_compensate_Y_float,s16,u16);
FAKE_VALUE_FUNC(s16,bmm050_compensate_Z,s16,u16);
FAKE_VALUE_FUNC(s32,bmm050_compensate_Z_s32,s16,u16);
FAKE_VALUE_FUNC(float,bmm050_compensate_Z_float,s16,u16);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_raw_xyz,struct bmm050_mag_data_s16_t *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_init_trim_registers);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_spi3,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_power_mode,u8 *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_power_mode,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_advanced_selftest,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_advanced_selftest,u8 *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_data_rate,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_data_rate,u8 *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_functional_state,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_functional_state,u8 *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_selftest,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_selftest,u8*);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_perform_advanced_selftest,s16 *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_rep_XY,u8 *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_rep_XY,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_rep_Z,u8 *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_rep_Z,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_presetmode,u8 *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_presetmode,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_control_measurement_x,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_set_control_measurement_y,u8);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_soft_rst);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_get_bmx055_remapped_raw_xyz,struct bmm050_remapped_mag_s16_data_t *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_read_bmx055_remapped_mag_data_XYZ_float,struct bmm050_remapped_mag_data_float_t *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_read_bmx055_remapped_mag_data_XYZ_s32,struct bmm050_remapped_mag_s32_data_t *);
FAKE_VALUE_FUNC(BMM050_RETURN_FUNCTION_TYPE,bmm050_read_bmx055_remapped_mdataXYZ,struct bmm050_remapped_mdata *);

#endif /* BCDS_BMM050_TH_HH_ */

/*************************************************************************** */
