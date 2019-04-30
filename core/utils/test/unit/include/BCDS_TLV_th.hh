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
 * @file
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
