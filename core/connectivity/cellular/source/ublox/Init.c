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

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_INIT

#include "Kiso_Cellular.h"

#include "Engine.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

Retcode_T Cellular_Initialize(Cellular_StateChanged_T onStateChanged)
{
    return Engine_Initialize(onStateChanged);
}

Retcode_T Cellular_Deinitialize(void)
{
    return Engine_Deinitialize();
}
