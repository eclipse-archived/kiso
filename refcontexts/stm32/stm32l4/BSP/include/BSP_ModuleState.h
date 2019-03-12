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

#ifndef BSP_MODULESTATE_H_
#define BSP_MODULESTATE_H_

/**
 * @defgroup BSPStates Board Support Package - Module State definitions
 * @{
 * @file
 * @brief Typedefs and enums supporting BSP Module state handlings
 * @details This header file contains the functionality necessary to describe
 * the state for a module.
 */

/* Include all headers which are needed by this file. */
#include "BCDS_HAL_CriticalSection.h"

/* Put the type and macro definitions here */

/**
 * @brief BSP_MODULE_STATE describes the possible state of a module
 */
enum BSP_ModuleState_E
{
    BSP_MODULE_STATE_UNINITIALIZED,
    BSP_MODULE_STATE_CONNECTED,
    BSP_MODULE_STATE_DISCONNECTED,
    BSP_MODULE_STATE_ENABLED,
    BSP_MODULE_STATE_ERROR,
};

enum BSP_ModuleLock_E
{
    BSP_MODULE_LOCKED,
    BSP_MODULE_UNLOCKED,
};

typedef volatile enum BSP_ModuleLock_E BSP_ModuleLock_T;


static inline Retcode_T BSP_ModuleLock(BSP_ModuleLock_T *stateVar)
{
    Retcode_T RetVal;
    uint32_t count=0;
    RetVal = HAL_CriticalSection_Enter(&count);
    if(RETCODE_OK == RetVal)
    {
        if(*stateVar == BSP_MODULE_LOCKED)
        {
            RetVal = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
            /* Intentionally ignore the return value of HAL_CriticalSectionLeave() */
            (void)HAL_CriticalSection_Leave(&count);
        }
        else
        {
            *stateVar = BSP_MODULE_LOCKED;
            RetVal = HAL_CriticalSection_Leave(&count);
        }
    }
    return RetVal;
}

static inline void BSP_ModuleUnLock(BSP_ModuleLock_T *stateVar)
{
    *stateVar = BSP_MODULE_UNLOCKED;
}

/* The actual state variable is defined here */
static BSP_ModuleLock_T Module_LockVar = BSP_MODULE_UNLOCKED;


#define BSP_MODULE_TRY_LOCK \
do{\
    Retcode_T RetVal;\
\
    RetVal = BSP_ModuleLock(&Module_LockVar);\
    if(RetVal != RETCODE_OK)\
    {\
        return RetVal;\
    }\
}while(0)

#define BSP_MODULE_UNLOCK \
do{ \
    BSP_ModuleUnLock(&Module_LockVar); \
}while(0)


/* Put the function declarations here */

/**@} *//* @defgroup BSPStates */
#endif /* BSP_MODULESTATE_H_ */
