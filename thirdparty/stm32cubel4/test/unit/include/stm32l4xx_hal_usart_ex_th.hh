/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

#ifndef STM32L4XX_HAL_USART_EX_TH_HH_
#define STM32L4XX_HAL_USART_EX_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_usart_ex.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#undef USART_GETCLOCKSOURCE
FAKE_VOID_FUNC(uint32_t, USART_GETCLOCKSOURCE, USART_HandleTypeDef *, USART_ClockSourceTypeDef)
#undef USART_MASK_COMPUTATION
FAKE_VOID_FUNC(USART_MASK_COMPUTATION, USART_HandleTypeDef *)

#endif /* STM32L4XX_HAL_USART_EX_TH_HH_ */
