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
 * This unit test header substitutes "em_dma.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef BCDS_EM_DMA_TH_HH_
#define BCDS_EM_DMA_TH_HH_

/* include system header files */

#if defined(DMA_HOST)
#error "The test header specifies to provide the DMA operation mode related interfaces"
#endif
/* ban the real em_dma.h interface header */
#define __EM_DMA_H

/* faked variables needs to be initialized by the test fixture */
#define DMA_CONFIG_EN                                   (0x1UL << 0)   /**< Enable DMA */
#define   __I     volatile                                      /*!< Defines 'read only' permissions                 */
#define     __IO    volatile                                    /*!< Defines 'read / write' permissions              */
#define     __O     volatile                                    /*!< Defines 'write only' permissions                */
#define DMA_CHENS_CH0ENS                                0      /**< Channel 0 Enable Set */
#define DMA_CTRL_DST_INC_NONE                          0xC0000000UL  /**< No increment */
#define DMA_CTRL_DST_SIZE_WORD                         0x20000000UL  /**< Word/32-bit data size */
#define DMA_CTRL_SRC_INC_WORD                          0x08000000UL  /**< Word/32-bit increment */
#define DMA_CTRL_SRC_SIZE_WORD                         0x02000000UL  /**< Word/32-bit data size */
#define DMA_CTRL_R_POWER_1                             0x00000000UL  /**< Arbitrate after each transfer */
#define DMA_CTRL_CYCLE_CTRL_BASIC                      0x00000001UL  /**< Basic cycle type */
#define _DMA_CONFIG_RESETVALUE                         0x00000000UL
#define _DMA_IEN_RESETVALUE                            0x00000000UL
#define _DMA_CTRL_N_MINUS_1_SHIFT                      4             /**< Number of DMA transfers minus 1, shift mask. See PL230 documentation */
#define _DMA_CH_CTRL_SOURCESEL_MSC                      0x00000030UL                                  /**< Mode MSC for DMA_CH_CTRL */
#define DMA_CH_CTRL_SOURCESEL_MSC                       (_DMA_CH_CTRL_SOURCESEL_MSC << 16)            /**< Shifted mode MSC for DMA_CH_CTRL */
#define _DMA_CH_CTRL_SIGSEL_MSCWDATA                    0x00000000UL                                  /**< Mode MSCWDATA for DMA_CH_CTRL */
#define DMA_CH_CTRL_SIGSEL_MSCWDATA                     (_DMA_CH_CTRL_SIGSEL_MSCWDATA << 0)           /**< Shifted mode MSCWDATA for DMA_CH_CTRL */
#define XMD_DMA_BASE          (0x400C2000UL)                                    /**< DMA base address  */
#define DMA_CHENS_CH0ENS                                0                   /**< Channel 0 Enable Set */
typedef struct
{
    __IO uint32_t CTRL; /**< Channel Control Register  */
} XMD_dmaTypeDef_t;
typedef struct
{
    __I uint32_t STATUS; /**< DMA Status Registers  */
    __O uint32_t CONFIG; /**< DMA Configuration Register  */
    __IO uint32_t CTRLBASE; /**< Channel Control Data Base Pointer Register  */
    __I uint32_t ALTCTRLBASE; /**< Channel Alternate Control Data Base Pointer Register  */
    __I uint32_t CHWAITSTATUS; /**< Channel Wait on Request Status Register  */
    __O uint32_t CHSWREQ; /**< Channel Software Request Register  */
    __IO uint32_t CHUSEBURSTS; /**< Channel Useburst Set Register  */
    __O uint32_t CHUSEBURSTC; /**< Channel Useburst Clear Register  */
    __IO uint32_t CHREQMASKS; /**< Channel Request Mask Set Register  */
    __O uint32_t CHREQMASKC; /**< Channel Request Mask Clear Register  */
    __IO uint32_t CHENS; /**< Channel Enable Set Register  */
    __O uint32_t CHENC; /**< Channel Enable Clear Register  */
    __IO uint32_t CHALTS; /**< Channel Alternate Set Register  */
    __O uint32_t CHALTC; /**< Channel Alternate Clear Register  */
    __IO uint32_t CHPRIS; /**< Channel Priority Set Register  */
    __O uint32_t CHPRIC; /**< Channel Priority Clear Register  */
    uint32_t RESERVED0[3]; /**< Reserved for future use **/
    __IO uint32_t ERRORC; /**< Bus Error Clear Register  */

    uint32_t RESERVED1[880]; /**< Reserved for future use **/
    __I uint32_t CHREQSTATUS; /**< Channel Request Status  */
    uint32_t RESERVED2[1]; /**< Reserved for future use **/
    __I uint32_t CHSREQSTATUS; /**< Channel Single Request Status  */

    uint32_t RESERVED3[121]; /**< Reserved for future use **/
    __I uint32_t IF; /**< Interrupt Flag Register  */
    __IO uint32_t IFS; /**< Interrupt Flag Set Register  */
    __IO uint32_t IFC; /**< Interrupt Flag Clear Register  */
    __IO uint32_t IEN; /**< Interrupt Enable register  */

    uint32_t RESERVED4[60]; /**< Reserved registers */

    XMD_dmaTypeDef_t CH[8]; /**< Channel registers */
} XMD_dmaTypeDef;
#undef DMA
XMD_dmaTypeDef *DMA = (XMD_dmaTypeDef*) malloc(sizeof(XMD_dmaTypeDef));
typedef struct
{
    __IO uint32_t CTRL; /**< Memory System Control Register  */
    __IO uint32_t READCTRL; /**< Read Control Register  */
    __IO uint32_t WRITECTRL; /**< Write Control Register  */
    __IO uint32_t WRITECMD; /**< Write Command Register  */
    __IO uint32_t ADDRB; /**< Page Erase/Write Address Buffer  */

    uint32_t RESERVED0[1]; /**< Reserved for future use **/
    __IO uint32_t WDATA; /**< Write Data Register  */
    __I uint32_t STATUS; /**< Status Register  */

    uint32_t RESERVED1[3]; /**< Reserved for future use **/
    __I uint32_t IF; /**< Interrupt Flag Register  */
    __IO uint32_t IFS; /**< Interrupt Flag Set Register  */
    __IO uint32_t IFC; /**< Interrupt Flag Clear Register  */
    __IO uint32_t IEN; /**< Interrupt Enable Register  */
    __IO uint32_t LOCK; /**< Configuration Lock Register  */
    __IO uint32_t MASSLOCK;     /**< Mass Erase Lock Register  */
} XMD_mscTypeDef; /** @} */
XMD_mscTypeDef *MSC = (XMD_mscTypeDef*) malloc(sizeof(XMD_mscTypeDef));
typedef struct
{
  /* Note! Use of double __IO (volatile) qualifier to ensure that both */
  /* pointer and referenced memory are declared volatile. */
  __IO void * __IO SRCEND;     /**< DMA source address end */
  __IO void * __IO DSTEND;     /**< DMA destination address end */
  __IO uint32_t    CTRL;       /**< DMA control register */
  __IO uint32_t    USER;       /**< DMA padding register, available for user */
} DMA_DESCRIPTOR_TypeDef;      /** @} */
/* mock-ups for the provided interfaces */

#endif /* BCDS_EM_DMA_TH_HH_ */

/** ************************************************************************* */
