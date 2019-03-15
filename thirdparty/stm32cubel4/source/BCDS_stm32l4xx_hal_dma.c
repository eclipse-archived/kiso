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

#include "BCDS_Stm32CubeInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_STM32CUBE_MODULE_ID_HAL_DMA

#include "BCDS_stm32l4xx_hal_dma.h"

void BCDS_HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma, uint32_t channel)
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
