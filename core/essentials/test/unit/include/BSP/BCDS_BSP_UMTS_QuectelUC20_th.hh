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
 
#ifndef BCDS_BCDS_UMTS_QECTELUC20_TH_HH
#define BCDS_BCDS_UMTS_QECTELUC20_TH_HH

#include "BCDS_HAL_th.hh"
#include "BCDS_BSP_UMTS_QuectelUC20.h"


/* Mock-ups for the provided interfaces */
#if BCDS_FEATURE_BSP_UMTS_QUECTELUC20

FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_QuectelUC20_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_QuectelUC20_Disable);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_PowerOn);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_PowerOff);
FAKE_VOID_FUNC(BSP_UMTS_QuectelUC20_Reset);
FAKE_VALUE_FUNC(HWHandle_T, BSP_UMTS_QuectelUC20_GetUARTHandle);

#endif /* BCDS_FEATURE_BSP_UMTS_QUECTELUC20 */
#endif /* BCDS_BCDS_UMTS_QECTELUC20_TH_HH */
