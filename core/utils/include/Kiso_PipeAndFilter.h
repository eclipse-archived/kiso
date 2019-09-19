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
 *
 * @defgroup PIPEANDFILTER PipeAndFilter
 * @{
 *
 * @brief
 *     This file allows you to create a pipe and filter architecture pattern
 *
 * @details
 *     We have here an implementation of the pipe and filter architectural pattern
 *
 * @par Overview
 *     The component looks like bla...
 * @par Utilization
 *     Please refer directly to the functions to understand how to use it.
 * @par Extension
 *     You are free to propose a different implementation for this API. We will try to have per OS a specific implementation.
 *     You could also have it on your application.
 *
 * @file
 **/

#ifndef INCLUDE_KISO_PIPEANDFILTER_H_
#define INCLUDE_KISO_PIPEANDFILTER_H_

#if KISO_FEATURE_PIPEANDFILTER

#include "Kiso_Retcode.h"
#include "PipeAndFilterConfig.h"

/**
 * @brief
 *     Function pattern used to define a pipe
 * @param [in] bufferIn
 *     Pointer to the buffer that will contain the input data.
 * @param [in] bufferInSize
 *     Number of bytes received.
 * @param [out] bufferOut
 *     Pointer to the buffer that needs to contain the output data
 * @param [out] bufferOutSize
 *     Number of bytes that needs to be put in the next pipe
 * @return
 *     RETCODE_OK - If everything was fine \n
 *     OTHER - Needs to be define by the user \n
 * 
 */
typedef Retcode_T (*PipeAndFilter_FilterFunction_T) (uint8_t * bufferIn, uint32_t bufferInSize, uint8_t * bufferOut, uint32_t * bufferOutSize);

/**
 * @brief
 *     Will contain a pointer to an internal pipe structure.
 *     In case of Carbon, it will contain a stream-buffer handle.
 */
typedef void * PipeAndFilter_PipeInternalHandle_T;

/**
 * @brief
 *     Will contain a pointer to an internal filter handle structure.
 *     In case of Carbon, it will contain a task handle.
 */
typedef void * PipeAndFilter_FilterInternalHandle_T;


/**
 * @brief
 *     Structure used to link the first pipe to the next filter.
 * @warning
 *     Do not modify it yourself! The creational APIs will do it for you.
 */
typedef struct PipeAndFilter_Pipe_S
{
	PipeAndFilter_FilterInternalHandle_T filterInternalHandle; /**< link to the receiver handle */
	PipeAndFilter_PipeInternalHandle_T pipeInternalHandle; /**< link to the pipe handle */
}PipeAndFilter_Pipe_T;

/**
 * @brief
 *     Structure containing the needed elements for the filter.
 * @warning
 *     Do not modify it yourself! The creational APIs will do it for you.
 */
typedef struct PipeAndFilter_Filter_S
{
	PipeAndFilter_FilterFunction_T filterFunction; /**< filter function to call */
	PipeAndFilter_Pipe_T * pipeInHandle; /**< Pipe as input */
	PipeAndFilter_Pipe_T * pipeOutHandle; /**< Pipe as output */
	PipeAndFilter_FilterInternalHandle_T filterInternalHandle; /**< Internal handle */
} PipeAndFilter_Filter_T;


/******** Creational *********/
/**
 * @brief
 *     Create a pipe.
 * @param [out] pipeHandle
 *     Handle to the pipe.
 * @return
 *     RETCODE_OK - Success \n
 *     RETCODE_OUT_OF_RESOURCES - FreeRTOS heap does not have enough space to create the resource \n
 *
 * @note
 *     The size of the pipe created is defined in the configuration "PipeAndFilterCfg.h" to be consistent for the entire system.
 *
 * @code
 *     PipeAndFilter_Pipe_T pipe1;
 *
 *     Retcode_T init(void)
 *     {
 *         Retcode_T retcode = RETCODE_OK;
 *         retcode = PipeAndFilter_CreatePipe(&pipe1);
 *         return retcode;
 *     }
 * @endcode
 *
 */
Retcode_T PipeAndFilter_CreatePipe(PipeAndFilter_Pipe_T * pipeHandle);

/**
 * @brief
 *     Create a filter.
 * @param [in] function
 *     Filter function that needs to be called if the input pipe was filled with data.
 * @param [in] pipeInHandle
 *     Handle of the input pipe.
 *     In the case of no input pipe, this can be set to NULL. In this case, the `function` will behave like a while loop.
 * @param [in] pipeOutHandle
 *     Handle of the input pipe.
 *     In the case of no output pipe, this can be set to NULL. In this case, the `function` will wait for another input.
 * @param [out] filterHandle
 *     Handle of the filter.
 * @return
 *    RETCODE_OK - Success \n
 *    RETCODE_OUT_OF_RESOURCES - FreeRTOS heap does not have enough space to create the resource \n
 *
 * @note
 *     The structure allows today only to have one pipe as input and one pipe as output.
 *     Multiple pipes as input or outputs are not allowed.
 * @warning
 *     Please note modifying the content of filterHandle after initialization may generate inconsistencies in the behavior!
 *
 * @code
 *    PipeAndFilter_Pipe_T pipe1;
 *    PipeAndFilter_Pipe_T pipe2;
 *    PipeAndFilter_Pipe_T pipe3;
 *
 *    PipeAndFilter_Filter_T filter1;
 *    PipeAndFilter_Filter_T filter2;
 *
 *    Retcode_T filterfunction1 (uint8_t * bufferIn, uint32_t bufferInSize, uint8_t * bufferOut, uint32_t * bufferOutSize)
 *    {
 *        // Execute something
 *    }
 *
 *    Retcode_T filterfunction2 (uint8_t * bufferIn, uint32_t bufferInSize, uint8_t * bufferOut, uint32_t * bufferOutSize)
 *    {
 *        // Execute something
 *    }
 *
 *     Retcode_T init(void)
 *    {
 *        Retcode_T retcode = RETCODE_OK;
 *
 *        (void)PipeAndFilter_CreatePipe(&pipe1);
 *        (void)PipeAndFilter_CreatePipe(&pipe2);
 *        (void)PipeAndFilter_CreatePipe(&pipe3);
 *
 *        (void)PipeAndFilter_createFilter((PipeAndFilter_FilterFunction_T)filterfunction1, &pipe1, &pipe2, &filter1);
 *        (void)PipeAndFilter_createFilter((PipeAndFilter_FilterFunction_T)filterfunction2, &pipe2, &pipe3, &filter2);
 *        return retcode;
 *    }
 * @endcode
 * 
 */
Retcode_T PipeAndFilter_CreateFilter(PipeAndFilter_FilterFunction_T filterFunction, PipeAndFilter_Pipe_T * pipeInHandle, PipeAndFilter_Pipe_T * pipeOutHandle, PipeAndFilter_Filter_T * filterHandle);

/******** Behavioral *********/
/**
 * @brief
 *     Fill a pipe. It should be used to fill the first pipe.
 * @param [in] pipeHandle
 * @param [in] xTxBuffer
 * @param [in] xToSendBytes
 * @return
 *     RETCODE_OK - Success \n
 *     RETCODE_NULL_POINTER - One of the inputs where NULL. \n
 *     RETCODE_OUT_OF_RESOURCES - If the initialization was done, it means that the pipe is full. \n
 *
 * @note
 *     This function should be called from a non-ISR context.
 *
 * @code
 *     PipeAndFilter_Pipe_T pipe1;
 *
 *     void function(void)
 *     {
 *         uint8_t buffer[10] = {0};
 *         (void)PipeAndFilter_FillPipe(pipe1, buffer, 10);
 *     }
 * @endcode
 * 
 */
Retcode_T PipeAndFilter_FillPipe(PipeAndFilter_Pipe_T pipe, uint8_t * xTxBuffer, uint32_t xToSendBytes);

/**
 * @brief
 *     Fill a pipe. It should be used to fill the first pipe.
 * @param [in] pipeHandle
 * @param [in] xTxBuffer
 * @param [in] xToSendBytes
 * @return
 *     RETCODE_OK - Success \n
 *     RETCODE_NULL_POINTER - One of the inputs where NULL. \n
 *     RETCODE_OUT_OF_RESOURCES - If the initialization was done, it means that the pipe is full. \n
 *
 * @note
 *     This function should be called from a ISR context.
 *
 * @code
 *     PipeAndFilter_Pipe_T pipe1;
 *
 *     void Isr_function(void)
 *     {
 *         uint8_t buffer[10] = {0};
 *         (void)PipeAndFilter_FillPipeFromISR(pipe1, buffer, 10);
 *     }
 * @endcode
 * 
 */
Retcode_T PipeAndFilter_FillPipeFromISR(PipeAndFilter_Pipe_T pipe, uint8_t * xTxBuffer, uint32_t xToSendBytes);

#endif /* if KISO_FEATURE_PIPEANDFILTER */

#endif /* INCLUDE_KISO_PIPEANDFILTER_H_ */
/**@} */
