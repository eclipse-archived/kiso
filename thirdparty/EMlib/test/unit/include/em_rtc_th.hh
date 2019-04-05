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
 * This unit test header substitutes "em_rtc.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */
#ifndef EM_RTC_TH_HH_
#define EM_RTC_TH_HH_

/* include system header files */

/* ban or wrap the real <INTERFACE_HEADER>_ih.h interface header */
#define __EM_RTC_H
//#include "efm32g_rtc.h"

#define RTC_PRESENT
/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

/**************************************************************************//**
 * @defgroup EFM32G_RTC
 * @{
 * @brief EFM32G_RTC Register Declaration
 *****************************************************************************/



/** RTC initialization structure. */
typedef struct
{
  bool enable;   /**< Start counting when init completed. */
  bool debugRun; /**< Counter shall keep running during debug halt. */
  bool comp0Top; /**< Use compare register 0 as max count value. */
} RTC_Init_TypeDef;

typedef struct
{
  __IO uint32_t CTRL;     /**< Control Register  */
  __I uint32_t  CNT;      /**< Counter Value Register  */
  __IO uint32_t COMP0;    /**< Compare Value Register 0  */
  __IO uint32_t COMP1;    /**< Compare Value Register 1  */
  __I uint32_t  IF;       /**< Interrupt Flag Register  */
  __IO uint32_t IFS;      /**< Interrupt Flag Set Register  */
  __IO uint32_t IFC;      /**< Interrupt Flag Clear Register  */
  __IO uint32_t IEN;      /**< Interrupt Enable Register  */

  __IO uint32_t FREEZE;   /**< Freeze Register  */
  __I uint32_t  SYNCBUSY; /**< Synchronization Busy Register  */
} RTC_TypeDef;


RTC_TypeDef RTC_baseAddress;
#define RTC_BASE          (&RTC_baseAddress) /**< RTC base address  */

#define RTC          ((RTC_TypeDef *) RTC_BASE)             /**< RTC base pointer */

/* Bit fields for RTC IFC */
#define _RTC_IFC_RESETVALUE              0x00000000UL                  /**< Default value for RTC_IFC */
#define _RTC_IFC_MASK                    0x00000007UL                  /**< Mask for RTC_IFC */
#define RTC_IFC_OF                       (0x1UL << 0)                  /**< Clear Overflow Interrupt Flag */
#define _RTC_IFC_OF_SHIFT                0                             /**< Shift value for RTC_OF */
#define _RTC_IFC_OF_MASK                 0x1UL                         /**< Bit mask for RTC_OF */
#define _RTC_IFC_OF_DEFAULT              0x00000000UL                  /**< Mode DEFAULT for RTC_IFC */
#define RTC_IFC_OF_DEFAULT               (_RTC_IFC_OF_DEFAULT << 0)    /**< Shifted mode DEFAULT for RTC_IFC */
#define RTC_IFC_COMP0                    (0x1UL << 1)                  /**< Clear Compare match 0 Interrupt Flag */
#define _RTC_IFC_COMP0_SHIFT             1                             /**< Shift value for RTC_COMP0 */
#define _RTC_IFC_COMP0_MASK              0x2UL                         /**< Bit mask for RTC_COMP0 */
#define _RTC_IFC_COMP0_DEFAULT           0x00000000UL                  /**< Mode DEFAULT for RTC_IFC */
#define RTC_IFC_COMP0_DEFAULT            (_RTC_IFC_COMP0_DEFAULT << 1) /**< Shifted mode DEFAULT for RTC_IFC */
#define RTC_IFC_COMP1                    (0x1UL << 2)                  /**< Clear Compare match 1 Interrupt Flag */
#define _RTC_IFC_COMP1_SHIFT             2                             /**< Shift value for RTC_COMP1 */
#define _RTC_IFC_COMP1_MASK              0x4UL                         /**< Bit mask for RTC_COMP1 */
#define _RTC_IFC_COMP1_DEFAULT           0x00000000UL                  /**< Mode DEFAULT for RTC_IFC */
#define RTC_IFC_COMP1_DEFAULT            (_RTC_IFC_COMP1_DEFAULT << 2) /**< Shifted mode DEFAULT for RTC_IFC */

/* Bit fields for RTC IEN */
#define _RTC_IEN_RESETVALUE              0x00000000UL                  /**< Default value for RTC_IEN */
#define _RTC_IEN_MASK                    0x00000007UL                  /**< Mask for RTC_IEN */
#define RTC_IEN_OF                       (0x1UL << 0)                  /**< Overflow Interrupt Enable */
#define _RTC_IEN_OF_SHIFT                0                             /**< Shift value for RTC_OF */
#define _RTC_IEN_OF_MASK                 0x1UL                         /**< Bit mask for RTC_OF */
#define _RTC_IEN_OF_DEFAULT              0x00000000UL                  /**< Mode DEFAULT for RTC_IEN */
#define RTC_IEN_OF_DEFAULT               (_RTC_IEN_OF_DEFAULT << 0)    /**< Shifted mode DEFAULT for RTC_IEN */
#define RTC_IEN_COMP0                    (0x1UL << 1)                  /**< Compare Match 0 Interrupt Enable */
#define _RTC_IEN_COMP0_SHIFT             1                             /**< Shift value for RTC_COMP0 */
#define _RTC_IEN_COMP0_MASK              0x2UL                         /**< Bit mask for RTC_COMP0 */
#define _RTC_IEN_COMP0_DEFAULT           0x00000000UL                  /**< Mode DEFAULT for RTC_IEN */
#define RTC_IEN_COMP0_DEFAULT            (_RTC_IEN_COMP0_DEFAULT << 1) /**< Shifted mode DEFAULT for RTC_IEN */
#define RTC_IEN_COMP1                    (0x1UL << 2)                  /**< Compare Match 1 Interrupt Enable */
#define _RTC_IEN_COMP1_SHIFT             2                             /**< Shift value for RTC_COMP1 */
#define _RTC_IEN_COMP1_MASK              0x4UL                         /**< Bit mask for RTC_COMP1 */
#define _RTC_IEN_COMP1_DEFAULT           0x00000000UL                  /**< Mode DEFAULT for RTC_IEN */
#define RTC_IEN_COMP1_DEFAULT            (_RTC_IEN_COMP1_DEFAULT << 2) /**< Shifted mode DEFAULT for RTC_IEN */


/* Bit fields for RTC IF */
#define _RTC_IF_RESETVALUE               0x00000000UL                 /**< Default value for RTC_IF */
#define _RTC_IF_MASK                     0x00000007UL                 /**< Mask for RTC_IF */
#define RTC_IF_OF                        (0x1UL << 0)                 /**< Overflow Interrupt Flag */
#define _RTC_IF_OF_SHIFT                 0                            /**< Shift value for RTC_OF */
#define _RTC_IF_OF_MASK                  0x1UL                        /**< Bit mask for RTC_OF */
#define _RTC_IF_OF_DEFAULT               0x00000000UL                 /**< Mode DEFAULT for RTC_IF */
#define RTC_IF_OF_DEFAULT                (_RTC_IF_OF_DEFAULT << 0)    /**< Shifted mode DEFAULT for RTC_IF */
#define RTC_IF_COMP0                     (0x1UL << 1)                 /**< Compare Match 0 Interrupt Flag */
#define _RTC_IF_COMP0_SHIFT              1                            /**< Shift value for RTC_COMP0 */
#define _RTC_IF_COMP0_MASK               0x2UL                        /**< Bit mask for RTC_COMP0 */
#define _RTC_IF_COMP0_DEFAULT            0x00000000UL                 /**< Mode DEFAULT for RTC_IF */
#define RTC_IF_COMP0_DEFAULT             (_RTC_IF_COMP0_DEFAULT << 1) /**< Shifted mode DEFAULT for RTC_IF */
#define RTC_IF_COMP1                     (0x1UL << 2)                 /**< Compare Match 1 Interrupt Flag */
#define _RTC_IF_COMP1_SHIFT              2                            /**< Shift value for RTC_COMP1 */
#define _RTC_IF_COMP1_MASK               0x4UL                        /**< Bit mask for RTC_COMP1 */
#define _RTC_IF_COMP1_DEFAULT            0x00000000UL                 /**< Mode DEFAULT for RTC_IF */
#define RTC_IF_COMP1_DEFAULT             (_RTC_IF_COMP1_DEFAULT << 2) /**< Shifted mode DEFAULT for RTC_IF */




FAKE_VOID_FUNC(RTC_Enable,bool);
FAKE_VOID_FUNC(RTC_CompareSet,unsigned int,uint32_t);
FAKE_VALUE_FUNC(uint32_t,RTC_CompareGet,unsigned int);
FAKE_VOID_FUNC(RTC_FreezeEnable,bool);
FAKE_VOID_FUNC(RTC_Init,const RTC_Init_TypeDef*);
FAKE_VOID_FUNC(RTC_Sync,uint32_t);
FAKE_VOID_FUNC(RTC_IntSet,uint32_t);
FAKE_VOID_FUNC(RTC_IntEnable,uint32_t);
FAKE_VOID_FUNC(RTC_IntDisable,uint32_t);
FAKE_VOID_FUNC(RTC_IntClear,uint32_t);
FAKE_VALUE_FUNC(uint32_t,RTC_CounterGet);
FAKE_VALUE_FUNC(uint32_t,RTC_IntGet);

#endif /* EM_RTC_TH_HH_ */

/** ************************************************************************* */
