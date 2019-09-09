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
 * @ingroup UTILS
 * @defgroup CMDLINEDEBUGGER CmdLineDebugger
 * @{
 *
 * @brief Command Line Debug Interface Header
 *
 * @details Declaration of a command ```"example"``` and its corresponding callback:
 * @code{c}
 *
 * Retcode_T ExampleCommand(const char ** argv, uint32_t argc);
 *
 *
 * struct CmdLineDbg_Element_S help = {
 *      .callback = ExampleCommand,
 *      .commandString = (char *)"example",
 *      .next = NULL
 *      };
 *
 *
 *
 *   // ExampleCommand Callback implementation
 * Retcode_T ExampleCommand(const char ** argv, uint32_t argc)
 * {
 *      Retcode_T rc = RETCODE_OK;
 *
 *      // ... //
 *
 *      return rc;
 * }
 * @endcode
 *
 * Parser invocation:
 *
 * @code{c}
 *
 * struct CmdLineDbg_Element_S testList[] =
 *       {
 *               {
 *                       testCallback,
 *                       "TestCommand",
 *                       NULL
 *               },
 *               {
 *                      sudoCallback,
 *                      "sudo",
 *                      NULL
 *               },
 *              {
 *                      helpCallback,
 *                      "help",
 *                      NULL
 *              }
 *      };
 *
 *
 * void Loop(void)
 * {
 *      Retcode_T rc = RETCODE_OK;
 *
 *      // Initialize Command Array
 *      // We already know that there are only three elements in the array
 *      rc = CmdLineDbg_RegisterCmdArray(testList, 3);
 *
 *      char inputBuffer[80]; // Example character buffer
 *      size_t inputStrLen;
 *
 *      while(RETCODE_OK == rc)
 *      {
 *         // GetStringFromPeripheral(inputBuffer, 80);
 *
 *          rc = CmdLineDbg_Parse(testList, inputBuffer);
 *      }
 *
 *
 * }
 *
 * @endcode
 *
 * @file
 **/

#ifndef KISO_CMDLINEDEBUGGER_H
#define KISO_CMDLINEDEBUGGER_H

#include "Kiso_Utils.h"

#if KISO_FEATURE_CMDLINEDEBUGGER

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"


/**
 * Command Line Callback Definition
 *
 * @param[in] argc : Argument count. The amount of parameters inside the argument vector
 * @param[in] argv : Argument vector
 */
typedef Retcode_T (*CmdLineDbg_Callback_T)(uint32_t argc, const char ** argv);

/**
 * @brief Command Line List Element
 *
 * @details The CmdLineDbg_Element_S structure defines
 *          the basic structure of a command line list element
 */
struct CmdLineDbg_Element_S
{
    const CmdLineDbg_Callback_T callback; /**< callback to execute */
    const char * commandString; /**< command string for match */
    struct CmdLineDbg_Element_S * next; /**< next element for linked-listing */
};

/**
 * @brief Basic Command Line Parser
 *
 * @details ComdLineDbg_Parse takes a string inside a input buffer
 *           and matches this string against a list of commands registered
 *           inside a command list. For a successful match,
 *           the corresponding callback is registered
 *
 *           The input buffer must be a C-Style string
 *
 * @param[in] list  - Command List Register
 * @param[in] input - Pointer to input buffer
 *
 * @retval #RETCODE_OK when successful
 * @retval #RETCODE_NULL_POINTER when any of the input parameter is NULL
 * @retval #RETCODE_CMDLINE_DEBUGGER_COMMAND_NOT_FOUND when the command is not found
 */
Retcode_T CmdLineDbg_Parse(struct CmdLineDbg_Element_S * list, char * input);

/**
 * @brief Register Command Into List
 *
 * @details CmdLineDbg_RegisterCmd  takes registers a command into the command list
 *
 * @param[in] list  - Command List Register
 * @param[in] cmd   - Pointer to command to be registered
 *
 * @retval #RETCODE_OK when successful
 * @retval #RETCODE_NULL_POINTER when any of the input parameter is NULL
 */
Retcode_T CmdLineDbg_RegisterCmd(struct CmdLineDbg_Element_S * list, struct CmdLineDbg_Element_S * cmd);

/**
 * @brief Convert Command Element Array into Linked List
 *
 * @details CmdLineDbg_RegisterCmdArray takes an array of command
 *          elements and converts it into a linked list that can be registered
 *
 * @param[in] list      - array to be converted into list
 * @param[in] nElements - Amount of elements stored in the array
 *
 * @retval #RETCODE_OK when successful
 * @retval #RETCODE_NULL_POINTER when the list pointer is NULL
 * @retval #RETCODE_INVALID_PARAM when the amount of elements is equal to 0
 */
Retcode_T CmdLineDbg_RegisterCmdArray(struct CmdLineDbg_Element_S * list, size_t nElements);

#endif /* if KISO_FEATURE_CMDLINEDEBUGGER */

#endif /* KISO_CommandLineInterface_H */
/** @} */
