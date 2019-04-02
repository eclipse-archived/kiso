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
 * This unit test header substitutes "em_msc.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef EM_MSC_TH_HH_
#define EM_MSC_TH_HH_
/* ban the real em_msc.h interface header */
#define __EM_MSC_H

/* faked variables needs to be initialized by the test fixture */
#define MSC_UNLOCK_CODE      0x1B71                             /**< MSC unlock code */
#define MSC_WRITECTRL_WREN                      (0x1UL << 0)    /**< Enable Write/Erase Controller  */
#define MSC_WRITECMD_LADDRIM                    (0x1UL << 0)        /**< Load MSC_ADDRB into ADDR */
#define MSC_WRITECMD_WRITEONCE                  (0x1UL << 3)      /**< Word Write-Once Trigger */
#define MSC_STATUS_BUSY                         0              /**< Erase/Write Busy */
#define MSC_WRITECMD_WRITETRIG                  (0x1UL << 4)                             /**< Word Write Sequence Trigger */
#define MSC_WRITECMD_ERASEPAGE                  (0x1UL << 1)                             /**< Erase Page */
#define _MSC_MASSLOCK_LOCKKEY_UNLOCK            0x0000631AUL                          /**< Mode UNLOCK for MSC_MASSLOCK */
#define MSC_MASSLOCK_LOCKKEY_UNLOCK             (_MSC_MASSLOCK_LOCKKEY_UNLOCK << 0)   /**< Shifted mode UNLOCK for MSC_MASSLOCK */
#define _MSC_MASSLOCK_LOCKKEY_LOCK              0x00000000UL                          /**< Mode LOCK for MSC_MASSLOCK */
#define MSC_MASSLOCK_LOCKKEY_LOCK               (_MSC_MASSLOCK_LOCKKEY_LOCK << 0)     /**< Shifted mode LOCK for MSC_MASSLOCK */
#define MSC_WRITECMD_ERASEMAIN1                 (0x1UL << 9)                /**< Mass erase region 1 */
#define MSC_WRITECMD_ERASEMAIN0                 (0x1UL << 8)
#define XMD_MSC_STATUS_BUSY                         (0x1UL << 0)               /**< Erase/Write Busy */
#define MSC_STATUS_BUSY                         0                            /**< Erase/Write Busy */
#define FLASH_PAGE_SIZE                         (4096)
/*Fake functions*/

typedef enum
{
  mscReturnOk          = 0,  /**< Flash write/erase successful. */
  mscReturnInvalidAddr = -1, /**< Invalid address. Write to an address that is not flash. */
  mscReturnLocked      = -2, /**< Flash address is locked. */
  mscReturnTimeOut     = -3, /**< Timeout while writing to flash. */
  mscReturnUnaligned   = -4  /**< Unaligned access to flash. */
} msc_Return_TypeDef;

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(MSC_Init);
FAKE_VOID_FUNC(MSC_Deinit);
FAKE_VALUE_FUNC(msc_Return_TypeDef, MSC_ErasePage,uint32_t *);
FAKE_VALUE_FUNC(msc_Return_TypeDef, MSC_WriteWord,uint32_t*, void const*, int);
#endif /* EM_MSC_TH_HH_ */
