/*----------------------------------------------------------------------------*/
/*
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 */
/*----------------------------------------------------------------------------*/

/**
 * @ingroup UTILS
 *
 * @defgroup TLV TLV
 * @{
 *
 * @brief TLV data handler
 *
 * @details
 *
 * @file
 **/

/* header definition ******************************************************** */
#ifndef BCDS_TLV_H_
#define BCDS_TLV_H_

/* public interface declaration ********************************************* */

#include "BCDS_Utils.h"

#if BCDS_FEATURE_TLV

#include "BCDS_Basics.h"

/* public Type and macro definitions */

struct TLV_Element_S
{
    uint8_t* DataBuffer;
    uint16_t DataType;
    uint16_t DataLength;
};

typedef struct TLV_Element_S TLV_Element_T;

typedef struct TLV_Group_S * TLV_GroupHandle_TP;

/* public function prototype declarations */

TLV_GroupHandle_TP TLV_AddGroup(void* const Buffer, const uint16_t Size);

void TLV_RemoveGroup(TLV_GroupHandle_TP Handle);

TLV_Element_T* TLV_AddElement(TLV_GroupHandle_TP const Handle, const uint16_t Type, const uint16_t Length, const void* const Value);

TLV_Element_T* TLV_GetElement(TLV_GroupHandle_TP const Handle, const uint16_t Type);

void TLV_RemoveElement(TLV_GroupHandle_TP const Handle, const uint16_t Type);

/* public global variable declarations */

/* inline function definitions */

#endif /* if BCDS_FEATURE_TLV */

#endif /* BCDS_TLV_H_ */
/**@} */
