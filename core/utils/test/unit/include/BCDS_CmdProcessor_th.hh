/****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file        CmdProcessor_th.hh
 *
 *  Mockup implementation for the CmdProcessor module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_CMDPROCESSOR_TH_HH_
#define BCDS_CMDPROCESSOR_TH_HH_

/* include system header files */

/* wrap the real  interface header */
#include "BCDS_CmdProcessor.h"
#include "BCDS_Retcode.h"

/* faked variables needs to be initialized by the test fixture */
typedef void (*CmdProcessor_Func_T)(void *param1, uint32_t param2);

/**
 * mock-ups for the provided interfaces
 * Deprecated functions will be removed as part of PFM-659
 * Deprecated since vXXX (TODO: replace XXX with next version)
 */
FAKE_VOID_FUNC(CmdProcessor_suspend , CmdProcessor_T *);
FAKE_VOID_FUNC(CmdProcessor_resume , CmdProcessor_T *);
FAKE_VALUE_FUNC(Retcode_T,CmdProcessor_initialize,CmdProcessor_T *,char *,uint32_t,uint32_t,uint32_t);
FAKE_VALUE_FUNC(Retcode_T,CmdProcessor_enqueue,CmdProcessor_T *,CmdProcessor_Func_T,void *,uint32_t);
FAKE_VALUE_FUNC(Retcode_T,CmdProcessor_enqueueFromIsr,CmdProcessor_T *,CmdProcessor_Func_T,void *,uint32_t);

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(CmdProcessor_Suspend , CmdProcessor_T *);
FAKE_VOID_FUNC(CmdProcessor_Resume , CmdProcessor_T *);
FAKE_VALUE_FUNC(Retcode_T,CmdProcessor_Initialize,CmdProcessor_T *,char *,uint32_t,uint32_t,uint32_t);
FAKE_VALUE_FUNC(Retcode_T,CmdProcessor_Enqueue,CmdProcessor_T *,CmdProcessor_Func_T,void *,uint32_t);
FAKE_VALUE_FUNC(Retcode_T,CmdProcessor_EnqueueFromIsr,CmdProcessor_T *,CmdProcessor_Func_T,void *,uint32_t);

#endif /* BCDS_CMDPROCESSOR_TH_HH_ */

/** ************************************************************************* */
