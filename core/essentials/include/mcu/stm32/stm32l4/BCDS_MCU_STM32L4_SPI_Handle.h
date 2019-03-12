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
 * Defines MCU spi handle
 *
 * @details
 * MCU functions do always have at least an integer parameter of type SPI_T.
 * This integer is casted by MCU to a pointer to MCU_SPI_handle_T.
 * The data pointed to by SPI_TypeDef* is managed in BSP.
 */

#ifndef BCDS_MCU_STM32L4_SPI_HANDLE_H_
#define BCDS_MCU_STM32L4_SPI_HANDLE_H_

#if BCDS_FEATURE_SPI

#include "BCDS_MCU_SPI.h"

#include "stm32l4xx.h"
#include "stm32l4xx_hal_dma.h"
#include "stm32l4xx_hal_spi.h"

/**
 * @brief This data type represents a function pointer which is used between
 * BSP and MCU SPI as a callback whenever an IRQ event is to be notified from
 * the BSP to the MCU SPI driver.
 *
 * @param [in] spi : Handle of the SPI whose IRQ event should be handled.
 */
typedef void (*MCU_BSP_SPI_IRQ_Callback_T)(SPI_T spi);
typedef void (*MCU_BSP_SPI_DMA_Callback_T)(SPI_T spi);


/**
 * @brief Declares a function pointer for receive and transmit functions
 * which has the same signature like:
 *  - HAL_SPI_Transmit_IT
 *  - HAL_SPI_Receive_IT
 *  - HAL_SPI_Transmit_DMA
 *  - HAL_SPI_Receive_DMA
 */
typedef HAL_StatusTypeDef (*STM_HAL_RxTxFunPtr)(SPI_HandleTypeDef *hspi,
        uint8_t *pData,
        uint16_t Size);

/**
 * @brief Declares a function pointer for transfer function:
 *  - HAL_SPI_Transfer_IT
 *  - HAL_SPI_Transfer_DMA
 */
typedef HAL_StatusTypeDef (*STM_HAL_TransferFuncPtr)(SPI_HandleTypeDef *hspi,
        uint8_t *pDataOut,
        uint8_t *pDataIn,
        uint16_t Size);


/**
 * @brief   Structure which is used as SPI handle.
 *
 * @detail  The handle is usually created by the BSP and fetched by the
 * application layer to use the SPI interface.
 */
struct MCU_SPI_S
{
    SPI_HandleTypeDef hspi;            /**<    data structure of driver for type DriverType*/

    MCU_BSP_SPI_IRQ_Callback_T IRQCallback;     /**<    Function pointer of the MCU
                                                        SPI IRQ callback handler which
                                                        is used by the BSP to notify
                                                        MCU about IRQ events. Will
                                                        be set by MCU SPI upon
                                                        initialization. */
    enum BCDS_HAL_TransferMode_E TransferMode;  /**<    Identifies the transfer
                                                        mode that is currently configured for this SPI instance.
                                                        This value will be set by the BSP upon configuration of the
                                                        SPI interface.*/
    MCU_BSP_SPI_DMA_Callback_T DmaRxCallback;   /**<    Function invoked by BSP in case IRQ and Rx DMA mode */
    MCU_BSP_SPI_DMA_Callback_T DmaTxCallback;   /**<    Function invoked by BSP in case IRQ and Tx DMA mode  */

    MCU_SPI_Callback_T AppCallback; /**< */
    /** Function pointer to the vendor library Tx function. Will be set
     *  upon initialization. */
    HAL_StatusTypeDef (*TxFunPtr)(SPI_HandleTypeDef *hspi,
            uint8_t *pData,
            uint16_t Size);

    /** Function pointer to the vendor library Rx function. Will be set upon
     *  initialization.*/
    HAL_StatusTypeDef (*RxFunPtr)(SPI_HandleTypeDef *hspi,
            uint8_t *pData,
            uint16_t Size);

    /** Function pointer to the vendor library RxTx function. Will be set upon
     *  initialization.*/
    HAL_StatusTypeDef (*TransferFunPtr)(SPI_HandleTypeDef *hspi,
            uint8_t *pDataOut,
            uint8_t *pDataIn,
            uint16_t Size);
};


#endif /* BCDS_FEATURE_SPI && BCDS_SPI_COUNT */
#endif /* BCDS_MCU_STM32L4_SPI_HANDLE_H_ */

