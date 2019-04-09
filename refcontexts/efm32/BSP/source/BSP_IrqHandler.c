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

#include "BCDS_HAL.h"
#include "BSP_BoardType.h"
#include "BSP_BoardSettings.h"
#include "BSP_BoardShared.h"
#include "BSP_IrqHandler.h"

#if BCDS_BOARD_XDK110

#include "BSP_BT_EM9301.h"
#include "BSP_SDCard.h"
#include "BSP_SensorNode.h"
#include "BSP_WiFi_cc3100mod.h"
#include "BSP_LoRa_RN2xx3.h"
#include "BSP_UMTS_LisaU2.h"
#include "BSP_UMTS_QuectelUC20.h"

#elif BCDS_BOARD_APLM
#include "BSP_Lora_SX1272.h"
#include "BSP_TestInterface.h"
#endif

#include "BCDS_MCU_Timer.h"
#include "BCDS_MCU_Timer_Handle.h"

#if BCDS_FEATURE_HAL_TICK_HANDLER
/*
 * Refer to prototype for description
 */
void SysTick_Handler(void)
{
    if (NULL != SysTickOsPreProcessing)
    {
        SysTickOsPreProcessing();
    }

    BoardCommon_IncTick();

    if (NULL != SysTickOsPostProcessing)
    {
        SysTickOsPostProcessing();
    }
}
#endif

/*
 * Refer to prototype for description
 */
void GPIO_EVEN_IRQHandler(void)
{
    uint32_t iflags;

    /* Get all even interrupts. */
    iflags = GPIO_IntGetEnabled() & BSP_GPIO_INT_EVEN_MASK;

    /* Clean only even interrupts. */
    GPIO_IntClear(iflags);

    BoardCommon_GPIOInterruptDispatch(iflags);
}

/*
 * Refer to prototype for description
 */
void GPIO_ODD_IRQHandler(void)
{
    uint32_t iflags;

    /* Get all odd interrupts. */
    iflags = GPIO_IntGetEnabled() & BSP_GPIO_INT_ODD_MASK;

    /* Clean only even interrupts. */
    GPIO_IntClear(iflags);

    BoardCommon_GPIOInterruptDispatch(iflags);
}
#if BCDS_BOARD_XDK110
#if BCDS_FEATURE_BSP_BT_EM9301
/*
 * Refer to interface header for description
 */
void UART0_TX_IRQHandler(void)
{
    BT_EM9301_UART_TxISR();
}
/*
 * Refer to interface header for description
 */
void UART0_RX_IRQHandler(void)
{
    BT_EM9301_UART_RxISR();
}
#endif


#if BCDS_FEATURE_BSP_WIFI_CC3100MOD
/*
 * Refer to interface header for description
 */
void USART0_TX_IRQHandler(void)
{
    WiFi_CC3100MOD_SPITxISR();
}
/*
 * Refer to interface header for description
 */
void USART0_RX_IRQHandler(void)
{
    WiFi_CC3100MOD_SPIRxISR();
}
#endif

#if BCDS_FEATURE_BSP_SENSOR_NODE
/*
 * Refer to interface header for description
 */
void I2C0_IRQHandler(void)
{
    SensorNode_I2CISR();
}

#endif

#if BCDS_FEATURE_BSP_EXTENSIONPORT
/*
 * Refer header BSP_IrqHandler.h for description
 */
void I2C1_IRQHandler(void)
{
    BSP_ExtensionPort_I2cIsr();
}

/*
 * Refer header BSP_IrqHandler.h for description
 */
void USART2_TX_IRQHandler(void)
{
	BSP_ExtensionPort_SpiTxIsr();
}

/*
 * Refer header BSP_IrqHandler.h for description
 */
void USART2_RX_IRQHandler(void)
{
	BSP_ExtensionPort_SpiRxIsr();
}

/*
 * Refer header BSP_IrqHandler.h for description
 */
void UART1_TX_IRQHandler(void)
{
    BSP_ExtensionPort_UARTTxIsr();
}

/*
 * Refer header BSP_IrqHandler.h for description
 */
void UART1_RX_IRQHandler(void)
{
    BSP_ExtensionPort_UARTRxIsr();
}

#endif

#if BCDS_FEATURE_BSP_SD_CARD
/*
 * Refer to interface header for description
 */
void USART1_TX_IRQHandler(void)
{
    SDCard_SPITxISR();
}
/*
 * Refer to interface header for description
 */
void USART1_RX_IRQHandler(void)
{
    SDCard_SPIRxISR();
}
#endif
#endif  /*BCDS_BOARD_XDK110*/

#if BCDS_BOARD_APLM

#if BCDS_FEATURE_BSP_LORA_SX1272
/* Refer to interface header for description */
void USART2_TX_IRQHandler(void)
{
	LoRa_SX1272_SPITxISR();
}
/* Refer to interface header for description */
void USART2_RX_IRQHandler(void)
{
	LoRa_SX1272_SPIRxISR();
}
#endif /*BCDS_FEATURE_BSP_LORA_SX1272*/

#if BCDS_FEATURE_BSP_TEST_INTERFACE
/* Refer to interface header for description */
void LEUART1_IRQHandler(void)
{
	BSP_LEUART1_IRQHandler();
}
#endif /*BCDS_FEATURE_BSP_TEST_INTERFACE*/

#endif /*BCDS_BOARD_APLM*/

#if BCDS_FEATURE_EFM32_BURTC
void BURTC_IRQHandler(void)
{
    MCU_Timer_HandleBurtcIrq();
}
#endif

#if BCDS_FEATURE_EFM32_RTC
void RTC_IRQHandler(void)
{
    MCU_Timer_HandleRtcIrq();
}
#endif


/** @todo workaround to address weak linkage of SysTick_Handler defined in IrqHandler.c file */

Retcode_T IrqHandler_SysTickDummyHandler(void)
{
    return RETCODE_OK;
}
