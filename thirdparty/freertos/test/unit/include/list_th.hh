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

#ifndef KISO_LIST_TH_HH_
#define KISO_LIST_TH_HH_

/* ban or wrap the real interface header */
#define LIST_H

/* Include the needed headers */

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#define configLIST_VOLATILE

#define listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define listSECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define listFIRST_LIST_INTEGRITY_CHECK_VALUE
#define listSECOND_LIST_INTEGRITY_CHECK_VALUE
#define listSET_FIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE(pxItem)
#define listSET_SECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE(pxItem)
#define listSET_LIST_INTEGRITY_CHECK_1_VALUE(pxList)
#define listSET_LIST_INTEGRITY_CHECK_2_VALUE(pxList)
#define listTEST_LIST_ITEM_INTEGRITY(pxItem)
#define listTEST_LIST_INTEGRITY(pxList)

struct xLIST_ITEM
{
    listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE          /*< Set to a known value if configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
        configLIST_VOLATILE TickType_t xItemValue;     /*< The value being listed.  In most cases this is used to sort the list in descending order. */
    struct xLIST_ITEM *configLIST_VOLATILE pxNext;     /*< Pointer to the next ListItem_t in the list. */
    struct xLIST_ITEM *configLIST_VOLATILE pxPrevious; /*< Pointer to the previous ListItem_t in the list. */
    void *pvOwner;                                     /*< Pointer to the object (normally a TCB) that contains the list item.  There is therefore a two way link between the object containing the list item and the list item itself. */
    void *configLIST_VOLATILE pvContainer;             /*< Pointer to the list in which this list item is placed (if any). */
    listSECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE         /*< Set to a known value if configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
};
typedef struct xLIST_ITEM ListItem_t;

struct xMINI_LIST_ITEM
{
    listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE /*< Set to a known value if configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
        configLIST_VOLATILE TickType_t xItemValue;
    struct xLIST_ITEM *configLIST_VOLATILE pxNext;
    struct xLIST_ITEM *configLIST_VOLATILE pxPrevious;
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

typedef struct xLIST
{
    listFIRST_LIST_INTEGRITY_CHECK_VALUE /*< Set to a known value if configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
        configLIST_VOLATILE
            UBaseType_t uxNumberOfItems;
    ListItem_t *configLIST_VOLATILE pxIndex; /*< Used to walk through the list.  Points to the last item returned by a call to listGET_OWNER_OF_NEXT_ENTRY (). */
    MiniListItem_t xListEnd;                 /*< List item that contains the maximum possible item value meaning it is always at the end of the list and is therefore used as a marker. */
    listSECOND_LIST_INTEGRITY_CHECK_VALUE    /*< Set to a known value if configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
} List_t;

#endif /* KISO_LIST_TH_HH_ */
