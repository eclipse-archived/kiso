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
 
#ifndef BCDS_BCDS_BSP_LoRa_RN2xx3_th_HH
#define BCDS_BCDS_BSP_LoRa_RN2xx3_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_LoRa_RN2xx3.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_LORA_RN2XX3

FAKE_VALUE_FUNC(Retcode_T, BSP_LoRa_RN2xx3_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_LoRa_RN2xx3_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_LoRa_RN2xx3_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_LoRa_RN2xx3_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_LoRa_RN2xx3_GetUartHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_LoRa_RN2xx3_SetResetPin,uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_LoRa_RN2xx3_ChangeBaudrate,uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_LoRa_RN2xx3_WakeUp);

#endif /* BCDS_FEATURE_BSP_LORA_RN2XX3 */

#endif /* BCDS_BCDS_BSP_LoRa_RN2xx3_th_HH */
