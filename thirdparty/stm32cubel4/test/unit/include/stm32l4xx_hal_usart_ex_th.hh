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

#ifndef STM32L4XX_HAL_USART_EX_TH_HH_
#define STM32L4XX_HAL_USART_EX_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_usart_ex.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#undef USART_GETCLOCKSOURCE
FAKE_VOID_FUNC(uint32_t, USART_GETCLOCKSOURCE, USART_HandleTypeDef*, USART_ClockSourceTypeDef);
#undef USART_MASK_COMPUTATION
FAKE_VOID_FUNC(USART_MASK_COMPUTATION, USART_HandleTypeDef*);

#endif /* STM32L4XX_HAL_USART_EX_TH_HH_ */
