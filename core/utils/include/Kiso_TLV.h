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
 * @defgroup TLV TLV
 * @{
 *
 * @brief
 *      TLV data handler
 *
 * @details
 *
 * @file
 **/

/* Header definition ******************************************************** */
#ifndef KISO_TLV_H_
#define KISO_TLV_H_

/* Public interface declaration ********************************************* */

#include "Kiso_Utils.h"

#if KISO_FEATURE_TLV

#include "Kiso_Basics.h"

/* Public Type and macro definitions */

struct TLV_Element_S
{
    uint8_t* DataBuffer;
    uint16_t DataType;
    uint16_t DataLength;
};

typedef struct TLV_Element_S TLV_Element_T;

typedef struct TLV_Group_S * TLV_GroupHandle_TP;

/* Public function prototype declarations */

TLV_GroupHandle_TP TLV_AddGroup(void* const Buffer, const uint16_t Size);

void TLV_RemoveGroup(TLV_GroupHandle_TP Handle);

TLV_Element_T* TLV_AddElement(TLV_GroupHandle_TP const Handle, const uint16_t Type, const uint16_t Length, const void* const Value);

TLV_Element_T* TLV_GetElement(TLV_GroupHandle_TP const Handle, const uint16_t Type);

void TLV_RemoveElement(TLV_GroupHandle_TP const Handle, const uint16_t Type);

/* Public global variable declarations */

/* Inline function definitions */

#endif /* if KISO_FEATURE_TLV */

#endif /* KISO_TLV_H_ */
/**@} */
