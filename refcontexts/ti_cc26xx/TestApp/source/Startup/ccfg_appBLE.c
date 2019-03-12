/******************************************************************************
 *  Filename:       ccfg.c
*  Revised:        $Date: 2016-09-26 11:02:06 +0200 (ma, 26 sep 2016) $
*  Revision:       $Revision: 17337 $
 *
 *  Description:    Customer Configuration for CC26xx device family (HW rev 2).
 *
 *  Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  NOTE :
 *  According to BCDS standards, the file has been modified.
 ******************************************************************************/
/*
 ===> READ THIS BEFORE MODIFYING THIS FILE


 ===> READ THIS BEFORE MODIFYING THIS FILE


 ===> READ THIS BEFORE MODIFYING THIS FILE


 The customer configuration area (ccfg section) is located at the end of the
 flash and reflect the hw configuration of the device. it is very important
 that it remains align with the version of driverlib you are using.
 all BLE project except sensor tag use the same configuration.
 Keeping the "#include <startup_files/ccfg.c>" guarantee that your project using
 driverlib and the ccfg area will be align.

 you can modify it if you want, the recommend way will be to remove the
 bellow include, copy the content of the <startup_files/ccfg.c> file in this
 file and rebuild.

 ==> KEEP IN MIND that if you do so, be sure that any further update of the
 driverlib must be align with your modified version of ccfg area. */

#include <stdint.h>
#include <inc/hw_types.h>
#include <inc/hw_ccfg.h>
#include <inc/hw_ccfg_simple_struct.h>

/* NOTE : This file is modified as per instructions given above
 * ie., copied the content of <startup_files/ccfg.c>
 * modified and built.
 * Modification purpose: For release build
 */

/*****************************************************************************

 Introduction

 This file contains fields used by Boot ROM, startup code, and SW radio
 stacks to configure chip behavior.

 Fields are documented in more details in hw_ccfg.h and CCFG.html in
 DriverLib documentation (doc_overview.html -> CPU Domain Memory Map -> CCFG).

 *******************************************************************************/

/******************************************************************************

 Set the values of the individual bit fields.

 *******************************************************************************/

/****************************************
 Alternative DC/DC setting
 *****************************************/

/** Disable alternate DC/DC settings */
/*
 *         Configuration               |   Macro Value
 *         ------------                |   ---------
 *     Enable Alternate DCDC setting   |   0X0
 *     Disable Alternate DCDC setting  |   0X1
 *
 *     @note DEFAULT VALUE     Alternate DCDC setting enabled
 */
#define SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_ALT_DCDC_SETTING 0x0

/** Minimum voltage to use for alternative DCDC setting */
/*  Voltage = (28 + ALT_DCDC_VMIN)/16:
 *    0: 1.75V
 *    1: 1.8125V
 *    ...
 *    14: 2.625V
 *    15: 2.6875V
 */
#define SET_CCFG_MODE_CONF_1_ALT_DCDC_VMIN              0x8        /**< 2.25V */

/** Enable/disable DC/DC dithering for alternative DCDC setting */
/*
 *         Configuration       |   Macro Value
 *         ------------        |   ---------
 *     Enable DC/DC dithering  |   0X1
 *     Disable DC/DC dithering |   0X0
 *
 *     @note DEFAULT VALUE     disable DC/DC dithering
 */
#define SET_CCFG_MODE_CONF_1_ALT_DCDC_DITHER_EN         0x0

/** Inductor peak current for alternative DCDC setting */
/*    0: 31mA (min)
 *    ...
 *    4: Typical 47mA
 *    ...
 *    7: 59mA (max)
 */
#define SET_CCFG_MODE_CONF_1_ALT_DCDC_IPEAK             0x2        /**< 39mA */

/****************************************
 XOSC override settings
 *****************************************/

/** Disable flag for XOSC override functionality */
/*
 *         Configuration   |   Macro Value
 *         ------------    |   ---------
 *     Enable override     |   0X0
 *     Disable override    |   0X1
 *
 *     @note DEFAULT VALUE     Disable override
 */
#define SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_XOSC_OVR        0x1

/** Signed delta value for IBIAS_INIT */
#define SET_CCFG_MODE_CONF_1_DELTA_IBIAS_INIT           0x0

/** Signed delta value for IBIAS_OFFSET */
#define SET_CCFG_MODE_CONF_1_DELTA_IBIAS_OFFSET         0x0

/** Unsigned value of maximum XOSC startup time in units of 100us */
#define SET_CCFG_MODE_CONF_1_XOSC_MAX_START             0x10       /**< 1600us */

/****************************************
 Power settings
 *****************************************/

/** Signed delta value to apply to the VDDR_TRIM_SLEEP target, minus one */
/*    0x8 (-8) : Delta = -7
 *    ...
 *    0xF (-1) : Delta = 0
 *    0x0 (0) : Delta = +1
 *    ...
 *    0x7 (7) : Delta = +8
 */
#define SET_CCFG_MODE_CONF_VDDR_TRIM_SLEEP_DELTA        0xF

/** DC/DC during recharge in powerdown */
/*
 *         Configuration                               |   Macro Value
 *         ------------                                |   ---------
 *     Use DC/DC during recharge in powerdown          |   0X0
 *     Do not Use DC/DC during recharge in powerdown   |   0X1
 *
 */
#define SET_CCFG_MODE_CONF_DCDC_RECHARGE                0x0

/** DC/DC in active mode */
/*
 *         Configuration                     |   Macro Value
 *         ------------                      |   ---------
 *     Use DC/DC during active mode          |   0X0
 *     Do not Use DC/DC during active mode   |   0X1
 *
 */
#define SET_CCFG_MODE_CONF_DCDC_ACTIVE                  0x0

/** VDDS BOD level */
/*
 *         Configuration                |   Macro Value
 *         ------------                 |   ---------
 *      VDDS BOD level is 2.0V          |   0X0
 * (necessary for ext load              |
 * mode, or for maximum PA output power |
 * on CC13xx)                           |
 *                                      |
 *      VDDS BOD level is 1.8V          |   0X1
 * (or 1.65V for external regulator     |
 * mode)                                |
 *
 *     @note DEFAULT VALUE     VDDS BOD level is 1.8V
 */
#define SET_CCFG_MODE_CONF_VDDS_BOD_LEVEL               0x1

/** Value of VDDR capacitance
 * @note Unsigned 8-bit integer, representing the minimum decoupling capacitance (worst case) on VDDR, in units of 100nF.
 * This should take into account capacitor tolerance and voltage dependent capacitance variation.
 * This bit affects the recharge period calculation when going into powerdown or standby.
 */
#define SET_CCFG_MODE_CONF_VDDR_CAP                     0x3A

/****************************************
 Clock settings
 *****************************************/

/** SCLK LF option */
/*
 *         Configuration                |   Macro Value
 *         ------------                 |   ---------
 *      Low frequency clock derived     |
 *      from High Frequency XOSC        |   0X0
 *                                      |
 *      External LF clock               |   0X1
 *                                      |
 *      Low frequency XOSC              |   0x2
 *                                      |
 *      Low frequency RCOSC             |   0x3
 *
 */
#define SET_CCFG_MODE_CONF_SCLK_LF_OPTION               0x2

#ifndef SET_CCFG_MODE_CONF_VDDR_TRIM_SLEEP_TC
#define SET_CCFG_MODE_CONF_VDDR_TRIM_SLEEP_TC           0x1        // Temperature compensation on VDDR sleep trim disabled (default)
// #define SET_CCFG_MODE_CONF_VDDR_TRIM_SLEEP_TC        0x0        // Temperature compensation on VDDR sleep trim enabled
#endif

/** Enable modification (delta) to XOSC cap-array */
/*
 *         Configuration             |   Macro Value
 *         ------------              |   ---------
 *      Apply cap-array delta        |   0X0
 *      Do not apply cap-array delta |   0X1
 *
 *     @note DEFAULT VALUE     Do not apply cap-array delta
 */
#define SET_CCFG_MODE_CONF_XOSC_CAP_MOD                 0x1

/** Signed 8-bit value, directly modifying trimmed XOSC cap-array value */
#define SET_CCFG_MODE_CONF_XOSC_CAPARRAY_DELTA          0xFF

/** DIO number if using external LF clock */
#define SET_CCFG_EXT_LF_CLK_DIO                         0x01

/** RTC increment representing the external LF clock frequency */
#define SET_CCFG_EXT_LF_CLK_RTC_INCREMENT               0x800000

/** Special HF clock source setting */
/*
 *         Configuration                |   Macro Value
 *         ------------                 |   ---------
 *      Use HPOSC as HF source          |
 *   (if executing on a HPOSC chip,     |   0x1
 *    otherwise using default (=0x3))   |
 *                                      |
 *      HF source is a 48 MHz xtal      |   0x2
 *                                      |
 * HF source is a 24 MHz xtal (default) |   0x3
 *                                      |
 */
#define SET_CCFG_MODE_CONF_XOSC_FREQ               		0x3

/****************************************
 Bootloader settings
 *****************************************/

/** Bootloader enable */
/*
 *         Configuration    |   Macro Value
 *         ------------     |   ---------
 *           Disable        |   0x00
 *                          |
 *           Enable         |   0xC5
 *
 *     @note DEFAULT VALUE     Enable
 *
 *     @note For Release build, this register must be *disabled* to disallow access to
 *     Flash Contents through Bootloader interface
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_BL_CONFIG_BOOTLOADER_ENABLE            0xC5
#else
#define SET_CCFG_BL_CONFIG_BOOTLOADER_ENABLE            0x00
#endif

/** Bootloader pin level configuration */
/*
 *         Configuration    |   Macro Value
 *         ------------     |   ---------
 *      Active low          |   0x0
 *                          |
 *      Active high         |   0x1
 *
 *     @note DEFAULT VALUE     Active high
 */
#define SET_CCFG_BL_CONFIG_BL_LEVEL                     0x1

/** DIO number for bootloader failure analysis */
#define SET_CCFG_BL_CONFIG_BL_PIN_NUMBER                0xFF

/** Enable of Bootloader failure analysis */
/*
 *      Configuration   |   Macro Value
 *      ------------    |   ---------
 *      Enabled         |   0xC5
 *                      |
 *      Disabled        |   0xFF
 *
 *     @note DEFAULT VALUE     Disabled
 *
 *     @note For Release build, this register must be set to 0x00 to disallow access to
 *     Flash Contents through Bootloader interface
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_BL_CONFIG_BL_ENABLE                    0xFF
#else
#define SET_CCFG_BL_CONFIG_BL_ENABLE                    0x00
#endif

/****************************************
 Debug access settings
 *****************************************/

/** TI FA (Failure Analysis) */
/*
 *         Configuration                    |   Macro Value
 *         ------------                     |   ---------
 *     Disable unlocking of TI FA option    |
 *     with the unlock code                 |   0X00
 *                                          |
 *     Enable unlocking of TI FA option     |
 *     with the unlock code                 |   0XC5
 *
 *     @note DEFAULT VALUE     Enable unlocking of TI FA option with the unlock code
 *
 *     @note For release build, this register must be set to 0x00 to disallow
 *     failure analysis access by TI
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_CCFG_TI_OPTIONS_TI_FA_ENABLE           0xC5
#else
#define SET_CCFG_CCFG_TI_OPTIONS_TI_FA_ENABLE           0x00
#endif

/** CPU DAP access control */
/*
 *         Configuration        |   Macro Value
 *         ------------         |   ---------
 *      Access disabled         |   0x00
 *                              |
 *      Access enabled if also  |
 *      enabled in FCFG         |   0xC5
 *
 *     @note DEFAULT VALUE     Access enabled if also enabled in FCFG
 *
 *     @note For release build, this register must be set to 0x00 to disallow access
 *     to this module through JTAG
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_CCFG_TAP_DAP_0_CPU_DAP_ENABLE          0xC5
#else
#define SET_CCFG_CCFG_TAP_DAP_0_CPU_DAP_ENABLE          0x00
#endif

/** PRCM TAP access control */
/*
 *         Configuration        |   Macro Value
 *         ------------         |   ---------
 *      Access disabled         |   0x00
 *                              |
 *      Access enabled if also  |
 *      enabled in FCFG         |   0xC5
 *
 *     @note DEFAULT VALUE     Access enabled if also enabled in FCFG
 *
 *     @note For release build, this register must be set to 0x00 to disallow access
 *     to this module through JTAG
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_CCFG_TAP_DAP_0_PRCM_TAP_ENABLE         0xC5
#else
#define SET_CCFG_CCFG_TAP_DAP_0_PRCM_TAP_ENABLE         0x00
#endif

/** TEST TAP access control */
/*
 *         Configuration        |   Macro Value
 *         ------------         |   ---------
 *      Access disabled         |   0x00
 *                              |
 *      Access enabled if also  |
 *      enabled in FCFG         |   0xC5
 *
 *     @note DEFAULT VALUE     Access enabled if also enabled in FCFG
 *
 *     @note For release build, this register must be set to 0x00 to disallow access
 *     to this module through JTAG
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_CCFG_TAP_DAP_0_TEST_TAP_ENABLE         0xC5
#else
#define SET_CCFG_CCFG_TAP_DAP_0_TEST_TAP_ENABLE         0x00
#endif

/** PBIST2 TAP access control */
/*
 *         Configuration        |   Macro Value
 *         ------------         |   ---------
 *      Access disabled         |   0x00
 *                              |
 *      Access enabled if also  |
 *      enabled in FCFG         |   0xC5
 *
 *     @note DEFAULT VALUE     Access enabled if also enabled in FCFG
 *
 *     @note For release build, this register must be set to 0x00 to disallow access
 *     to this module through JTAG
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_CCFG_TAP_DAP_1_PBIST2_TAP_ENABLE       0xC5
#else
#define SET_CCFG_CCFG_TAP_DAP_1_PBIST2_TAP_ENABLE       0x00
#endif

/** PBIST1 TAP access control */
/*
 *         Configuration        |   Macro Value
 *         ------------         |   ---------
 *      Access disabled         |   0x00
 *                              |
 *      Access enabled if also  |
 *      enabled in FCFG         |   0xC5
 *
 *     @note DEFAULT VALUE     Access enabled if also enabled in FCFG
 *
 *     @note For release build, this register must be set to 0x00 to disallow access
 *     to this module through JTAG
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_CCFG_TAP_DAP_1_PBIST1_TAP_ENABLE       0xC5
#else
#define SET_CCFG_CCFG_TAP_DAP_1_PBIST1_TAP_ENABLE       0x00
#endif

/** WUC TAP access control */
/*
 *         Configuration        |   Macro Value
 *         ------------         |   ---------
 *      Access disabled         |   0x00
 *                              |
 *      Access enabled if also  |
 *      enabled in FCFG         |   0xC5
 *
 *     @note DEFAULT VALUE     Access enabled if also enabled in FCFG
 *
 *     @note For release build, this register must be set to 0x00 to disallow access
 *     to this module through JTAG
 */
#ifndef RELEASE_BUILD
#define SET_CCFG_CCFG_TAP_DAP_1_WUC_TAP_ENABLE          0xC5
#else
#define SET_CCFG_CCFG_TAP_DAP_1_WUC_TAP_ENABLE          0x00
#endif

/**
 * Alternative IEEE 802.15.4 MAC address
 *
 * @note If different from 0xFFFFFFFF it overrides the value in FCFG.
 */
#define SET_CCFG_IEEE_MAC_0                             0xFFFFFFFF /**< Bits [31:0] */
#define SET_CCFG_IEEE_MAC_1                             0xFFFFFFFF /**< Bits [63:32] */

/**
 * Alternative BLE address
 *
 * @note If different from 0xFFFFFFFF it overrides the value in FCFG.
 */
#define SET_CCFG_IEEE_BLE_0                             0xFFFFFFFF /**< Bits [31:0] */
#define SET_CCFG_IEEE_BLE_1                             0xFFFFFFFF /**< Bits [63:32] */

/****************************************
 Flash erase settings
 *****************************************/
/** Chip erase */
/*
 *              Configuration           |   Macro Value
 *              ------------            |   ---------
 *      Any chip erase request detected |
 *      during boot will be ignored     |   0x0
 *                                      |
 *      Any chip erase request detected |
 *      during boot will be performed   |
 *      by the boot FW                  |   0x1
 *
 *     @note DEFAULT VALUE     Any chip erase request detected during boot will be performed by the boot FW
 */
#define SET_CCFG_ERASE_CONF_CHIP_ERASE_DIS_N            0x1

/** Bank erase (also known as, Mass erase) */
/*
 *              Configuration                       |   Macro Value
 *              ------------                        |   ---------
 *      Disable the boot loader bank erase function |   0x0
 *                                                  |
 *      Enable the boot loader bank erase function  |   0x1
 *
 *     @note DEFAULT VALUE    Enable the boot loader bank erase function
 */
#define SET_CCFG_ERASE_CONF_BANK_ERASE_DIS_N            0x1

/****************************************
 Flash image valid
 *****************************************/
/*
 *              Configuration                       |   Macro Value
 *              ------------                        |   ---------
 *          Flash image is valid                    |   0x00000000
 *                                                  |
 *      Flash image is invalid, call bootloader     |   <non-zero>
 *
 */
#define SET_CCFG_IMAGE_VALID_CONF_IMAGE_VALID           0x00000000

/****************************************
 Flash sector write protection
 *****************************************/

/** Protect sectors 0-31 */
/**
 * @note  Each bit write protects one flash sector from being both programmed and erased.
 * Bit must be set to 0 in order to enable sector write protect.
 * The sector write protection is enabled by setting corresponding bit in the FSM_BSLE0- and FSM_BSLP0-registers in the flash controller.
 */
#define SET_CCFG_CCFG_PROT_31_0                         0xFFFFFFFF

/** Protect sectors 32-63 */
/**
 * @note  Each bit write protects one flash sector from being both programmed and erased.
 * Bit must be set to 0 in order to enable sector write protect.
 * The sector write protection is enabled by setting corresponding bit in the FSM_BSLE1- and FSM_BSLP1-registers in the flash controller.
 * These registers are not in use by CC26xx.
 */
#define SET_CCFG_CCFG_PROT_63_32                        0xFFFFFFFF

/** Protect sectors 64-95 */
/**
 * @note  Each bit write protects one flash sector from being both programmed and erased.
 * Bit must be set to 0 in order to enable sector write protect. Not in use on CC26xx
 */
#define SET_CCFG_CCFG_PROT_95_64                        0xFFFFFFFF

/** Protect sectors 96-127 */
/**
 * @note  Each bit write protects one flash sector from being both programmed and erased.
 * Bit must be set to 0 in order to enable sector write protect. Not in use on CC26xx
 */
#define SET_CCFG_CCFG_PROT_127_96                       0xFFFFFFFF

/**
 * CCFG values that should not be modified.
 *
 * @note The values below are not modified. Values are same as present in "startup_files/ccfg.c"
 */
#define SET_CCFG_SIZE_AND_DIS_FLAGS_SIZE_OF_CCFG        0x0058
#define SET_CCFG_SIZE_AND_DIS_FLAGS_DISABLE_FLAGS       (CCFG_SIZE_AND_DIS_FLAGS_DISABLE_FLAGS_M >> CCFG_SIZE_AND_DIS_FLAGS_DISABLE_FLAGS_S)

//#####################################
// Select TCXO
//#####################################
#ifndef SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_TCXO
#define SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_TCXO            0x1    // Disable TCXO
// #define SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_TCXO         0x0    // Enable TXCO
#endif

//#####################################
// Select between cache or GPRAM
//#####################################
#ifndef SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_GPRAM
// #define SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_GPRAM        0x0        // Cache is disabled and GPRAM is available at 0x11000000-0x11001FFF
#define SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_GPRAM           0x1        // Cache is enabled and GPRAM is disabled (unavailable)
#endif

#define SET_CCFG_MODE_CONF_VDDR_EXT_LOAD                0x1
#define SET_CCFG_MODE_CONF_RTC_COMP                     0x1
#define SET_CCFG_MODE_CONF_HF_COMP                      0x1

#define SET_CCFG_VOLT_LOAD_0_VDDR_EXT_TP45              0xFF
#define SET_CCFG_VOLT_LOAD_0_VDDR_EXT_TP25              0xFF
#define SET_CCFG_VOLT_LOAD_0_VDDR_EXT_TP5               0xFF
#define SET_CCFG_VOLT_LOAD_0_VDDR_EXT_TM15              0xFF

#define SET_CCFG_VOLT_LOAD_1_VDDR_EXT_TP125             0xFF
#define SET_CCFG_VOLT_LOAD_1_VDDR_EXT_TP105             0xFF
#define SET_CCFG_VOLT_LOAD_1_VDDR_EXT_TP85              0xFF
#define SET_CCFG_VOLT_LOAD_1_VDDR_EXT_TP65              0xFF

#define SET_CCFG_RTC_OFFSET_RTC_COMP_P0                 0xFFFF
#define SET_CCFG_RTC_OFFSET_RTC_COMP_P1                 0xFF
#define SET_CCFG_RTC_OFFSET_RTC_COMP_P2                 0xFF

#define SET_CCFG_FREQ_OFFSET_HF_COMP_P0                 0xFFFF
#define SET_CCFG_FREQ_OFFSET_HF_COMP_P1                 0xFF
#define SET_CCFG_FREQ_OFFSET_HF_COMP_P2                 0xFF

/**
 * Concatenate bit fields to words.
 * DO NOT EDIT!
 *
 * @note The values below are not modified. Values are same as present in "startup_files/ccfg.c"
 */
#define DEFAULT_CCFG_EXT_LF_CLK        ( \
	 ((((uint32_t)( SET_CCFG_EXT_LF_CLK_DIO                          )) << CCFG_EXT_LF_CLK_DIO_S                          ) | ~CCFG_EXT_LF_CLK_DIO_M                          ) & \
	 ((((uint32_t)( SET_CCFG_EXT_LF_CLK_RTC_INCREMENT                )) << CCFG_EXT_LF_CLK_RTC_INCREMENT_S                ) | ~CCFG_EXT_LF_CLK_RTC_INCREMENT_M                ) )

#define DEFAULT_CCFG_MODE_CONF_1         ( \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_1_ALT_DCDC_VMIN               )) << CCFG_MODE_CONF_1_ALT_DCDC_VMIN_S               ) | ~CCFG_MODE_CONF_1_ALT_DCDC_VMIN_M               ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_1_ALT_DCDC_DITHER_EN          )) << CCFG_MODE_CONF_1_ALT_DCDC_DITHER_EN_S          ) | ~CCFG_MODE_CONF_1_ALT_DCDC_DITHER_EN_M          ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_1_ALT_DCDC_IPEAK              )) << CCFG_MODE_CONF_1_ALT_DCDC_IPEAK_S              ) | ~CCFG_MODE_CONF_1_ALT_DCDC_IPEAK_M              ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_1_DELTA_IBIAS_INIT            )) << CCFG_MODE_CONF_1_DELTA_IBIAS_INIT_S            ) | ~CCFG_MODE_CONF_1_DELTA_IBIAS_INIT_M            ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_1_DELTA_IBIAS_OFFSET          )) << CCFG_MODE_CONF_1_DELTA_IBIAS_OFFSET_S          ) | ~CCFG_MODE_CONF_1_DELTA_IBIAS_OFFSET_M          ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_1_XOSC_MAX_START              )) << CCFG_MODE_CONF_1_XOSC_MAX_START_S              ) | ~CCFG_MODE_CONF_1_XOSC_MAX_START_M              ) )

#define DEFAULT_CCFG_SIZE_AND_DIS_FLAGS  ( \
	 ((((uint32_t)( SET_CCFG_SIZE_AND_DIS_FLAGS_SIZE_OF_CCFG         )) << CCFG_SIZE_AND_DIS_FLAGS_SIZE_OF_CCFG_S         ) | ~CCFG_SIZE_AND_DIS_FLAGS_SIZE_OF_CCFG_M         ) & \
	 ((((uint32_t)( SET_CCFG_SIZE_AND_DIS_FLAGS_DISABLE_FLAGS        )) << CCFG_SIZE_AND_DIS_FLAGS_DISABLE_FLAGS_S        ) | ~CCFG_SIZE_AND_DIS_FLAGS_DISABLE_FLAGS_M        ) & \
	 ((((uint32_t)( SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_TCXO             )) << CCFG_SIZE_AND_DIS_FLAGS_DIS_TCXO_S             ) | ~CCFG_SIZE_AND_DIS_FLAGS_DIS_TCXO_M             ) & \
	 ((((uint32_t)( SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_GPRAM            )) << CCFG_SIZE_AND_DIS_FLAGS_DIS_GPRAM_S            ) | ~CCFG_SIZE_AND_DIS_FLAGS_DIS_GPRAM_M            ) & \
	 ((((uint32_t)( SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_ALT_DCDC_SETTING )) << CCFG_SIZE_AND_DIS_FLAGS_DIS_ALT_DCDC_SETTING_S ) | ~CCFG_SIZE_AND_DIS_FLAGS_DIS_ALT_DCDC_SETTING_M ) & \
	 ((((uint32_t)( SET_CCFG_SIZE_AND_DIS_FLAGS_DIS_XOSC_OVR         )) << CCFG_SIZE_AND_DIS_FLAGS_DIS_XOSC_OVR_S         ) | ~CCFG_SIZE_AND_DIS_FLAGS_DIS_XOSC_OVR_M         ) )

#define DEFAULT_CCFG_MODE_CONF           ( \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_VDDR_TRIM_SLEEP_DELTA         )) << CCFG_MODE_CONF_VDDR_TRIM_SLEEP_DELTA_S         ) | ~CCFG_MODE_CONF_VDDR_TRIM_SLEEP_DELTA_M         ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_DCDC_RECHARGE                 )) << CCFG_MODE_CONF_DCDC_RECHARGE_S                 ) | ~CCFG_MODE_CONF_DCDC_RECHARGE_M                 ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_DCDC_ACTIVE                   )) << CCFG_MODE_CONF_DCDC_ACTIVE_S                   ) | ~CCFG_MODE_CONF_DCDC_ACTIVE_M                   ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_VDDR_EXT_LOAD                 )) << CCFG_MODE_CONF_VDDR_EXT_LOAD_S                 ) | ~CCFG_MODE_CONF_VDDR_EXT_LOAD_M                 ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_VDDS_BOD_LEVEL                )) << CCFG_MODE_CONF_VDDS_BOD_LEVEL_S                ) | ~CCFG_MODE_CONF_VDDS_BOD_LEVEL_M                ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_SCLK_LF_OPTION                )) << CCFG_MODE_CONF_SCLK_LF_OPTION_S                ) | ~CCFG_MODE_CONF_SCLK_LF_OPTION_M                ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_VDDR_TRIM_SLEEP_TC            )) << CCFG_MODE_CONF_VDDR_TRIM_SLEEP_TC_S            ) | ~CCFG_MODE_CONF_VDDR_TRIM_SLEEP_TC_M            ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_RTC_COMP                      )) << CCFG_MODE_CONF_RTC_COMP_S                      ) | ~CCFG_MODE_CONF_RTC_COMP_M                      ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_XOSC_FREQ                     )) << CCFG_MODE_CONF_XOSC_FREQ_S                     ) | ~CCFG_MODE_CONF_XOSC_FREQ_M                     ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_XOSC_CAP_MOD                  )) << CCFG_MODE_CONF_XOSC_CAP_MOD_S                  ) | ~CCFG_MODE_CONF_XOSC_CAP_MOD_M                  ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_HF_COMP                       )) << CCFG_MODE_CONF_HF_COMP_S                       ) | ~CCFG_MODE_CONF_HF_COMP_M                       ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_XOSC_CAPARRAY_DELTA           )) << CCFG_MODE_CONF_XOSC_CAPARRAY_DELTA_S           ) | ~CCFG_MODE_CONF_XOSC_CAPARRAY_DELTA_M           ) & \
	 ((((uint32_t)( SET_CCFG_MODE_CONF_VDDR_CAP                      )) << CCFG_MODE_CONF_VDDR_CAP_S                      ) | ~CCFG_MODE_CONF_VDDR_CAP_M                      ) )

#define DEFAULT_CCFG_VOLT_LOAD_0         ( \
	 ((((uint32_t)( SET_CCFG_VOLT_LOAD_0_VDDR_EXT_TP45               )) << CCFG_VOLT_LOAD_0_VDDR_EXT_TP45_S               ) | ~CCFG_VOLT_LOAD_0_VDDR_EXT_TP45_M               ) & \
	 ((((uint32_t)( SET_CCFG_VOLT_LOAD_0_VDDR_EXT_TP25               )) << CCFG_VOLT_LOAD_0_VDDR_EXT_TP25_S               ) | ~CCFG_VOLT_LOAD_0_VDDR_EXT_TP25_M               ) & \
	 ((((uint32_t)( SET_CCFG_VOLT_LOAD_0_VDDR_EXT_TP5                )) << CCFG_VOLT_LOAD_0_VDDR_EXT_TP5_S                ) | ~CCFG_VOLT_LOAD_0_VDDR_EXT_TP5_M                ) & \
	 ((((uint32_t)( SET_CCFG_VOLT_LOAD_0_VDDR_EXT_TM15               )) << CCFG_VOLT_LOAD_0_VDDR_EXT_TM15_S               ) | ~CCFG_VOLT_LOAD_0_VDDR_EXT_TM15_M               ) )

#define DEFAULT_CCFG_VOLT_LOAD_1         ( \
	 ((((uint32_t)( SET_CCFG_VOLT_LOAD_1_VDDR_EXT_TP125              )) << CCFG_VOLT_LOAD_1_VDDR_EXT_TP125_S              ) | ~CCFG_VOLT_LOAD_1_VDDR_EXT_TP125_M              ) & \
	 ((((uint32_t)( SET_CCFG_VOLT_LOAD_1_VDDR_EXT_TP105              )) << CCFG_VOLT_LOAD_1_VDDR_EXT_TP105_S              ) | ~CCFG_VOLT_LOAD_1_VDDR_EXT_TP105_M              ) & \
	 ((((uint32_t)( SET_CCFG_VOLT_LOAD_1_VDDR_EXT_TP85               )) << CCFG_VOLT_LOAD_1_VDDR_EXT_TP85_S               ) | ~CCFG_VOLT_LOAD_1_VDDR_EXT_TP85_M               ) & \
	 ((((uint32_t)( SET_CCFG_VOLT_LOAD_1_VDDR_EXT_TP65               )) << CCFG_VOLT_LOAD_1_VDDR_EXT_TP65_S               ) | ~CCFG_VOLT_LOAD_1_VDDR_EXT_TP65_M               ) )

#define DEFAULT_CCFG_RTC_OFFSET          ( \
	 ((((uint32_t)( SET_CCFG_RTC_OFFSET_RTC_COMP_P0                  )) << CCFG_RTC_OFFSET_RTC_COMP_P0_S                  ) | ~CCFG_RTC_OFFSET_RTC_COMP_P0_M                  ) & \
	 ((((uint32_t)( SET_CCFG_RTC_OFFSET_RTC_COMP_P1                  )) << CCFG_RTC_OFFSET_RTC_COMP_P1_S                  ) | ~CCFG_RTC_OFFSET_RTC_COMP_P1_M                  ) & \
	 ((((uint32_t)( SET_CCFG_RTC_OFFSET_RTC_COMP_P2                  )) << CCFG_RTC_OFFSET_RTC_COMP_P2_S                  ) | ~CCFG_RTC_OFFSET_RTC_COMP_P2_M                  ) )

#define DEFAULT_CCFG_FREQ_OFFSET         ( \
	 ((((uint32_t)( SET_CCFG_FREQ_OFFSET_HF_COMP_P0                  )) << CCFG_FREQ_OFFSET_HF_COMP_P0_S                  ) | ~CCFG_FREQ_OFFSET_HF_COMP_P0_M                  ) & \
	 ((((uint32_t)( SET_CCFG_FREQ_OFFSET_HF_COMP_P1                  )) << CCFG_FREQ_OFFSET_HF_COMP_P1_S                  ) | ~CCFG_FREQ_OFFSET_HF_COMP_P1_M                  ) & \
	 ((((uint32_t)( SET_CCFG_FREQ_OFFSET_HF_COMP_P2                  )) << CCFG_FREQ_OFFSET_HF_COMP_P2_S                  ) | ~CCFG_FREQ_OFFSET_HF_COMP_P2_M                  ) )

#define DEFAULT_CCFG_IEEE_MAC_0          SET_CCFG_IEEE_MAC_0
#define DEFAULT_CCFG_IEEE_MAC_1          SET_CCFG_IEEE_MAC_1
#define DEFAULT_CCFG_IEEE_BLE_0          SET_CCFG_IEEE_BLE_0
#define DEFAULT_CCFG_IEEE_BLE_1          SET_CCFG_IEEE_BLE_1

#define DEFAULT_CCFG_BL_CONFIG           ( \
	 ((((uint32_t)( SET_CCFG_BL_CONFIG_BOOTLOADER_ENABLE             )) << CCFG_BL_CONFIG_BOOTLOADER_ENABLE_S             ) | ~CCFG_BL_CONFIG_BOOTLOADER_ENABLE_M             ) & \
	 ((((uint32_t)( SET_CCFG_BL_CONFIG_BL_LEVEL                      )) << CCFG_BL_CONFIG_BL_LEVEL_S                      ) | ~CCFG_BL_CONFIG_BL_LEVEL_M                      ) & \
	 ((((uint32_t)( SET_CCFG_BL_CONFIG_BL_PIN_NUMBER                 )) << CCFG_BL_CONFIG_BL_PIN_NUMBER_S                 ) | ~CCFG_BL_CONFIG_BL_PIN_NUMBER_M                 ) & \
	 ((((uint32_t)( SET_CCFG_BL_CONFIG_BL_ENABLE                     )) << CCFG_BL_CONFIG_BL_ENABLE_S                     ) | ~CCFG_BL_CONFIG_BL_ENABLE_M                     ) )

#define DEFAULT_CCFG_ERASE_CONF          ( \
	 ((((uint32_t)( SET_CCFG_ERASE_CONF_CHIP_ERASE_DIS_N             )) << CCFG_ERASE_CONF_CHIP_ERASE_DIS_N_S             ) | ~CCFG_ERASE_CONF_CHIP_ERASE_DIS_N_M             ) & \
	 ((((uint32_t)( SET_CCFG_ERASE_CONF_BANK_ERASE_DIS_N             )) << CCFG_ERASE_CONF_BANK_ERASE_DIS_N_S             ) | ~CCFG_ERASE_CONF_BANK_ERASE_DIS_N_M             ) )

#define DEFAULT_CCFG_CCFG_TI_OPTIONS     ( \
	 ((((uint32_t)( SET_CCFG_CCFG_TI_OPTIONS_TI_FA_ENABLE            )) << CCFG_CCFG_TI_OPTIONS_TI_FA_ENABLE_S            ) | ~CCFG_CCFG_TI_OPTIONS_TI_FA_ENABLE_M            ) )

#define DEFAULT_CCFG_CCFG_TAP_DAP_0      ( \
	 ((((uint32_t)( SET_CCFG_CCFG_TAP_DAP_0_CPU_DAP_ENABLE           )) << CCFG_CCFG_TAP_DAP_0_CPU_DAP_ENABLE_S           ) | ~CCFG_CCFG_TAP_DAP_0_CPU_DAP_ENABLE_M           ) & \
	 ((((uint32_t)( SET_CCFG_CCFG_TAP_DAP_0_PRCM_TAP_ENABLE          )) << CCFG_CCFG_TAP_DAP_0_PRCM_TAP_ENABLE_S          ) | ~CCFG_CCFG_TAP_DAP_0_PRCM_TAP_ENABLE_M          ) & \
	 ((((uint32_t)( SET_CCFG_CCFG_TAP_DAP_0_TEST_TAP_ENABLE          )) << CCFG_CCFG_TAP_DAP_0_TEST_TAP_ENABLE_S          ) | ~CCFG_CCFG_TAP_DAP_0_TEST_TAP_ENABLE_M          ) )

#define DEFAULT_CCFG_CCFG_TAP_DAP_1      ( \
	 ((((uint32_t)( SET_CCFG_CCFG_TAP_DAP_1_PBIST2_TAP_ENABLE        )) << CCFG_CCFG_TAP_DAP_1_PBIST2_TAP_ENABLE_S        ) | ~CCFG_CCFG_TAP_DAP_1_PBIST2_TAP_ENABLE_M        ) & \
	 ((((uint32_t)( SET_CCFG_CCFG_TAP_DAP_1_PBIST1_TAP_ENABLE        )) << CCFG_CCFG_TAP_DAP_1_PBIST1_TAP_ENABLE_S        ) | ~CCFG_CCFG_TAP_DAP_1_PBIST1_TAP_ENABLE_M        ) & \
	 ((((uint32_t)( SET_CCFG_CCFG_TAP_DAP_1_WUC_TAP_ENABLE           )) << CCFG_CCFG_TAP_DAP_1_WUC_TAP_ENABLE_S           ) | ~CCFG_CCFG_TAP_DAP_1_WUC_TAP_ENABLE_M           ) )

#define DEFAULT_CCFG_IMAGE_VALID_CONF    SET_CCFG_IMAGE_VALID_CONF_IMAGE_VALID

#define DEFAULT_CCFG_CCFG_PROT_31_0      SET_CCFG_CCFG_PROT_31_0
#define DEFAULT_CCFG_CCFG_PROT_63_32     SET_CCFG_CCFG_PROT_63_32
#define DEFAULT_CCFG_CCFG_PROT_95_64     SET_CCFG_CCFG_PROT_95_64
#define DEFAULT_CCFG_CCFG_PROT_127_96    SET_CCFG_CCFG_PROT_127_96

/**
 *  Customer configuration area in lock page
 */
#if defined(__IAR_SYSTEMS_ICC__)
__root const ccfg_t __ccfg @ ".ccfg" =
#elif defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(__ccfg, ".ccfg")
#pragma RETAIN(__ccfg)
const ccfg_t __ccfg =
#else
const ccfg_t __ccfg __attribute__((section(".ccfg"))) __attribute__((used)) =
        #endif
        {
        DEFAULT_CCFG_EXT_LF_CLK, /**< Mapped to address 0x50003FA8 (0x50003xxx maps to last sector in FLASH. Independent of FLASH size) */
        DEFAULT_CCFG_MODE_CONF_1, /**< Mapped to address 0x50003FAC */
        DEFAULT_CCFG_SIZE_AND_DIS_FLAGS, /**< Mapped to address 0x50003FB0 */
        DEFAULT_CCFG_MODE_CONF, /**< Mapped to address 0x50003FB4 */
        DEFAULT_CCFG_VOLT_LOAD_0, /**< Mapped to address 0x50003FB8 */
        DEFAULT_CCFG_VOLT_LOAD_1, /**< Mapped to address 0x50003FBC */
        DEFAULT_CCFG_RTC_OFFSET, /**< Mapped to address 0x50003FC0 */
        DEFAULT_CCFG_FREQ_OFFSET, /**< Mapped to address 0x50003FC4 */
        DEFAULT_CCFG_IEEE_MAC_0, /**< Mapped to address 0x50003FC8 */
        DEFAULT_CCFG_IEEE_MAC_1, /**< Mapped to address 0x50003FCC */
        DEFAULT_CCFG_IEEE_BLE_0, /**< Mapped to address 0x50003FD0 */
        DEFAULT_CCFG_IEEE_BLE_1, /**< Mapped to address 0x50003FD4 */
        DEFAULT_CCFG_BL_CONFIG, /**< Mapped to address 0x50003FD8 */
        DEFAULT_CCFG_ERASE_CONF, /**< Mapped to address 0x50003FDC */
        DEFAULT_CCFG_CCFG_TI_OPTIONS, /**< Mapped to address 0x50003FE0 */
        DEFAULT_CCFG_CCFG_TAP_DAP_0, /**< Mapped to address 0x50003FE4 */
        DEFAULT_CCFG_CCFG_TAP_DAP_1, /**< Mapped to address 0x50003FE8 */
        DEFAULT_CCFG_IMAGE_VALID_CONF, /**< Mapped to address 0x50003FEC */
        DEFAULT_CCFG_CCFG_PROT_31_0, /**< Mapped to address 0x50003FF0 */
        DEFAULT_CCFG_CCFG_PROT_63_32, /**< Mapped to address 0x50003FF4 */
        DEFAULT_CCFG_CCFG_PROT_95_64, /**< Mapped to address 0x50003FF8 */
        DEFAULT_CCFG_CCFG_PROT_127_96, /**< Mapped to address 0x50003FFC */
        };
