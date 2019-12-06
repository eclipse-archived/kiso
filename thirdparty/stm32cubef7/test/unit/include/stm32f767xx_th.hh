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
 *
 * @file
 *
 * Mock-up implementation for the stm32f767xx module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F767XX_TH_HH
#define STM32F767XX_TH_HH

/* Include the needed headers */
#include "gtest.h"

/* Include or ban the real interface header */
#include "stm32f767xx.h"

/* Faked variables needs to be initialized by the test fixture */
uint8_t fakeRccReg[sizeof(RCC_TypeDef)] __attribute__((unused)) = {0};

/** RCC Register */
#undef RCC
#define RCC      ((RCC_TypeDef*)&fakeRccReg)

/* Mock-ups for the provided interfaces */

#undef IS_ADC_ALL_INSTANCE
FAKE_VOID_FUNC(IS_ADC_ALL_INSTANCE, ADC_TypeDef*);

#undef IS_ADC_MULTIMODE_MASTER_INSTANCE
FAKE_VOID_FUNC(IS_ADC_MULTIMODE_MASTER_INSTANCE, ADC_TypeDef*);

#undef IS_ADC_COMMON_INSTANCE
FAKE_VOID_FUNC(IS_ADC_COMMON_INSTANCE, ADC_TypeDef*);

#undef IS_CAN_ALL_INSTANCE
FAKE_VOID_FUNC(IS_CAN_ALL_INSTANCE, CAN_TypeDef*);

#undef IS_CRC_ALL_INSTANCE
FAKE_VOID_FUNC(IS_CRC_ALL_INSTANCE, CRC_TypeDef*);

#undef IS_DAC_ALL_INSTANCE
FAKE_VOID_FUNC(IS_DAC_ALL_INSTANCE, DAC_TypeDef*);

#undef IS_DCMI_ALL_INSTANCE
FAKE_VOID_FUNC(IS_DCMI_ALL_INSTANCE, DCMI_TypeDef*);

#undef IS_DFSDM_FILTER_ALL_INSTANCE
FAKE_VOID_FUNC(IS_DFSDM_FILTER_ALL_INSTANCE, DFSDM_Filter_TypeDef*);

#undef IS_DFSDM_CHANNEL_ALL_INSTANCE
FAKE_VOID_FUNC(IS_DFSDM_CHANNEL_ALL_INSTANCE, DFSDM_Filter_TypeDef*);

#undef IS_DMA2D_ALL_INSTANCE
FAKE_VOID_FUNC(IS_DMA2D_ALL_INSTANCE, DMA2D_TypeDef*);

#undef IS_DMA_STREAM_ALL_INSTANCE
FAKE_VOID_FUNC(IS_DMA_STREAM_ALL_INSTANCE, DMA_TypeDef*);

#undef IS_GPIO_ALL_INSTANCE
FAKE_VOID_FUNC(IS_GPIO_ALL_INSTANCE, GPIO_TypeDef*);

#undef IS_GPIO_AF_INSTANCE
FAKE_VOID_FUNC(IS_GPIO_AF_INSTANCE, GPIO_TypeDef*);

#undef IS_CEC_ALL_INSTANCE
FAKE_VOID_FUNC(IS_CEC_ALL_INSTANCE, CEC_TypeDef*);

#undef IS_QSPI_ALL_INSTANCE
FAKE_VOID_FUNC(IS_QSPI_ALL_INSTANCE, QUADSPI_TypeDef*);

#undef IS_I2C_ALL_INSTANCE
FAKE_VOID_FUNC(IS_I2C_ALL_INSTANCE, I2C_TypeDef*);

#undef IS_SMBUS_ALL_INSTANCE
FAKE_VOID_FUNC(IS_SMBUS_ALL_INSTANCE, I2C_TypeDef*);

#undef IS_I2S_ALL_INSTANCE
FAKE_VOID_FUNC(IS_I2S_ALL_INSTANCE, SPI_TypeDef*);

#undef IS_LPTIM_INSTANCE
FAKE_VOID_FUNC(IS_LPTIM_INSTANCE, LPTIM_TypeDef*);

#undef IS_LTDC_ALL_INSTANCE
FAKE_VOID_FUNC(IS_LTDC_ALL_INSTANCE, LTDC_TypeDef*);

#undef IS_MDIOS_ALL_INSTANCE
FAKE_VOID_FUNC(IS_MDIOS_ALL_INSTANCE, MDIOS_TypeDef*);

#undef IS_JPEG_ALL_INSTANCE
FAKE_VOID_FUNC(IS_JPEG_ALL_INSTANCE, MDIOS_TypeDef*);

#undef IS_RNG_ALL_INSTANCE
FAKE_VOID_FUNC(IS_RNG_ALL_INSTANCE, RNG_TypeDef*);

#undef IS_RTC_ALL_INSTANCE
FAKE_VOID_FUNC(IS_RTC_ALL_INSTANCE, RTC_TypeDef*);

#undef IS_SAI_ALL_INSTANCE
FAKE_VOID_FUNC(IS_SAI_ALL_INSTANCE, SAI_Block_TypeDef*);

#undef IS_SDMMC_ALL_INSTANCE
FAKE_VOID_FUNC(IS_SDMMC_ALL_INSTANCE, SDMMC_TypeDef*);

#undef IS_SPDIFRX_ALL_INSTANCE
FAKE_VOID_FUNC(IS_SPDIFRX_ALL_INSTANCE, SPDIFRX_TypeDef*);

#undef IS_SPI_ALL_INSTANCE
FAKE_VOID_FUNC(IS_SPI_ALL_INSTANCE, SPI_TypeDef*);

#undef IS_TIM_INSTANCE
FAKE_VOID_FUNC(IS_TIM_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_32B_COUNTER_INSTANCE
FAKE_VOID_FUNC(IS_TIM_32B_COUNTER_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_BREAK_INSTANCE
FAKE_VOID_FUNC(IS_TIM_BREAK_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_BREAKSOURCE_INSTANCE
FAKE_VOID_FUNC(IS_TIM_BREAKSOURCE_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_BKIN2_INSTANCE
FAKE_VOID_FUNC(IS_TIM_BKIN2_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CC1_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CC1_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CC2_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CC2_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CC3_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CC3_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CC4_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CC4_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CC5_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CC5_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CC6_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CC6_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CCDMA_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CCDMA_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_DMA_INSTANCE
FAKE_VOID_FUNC(IS_TIM_DMA_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_DMA_CC_INSTANCE
FAKE_VOID_FUNC(IS_TIM_DMA_CC_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_DMABURST_INSTANCE
FAKE_VOID_FUNC(IS_TIM_DMABURST_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_COMBINED3PHASEPWM_INSTANCE
FAKE_VOID_FUNC(IS_TIM_COMBINED3PHASEPWM_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_COUNTER_MODE_SELECT_INSTANCE
FAKE_VOID_FUNC(IS_TIM_COUNTER_MODE_SELECT_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_ENCODER_INTERFACE_INSTANCE
FAKE_VOID_FUNC(IS_TIM_ENCODER_INTERFACE_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_OCXREF_CLEAR_INSTANCE
FAKE_VOID_FUNC(IS_TIM_OCXREF_CLEAR_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CLOCKSOURCE_TIX_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CLOCKSOURCE_TIX_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CLOCKSOURCE_ITRX_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CLOCKSOURCE_ITRX_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_ADVANCED_INSTANCE
FAKE_VOID_FUNC(IS_TIM_ADVANCED_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_XOR_INSTANCE
FAKE_VOID_FUNC(IS_TIM_XOR_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_MASTER_INSTANCE
FAKE_VOID_FUNC(IS_TIM_MASTER_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_SLAVE_INSTANCE
FAKE_VOID_FUNC(IS_TIM_SLAVE_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_ETR_INSTANCE
FAKE_VOID_FUNC(IS_TIM_ETR_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_REMAP_INSTANCE
FAKE_VOID_FUNC(IS_TIM_REMAP_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CCX_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CCX_INSTANCE, TIM_TypeDef*, uint32_t);

#undef IS_TIM_CCXN_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CCXN_INSTANCE, TIM_TypeDef*, uint32_t);

#undef IS_TIM_TRGO2_INSTANCE
FAKE_VOID_FUNC(IS_TIM_TRGO2_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_SYNCHRO_INSTANCE
FAKE_VOID_FUNC(IS_TIM_SYNCHRO_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CLOCK_DIVISION_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CLOCK_DIVISION_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_REPETITION_COUNTER_INSTANCE
FAKE_VOID_FUNC(IS_TIM_REPETITION_COUNTER_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE
FAKE_VOID_FUNC(IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE
FAKE_VOID_FUNC(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE, TIM_TypeDef*);

#undef IS_TIM_COMMUTATION_EVENT_INSTANCE
FAKE_VOID_FUNC(IS_TIM_COMMUTATION_EVENT_INSTANCE, TIM_TypeDef*);

#undef IS_USART_INSTANCE
FAKE_VOID_FUNC(IS_USART_INSTANCE, USART_TypeDef*);

#undef IS_UART_INSTANCE
FAKE_VOID_FUNC(IS_UART_INSTANCE, USART_TypeDef*);

#undef IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE
FAKE_VOID_FUNC(IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE, USART_TypeDef*);

#undef IS_UART_DRIVER_ENABLE_INSTANCE
FAKE_VOID_FUNC(IS_UART_DRIVER_ENABLE_INSTANCE, USART_TypeDef*);

#undef IS_UART_HALFDUPLEX_INSTANCE
FAKE_VOID_FUNC(IS_UART_HALFDUPLEX_INSTANCE, USART_TypeDef*);

#undef IS_UART_HWFLOW_INSTANCE
FAKE_VOID_FUNC(IS_UART_HWFLOW_INSTANCE, USART_TypeDef*);

#undef IS_UART_LIN_INSTANCE
FAKE_VOID_FUNC(IS_UART_LIN_INSTANCE, USART_TypeDef*);

#undef IS_SMARTCARD_INSTANCE
FAKE_VOID_FUNC(IS_SMARTCARD_INSTANCE, USART_TypeDef*);

#undef IS_IRDA_INSTANCE
FAKE_VOID_FUNC(IS_IRDA_INSTANCE, USART_TypeDef*);

#undef IS_IWDG_ALL_INSTANCE
FAKE_VOID_FUNC(IS_IWDG_ALL_INSTANCE, IWDG_TypeDef*);

#undef IS_WWDG_ALL_INSTANCE
FAKE_VOID_FUNC(IS_WWDG_ALL_INSTANCE, WWDG_TypeDef*);

# endif /* STM32F767XX_TH_HH */