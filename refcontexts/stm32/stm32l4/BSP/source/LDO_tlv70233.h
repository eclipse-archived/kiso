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
 * @ingroup BSP_BOARD
 * @{
 *  @file
 *  @brief Interfaces to the 3.3V / 300mA TLV702 series low-dropout (LDO) linear
 *  regulator board peripheral.
 */

#ifndef LDO_TLV70233_H_
#define LDO_TLV70233_H_

#include "BCDS_Essentials.h"

Retcode_T BSP_LDO_TLV70233_Connect(void);
Retcode_T BSP_LDO_TLV70233_Disconnect(void);

/*
 * @brief Enables the 3V3 power rail and keeps track of the reference count.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T BSP_LDO_TLV70233_Enable(void);

/*
 * @brief Disables the 3V3 power rail if the tracked reference count is zero.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T BSP_LDO_TLV70233_Disable(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E LDO_TLV70233_getState(void);

#endif /* LDO_TLV70233_H_ */
/**@}*//* @ingroup BSP_BOARD */
