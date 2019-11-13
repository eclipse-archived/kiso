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

#ifndef APPMODULES_H_
#define APPMODULES_H_

/**
 * @brief Enumerates application modules which are reporting error codes according to RETCODE specification.
 * @info  usage:
 *      #undef KISO_APP_MODULE_ID
 *      #define KISO_APP_MODULE_ID APP_MODULE_ID_xxx
 */
enum App_ModuleID_E
{
    APP_MODULE_ID_MAIN = 1,
    APP_MODULE_ID_APP
    /* Define next module ID here and assign a value to it! */
};

#endif
