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

#include "Kiso_stm32l4xx_hal_dma.h"

void KISO_HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma, uint32_t channel)
{
    uint32_t tcflag = DMA_FLAG_TC1 << (4 * (channel - 1));
    uint32_t htflag = tcflag << 1;
    uint32_t teflag = tcflag << 2;

    /* Transfer Error Interrupt management ***************************************/
    if (RESET != (hdma->DmaBaseAddress->ISR & teflag))
    {
        if (RESET != __HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TE))
        {
            /* Disable the transfer error interrupt */
            __HAL_DMA_DISABLE_IT(hdma, DMA_IT_TE);

            /* Clear the transfer error flag */
            hdma->DmaBaseAddress->IFCR |= teflag;

            /* Update error code */
            hdma->ErrorCode |= HAL_DMA_ERROR_TE;

            /* Change the DMA state */
            hdma->State = HAL_DMA_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK(hdma);

            if (NULL != hdma->XferErrorCallback)
            {
                /* Transfer error callback */
                hdma->XferErrorCallback(hdma);
            }
        }
    }

    /* Half Transfer Complete Interrupt management ******************************/
    if (RESET != (hdma->DmaBaseAddress->ISR & htflag))
    {
        if (RESET != __HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_HT))
        {
            /* Disable the half transfer interrupt if the DMA mode is not CIRCULAR */
            if (0 == (hdma->Instance->CCR & DMA_CCR_CIRC))
            {
                /* Disable the half transfer interrupt */
                __HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
            }
            /* Clear the half transfer complete flag */
            hdma->DmaBaseAddress->IFCR |= htflag;

          /* DMA peripheral state is not updated in Half Transfer */
          /* but in Transfer Complete case */

            if (NULL != hdma->XferHalfCpltCallback)
            {
                /* Half transfer callback */
                hdma->XferHalfCpltCallback(hdma);
            }
        }
    }

    /* Transfer Complete Interrupt management ***********************************/
    if (RESET != (hdma->DmaBaseAddress->ISR & tcflag))
    {
        if (RESET != __HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC))
        {
            if (0 == (hdma->Instance->CCR & DMA_CCR_CIRC))
            {
                /* Disable the transfer complete interrupt */
                __HAL_DMA_DISABLE_IT(hdma, DMA_IT_TC);
            }
            /* Clear the transfer complete flag */
            hdma->DmaBaseAddress->IFCR |= tcflag;

            /* Update error code */
            hdma->ErrorCode |= HAL_DMA_ERROR_NONE;

            /* Change the DMA state */
            hdma->State = HAL_DMA_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK(hdma);

            if (NULL != hdma->XferCpltCallback)
            {
                /* Transfer complete callback */
                hdma->XferCpltCallback(hdma);
            }
        }
    }
}
