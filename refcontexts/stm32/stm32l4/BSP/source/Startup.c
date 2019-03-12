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
 * @ingroup BSP_STARTUP
 * @{
 *
 * @file
 * @brief C start-up code for STM32L486xx devices
 * @details The implementation is based on startup_stm32l486xx.s V1.1.2
 * (12-September-2016) and is compatible with the GCC 'GNU Tools ARM Embedded'
 * toolchain. The module performs the following initialization steps:
 *   - Set initial value of program counter to Reset_Handler
 *   - Set initial stack pointer to highest address of the user mode stack
 *   - Set the vector table entries with the exceptions ISR address
 *   - Configure the clock system
 *   - Branch to main
 *
 * After Reset the Cortex-M4 processor is in Thread mode, priority is
 * Privileged, and the Stack is set to System.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_IRQHANDLER

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_Essentials.h"

#if BCDS_FEATURE_BSP_STARTUP

#include "BoardSettings.h"
#include "BT_em9301.h"
#include "Button.h"
#include "GNSS_g7020.h"
#include "GSM_sim800h.h"
#include "Memory_w25q256fv.h"
#include "SensorNode.h"
#include "LED_lm2755.h"
#include "TestInterface.h"
#include "AnalogSensor.h"
#include "SystemClock.h"
#include "Charger_bq2407x.h"

/* Put the type and macro definitions here */

/* The symbol is defined in the template, but is not referenced. */
#define BootRAM UINT32_C(0xF1E0F85F)

/* Local compilation time configuration settings */
#define PROVIDE_NMI_HANDLER                                                     1
#define PROVIDE_HARDFAULT_HANDLER                                               0

/**
 * @brief Function prototype for interrupt service routine handlers
 */
typedef void (*pFunc)(void);

/**
 * Start address for the initialization values of the .data section (linker defined symbol)
 */
extern uint32_t _sidata[];

/**
 * Start address for the .data section (linker defined symbol)
 */
extern uint32_t _sdata[];

/**
 * End address for the .data section (linker defined symbol)
 */
extern uint32_t _edata[];

/**
 * Start address for the .bss section (linker defined symbol)
 */
extern uint32_t _sbss[];

/**
 * End address for the .bss section (linker defined symbol)
 */
extern uint32_t _ebss[];

/**
 * Top address of the user mode stack (linker defined symbol)
 */
extern uint32_t _estack[];

/*
 * Function is defined by the standard C library
 */
extern void __libc_init_array(void);

#ifndef __NO_SYSTEM_INIT
extern void SystemInit(void); /* CMSIS System Initialization      */
#endif /* __NO_SYSTEM_INIT */

/*
 * Function is defined by the application according to ISO C99 requirements.
 */
extern int main(void);

/* Prototypes for interrupt service routines referenced by the vector table */

/**
 * @brief This is the code that gets called when the processor receives an
 * unexpected interrupt. This simply enters an infinite loop, preserving
 * the system state for examination by a debugger unless hardware watchdog
 * resets the MCU.
 */
void Default_Handler(void);

/**
 * @brief This is the program entry point, the first function that gets called
 * when the processor first starts execution following a reset event. Only the
 * absolutely necessary set is performed, after which the application supplied
 * main() routine is called.
 */
void Reset_Handler(void);

#if (PROVIDE_NMI_HANDLER)
/**
 * @brief Non-Maskable Interrupt (NMI) exception handler
 * @details The MCU generates NMI exceptions on various unrecoverable hardware
 * errors. The following list provides a non-conclusive list of such error
 * sources:
 * - SRAM2 parity error (only if parity check feature is enabled)
 * - FLASH ECC double bit error
 * - HSE clock loss (only if clock security system is enabled)
 */
void NMI_Handler(void);
#else /* if (PROVIDE_NMI_HANDLER) */
void NMI_Handler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (PROVIDE_NMI_HANDLER) */

#if (PROVIDE_HARDFAULT_HANDLER)
void HardFault_Handler(void) __attribute__ ((alias("Default_Handler")));
#else /* if (PROVIDE_HARDFAULT_HANDLER) */
void HardFault_Handler(void);
#endif /* if (PROVIDE_HARDFAULT_HANDLER) */

void MemManage_Handler(void) __attribute__ ((alias("Default_Handler")));

void BusFault_Handler(void) __attribute__ ((alias("Default_Handler")));

void UsageFault_Handler(void) __attribute__ ((alias("Default_Handler")));

void DebugMon_Handler(void) __attribute__ ((alias("Default_Handler")));

#if (BCDS_FEATURE_BSP_RTOS_IN_USE)
void SVC_Handler(void);
void PendSV_Handler(void);
#else /* if (BCDS_FEATURE_BSP_RTOS_IN_USE) */
void SVC_Handler(void) __attribute__ ((alias("Default_Handler")));
void PendSV_Handler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_RTOS_IN_USE) */


#if (BCDS_FEATURE_HAL_TICK_HANDLER)
#define SysTick_Handler SystemClock_ISR
#else /* if (BCDS_FEATURE_HAL_TICK_HANDLER) */
void SysTick_Handler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_HAL_TICK_HANDLER) */

void WWDG_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void PVD_PVM_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TAMP_STAMP_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void RTC_WKUP_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void FLASH_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void RCC_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void EXTI0_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

#if (BCDS_FEATURE_BSP_BUTTON)
#define EXTI1_IRQHandler Button_EXTI1ISR
#else /* if (BCDS_FEATURE_BSP_BUTTON) */
void EXTI1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_BUTTON) */

void EXTI2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

#if (BCDS_FEATURE_BSP_SENSOR_NODE)
#define EXTI3_IRQHandler SensorNode_EXTI3ISR
#else /* if (BCDS_FEATURE_BSP_SENSOR_NODE) */
void EXTI3_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_SENSOR_NODE) */

void EXTI4_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

#if (BSP_FEATURE_ANALOG_SENSOR)
#define DMA1_Channel1_IRQHandler BSP_AnalogSensor_ConversionCompleteISR
#else /* if (BSP_FEATURE_ANALOG_SENSOR) */
void DMA1_Channel1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BSP_FEATURE_ANALOG_SENSOR) */

#if (BCDS_FEATURE_BSP_MEMORY_W25Q256FV)
#define DMA1_Channel2_IRQHandler Mem_W25Q256FV_SPI_DMARxISR
#else /* if (BCDS_FEATURE_BSP_MEMORY_W25Q256FV) */
void DMA1_Channel2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_MEMORY_W25Q256FV) */

#if (BCDS_FEATURE_BSP_MEMORY_W25Q256FV)
#define DMA1_Channel3_IRQHandler Mem_W25Q256FV_SPI_DMATxISR
#else /* if (BCDS_FEATURE_BSP_MEMORY_W25Q256FV) */
void DMA1_Channel3_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_MEMORY_W25Q256FV) */

void DMA1_Channel4_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DMA1_Channel5_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DMA1_Channel6_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DMA1_Channel7_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void ADC1_2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

/** @todo 2016-12-16, BCDS/ENG1: Add vector header that declares the CAN ISR prototypes instead of indirect declaration in source code */
#if (BCDS_FEATURE_BSP_VECTOR_CAN)
void CanIsrTx_0(void);
#define CAN1_TX_IRQHandler CanIsrTx_0
#elif (BCDS_FEATURE_BSP_CUSTOM_CAN) /* if (BCDS_FEATURE_BSP_VECTOR_CAN) */
void CAN1_TX_IRQHandler(void);
#else /* if (BCDS_FEATURE_BSP_VECTOR_CAN) */
void CAN1_TX_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_VECTOR_CAN) */

void CAN1_RX0_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

#if (BCDS_FEATURE_BSP_VECTOR_CAN)
void CanIsrRx1_0(void);
void CanIsrStatus_0(void);
#define CAN1_RX1_IRQHandler CanIsrRx1_0
#define CAN1_SCE_IRQHandler CanIsrStatus_0
#elif (BCDS_FEATURE_BSP_CUSTOM_CAN) /* if (BCDS_FEATURE_BSP_VECTOR_CAN) */
void CAN1_RX1_IRQHandler(void);
void CAN1_SCE_IRQHandler(void);
#else /* if (BCDS_FEATURE_BSP_VECTOR_CAN) */
void CAN1_RX1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_VECTOR_CAN) */

#if (BCDS_FEATURE_BSP_CHARGER_BQ2407X)
#define EXTI9_5_IRQHandler Charging_Stat_EXTI7ISR
#else /* if (BCDS_FEATURE_BSP_CHARGER_BQ2407X) */
void EXTI9_5_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_CHARGER_BQ2407X) */

void TIM1_BRK_TIM15_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM1_UP_TIM16_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM1_TRG_COM_TIM17_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM1_CC_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

#if (BCDS_FEATURE_BSP_BUTTON)
#define TIM3_IRQHandler Button_TIM3ISR
#else /* if (BCDS_FEATURE_BSP_BUTTON) */
void TIM3_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_BUTTON) */

#if (BCDS_FEATURE_BSP_CHARGER_BQ2407X)
#define TIM4_IRQHandler ChargerTIM_ISR
#else /* if (BCDS_FEATURE_BSP_CHARGER_BQ2407X) */
void TIM4_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_CHARGER_BQ2407X) */

#if (BCDS_FEATURE_BSP_SENSOR_NODE)
#define I2C1_EV_IRQHandler SensorNode_I2CEvISR
#else /* if (BCDS_FEATURE_BSP_SENSOR_NODE) */
void I2C1_EV_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_SENSOR_NODE) */

#if (BCDS_FEATURE_BSP_SENSOR_NODE)
#define I2C1_ER_IRQHandler SensorNode_I2CErISR
#else /* if (BCDS_FEATURE_BSP_SENSOR_NODE) */
void I2C1_ER_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_SENSOR_NODE) */

#if (BCDS_FEATURE_BSP_LED_LM2755)
#define I2C2_EV_IRQHandler LED_LM2755_I2CEvISR
#else /* if (BCDS_FEATURE_BSP_LED_LM2755) */
void I2C2_EV_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_LED_LM2755) */

#if (BCDS_FEATURE_BSP_LED_LM2755)
#define I2C2_ER_IRQHandler LED_LM2755_I2CErISR
#else /* if (BCDS_FEATURE_BSP_LED_LM2755) */
void I2C2_ER_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_LED_LM2755) */

void SPI1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void SPI2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

#if (BCDS_FEATURE_BSP_BT_EM9301)
#define USART1_IRQHandler BT_EM9301_UARTISR
#else /* if (BCDS_FEATURE_BSP_BT_EM9301) */
void USART1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_BT_EM9301) */

#if (BCDS_FEATURE_BSP_GSM_SIM800H)
#define USART2_IRQHandler GSM_SIM800H_UARTISR
#else /* if (BCDS_FEATURE_BSP_GSM_SIM800H) */
void USART2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_GSM_SIM800H) */

#if BCDS_FEATURE_BSP_TEST_INTERFACE
#define USART3_IRQHandler TestInterface_UARTISR
#else /* if BCDS_FEATURE_BSP_TEST_INTERFACE */
void USART3_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if BCDS_FEATURE_BSP_TEST_INTERFACE */

#if (BCDS_FEATURE_BSP_SENSOR_NODE)
#define EXTI15_10_IRQHandler SensorNode_EXTI15_10ISR
#else /* if (BCDS_FEATURE_BSP_SENSOR_NODE) */
void EXTI15_10_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_SENSOR_NODE) */

void RTC_Alarm_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DFSDM1_FLT3_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM8_BRK_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM8_UP_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM8_TRG_COM_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM8_CC_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void ADC3_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void FMC_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void SDMMC1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM5_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void SPI3_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void UART4_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void UART5_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM6_DAC_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TIM7_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DMA2_Channel1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DMA2_Channel2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DMA2_Channel3_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DMA2_Channel4_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DMA2_Channel5_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DFSDM1_FLT0_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DFSDM1_FLT1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void DFSDM1_FLT2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void COMP_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void LPTIM1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void LPTIM2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void OTG_FS_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

#if (BCDS_FEATURE_BSP_BT_EM9301)
#define DMA2_Channel6_IRQHandler BT_EM9301_UARTTxIsr
#else /* if (BCDS_FEATURE_BSP_BT_EM9301) */
void DMA2_Channel6_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_BT_EM9301) */

#if (BCDS_FEATURE_BSP_BT_EM9301)
#define DMA2_Channel7_IRQHandler BT_EM9301_UARTRxIsr
#else /* if (BCDS_FEATURE_BSP_BT_EM9301) */
void DMA2_Channel7_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_BT_EM9301) */

#if (BCDS_FEATURE_BSP_GNSS_G7020)
#define LPUART1_IRQHandler GNSS_G7020_UARTISR
#else /* if (BCDS_FEATURE_BSP_GNSS_G7020) */
void LPUART1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));
#endif /* if (BCDS_FEATURE_BSP_GNSS_G7020) */

void QUADSPI_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void I2C3_EV_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void I2C3_ER_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void SAI1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void SAI2_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void SWPMI1_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void TSC_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void LCD_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void AES_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void RNG_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

void FPU_IRQHandler(void) __attribute__ ((alias("Default_Handler")));

/* Put constant and variable definitions here */

/**
 * @brief MCU Vector table allocated into the .isr_vector segment
 */
const pFunc g_pfnVectors[] __attribute__ ((section(".isr_vector"))) =
        {
                /* Cortex-M Exception Handlers */

                /*lint -e(611) > 2016-11-25, BCDS/ENG1: In this use case
                 the explicit cast is required and it is not possible to set a
                 non literal type for a linker defined symbol. Strict ISO C99
                 compatibility is not targeted. */
                (pFunc) _estack,
                Reset_Handler,
                NMI_Handler,
                HardFault_Handler,
                MemManage_Handler,
                BusFault_Handler,
                UsageFault_Handler,
                NULL, /* Reserved */
                NULL, /* Reserved */
                NULL, /* Reserved */
                NULL, /* Reserved */
                SVC_Handler,
                DebugMon_Handler,
                NULL, /* Reserved */
                PendSV_Handler,
                SysTick_Handler,

                /* External interrupts */
                WWDG_IRQHandler,
                PVD_PVM_IRQHandler,
                TAMP_STAMP_IRQHandler,
                RTC_WKUP_IRQHandler,
                FLASH_IRQHandler,
                RCC_IRQHandler,
                EXTI0_IRQHandler,
                EXTI1_IRQHandler,
                EXTI2_IRQHandler,
                EXTI3_IRQHandler,
                EXTI4_IRQHandler,
                DMA1_Channel1_IRQHandler,
                DMA1_Channel2_IRQHandler,
                DMA1_Channel3_IRQHandler,
                DMA1_Channel4_IRQHandler,
                DMA1_Channel5_IRQHandler,
                DMA1_Channel6_IRQHandler,
                DMA1_Channel7_IRQHandler,
                ADC1_2_IRQHandler,
                CAN1_TX_IRQHandler,
                CAN1_RX0_IRQHandler,
                CAN1_RX1_IRQHandler,
                CAN1_SCE_IRQHandler,
                EXTI9_5_IRQHandler,
                TIM1_BRK_TIM15_IRQHandler,
                TIM1_UP_TIM16_IRQHandler,
                TIM1_TRG_COM_TIM17_IRQHandler,
                TIM1_CC_IRQHandler,
                TIM2_IRQHandler,
                TIM3_IRQHandler,
                TIM4_IRQHandler,
                I2C1_EV_IRQHandler,
                I2C1_ER_IRQHandler,
                I2C2_EV_IRQHandler,
                I2C2_ER_IRQHandler,
                SPI1_IRQHandler,
                SPI2_IRQHandler,
                USART1_IRQHandler,
                USART2_IRQHandler,
                USART3_IRQHandler,
                EXTI15_10_IRQHandler,
                RTC_Alarm_IRQHandler,
                DFSDM1_FLT3_IRQHandler,
                TIM8_BRK_IRQHandler,
                TIM8_UP_IRQHandler,
                TIM8_TRG_COM_IRQHandler,
                TIM8_CC_IRQHandler,
                ADC3_IRQHandler,
                FMC_IRQHandler,
                SDMMC1_IRQHandler,
                TIM5_IRQHandler,
                SPI3_IRQHandler,
                UART4_IRQHandler,
                UART5_IRQHandler,
                TIM6_DAC_IRQHandler,
                TIM7_IRQHandler,
                DMA2_Channel1_IRQHandler,
                DMA2_Channel2_IRQHandler,
                DMA2_Channel3_IRQHandler,
                DMA2_Channel4_IRQHandler,
                DMA2_Channel5_IRQHandler,
                DFSDM1_FLT0_IRQHandler,
                DFSDM1_FLT1_IRQHandler,
                DFSDM1_FLT2_IRQHandler,
                COMP_IRQHandler,
                LPTIM1_IRQHandler,
                LPTIM2_IRQHandler,
                OTG_FS_IRQHandler,
                DMA2_Channel6_IRQHandler,
                DMA2_Channel7_IRQHandler,
                LPUART1_IRQHandler,
                QUADSPI_IRQHandler,
                I2C3_EV_IRQHandler,
                I2C3_ER_IRQHandler,
                SAI1_IRQHandler,
                SAI2_IRQHandler,
                SWPMI1_IRQHandler,
                TSC_IRQHandler,
                LCD_IRQHandler,
                AES_IRQHandler,
                RNG_IRQHandler,
                FPU_IRQHandler
        };

/* Put private function declarations here */

/* Put function implementations here */

void Reset_Handler(void)
{
    /** @todo 2016-11-28, BCDS/ENG1: Explicitly set-up stack pointer for corner case
     *        when Reset_handler() is called by software other than the bootloader
     *        which reads and configures main SP before branching
     */
    uint32_t *pSrc, *pDest;

    /* Copy data segment initializers from flash to RAM */
    for (pSrc = _sidata, pDest = _sdata; pDest < _edata;)
    {
        *pDest++ = *pSrc++;
    }

    /* Zero fill bss segment */
    pDest = _sbss;

    for (; pDest < _ebss;)
    {
        *pDest++ = UINT32_C(0);
    }

#ifndef __NO_SYSTEM_INIT
    /* Call the clock system initialization function. */
    SystemInit();
#endif /* __NO_SYSTEM_INIT */

    /* Call static constructors */
    __libc_init_array();

    /* Call the application's entry point */
    (void) main();

    for (;;)
    {
        ; /* spin loop if reached */
    }
}

#if (PROVIDE_NMI_HANDLER)
void NMI_Handler(void)
{
    /** @todo 2016-11-25, BCDS/ENG1: Only define custom NMI_Handler() if really
     *        required by BSE R3.2 configuration.
     */
    HAL_RCC_NMI_IRQHandler();
}
#endif /* if (PROVIDE_NMI_HANDLER) */

void Default_Handler(void)
{
    /*lint -e(506) > 2017-01-26, BCDS/ENG1: Intended behavior is to assert when reached. */
    assert(0);

    for (;;)
    {
        ; /* spin loop if reached */
    }
}

#endif /* BCDS_FEATURE_BSP_STARTUP */
/**@}*//* @ingroup BSP_STARTUP */
