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
 * @file Delay.c
 *
 * @brief Implementation of a Delay functions for non Os
 *
 */

#include "Kiso_Essentials.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_DELAY

/* additional interface header files */
#include "Kiso_Basics.h"

/* own header files */
#include "Kiso_HAL_Delay.h"
#include "Cmsis.h"

#include "Kiso_BSP_Board.h"
#ifdef __GNUC__

static Retcode_T NoOs_Delay_Ms(uint32_t timeInMs);



static HAL_Delay_MsHook_T _Delay_MsHook = 	NoOs_Delay_Ms;




Retcode_T HAL_Delay_SetMsHook( HAL_Delay_MsHook_T msHook){
	if(NULL != msHook)
	{
		_Delay_MsHook = msHook;
		return RETCODE_OK;
	}
	return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
}


Retcode_T HAL_Delay_WaitUs(uint32_t timeInUs)
{
	BSP_Board_DelayUs(timeInUs);
	return RETCODE_OK;
}


Retcode_T HAL_Delay_WaitMs(uint32_t timeInMs){
	if(NULL != _Delay_MsHook)
	{
		return(_Delay_MsHook(timeInMs));
	}
	return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
}

static Retcode_T NoOs_Delay_Ms(uint32_t timeInMs){
	Retcode_T rc = RETCODE_OK;
	if(0 == timeInMs ){
		return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
	}

	do{
		rc = HAL_Delay_WaitUs(1000);
	}
	while(timeInMs--);
	return rc;
}


#endif
