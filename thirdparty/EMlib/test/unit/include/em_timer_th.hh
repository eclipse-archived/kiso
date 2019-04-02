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
 * This unit test header substitutes "em_timer.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */
#ifndef _EM_TIMER_TH_HH_
#define _EM_TIMER_TH_HH_

/* include system header files */
#include "em_device_th.hh"

/* ban or wrap the real em_timer.h interface header */
#define __EM_TIMER_H

/* Enabling Timer3 Functionality */
#define _CMU_HFPERCLKEN0_TIMER3_MASK                0x100UL                                	/**< Bit mask for CMU_TIMER3 */

/* faked variables needs to be initialized by the test fixture */
#define _TIMER_CC_CTRL_MODE_OFF                    0x00000000UL                             /**< Mode OFF for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_MODE_INPUTCAPTURE           0x00000001UL                             /**< Mode INPUTCAPTURE for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_MODE_OUTPUTCOMPARE          0x00000002UL                             /**< Mode OUTPUTCOMPARE for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_MODE_PWM                    0x00000003UL                             /**< Mode PWM for TIMER_CC_CTRL */
#define _TIMER_CTRL_CLKSEL_PRESCHFPERCLK           0x00000000UL                             /**< Mode PRESCHFPERCLK for TIMER_CTRL */
#define _TIMER_CTRL_CLKSEL_CC1                     0x00000001UL                             /**< Mode CC1 for TIMER_CTRL */
#define _TIMER_CTRL_CLKSEL_TIMEROUF                0x00000002UL                             /**< Mode TIMEROUF for TIMER_CTRL */
#define _TIMER_CC_CTRL_ICEDGE_RISING               0x00000000UL                             /**< Mode RISING for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_ICEDGE_FALLING              0x00000001UL                             /**< Mode FALLING for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_ICEDGE_BOTH                 0x00000002UL                             /**< Mode BOTH for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_ICEDGE_NONE                 0x00000003UL                             /**< Mode NONE for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_ICEVCTRL_EVERYEDGE          0x00000000UL                             /**< Mode EVERYEDGE for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_ICEVCTRL_EVERYSECONDEDGE    0x00000001UL                             /**< Mode EVERYSECONDEDGE for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_ICEVCTRL_RISING             0x00000002UL                             /**< Mode RISING for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_ICEVCTRL_FALLING            0x00000003UL                             /**< Mode FALLING for TIMER_CC_CTRL */
#define _TIMER_CTRL_FALLA_NONE                     0x00000000UL                             /**< Mode NONE for TIMER_CTRL */
#define _TIMER_CTRL_FALLA_START                    0x00000001UL                             /**< Mode START for TIMER_CTRL */
#define _TIMER_CTRL_FALLA_STOP                     0x00000002UL                             /**< Mode STOP for TIMER_CTRL */
#define _TIMER_CTRL_FALLA_RELOADSTART              0x00000003UL                             /**< Mode RELOADSTART for TIMER_CTRL */
#define _TIMER_CTRL_MODE_UP                        0x00000000UL                             /**< Mode UP for TIMER_CTRL */
#define _TIMER_CTRL_MODE_DOWN                      0x00000001UL                             /**< Mode DOWN for TIMER_CTRL */
#define _TIMER_CTRL_MODE_UPDOWN                    0x00000002UL                             /**< Mode UPDOWN for TIMER_CTRL */
#define _TIMER_CTRL_MODE_QDEC                      0x00000003UL                             /**< Mode QDEC for TIMER_CTRL */
#define _TIMER_CC_CTRL_CUFOA_NONE                  0x00000000UL                             /**< Mode NONE for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_CUFOA_TOGGLE                0x00000001UL                             /**< Mode TOGGLE for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_CUFOA_CLEAR                 0x00000002UL                             /**< Mode CLEAR for TIMER_CC_CTRL */
#define _TIMER_CC_CTRL_CUFOA_SET                   0x00000003UL                             /**< Mode SET for TIMER_CC_CTRL */
#define _TIMER_CTRL_PRESC_DIV1                     0x00000000UL                             /**< Mode DIV1 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV2                     0x00000001UL                             /**< Mode DIV2 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV4                     0x00000002UL                             /**< Mode DIV4 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV8                     0x00000003UL                             /**< Mode DIV8 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV16                    0x00000004UL                             /**< Mode DIV16 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV32                    0x00000005UL                             /**< Mode DIV32 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV64                    0x00000006UL                             /**< Mode DIV64 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV128                   0x00000007UL                             /**< Mode DIV128 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV256                   0x00000008UL                             /**< Mode DIV256 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV512                   0x00000009UL                             /**< Mode DIV512 for TIMER_CTRL */
#define _TIMER_CTRL_PRESC_DIV1024                  0x0000000AUL                             /**< Mode DIV1024 for TIMER_CTRL */
#define _ADC_SINGLECTRL_PRSSEL_PRSCH0              0x00000000UL                             /**< Mode PRSCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_PRSSEL_PRSCH1              0x00000001UL                             /**< Mode PRSCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_PRSSEL_PRSCH2              0x00000002UL                             /**< Mode PRSCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_PRSSEL_PRSCH3              0x00000003UL                             /**< Mode PRSCH3 for ADC_SINGLECTRL */
#define TIMER_IF_OF                                (0x1UL << 0)                     		/**< Overflow Interrupt Flag */
#define TIMER3       							   ((TIMER_TypeDef *) TIMER3_BASE)        	/**< TIMER3 base pointer */
#define TIMER3_BASE       						   (0x40010C00UL) 							/**< TIMER3 base address  */
#define TIMER_ROUTE_CC0PEN                         (0x1UL << 0)                          	/**< CC Channel 0 Pin Enable */
#define TIMER_ROUTE_CC1PEN                         (0x1UL << 1)                          	/**< CC Channel 1 Pin Enable */
#define TIMER_ROUTE_CC2PEN                         (0x1UL << 2)                          	/**< CC Channel 2 Pin Enable */
#define _TIMER_ROUTE_LOCATION_LOC3                 0x00000003UL                          	/**< Mode LOC3 for TIMER_ROUTE */
#define TIMER_ROUTE_LOCATION_LOC3                  (_TIMER_ROUTE_LOCATION_LOC3 << 16)    	/**< Shifted mode LOC3 for TIMER_ROUTE */
#define TIMER_IF_CC0                               (0x1UL << 4)                     		/**< CC Channel 0 Interrupt Flag */
#define _TIMER_IF_CC0_SHIFT                        4                                		/**< Shift value for TIMER_CC0 */
#define _TIMER_IF_CC0_MASK                         0x10UL                           		/**< Bit mask for TIMER_CC0 */
#define _TIMER_IF_CC0_DEFAULT                      0x00000000UL                     		/**< Mode DEFAULT for TIMER_IF */
#define TIMER_IF_CC0_DEFAULT                       (_TIMER_IF_CC0_DEFAULT << 4)     		/**< Shifted mode DEFAULT for TIMER_IF */
#define TIMER_IF_CC1                               (0x1UL << 5)                     		/**< CC Channel 1 Interrupt Flag */
#define _TIMER_IF_CC1_SHIFT                        5                                		/**< Shift value for TIMER_CC1 */
#define _TIMER_IF_CC1_MASK                         0x20UL                           		/**< Bit mask for TIMER_CC1 */
#define _TIMER_IF_CC1_DEFAULT                      0x00000000UL                     		/**< Mode DEFAULT for TIMER_IF */
#define TIMER_IF_CC1_DEFAULT                       (_TIMER_IF_CC1_DEFAULT << 5)     		/**< Shifted mode DEFAULT for TIMER_IF */
#define TIMER_IF_CC2                               (0x1UL << 6)                     		/**< CC Channel 2 Interrupt Flag */
#define _TIMER_IF_CC2_SHIFT                        6                                		/**< Shift value for TIMER_CC2 */
#define _TIMER_IF_CC2_MASK                         0x40UL                           		/**< Bit mask for TIMER_CC2 */
#define _TIMER_IF_CC2_DEFAULT                      0x00000000UL                     		/**< Mode DEFAULT for TIMER_IF */
#define TIMER_IF_CC2_DEFAULT                       (_TIMER_IF_CC2_DEFAULT << 6)     		/**< Shifted mode DEFAULT for TIMER_IF */
#define _TIMER_ROUTE_LOCATION_SHIFT                16                                    	/**< Shift value for TIMER_LOCATION */
#define _TIMER_ROUTE_LOCATION_MASK                 0x70000UL                             	/**< Bit mask for TIMER_LOCATION */
#define _TIMER_ROUTE_LOCATION_DEFAULT              0x00000000UL                          	/**< Mode DEFAULT for TIMER_ROUTE */
#define _TIMER_ROUTE_LOCATION_LOC0                 0x00000000UL                          	/**< Mode LOC0 for TIMER_ROUTE */
#define _TIMER_ROUTE_LOCATION_LOC1                 0x00000001UL                          	/**< Mode LOC1 for TIMER_ROUTE */
#define _TIMER_ROUTE_LOCATION_LOC2                 0x00000002UL                          	/**< Mode LOC2 for TIMER_ROUTE */
#define _TIMER_ROUTE_LOCATION_LOC3                 0x00000003UL                          	/**< Mode LOC3 for TIMER_ROUTE */
#define _TIMER_ROUTE_LOCATION_LOC4                 0x00000004UL                          	/**< Mode LOC4 for TIMER_ROUTE */
#define _TIMER_ROUTE_LOCATION_LOC5                 0x00000005UL                          	/**< Mode LOC5 for TIMER_ROUTE */
#define TIMER_ROUTE_LOCATION_DEFAULT               (_TIMER_ROUTE_LOCATION_DEFAULT << 16) 	/**< Shifted mode DEFAULT for TIMER_ROUTE */
#define TIMER_ROUTE_LOCATION_LOC0                  (_TIMER_ROUTE_LOCATION_LOC0 << 16)    	/**< Shifted mode LOC0 for TIMER_ROUTE */
#define TIMER_ROUTE_LOCATION_LOC1                  (_TIMER_ROUTE_LOCATION_LOC1 << 16)    	/**< Shifted mode LOC1 for TIMER_ROUTE */
#define TIMER_ROUTE_LOCATION_LOC2                  (_TIMER_ROUTE_LOCATION_LOC2 << 16)    	/**< Shifted mode LOC2 for TIMER_ROUTE */
#define TIMER_ROUTE_LOCATION_LOC3                  (_TIMER_ROUTE_LOCATION_LOC3 << 16)    	/**< Shifted mode LOC3 for TIMER_ROUTE */
#define TIMER_ROUTE_LOCATION_LOC4                  (_TIMER_ROUTE_LOCATION_LOC4 << 16)    	/**< Shifted mode LOC4 for TIMER_ROUTE */
#define TIMER_ROUTE_LOCATION_LOC5                  (_TIMER_ROUTE_LOCATION_LOC5 << 16)    	/**< Shifted mode LOC5 for TIMER_ROUTE */



/* mock-ups for the provided interfaces */
/** Timer compare/capture mode. */
typedef enum
{
  timerCCModeOff     = _TIMER_CC_CTRL_MODE_OFF,           /**< Channel turned off. */
  timerCCModeCapture = _TIMER_CC_CTRL_MODE_INPUTCAPTURE,  /**< Input capture. */
  timerCCModeCompare = _TIMER_CC_CTRL_MODE_OUTPUTCOMPARE, /**< Output compare. */
  timerCCModePWM     = _TIMER_CC_CTRL_MODE_PWM            /**< Pulse-Width modulation. */
} TIMER_CCMode_TypeDef;


/** Clock select. */
typedef enum
{
  /** Prescaled HFPER clock. */
  timerClkSelHFPerClk = _TIMER_CTRL_CLKSEL_PRESCHFPERCLK,

  /** Prescaled HFPER clock. */
  timerClkSelCC1      = _TIMER_CTRL_CLKSEL_CC1,

  /**
   * Cascaded, clocked by underflow (down-counting) or overflow (up-counting)
   * by lower numbered timer.
   */
  timerClkSelCascade  = _TIMER_CTRL_CLKSEL_TIMEROUF
} TIMER_ClkSel_TypeDef;


/** Input capture edge select. */
typedef enum
{
  /** Rising edges detected. */
  timerEdgeRising  = _TIMER_CC_CTRL_ICEDGE_RISING,

  /** Falling edges detected. */
  timerEdgeFalling = _TIMER_CC_CTRL_ICEDGE_FALLING,

  /** Both edges detected. */
  timerEdgeBoth    = _TIMER_CC_CTRL_ICEDGE_BOTH,

  /** No edge detection, leave signal as is. */
  timerEdgeNone    = _TIMER_CC_CTRL_ICEDGE_NONE
} TIMER_Edge_TypeDef;


/** Input capture event control. */
typedef enum
{
  /** PRS output pulse, interrupt flag and DMA request set on every capture. */
  timerEventEveryEdge    = _TIMER_CC_CTRL_ICEVCTRL_EVERYEDGE,
  /** PRS output pulse, interrupt flag and DMA request set on every second capture. */
  timerEventEvery2ndEdge = _TIMER_CC_CTRL_ICEVCTRL_EVERYSECONDEDGE,
  /**
   * PRS output pulse, interrupt flag and DMA request set on rising edge (if
   * input capture edge = BOTH).
   */
  timerEventRising       = _TIMER_CC_CTRL_ICEVCTRL_RISING,
  /**
   * PRS output pulse, interrupt flag and DMA request set on falling edge (if
   * input capture edge = BOTH).
   */
  timerEventFalling      = _TIMER_CC_CTRL_ICEVCTRL_FALLING
} TIMER_Event_TypeDef;


/** Input edge action. */
typedef enum
{
  /** No action taken. */
  timerInputActionNone        = _TIMER_CTRL_FALLA_NONE,

  /** Start counter without reload. */
  timerInputActionStart       = _TIMER_CTRL_FALLA_START,

  /** Stop counter without reload. */
  timerInputActionStop        = _TIMER_CTRL_FALLA_STOP,

  /** Reload and start counter. */
  timerInputActionReloadStart = _TIMER_CTRL_FALLA_RELOADSTART
} TIMER_InputAction_TypeDef;


/** Timer mode. */
typedef enum
{
  timerModeUp     = _TIMER_CTRL_MODE_UP,     /**< Up-counting. */
  timerModeDown   = _TIMER_CTRL_MODE_DOWN,   /**< Down-counting. */
  timerModeUpDown = _TIMER_CTRL_MODE_UPDOWN, /**< Up/down-counting. */
  timerModeQDec   = _TIMER_CTRL_MODE_QDEC    /**< Quadrature decoder. */
} TIMER_Mode_TypeDef;


/** Compare/capture output action. */
typedef enum
{
  /** No action. */
  timerOutputActionNone   = _TIMER_CC_CTRL_CUFOA_NONE,

  /** Toggle on event. */
  timerOutputActionToggle = _TIMER_CC_CTRL_CUFOA_TOGGLE,

  /** Clear on event. */
  timerOutputActionClear  = _TIMER_CC_CTRL_CUFOA_CLEAR,

  /** Set on event. */
  timerOutputActionSet    = _TIMER_CC_CTRL_CUFOA_SET
} TIMER_OutputAction_TypeDef;


/** Prescaler. */
typedef enum
{
  timerPrescale1    = _TIMER_CTRL_PRESC_DIV1,     /**< Divide by 1. */
  timerPrescale2    = _TIMER_CTRL_PRESC_DIV2,     /**< Divide by 2. */
  timerPrescale4    = _TIMER_CTRL_PRESC_DIV4,     /**< Divide by 4. */
  timerPrescale8    = _TIMER_CTRL_PRESC_DIV8,     /**< Divide by 8. */
  timerPrescale16   = _TIMER_CTRL_PRESC_DIV16,    /**< Divide by 16. */
  timerPrescale32   = _TIMER_CTRL_PRESC_DIV32,    /**< Divide by 32. */
  timerPrescale64   = _TIMER_CTRL_PRESC_DIV64,    /**< Divide by 64. */
  timerPrescale128  = _TIMER_CTRL_PRESC_DIV128,   /**< Divide by 128. */
  timerPrescale256  = _TIMER_CTRL_PRESC_DIV256,   /**< Divide by 256. */
  timerPrescale512  = _TIMER_CTRL_PRESC_DIV512,   /**< Divide by 512. */
  timerPrescale1024 = _TIMER_CTRL_PRESC_DIV1024   /**< Divide by 1024. */
} TIMER_Prescale_TypeDef;

#define ADC_PRESENT
#if defined(ADC_PRESENT)
/** Peripheral Reflex System signal. */
typedef enum
{
  timerPRSSELCh0 = _ADC_SINGLECTRL_PRSSEL_PRSCH0, /**< PRS channel 0. */
  timerPRSSELCh1 = _ADC_SINGLECTRL_PRSSEL_PRSCH1, /**< PRS channel 1. */
  timerPRSSELCh2 = _ADC_SINGLECTRL_PRSSEL_PRSCH2, /**< PRS channel 2. */
  timerPRSSELCh3 = _ADC_SINGLECTRL_PRSSEL_PRSCH3, /**< PRS channel 3. */

#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH7 )
  timerPRSSELCh4 = _ADC_SINGLECTRL_PRSSEL_PRSCH4, /**< PRS channel 4. */
  timerPRSSELCh5 = _ADC_SINGLECTRL_PRSSEL_PRSCH5, /**< PRS channel 5. */
  timerPRSSELCh6 = _ADC_SINGLECTRL_PRSSEL_PRSCH6, /**< PRS channel 6. */
  timerPRSSELCh7 = _ADC_SINGLECTRL_PRSSEL_PRSCH7  /**< PRS channel 7. */
#endif
} TIMER_PRSSEL_TypeDef;
#endif

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** TIMER initialization structure. */
typedef struct
{
  /** Start counting when init completed. */
  bool                      enable;

  /** Counter shall keep running during debug halt. */
  bool                      debugRun;

  /** Prescaling factor, if HFPER clock used. */
  TIMER_Prescale_TypeDef    prescale;

  /** Clock selection. */
  TIMER_ClkSel_TypeDef      clkSel;

#if defined( TIMER_CTRL_X2CNT ) && defined( TIMER_CTRL_ATI )
  /** 2x Count mode, counter increments/decrements by 2, meant for PWN mode. */
  bool                      count2x;

  /** ATI (Always Track Inputs) makes CCPOL always track
   * the polarity of the inputs. */
  bool                      ati;
#endif

  /** Action on falling input edge. */
  TIMER_InputAction_TypeDef fallAction;

  /** Action on rising input edge. */
  TIMER_InputAction_TypeDef riseAction;

  /** Counting mode. */
  TIMER_Mode_TypeDef        mode;

  /** DMA request clear on active. */
  bool                      dmaClrAct;

  /** Select X2 or X4 quadrature decode mode (if used). */
  bool                      quadModeX4;

  /** Determines if only counting up or down once. */
  bool                      oneShot;

  /** Timer start/stop/reload by other timers. */
  bool                      sync;
} TIMER_Init_TypeDef;

/** Default config for TIMER init structure. */
#if defined( TIMER_CTRL_X2CNT ) && defined( TIMER_CTRL_ATI )
#define TIMER_INIT_DEFAULT                                                              \
  { true,                   /* Enable timer when init complete. */                      \
    false,                  /* Stop counter during debug halt. */                       \
    timerPrescale1,         /* No prescaling. */                                        \
    timerClkSelHFPerClk,    /* Select HFPER clock. */                                   \
    false,                  /* Not 2x count mode. */                                    \
    false,                  /* No ATI. */                                               \
    timerInputActionNone,   /* No action on falling input edge. */                      \
    timerInputActionNone,   /* No action on rising input edge. */                       \
    timerModeUp,            /* Up-counting. */                                          \
    false,                  /* Do not clear DMA requests when DMA channel is active. */ \
    false,                  /* Select X2 quadrature decode mode (if used). */           \
    false,                  /* Disable one shot. */                                     \
    false                   /* Not started/stopped/reloaded by other timers. */         \
  }
#else
#define TIMER_INIT_DEFAULT                                                              \
  { true,                   /* Enable timer when init complete. */                      \
    false,                  /* Stop counter during debug halt. */                       \
    timerPrescale1,         /* No prescaling. */                                        \
    timerClkSelHFPerClk,    /* Select HFPER clock. */                                   \
    timerInputActionNone,   /* No action on falling input edge. */                      \
    timerInputActionNone,   /* No action on rising input edge. */                       \
    timerModeUp,            /* Up-counting. */                                          \
    false,                  /* Do not clear DMA requests when DMA channel is active. */ \
    false,                  /* Select X2 quadrature decode mode (if used). */           \
    false,                  /* Disable one shot. */                                     \
    false                   /* Not started/stopped/reloaded by other timers. */         \
  }
#endif

/** TIMER compare/capture initialization structure. */
typedef struct
{
  /** Input capture event control. */
  TIMER_Event_TypeDef        eventCtrl;

  /** Input capture edge select. */
  TIMER_Edge_TypeDef         edge;

#if defined(ADC_PRESENT)
  /**
   * Peripheral reflex system trigger selection. Only applicable if @p prsInput
   * is enabled.
   */
  TIMER_PRSSEL_TypeDef       prsSel;
#endif

  /** Counter underflow output action. */
  TIMER_OutputAction_TypeDef cufoa;

  /** Counter overflow output action. */
  TIMER_OutputAction_TypeDef cofoa;

  /** Counter match output action. */
  TIMER_OutputAction_TypeDef cmoa;

  /** Compare/capture channel mode. */
  TIMER_CCMode_TypeDef       mode;

  /** Enable digital filter. */
  bool                       filter;

  /** Select TIMERnCCx (false) or PRS input (true). */
  bool                       prsInput;

  /**
   * Compare output initial state. Only used in Output Compare and PWM mode.
   * When true, the compare/PWM output is set high when the counter is
   * disabled. When counting resumes, this value will represent the initial
   * value for the compare/PWM output. If the bit is cleared, the output
   * will be cleared when the counter is disabled.
   */
  bool                       coist;

  /** Invert output from compare/capture channel. */
  bool                       outInvert;
} TIMER_InitCC_TypeDef;

/** Default config for TIMER compare/capture init structure. */
#if defined(ADC_PRESENT)
#define TIMER_INITCC_DEFAULT                                                   \
  { timerEventEveryEdge,      /* Event on every capture. */                    \
    timerEdgeRising,          /* Input capture edge on rising edge. */         \
    timerPRSSELCh0,           /* Not used by default, select PRS channel 0. */ \
    timerOutputActionNone,    /* No action on underflow. */                    \
    timerOutputActionNone,    /* No action on overflow. */                     \
    timerOutputActionNone,    /* No action on match. */                        \
    timerCCModeOff,           /* Disable compare/capture channel. */           \
    false,                    /* Disable filter. */                            \
    false,                    /* Select TIMERnCCx input. */                    \
    false,                    /* Clear output when countre disabled. */        \
    false                     /* Do not invert output. */                      \
  }
#else
#define TIMER_INITCC_DEFAULT                                                   \
  { timerEventEveryEdge,      /* Event on every capture. */                    \
    timerEdgeRising,          /* Input capture edge on rising edge. */         \
    timerOutputActionNone,    /* No action on underflow. */                    \
    timerOutputActionNone,    /* No action on overflow. */                     \
    timerOutputActionNone,    /* No action on match. */                        \
    timerCCModeOff,           /* Disable compare/capture channel. */           \
    false,                    /* Disable filter. */                            \
    false,                    /* Select TIMERnCCx input. */                    \
    false,                    /* Clear output when countre disabled. */        \
    false                     /* Do not invert output. */                      \
  }
#endif

typedef struct
{
  __IO uint32_t CTRL; /**< CC Channel Control Register  */
  __IO uint32_t CCV;  /**< CC Channel Value Register  */
  __I uint32_t  CCVP; /**< CC Channel Value Peek Register  */
  __IO uint32_t CCVB; /**< CC Channel Buffer Register  */
} TIMER_CC_TypeDef;

typedef struct
{
  __IO uint32_t    CTRL;         /**< Control Register  */
  __IO uint32_t    CMD;          /**< Command Register  */
  __I uint32_t     STATUS;       /**< Status Register  */
  __IO uint32_t    IEN;          /**< Interrupt Enable Register  */
  __I uint32_t     IF;           /**< Interrupt Flag Register  */
  __IO uint32_t    IFS;          /**< Interrupt Flag Set Register  */
  __IO uint32_t    IFC;          /**< Interrupt Flag Clear Register  */
  __IO uint32_t    TOP;          /**< Counter Top Value Register  */
  __IO uint32_t    TOPB;         /**< Counter Top Value Buffer Register  */
  __IO uint32_t    CNT;          /**< Counter Value Register  */
  __IO uint32_t    ROUTE;        /**< I/O Routing Register  */

  uint32_t         RESERVED0[1]; /**< Reserved registers */

  TIMER_CC_TypeDef CC[3];        /**< Compare/Capture Channel */

  uint32_t         RESERVED1[4]; /**< Reserved for future use **/
  __IO uint32_t    DTCTRL;       /**< DTI Control Register  */
  __IO uint32_t    DTTIME;       /**< DTI Time Control Register  */
  __IO uint32_t    DTFC;         /**< DTI Fault Configuration Register  */
  __IO uint32_t    DTOGEN;       /**< DTI Output Generation Enable Register  */
  __I uint32_t     DTFAULT;      /**< DTI Fault Register  */
  __O uint32_t     DTFAULTC;     /**< DTI Fault Clear Register  */
  __IO uint32_t    DTLOCK;       /**< DTI Configuration Lock Register  */
} TIMER_TypeDef;                 /** @} */


/* Hardware Dependent Usage Register Maps Faked with Fixed Memory */
#ifdef _TIMER3_FAKE_
#undef TIMER3
TIMER_TypeDef timer3Memory;
#define TIMER3 ((TIMER_TypeDef*)(&timer3Memory))
#endif
TIMER_TypeDef *TIMER0 =(TIMER_TypeDef*) malloc(sizeof(TIMER_TypeDef));

#define TIMER_CMD_STOP                             (0x1UL << 1)                    /**< Stop Timer */
#define _TIMER_TOP_RESETVALUE                      0x0000FFFFUL                  /**< Default value for TIMER_TOP */
#define _TIMER_CTRL_RESETVALUE                     0x00000000UL                             /**< Default value for TIMER_CTRL */
#define _TIMER_CC_CTRL_RESETVALUE                  0x00000000UL                                    /**< Default value for TIMER_CC_CTRL */
#define _TIMER_IEN_RESETVALUE                      0x00000000UL                      /**< Default value for TIMER_IEN */
TIMER_TypeDef *TIMER1 =(TIMER_TypeDef*) malloc(sizeof(TIMER_TypeDef));

/* faked variables needs to be initialized by the test fixture */
FAKE_VALUE_FUNC(uint32_t,TIMER_CaptureGet,TIMER_TypeDef *,unsigned int);
FAKE_VOID_FUNC(TIMER_CompareBufSet,TIMER_TypeDef *,unsigned int,uint32_t);
FAKE_VOID_FUNC(TIMER_CompareSet,TIMER_TypeDef *,unsigned int,uint32_t);
FAKE_VALUE_FUNC(uint32_t,TIMER_CounterGet,TIMER_TypeDef *);
FAKE_VOID_FUNC(TIMER_CounterSet,TIMER_TypeDef *,uint32_t);
FAKE_VOID_FUNC(TIMER_Enable,TIMER_TypeDef *,bool);
FAKE_VOID_FUNC(TIMER_Init,TIMER_TypeDef *,const TIMER_Init_TypeDef *);
FAKE_VOID_FUNC(TIMER_InitCC,TIMER_TypeDef *,unsigned int,const TIMER_InitCC_TypeDef *);
FAKE_VOID_FUNC(TIMER_IntClear,TIMER_TypeDef *,uint32_t);
FAKE_VOID_FUNC(TIMER_IntDisable,TIMER_TypeDef *,uint32_t);
FAKE_VOID_FUNC(TIMER_IntEnable,TIMER_TypeDef *,uint32_t);
FAKE_VALUE_FUNC(uint32_t,TIMER_IntGet,TIMER_TypeDef *);
FAKE_VALUE_FUNC(uint32_t,TIMER_IntGetEnabled,TIMER_TypeDef *);
FAKE_VOID_FUNC(TIMER_IntSet,TIMER_TypeDef *,uint32_t);
FAKE_VOID_FUNC(TIMER_Lock,TIMER_TypeDef *);
FAKE_VOID_FUNC(TIMER_Reset,TIMER_TypeDef *);
FAKE_VOID_FUNC(TIMER_TopBufSet,TIMER_TypeDef *,uint32_t);
FAKE_VALUE_FUNC(uint32_t,TIMER_TopGet,TIMER_TypeDef *);
FAKE_VOID_FUNC(TIMER_TopSet,TIMER_TypeDef *,uint32_t);
FAKE_VOID_FUNC(TIMER_Unlock,TIMER_TypeDef *);

#endif /* _EM_TIMER_TH_HH_ */

/** ************************************************************************* */
