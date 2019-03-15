/**
  ******************************************************************************
  * @file    stm32l476g_eval_sd.c
  * @author  MCD Application Team
  * @version V1.0.4
  * @date    08-April-2016
  * @brief   This file includes the uSD card driver.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================  
  (#) This driver is used to drive the micro SD external card mounted on STM32L476G-EVAL 
     evaluation board.
     
  (#) This driver does not need a specific component driver for the micro SD device
     to be included with.

  (#) Initialization steps:
       (++) Initialize the micro SD card using the BSP_SD_Init() function. This 
            function includes the MSP layer hardware resources initialization and the
            SDMMC1 interface configuration to interface with the external micro SD. It 
            also includes the micro SD initialization sequence.
       (++) To check the SD card presence you can use the function BSP_SD_IsDetected() which 
            returns the detection status 
       (++) If SD presence detection interrupt mode is desired, you must configure the 
            SD detection interrupt mode by calling the function BSP_SD_ITConfig(). The interrupt 
            is generated as an external interrupt whenever the micro SD card is 
            plugged/unplugged in/from the evaluation board. The SD detection interrupt
            is handeled by calling the function BSP_SD_DetectIT() which is called in the IRQ
            handler file, the user callback is implemented in the function BSP_SD_DetectCallback().
       (++) The function BSP_SD_GetCardInfo() is used to get the micro SD card information 
            which is stored in the structure "HAL_SD_CardInfoTypedef".
  
  (#) Micro SD card operations
       (++) The micro SD card can be accessed with read/write block(s) operations once 
            it is reay for access. The access cand be performed whether using the polling 
            mode by calling the functions BSP_SD_ReadBlocks()/BSP_SD_WriteBlocks(), or by DMA 
            transfer using the functions BSP_SD_ReadBlocks_DMA()/BSP_SD_WriteBlocks_DMA()
       (++) The DMA transfer complete is used with interrupt mode. Once the SD transfer
            is complete, the SD interrupt is handeled using the function BSP_SD_IRQHandler(),
            the DMA Tx/Rx transfer complete are handeled using the functions
            BSP_SD_DMA_Tx_IRQHandler()/BSP_SD_DMA_Rx_IRQHandler(). The corresponding user callbacks 
            are implemented by the user at application level. 
       (++) The SD erase block(s) is performed using the function BSP_SD_Erase() with specifying
            the number of blocks to erase.
       (++) The SD runtime status is returned when calling the function BSP_SD_GetStatus().
   [..] 
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l476g_eval_sd.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L476G_EVAL
  * @{
  */

/** @defgroup STM32L476G_EVAL_SD STM32L476G_EVAL SD
  * @{
  */

/* Private variables ---------------------------------------------------------*/

/** @defgroup STM32L476G_SD_Private_Variables Private Variables
  * @{
  */
SD_HandleTypeDef uSdHandle;
static SD_CardInfo uSdCardInfo;
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/** @defgroup STM32L476G_EVAL_SD_Private_Functions Private Functions
  * @{
  */
static void SD_MspInit(void);
HAL_SD_ErrorTypedef SD_DMAConfigRx(SD_HandleTypeDef *hsd);
HAL_SD_ErrorTypedef SD_DMAConfigTx(SD_HandleTypeDef *hsd);

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @addtogroup STM32L476G_EVAL_SD_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the SD card device.
  * @retval SD status.
  */
uint8_t BSP_SD_Init(void)
{ 
  uint8_t state = MSD_OK;
  
  /* uSD device interface configuration */
  uSdHandle.Instance = SDMMC1;

  uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  uSdHandle.Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
  uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_1B;
  uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  uSdHandle.Init.ClockDiv            = 2;
  
  /* Check if the SD card is plugged in the slot */
  SD_MspInit();
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    return MSD_ERROR;
  }
  
  /* HAL SD initialization */
  if(HAL_SD_Init(&uSdHandle, &uSdCardInfo) != SD_OK)
  {
    state = MSD_ERROR;
  }
  
  /* Configure SD Bus width */
  if(state == MSD_OK)
  {
    /* Enable wide operation */
    if(HAL_SD_WideBusOperation_Config(&uSdHandle, SDMMC_BUS_WIDE_4B) != SD_OK)
    {
      state = MSD_ERROR;
    }
    else
    {
      state = MSD_OK;
    }
  }
  
  return  state;
}

/**
  * @brief  Configures Interrupt mode for SD detection pin.
  * @retval Returns 0
  */
uint8_t BSP_SD_ITConfig(void)
{ 
  GPIO_InitTypeDef gpioinitstruct = {0};
  
  /* Configure Interrupt mode for SD detection pin */ 
  gpioinitstruct.Mode      = GPIO_MODE_IT_RISING_FALLING;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Pin       = SD_DETECT_PIN;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gpioinitstruct);
    
  /* NVIC configuration for SD detection interrupts */
  HAL_NVIC_SetPriority(SD_DETECT_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SD_DETECT_IRQn);
  
  return 0;
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
uint8_t BSP_SD_IsDetected(void)
{
  __IO uint8_t status = SD_PRESENT;

  /* Check SD card detect pin */
  if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET) 
  {
    status = SD_NOT_PRESENT;
  }
  
  return status;
}

/** @brief  SD detect IT treatment
  * @retval None
  */
void BSP_SD_DetectIT(void)
{
  /* SD detect IT callback */
  BSP_SD_DetectCallback();
  
}


/** @brief  SD detect IT detection callback
  * @retval None
  */
__weak void BSP_SD_DetectCallback(void)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
  the BSP_SD_DetectCallback could be implemented in the user file
  */ 
  
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  if(HAL_SD_ReadBlocks(&uSdHandle, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  if(HAL_SD_WriteBlocks(&uSdHandle, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t state = MSD_OK;
  
  /* Invalidate the dma tx handle*/
  uSdHandle.hdmatx = NULL;
    
  /* Prepare the dma channel for a read operation */
  state = ((SD_DMAConfigRx(&uSdHandle) == SD_OK) ? MSD_OK : MSD_ERROR);
  
  if(state == MSD_OK)
  {
    /* Read block(s) in DMA transfer mode */
    state = ((HAL_SD_ReadBlocks_DMA(&uSdHandle, pData, ReadAddr, BlockSize, NumOfBlocks) == SD_OK) ? MSD_OK : MSD_ERROR);
    
    /* Wait until transfer is complete */
    if(state == MSD_OK)
    {
      state = ((HAL_SD_CheckReadOperation(&uSdHandle, (uint32_t)SD_DATATIMEOUT) == SD_OK) ? MSD_OK : MSD_ERROR);
    }    
  }
  
  return state; 
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write 
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t state = MSD_OK;
  
  /* Invalidate the dma rx handle*/
  uSdHandle.hdmarx = NULL;
    
  /* Prepare the dma channel for a read operation */
  state = ((SD_DMAConfigTx(&uSdHandle) == SD_OK) ? MSD_OK : MSD_ERROR);
  
  if(state == MSD_OK)
  { 
    /* Write block(s) in DMA transfer mode */
    state = ((HAL_SD_WriteBlocks_DMA(&uSdHandle, pData, WriteAddr, BlockSize, NumOfBlocks) == SD_OK) ? MSD_OK : MSD_ERROR);
    
    /* Wait until transfer is complete */
    if(state == MSD_OK)
    {
      state = ((HAL_SD_CheckWriteOperation(&uSdHandle, (uint32_t)SD_DATATIMEOUT) == SD_OK) ? MSD_OK : MSD_ERROR);
    }
  }
  
  return state;  
}

/**
  * @brief  Erases the specified memory area of the given SD card. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t BSP_SD_Erase(uint64_t StartAddr, uint64_t EndAddr)
{
  if(HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Handles SD card interrupt request.
  * @retval None
  */
void BSP_SD_IRQHandler(void)
{
  HAL_SD_IRQHandler(&uSdHandle);
}

/**
  * @brief  Handles SD DMA Tx transfer interrupt request.
  * @retval None
  */
void BSP_SD_DMA_Tx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uSdHandle.hdmatx); 
}

/**
  * @brief  Handles SD DMA Rx transfer interrupt request.
  * @retval None
  */
void BSP_SD_DMA_Rx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  *            @arg  SD_TRANSFER_ERROR: Data transfer error 
  */
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void)
{
  return(HAL_SD_GetStatus(&uSdHandle));
}

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  * @retval None 
  */
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypedef *CardInfo)
{
  /* Get SD card Information */
  HAL_SD_Get_CardInfo(&uSdHandle, CardInfo);
}

/**
  * @}
  */


/** @addtogroup STM32L476G_EVAL_SD_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the SD MSP.
  * @note   The SDMMC clock configuration done within this function assumes that
  *         the PLLSAI1 input clock runs at 8 MHz.
  * @retval None
  */
static void SD_MspInit(void)
{
  GPIO_InitTypeDef gpioinitstruct = {0};
  RCC_PeriphCLKInitTypeDef  RCC_PeriphClkInit;

  HAL_RCCEx_GetPeriphCLKConfig(&RCC_PeriphClkInit);

  /* Configure the Eval SDMMC1 clock source. The clock is derived from the PLLSAI1 */
  /* Hypothesis is that PLLSAI1 VCO input is 8Mhz */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
  RCC_PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  RCC_PeriphClkInit.PLLSAI1.PLLSAI1Q = 4;
  RCC_PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  RCC_PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
  {
    while(1) {}
  }
  
  /* Enable SDMMC1 clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();
  
  /* Enable DMA2 clocks */
  __DMAx_TxRx_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __SD_DETECT_GPIO_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Alternate = GPIO_AF12_SDMMC1;
  
  /* GPIOC configuration */
  gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
   
  HAL_GPIO_Init(GPIOC, &gpioinitstruct);

  /* GPIOD configuration */
  gpioinitstruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* SD Card detect pin configuration */
  gpioinitstruct.Mode      = GPIO_MODE_INPUT;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Pin       = SD_DETECT_PIN;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gpioinitstruct);
    
  /* NVIC configuration for SDMMC1 interrupts */
  HAL_NVIC_SetPriority(SDMMC1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
  
  /* DMA initialization should be done here but , as there is only one channel for RX and TX it is configured and done directly when required*/
}

/**
  * @brief Configure the DMA to receive data from the SD card
  * @retval
  *  SD_ERROR or SD_OK
  */
HAL_SD_ErrorTypedef SD_DMAConfigRx(SD_HandleTypeDef *hsd)
{
  static DMA_HandleTypeDef hdma_rx;
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Configure DMA Rx parameters */
  hdma_rx.Init.Request             = DMA_REQUEST_7;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_rx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

  hdma_rx.Instance = DMA2_Channel4;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hsd, hdmarx, hdma_rx);

  /* Stop any ongoing transfer and reset the state*/
  HAL_DMA_Abort(&hdma_rx);
  
  /* Deinitialize the Channel for new transfer */
  HAL_DMA_DeInit(&hdma_rx);

  /* Configure the DMA Channel */
  status = HAL_DMA_Init(&hdma_rx);
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
  
  return (status != HAL_OK? SD_ERROR : SD_OK);
}

/**
  * @brief Configure the DMA to transmit data to the SD card
  * @retval
  *  SD_ERROR or SD_OK
  */
HAL_SD_ErrorTypedef SD_DMAConfigTx(SD_HandleTypeDef *hsd)
{
  static DMA_HandleTypeDef hdma_tx;
  HAL_StatusTypeDef status;
  
  /* Configure DMA Tx parameters */
  hdma_tx.Init.Request             = DMA_REQUEST_7;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_tx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  
  hdma_tx.Instance = DMA2_Channel4;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hsd, hdmatx, hdma_tx);
  
  /* Stop any ongoing transfer and reset the state*/
  HAL_DMA_Abort(&hdma_tx);
  
  /* Deinitialize the Channel for new transfer */
  HAL_DMA_DeInit(&hdma_tx);
  
  /* Configure the DMA Channel */
  status = HAL_DMA_Init(&hdma_tx); 
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);

  return (status != HAL_OK? SD_ERROR : SD_OK);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
