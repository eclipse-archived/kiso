/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) 2015-2018 Bosch Connected Devices and Solutions GmbH. 
 * Copyright (C) 2019 Robert Bosch GmbH.
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
 * @brief
 * Defines MCU uart handle
 *
 * @details
 * MCU functions do always have at least an integer parameter of type UART_T.
 * This integer is casted by MCU to a pointer to MCU_UART_handle_T.
 * The data pointed to by USART_TypeDef* is managed in BSP.
 */

#ifndef BCDS_MCU_STM32L4_UART_HANDLE_H_
#define BCDS_MCU_STM32L4_UART_HANDLE_H_

#include "BCDS_MCU_UART.h"

#if BCDS_FEATURE_UART

#include "stm32l4xx.h"
#include "stm32l4xx_hal_dma.h"
#include "stm32l4xx_hal_uart.h"
/**
 * @brief   Data type definition for the UART IRQ callback function.
 *
 * The IRQ callback is registered at the MCU_UART_Initialize function
 * by setting a member inside the structure of the uart handle.
 *
 * It is invoked by the UART BSP driver to notify the MCU.
 *
 * @param[in] uart : The handle of uart
 *
 * @note There is no need for an event parameter because all information is
 * available through the handle of type UART_T.
 *
 * @see MCU_UART_Initialize()
 */
typedef void (*UART_IRQ_Callback_T)(UART_T uart);
typedef void (*UART_DMA_Callback_T)(UART_T uart);



/** @brief
 *  Each public MCU interface function gets a handle parameter. This handle is wrapped as UART_T type,
 *  which is in fact a pointer to structure MCU_UART_handle_S and the storage of this structure
 *  is provided by BSP and a pointer is stored by MCU inside the structure MCU_UART_Driver_S.
 *  For each uart a MCU_UART_Driver_S structure is stored by the MCU inside MCU_UART_ArrayDriver.
 */
struct MCU_UART_S
{
    UART_HandleTypeDef huart; /**< data structure of driver for type DriverType */

    enum BCDS_HAL_TransferMode_E TransferMode; /**< Set by BSP to tell MCU whether to use interrupt-mode or DMA */
    UART_IRQ_Callback_T IrqCallback; /**< Function invoked by BSP in case IRQ and interrupt mode */
    UART_DMA_Callback_T DmaRxCallback; /**< Function invoked by BSP in case IRQ and Rx DMA mode */
    UART_DMA_Callback_T DmaTxCallback; /**< Function invoked by BSP in case IRQ and Tx DMA mode  */

    MCU_UART_Callback_T AppCallback; /**< Callback function given in MCU_UART_Initialize to notify application */

    bool isSending; /**< Flag to indicate that send procedure is ongoing */
    bool isReceiving; /**< Flag to indicate that receive procedure is ongoing */

    uint32_t RxUserSize; /**< Rx length of data expected by user. The actual count is inside ring buffer. */
    uint8_t * RxUserData_ptr; /**< Pointer to user data buffer. Is NULL if receive function has not been called by user or receive has been finished. */
};


#endif /* BCDS_FEATURE_UART && UART_COUNT */

#endif /* BCDS_MCU_STM32L4_UART_HANDLE_H_ */
