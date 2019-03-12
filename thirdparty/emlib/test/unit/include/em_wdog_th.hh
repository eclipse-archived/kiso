/****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *
 *  Mockup implementation for the em_wdog module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
#ifndef BOOTLOADERAPP_EMWDOG_TH_HH_
#define BOOTLOADERAPP_EMWDOG_TH_HH_

/* include system header files */
#define EM_WDOG_H

/** Watchdog clock selection. */
typedef enum
{
  wdogClkSelULFRCO = _WDOG_CTRL_CLKSEL_ULFRCO,   /**< Ultra low frequency (1 kHz) clock */
  wdogClkSelLFRCO  = _WDOG_CTRL_CLKSEL_LFRCO,    /**< Low frequency RC oscillator */
  wdogClkSelLFXO   = _WDOG_CTRL_CLKSEL_LFXO      /**< Low frequency crystal oscillator */
} WDOG_ClkSel_TypeDef;
/* wrap the real App.h interface header */

/** Watchdog period selection. */
typedef enum
{
  wdogPeriod_9    = 0x0, /**< 9 clock periods */
  wdogPeriod_17   = 0x1, /**< 17 clock periods */
  wdogPeriod_33   = 0x2, /**< 33 clock periods */
  wdogPeriod_65   = 0x3, /**< 65 clock periods */
  wdogPeriod_129  = 0x4, /**< 129 clock periods */
  wdogPeriod_257  = 0x5, /**< 257 clock periods */
  wdogPeriod_513  = 0x6, /**< 513 clock periods */
  wdogPeriod_1k   = 0x7, /**< 1025 clock periods */
  wdogPeriod_2k   = 0x8, /**< 2049 clock periods */
  wdogPeriod_4k   = 0x9, /**< 4097 clock periods */
  wdogPeriod_8k   = 0xA, /**< 8193 clock periods */
  wdogPeriod_16k  = 0xB, /**< 16385 clock periods */
  wdogPeriod_32k  = 0xC, /**< 32769 clock periods */
  wdogPeriod_64k  = 0xD, /**< 65537 clock periods */
  wdogPeriod_128k = 0xE, /**< 131073 clock periods */
  wdogPeriod_256k = 0xF  /**< 262145 clock periods */
} WDOG_PeriodSel_TypeDef;



/** Watchdog initialization structure. */

typedef struct
{
  /** Enable watchdog when init completed. */
  bool                   enable;

  /** Counter shall keep running during debug halt. */
  bool                   debugRun;

  /** Counter shall keep running when in EM2. */
  bool                   em2Run;

  /** Counter shall keep running when in EM3. */
  bool                   em3Run;

  /** Block EMU from entering EM4. */
  bool                   em4Block;

  /** Block SW from disabling LFRCO/LFXO oscillators. */
  bool                   swoscBlock;

  /** Block SW from modifying the configuration (a reset is needed to reconfigure). */
  bool                   lock;

  /** Clock source to use for watchdog. */
  WDOG_ClkSel_TypeDef    clkSel;

  /** Watchdog timeout period. */
  WDOG_PeriodSel_TypeDef perSel;

#if defined( _WDOG_CTRL_WARNSEL_MASK )
  /** Select warning time as % of the watchdog timeout */
  WDOG_WarnSel_TypeDef   warnSel;
#endif

#if defined( _WDOG_CTRL_WINSEL_MASK )
  /** Select illegal window time as % of the watchdog timeout */
  WDOG_WinSel_TypeDef    winSel;
#endif

#if defined( _WDOG_CTRL_WDOGRSTDIS_MASK )
  /** Disable watchdog reset output if true */
  bool                   resetDisable;
#endif

} WDOG_Init_TypeDef;


/* faked variables needs to be initialized by the test fixture */
/* mock-ups for the provided interfaces */
#define __STATIC_INLINE static inline
/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC (WDOG_Feed);
FAKE_VOID_FUNC (WDOG_Init,const WDOG_Init_TypeDef *);

#endif
