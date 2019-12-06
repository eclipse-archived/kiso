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
 *
 * @file
 *
 * Mock-up implementation for the stm32f7xx_ll_dma2d module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_DMA2D_TH_HH
#define STM32F7XX_LL_DMA2D_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_DMA2D_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief LL DMA2D Init Structure Definition
  */
typedef struct
{
  uint32_t Mode;                 /*!< Specifies the DMA2D transfer mode.
                                      - This parameter can be one value of @ref DMA2D_LL_EC_MODE.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetMode().*/

  uint32_t ColorMode;            /*!< Specifies the color format of the output image.
                                      - This parameter can be one value of @ref DMA2D_LL_EC_OUTPUT_COLOR_MODE.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColorMode(). */

  uint32_t OutputBlue;           /*!< Specifies the Blue value of the output image.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if ARGB8888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if RGB888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if RGB565 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if ARGB1555 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x0F if ARGB4444 color mode is selected.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColor() or configuration
                                      function @ref LL_DMA2D_ConfigOutputColor(). */

  uint32_t OutputGreen;          /*!< Specifies the Green value of the output image.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if ARGB8888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if RGB888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x3F if RGB565 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if ARGB1555 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x0F if ARGB4444 color mode is selected.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColor() or configuration
                                      function @ref LL_DMA2D_ConfigOutputColor(). */

  uint32_t OutputRed;            /*!< Specifies the Red value of the output image.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if ARGB8888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if RGB888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if RGB565 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if ARGB1555 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x0F if ARGB4444 color mode is selected.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColor() or configuration
                                      function @ref LL_DMA2D_ConfigOutputColor(). */

  uint32_t OutputAlpha;          /*!< Specifies the Alpha channel of the output image.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if ARGB8888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x01 if ARGB1555 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x0F if ARGB4444 color mode is selected.
                                      - This parameter is not considered if RGB888 or RGB565 color mode is selected.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColor() or configuration
                                      function @ref LL_DMA2D_ConfigOutputColor(). */

  uint32_t OutputMemoryAddress;  /*!< Specifies the memory address.
                                      - This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFFFFFF.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputMemAddr(). */

  uint32_t  LineOffset;          /*!< Specifies the output line offset value.
                                      - This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetLineOffset(). */

  uint32_t NbrOfLines;           /*!< Specifies the number of lines of the area to be transferred.
                                      - This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetNbrOfLines(). */

  uint32_t NbrOfPixelsPerLines;  /*!< Specifies the number of pixels per lines of the area to be transfered.
                                      - This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetNbrOfPixelsPerLines(). */

#if defined(DMA2D_ALPHA_INV_RB_SWAP_SUPPORT)
  uint32_t AlphaInversionMode;   /*!< Specifies the output alpha inversion mode.
                                      - This parameter can be one value of @ref DMA2D_LL_EC_ALPHA_INVERSION.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputAlphaInvMode(). */

  uint32_t RBSwapMode;           /*!< Specifies the output Red Blue swap mode.
                                      - This parameter can be one value of @ref DMA2D_LL_EC_RED_BLUE_SWAP.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputRBSwapMode(). */
#endif /* DMA2D_ALPHA_INV_RB_SWAP_SUPPORT */

} LL_DMA2D_InitTypeDef;

/**
  * @brief LL DMA2D Layer Configuration Structure Definition
  */
typedef struct
{
  uint32_t MemoryAddress;        /*!< Specifies the foreground or background memory address.
                                      - This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFFFFFF.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetMemAddr() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetMemAddr() for background layer. */

  uint32_t  LineOffset;          /*!< Specifies the foreground or background line offset value.
                                      - This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetLineOffset() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetLineOffset() for background layer. */

  uint32_t ColorMode;            /*!< Specifies the foreground or background color mode.
                                      - This parameter can be one value of @ref DMA2D_LL_EC_INPUT_COLOR_MODE.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetColorMode() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetColorMode() for background layer. */

  uint32_t CLUTColorMode;        /*!< Specifies the foreground or background CLUT color mode.
                                       - This parameter can be one value of @ref DMA2D_LL_EC_CLUT_COLOR_MODE.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetCLUTColorMode() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetCLUTColorMode() for background layer. */

  uint32_t CLUTSize;             /*!< Specifies the foreground or background CLUT size.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetCLUTSize() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetCLUTSize() for background layer. */

  uint32_t AlphaMode;            /*!< Specifies the foreground or background alpha mode.
                                       - This parameter can be one value of @ref DMA2D_LL_EC_ALPHA_MODE.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetAlphaMode() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetAlphaMode() for background layer. */

  uint32_t Alpha;                /*!< Specifies the foreground or background Alpha value.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetAlpha() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetAlpha() for background layer. */

  uint32_t Blue;                 /*!< Specifies the foreground or background Blue color value.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetBlueColor() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetBlueColor() for background layer. */

  uint32_t Green;                /*!< Specifies the foreground or background Green color value.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetGreenColor() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetGreenColor() for background layer. */

  uint32_t Red;                  /*!< Specifies the foreground or background Red color value.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetRedColor() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetRedColor() for background layer. */

  uint32_t CLUTMemoryAddress;    /*!< Specifies the foreground or background CLUT memory address.
                                      - This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFFFFFF.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetCLUTMemAddr() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetCLUTMemAddr() for background layer. */

#if defined(DMA2D_ALPHA_INV_RB_SWAP_SUPPORT)
  uint32_t AlphaInversionMode;   /*!< Specifies the foreground or background alpha inversion mode.
                                      - This parameter can be one value of @ref DMA2D_LL_EC_ALPHA_INVERSION.

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetAlphaInvMode() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetAlphaInvMode() for background layer. */

  uint32_t RBSwapMode;           /*!< Specifies the foreground or background Red Blue swap mode.
                                      This parameter can be one value of @ref DMA2D_LL_EC_RED_BLUE_SWAP .

                                      This parameter can be modified afterwards using unitary functions
                                      - @ref LL_DMA2D_FGND_SetRBSwapMode() for foreground layer,
                                      - @ref LL_DMA2D_BGND_SetRBSwapMode() for background layer. */
#endif /* DMA2D_ALPHA_INV_RB_SWAP_SUPPORT */

} LL_DMA2D_LayerCfgTypeDef;

/**
  * @brief LL DMA2D Output Color Structure Definition
  */
typedef struct
{
  uint32_t ColorMode;            /*!< Specifies the color format of the output image.
                                      - This parameter can be one value of @ref DMA2D_LL_EC_OUTPUT_COLOR_MODE.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColorMode(). */

  uint32_t OutputBlue;           /*!< Specifies the Blue value of the output image.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if ARGB8888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if RGB888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if RGB565 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if ARGB1555 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x0F if ARGB4444 color mode is selected.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColor() or configuration
                                      function @ref LL_DMA2D_ConfigOutputColor(). */

  uint32_t OutputGreen;          /*!< Specifies the Green value of the output image.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if ARGB8888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if RGB888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x3F if RGB565 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if ARGB1555 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x0F if ARGB4444 color mode is selected.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColor() or configuration
                                      function @ref LL_DMA2D_ConfigOutputColor(). */

  uint32_t OutputRed;            /*!< Specifies the Red value of the output image.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if ARGB8888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if RGB888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if RGB565 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F if ARGB1555 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x0F if ARGB4444 color mode is selected.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColor() or configuration
                                      function @ref LL_DMA2D_ConfigOutputColor(). */

  uint32_t OutputAlpha;          /*!< Specifies the Alpha channel of the output image.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF if ARGB8888 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x01 if ARGB1555 color mode is selected.
                                      - This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x0F if ARGB4444 color mode is selected.
                                      - This parameter is not considered if RGB888 or RGB565 color mode is selected.

                                      This parameter can be modified afterwards using unitary function @ref LL_DMA2D_SetOutputColor() or configuration
                                      function @ref LL_DMA2D_ConfigOutputColor(). */

} LL_DMA2D_ColorTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_DMA2D_Start, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsTransferOngoing, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_Suspend, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_Resume, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsSuspended, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_Abort, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsAborted, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetOutputColorMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputColorMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetOutputRBSwapMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputRBSwapMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetOutputAlphaInvMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputAlphaInvMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetLineOffset, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetLineOffset, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetNbrOfPixelsPerLines, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetNbrOfPixelsPerLines, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetNbrOfLines, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetNbrOfLines, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetOutputMemAddr, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputMemAddr, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetOutputColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputColor, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetLineWatermark, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetLineWatermark, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_SetDeadTime, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetDeadTime, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_EnableDeadTime, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_DisableDeadTime, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsEnabledDeadTime, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetMemAddr, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetMemAddr, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_EnableCLUTLoad, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_IsEnabledCLUTLoad, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetColorMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetColorMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetAlphaMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetAlphaMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetAlpha, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetAlpha, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetRBSwapMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetRBSwapMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetAlphaInvMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetAlphaInvMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetLineOffset, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetLineOffset, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetColor, DMA2D_TypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetRedColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetRedColor, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetGreenColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetGreenColor, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetBlueColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetBlueColor, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetCLUTMemAddr, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetCLUTMemAddr, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetCLUTSize, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetCLUTSize, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_FGND_SetCLUTColorMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_FGND_GetCLUTColorMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetMemAddr, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetMemAddr, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_EnableCLUTLoad, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_IsEnabledCLUTLoad, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetColorMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetColorMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetAlphaMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetAlphaMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetAlpha, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetAlpha, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetRBSwapMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetRBSwapMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetAlphaInvMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetAlphaInvMode, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetLineOffset, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetLineOffset, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetColor, DMA2D_TypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetRedColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetRedColor, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetGreenColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetGreenColor, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetBlueColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetBlueColor, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetCLUTMemAddr, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetCLUTMemAddr, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetCLUTSize, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetCLUTSize, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_BGND_SetCLUTColorMode, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_BGND_GetCLUTColorMode, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsActiveFlag_CE, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsActiveFlag_CTC, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsActiveFlag_CAE, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsActiveFlag_TW, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsActiveFlag_TC, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsActiveFlag_TE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_ClearFlag_CE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_ClearFlag_CTC, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_ClearFlag_CAE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_ClearFlag_TW, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_ClearFlag_TC, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_ClearFlag_TE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_EnableIT_CE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_EnableIT_CTC, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_EnableIT_CAE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_EnableIT_TW, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_EnableIT_TC, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_EnableIT_TE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_DisableIT_CE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_DisableIT_CTC, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_DisableIT_CAE, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_DisableIT_TW, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_DisableIT_TC, DMA2D_TypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_DisableIT_TE, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsEnabledIT_CE, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsEnabledIT_CTC, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsEnabledIT_CAE, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsEnabledIT_TW, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsEnabledIT_TC, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_IsEnabledIT_TE, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_DMA2D_DeInit, DMA2D_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_DMA2D_Init, DMA2D_TypeDef*, LL_DMA2D_InitTypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_StructInit, LL_DMA2D_InitTypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_ConfigLayer, DMA2D_TypeDef*, LL_DMA2D_LayerCfgTypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA2D_LayerCfgStructInit, LL_DMA2D_LayerCfgTypeDef*);

FAKE_VOID_FUNC(LL_DMA2D_ConfigOutputColor, DMA2D_TypeDef*, LL_DMA2D_ColorTypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputBlueColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputGreenColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputRedColor, DMA2D_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DMA2D_GetOutputAlphaColor, DMA2D_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DMA2D_ConfigSize, DMA2D_TypeDef*, uint32_t, uint32_t);

# endif /* STM32F7XX_LL_DMA2D_TH_HH */
