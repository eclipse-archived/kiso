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

#ifndef BCDS_BCDS_BSP_CAN_SN65HVD234_th_HH
#define BCDS_BCDS_BSP_CAN_SN65HVD234_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_CAN_SN65HVD234.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_CAN_SN65HVD234

FAKE_VALUE_FUNC(Retcode_T, BSP_CAN_SN65HVD234_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_CAN_SN65HVD234_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_CAN_SN65HVD234_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_CAN_SN65HVD234_Disconnect);
FAKE_VALUE_FUNC(Retcode_T, BSP_CAN_SN65HVD234_EnableIRQ);
FAKE_VALUE_FUNC(Retcode_T, BSP_CAN_SN65HVD234_DisableIRQ);

#endif /* BCDS_FEATURE_BSP_CAN_SN65HVD234 */
#endif /* BCDS_BCDS_BSP_CAN_SN65HVD234_th_HH */
