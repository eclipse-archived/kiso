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
 * @ingroup BSP_XDK110_LoRa_MODULE
 * @{
 */
/**
 *
 *  @file
 *
 *  @brief Handling routines provided for LoRa module related interrupts
 *
 */

#ifndef LoRa_SX1272_H
#define LoRa_SX1272_H

#include "BCDS_BSP_LoRa_SX1272.h"

#if BCDS_FEATURE_BSP_LORA_SX1272

/**
 * @brief USART transmit interrupt service routine used to capture interrupts at the USART
 * resource and to forward them to the MCU driver for processing.
 */
void LoRa_SX1272_SPITxISR(void);

/**
 * @brief USART receive interrupt service routine used to capture interrupts at the USART
 * resource and to forward them to the MCU driver for processing.
 */
void LoRa_SX1272_SPIRxISR(void);

HWHandle_T BSP_LoRa_SX1272_GetSPIHandle(void);

#endif /* BCDS_FEATURE_BSP_LoRa_SX1272 */

#endif /* LoRa_SX1272_H */

/**@}*/

