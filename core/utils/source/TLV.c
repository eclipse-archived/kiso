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
 *
 * @brief
 *      TLV Interface Implementation
 *
 * @details
 *      This source file implements following features:
 *      - TLV_AddGroup()
 *      - TLV_RemoveGroup()
 *      - TLV_AddElement()
 *      - TLV_GetElement()
 *      - TLV_RemoveElement()
 * 
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_TLV

/* Include Kiso_TLV interface header */
#include "Kiso_TLV.h"

#if KISO_FEATURE_TLV

/* KISO basics header files */
#include "Kiso_Basics.h"
#include "Kiso_Assert.h"

/* Local Type and macro definitions */
#define GET_ELEMENT(Handle, index) \
    (&((TLV_Element_T *)((Handle)->Buffer))[index])

struct TLV_Group_S
{
    uint8_t *Buffer;
    uint16_t Limit;
    uint16_t Elements;
};

typedef struct TLV_Group_S TLV_group_t;

/* Local function prototype declarations */

static uint16_t GetAvailableMemorySize(TLV_GroupHandle_TP Handle);

/* Static assertion tests */

static_assert((sizeof(TLV_Element_T) % sizeof(uint32_t)) == 0,
              "TLV_Element_T structure must be aligned to the machine word Size for optimal memory utilization.");

static_assert(sizeof(TLV_group_t) % sizeof(uint32_t) == 0,
              "TLV_group_t structure must be aligned to the machine word Size for optimal memory utilization.");

static_assert(sizeof(TLV_group_t) % sizeof(TLV_Element_T) == 0,
              "TLV_group_t must be divisible by TLV_Element_T to make it sure that the alignment rules for the"
              "TLV Element table entries are always fulfilled and no unaligned access is attempted.");

/* Constant definitions */

/* Local variables */

/* Global variables */

/* Inline functions */

static inline uint32_t IsElementValid(TLV_Element_T *Element)
{
    return (Element->DataBuffer && Element->DataLength);
}

/* Local functions */

static uint16_t GarbageCollectGroup(TLV_GroupHandle_TP const Handle)
{
    TLV_Element_T *Element;
    uint16_t ElementTableIndex;

    /* First step is to remove all invalid Elements from the table */

    /* The Element table Size should be up to date when GarbageCollectGroup()
     * is called as GetAvailableMemorySize() calls to GetLastElement()
     * within TLV_AddElement()
     */

    /* Search for invalid Elements in table */
    for (ElementTableIndex = 0; ElementTableIndex < Handle->Elements; ElementTableIndex++)
    {
        Element = GET_ELEMENT(Handle, ElementTableIndex);

        if (!IsElementValid(Element))
        {
            TLV_Element_T *nextValidElement;
            uint16_t nextFreeSlot;

            /* Found an invalid Element, now search for next valid Element */
            nextValidElement = (TLV_Element_T *)NULL;

            for (nextFreeSlot = ElementTableIndex + 1; nextFreeSlot < Handle->Elements; nextFreeSlot++)
            {
                if (IsElementValid(GET_ELEMENT(Handle, ElementTableIndex)))
                {
                    nextValidElement = GET_ELEMENT(Handle, ElementTableIndex);
                    break;
                }
            }

            if (nextValidElement)
            {
                /* Move next valid Element to first empty slot in table */
                memcpy(Element, nextValidElement, sizeof(TLV_Element_T));

                /* Invalidate old location of Element */
                memset(nextValidElement, 0, sizeof(TLV_Element_T));
            }
            else
            {
                /* No more valid Elements found in table
                 * it is not necessary to update the Elements count here
                 * as GetLastElement() in GetAvailableMemorySize() will
                 * update it
                 */
                break;
            }
        }
    }

    /* Second step is to move all data of valid Elements to the end of the Buffer
     * filling up gaps
     */
    {
        uint8_t *WritePointer;

        WritePointer = &(Handle->Buffer[Handle->Limit]);

        for (ElementTableIndex = 0; ElementTableIndex < Handle->Elements; ElementTableIndex++)
        {
            Element = GET_ELEMENT(Handle, ElementTableIndex);

            if (IsElementValid(Element))
            {
                WritePointer = (uint8_t *)(WritePointer - (Element->DataLength));

                if (WritePointer != Element->DataBuffer)
                {
                    /* Use memmove() to move the data as it allows overlapping source and target addresses */
                    memmove(WritePointer, Element->DataBuffer, Element->DataLength);
                    Element->DataBuffer = WritePointer;
                }
            }
        }
    }

    return (GetAvailableMemorySize(Handle));
}

static TLV_Element_T *GetLastElement(TLV_GroupHandle_TP const Handle)
{
    TLV_Element_T *Result = (TLV_Element_T *)NULL;
    uint16_t ElementIndex = Handle->Elements;

    while (ElementIndex--)
    {
        TLV_Element_T *Element = GET_ELEMENT(Handle, ElementIndex);
        if (IsElementValid(Element))
        {
            Result = Element;
            break;
        }
        /* the last Element had been invalidated, decrease Elements table Size */
        Handle->Elements--;
    }

    return (Result);
}

static uint16_t GetAvailableMemorySize(TLV_GroupHandle_TP const Handle)
{
    TLV_Element_T *Element = GetLastElement(Handle);
    uint16_t AvailableMemory = 0;

    if (Element != (TLV_Element_T *)NULL)
    {
        /** \todo: Take a closer look if there is a better way - -Wpointer-to-int-cast */
        AvailableMemory = (unsigned long)Element->DataBuffer - ((unsigned long)Element + sizeof(TLV_Element_T));
    }
    else
    {
        AvailableMemory = Handle->Limit;
    }

    return AvailableMemory;
}

/* Global functions */

TLV_GroupHandle_TP TLV_AddGroup(void *const Buffer, const uint16_t Size)
{
    /* Perform plausibility tests on the input parameters */
    if ((NULL == Buffer) ||
        ((unsigned long)Buffer % sizeof(TLV_group_t) != 0) ||
        (Size <= (sizeof(TLV_group_t))))
    {
        /* Return invalid Handle to report generic failure */
        return ((TLV_GroupHandle_TP)NULL);
    }

    {
        TLV_group_t Handle;

        /* Set up group descriptor */
        Handle.Buffer = &(((uint8_t *)Buffer)[sizeof(TLV_group_t)]);
        Handle.Limit = Size - sizeof(TLV_group_t);
        Handle.Elements = UINT16_C(0);

        /* Reset Buffer */
        (void)memset(Buffer, 0, Size);

        /* Copy group descriptor to Buffer */
        (void)memcpy(Buffer, &Handle, sizeof(TLV_group_t));
    }

    /* Return newly created group Handle */
    return ((TLV_GroupHandle_TP)Buffer);
}

void TLV_RemoveGroup(TLV_GroupHandle_TP Handle)
{
    /* The implementation design does not require resources to be freed up */
    (void)Handle;
}

TLV_Element_T *TLV_AddElement(TLV_GroupHandle_TP const Handle, const uint16_t Type, const uint16_t Length, const void *const Value)
{
    /* Perform plausibility tests on the input parameters */
    if (((TLV_GroupHandle_TP)NULL == Handle) ||
        (NULL == Value))
    {
        /* Return invalid Handle to report generic failure */
        return ((TLV_Element_T *)NULL);
    }

    TLV_Element_T *Element = TLV_GetElement(Handle, Type);

    /* Test whether Element already exists in the database */
    if (Element)
    {
        /* Test whether new data Length fits into already reserved space */
        if (Element->DataLength >= Length)
        {
            /* Update Element in place */
            Element->DataLength = Length;
            memcpy(Element->DataBuffer, Value, Length);
        }
        else
        {
            /* Invalidate old Element */
            TLV_RemoveElement(Handle, Type);

            /* Reset the Element variable to NULL to indicate that a new Element have to be added */
            Element = (TLV_Element_T *)NULL;
        }
    }

    /* Test whether a new Element have to be added */
    if (!Element)
    {
        uint16_t AvailableMemory, RequiredMemory;

        /* Test available memory space */
        RequiredMemory = Length + sizeof(TLV_Element_T);
        AvailableMemory = GetAvailableMemorySize(Handle);

        if (AvailableMemory < RequiredMemory)
        {
            /* Perform garbage collection on the database */
            AvailableMemory = GarbageCollectGroup(Handle);
        }

        /* Test available memory space after garbage collection */
        if (AvailableMemory < RequiredMemory)
        {
            ; /* Insufficient memory error - return NULL */
        }
        else
        {
            /* Add new Element and related data to database */
            Element = GetLastElement(Handle);

            if (Element)
            {
                /* Element database is not empty */
                TLV_Element_T *LastElement;

                LastElement = Element;
                Element = &Element[1];

                Element->DataBuffer = (uint8_t *)(&LastElement->DataBuffer[0] - Length);
            }
            else
            {
                /* Element database is empty */
                Element = (TLV_Element_T *)Handle->Buffer;
                Element->DataBuffer = (uint8_t *)(&Handle->Buffer[Handle->Limit] - Length);
            }

            Element->DataType = Type;
            Element->DataLength = Length;
            memcpy(Element->DataBuffer, Value, Length);

            /* Increment number of Elements in table */
            Handle->Elements++;
        }
    }

    return (Element);
}

TLV_Element_T *TLV_GetElement(TLV_GroupHandle_TP const Handle, const uint16_t Type)
{
    /* Perform plausibility tests on the input parameters */
    if ((TLV_GroupHandle_TP)NULL == Handle)
    {
        /* Return invalid Handle to report generic failure */
        return ((TLV_Element_T *)NULL);
    }

    {
        uint16_t ElementIndex;
        TLV_Element_T *Result = (TLV_Element_T *)NULL;

        /* Search for matching TLV Elements in the Buffer */
        for (ElementIndex = 0; ElementIndex < Handle->Elements; ElementIndex++)
        {
            TLV_Element_T *Element = GET_ELEMENT(Handle, ElementIndex);

            if (Element->DataType == Type)
            {
                Result = Element;
                break;
            }
        }

        return (Result);
    }
}

void TLV_RemoveElement(TLV_GroupHandle_TP const Handle, const uint16_t Type)
{
    TLV_Element_T *Element = TLV_GetElement(Handle, Type);

    /* Check whether requested Element exists
     * TLV_GetElement() performs plausibility check on the passed Handle
     */
    if (Element)
    {
        memset(Element, 0, sizeof(TLV_Element_T));
    }
}

#endif /* if KISO_FEATURE_TLV */
