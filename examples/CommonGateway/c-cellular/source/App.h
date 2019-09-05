/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     Robert Bosch GmbH - initial contribution
 */

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

/**
 * @brief Initialize application.
 *
 * @note To be called from a CmdProcessor instance.
 *
 * @param[in] param
 * Pointer to the CmdProcessor instance invoking this function.
 * @param[in] len
 * Unused.
 */
void App_InitSystem(void *param, uint32_t len);
