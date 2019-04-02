/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @file
 *
 * @brief Unit Test Mock-up Header
 *
 * This unit test header substitutes "em_letimer.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef EM_LETIMER_TH_HH_
#define EM_LETIMER_TH_HH_

/* include system header files */
#include "em_device_th.hh"

/* ban the real em_letimer_ih.h interface header */
#define __EM_LETIMER_H

#define LETIMER_IF_UF                        (0x1UL << 2)
#define LETIMER_IFC_UF                       (0x1UL << 2)
#define LETIMER_IFC_COMP0                    (0x1UL << 0)
#define LETIMER_IEN_COMP0                    (0x1UL << 0)

/* Bit fields for LETIMER CTRL */
#define _LETIMER_CTRL_RESETVALUE             0x00000000UL                           /**< Default value for LETIMER_CTRL */
#define _LETIMER_CTRL_MASK                   0x00001FFFUL                           /**< Mask for LETIMER_CTRL */
#define _LETIMER_CTRL_REPMODE_SHIFT          0                                      /**< Shift value for LETIMER_REPMODE */
#define _LETIMER_CTRL_REPMODE_MASK           0x3UL                                  /**< Bit mask for LETIMER_REPMODE */
#define _LETIMER_CTRL_REPMODE_DEFAULT        0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define _LETIMER_CTRL_REPMODE_FREE           0x00000000UL                           /**< Mode FREE for LETIMER_CTRL */
#define _LETIMER_CTRL_REPMODE_ONESHOT        0x00000001UL                           /**< Mode ONESHOT for LETIMER_CTRL */
#define _LETIMER_CTRL_REPMODE_BUFFERED       0x00000002UL                           /**< Mode BUFFERED for LETIMER_CTRL */
#define _LETIMER_CTRL_REPMODE_DOUBLE         0x00000003UL                           /**< Mode DOUBLE for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA0_SHIFT            2                                      /**< Shift value for LETIMER_UFOA0 */
#define _LETIMER_CTRL_UFOA0_MASK             0xCUL                                  /**< Bit mask for LETIMER_UFOA0 */
#define _LETIMER_CTRL_UFOA0_DEFAULT          0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA0_NONE             0x00000000UL                           /**< Mode NONE for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA0_TOGGLE           0x00000001UL                           /**< Mode TOGGLE for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA0_PULSE            0x00000002UL                           /**< Mode PULSE for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA0_PWM              0x00000003UL                           /**< Mode PWM for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA0_DEFAULT           (_LETIMER_CTRL_UFOA0_DEFAULT << 2)     /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA0_NONE              (_LETIMER_CTRL_UFOA0_NONE << 2)        /**< Shifted mode NONE for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA0_TOGGLE            (_LETIMER_CTRL_UFOA0_TOGGLE << 2)      /**< Shifted mode TOGGLE for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA0_PULSE             (_LETIMER_CTRL_UFOA0_PULSE << 2)       /**< Shifted mode PULSE for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA0_PWM               (_LETIMER_CTRL_UFOA0_PWM << 2)         /**< Shifted mode PWM for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA1_SHIFT            4                                      /**< Shift value for LETIMER_UFOA1 */
#define _LETIMER_CTRL_UFOA1_MASK             0x30UL                                 /**< Bit mask for LETIMER_UFOA1 */
#define _LETIMER_CTRL_UFOA1_DEFAULT          0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA1_NONE             0x00000000UL                           /**< Mode NONE for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA1_TOGGLE           0x00000001UL                           /**< Mode TOGGLE for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA1_PULSE            0x00000002UL                           /**< Mode PULSE for LETIMER_CTRL */
#define _LETIMER_CTRL_UFOA1_PWM              0x00000003UL                           /**< Mode PWM for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA1_DEFAULT           (_LETIMER_CTRL_UFOA1_DEFAULT << 4)     /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA1_NONE              (_LETIMER_CTRL_UFOA1_NONE << 4)        /**< Shifted mode NONE for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA1_TOGGLE            (_LETIMER_CTRL_UFOA1_TOGGLE << 4)      /**< Shifted mode TOGGLE for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA1_PULSE             (_LETIMER_CTRL_UFOA1_PULSE << 4)       /**< Shifted mode PULSE for LETIMER_CTRL */
#define LETIMER_CTRL_UFOA1_PWM               (_LETIMER_CTRL_UFOA1_PWM << 4)         /**< Shifted mode PWM for LETIMER_CTRL */
#define LETIMER_CTRL_OPOL0                   (0x1UL << 6)                           /**< Output 0 Polarity */
#define _LETIMER_CTRL_OPOL0_SHIFT            6                                      /**< Shift value for LETIMER_OPOL0 */
#define _LETIMER_CTRL_OPOL0_MASK             0x40UL                                 /**< Bit mask for LETIMER_OPOL0 */
#define _LETIMER_CTRL_OPOL0_DEFAULT          0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_OPOL0_DEFAULT           (_LETIMER_CTRL_OPOL0_DEFAULT << 6)     /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_OPOL1                   (0x1UL << 7)                           /**< Output 1 Polarity */
#define _LETIMER_CTRL_OPOL1_SHIFT            7                                      /**< Shift value for LETIMER_OPOL1 */
#define _LETIMER_CTRL_OPOL1_MASK             0x80UL                                 /**< Bit mask for LETIMER_OPOL1 */
#define _LETIMER_CTRL_OPOL1_DEFAULT          0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_OPOL1_DEFAULT           (_LETIMER_CTRL_OPOL1_DEFAULT << 7)     /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_BUFTOP                  (0x1UL << 8)                           /**< Buffered Top */
#define _LETIMER_CTRL_BUFTOP_SHIFT           8                                      /**< Shift value for LETIMER_BUFTOP */
#define _LETIMER_CTRL_BUFTOP_MASK            0x100UL                                /**< Bit mask for LETIMER_BUFTOP */
#define _LETIMER_CTRL_BUFTOP_DEFAULT         0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_BUFTOP_DEFAULT          (_LETIMER_CTRL_BUFTOP_DEFAULT << 8)    /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_COMP0TOP                (0x1UL << 9)                           /**< Compare Value 0 Is Top Value */
#define _LETIMER_CTRL_COMP0TOP_SHIFT         9                                      /**< Shift value for LETIMER_COMP0TOP */
#define _LETIMER_CTRL_COMP0TOP_MASK          0x200UL                                /**< Bit mask for LETIMER_COMP0TOP */
#define _LETIMER_CTRL_COMP0TOP_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_COMP0TOP_DEFAULT        (_LETIMER_CTRL_COMP0TOP_DEFAULT << 9)  /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_RTCC0TEN                (0x1UL << 10)                          /**< RTC Compare 0 Trigger Enable */
#define _LETIMER_CTRL_RTCC0TEN_SHIFT         10                                     /**< Shift value for LETIMER_RTCC0TEN */
#define _LETIMER_CTRL_RTCC0TEN_MASK          0x400UL                                /**< Bit mask for LETIMER_RTCC0TEN */
#define _LETIMER_CTRL_RTCC0TEN_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_RTCC0TEN_DEFAULT        (_LETIMER_CTRL_RTCC0TEN_DEFAULT << 10) /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_RTCC1TEN                (0x1UL << 11)                          /**< RTC Compare 1 Trigger Enable */
#define _LETIMER_CTRL_RTCC1TEN_SHIFT         11                                     /**< Shift value for LETIMER_RTCC1TEN */
#define _LETIMER_CTRL_RTCC1TEN_MASK          0x800UL                                /**< Bit mask for LETIMER_RTCC1TEN */
#define _LETIMER_CTRL_RTCC1TEN_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_RTCC1TEN_DEFAULT        (_LETIMER_CTRL_RTCC1TEN_DEFAULT << 11) /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_DEBUGRUN                (0x1UL << 12)                          /**< Debug Mode Run Enable */
#define _LETIMER_CTRL_DEBUGRUN_SHIFT         12                                     /**< Shift value for LETIMER_DEBUGRUN */
#define _LETIMER_CTRL_DEBUGRUN_MASK          0x1000UL                               /**< Bit mask for LETIMER_DEBUGRUN */
#define _LETIMER_CTRL_DEBUGRUN_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_DEBUGRUN_DEFAULT        (_LETIMER_CTRL_DEBUGRUN_DEFAULT << 12) /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_REPMODE_DEFAULT         (_LETIMER_CTRL_REPMODE_DEFAULT << 0)   /**< Shifted mode DEFAULT for LETIMER_CTRL */
#define LETIMER_CTRL_REPMODE_FREE            (_LETIMER_CTRL_REPMODE_FREE << 0)      /**< Shifted mode FREE for LETIMER_CTRL */
#define LETIMER_CTRL_REPMODE_ONESHOT         (_LETIMER_CTRL_REPMODE_ONESHOT << 0)   /**< Shifted mode ONESHOT for LETIMER_CTRL */
#define LETIMER_CTRL_REPMODE_BUFFERED        (_LETIMER_CTRL_REPMODE_BUFFERED << 0)  /**< Shifted mode BUFFERED for LETIMER_CTRL */
#define LETIMER_CTRL_REPMODE_DOUBLE          (_LETIMER_CTRL_REPMODE_DOUBLE << 0)    /**< Shifted mode DOUBLE for LETIMER_CTRL */

#define LETIMER_CMD_CLEAR                    (0x1UL << 2)

/** Repeat mode. */
typedef enum
{
  /** Count until stopped by SW. */
  letimerRepeatFree     = _LETIMER_CTRL_REPMODE_FREE,
  /** Count REP0 times. */
  letimerRepeatOneshot  = _LETIMER_CTRL_REPMODE_ONESHOT,
  /**
   * Count REP0 times, if REP1 has been written to, it is loaded into
   * REP0 when REP0 is about to be decremented to 0.
   */
  letimerRepeatBuffered = _LETIMER_CTRL_REPMODE_BUFFERED,
  /**
   * Run as long as both REP0 and REP1 are not 0. Both REP0 and REP1
   * are decremented when counter underflows.
   */
  letimerRepeatDouble   = _LETIMER_CTRL_REPMODE_DOUBLE
} LETIMER_RepeatMode_TypeDef;

/** Underflow action on output. */
typedef enum
{
  /** No output action. */
  letimerUFOANone   = _LETIMER_CTRL_UFOA0_NONE,
  /** Toggle output when counter underflows. */
  letimerUFOAToggle = _LETIMER_CTRL_UFOA0_TOGGLE,
  /** Hold output one LETIMER clock cycle when counter underflows. */
  letimerUFOAPulse  = _LETIMER_CTRL_UFOA0_PULSE,
  /** Set output idle when counter underflows, and active when matching COMP1. */
  letimerUFOAPwm    = _LETIMER_CTRL_UFOA0_PWM
} LETIMER_UFOA_TypeDef;

typedef struct
{
  bool                       enable;         /**< Start counting when init completed. */
  bool                       debugRun;       /**< Counter shall keep running during debug halt. */
  bool                       rtcComp0Enable; /**< Start counting on RTC COMP0 match. */
  bool                       rtcComp1Enable; /**< Start counting on RTC COMP1 match. */
  bool                       comp0Top;       /**< Load COMP0 register into CNT when counter underflows. */
  bool                       bufTop;         /**< Load COMP1 into COMP0 when REP0 reaches 0. */
  uint8_t                    out0Pol;        /**< Idle value for output 0. */
  uint8_t                    out1Pol;        /**< Idle value for output 1. */
  LETIMER_UFOA_TypeDef       ufoa0;          /**< Underflow output 0 action. */
  LETIMER_UFOA_TypeDef       ufoa1;          /**< Underflow output 1 action. */
  LETIMER_RepeatMode_TypeDef repMode;        /**< Repeat mode. */
} LETIMER_Init_TypeDef;

/* Hardware Dependent Usage Register Maps Faked with Fixed Memory */
LETIMER_TypeDef *LETIMER0 =(LETIMER_TypeDef*) malloc(sizeof(LETIMER_TypeDef));

#define _CMU_LFACLKEN0_LETIMER0_MASK

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(uint32_t,LETIMER_CounterGet,LETIMER_TypeDef *);
FAKE_VOID_FUNC(LETIMER_IntClear,LETIMER_TypeDef *, uint32_t);
FAKE_VOID_FUNC(LETIMER_IntDisable,LETIMER_TypeDef*, uint32_t);
FAKE_VOID_FUNC(LETIMER_IntEnable,LETIMER_TypeDef *, uint32_t);
FAKE_VALUE_FUNC(uint32_t,LETIMER_IntGet,LETIMER_TypeDef *);
FAKE_VOID_FUNC(LETIMER_IntSet,LETIMER_TypeDef *, uint32_t);
FAKE_VALUE_FUNC(uint32_t,LETIMER_CompareGet,LETIMER_TypeDef *, unsigned int);
FAKE_VOID_FUNC(LETIMER_CompareSet,LETIMER_TypeDef *,unsigned int,uint32_t);
FAKE_VOID_FUNC(LETIMER_Enable,LETIMER_TypeDef *, bool);
FAKE_VOID_FUNC(LETIMER_FreezeEnable,LETIMER_TypeDef *,bool);
FAKE_VOID_FUNC(LETIMER_Init,LETIMER_TypeDef *, const LETIMER_Init_TypeDef *);
FAKE_VALUE_FUNC(uint32_t,LETIMER_RepeatGet,LETIMER_TypeDef *, unsigned int );
FAKE_VOID_FUNC(LETIMER_RepeatSet,LETIMER_TypeDef *,unsigned int,uint32_t);
FAKE_VOID_FUNC(LETIMER_Reset,LETIMER_TypeDef *);


#endif /* EM_LETIMER_TH_HH_ */

/** ************************************************************************* */
