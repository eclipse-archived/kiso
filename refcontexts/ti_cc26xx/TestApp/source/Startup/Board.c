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

#ifdef BCDS_MODULE_ID
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_IROLL_APP_MODULE_ID_BOARD
#endif

/* system header files */
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/ioc.h>
#include <driverlib/udma.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/family/arm/m3/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/PIN.h>
#include "Board.h"
/* additional interface header files */

/* own header files */

/* functions */

/*
 *  ========================= IO driver initialization =========================
 *  From main, PIN_init(BoardGpioInitTable) should be called to setup safe
 *  settings for this board.
 *  When a pin is allocated and then de-allocated, it will revert to the state
 *  configured in this table.
 */

/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(PINCC26XX_hwAttrs, ".const:PINCC26XX_hwAttrs")
#endif


PIN_Config BoardGpioInitTableTest[] = {
#if defined(R01)
        /* making UART pins as tri-state as Iroll is not using LED */
		BOARD_UART_TX | PIN_BM_INPUT_EN | PIN_BM_GPIO_OUTPUT_EN | PIN_GPIO_OUTPUT_DIS | PIN_INPUT_DIS | PIN_NOPULL,
		BOARD_UART_RX | PIN_BM_INPUT_EN | PIN_BM_GPIO_OUTPUT_EN | PIN_GPIO_OUTPUT_DIS | PIN_INPUT_DIS | PIN_NOPULL,
#endif
        PIN_TERMINATE,                                                                   /**< Terminate list */

};

#ifndef BSP_SUPPORT

const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {
    .intPriority = ~0,
    .swiPriority = 0
};
#endif /*BSP_SUPPORT*/


/*============================================================================*/
/*
 *  ============================= UART begin ===================================
 */
#ifndef BSP_SUPPORT
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartCC26XXHWAttrs, ".const:uartCC26XXHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

/* UART objects */
UARTCC26XX_Object uartCC26XXObjects[CC2650_UARTCOUNT];

/* UART hardware parameter structure, also used to assign UART pins */
const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[CC2650_UARTCOUNT] = {
        { /* CC2650_UART0 */
        .baseAddr = UART0_BASE,
                .intNum = INT_UART0_COMB,
                .powerMngrId = PowerCC26XX_PERIPH_UART0,
                .intPriority    = ~0,
                .swiPriority    = 0,
                .txPin = BOARD_UART_TX,
                .rxPin = BOARD_UART_RX,
                .ctsPin = PIN_UNASSIGNED,
                .rtsPin = PIN_UNASSIGNED
        },
};

/* UART configuration structure */
#ifdef UART_ENABLED
__root const UART_Config UART_config[] = {
        { &UARTCC26XX_fxnTable, &uartCC26XXObjects[0], &uartCC26XXHWAttrs[0] },
        { NULL, NULL, NULL }
};
#else
const UART_Config UART_config[] = {
        { &UARTCC26XX_fxnTable, &uartCC26XXObjects[0], &uartCC26XXHWAttrs[0] },
        { NULL, NULL, NULL }
};
#endif

const uint_least8_t UART_count = CC2650_UARTCOUNT;
#endif /*BSP_SUPPORT*/

/*
 *  ============================= UART end =====================================
 */
#ifndef BSP_SUPPORT
#ifdef I2C_config
#undef I2C_config
#endif

#ifdef I2C_count
#undef I2C_count
#endif
/*
 *  ============================= I2C Begin=====================================
*/
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cCC26xxHWAttrs, ".const:i2cCC26xxHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/i2c/I2CCC26XX.h>

/* I2C objects */
I2CCC26XX_Object i2cCC26xxObjects[CC2650_I2CCOUNT];

/* I2C configuration structure, describing which pins are to be used */
const I2CCC26XX_HWAttrsV1 i2cCC26xxHWAttrs[CC2650_I2CCOUNT] = {
    {
        .baseAddr = I2C0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_I2C0,
        .intNum = INT_I2C_IRQ,
        .intPriority = ~0,
        .swiPriority = 0,
        .sdaPin = BOARD_I2C0_SDA0,
        .sclPin = BOARD_I2C0_SCL0,
    }
};

const I2C_Config I2C_config[] = {
    {
        .fxnTablePtr = &I2CCC26XX_fxnTable,
        .object = &i2cCC26xxObjects[0],
        .hwAttrs = &i2cCC26xxHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

const uint_least8_t I2C_count = CC2650_I2CCOUNT;
#endif /*BSP_SUPPORT*/
/*
 *  ========================== I2C end =========================================
 */
#ifndef BSP_SUPPORT
/*
 *  ============================= UDMA begin ===================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UDMACC26XX_config, ".const:UDMACC26XX_config")
#pragma DATA_SECTION(udmaHWAttrs, ".const:udmaHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/dma/UDMACC26XX.h>

/* UDMA objects */
UDMACC26XX_Object UdmaObjects[CC2650_UDMACOUNT];

/* UDMA configuration structure */
const UDMACC26XX_HWAttrs udmaHWAttrs[CC2650_UDMACOUNT] = {
	    {
	        .baseAddr    = UDMA0_BASE,
	        .powerMngrId = PowerCC26XX_PERIPH_UDMA,
	        .intNum      = INT_DMA_ERR
	    }
};

/* UDMA configuration structure */
__root const UDMACC26XX_Config UDMACC26XX_config[] = {
        { &UdmaObjects[0], &udmaHWAttrs[0] },
        { NULL, NULL },
};
/*
 *  ============================= UDMA end =====================================
 */
#endif /*BSP_SUPPORT*/

/*  ============================= WATCHDOG begin ===================================
*/
#ifndef BSP_SUPPORT
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(Watchdog_config, ".const:Watchdog_config")
#pragma DATA_SECTION(watchdogCC26XXHwAttrs, ".const:watchdogCC26XXHwAttrs")
#endif

/* Include drivers */
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogCC26XX.h>

/* WATCHDOG objects */
WatchdogCC26XX_Object watchdogCC26XXObjects[CC2650_WATCHDOGCOUNT];

/* Watchdog hardware parameter structure */
const WatchdogCC26XX_HWAttrs watchdogCC26XXHwAttrs[CC2650_WATCHDOGCOUNT] =  {
  {
    .baseAddr = WDT_BASE,
	.reloadValue = 1000 /* Reload value in milliseconds */
  }
};

/* WATCHDOG configuration structure */
const Watchdog_Config Watchdog_config[CC2650_WATCHDOGCOUNT] = {
  { &WatchdogCC26XX_fxnTable, &watchdogCC26XXObjects[0], &watchdogCC26XXHwAttrs[0] },
};

/*variable used to configure the watchdog modules */
const uint_least8_t Watchdog_count= CC2650_WATCHDOGCOUNT ;
#endif /*BSP_SUPPORT*/
/*
 *  ============================= WATCHDOG end =====================================
*/

#ifndef BSP_SUPPORT
/*
 *  ============================= Power begin ===================================
*/
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(PowerCC26XX_config, ".const:PowerCC26XX_config")
#endif

const PowerCC26XX_Config PowerCC26XX_config = {
    .policyInitFxn = NULL,
    .policyFxn = &PowerCC26XX_standbyPolicy,
    .calibrateFxn = &PowerCC26XX_calibrate,
    .enablePolicy = TRUE,
    .calibrateRCOSC_LF = TRUE,
    .calibrateRCOSC_HF = TRUE,
};
/*
 *  ============================= Power end ===================================
*/
#endif /*BSP_SUPPORT*/

#ifndef BSP_SUPPORT
/*
 *  ========================= TRNG begin ====================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(TRNGCC26XX_config, ".const:TRNGCC26XX_config")
#pragma DATA_SECTION(TRNGCC26XXHWAttrs, ".const:TRNGCC26XXHWAttrs")
#endif

/* Include drivers */
#include <TRNGCC26XX.h>

/* TRNG objects */
TRNGCC26XX_Object trngCC26XXObjects[CC2650_TRNGCOUNT];

/* TRNG configuration structure, describing which pins are to be used */
const TRNGCC26XX_HWAttrs TRNGCC26XXHWAttrs[CC2650_TRNGCOUNT] = {
    {
        .powerMngrId    = PowerCC26XX_PERIPH_TRNG,
    }
};

/* TRNG configuration structure */
const TRNGCC26XX_Config TRNGCC26XX_config[] = {
    {
         .object  = &trngCC26XXObjects[0],
         .hwAttrs = &TRNGCC26XXHWAttrs[0]
    },
    {NULL, NULL}
};

/*
 *  ========================= TRNG end ====================================
 */
#endif /*BSP_SUPPORT*/

#ifndef BSP_SUPPORT
/*
 *  ========================= RF driver begin ==================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(RFCC26XX_hwAttrs, ".const:RFCC26XX_hwAttrs")
#endif

/* Include drivers */
#include <ti/drivers/rf/RF.h>

/* RF hwi and swi priority */
const RFCC26XX_HWAttrs RFCC26XX_hwAttrs = {
    .hwiCpe0Priority = ~0,
    .hwiHwPriority   = ~0,
    .swiCpe0Priority =  5,
    .swiHwPriority   =  5,
};

/*
 *  ========================== RF driver end ===================================
 */
#endif /*BSP_SUPPORT*/
#ifndef BSP_SUPPORT
/*
 *  =============================== Crypto begin ===============================
 */
#include <ti/drivers/crypto/CryptoCC26XX.h>

CryptoCC26XX_Object cryptoCC26XXObjects[CC2640R2_CRYPTOCOUNT];

const CryptoCC26XX_HWAttrs cryptoCC26XXHWAttrs[CC2640R2_CRYPTOCOUNT] = {
    {
        .baseAddr       = CRYPTO_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_CRYPTO,
        .intNum         = INT_CRYPTO_RESULT_AVAIL_IRQ,
        .intPriority    = ~0,
    }
};

const CryptoCC26XX_Config CryptoCC26XX_config[CC2640R2_CRYPTOCOUNT] = {
    {
         .object  = &cryptoCC26XXObjects[CC2640R2_CRYPTO0],
         .hwAttrs = &cryptoCC26XXHWAttrs[CC2640R2_CRYPTO0]
    },
};
#endif /*BSP_SUPPORT*/
/*
 *  ============================ Crypto end ===================================
 */
