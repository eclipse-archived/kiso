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


#define _DMA_CTRL_SRC_INC_BYTE 0
#define _DMA_CTRL_SRC_INC_HALFWORD 1
#define _DMA_CTRL_SRC_INC_WORD 2
#define _DMA_CTRL_SRC_INC_NONE 3
/**
 * Amount source/destination address should be incremented for each data
 * transfer.
 */
typedef enum
{
  dmaDataInc1    = _DMA_CTRL_SRC_INC_BYTE,     /**< Increment address 1 byte. */
  dmaDataInc2    = _DMA_CTRL_SRC_INC_HALFWORD, /**< Increment address 2 bytes. */
  dmaDataInc4    = _DMA_CTRL_SRC_INC_WORD,     /**< Increment address 4 bytes. */
  dmaDataIncNone = _DMA_CTRL_SRC_INC_NONE      /**< Do not increment address. */
} DMA_DataInc_TypeDef;

#define _DMA_CTRL_SRC_SIZE_BYTE 0
#define _DMA_CTRL_SRC_SIZE_HALFWORD 1
#define _DMA_CTRL_SRC_SIZE_WORD 2



/** Data sizes (in number of bytes) to be read/written by DMA transfer. */
typedef enum
{
  dmaDataSize1 = _DMA_CTRL_SRC_SIZE_BYTE,     /**< 1 byte DMA transfer size. */
  dmaDataSize2 = _DMA_CTRL_SRC_SIZE_HALFWORD, /**< 2 byte DMA transfer size. */
  dmaDataSize4 = _DMA_CTRL_SRC_SIZE_WORD      /**< 4 byte DMA transfer size. */
} DMA_DataSize_TypeDef;


/** Type of DMA transfer. */
typedef enum
{
  /** Basic DMA cycle. */
  dmaCycleCtrlBasic            = 1,
  /** Auto-request DMA cycle. */
  dmaCycleCtrlAuto             = 2,
  /** Ping-pong DMA cycle. */
  dmaCycleCtrlPingPong         = 3,
  /** Memory scatter-gather DMA cycle. */
  dmaCycleCtrlMemScatterGather = 4,
  /** Peripheral scatter-gather DMA cycle. */
  dmaCycleCtrlPerScatterGather = 5
} DMA_CycleCtrl_TypeDef;


/** Number of transfers before controller does new arbitration. */
typedef enum
{
  dmaArbitrate1    = 0,    /**< Arbitrate after 1 DMA transfer. */
  dmaArbitrate2    = 1,    /**< Arbitrate after 2 DMA transfers. */
  dmaArbitrate4    = 2,    /**< Arbitrate after 4 DMA transfers. */
  dmaArbitrate8    = 3,    /**< Arbitrate after 8 DMA transfers. */
  dmaArbitrate16   = 4,   /**< Arbitrate after 16 DMA transfers. */
  dmaArbitrate32   = 5,   /**< Arbitrate after 32 DMA transfers. */
  dmaArbitrate64   = 6,   /**< Arbitrate after 64 DMA transfers. */
  dmaArbitrate128  = 7,  /**< Arbitrate after 128 DMA transfers. */
  dmaArbitrate256  = 8,  /**< Arbitrate after 256 DMA transfers. */
  dmaArbitrate512  = 9,  /**< Arbitrate after 512 DMA transfers. */
  dmaArbitrate1024 = 10  /**< Arbitrate after 1024 DMA transfers. */
} DMA_ArbiterConfig_TypeDef;

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


typedef XMD_dmaTypeDef DMA_TypeDef;

/**
 * Configuration structure for primary or alternate descriptor
 * (not used for scatter-gather DMA cycles).
 */
typedef struct
{
  /** Destination increment size for each DMA transfer */
  DMA_DataInc_TypeDef       dstInc;

  /** Source increment size for each DMA transfer */
  DMA_DataInc_TypeDef       srcInc;

  /** DMA transfer unit size. */
  DMA_DataSize_TypeDef      size;

  /**
   * Arbitration rate, ie number of DMA transfers done before rearbitration
   * takes place.
   */
  DMA_ArbiterConfig_TypeDef arbRate;

  /**
   * HPROT signal state, please refer to reference manual, DMA chapter for
   * further details. Normally set to 0 if protection is not an issue.
   * The following bits are available:
   * @li bit 0 - HPROT[1] control for source read accesses,
   *   privileged/non-privileged access
   * @li bit 3 - HPROT[1] control for destination write accesses,
   *   privileged/non-privileged access
   */
  uint8_t hprot;
} DMA_CfgDescr_TypeDef;

/**
 * @brief
 *   DMA interrupt callback function pointer.
 * @details
 *   Parameters:
 *   @li channel - The DMA channel the callback function is invoked for.
 *   @li primary - Indicates if callback is invoked for completion of primary
 *     (true) or alternate (false) descriptor. This is mainly useful for
 *     ping-pong DMA cycles, in order to know which descriptor to refresh.
 *   @li user - User definable reference that may be used to pass information
 *     to be used by the callback handler. If used, the referenced data must be
 *     valid at the point when the interrupt handler invokes the callback.
 *     If callback changes  any data in the provided user structure, remember
 *     that those changes are done in interrupt context, and proper protection
 *     of data may be required.
 */
typedef void (*DMA_FuncPtr_TypeDef)(unsigned int channel, bool primary, void *user);


/**
 * @brief
 *   Callback structure that can be used to define DMA complete actions.
 * @details
 *   A reference to this structure is only stored in the primary descriptor
 *   for a channel (if callback feature is used). If callback is required
 *   for both primary and alternate descriptor completion, this must be
 *   handled by one common callback, using the provided 'primary' parameter
 *   with the callback function.
 */
typedef struct
{
  /**
   * Pointer to callback function to invoke when DMA transfer cycle done.
   * Notice that this function is invoked in interrupt context, and therefore
   * should be short and non-blocking.
   */
  DMA_FuncPtr_TypeDef cbFunc;

  /** User defined pointer to provide with callback function. */
  void                *userPtr;

  /**
   * For internal use only: Indicates if next callback applies to primary
   * or alternate descriptor completion. Mainly useful for ping-pong DMA
   * cycles. Set this value to 0 prior to configuring callback handling.
   */
  uint8_t             primary;
} DMA_CB_TypeDef;


/** Configuration structure for a channel. */
typedef struct
{
  /**
   * Select if channel priority is in the high or default priority group
   * with respect to arbitration. Within a priority group, lower numbered
   * channels have higher priority than higher numbered channels.
   */
  bool     highPri;

  /**
   * Select if interrupt shall be enabled for channel (triggering interrupt
   * handler when dma_done signal is asserted). It should normally be
   * enabled if using the callback feature for a channel, and disabled if
   * not using the callback feature.
   */
  bool     enableInt;

  /**
   * Channel control specifying the source of DMA signals. If accessing
   * peripherals, use one of the DMAREQ_nnn defines available for the
   * peripheral. Set it to 0 for memory-to-memory DMA cycles.
   */
  uint32_t select;

  /**
   * @brief
   *   User definable callback handling configuration.
   * @details
   *   Please refer to structure definition for details. The callback
   *   is invoked when the specified DMA cycle is complete (when dma_done
   *   signal asserted). The callback is invoked in interrupt context,
   *   and should be efficient and non-blocking. Set to NULL to not
   *   use the callback feature.
   * @note
   *   The referenced structure is used by the interrupt handler, and must
   *   be available until no longer used. Thus, in most cases it should
   *   not be located on the stack.
   */
  DMA_CB_TypeDef *cb;
} DMA_CfgChannel_TypeDef;


/* mock-ups for the provided interfaces */

FAKE_VOID_FUNC( DMA_CfgDescr, uint32_t, bool, DMA_CfgDescr_TypeDef * );
FAKE_VOID_FUNC( DMA_ActivateBasic, uint32_t, bool, bool, void *, void *, uint32_t);
FAKE_VOID_FUNC( DMA_ActivatePingPong, unsigned int, bool, void *, void *, unsigned int, void *, void *, unsigned int);
FAKE_VOID_FUNC( DMA_CfgChannel, uint32_t, DMA_CfgChannel_TypeDef * );


#endif /* BCDS_EM_DMA_TH_HH_ */

/** ************************************************************************* */
