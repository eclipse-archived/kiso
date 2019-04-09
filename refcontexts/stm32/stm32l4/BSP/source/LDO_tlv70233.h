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
