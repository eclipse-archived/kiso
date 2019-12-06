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
 
#ifndef CDDK_STM32F7MEMSECTORMAP_TH_HH_
#define CDDK_STM32F7MEMSECTORMAP_TH_HH_

/* Include or ban the real interface header */
#define CDDK_STM32F7MEMSECTORMAP_H_

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

uint8_t fakeRegArray[1200] = {0};
uint8_t fakeOtpArray[100] = {0};

/* Mock-ups for the provided interfaces */
#define NB_SB_SECTOR               ((uint32_t)12)                   /* Number of available sectors. */
#define ADDR_FLASH_SB_SECTOR_0     ((uint32_t)&fakeRegArray[0])     /* Base address of Sector 0, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_1     ((uint32_t)&fakeRegArray[100])   /* Base address of Sector 1, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_2     ((uint32_t)&fakeRegArray[200])   /* Base address of Sector 2, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_3     ((uint32_t)&fakeRegArray[300])   /* Base address of Sector 3, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_4     ((uint32_t)&fakeRegArray[400])   /* Base address of Sector 4, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_5     ((uint32_t)&fakeRegArray[500])   /* Base address of Sector 5, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_6     ((uint32_t)&fakeRegArray[600])   /* Base address of Sector 6, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_7     ((uint32_t)&fakeRegArray[700])   /* Base address of Sector 7, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_8     ((uint32_t)&fakeRegArray[800])   /* Base address of Sector 8, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_9     ((uint32_t)&fakeRegArray[900])   /* Base address of Sector 9, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_10    ((uint32_t)&fakeRegArray[1000])  /* Base address of Sector 10, 100 bytes */
#define ADDR_FLASH_SB_SECTOR_11    ((uint32_t)&fakeRegArray[1100])  /* Base address of Sector 11, 100 bytes */
#define ADDR_FLASH_OTP    	   ((uint32_t)&fakeOtpArray[0])     /* Base address of OTP, 100 bytes */

#define NB_DB_SECTOR               ((uint32_t)24)                   /* Number of available sectors. */
#define ADDR_FLASH_DB_SECTOR_0     ((uint32_t)&fakeRegArray[0])     /* Base address of Sector 0, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_1     ((uint32_t)&fakeRegArray[50])    /* Base address of Sector 1, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_2     ((uint32_t)&fakeRegArray[100])   /* Base address of Sector 2, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_3     ((uint32_t)&fakeRegArray[150])   /* Base address of Sector 3, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_4     ((uint32_t)&fakeRegArray[200])   /* Base address of Sector 4, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_5     ((uint32_t)&fakeRegArray[250])   /* Base address of Sector 5, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_6     ((uint32_t)&fakeRegArray[300])   /* Base address of Sector 6, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_7     ((uint32_t)&fakeRegArray[350])   /* Base address of Sector 7, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_8     ((uint32_t)&fakeRegArray[400])   /* Base address of Sector 8, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_9     ((uint32_t)&fakeRegArray[450])   /* Base address of Sector 9, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_10    ((uint32_t)&fakeRegArray[500])   /* Base address of Sector 10, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_11    ((uint32_t)&fakeRegArray[550])   /* Base address of Sector 11, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_12    ((uint32_t)&fakeRegArray[600])   /* Base address of Sector 12, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_13    ((uint32_t)&fakeRegArray[650])   /* Base address of Sector 13, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_14    ((uint32_t)&fakeRegArray[700])   /* Base address of Sector 14, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_15    ((uint32_t)&fakeRegArray[750])   /* Base address of Sector 15, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_16    ((uint32_t)&fakeRegArray[800])   /* Base address of Sector 16, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_17    ((uint32_t)&fakeRegArray[850])   /* Base address of Sector 17, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_18    ((uint32_t)&fakeRegArray[900])   /* Base address of Sector 18, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_19    ((uint32_t)&fakeRegArray[950])   /* Base address of Sector 19, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_20    ((uint32_t)&fakeRegArray[1000])  /* Base address of Sector 20, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_21    ((uint32_t)&fakeRegArray[1050])  /* Base address of Sector 21, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_22    ((uint32_t)&fakeRegArray[1100])  /* Base address of Sector 22, 50 bytes */
#define ADDR_FLASH_DB_SECTOR_23    ((uint32_t)&fakeRegArray[1150])  /* Base address of Sector 23, 50 bytes */

#undef FLASH_BASE
#define FLASH_BASE              ADDR_FLASH_SB_SECTOR_0

#undef FLASH_OTP_BASE
#define FLASH_OTP_BASE			ADDR_FLASH_OTP

#undef FLASH_OTP_END
#define FLASH_OTP_END			ADDR_FLASH_OTP + sizeof(fakeOtpArray)

#undef FLASH_END
#define FLASH_END               FLASH_BASE + sizeof(fakeRegArray)

#define CDDK_FLASH_INTERN_SECTOR_ERROR           (uint32_t) 0xFFFFFFFF

#define IS_START_ADDR_SB_SECTOR_0(addr)     (addr == ADDR_FLASH_SB_SECTOR_0) ? 0 : CDDK_FLASH_INTERN_SECTOR_ERROR
#define IS_START_ADDR_SB_SECTOR_1(addr)     (addr == ADDR_FLASH_SB_SECTOR_1) ? 1 : IS_START_ADDR_SB_SECTOR_0(addr)
#define IS_START_ADDR_SB_SECTOR_2(addr)     (addr == ADDR_FLASH_SB_SECTOR_2) ? 2 : IS_START_ADDR_SB_SECTOR_1(addr)
#define IS_START_ADDR_SB_SECTOR_3(addr)     (addr == ADDR_FLASH_SB_SECTOR_3) ? 3 : IS_START_ADDR_SB_SECTOR_2(addr)
#define IS_START_ADDR_SB_SECTOR_4(addr)     (addr == ADDR_FLASH_SB_SECTOR_4) ? 4 : IS_START_ADDR_SB_SECTOR_3(addr)
#define IS_START_ADDR_SB_SECTOR_5(addr)     (addr == ADDR_FLASH_SB_SECTOR_5) ? 5 : IS_START_ADDR_SB_SECTOR_4(addr)
#define IS_START_ADDR_SB_SECTOR_6(addr)     (addr == ADDR_FLASH_SB_SECTOR_6) ? 6 : IS_START_ADDR_SB_SECTOR_5(addr)
#define IS_START_ADDR_SB_SECTOR_7(addr)     (addr == ADDR_FLASH_SB_SECTOR_7) ? 7 : IS_START_ADDR_SB_SECTOR_6(addr)
#define IS_START_ADDR_SB_SECTOR_8(addr)     (addr == ADDR_FLASH_SB_SECTOR_8) ? 8 : IS_START_ADDR_SB_SECTOR_7(addr)
#define IS_START_ADDR_SB_SECTOR_9(addr)     (addr == ADDR_FLASH_SB_SECTOR_9) ? 9 : IS_START_ADDR_SB_SECTOR_8(addr)
#define IS_START_ADDR_SB_SECTOR_10(addr)    (addr == ADDR_FLASH_SB_SECTOR_10) ? 10 : IS_START_ADDR_SB_SECTOR_9(addr)
#define IS_START_ADDR_SB_SECTOR_11(addr)    (addr == ADDR_FLASH_SB_SECTOR_11) ? 11 : IS_START_ADDR_SB_SECTOR_10(addr)

#define IS_START_ADDR_DB_SECTOR_0(addr)     (addr == ADDR_FLASH_DB_SECTOR_0) ? 0 : CDDK_FLASH_INTERN_SECTOR_ERROR
#define IS_START_ADDR_DB_SECTOR_1(addr)     (addr == ADDR_FLASH_DB_SECTOR_1) ? 1 : IS_START_ADDR_DB_SECTOR_0(addr)
#define IS_START_ADDR_DB_SECTOR_2(addr)     (addr == ADDR_FLASH_DB_SECTOR_2) ? 2 : IS_START_ADDR_DB_SECTOR_1(addr)
#define IS_START_ADDR_DB_SECTOR_3(addr)     (addr == ADDR_FLASH_DB_SECTOR_3) ? 3 : IS_START_ADDR_DB_SECTOR_2(addr)
#define IS_START_ADDR_DB_SECTOR_4(addr)     (addr == ADDR_FLASH_DB_SECTOR_4) ? 4 : IS_START_ADDR_DB_SECTOR_3(addr)
#define IS_START_ADDR_DB_SECTOR_5(addr)     (addr == ADDR_FLASH_DB_SECTOR_5) ? 5 : IS_START_ADDR_DB_SECTOR_4(addr)
#define IS_START_ADDR_DB_SECTOR_6(addr)     (addr == ADDR_FLASH_DB_SECTOR_6) ? 6 : IS_START_ADDR_DB_SECTOR_5(addr)
#define IS_START_ADDR_DB_SECTOR_7(addr)     (addr == ADDR_FLASH_DB_SECTOR_7) ? 7 : IS_START_ADDR_DB_SECTOR_6(addr)
#define IS_START_ADDR_DB_SECTOR_8(addr)     (addr == ADDR_FLASH_DB_SECTOR_8) ? 8 : IS_START_ADDR_DB_SECTOR_7(addr)
#define IS_START_ADDR_DB_SECTOR_9(addr)     (addr == ADDR_FLASH_DB_SECTOR_9) ? 9 : IS_START_ADDR_DB_SECTOR_8(addr)
#define IS_START_ADDR_DB_SECTOR_10(addr)    (addr == ADDR_FLASH_DB_SECTOR_10) ? 10 : IS_START_ADDR_DB_SECTOR_9(addr)
#define IS_START_ADDR_DB_SECTOR_11(addr)    (addr == ADDR_FLASH_DB_SECTOR_11) ? 11 : IS_START_ADDR_DB_SECTOR_10(addr)
#define IS_START_ADDR_DB_SECTOR_12(addr)    (addr == ADDR_FLASH_DB_SECTOR_12) ? 12 : IS_START_ADDR_DB_SECTOR_11(addr)
#define IS_START_ADDR_DB_SECTOR_13(addr)    (addr == ADDR_FLASH_DB_SECTOR_13) ? 13 : IS_START_ADDR_DB_SECTOR_12(addr)
#define IS_START_ADDR_DB_SECTOR_14(addr)    (addr == ADDR_FLASH_DB_SECTOR_14) ? 14 : IS_START_ADDR_DB_SECTOR_13(addr)
#define IS_START_ADDR_DB_SECTOR_15(addr)    (addr == ADDR_FLASH_DB_SECTOR_15) ? 15 : IS_START_ADDR_DB_SECTOR_14(addr)
#define IS_START_ADDR_DB_SECTOR_16(addr)    (addr == ADDR_FLASH_DB_SECTOR_16) ? 16 : IS_START_ADDR_DB_SECTOR_15(addr)
#define IS_START_ADDR_DB_SECTOR_17(addr)    (addr == ADDR_FLASH_DB_SECTOR_17) ? 17 : IS_START_ADDR_DB_SECTOR_16(addr)
#define IS_START_ADDR_DB_SECTOR_18(addr)    (addr == ADDR_FLASH_DB_SECTOR_18) ? 18 : IS_START_ADDR_DB_SECTOR_17(addr)
#define IS_START_ADDR_DB_SECTOR_19(addr)    (addr == ADDR_FLASH_DB_SECTOR_19) ? 19 : IS_START_ADDR_DB_SECTOR_18(addr)
#define IS_START_ADDR_DB_SECTOR_20(addr)    (addr == ADDR_FLASH_DB_SECTOR_20) ? 20 : IS_START_ADDR_DB_SECTOR_19(addr)
#define IS_START_ADDR_DB_SECTOR_21(addr)    (addr == ADDR_FLASH_DB_SECTOR_21) ? 21 : IS_START_ADDR_DB_SECTOR_20(addr)
#define IS_START_ADDR_DB_SECTOR_22(addr)    (addr == ADDR_FLASH_DB_SECTOR_22) ? 22 : IS_START_ADDR_DB_SECTOR_21(addr)
#define IS_START_ADDR_DB_SECTOR_23(addr)    (addr == ADDR_FLASH_DB_SECTOR_23) ? 23 : IS_START_ADDR_DB_SECTOR_22(addr)

#define GET_SB_START_SECTOR(addr)      IS_START_ADDR_SB_SECTOR_11(addr)
#define GET_SB_END_SECTOR(addr)        (addr == FLASH_END) ? NB_SB_SECTOR : IS_START_ADDR_SB_SECTOR_11((addr + 1))

#define GET_DB_START_SECTOR(addr)      IS_START_ADDR_DB_SECTOR_23(addr)
#define GET_DB_END_SECTOR(addr)        (addr == FLASH_END) ? NB_DB_SECTOR : IS_START_ADDR_DB_SECTOR_23((addr + 1))

#endif /* CDDK_STM32F7MEMSECTORMAP_TH_HH_ */
