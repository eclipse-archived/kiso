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
#ifndef BCDS_BMA2X2_TH_HH_
#define BCDS_BMA2X2_TH_HH_


/* wrap the real bma2x2.h interface header */
#include "bma2x2.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_burst_read,u8 ,u8 *, u32 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_init,struct bma2x2_t *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_write_reg,u8 ,u8 *, u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_reg,u8 ,u8 *, u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_x,s16 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_eight_resolution_x,s8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_y,s16 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_eight_resolution_y,s8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_z,s16 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_eight_resolution_z,s8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_xyz,struct bma2x2_accel_data*);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_eight_resolution_xyz,struct bma2x2_accel_eight_resolution*);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_tap_stat,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_orient_stat,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_fifo_status,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_fifo_frame_count,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_fifo_overrun,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_stat,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_range,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_range,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_bw,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_bw,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_power_mode,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_power_mode,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_mode_value,u8);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_sleep_durn,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_sleep_durn,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_sleep_timer_mode,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_sleep_timer_mode,u8);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_high_bw,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_high_bw,u8);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_shadow_dis,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_shadow_dis,u8);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_soft_rst);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_update_image);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_enable,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_enable,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_fifo_full,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_fifo_full,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_fifo_wm,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_fifo_wm,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_slow_no_motion,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_slow_no_motion,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_low_g,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_low_g,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_high_g,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_high_g,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_slope,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_slope,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_slow_no_motion,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_slow_no_motion,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_double_tap,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_double_tap,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_single_tap,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_single_tap,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_orient,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_orient,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_flat,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_flat,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_new_data,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_new_data,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr1_fifo_wm,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr1_fifo_wm,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr2_fifo_wm,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr2_fifo_wm,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr1_fifo_full,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr1_fifo_full,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr2_fifo_full,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr2_fifo_full,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_source,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_source,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_output_type,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_output_type,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_intr_level,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_intr_level,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_rst_intr,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_latch_intr,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_latch_intr,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_durn,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_durn,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_thres,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_thres,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_low_high_g_hyst,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_low_high_g_hyst,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_low_g_mode,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_low_g_mode,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_tap_durn,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_tap_durn,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_tap_shock,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_tap_shock,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_tap_quiet,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_tap_quiet,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_tap_thres,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_tap_thres,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_tap_sample,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_tap_sample,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_orient_mode,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_orient_mode,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_orient_block,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_orient_block,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_orient_hyst,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_orient_hyst,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_theta,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_theta,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_orient_enable,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_orient_enable,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_flat_hyst,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_flat_hyst,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_flat_hold_time,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_flat_hold_time,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_fifo_wml_trig,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_fifo_wml_trig,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_selftest_axis,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_selftest_axis,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_selftest_sign,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_selftest_sign,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_nvmprog_mode,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_nvmprog_mode,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_nvprog_trig,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_nvmprog_ready,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_nvmprog_remain,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_spi3,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_spi3,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_i2c_wdt,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_i2c_wdt,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_slow_comp,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_slow_comp,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_cal_rdy,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_cal_trigger,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_offset_target,u8 ,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_offset_target,u8 ,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_offset,u8 ,s8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_offset_rst, u8);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_fifo_mode,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_fifo_mode,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_fifo_data_select,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_set_fifo_data_select,u8 );
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_get_fifo_data_output_reg,u8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_temp,s8 *);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_xyzt,struct bma2x2_accel_data_temp*);
FAKE_VALUE_FUNC(BMA2x2_RETURN_FUNCTION_TYPE,bma2x2_read_accel_eight_resolution_xyzt,struct bma2x2_accel_eight_resolution_temp*);

#endif /* BCDS_BMA2X2_TH_HH_ */

/** ************************************************************************* */
