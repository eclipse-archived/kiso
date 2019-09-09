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
#ifndef KISO_BNO055_TH_HH_
#define KISO_BNO055_TH_HH_


#include "bno055.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_init, struct bno055_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_write_register, u8, u8 *, u8);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_register, u8, u8 *, u8);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_chip_id, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_sw_rev_id, u16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_page_id, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_write_page_id, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_accel_rev_id, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_mag_rev_id, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gyro_rev_id, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_bl_rev_id, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_accel_x, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_accel_y, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_accel_z, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_accel_xyz, struct bno055_accel_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_mag_x, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_mag_y, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_mag_z, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_mag_xyz, struct bno055_mag_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gyro_x, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gyro_y, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gyro_z, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gyro_xyz, struct bno055_gyro_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_euler_h, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_euler_r, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_euler_p, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_euler_hrp, struct bno055_euler_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_quaternion_w, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_quaternion_x, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_quaternion_y, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_quaternion_z, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_quaternion_wxyz, struct bno055_quaternion_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_linear_accel_x, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_linear_accel_y, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_linear_accel_z, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_linear_accel_xyz, struct bno055_linear_accel_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gravity_x, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gravity_y, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gravity_z, s16 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gravity_xyz, struct bno055_gravity_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_temp_data, s8 *);
#ifdef	BNO055_FLOAT_ENABLE
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_accel_x_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_accel_y_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_accel_x_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_accel_x_mg, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_accel_y_mg, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_accel_z_mg, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_accel_xyz_msq, struct bno055_accel_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_accel_xyz_mg, struct bno055_accel_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_mag_x_uT, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_mag_y_uT, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_mag_z_uT, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_mag_xyz_uT, struct bno055_mag_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gyro_x_dps, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gyro_x_rps, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gyro_y_dps, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gyro_y_rps, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gyro_z_dps, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gyro_z_rps, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gyro_xyz_dps, struct bno055_gyro_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gyro_xyz_rps, struct bno055_gyro_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_euler_h_deg, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_euler_h_rad, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_euler_r_deg, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_euler_r_rad, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_euler_p_deg, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_euler_p_rad, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_euler_hpr_rad, struct bno055_euler_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_euler_hpr_deg, struct bno055_euler_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_linear_accel_x_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_linear_accel_y_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_linear_accel_z_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_linear_accel_xyz_msq, struct bno055_linear_accel_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_gravity_float_x_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_gravity_float_y_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_gravity_float_z_msq, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_gravity_xyz_msq, struct bno055_gravity_float_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_temp_fahrenheit, float *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_convert_float_temp_celsius, float *);
#endif

FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_mag_calib_stat, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_calib_stat, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_calib_stat, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_sys_calib_stat, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_selftest_accel, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_selftest_mag, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_selftest_gyro, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_selftest_mcu, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_stat_gyro_any_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_stat_gyro_highrate, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_stat_accel_high_g, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_stat_accel_any_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_stat_accel_no_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_stat_main_clk, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_sys_stat_code, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_sys_error_code, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_unit, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_unit, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_unit, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_unit, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_euler_unit, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_euler_unit, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_tilt_unit, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_tilt_unit, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_temp_unit, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_temp_unit, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_data_output_format, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_data_output_format, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_operation_mode, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_operation_mode, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_power_mode, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_power_mode, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_rst, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_rst, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_clk_src, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_clk_src, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_sys_rst, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_sys_rst, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_selftest, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_selftest, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_temp_source, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_temp_source, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_axis_remap_value, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_axis_remap_value, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_remap_x_sign, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_remap_x_sign, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_remap_y_sign, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_remap_y_sign, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_remap_z_sign, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_remap_z_sign, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_sic_matrix, struct bno055_sic_matrix_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_write_sic_matrix, struct bno055_sic_matrix_t  * );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_accel_offset, struct bno055_accel_offset_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_write_accel_offset, struct bno055_accel_offset_t  * );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_mag_offset, struct bno055_mag_offset_t *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_write_mag_offset, struct bno055_mag_offset_t  * );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_read_gyro_offset, struct bno055_gyro_offset_t  *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_write_gyro_offset, struct bno055_gyro_offset_t  *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_range, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_range, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_bw, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_bw, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_power_mode, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_power_mode, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_mag_data_output_rate, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_mag_data_output_rate, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_mag_operation_mode, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_mag_operation_mode, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_mag_power_mode, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_mag_power_mode, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_range, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_range, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_bw, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_bw, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_power_mode, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_power_mode, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_sleep_tmr_mode, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_sleep_tmr_mode, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_sleep_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_sleep_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_sleep_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_sleep_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_auto_sleep_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_gyro_set_auto_sleep_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_mag_sleep_mode, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_mag_sleep_mode, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_mag_sleep_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_mag_sleep_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_mask_gyro_any_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_mask_gyro_any_motion, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_mask_gyro_highrate, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_mask_gyro_highrate, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_mask_accel_high_g, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_mask_accel_high_g, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_mask_accel_any_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_mask_accel_any_motion, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_mask_accel_no_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_mask_accel_no_motion, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_gyro_any_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_gyro_any_motion, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_gyro_highrate, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_gyro_highrate, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_accel_high_g, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_accel_high_g, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_accel_any_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_accel_any_motion, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_intr_accel_no_motion, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_intr_accel_no_motion, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_any_motion_thres, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_any_motion_thres, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_any_motion_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_any_motion_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_any_motion_no_motion_axis_enable, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_any_motion_no_motion_axis_enable, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_high_g_axis_enable, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_high_g_axis_enable, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_high_g_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_high_g_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_high_g_thres, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_high_g_thres, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_slow_no_motion_thres, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_slow_no_motion_thres, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_slow_no_motion_enable, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_slow_no_motion_enable, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_accel_slow_no_motion_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_accel_slow_no_motion_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_any_motion_axis_enable, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_any_motion_axis_enable, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_axis_enable, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_axis_enable, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_any_motion_filter, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_any_motion_filter, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_filter, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_filter, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_x_thres, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_x_thres, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_x_hyst, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_x_hyst, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_x_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_x_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_y_thres, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_y_thres, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_y_hyst, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_y_hyst, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_y_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_y_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_z_thres, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_z_thres, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_z_hyst, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_z_hyst, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_highrate_z_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_highrate_z_durn, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_any_motion_thres, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_any_motion_thres, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_any_motion_slope_samples, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_any_motion_slope_samples, u8 );
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_get_gyro_any_motion_awake_durn, u8 *);
FAKE_VALUE_FUNC(BNO055_RETURN_FUNCTION_TYPE, bno055_set_gyro_any_motion_awake_durn, u8 );

#endif /* KISO_BNO055_TH_HH_ */

/*************************************************************************** */
