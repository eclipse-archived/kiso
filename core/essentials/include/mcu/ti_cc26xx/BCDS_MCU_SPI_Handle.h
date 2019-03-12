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
 * Defines MCU SPI handle
 *
 * @details
 * MCU functions do always have at least an integer parameter of type SPI_T.
 * This integer is casted by MCU to a pointer to MCU_SPI_handle_T.
 */

#ifndef BCDS_MCU_SPI_HANDLE_H_
#define BCDS_MCU_SPI_HANDLE_H_

#if BCDS_FEATURE_SPI

#include "BCDS_MCU_SPI.h"

/* TI SPI abstraction layer.  BLE SDK 3.0 C:\ti\simplelink_cc2640r2_sdk_1_00_00_22\source\ti\drivers */
#include "drivers/SPI.h"

enum MCU_SPIState_E
{
    MCU_SPI_STATE_INIT,
    MCU_SPI_STATE_READY,
    MCU_SPI_STATE_ABORTING,
    MCU_SPI_STATE_TX,
    MCU_SPI_STATE_RX,
    MCU_SPI_STATE_TX_RX
};

/**
 * @brief   Structure which is used as SPI handle.
 *
 * @detail  The handle is usually created by the BSP and fetched by the
 * application layer to use the SPI interface.
 */
struct MCU_SPI_Handle_S;

struct MCU_SPI_Handle_S
{
    enum MCU_SPIState_E  volatile state; /**< state of the peripheral using the SPI resource*/
    SPI_Handle  handle; /**< Handle expected by TI SPI driver */
    enum BCDS_HAL_TransferMode_E transferMode; /**< transfer mode used */
    enum BCDS_HAL_CallMode_E callMode; /**< call mode used */
    void (*transferCallback)(SPI_T spi); /**< function that will be called upon transfer completion */
    MCU_SPI_Callback_T appCallback; /**< function registered by the application to receive the SPI transfer events( transfer_complete or transfer_error )*/
    Retcode_T (*readFunc)(struct MCU_SPI_Handle_S* spi, uint8_t * data, uint32_t length); /**< SPI transfer function pointer */
    Retcode_T (*writeFunc)(struct MCU_SPI_Handle_S* spi, uint8_t * data, uint32_t length);
    Retcode_T (*transferFunc)(struct MCU_SPI_Handle_S* spi, uint8_t * dataOut, uint8_t * dataIn, uint32_t length);
    Retcode_T (*abortFunc)(struct MCU_SPI_Handle_S* spi);
    SPI_Transaction  transaction;/**transaction needed by TI driver to perform send receive operation   */
    uint32_t  volatile count;/** counter of transferred frames  */
};

#endif /* BCDS_FEATURE_SPI && BCDS_SPI_COUNT */
#endif /* BCDS_MCU_SPI_HANDLE_H_ */

