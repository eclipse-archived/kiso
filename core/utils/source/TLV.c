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


/* module includes ********************************************************** */

/* own header files */
#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_TLV

#include "BCDS_TLV.h"

#if BCDS_FEATURE_TLV

/* system header files */
#include "BCDS_Basics.h"
#include "BCDS_Assert.h"

/* additional interface header files */

/* local Type and macro definitions */
#define GET_ELEMENT(Handle, index) \
    (&((TLV_Element_T*)((Handle)->Buffer))[index])

struct TLV_Group_S
{
    uint8_t* Buffer;
    uint16_t Limit;
    uint16_t Elements;
};

typedef struct TLV_Group_S TLV_group_t;

/* local function prototype declarations */

static uint16_t GetAvailableMemorySize(TLV_GroupHandle_TP const Handle);

/* static assertion tests *************************************************** */
/*lint -save -e514 */
/*lint -save -e19 */
static_assert((sizeof(TLV_Element_T) % sizeof(uint32_t)) == 0,
        "TLV_Element_T structure must be aligned to the machine word Size for optimal memory utilization.")

static_assert(sizeof(TLV_group_t) % sizeof(uint32_t) == 0,
        "TLV_group_t structure must be aligned to the machine word Size for optimal memory utilization.")

static_assert(sizeof(TLV_group_t) % sizeof(TLV_Element_T) == 0,
        "TLV_group_t must be divisible by TLV_Element_T to make it sure that the alignment rules for the"
        "TLV Element table entries are always fulfilled and no unaligned access is attempted.")

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

static inline uint32_t IsElementValid(TLV_Element_T* Element)
{
    return (Element->DataBuffer && Element->DataLength);
}

/* local functions ********************************************************** */

static uint16_t GarbageCollectGroup(TLV_GroupHandle_TP const Handle)
{
    TLV_Element_T* Element;
    uint16_t ElementTableIndex;

    /* first step is to remove all invalid Elements from the table */

    /* the Element table Size should be up to date when GarbageCollectGroup()
     * is called as GetAvailableMemorySize() calls to GetLastElement()
     * within TLV_AddElement()
     */

    /* search for invalid Elements in table */
    for (ElementTableIndex = 0; ElementTableIndex < Handle->Elements; ElementTableIndex++)
    {
        Element = GET_ELEMENT(Handle, ElementTableIndex);

        if (!IsElementValid(Element))
        {
            TLV_Element_T* nextValidElement;
            uint16_t nextFreeSlot;

            /* found an invalid Element, now search for next valid Element */
            nextValidElement = (TLV_Element_T*) NULL;

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
                /* move next valid Element to first empty slot in table */
                memcpy(Element, nextValidElement, sizeof(TLV_Element_T));

                /* invalidate old location of Element */
                memset(nextValidElement, 0, sizeof(TLV_Element_T));
            }
            else
            {
                /* no more valid Elements found in table
                 * it is not necessary to update the Elements count here
                 * as GetLastElement() in GetAvailableMemorySize() will
                 * update it
                 */
                break;
            }
        }
    }

    /* second step is to move all data of valid Elements to the end of the Buffer
     * filling up gaps
     */
    {
        uint8_t* WritePointer;

        WritePointer = &(Handle->Buffer[Handle->Limit]);

        for (ElementTableIndex = 0; ElementTableIndex < Handle->Elements; ElementTableIndex++)
        {
            Element = GET_ELEMENT(Handle, ElementTableIndex);

            if (IsElementValid(Element))
            {
                WritePointer = (uint8_t*)(WritePointer - (Element->DataLength));

                if (WritePointer != Element->DataBuffer)
                {
                    /* use memmove() to move the data as it allows overlapping source and target addresses */
                    memmove(WritePointer, Element->DataBuffer, Element->DataLength);
                    Element->DataBuffer = WritePointer;
                }
            }
        }
    }

    return (GetAvailableMemorySize(Handle));
}

static TLV_Element_T* GetLastElement(TLV_GroupHandle_TP const Handle)
{
    TLV_Element_T* Result = (TLV_Element_T*) NULL;
    uint16_t ElementIndex = Handle->Elements;

    while (ElementIndex--)
    {
        TLV_Element_T* Element = GET_ELEMENT(Handle, ElementIndex);
        if (IsElementValid(Element))
        {
            Result = Element;
            break;
        }
        else
        {
            /* the last Element had been invalidated, decrease Elements table Size */
            Handle->Elements--;
        }
    }

    return (Result);
}

static uint16_t GetAvailableMemorySize(TLV_GroupHandle_TP const Handle)
{
    TLV_Element_T* Element = GetLastElement(Handle);
    uint32_t AvailableMemory;

    if (Element != (TLV_Element_T*) NULL)
    {
        AvailableMemory = ((uint32_t) Element->DataBuffer) - (((uint32_t)Element) + (uint32_t)sizeof(TLV_Element_T));
    }
    else
    {
        AvailableMemory = Handle->Limit;
    }

    return (uint16_t)(AvailableMemory); // Not ok fix... Function need to be revisited
}

/* global functions ********************************************************* */

TLV_GroupHandle_TP TLV_AddGroup(void* const Buffer, const uint16_t Size)
{
    /* perform plausibility tests on the input parameters */
    if ((NULL == Buffer) ||
            ((uint32_t) Buffer % sizeof(TLV_group_t) != 0) ||
            (Size <= (sizeof(TLV_group_t))))
    {
        /* return invalid Handle to report generic failure */
        return ((TLV_GroupHandle_TP) NULL);
    }

    {
        TLV_group_t Handle;

        /* set up group descriptor */
        Handle.Buffer = &(((uint8_t*) Buffer)[sizeof(TLV_group_t)]);
        Handle.Limit = Size - sizeof(TLV_group_t);
        Handle.Elements = UINT16_C(0);

        /* reset Buffer */
        (void) memset(Buffer, 0, Size);

        /* copy group descriptor to Buffer */
        (void) memcpy(Buffer, &Handle, sizeof(TLV_group_t));
    }

    /* return newly created group Handle */
    return ((TLV_GroupHandle_TP) Buffer);
}

void TLV_RemoveGroup(TLV_GroupHandle_TP Handle)
{
    /* the implementation design does not require resources to be freed up */
    (void) Handle;
}

TLV_Element_T* TLV_AddElement(TLV_GroupHandle_TP const Handle, const uint16_t Type, const uint16_t Length, const void* const Value)
{
    /* perform plausibility tests on the input parameters */
    if (((TLV_GroupHandle_TP) NULL == Handle) ||
            (NULL == Value))
    {
        /* return invalid Handle to report generic failure */
        return ((TLV_Element_T*) NULL);
    }

    TLV_Element_T* Element = TLV_GetElement(Handle, Type);

    /* test whether Element already exists in the database */
    if (Element)
    {
        /* test whether new data Length fits into already reserved space */
        if (Element->DataLength >= Length)
        {
            /* update Element in place */
            Element->DataLength = Length;
            memcpy(Element->DataBuffer, (uint8_t*) Value, Length);
        }
        else
        {
            /* invalidate old Element */
            TLV_RemoveElement(Handle, Type);

            /* reset the Element variable to NULL to indicate that a new Element have to be added */
            Element = (TLV_Element_T*) NULL;
        }
    }

    /* test whether a new Element have to be added */
    if (!Element)
    {
        uint16_t AvailableMemory, RequiredMemory;

        /* test available memory space */
        RequiredMemory = Length + sizeof(TLV_Element_T);
        AvailableMemory = GetAvailableMemorySize(Handle);

        if (AvailableMemory < RequiredMemory)
        {
            /* perform garbage collection on the database */
            AvailableMemory = GarbageCollectGroup(Handle);
        }

        /* test available memory space after garbage collection */
        if (AvailableMemory < RequiredMemory)
        {
            ; /* insufficient memory error - return NULL */
        }
        else
        {
            /* add new Element and related data to database */
            Element = GetLastElement(Handle);

            if (Element)
            {
                /* Element database is not empty */
                TLV_Element_T* LastElement;

                LastElement = Element;
                Element = &Element[1];

                Element->DataBuffer = (uint8_t*) ((uint32_t) &LastElement->DataBuffer[0] - Length);
            }
            else
            {
                /* Element database is empty */
                Element = (TLV_Element_T*) Handle->Buffer;
                Element->DataBuffer = (uint8_t*) ((uint32_t) &Handle->Buffer[Handle->Limit] - Length);
            }

            Element->DataType = Type;
            Element->DataLength = Length;
            memcpy(Element->DataBuffer, Value, Length);

            /* increment number of Elements in table */
            Handle->Elements++;
        }
    }

    return (Element);
}

TLV_Element_T* TLV_GetElement(TLV_GroupHandle_TP const Handle, const uint16_t Type)
{
    /* perform plausibility tests on the input parameters */
    if ((TLV_GroupHandle_TP) NULL == Handle)
    {
        /* return invalid Handle to report generic failure */
        return ((TLV_Element_T*) NULL);
    }

    {
        uint16_t ElementIndex;
        TLV_Element_T* Result = (TLV_Element_T*) NULL;

        /* search for matching TLV Elements in the Buffer */
        for (ElementIndex = 0; ElementIndex < Handle->Elements; ElementIndex++)
        {
            TLV_Element_T* Element = GET_ELEMENT(Handle, ElementIndex);

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
    TLV_Element_T* Element;

    /* check whether requested Element exists
     * TLV_GetElement() performs plausibility check on the passed Handle
     */
    Element = TLV_GetElement(Handle, Type);

    if (Element)
    {
        memset(Element, 0, sizeof(TLV_Element_T));
    }
}

#endif /* if BCDS_FEATURE_TLV */
