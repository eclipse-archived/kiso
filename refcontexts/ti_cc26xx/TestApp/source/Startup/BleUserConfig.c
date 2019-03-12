/******************************************************************************

 @file       ble_user_config.c

 @brief This file contains user configurable variables for the BLE
        Application.

 Group: CMCU, SCS
 Target Device: CC2640R2

 ******************************************************************************
 
 Copyright (c) 2016-2017, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: simplelink_cc2640r2_sdk_01_50_00_58
 Release Date: 2017-10-17 18:09:51
 
 NOTE :
 According to BCDS standards, the file has been modified.
 *****************************************************************************/

/* own header files */

/* system header files */

/* additional interface header files */

#include "hal_types.h"
#include "BleUserConfig.h"
#include <ti/drivers/rf/RF.h>

#if defined(BLE_V42_FEATURES) && (BLE_V42_FEATURES & SECURE_CONNS_CFG)
#include "ecc/ECCROMCC26XX.h"
#endif //BLE_V42_FEATURES & SECURE_CONNS_CFG

#include <ti/drivers/crypto/CryptoCC26XX.h>
#include "TRNGCC26XX.h"

/* Macro definitions */

/** Tx Power */
#define NUM_TX_POWER_VALUES (sizeof( TxPowerTable ) / sizeof( txPwrVal_t ))

/** Default Tx Power Index */
#define DEFAULT_TX_POWER               7

/** Override NOP */
#define OVERRIDE_NOP                   0xC0000001

/* Local functions */

void driverTable_fnSpinlock( void );

/* Global variables */

/**
 * RF Override Registers
 * Note: Used with CMD_RADIO_SETUP; called at boot time and after wake.
 * Note: Must be in RAM as these overrides may need to be modified at runtime
 * based on temperature compensation, although it is possible this may
 * be automated in CM0 in PG2.0.
 */
#if defined( USE_FPGA )
  regOverride_t rfRegTbl[] = {
#if defined(CC26X2)
    // CC2642
    0x000151D0,
    0x00041110,
    0x00000083,
    0x014B02A3,
    0x00800403,
    0x80000303,
    0xFFFFFFFF };
#elif defined( CC26XX_R2 )
    HW_REG_OVERRIDE(0x6084, 0x05F8), /* RFC_RFE:SPARE0. Select R1-style gain table */
    0xFFFFFFFF };
#else // !R2
    // CC26x0
    0x00015164,
    0x00041110,
    0x00000083,
    0x016302A3,
    0x00800403,
    0x80000303,
    0xFFFFFFFF };
#endif // R2/CC26xx

#elif defined( CC26XX_R2 )
  regOverride_t rfRegTbl[] = {
    HW_REG_OVERRIDE(0x6084, 0x05F8), /* RFC_RFE:SPARE0. Select R1-style gain table */
    0x04280243,  /* 10 us added to the RF SYNTH calibration */
    0x00FF8A43,  // set advLenMask to 0xFF to avoid ROM patch
#ifdef CACHE_AS_RAM
    0x00018063,
#endif //CACHE_AS_RAM
    0xFFFFFFFF };

#elif defined( CC26XX )

  /* CC26xx Normal Package with Flash Settings for 48 MHz device */

  #if defined( CC2650EM_7ID ) || defined(R01)
  regOverride_t rfRegTbl[] = {
  // Recommended overrides for Bluetooth Low Energy, differential mode internal bias
  //uint32_t bleIdOverrides[] = {
    0x00001007,
    0x00354038,
    0x4001402D,
    0x00608402,
    0x4001405D,
    0x1801F800,
    0x000784A3,
    0xA47E0583,
    0xEAE00603,
    0x00010623,
    0x02010403,
    0x40014035,
    0x177F0408,
    0x38000463,
    0x00456088,
    0x013800C3,
    0x036052AC,
    0x01AD02A3,
    0x01680263,
#ifdef CACHE_AS_RAM
    0x00018063,
#endif //CACHE_AS_RAM
    0xFFFFFFFF,
  };

  #elif defined( CC2650EM_5XD ) || defined( CC2650M5A )
  regOverride_t rfRegTbl[] = {
  // Recommended overrides for Bluetooth Low Energy, differential mode external bias
  //uint32_t bleXdOverrides[] = {
    0x00001007,
    0x00354038,
    0x4001402D,
    0x00608402,
    0x4001405D,
    0x1801F800,
    0x000784A3,
    0xA47E0583,
    0xEAE00603,
    0x00010623,
    0x02010403,
    0x40014035,
    0x177F0408,
    0x38000463,
    0x00456088,
    0x013800C3,
    0x036052AC,
    0x01AD02A3,
    0x01680263,
#ifdef CACHE_AS_RAM
    0x00018063,
#endif //CACHE_AS_RAM
    0xFFFFFFFF,
  };

  #elif defined( CC2650EM_4XS )
  regOverride_t rfRegTbl[] = {
  // Recommended overrides for Bluetooth Low Energy, single-ended mode external bias
  //uint32_t bleXsOverrides[] = {
    0x00001007,
    0x00354038,
    0x4001402D,
    0x00608402,
    0x4001405D,
    0x1801F800,
    0x000784A3,
    0xA47E0583,
    0xEAE00603,
    0x00010623,
    0x02010403,
    0x40014035,
    0x177F0408,
    0x38000463,
    0x000288A3,
    0x00456088,
    0x013800C3,
    0x036052AC,
    0x01AD02A3,
    0x01680263,
#ifdef CACHE_AS_RAM
    0x00018063,
#endif //CACHE_AS_RAM
    0xFFFFFFFF,
  };

  #elif defined( CC2650EM_4IS )
  regOverride_t rfRegTbl[] = {
  // Recommended overrides for Bluetooth Low Energy, single-ended mode internal bias
  //uint32_t bleIsOverrides[] = {
    0x00001007,
    0x00354038,
    0x4001402D,
    0x00608402,
    0x4001405D,
    0x1801F800,
    0x000784A3,
    0xA47E0583,
    0xEAE00603,
    0x00010623,
    0x02010403,
    0x40014035,
    0x177F0408,
    0x38000463,
    0x000288A3,
    0x00456088,
    0x013800C3,
    0x036052AC,
    0x01AD02A3,
    0x01680263,
#ifdef CACHE_AS_RAM
    0x00018063,
#endif //CACHE_AS_RAM
    0xFFFFFFFF,
  };

  #else /* unknown device package */

  #error "***BLE USER CONFIG BUILD ERROR*** Unknown package type!"

  #endif /* <board> */

#elif defined( CC13XX )

  #if defined( CC1350LP_7XD ) || defined( CC2650EM_7ID )
  regOverride_t rfRegTbl[] = {
  /* Recommended overrides for Bluetooth Low Energy CC1350 Launchpad */
  /* (differential mode external bias) and for CC1350EM, which uses the */
  /* "CC2650EM_7ID" symbol define. */
    0x00001007,
    0x849f0002,
    0xc7440002,
    0x00344038,
    0x00456088,
    0x05fd6084,
    0x7f004020,
    0x00404064,
    0x4001405d,
    0x18000000,
    0x013800c3,
    0x000784a3,
    0xb1070503,
    0x05330523,
    0xa47e0583,
    0xeae00603,
    0x00010623,
    0x00038883,
    0x00f388a3,
    0x04b00243,
#ifdef CACHE_AS_RAM
    0x00018063,
#endif //CACHE_AS_RAM
    0xffffffff,
  };

  #elif defined( CC1350STK_7XS )

  /**
   * NB! Preliminary values copied from apps RF example,
   * may change after characterization.
   */

  regOverride_t rfRegTbl[] = {
    /* override_use_patch_ble_1mbps.xml */
    /* PHY: Use MCE ROM, RFE RAM patch*/
    MCE_RFE_OVERRIDE(0,0,0,1,0,0),
    /* override_synth_ble_1mbps.xml */
    /* Synth: Set recommended RTRIM to 4 */
    HW_REG_OVERRIDE(0x4038,0x0034),
    /* Synth: Set Fref to 3.43 MHz */
    (uint32_t)0x000784A3,
    /* Synth: Configure fine calibration setting */
    HW_REG_OVERRIDE(0x4020,0x7F00),
    /* Synth: Configure fine calibration setting */
    HW_REG_OVERRIDE(0x4064,0x0040),
    /* Synth: Configure fine calibration setting */
    (uint32_t)0xB1070503,
    /* Synth: Configure fine calibration setting */
    (uint32_t)0x05330523,
    /* Synth: Set loop bandwidth after lock to 80 kHz */
    (uint32_t)0xA47E0583,
    /* Synth: Set loop bandwidth after lock to 80 kHz */
    (uint32_t)0xEAE00603,
    /* Synth: Set loop bandwidth after lock to 80 kHz */
    (uint32_t)0x00010623,
    /* Synth: Configure PLL bias */
    HW32_ARRAY_OVERRIDE(0x405C,1),
    /* Synth: Configure PLL bias */
    (uint32_t)0x18000000,
    /* Synth: Configure VCO LDO (in ADI1, set VCOLDOCFG=0x9F to use voltage input reference) */
    ADI_REG_OVERRIDE(1,4,0x9F),
    /* Synth: Configure synth LDO (in ADI1, set SLDOCTL0.COMP_CAP=1) */
    ADI_HALFREG_OVERRIDE(1,7,0x4,0x4),
    /* override_phy_ble_1mbps.xml */
    /* Tx: Configure symbol shape for BLE frequency deviation requirements */
    (uint32_t)0x013800C3,
    /* Rx: Configure AGC reference level */
    HW_REG_OVERRIDE(0x6088, 0x0045),
    /* Rx: Configure AGC gain level */
    HW_REG_OVERRIDE(0x6084, 0x05FD),
    /* Rx: Configure LNA bias current trim offset */
    (uint32_t)0x00038883,
    /* override_frontend_xd.xml */
    /* Rx: Set RSSI offset to adjust reported RSSI by +13 dB */
    (uint32_t)0x00F388A3,

    ADI_HALFREG_OVERRIDE(0, 16, 0x7, 1),

    (uint32_t)0xFFFFFFFF,  };

  #else /* unknown board package */

  #error "***BLE USER CONFIG BUILD ERROR*** Unknown board type!"

  #endif /* <board> */

#else /* unknown platform */

  #error "ERROR: Unknown platform!"

#endif /* <board> */

/**
 * Tx Power Table Used Depends on Device Package
 */

#if defined( USE_FPGA )

  /* Differential Output (same as CC2650EM_7ID for now) */

  /* Tx Power Values (Pout, IB, GC, TC) */
  const txPwrVal_t TxPowerTable[] =
  { { TX_POWER_MINUS_21_DBM, GEN_TX_POWER_VAL( 0x07, 3, 0x0C ) },
    { TX_POWER_MINUS_18_DBM, GEN_TX_POWER_VAL( 0x09, 3, 0x0C ) },
    { TX_POWER_MINUS_15_DBM, GEN_TX_POWER_VAL( 0x0B, 3, 0x0C ) },
    { TX_POWER_MINUS_12_DBM, GEN_TX_POWER_VAL( 0x0B, 1, 0x14 ) },
    { TX_POWER_MINUS_9_DBM,  GEN_TX_POWER_VAL( 0x0E, 1, 0x19 ) },
    { TX_POWER_MINUS_6_DBM,  GEN_TX_POWER_VAL( 0x12, 1, 0x1D ) },
    { TX_POWER_MINUS_3_DBM,  GEN_TX_POWER_VAL( 0x18, 1, 0x25 ) },
    { TX_POWER_0_DBM,        GEN_TX_POWER_VAL( 0x21, 1, 0x31 ) },
    { TX_POWER_1_DBM,        GEN_TX_POWER_VAL( 0x14, 0, 0x42 ) },
    { TX_POWER_2_DBM,        GEN_TX_POWER_VAL( 0x18, 0, 0x4E ) },
    { TX_POWER_3_DBM,        GEN_TX_POWER_VAL( 0x1C, 0, 0x5A ) },
    { TX_POWER_4_DBM,        GEN_TX_POWER_VAL( 0x24, 0, 0x93 ) },
    { TX_POWER_5_DBM,        GEN_TX_POWER_VAL( 0x30, 0, 0x93 ) } };

#elif defined( CC26XX )

  #if defined( CC2650EM_7ID ) || defined( CC2650EM_5XD ) || defined( CC2650M5A ) || defined(R01)

  /* Differential Output */

  /* Tx Power Values (Pout, IB, GC, TC) */
  const txPwrVal_t TxPowerTable[] =
  { { TX_POWER_MINUS_21_DBM, GEN_TX_POWER_VAL( 0x07, 3, 0x0C ) },
    { TX_POWER_MINUS_18_DBM, GEN_TX_POWER_VAL( 0x09, 3, 0x0C ) },
    { TX_POWER_MINUS_15_DBM, GEN_TX_POWER_VAL( 0x0B, 3, 0x0C ) },
    { TX_POWER_MINUS_12_DBM, GEN_TX_POWER_VAL( 0x0B, 1, 0x14 ) },
    { TX_POWER_MINUS_9_DBM,  GEN_TX_POWER_VAL( 0x0E, 1, 0x19 ) },
    { TX_POWER_MINUS_6_DBM,  GEN_TX_POWER_VAL( 0x12, 1, 0x1D ) },
    { TX_POWER_MINUS_3_DBM,  GEN_TX_POWER_VAL( 0x18, 1, 0x25 ) },
    { TX_POWER_0_DBM,        GEN_TX_POWER_VAL( 0x21, 1, 0x31 ) },
    { TX_POWER_1_DBM,        GEN_TX_POWER_VAL( 0x14, 0, 0x42 ) },
    { TX_POWER_2_DBM,        GEN_TX_POWER_VAL( 0x18, 0, 0x4E ) },
    { TX_POWER_3_DBM,        GEN_TX_POWER_VAL( 0x1C, 0, 0x5A ) },
    { TX_POWER_4_DBM,        GEN_TX_POWER_VAL( 0x24, 0, 0x93 ) },
    { TX_POWER_5_DBM,        GEN_TX_POWER_VAL( 0x30, 0, 0x93 ) } };

  #elif defined( CC2650EM_4XS ) || defined( CC2650EM_4IS )

  /* Single-Ended Output */

  /* Tx Power Values (Pout, IB, GC, TC) */
  const txPwrVal_t TxPowerTable[] =
  { { TX_POWER_MINUS_21_DBM, GEN_TX_POWER_VAL( 0x07, 3, 0x0C ) },
    { TX_POWER_MINUS_18_DBM, GEN_TX_POWER_VAL( 0x09, 3, 0x10 ) },
    { TX_POWER_MINUS_15_DBM, GEN_TX_POWER_VAL( 0x0B, 3, 0x14 ) },
    { TX_POWER_MINUS_12_DBM, GEN_TX_POWER_VAL( 0x0E, 3, 0x14 ) },
    { TX_POWER_MINUS_9_DBM,  GEN_TX_POWER_VAL( 0x0F, 1, 0x21 ) },
    { TX_POWER_MINUS_6_DBM,  GEN_TX_POWER_VAL( 0x14, 1, 0x29 ) },
    { TX_POWER_MINUS_3_DBM,  GEN_TX_POWER_VAL( 0x1C, 1, 0x35 ) },
    { TX_POWER_0_DBM,        GEN_TX_POWER_VAL( 0x2C, 1, 0x56 ) },
    { TX_POWER_1_DBM,        GEN_TX_POWER_VAL( 0x1F, 0, 0x6A ) },
    { TX_POWER_2_DBM,        GEN_TX_POWER_VAL( 0x29, 0, 0x9C ) } };

  #elif defined( CC2640R2EM_CXS )

  /* Single-Ended Output (WCSP) */

  /* Tx Power Values (Pout, IB, GC, TC) */
  const txPwrVal_t TxPowerTable[] =
  { { TX_POWER_MINUS_21_DBM, GEN_TX_POWER_VAL( 0xC3, 3, 0x0C ) },
    { TX_POWER_MINUS_18_DBM, GEN_TX_POWER_VAL( 0xC5, 3, 0x10 ) },
    { TX_POWER_MINUS_15_DBM, GEN_TX_POWER_VAL( 0xC7, 3, 0x10 ) },
    { TX_POWER_MINUS_12_DBM, GEN_TX_POWER_VAL( 0xC9, 3, 0x10 ) },
    { TX_POWER_MINUS_9_DBM,  GEN_TX_POWER_VAL( 0xCC, 3, 0x19 ) },
    { TX_POWER_MINUS_6_DBM,  GEN_TX_POWER_VAL( 0x4E, 1, 0x25 ) },
    { TX_POWER_MINUS_3_DBM,  GEN_TX_POWER_VAL( 0x54, 1, 0x2D ) },
    { TX_POWER_0_DBM,        GEN_TX_POWER_VAL( 0x10, 0, 0x46 ) },
    { TX_POWER_1_DBM,        GEN_TX_POWER_VAL( 0x16, 0, 0x62 ) },
    { TX_POWER_2_DBM,        GEN_TX_POWER_VAL( 0x25, 0, 0xBC ) } };
  #else /* unknown board package */

  #error "***BLE USER CONFIG BUILD ERROR*** Unknown CC2650 board type!"

  #endif /* <board> */

#elif defined( CC13XX )

#if defined( CC1350LP_7XD ) || defined( CC2650EM_7ID )
  /* Tx Power Values (Pout, IB, GC, TC) */
  const txPwrVal_t TxPowerTable[] =
  { { TX_POWER_MINUS_21_DBM, 0x0DC8 },
    { TX_POWER_MINUS_18_DBM, 0x0DCB },
    { TX_POWER_MINUS_15_DBM, 0x15CE },
    { TX_POWER_MINUS_12_DBM, 0x19D4 },
    { TX_POWER_MINUS_9_DBM,  0x1DDA },
    { TX_POWER_MINUS_6_DBM,  0x25E3 },
    { TX_POWER_MINUS_3_DBM,  0x2DEF },
    { TX_POWER_0_DBM,        0x5B29 },
    { TX_POWER_1_DBM,        0x6321 },
    { TX_POWER_2_DBM,        0x6F26 },
    { TX_POWER_3_DBM,        0x7F2C },
    { TX_POWER_4_DBM,        0x7734 },
    { TX_POWER_5_DBM,        0x5F3C } };

#elif defined ( CC1350STK_7XS)

  /* Single-Ended Output. NB! Copied from CC1350LP_7XD, may change */

  /* Tx Power Values (Pout, IB, GC, TC) */
  const txPwrVal_t TxPowerTable[] =
  { { TX_POWER_MINUS_21_DBM, 0x0DC8 },
    { TX_POWER_MINUS_18_DBM, 0x0DCB },
    { TX_POWER_MINUS_15_DBM, 0x15CE },
    { TX_POWER_MINUS_12_DBM, 0x19D4 },
    { TX_POWER_MINUS_9_DBM,  0x1DDA },
    { TX_POWER_MINUS_6_DBM,  0x25E3 },
    { TX_POWER_MINUS_3_DBM,  0x2DEF },
    { TX_POWER_0_DBM,        0x5B29 },
    { TX_POWER_1_DBM,        0x6321 },
    { TX_POWER_2_DBM,        0x6F26 },
    { TX_POWER_3_DBM,        0x7F2C },
    { TX_POWER_4_DBM,        0x7734 },
    { TX_POWER_5_DBM,        0x5F3C } };
  #else /* unknown board package */

  #error "***BLE USER CONFIG BUILD ERROR*** Unknown CC1350 board type!"

  #endif /* <board> */

#else /* unknown platform */

  #error "ERROR: Unknown platform!"

#endif /* <board> */

/* Tx Power Table */
txPwrTbl_t appTxPwrTbl = { TxPowerTable,
                        NUM_TX_POWER_VALUES,  /* max */
                        DEFAULT_TX_POWER };   /* default */


#ifdef ICALL_JT
#include "icall.h"

#ifndef DISABLE_RF_DRIVER
/* RF Driver API Table */
rfDrvTblPtr_t rfDriverTableBLE[] =
  { (uint32)RF_open,
    (uint32)driverTable_fnSpinlock, /* RF_close */
    (uint32)RF_postCmd,
    (uint32)driverTable_fnSpinlock, /* RF_pendCmd */
    (uint32)RF_runCmd,
    (uint32)RF_cancelCmd,
    (uint32)RF_flushCmd,
    (uint32)driverTable_fnSpinlock, /* RF_yield */
    (uint32)RF_Params_init,
    (uint32)RF_runImmediateCmd,
    (uint32)RF_runDirectCmd,
    (uint32)driverTable_fnSpinlock, /* RF_ratCompare */
    (uint32)driverTable_fnSpinlock, /* RF_ratCapture */
    (uint32)driverTable_fnSpinlock, /* RF_ratHwOutput */
    (uint32)driverTable_fnSpinlock, /* RF_ratDisableChannel */
    (uint32)RF_getCurrentTime,
    (uint32)RF_getRssi,
    (uint32)RF_getInfo,
    (uint32)RF_getCmdOp };
#endif /* !DISABLE_RF_DRIVER */

cryptoDrvTblPtr_t cryptoDriverTableBLE[] =
  { (uint32)driverTable_fnSpinlock,          // CryptoCC26XX_close
    (uint32)CryptoCC26XX_init,
    (uint32)CryptoCC26XX_open,
    (uint32)CryptoCC26XX_Params_init,
    (uint32)driverTable_fnSpinlock,          /* CryptoCC26XX_Transac_init */
    (uint32)CryptoCC26XX_allocateKey,
    (uint32)driverTable_fnSpinlock,          /* CryptoCC26XX_releaseKey */
    (uint32)CryptoCC26XX_transact,
    (uint32)CryptoCC26XX_transactPolling,
    (uint32)driverTable_fnSpinlock,          /* CryptoCC26XX_transactCallback */
    (uint32)CryptoCC26XX_loadKey };

/* BLE Stack Configuration Structure */
const stackSpecific_t bleStackConfig =
{
  .maxNumConns                          = MAX_NUM_BLE_CONNS,
  .maxNumPDUs                           = MAX_NUM_PDU,
  .maxPduSize                           = MAX_PDU_SIZE,
  .maxNumPSM                            = L2CAP_NUM_PSM,
  .maxNumCoChannels                     = L2CAP_NUM_CO_CHANNELS,
  .pfnBMAlloc                           = &pfnBMAlloc,
  .pfnBMFree                            = &pfnBMFree,
  .rfDriverParams.powerUpDurationMargin = RF_POWER_UP_DURATION_MARGIN,
  .rfDriverParams.inactivityTimeout     = RF_INACTIVITY_TIMEOUT,
  .rfDriverParams.powerUpDuration       = RF_POWER_UP_DURATION,
  .rfDriverParams.powerUpXOSC           = RF_POWER_XOSC,
  .rfDriverParams.pErrCb                = &(RF_ERR_CB)
};

#ifdef OSAL_SNV_EXTFLASH
const extflashDrvTblPtr_t extflashDriverTable[] =
{
  (uint32) ExtFlash_open,
  (uint32) ExtFlash_close,
  (uint32) ExtFlash_read,
  (uint32) ExtFlash_write,
  (uint32) ExtFlash_erase
};
#endif // OSAL_SNV_EXTFLASH

/*
* Table for Driver can be found in icall_user_config.c
* if a driver is not to be used, then the pointer shoul dbe set to NULL,
* for this example, this is done in BleUserConfig.h
*/
const drvTblPtr_t driverTable =
{
#ifndef DISABLE_RF_DRIVER
  .rfDrvTbl       = rfDriverTableBLE,
#else
  .rfDrvTbl       = NULL,
#endif
  .eccDrvTbl      = eccDriverTable,
  .cryptoDrvTbl   = cryptoDriverTableBLE,
  .trngDrvTbl     = trngDriverTable,
#ifdef OSAL_SNV_EXTFLASH
  .extflashDrvTbl = extflashDriverTable,
#endif // OSAL_SNV_EXTFLASH
};

const boardConfig_t boardConfig =
{
  .rfFeModeBias = RF_FE_MODE_AND_BIAS,
  .rfRegTbl     = rfRegTbl,
  .txPwrTbl     = &appTxPwrTbl
};

#else /* !(ICALL_JT) */

#ifndef DISABLE_RF_DRIVER
/* RF Driver API Table */
rfDrvTblPtr_t rfDriverTable[] =
{
  (uint32) RF_open,
  (uint32) driverTable_fnSpinlock, /* RF_close */
  (uint32) RF_postCmd,
  (uint32) driverTable_fnSpinlock, /* RF_pendCmd */
  (uint32) RF_runCmd,
  (uint32) RF_cancelCmd,
  (uint32) RF_flushCmd,
  (uint32) driverTable_fnSpinlock, /* RF_yield */
  (uint32) RF_Params_init,
  (uint32) RF_runImmediateCmd,
  (uint32) RF_runDirectCmd,
  (uint32) driverTable_fnSpinlock, /* RF_ratCompare */
  (uint32) driverTable_fnSpinlock, /* RF_ratCapture */
  (uint32) driverTable_fnSpinlock, /* RF_ratHwOutput */
  (uint32) driverTable_fnSpinlock, /* RF_ratDisableChannel */
  (uint32) RF_getCurrentTime,
  (uint32) RF_getRssi,
  (uint32) RF_getInfo,
  (uint32)RF_getCmdOp
};
#endif // !DISABLE_RF_DRIVER


#if defined(BLE_V42_FEATURES) && (BLE_V42_FEATURES & SECURE_CONNS_CFG)
/* ECC Driver API Table */
eccDrvTblPtr_t eccDriverTable[] =
{
  (uint32) ECCROMCC26XX_init,
  (uint32) ECCROMCC26XX_Params_init,
  (uint32) ECCROMCC26XX_genKeys,
  (uint32) ECCROMCC26XX_genDHKey
};
#endif //BLE_V42_FEATURES & SECURE_CONNS_CFG

/* Crypto Driver API Table */
cryptoDrvTblPtr_t cryptoDriverTable[] =
{
  (uint32) driverTable_fnSpinlock,          /* CryptoCC26XX_close */
  (uint32) CryptoCC26XX_init,
  (uint32) CryptoCC26XX_open,
  (uint32) CryptoCC26XX_Params_init,
  (uint32) driverTable_fnSpinlock,          /* CryptoCC26XX_Transac_init */
  (uint32) CryptoCC26XX_allocateKey,
  (uint32) driverTable_fnSpinlock,          /* CryptoCC26XX_releaseKey */
  (uint32) CryptoCC26XX_transact,
  (uint32) CryptoCC26XX_transactPolling,
  (uint32) driverTable_fnSpinlock,          /* CryptoCC26XX_transactCallback */
  (uint32) CryptoCC26XX_loadKey
};

trngDrvTblPtr_t trngDriverTable[] =
{
  (uint32) TRNGCC26XX_init,
  (uint32) TRNGCC26XX_open,
  (uint32) TRNGCC26XX_getNumber
};


#endif /* ICALL_JT */
/**
 * @brief           This routine registers the Application's assert handler.
 *
 * @param[in]       appAssertHandler - Application's assert handler.
 *
 */
void RegisterAssertCback( assertCback_t appAssertHandler )
{
  appAssertCback = appAssertHandler;

#ifdef EXT_HAL_ASSERT
  /* also set the Assert callback pointer used by halAssertHandlerExt */
  /**
   * Note: Normally, this pointer will be intialized by the stack, but in the
   * event HAL_ASSERT is used by the Application, we initialize it
   * directly here.
   * */
  halAssertCback = appAssertHandler;
#endif /* EXT_HAL_ASSERT */

  return;
}

/**
 * @brief       This routine is used to trap calls to unpopulated indexes of
 *              driver function pointer tables.
 *
 */
void driverTable_fnSpinlock( void )
{
  volatile uint8 i = 1;

  while(i);
}

/**
 * @brief    	    This is the Application default assert callback, in the event
 *                  none is registered.
 *
 * @param[in]       assertCause    - Assert cause as defined in hal_assert.h.
 * @param[in]       assertSubcause - Optional assert subcause (see hal_assert.h).
 *
 */
void DefaultAssertCback( uint8 assertCause, uint8 assertSubcause )
{
#ifdef HAL_ASSERT_SPIN
  driverTable_fnSpinlock;
#endif /* HAL_ASSERT_SPIN */

  return;
}

/* Application Assert Callback Function Pointer */
assertCback_t appAssertCback = DefaultAssertCback;
