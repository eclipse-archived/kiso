/*----------------------------------------------------------------------------*/
/**
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 *
 * @file
 *
 * @brief Command Processor for Hydrogen
 *
 * @details This is the implementation of the command processor for the BCDS Hydrogen platform flavor
 *
 * This module implements following functions:
 *
 * - @ref CmdProcessor_Initialize()
 * - @ref CmdProcessor_Enqueue()
 * - @ref CmdProcessor_EnqueueFromIsr()
 */
/*----------------------------------------------------------------------------*/

#include "BCDS_CmdProcessor.h"

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_CMDPROCESSOR


#if (1 == BCDS_FEATURE_CMDPROCESSOR)

/* TI-RTOS APIs used */
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h> /**< Task API */
#include <ti/sysbios/knl/Mailbox.h> /**< Mailbox API for Queues */
#include <ICall.h> /**< ICall is used for memory allocation */

#define CMDPROCESSOR_ENQUEUE_WAIT_TIME       BIOS_WAIT_FOREVER
#define CMDPROCESSOR_DEQUEUE_WAIT_TIME       BIOS_WAIT_FOREVER

/* Task loop condition for unit-tests */
#ifndef CMD_PROCESSOR_TASK_LOOP_CONDITION
#define CMD_PROCESSOR_TASK_LOOP_CONDITION    true
#endif

/* Singleton default parameters */

#ifndef DEFAULT_TASK_STACK_SIZE
#define DEFAULT_TASK_STACK_SIZE           UINT32_C(644)
#endif

#ifndef CMD_PROCESSOR_DEFAULT_QUEUE_SIZE
#define CMD_PROCESSOR_DEFAULT_QUEUE_SIZE   UINT32_C(20)
#endif

/**
 * Command processor queue element.
 */
struct CmdProcessor_Cmd_S
{
    CmdProcessor_Func_T func; /**< Command function to be executed */
    void *param1; /**< First command parameter */
    uint32_t param2; /**< Second command parameter */
};

/*The data type for command processor elements */
typedef struct CmdProcessor_Cmd_S CmdProcessor_Cmd_T;

/**
 * Default command processor singleton
 */
static CmdProcessor_T CmdProcessorContext;

/** Command Processor Task Runner
 *
 * @details    This function runs as a task in the background.
 *
 * @param[in] arg0 : Command processor handle. Given as an input by the task creation method.
 *
 * @param[in] arg1 : Unused
 *
 */
static void TaskLoopRunner(UArg arg0, UArg arg1);

/**
 * Command Processor Dequeue and Execute Function
 *
 * @details This function dequeues a command from the command processor queue and executes it.
 *
 * @param[in] cmdProcessor  : Command processor handle
 *
 */
static Retcode_T DequeueAndExecute(CmdProcessor_T * cmdProcessor);


/** The description of the function is available in header file */
Retcode_T CmdProcessor_Initialize(CmdProcessor_T * cmdProcessor, char *name, uint32_t taskPriority, uint32_t taskStackDepth, uint32_t queueSize)
{
    BCDS_UNUSED(cmdProcessor);
    BCDS_UNUSED(name);

    Retcode_T retcode = RETCODE_OK;

    Ptr taskStackPtr = (Ptr) NULL;

    /* Get default Command Processor Handle */
    CmdProcessor_T * cmdProcessorHandle = &CmdProcessorContext;

    if (UINT32_C(0) == taskPriority)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (UINT32_C(0) == taskStackDepth)
    {
        taskStackDepth = DEFAULT_TASK_STACK_SIZE;
    }

    if (UINT32_C(0) == queueSize)
    {
        queueSize = CMD_PROCESSOR_DEFAULT_QUEUE_SIZE;
    }

    /* Initialize and register the queue*/

    Mailbox_Params mailboxParams;
    Mailbox_Params_init(&mailboxParams);

    cmdProcessorHandle->queue = (cmdProcessorQueueHandle_t) Mailbox_create(sizeof(CmdProcessor_Cmd_T), queueSize, &mailboxParams, NULL);

    if (NULL == cmdProcessorHandle->queue)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_QUEUE_INIT_FAIL);
    }

    /* Acquire Task Stack */
    if (RETCODE_OK == retcode)
    {
        taskStackPtr = (Ptr) ICall_malloc(taskStackDepth);
        if (NULL == taskStackPtr)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }

    /* Initialize the task */
    if (RETCODE_OK == retcode)
    {
        Task_Params taskParams;

        Task_Params_init(&taskParams);
        taskParams.arg0 = (UArg) cmdProcessorHandle; /**< The task receives its context handle */
        taskParams.stack = taskStackPtr; /**< Stack pointer obtained using dynamic allocation */
        taskParams.stackSize = taskStackDepth; /**< Stack pointer depth */
        taskParams.priority = taskPriority; /**< Task priority */

        cmdProcessorHandle->task = (cmdProcessorTaskHandle_t) Task_create((Task_FuncPtr) TaskLoopRunner, &taskParams, NULL);

        if (NULL == cmdProcessorHandle->task)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_TASK_INIT_FAIL);
        }
    }

    return retcode;
}

/**  The description of the function is available in header file. */
Retcode_T CmdProcessor_Enqueue(CmdProcessor_T * cmdProcessor, CmdProcessor_Func_T func, void *param1, uint32_t param2)
{
    BCDS_UNUSED(cmdProcessor);

    /* Get default Command Processor Handle */
    CmdProcessor_T * cmdProcessorHandle = &CmdProcessorContext;

    Retcode_T retcode = RETCODE_OK;
    Bool osRc = TRUE;

    CmdProcessor_Cmd_T cmd;

    if (NULL == func)
    {
        /* Tried to enqueue an invalid element */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        /* Assemble command element */
        cmd.func = func;
        cmd.param1 = param1;
        cmd.param2 = param2;

        /* Enqueue command element */
        if (NULL != cmdProcessorHandle->queue)
        {
            osRc = Mailbox_post((Mailbox_Handle) cmdProcessorHandle->queue, &cmd, CMDPROCESSOR_ENQUEUE_WAIT_TIME);

            if (FALSE == osRc)
            {
                /*< Queue enqueue failure */
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_QUEUE_ENQUEUE_FAIL);
            }
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        }
    }

    return retcode;
}

Retcode_T CmdProcessor_EnqueueFromIsr(CmdProcessor_T *cmdProcessor, CmdProcessor_Func_T func, void *param1, uint32_t param2)
{
    BCDS_UNUSED(cmdProcessor);

    /* Get default Command Processor Handle */
    CmdProcessor_T * cmdProcessorHandle = &CmdProcessorContext;

    Retcode_T retcode = RETCODE_OK;
    Bool osRc = TRUE;

    CmdProcessor_Cmd_T cmd;

    if (NULL == func)
    {
        /* Tried to enqueue an invalid element */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        /* Construct command element */
        cmd.func = func;
        cmd.param1 = param1;
        cmd.param2 = param2;

        /* Enqueue command element */
        if (NULL != cmdProcessorHandle->queue)
        {
            /* The Mailbox post can be only done using a wait time of BIOS_NO_WAIT */
            osRc = Mailbox_post((Mailbox_Handle) cmdProcessorHandle->queue, &cmd, BIOS_NO_WAIT);

            if (FALSE == osRc)
            {
                /*< Queue enqueue failure */
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_QUEUE_ENQUEUE_FAIL);
            }
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        }
    }

    return retcode;
}

/* This is the task function. It will continuously run the Dequeue and Execute function */
static void TaskLoopRunner(UArg arg0, UArg arg1)
{
    BCDS_UNUSED(arg1);

    CmdProcessor_T * cmdProcessor = (CmdProcessor_T *) arg0;

    Retcode_T retcode = RETCODE_OK;

    do
    {
        retcode = DequeueAndExecute(cmdProcessor);

        if (RETCODE_OK != retcode)
        {
            Retcode_RaiseError(retcode);
        }

    }
    while (CMD_PROCESSOR_TASK_LOOP_CONDITION); //lint !e506 Boolean constant is used to ensure an infinite task loop
}

static Retcode_T DequeueAndExecute(CmdProcessor_T * cmdProcessor)
{
    Retcode_T retcode = RETCODE_OK;
    Bool osRc = TRUE;

    CmdProcessor_Cmd_T cmd;

    if (NULL == cmdProcessor)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else if (NULL == cmdProcessor->queue)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    /* Receive queue element */
    osRc = Mailbox_pend((Mailbox_Handle) cmdProcessor->queue, (Ptr) & cmd, CMDPROCESSOR_DEQUEUE_WAIT_TIME);
    if (FALSE == osRc)
    {
        /*< Queue dequeue failure */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_QUEUE_DEQUEUE_FAIL);
    }

    if (RETCODE_OK == retcode)
    {
        /* Execute command */
        if (NULL != cmd.func)
        {
            cmd.func(cmd.param1, cmd.param2);
        }
        else
        {
            /* Tried to dispatch/execute a NULL pointer */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CMDPROCESSOR_EXECUTE_FAILURE);
        }
    }

    return retcode;
}

/* Deprecated function. See header file for more information */
Retcode_T CmdProcessor_initialize(CmdProcessor_T * cmdProcessor, char *name, uint32_t taskPriority, uint32_t taskStackDepth, uint32_t queueSize)
{
    return (CmdProcessor_Initialize(cmdProcessor, name, taskPriority, taskStackDepth, queueSize));
}

/* Deprecated function. See header file for more information */
Retcode_T CmdProcessor_enqueue(CmdProcessor_T * cmdProcessor, CmdProcessor_Func_T func, void *param1, uint32_t param2)
{
    return (CmdProcessor_Enqueue(cmdProcessor, func, param1, param2));
}

#endif /* if BCDS_FEATURE_CMDPROCESSOR */
