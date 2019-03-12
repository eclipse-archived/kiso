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
 * @ingroup BSP_GSM_MODULE
 * @{
 *  @file
 *  @brief Handling routines provided for GSM related interrupts.
 */

#ifndef GSM_SIM800H_H_
#define GSM_SIM800H_H_

#include "BCDS_BSP_GSM_SIM800H.h"

#if BCDS_FEATURE_BSP_GSM_SIM800H

/**
 * @brief UART interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void GSM_SIM800H_UARTISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * receive channel and to forward them to the MCU driver for processing.
 */
void GSM_SIM800H_DMARxISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * transmit channel and to forward them to the MCU driver for processing.
 */
void GSM_SIM800H_DMATxISR(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E GSM_SIM800H_getState(void);

#endif /* BCDS_FEATURE_BSP_GSM_SIM800H */

#endif /* GSM_SIM800H_H_ */
/**@}*//* @ingroup BSP_GSM_MODULE */
