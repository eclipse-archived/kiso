/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* Modified by Robert Bosch GmbH */
/* Modification: adapt this header file to Kiso unit test header */

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
