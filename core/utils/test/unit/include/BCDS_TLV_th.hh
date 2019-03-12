/******************************************************************************/
/**
 *  Copyright (C) Bosch Connected Devices and Solutions.
 *  All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  Mockup implementation for the BCDS_TLV module.
 *
 * ****************************************************************************/

#ifndef BCDS_TLV_TH_HH_
#define BCDS_TLV_TH_HH_

/* include system header files */

/* ban the real BCDS_TLV.h interface header */
#define BCDS_TLV_H_

struct TLV_Element_S
{
    uint8_t* DataBuffer;
    uint16_t DataType;
    uint16_t DataLength;
};

typedef struct TLV_Element_S TLV_Element_T;

typedef struct TLV_Group_S * TLV_GroupHandle_TP;

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(TLV_GroupHandle_TP,TLV_AddGroup,void*,uint16_t);
FAKE_VOID_FUNC(TLV_RemoveGroup ,TLV_GroupHandle_TP);
FAKE_VALUE_FUNC(TLV_Element_T* ,TLV_AddElement,TLV_GroupHandle_TP , uint16_t, uint16_t , void * );
FAKE_VALUE_FUNC(TLV_Element_T* ,TLV_GetElement,TLV_GroupHandle_TP ,uint16_t);
FAKE_VOID_FUNC(TLV_RemoveElement ,TLV_GroupHandle_TP,uint16_t);

#endif /* BCDS_TLV_TH_HH_ */

/** ************************************************************************* */
