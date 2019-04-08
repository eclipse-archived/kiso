/**
  ******************************************************************************
  * @file    usbd_ioreq.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_ioreq.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_IOREQ_H
#define __USBD_IOREQ_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_def.h"
#include  "usbd_core.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_IOREQ
  * @brief header file for the usbd_ioreq.c file
  * @{
  */

/** @defgroup USBD_IOREQ_Exported_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_IOREQ_Exported_Types
  * @{
  */


/**
  * @}
  */



/** @defgroup USBD_IOREQ_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_IOREQ_Exported_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_IOREQ_Exported_FunctionsPrototype
  * @{
  */

USBD_StatusTypeDef  USBD_CtlSendData (USBD_HandleTypeDef *pdev,
                               uint8_t *pbuf,
                               uint16_t len);

USBD_StatusTypeDef  USBD_CtlContinueSendData (USBD_HandleTypeDef  *pdev,
                               uint8_t *pbuf,
                               uint16_t len);

USBD_StatusTypeDef USBD_CtlPrepareRx (USBD_HandleTypeDef  *pdev,
                               uint8_t *pbuf,
                               uint16_t len);

USBD_StatusTypeDef  USBD_CtlContinueRx (USBD_HandleTypeDef  *pdev,
                              uint8_t *pbuf,
                              uint16_t len);

USBD_StatusTypeDef  USBD_CtlSendStatus (USBD_HandleTypeDef  *pdev);

USBD_StatusTypeDef  USBD_CtlReceiveStatus (USBD_HandleTypeDef  *pdev);

uint32_t  USBD_GetRxCount (USBD_HandleTypeDef *pdev, uint8_t ep_addr);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_IOREQ_H */

/**
  * @}
  */

/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
