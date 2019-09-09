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
 * @file
 * @brief Command Line Debugger Interface Implementation
 *
 * This source file implements following features:
 * - CmdLineDbg_Parse()
 * - CmdLineDbg_RegisterCmd()
 * - CmdLineDbg_RegisterCmdArray()
 *
 * @ingroup CMDLINE
 * @{
 */

#include "Kiso_Utils.h"
#ifdef KISO_MODULE_ID
#undef KISO_MODULE_ID
#endif

#define KISO_MODULE_ID  KISO_UTILS_MODULE_ID_CMD_LINE_DEBUGGER

#include "Kiso_CmdLineDebugger.h"

#if KISO_FEATURE_CMDLINEDEBUGGER

/* Default Configuration options */
#ifndef KISO_COMMAND_LINE_CUSTOM_OPTIONS
#define KISO_COMMAND_LINE_CUSTOM_OPTIONS 0
#endif

#if(0 == KISO_COMMAND_LINE_CUSTOM_OPTIONS)
#define KISO_COMMAND_LINE_SEPARATOR_TOKENS  " " /**< Default Separator Token */
#define KISO_COMMAND_LINE_MAX_PARAMETERS      16
#endif

/**
 * @brief Extract Tokens from String
 *
 * @param argv : Argument vector to extract
 *
 * @param[in] input : Input Character String
 *
 * @return size_t : Amount of tokens found. This includes the Command String
 *
 */
static size_t CmdLine_ExtractTokensFromString(char ** argv, char * input);

/**
 * @brief Find command in the command list
 *
 * @detail Find a command by its name
 *
 * @param[in] list
 * @param[in] commandString
 *
 * @return NULL : Command was not found
 * @return struct CommandLineCommand_S : Pointer to command found
 */
static struct CmdLineDbg_Element_S * CmdLine_FindCmd(struct CmdLineDbg_Element_S * list, const char * commandString);

/**
 * @brief Execute Command
 *
 * @detail Executes command callback with user-provided arguments
 *
 * @param[in] command : pointer to command
 * @param[in] paramPtr : pointer to buffer containing parameter
 * @param[in] paramLen : number of elements inside buffer
 *
 * @return RETCODE_OK : Command was executed correctly
 *
 */
static Retcode_T CmdLine_ExecuteCmd(struct CmdLineDbg_Element_S * command, const char ** paramPtr, size_t paramLen);

/**
 * Global Variable containing separator
 */
static const char CmdLine_separator[] = KISO_COMMAND_LINE_SEPARATOR_TOKENS;

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

/*  The description of the function is available in Kiso_CmdLineDebugger.h */
Retcode_T CmdLineDbg_RegisterCmd(struct CmdLineDbg_Element_S * list, struct CmdLineDbg_Element_S * cmd)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);

    if (NULL == list)
    {
        return rc;
    }

    if (NULL == cmd)
    {
        return rc;
    }

    rc = RETCODE_OK;

    while (NULL != list->next) /* Get next element in list  */
    {
        list = list->next; /* Select next element in list */
    }

    list->next = cmd; /* Add command to list */

    cmd->next = NULL; /* Terminate the list */

    return rc;
}

/*  The description of the function is available in Kiso_CmdLineDebugger.h */
Retcode_T CmdLineDbg_RegisterCmdArray(struct CmdLineDbg_Element_S * list, size_t nElements)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);

    if (NULL == list)
    {
        return rc;
    }

    if (0 == nElements)
    {
        rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    else
    {
        /* Begin with Last Element */
        list[--nElements].next = NULL; /* Terminate List */

        while (0 != nElements)
        {
            list[nElements - 1].next = &list[nElements];
            nElements--;
        }

        rc = RETCODE_OK;
    }

    return rc;
}

/*  The description of the function is available in Kiso_CmdLineDebugger.h */
Retcode_T CmdLineDbg_Parse(struct CmdLineDbg_Element_S * list, char * input)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);

    char * argv[KISO_COMMAND_LINE_MAX_PARAMETERS];
    size_t argc = 0;

    struct CmdLineDbg_Element_S * result = NULL;

    if (NULL == list)
    {
        return rc;
    }
    else if (NULL == input)
    {
        return rc;
    }
    else
    {
        rc = RETCODE_OK;
    }

    /* Tokenize the string */
    argc = CmdLine_ExtractTokensFromString(argv, input);

    if (0 != argc)
    {
        result = CmdLine_FindCmd(list, argv[0]);
    }

    if (NULL != result) /* Command */
    {
        rc = CmdLine_ExecuteCmd(result, (const char **) argv, argc);
    }
    else
    {
        rc = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CMDLINE_DEBUGGER_COMMAND_NOT_FOUND);
    }

    return rc;
}

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

/* Find Command inside the linked list */
static struct CmdLineDbg_Element_S * CmdLine_FindCmd(struct CmdLineDbg_Element_S * list, const char * commandString)
{
    struct CmdLineDbg_Element_S * command = NULL;

    while (NULL != list)
    {
        /* 1 - Testing condition */
        if (!strncmp(list->commandString, commandString, strlen(list->commandString)))
        {
            command = list;
            break; /* Command was found */
        }
        else
        {
            /* 2 - Continue to next element */
            list = list->next;
        }
    }

    return command;
}

/* Tokenize String for argv mode */
static size_t CmdLine_ExtractTokensFromString(char ** argv, char * input)
{
    size_t i = 0;

    /*  Tokenize */
    argv[i] = strtok(input, CmdLine_separator);

    while (NULL != argv[i])
    {
        argv[++i] = strtok(NULL, CmdLine_separator);
    }

    return i;
}

/* Execute the command */
static Retcode_T CmdLine_ExecuteCmd(struct CmdLineDbg_Element_S * command, const char ** paramPtr, size_t paramLen)
{
    Retcode_T rc;

    if (NULL == command) /* Step 1 - Check if the command exists */
    {
        rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else if (NULL == command->callback) /* Step 2 - Check if the callback exists */
    {
        rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        rc = command->callback((uint32_t) paramLen, paramPtr);
    }

    return rc;
}

#endif /* KISO_FEATURE_CMDLINEDEBUGGER */
/** @}*/
