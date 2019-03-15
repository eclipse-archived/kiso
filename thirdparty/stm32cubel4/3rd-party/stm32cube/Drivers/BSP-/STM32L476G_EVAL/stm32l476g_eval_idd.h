 /**
  ******************************************************************************
  * @file    stm32l476g_eval_idd.h
  * @author  MCD Application Team
  * @version V1.0.4
  * @date    08-April-2016
  * @brief   Header file for stm32l476g_eval_idd.c module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L476G_EVAL_IDD_H
#define __STM32L476G_EVAL_IDD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l476g_eval.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L476G_EVAL
  * @{
  */

/** @addtogroup STM32L476G_EVAL_IDD
  * @{
  */

/** @defgroup STM32L476G_EVAL_IDD_Exported_Types  Exported Types
  * @{
  */

/** @defgroup IDD_Config  IDD Config
  * @{
  */
typedef enum 
{
  BSP_IDD_OK = 0,
  BSP_IDD_TIMEOUT = 1,
  BSP_IDD_ERROR = 0xFF
} 
IDD_StatusTypeDef;

typedef enum
{
  BSP_IDD_MODE_CALIBRATION = 0, 
  BSP_IDD_MODE_RUN,
  BSP_IDD_MODE_SLEEP,
  BSP_IDD_MODE_STOP,
  BSP_IDD_MODE_STANDBY,
  
}IDD_ModeTypdef;

/**
  * @}
  */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/ 
/** @defgroup STM32L476G_EVAL_IDD_Exported_Constants Exported Constants
  * @{
  */
/**
  * @brief  Shunt values on evaluation board in ohms
  */
#define EVAL_IDD_SHUNT0_VALUE                  ((uint16_t) 1000)     /*!< value in ohm */
#define EVAL_IDD_SHUNT1_VALUE                  ((uint16_t) 1)        /*!< value in ohm */

/**
  * @brief  IDD Ampli Gain on evaluation board 
  */
#define EVAL_IDD_AMPLI_GAIN                    ((uint16_t) 50)      /*!< value is gain */

/**
  * @brief  IDD Full Scale Measure 
  */
#define EVAL_IDD_FULL_SCALE                    ((uint16_t) 4096)     /*!< value is adc max value */

/**
  * @brief  IDD Vdd Min on eval 
  */
#define EVAL_IDD_VDD                           ((uint16_t) 3300)     /*!< value in millivolt */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup STM32L476G_EVAL_QSPI_Exported_Functions Exported Functions
  * @{
  */
uint8_t   BSP_IDD_Init(void);
void      BSP_IDD_EnterStopMode1(void);
void      BSP_IDD_EnterStopMode2(void);
void      BSP_IDD_ExitStopMode(void);
void      BSP_IDD_EnterSleepMode(void);
void      BSP_IDD_ExitSleepMode(void);
void      BSP_IDD_EnterRunMode(void);
void      BSP_IDD_EnterStandbyMode(void);
uint32_t  BSP_IDD_GetValue(IDD_ModeTypdef LowPowerMode, uint32_t Calibration, uint32_t *IddValue);

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

#ifdef __cplusplus
}
#endif

#endif /* __STM32L476G_EVAL_IDD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
