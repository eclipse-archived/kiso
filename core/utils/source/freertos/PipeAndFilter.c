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
 *
 * @brief
 * 		Pipe and filter Interface Implementation
 *
 * @details
 * 		This source file implements following features:
 * 		- RunFilter()
 * 		- PipeAndFilter_CreatePipe()
 * 		- PipeAndFilter_CreateFilter()
 * 		- PipeAndFilter_FillPipe()
 * 		- PipeAndFilter_FillPipeFromISR()
 * 
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_PIPEANDFILTER

/* Include Kiso_PipeAndFilter interface header */
#include "Kiso_PipeAndFilter.h"

#if KISO_FEATURE_PIPEANDFILTER

/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "message_buffer.h"
#include "task.h"

/* Will be used in unittest to be able to test the FreeRTOS task */
#ifndef RUN_FILTER_ALWAYS
#define RUN_FILTER_ALWAYS 1
#endif

/**
 * @brief
 * 		function that will be used as parameter for a freeRTOS task.
 * @param [in] void * pvParameters
 * 		Will contain the pipe context to be executed in the FreeRTOS task.
 */
void RunFilter(void *pvParameters);

void RunFilter(void *pvParameters)
{
    Retcode_T retcode = RETCODE_OK;
    size_t xReceivedBytes = 0;
    uint32_t xToSendBytes = 0;
    size_t xSendBytes = 0;
    uint8_t xRxBuffer[PIPE_SIZE] = {0};
    uint8_t xTxBuffer[PIPE_SIZE] = {0};

    PipeAndFilter_Filter_T *filterContext = (PipeAndFilter_Filter_T *)(pvParameters);

    if (filterContext == NULL || filterContext->filterFunction == NULL)
    {
        // NULL parameters was given!
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NULL_POINTER);
        Retcode_RaiseError(retcode);
        return;
    }

    do
    {
        // Clean variables
        retcode = RETCODE_OK;
        xReceivedBytes = 0;
        xToSendBytes = 0;

        // Get input if exist
        if (filterContext->pipeInHandle != NULL)
        {
            xReceivedBytes = xMessageBufferReceive((MessageBufferHandle_t)filterContext->pipeInHandle->pipeInternalHandle, (void *)xRxBuffer, PIPE_SIZE, portMAX_DELAY);

            if (xReceivedBytes == 0)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }
        }

        // Process input
        if (retcode == RETCODE_OK)
        {
            retcode = filterContext->filterFunction(xRxBuffer, xReceivedBytes, xTxBuffer, &xToSendBytes);
        }

        // Check filter output
        if (xToSendBytes > PIPE_SIZE)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }

        // Send output
        if (filterContext->pipeOutHandle != NULL && xToSendBytes != 0 && retcode == RETCODE_OK)
        {
            xSendBytes = xMessageBufferSend((MessageBufferHandle_t)filterContext->pipeOutHandle->pipeInternalHandle, (void *)xTxBuffer, xToSendBytes, portMAX_DELAY);

            if (xSendBytes != xToSendBytes)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
            }
        }

        // Raise error if occurred
        if (retcode != RETCODE_OK)
        {
            Retcode_RaiseError(retcode);
        }
    } while (RUN_FILTER_ALWAYS);
}

/*  The description of the function is available in Kiso_PipeAndFilter.h */
Retcode_T PipeAndFilter_CreatePipe(PipeAndFilter_Pipe_T *pipeHandle)
{
    Retcode_T retcode = RETCODE_OK;
    MessageBufferHandle_t xMessageBuffer;

    // Create stream-buffer
    xMessageBuffer = xMessageBufferCreate((size_t)PIPE_SIZE);

    if (xMessageBuffer == NULL)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    // Exit
    pipeHandle->pipeInternalHandle = (PipeAndFilter_PipeInternalHandle_T)xMessageBuffer;
    pipeHandle->filterInternalHandle = NULL;
    return retcode;
}

/*  The description of the function is available in Kiso_PipeAndFilter.h */
Retcode_T PipeAndFilter_CreateFilter(PipeAndFilter_FilterFunction_T filterFunction, PipeAndFilter_Pipe_T *pipeInHandle, PipeAndFilter_Pipe_T *pipeOutHandle, PipeAndFilter_Filter_T *filterHandle)
{
    Retcode_T retcode = RETCODE_OK;
    TaskHandle_t xHandle = NULL;

    // Verify Input parameters
    if (filterHandle == NULL) // We need the structure to be valid
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        // Fill filter internal structure
        filterHandle->filterFunction = filterFunction;
        filterHandle->pipeInHandle = pipeInHandle;
        filterHandle->pipeOutHandle = pipeOutHandle;

        (void)xTaskCreate(RunFilter, "Filter", FILTER_STACK_SIZE, filterHandle, FILTER_PRIORITY, &xHandle);

        if (xHandle == NULL)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }

        // Exit
        filterHandle->filterInternalHandle = (PipeAndFilter_FilterInternalHandle_T)xHandle;
        if (filterHandle->pipeInHandle != NULL)
        {
            filterHandle->pipeInHandle->filterInternalHandle = (PipeAndFilter_FilterInternalHandle_T)xHandle; // We link the internal filter to the input pipe. Needed later if we want to fill a pipe from an ISR.
        }
    }

    return retcode;
}

/*  The description of the function is available in Kiso_PipeAndFilter.h */
Retcode_T PipeAndFilter_FillPipe(PipeAndFilter_Pipe_T pipe, uint8_t *xTxBuffer, uint32_t xToSendBytes)
{
    Retcode_T retcode = RETCODE_OK;
    size_t xSendBytes = 0;

    if (pipe.pipeInternalHandle == NULL || xTxBuffer == NULL)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (retcode == RETCODE_OK)
    {
        xSendBytes = xMessageBufferSend((MessageBufferHandle_t)pipe.pipeInternalHandle, (void *)xTxBuffer, xToSendBytes, 0);
    }

    if (xSendBytes != xToSendBytes)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }
    return retcode;
}

/*  The description of the function is available in Kiso_PipeAndFilter.h */
Retcode_T PipeAndFilter_FillPipeFromISR(PipeAndFilter_Pipe_T pipe, uint8_t *xTxBuffer, uint32_t xToSendBytes)
{
    Retcode_T retcode = RETCODE_OK;
    size_t xSendBytes = 0;

    if (pipe.pipeInternalHandle == NULL || xTxBuffer == NULL)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (retcode == RETCODE_OK)
    {
        xSendBytes = xMessageBufferSendFromISR((MessageBufferHandle_t)pipe.pipeInternalHandle, (void *)xTxBuffer, xToSendBytes, (BaseType_t *)pipe.filterInternalHandle);
    }

    if (xSendBytes != xToSendBytes)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }
    return retcode;
}

#endif /* if KISO_FEATURE_PIPEANDFILTER */
