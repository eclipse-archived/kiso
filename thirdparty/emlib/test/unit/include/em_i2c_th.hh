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
 * This unit test header substitutes "em_leuart.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */
#ifndef _EM_I2C_TH_HH_
#define _EM_I2C_TH_HH_

/* include system header files */
#include "em_dma.h"                        // declares DMA_HandleTypeDef of HAL_handler

#include "em_i2c.h"

/* faked variables needs to be initialized by the test fixture */
#define I2C_TEST_BASE        (0x12345) /**< UART0 base address  */

#define I2C_TEST        ((I2C_TypeDef *) I2C_TEST_BASE)         /**< UART0 base pointer */



FAKE_VALUE_FUNC(uint32_t, I2C_BusFreqGet, I2C_TypeDef *);

FAKE_VALUE_FUNC(I2C_TransferReturn_TypeDef, I2C_Transfer, I2C_TypeDef *);
FAKE_VALUE_FUNC(I2C_TransferReturn_TypeDef, I2C_TransferInit, I2C_TypeDef *,I2C_TransferSeq_TypeDef *);


FAKE_VOID_FUNC(I2C_BusFreqSet, I2C_TypeDef *, uint32_t, uint32_t, I2C_ClockHLR_TypeDef);
FAKE_VOID_FUNC(I2C_Enable, I2C_TypeDef *, bool);
FAKE_VOID_FUNC(I2C_Init, I2C_TypeDef *, const I2C_Init_TypeDef *);
FAKE_VOID_FUNC(I2C_Reset, I2C_TypeDef*);




#endif /* BCDS_<FILE_BASE_NAME>_HH_*/


