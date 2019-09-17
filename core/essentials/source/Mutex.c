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
 * @file Mutex.c
 *
 * @brief Implementation of a Mutex for non Os
 *
 */
#include "Kiso_Essentials.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_LOCK


/* own header files */
#include "Kiso_HAL_Mutex.h"

/* additional interface header files */
#include "Kiso_Basics.h"
#include "Cmsis.h"



static Retcode_T NoOs_Mutex_Lock(HAL_Mutex_T *mutex);
static Retcode_T NoOs_Mutex_Unlock(HAL_Mutex_T *mutex);


static HAL_Mutex_Hook_T _HAL_Mutex_LockFunction 	= 	NoOs_Mutex_Lock;
static HAL_Mutex_Hook_T _HAL_Mutex_UnlockFunction	= 	NoOs_Mutex_Unlock;



Retcode_T HAL_Mutex_SetHooks(HAL_Mutex_Hook_T lockHook, HAL_Mutex_Hook_T unlockHook)
{
	if( NULL != lockHook && NULL != unlockHook)
	{

		_HAL_Mutex_LockFunction 	= 	lockHook;
		_HAL_Mutex_UnlockFunction 	= 	unlockHook;

		return RETCODE_OK;
	}
		return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
}


Retcode_T HAL_Mutex_Lock(HAL_Mutex_T *mutex)
{
	if(NULL != _HAL_Mutex_LockFunction)
	{
		return (_HAL_Mutex_LockFunction(mutex));
	}

	return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
}


Retcode_T HAL_Mutex_Unlock(HAL_Mutex_T *mutex)
{
	if(NULL != _HAL_Mutex_UnlockFunction)
	{
		return (_HAL_Mutex_UnlockFunction(mutex));
	}

	return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
}


static Retcode_T NoOs_Mutex_Lock(HAL_Mutex_T *mutex)
{
	KISO_UNUSED(mutex);
	return RETCODE_OK;
}

static Retcode_T NoOs_Mutex_Unlock(HAL_Mutex_T *mutex)
{
	KISO_UNUSED(mutex);
	return RETCODE_OK;
}

