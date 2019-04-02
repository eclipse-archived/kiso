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
 * This unit test header substitutes "em_cmu.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef EM_CMU_TH_HH_
#define EM_CMU_TH_HH_

/* include system header files */
#include "em_device_th.hh"

/* ban the real em_cmu.h interface header */
#define __EM_CMU_H

/* faked variables needs to be initialized by the test fixture */
/* Select register ids, for internal use */
#define _CMU_HFPERCLKEN0_TIMER0_MASK
#define _CMU_HFPERCLKEN0_TIMER0_SHIFT              4
#define CMU_NOSEL_REG              0
#define CMU_HFCLKSEL_REG           1
#define CMU_LFACLKSEL_REG          2
#define CMU_LFBCLKSEL_REG          3
#define CMU_DBGCLKSEL_REG          4
#if defined (_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY)
#define CMU_USBCCLKSEL_REG         5
#endif

#define CMU_SEL_REG_POS            0
#define CMU_SEL_REG_MASK           0xf

/* Divisor register ids, for internal use */
#define CMU_NODIV_REG              0
#define CMU_HFPERCLKDIV_REG        1
#define CMU_HFCORECLKDIV_REG       2
#define CMU_LFAPRESC0_REG          3
#define CMU_LFBPRESC0_REG          4
#if defined (_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY)
#define CMU_HFCLKDIV_REG           5
#endif
#define CMU_DIV_REG_POS            4
#define CMU_DIV_REG_MASK           0xf

/* Enable register ids, for internal use */
#define CMU_NO_EN_REG              0
#define CMU_HFPERCLKDIV_EN_REG     1
#define CMU_HFPERCLKEN0_EN_REG     2
#define CMU_HFCORECLKEN0_EN_REG    3
#define CMU_LFACLKEN0_EN_REG       4
#define CMU_LFBCLKEN0_EN_REG       5
#define CMU_PCNT_EN_REG            6

#define CMU_EN_REG_POS             8
#define CMU_EN_REG_MASK            0xf

/* Enable register bit position, for internal use */
#define CMU_EN_BIT_POS             12
#define CMU_EN_BIT_MASK            0x1f

/* Clock branch bitfield position, for internal use */
#define CMU_HF_CLK_BRANCH          0
#define CMU_HFPER_CLK_BRANCH       1
#define CMU_HFCORE_CLK_BRANCH      2
#define CMU_LFA_CLK_BRANCH         3
#define CMU_RTC_CLK_BRANCH         4
#define CMU_LETIMER_CLK_BRANCH     5
#define CMU_LCDPRE_CLK_BRANCH      6
#define CMU_LCD_CLK_BRANCH         7
#define CMU_LESENSE_CLK_BRANCH     8
#define CMU_LFB_CLK_BRANCH         9
#define CMU_LEUART0_CLK_BRANCH     10
#define CMU_LEUART1_CLK_BRANCH     11
#define CMU_DBG_CLK_BRANCH         12
#define CMU_AUX_CLK_BRANCH         13
#define CMU_USBC_CLK_BRANCH        14

#define CMU_CLK_BRANCH_POS         17
#define CMU_CLK_BRANCH_MASK        0x1f

#define _CMU_HFPERCLKEN0_USART0_SHIFT              0
#define _CMU_HFPERCLKEN0_USART1_SHIFT              1
#define _CMU_HFPERCLKEN0_USART2_SHIFT              2


/** @endcond */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** Clock divisors. These values are valid for prescalers. */
#define cmuClkDiv_1     1     /**< Divide clock by 1. */
#define cmuClkDiv_2     2     /**< Divide clock by 2. */
#define cmuClkDiv_4     4     /**< Divide clock by 4. */
#define cmuClkDiv_8     8     /**< Divide clock by 8. */
#define cmuClkDiv_16    16    /**< Divide clock by 16. */
#define cmuClkDiv_32    32    /**< Divide clock by 32. */
#define cmuClkDiv_64    64    /**< Divide clock by 64. */
#define cmuClkDiv_128   128   /**< Divide clock by 128. */
#define cmuClkDiv_256   256   /**< Divide clock by 256. */
#define cmuClkDiv_512   512   /**< Divide clock by 512. */
#define cmuClkDiv_1024  1024  /**< Divide clock by 1024. */
#define cmuClkDiv_2048  2048  /**< Divide clock by 2048. */
#define cmuClkDiv_4096  4096  /**< Divide clock by 4096. */
#define cmuClkDiv_8192  8192  /**< Divide clock by 8192. */
#define cmuClkDiv_16384 16384 /**< Divide clock by 16384. */
#define cmuClkDiv_32768 32768 /**< Divide clock by 32768. */

#define _CMU_HFPERCLKEN0_GPIO_SHIFT                12

#define _CMU_HFPERCLKEN0_TIMER3_SHIFT               8                                      /**< Shift value for CMU_TIMER3 */
/** Clock divider configuration */
typedef uint32_t CMU_ClkDiv_TypeDef;

/** High frequency RC bands. */
typedef enum
{
  /** 1MHz RC band. */
  cmuHFRCOBand_1MHz  = _CMU_HFRCOCTRL_BAND_1MHZ,
  /** 7MHz RC band. */
  cmuHFRCOBand_7MHz  = _CMU_HFRCOCTRL_BAND_7MHZ,
  /** 11MHz RC band. */
  cmuHFRCOBand_11MHz = _CMU_HFRCOCTRL_BAND_11MHZ,
  /** 14MHz RC band. */
  cmuHFRCOBand_14MHz = _CMU_HFRCOCTRL_BAND_14MHZ,
  /** 21MHz RC band. */
  cmuHFRCOBand_21MHz = _CMU_HFRCOCTRL_BAND_21MHZ,
  /** 28MHz RC band. */
  cmuHFRCOBand_28MHz = _CMU_HFRCOCTRL_BAND_28MHZ
} CMU_HFRCOBand_TypeDef;


#if defined(_EFM32_TINY_FAMILY) || defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY)
/** AUX High frequency RC bands. */
typedef enum
{
  /** 1MHz RC band. */
  cmuAUXHFRCOBand_1MHz  = _CMU_AUXHFRCOCTRL_BAND_1MHZ,
  /** 7MHz RC band. */
  cmuAUXHFRCOBand_7MHz  = _CMU_AUXHFRCOCTRL_BAND_7MHZ,
  /** 11MHz RC band. */
  cmuAUXHFRCOBand_11MHz = _CMU_AUXHFRCOCTRL_BAND_11MHZ,
  /** 14MHz RC band. */
  cmuAUXHFRCOBand_14MHz = _CMU_AUXHFRCOCTRL_BAND_14MHZ,
  /** 21MHz RC band. */
  cmuAUXHFRCOBand_21MHz = _CMU_AUXHFRCOCTRL_BAND_21MHZ,
  /** 28MHz RC band. */
  cmuAUXHFRCOBand_28MHz = _CMU_AUXHFRCOCTRL_BAND_28MHZ
} CMU_AUXHFRCOBand_TypeDef;
#endif


/** Selectable clock sources. */
typedef enum
{
  cmuSelect_Error,      /**< Usage error. */
  cmuSelect_Disabled,   /**< Clock selector disabled. */
  cmuSelect_LFXO,       /**< Low frequency crystal oscillator. */
  cmuSelect_LFRCO,      /**< Low frequency RC oscillator. */
  cmuSelect_HFXO,       /**< High frequency crystal oscillator. */
  cmuSelect_HFRCO,      /**< High frequency RC oscillator. */
  cmuSelect_CORELEDIV2, /**< Core low energy clock divided by 2. */
  cmuSelect_AUXHFRCO,   /**< Auxilliary clock source can be used for debug clock */
  cmuSelect_HFCLK,      /**< Divided HFCLK on Giant for debug clock, undivided on Tiny Gecko and for USBC (not used on Gecko) */
#if !defined(_EFM32_GECKO_FAMILY)
  cmuSelect_ULFRCO,     /**< Ultra low frequency RC oscillator. */
#endif
} CMU_Select_TypeDef;

/** Clock points in CMU. Please refer to CMU overview in reference manual. */
typedef enum
{
  /*******************/
  /* HF clock branch */
  /*******************/

  /** High frequency clock */
#if defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY)
  cmuClock_HF = (CMU_HFCLKDIV_REG << CMU_DIV_REG_POS) |
                (CMU_HFCLKSEL_REG << CMU_SEL_REG_POS) |
                (CMU_NO_EN_REG << CMU_EN_REG_POS) |
                (0 << CMU_EN_BIT_POS) |
                (CMU_HF_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#else
  cmuClock_HF = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                (CMU_HFCLKSEL_REG << CMU_SEL_REG_POS) |
                (CMU_NO_EN_REG << CMU_EN_REG_POS) |
                (0 << CMU_EN_BIT_POS) |
                (CMU_HF_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

  /** Debug clock */
  cmuClock_DBG = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_DBGCLKSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_NO_EN_REG << CMU_EN_REG_POS) |
                 (0 << CMU_EN_BIT_POS) |
                 (CMU_DBG_CLK_BRANCH << CMU_CLK_BRANCH_POS),

  /** AUX clock */
  cmuClock_AUX = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_NO_EN_REG << CMU_EN_REG_POS) |
                 (0 << CMU_EN_BIT_POS) |
                 (CMU_AUX_CLK_BRANCH << CMU_CLK_BRANCH_POS),

  /**********************************/
  /* HF peripheral clock sub-branch */
  /**********************************/

  /** High frequency peripheral clock */
  cmuClock_HFPER = (CMU_HFPERCLKDIV_REG << CMU_DIV_REG_POS) |
                   (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                   (CMU_HFPERCLKDIV_EN_REG << CMU_EN_REG_POS) |
                   (_CMU_HFPERCLKDIV_HFPERCLKEN_SHIFT << CMU_EN_BIT_POS) |
                   (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),

  /** Universal sync/async receiver/transmitter 0 clock. */
  cmuClock_USART0 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                    (_CMU_HFPERCLKEN0_USART0_SHIFT << CMU_EN_BIT_POS) |
                    (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),

  /** Universal sync/async receiver/transmitter 1 clock. */
  cmuClock_USART1 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                    (_CMU_HFPERCLKEN0_USART1_SHIFT << CMU_EN_BIT_POS) |
                    (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),

  /** Universal sync/async receiver/transmitter 2 clock. */
  cmuClock_USART2 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                    (_CMU_HFPERCLKEN0_USART2_SHIFT << CMU_EN_BIT_POS) |
                    (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),

#if defined(_CMU_HFPERCLKEN0_UART0_MASK)
  /** Universal async receiver/transmitter 0 clock. */
  cmuClock_UART0 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                   (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                   (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                   (_CMU_HFPERCLKEN0_UART0_SHIFT << CMU_EN_BIT_POS) |
                   (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_UART1_MASK)
  /** Universal async receiver/transmitter 1 clock. */
  cmuClock_UART1 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                   (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                   (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                   (_CMU_HFPERCLKEN0_UART1_SHIFT << CMU_EN_BIT_POS) |
                   (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_TIMER0_MASK)
  /** Timer 0 clock. */
  cmuClock_TIMER0 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                    (_CMU_HFPERCLKEN0_TIMER0_SHIFT << CMU_EN_BIT_POS) |
                    (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_TIMER1_MASK)
  /** Timer 1 clock. */
  cmuClock_TIMER1 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                    (_CMU_HFPERCLKEN0_TIMER1_SHIFT << CMU_EN_BIT_POS) |
                    (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_TIMER2_MASK)
  /** Timer 2 clock. */
  cmuClock_TIMER2 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                    (_CMU_HFPERCLKEN0_TIMER2_SHIFT << CMU_EN_BIT_POS) |
                    (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_TIMER3_MASK)
  /** Timer 3 clock. */
  cmuClock_TIMER3 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                    (_CMU_HFPERCLKEN0_TIMER3_SHIFT << CMU_EN_BIT_POS) |
                    (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_ACMP0_MASK)
  /** Analog comparator 0 clock. */
  cmuClock_ACMP0 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                   (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                   (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                   (_CMU_HFPERCLKEN0_ACMP0_SHIFT << CMU_EN_BIT_POS) |
                   (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_ACMP1_MASK)
  /** Analog comparator 1 clock. */
  cmuClock_ACMP1 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                   (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                   (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                   (_CMU_HFPERCLKEN0_ACMP1_SHIFT << CMU_EN_BIT_POS) |
                   (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_PRS_MASK)
  /** Peripheral reflex system clock. */
  cmuClock_PRS = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                 (_CMU_HFPERCLKEN0_PRS_SHIFT << CMU_EN_BIT_POS) |
                 (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_DAC0_MASK)
  /** Digital to analog converter 0 clock. */
  cmuClock_DAC0 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                  (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                  (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                  (_CMU_HFPERCLKEN0_DAC0_SHIFT << CMU_EN_BIT_POS) |
                  (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(GPIO_PRESENT)
  /** General purpose input/output clock. */
  cmuClock_GPIO = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                  (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                  (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                  (_CMU_HFPERCLKEN0_GPIO_SHIFT << CMU_EN_BIT_POS) |
                  (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(VCMP_PRESENT)
  /** Voltage comparator clock. */
  cmuClock_VCMP = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                  (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                  (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                  (_CMU_HFPERCLKEN0_VCMP_SHIFT << CMU_EN_BIT_POS) |
                  (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_ADC0_MASK)
  /** Analog to digital converter 0 clock. */
  cmuClock_ADC0 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                  (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                  (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                  (_CMU_HFPERCLKEN0_ADC0_SHIFT << CMU_EN_BIT_POS) |
                  (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_I2C0_MASK)
  /** I2C 0 clock. */
  cmuClock_I2C0 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                  (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                  (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                  (_CMU_HFPERCLKEN0_I2C0_SHIFT << CMU_EN_BIT_POS) |
                  (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_HFPERCLKEN0_I2C1_MASK)
  /** I2C 1 clock. */
  cmuClock_I2C1 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                  (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                  (CMU_HFPERCLKEN0_EN_REG << CMU_EN_REG_POS) |
                  (_CMU_HFPERCLKEN0_I2C1_SHIFT << CMU_EN_BIT_POS) |
                  (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

  /**********************/
  /* HF core sub-branch */
  /**********************/

  /** Core clock */
  cmuClock_CORE = (CMU_HFCORECLKDIV_REG << CMU_DIV_REG_POS) |
                  (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                  (CMU_NO_EN_REG << CMU_EN_REG_POS) |
                  (0 << CMU_EN_BIT_POS) |
                  (CMU_HFCORE_CLK_BRANCH << CMU_CLK_BRANCH_POS),

#if defined(AES_PRESENT)
  /** Advanced encryption standard accelerator clock. */
  cmuClock_AES = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_HFCORECLKEN0_EN_REG << CMU_EN_REG_POS) |
                 (_CMU_HFCORECLKEN0_AES_SHIFT << CMU_EN_BIT_POS) |
                 (CMU_HFCORE_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(DMA_PRESENT)
  /** Direct memory access controller clock. */
  cmuClock_DMA = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_HFCORECLKEN0_EN_REG << CMU_EN_REG_POS) |
                 (_CMU_HFCORECLKEN0_DMA_SHIFT << CMU_EN_BIT_POS) |
                 (CMU_HFCORE_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif
  /** Low energy clocking module clock. */
  cmuClock_CORELE = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_HFCORECLKEN0_EN_REG << CMU_EN_REG_POS) |
                    (_CMU_HFCORECLKEN0_LE_SHIFT << CMU_EN_BIT_POS) |
                    (CMU_HFCORE_CLK_BRANCH << CMU_CLK_BRANCH_POS),

#if defined(EBI_PRESENT)
  /** External bus interface clock. */
  cmuClock_EBI = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_HFCORECLKEN0_EN_REG << CMU_EN_REG_POS) |
                 (_CMU_HFCORECLKEN0_EBI_SHIFT << CMU_EN_BIT_POS) |
                 (CMU_HFCORE_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(USB_PRESENT)
  /** USB Core clock. */
  cmuClock_USBC = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                  (CMU_USBCCLKSEL_REG << CMU_SEL_REG_POS) |
                  (CMU_HFCORECLKEN0_EN_REG << CMU_EN_REG_POS) |
                  (_CMU_HFCORECLKEN0_USBC_SHIFT << CMU_EN_BIT_POS) |
                  (CMU_USBC_CLK_BRANCH << CMU_CLK_BRANCH_POS),

#endif

#if defined(USB_PRESENT)
  /** USB clock. */
  cmuClock_USB = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_HFCORECLKEN0_EN_REG << CMU_EN_REG_POS) |
                 (_CMU_HFCORECLKEN0_USB_SHIFT << CMU_EN_BIT_POS) |
                 (CMU_HFCORE_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

  /***************/
  /* LF A branch */
  /***************/

  /** Low frequency A clock */
  cmuClock_LFA = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_LFACLKSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_NO_EN_REG << CMU_EN_REG_POS) |
                 (0 << CMU_EN_BIT_POS) |
                 (CMU_LFA_CLK_BRANCH << CMU_CLK_BRANCH_POS),

#if defined(RTC_PRESENT)
  /** Real time counter clock. */
  cmuClock_RTC = (CMU_LFAPRESC0_REG << CMU_DIV_REG_POS) |
                 (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_LFACLKEN0_EN_REG << CMU_EN_REG_POS) |
                 (_CMU_LFACLKEN0_RTC_SHIFT << CMU_EN_BIT_POS) |
                 (CMU_RTC_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_LFACLKEN0_LETIMER0_MASK)
  /** Low energy timer 0 clock. */
  cmuClock_LETIMER0 = (CMU_LFAPRESC0_REG << CMU_DIV_REG_POS) |
                      (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                      (CMU_LFACLKEN0_EN_REG << CMU_EN_REG_POS) |
                      (_CMU_LFACLKEN0_LETIMER0_SHIFT << CMU_EN_BIT_POS) |
                      (CMU_LETIMER_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_LFACLKEN0_LCD_MASK)
  /** Liquid crystal display, pre FDIV clock. */
  cmuClock_LCDpre = (CMU_LFAPRESC0_REG << CMU_DIV_REG_POS) |
                    (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                    (CMU_NO_EN_REG << CMU_EN_REG_POS) |
                    (0 << CMU_EN_BIT_POS) |
                    (CMU_LCDPRE_CLK_BRANCH << CMU_CLK_BRANCH_POS),

  /** Liquid crystal display clock. Please notice that FDIV prescaler
   * must be set by special API. */
  cmuClock_LCD = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_LFACLKEN0_EN_REG << CMU_EN_REG_POS) |
                 (_CMU_LFACLKEN0_LCD_SHIFT << CMU_EN_BIT_POS) |
                 (CMU_LCD_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_PCNTCTRL_PCNT0CLKEN_MASK)
  /** Pulse counter 0 clock. */
  cmuClock_PCNT0 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                   (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                   (CMU_PCNT_EN_REG << CMU_EN_REG_POS) |
                   (_CMU_PCNTCTRL_PCNT0CLKEN_SHIFT << CMU_EN_BIT_POS) |
                   (CMU_LFA_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_PCNTCTRL_PCNT1CLKEN_MASK)
  /** Pulse counter 1 clock. */
  cmuClock_PCNT1 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                   (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                   (CMU_PCNT_EN_REG << CMU_EN_REG_POS) |
                   (_CMU_PCNTCTRL_PCNT1CLKEN_SHIFT << CMU_EN_BIT_POS) |
                   (CMU_LFA_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_PCNTCTRL_PCNT2CLKEN_MASK)
  /** Pulse counter 2 clock. */
  cmuClock_PCNT2 = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                   (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                   (CMU_PCNT_EN_REG << CMU_EN_REG_POS) |
                   (_CMU_PCNTCTRL_PCNT2CLKEN_SHIFT << CMU_EN_BIT_POS) |
                   (CMU_LFA_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif
#if defined(_CMU_LFACLKEN0_LESENSE_MASK)
  /** LESENSE clock. */
  cmuClock_LESENSE = (CMU_LFAPRESC0_REG << CMU_DIV_REG_POS) |
                     (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                     (CMU_LFACLKEN0_EN_REG << CMU_EN_REG_POS) |
                     (_CMU_LFACLKEN0_LESENSE_SHIFT << CMU_EN_BIT_POS) |
                     (CMU_LESENSE_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

  /***************/
  /* LF B branch */
  /***************/

  /** Low frequency B clock */
  cmuClock_LFB = (CMU_NODIV_REG << CMU_DIV_REG_POS) |
                 (CMU_LFBCLKSEL_REG << CMU_SEL_REG_POS) |
                 (CMU_NO_EN_REG << CMU_EN_REG_POS) |
                 (0 << CMU_EN_BIT_POS) |
                 (CMU_LFB_CLK_BRANCH << CMU_CLK_BRANCH_POS),

#if defined(_CMU_LFBCLKEN0_LEUART0_MASK)
  /** Low energy universal asynchronous receiver/transmitter 0 clock. */
  cmuClock_LEUART0 = (CMU_LFBPRESC0_REG << CMU_DIV_REG_POS) |
                     (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                     (CMU_LFBCLKEN0_EN_REG << CMU_EN_REG_POS) |
                     (_CMU_LFBCLKEN0_LEUART0_SHIFT << CMU_EN_BIT_POS) |
                     (CMU_LEUART0_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif

#if defined(_CMU_LFBCLKEN0_LEUART1_MASK)
  /** Low energy universal asynchronous receiver/transmitter 1 clock. */
  cmuClock_LEUART1 = (CMU_LFBPRESC0_REG << CMU_DIV_REG_POS) |
                     (CMU_NOSEL_REG << CMU_SEL_REG_POS) |
                     (CMU_LFBCLKEN0_EN_REG << CMU_EN_REG_POS) |
                     (_CMU_LFBCLKEN0_LEUART1_SHIFT << CMU_EN_BIT_POS) |
                     (CMU_LEUART1_CLK_BRANCH << CMU_CLK_BRANCH_POS),
#endif
} CMU_Clock_TypeDef;

typedef enum
{
  cmuOsc_LFXO,     /**< Low frequency crystal oscillator. */
  cmuOsc_LFRCO,    /**< Low frequency RC oscillator. */
  cmuOsc_HFXO,     /**< High frequency crystal oscillator. */
  cmuOsc_HFRCO,    /**< High frequency RC oscillator. */
  cmuOsc_AUXHFRCO, /**< Auxiliary high frequency RC oscillator. */
#if !defined(_EFM32_GECKO_FAMILY)
  cmuOsc_ULFRCO    /**< Ultra low frequency RC oscillator. */
#endif
} CMU_Osc_TypeDef;

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(CMU_ClockSelectSet,CMU_Clock_TypeDef , CMU_Select_TypeDef);
FAKE_VOID_FUNC(CMU_OscillatorEnable,CMU_Osc_TypeDef, bool , bool );
FAKE_VOID_FUNC(CMU_HFRCOBandSet,CMU_HFRCOBand_TypeDef);
FAKE_VOID_FUNC(CMU_ClockDivSet,CMU_Clock_TypeDef,uint32_t);
FAKE_VOID_FUNC(CMU_ClockEnable,CMU_Clock_TypeDef,bool);
FAKE_VALUE_FUNC(uint32_t,CMU_ClockFreqGet,CMU_Clock_TypeDef);
FAKE_VOID_FUNC(NVIC_DisableIRQ,IRQn_Type );

#endif /* EM_CMU_TH_HH_ */

/** ************************************************************************* */
