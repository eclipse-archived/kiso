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
 * This unit test header substitutes "em_adc.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef EM_ADC_TH_HH_
#define EM_ADC_TH_HH_


#include "em_device_th.hh"


typedef enum
{
  adcAcqTime1   = 1,    /**< 1 clock cycle. */
  adcAcqTime2,
  adcAcqTime4,
  adcAcqTime8,
  adcAcqTime16,
  adcAcqTime32,
  adcAcqTime64,
  adcAcqTime128,
  adcAcqTime256
} ADC_AcqTime_TypeDef;


/** Lowpass filter mode. */
typedef enum
{
  /** No filter or decoupling capacitor. */
  adcLPFilterBypass,

  /** On-chip RC filter. */
  adcLPFilterRC,

  /** On-chip decoupling capacitor. */
  adcLPFilterDeCap
} ADC_LPFilter_TypeDef;


/** Oversample rate select. */
typedef enum
{
  /** 2 samples per conversion result. */
  adcOvsRateSel2,

  /** 4 samples per conversion result. */
  adcOvsRateSel4,

  /** 8 samples per conversion result. */
  adcOvsRateSel8,

  /** 16 samples per conversion result. */
  adcOvsRateSel16,

  /** 32 samples per conversion result. */
  adcOvsRateSel32,

  /** 64 samples per conversion result. */
  adcOvsRateSel64,

  /** 128 samples per conversion result. */
  adcOvsRateSel128,

  /** 256 samples per conversion result. */
  adcOvsRateSel256,

  /** 512 samples per conversion result. */
  adcOvsRateSel512,

  /** 1024 samples per conversion result. */
  adcOvsRateSel1024,

  /** 2048 samples per conversion result. */
  adcOvsRateSel2048,

  /** 4096 samples per conversion result. */
  adcOvsRateSel4096
} ADC_OvsRateSel_TypeDef;


/** Peripheral Reflex System signal used to trigger single sample. */
typedef enum
{
  adcPRSSELCh0,
  adcPRSSELCh1,
  adcPRSSELCh2,
  adcPRSSELCh3,
  adcPRSSELCh4,
  adcPRSSELCh5,
  adcPRSSELCh6,
  adcPRSSELCh7
} ADC_PRSSEL_TypeDef;


/** Reference to ADC sample. */
typedef enum
{
  /** Internal 1.25V reference. */
  adcRef1V25,

  /** Internal 2.5V reference. */
  adcRef2V5,

  /** Buffered VDD. */
  adcRefVDD,

  /** Internal differential 5V reference. */
  adcRef5VDIFF,

  /** Single ended ext. ref. from pin 6. */
  adcRefExtSingle,

  /** Differential ext. ref. from pin 6 and 7. */
  adcRef2xExtDiff,

  /** Unbuffered 2xVDD. */
  adcRef2xVDD
} ADC_Ref_TypeDef;


/** Sample resolution. */
typedef enum
{
  adcRes12Bit,
  adcRes8Bit,
  adcRes6Bit,
  adcResOVS
} ADC_Res_TypeDef;


/** Single sample input selection. */
typedef enum
{
  /* Differential mode disabled */
  adcSingleInpCh0,
  adcSingleInpCh1,
  adcSingleInpCh2,
  adcSingleInpCh3,
  adcSingleInpCh4,
  adcSingleInpCh5,
  adcSingleInpCh6,
  adcSingleInpCh7,
  adcSingleInpTemp,
  adcSingleInpVDDDiv3,
  adcSingleInpVDD,
  adcSingleInpVSS,
  adcSingleInpVrefDiv2,
  adcSingleInpDACOut0,
  adcSingleInpDACOut1,
  /* TBD: Use define when available */
  adcSingleInpATEST,                                /**< ATEST. */

  /* Differential mode enabled */
  adcSingleInpCh0Ch1,
  adcSingleInpCh2Ch3,
  adcSingleInpCh4Ch5,
  adcSingleInpCh6Ch7,
  /* TBD: Use define when available */
  adcSingleInpDiff0
} ADC_SingleInput_TypeDef;


/** Acquisition time (in ADC clock cycles). */
typedef enum
{
  /** Start single conversion. */
  adcStartSingle,

  /** Start scan sequence. */
  adcStartScan,

  /**
   * Start scan sequence and single conversion, typically used when tailgating
   * single conversion after scan sequence.
   */
  adcStartScanAndSingle
} ADC_Start_TypeDef;


/** Warm-up mode. */
typedef enum
{
  /** ADC shutdown after each conversion. */
  adcWarmupNormal,

  /** Do not warm-up bandgap references. */
  adcWarmupFastBG,

  /** Reference selected for scan mode kept warm.*/
  adcWarmupKeepScanRefWarm,

  /** ADC and reference selected for scan mode kept warm.*/
  adcWarmupKeepADCWarm
} ADC_Warmup_TypeDef;


/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** ADC init structure, common for single conversion and scan sequence. */
typedef struct
{
  /**
   * Oversampling rate select. In order to have any effect, oversampling must
   * be enabled for single/scan mode.
   */
  ADC_OvsRateSel_TypeDef ovsRateSel;

  /** Lowpass or decoupling capacitor filter to use. */
  ADC_LPFilter_TypeDef   lpfMode;

  /** Warm-up mode to use for ADC. */
  ADC_Warmup_TypeDef     warmUpMode;

  /**
   * Timebase used for ADC warm up. Select N to give (N+1)HFPERCLK cycles.
   * (Additional delay is added for bandgap references, please refer to the
   * reference manual.) Normally, N should be selected so that the timebase
   * is at least 1 us. See ADC_TimebaseCalc() for a way to obtain
   * a suggested timebase of at least 1 us.
   */
  uint8_t                timebase;

  /** Clock division factor N, ADC clock =  HFPERCLK / (N + 1). */
  uint8_t                prescale;

  /** Enable/disable conversion tailgating. */
  bool                   tailgate;
} ADC_Init_TypeDef;

/** Default config for ADC init structure. */
#define ADC_INIT_DEFAULT                                                     \
  { adcOvsRateSel2,                /* 2x oversampling (if enabled). */       \
    adcLPFilterBypass,             /* No input filter selected. */           \
    adcWarmupNormal,               /* ADC shutdown after each conversion. */ \
    0,    /* Use HW default value. */               \
    0,       /* Use HW default value. */               \
    false                          /* Do not use tailgate. */                \
  }


/** Scan sequence init structure. */
typedef struct
{
  /**
   * Peripheral reflex system trigger selection. Only applicable if @p prsEnable
   * is enabled.
   */
  ADC_PRSSEL_TypeDef  prsSel;

  /** Acquisition time (in ADC clock cycles). */
  ADC_AcqTime_TypeDef acqTime;

  /**
   * Sample reference selection. Notice that for external references, the
   * ADC calibration register must be set explicitly.
   */
  ADC_Ref_TypeDef     reference;

  /** Sample resolution. */
  ADC_Res_TypeDef     resolution;

  /**
   * Input scan selection. If single ended (@p diff is false), use logical
   * combination of ADC_SCANCTRL_INPUTMASK_CHx defines. If differential input
   * (@p diff is true), use logical combination of ADC_SCANCTRL_INPUTMASK_CHxCHy
   * defines. (Notice underscore prefix for defines used.)
   */
  uint32_t            input;

  /** Select if single ended or differential input. */
  bool                diff;

  /** Peripheral reflex system trigger enable. */
  bool                prsEnable;

  /** Select if left adjustment should be done. */
  bool                leftAdjust;

  /** Select if continuous conversion until explicit stop. */
  bool                rep;
} ADC_InitScan_TypeDef;

/** Default config for ADC scan init structure. */
#define ADC_INITSCAN_DEFAULT                                                        \
  { adcPRSSELCh0,              /* PRS ch0 (if enabled). */                          \
    adcAcqTime1,               /* 1 ADC_CLK cycle acquisition time. */              \
    adcRef1V25,                /* 1.25V internal reference. */                      \
    adcRes12Bit,               /* 12 bit resolution. */                             \
    0,                         /* No input selected. */                             \
    false,                     /* Single ended input. */                            \
    false,                     /* PRS disabled. */                                  \
    false,                     /* Right adjust. */                                  \
    false                      /* Deactivate conversion after one scan sequence. */ \
  }


/** Single conversion init structure. */
typedef struct
{
  /**
   * Peripheral reflex system trigger selection. Only applicable if @p prsEnable
   * is enabled.
   */
  ADC_PRSSEL_TypeDef      prsSel;

  /** Acquisition time (in ADC clock cycles). */
  ADC_AcqTime_TypeDef     acqTime;

  /**
   * Sample reference selection. Notice that for external references, the
   * ADC calibration register must be set explicitly.
   */
  ADC_Ref_TypeDef         reference;

  /** Sample resolution. */
  ADC_Res_TypeDef         resolution;

  /**
   * Sample input selection, use single ended or differential input according
   * to setting of @p diff.
   */
  ADC_SingleInput_TypeDef input;

  /** Select if single ended or differential input. */
  bool                    diff;

  /** Peripheral reflex system trigger enable. */
  bool                    prsEnable;

  /** Select if left adjustment should be done. */
  bool                    leftAdjust;

  /** Select if continuous conversion until explicit stop. */
  bool                    rep;
} ADC_InitSingle_TypeDef;

/** Default config for ADC single conversion init structure. */
#define ADC_INITSINGLE_DEFAULT                                                      \
  { adcPRSSELCh0,              /* PRS ch0 (if enabled). */                          \
    adcAcqTime1,               /* 1 ADC_CLK cycle acquisition time. */              \
    adcRef1V25,                /* 1.25V internal reference. */                      \
    adcRes12Bit,               /* 12 bit resolution. */                             \
    adcSingleInpCh0,           /* CH0 input selected. */                            \
    false,                     /* Single ended input. */                            \
    false,                     /* PRS disabled. */                                  \
    false,                     /* Right adjust. */                                  \
    false                      /* Deactivate conversion after one scan sequence. */ \
  }

/*structure done for mocking the ADC_TypeDef from efm32lg_adc.h in library
 * which is required by the ADC module for testing*/
typedef struct
{
	uint32_t STATUS;
}ADC_TypeDef;

ADC_TypeDef Temp;

/*ADC0 base pointer mock */
ADC_TypeDef* ADC0 = &Temp;

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(uint32_t,ADC_TimebaseCalc,uint32_t);
FAKE_VALUE_FUNC(uint32_t,ADC_PrescaleCalc,uint32_t,uint32_t);
FAKE_VALUE_FUNC(uint32_t,ADC_DataSingleGet,ADC_TypeDef*);
FAKE_VOID_FUNC(ADC_Init,ADC_TypeDef*,ADC_Init_TypeDef*);
FAKE_VOID_FUNC(ADC_InitSingle,ADC_TypeDef*,ADC_InitSingle_TypeDef*);
FAKE_VOID_FUNC(ADC_Start,ADC_TypeDef*,uint32_t);

#endif /* EM_ADC_TH_HH_ */

/** ************************************************************************* */
