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


#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_LEANB2CAP

#include "BCDS_LeanB2CAP.h"

#if BCDS_FEATURE_LEANB2CAP

#include "LeanB2CAPConfig.h"
#include "LeanB2CAP.h"

#include "BCDS_Utils.h"
#include "BCDS_CRC.h"

/* Instances for LeanB2CAP. The total count is configurable and must be at-least of size 1UL. */
static LeanB2CAP_HandleDescriptor_T LeanB2CAPHandle[LEAN_B2CAP_INSTANCE_COUNT];

/* Constant array of command types supported by Lean B2CAP protocol. Refer LeanB2CAP_CommandType_E. */
static const uint8_t LeanB2CAPCommandList[LEANB2CAP_SUPPORTED_CMD_SIZE] =
{
    (uint8_t)LEAN_B2CAP_CMD_WRITE,
    (uint8_t)LEAN_B2CAP_CMD_WRITE_WITH_CONFIRMATION,
    (uint8_t)LEAN_B2CAP_CMD_WRITE_LONG,
    (uint8_t)LEAN_B2CAP_CMD_WRITE_LONG_WITH_CONFIRMATION,
    (uint8_t)LEAN_B2CAP_CMD_READ,
    (uint8_t)LEAN_B2CAP_CMD_READ_LONG,
    (uint8_t)LEAN_B2CAP_CMD_CONFIRMATION,
    (uint8_t)LEAN_B2CAP_CMD_INFO_CHANNEL0,
    (uint8_t)LEAN_B2CAP_CMD_INFO_CHANNEL1,
    (uint8_t)LEAN_B2CAP_CMD_INFO_CHANNEL2,
    (uint8_t)LEAN_B2CAP_CMD_INFO_CHANNEL3
};

/* The API description is in function declaration */
static Retcode_T LeanB2CAPIsCommandTypeValid(LeanB2CAP_CommandType_T Type)
{
    Retcode_T Rc = (Retcode_T)RETCODE_FAILURE;
    uint8_t CommandCheckIndex = 0U;

    for (CommandCheckIndex = 0U; CommandCheckIndex < LEANB2CAP_SUPPORTED_CMD_SIZE; CommandCheckIndex++)
    {
        if (LeanB2CAPCommandList[CommandCheckIndex] == (uint8_t)Type)
        {
            Rc = RETCODE_OK;
            break;
        }
    }

    if (Rc != RETCODE_OK)
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_LEANB2CAP_CMD_TYPE_ERROR);
    }

    return (Rc);
}

/* The API description is in Interface header */
Retcode_T LeanB2CAP_Initialize(LeanB2CAP_HandlePtr_T * Handle, LeanB2CAP_FrameData_T * ValidOutputDataStorageBufferInfo, LeanB2CAPCallback Callback)
{
    Retcode_T Rc = RETCODE_OK;
    uint8_t Index = LEANB2CAP_UNSIGNED_LONG_ZERO;

    if ((NULL != Handle) && (NULL != ValidOutputDataStorageBufferInfo) && (NULL != ValidOutputDataStorageBufferInfo->FrameDataPointer) && (NULL != Callback))
    {
        for (Index = LEANB2CAP_UNSIGNED_LONG_ZERO; Index < LEAN_B2CAP_INSTANCE_COUNT; Index++)
        {
            if (false == LeanB2CAPHandle[Index].HandleOccupance)
            {
                LeanB2CAPHandle[Index].HandleOccupance = true;
                LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_SD;
                LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].CRC2OfRxFrameInProgress = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].CmdTypeOfRxFrameInProgress = (LeanB2CAP_CommandType_T)LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].RxFrameOutputDataPtr = ValidOutputDataStorageBufferInfo->FrameDataPointer;
                LeanB2CAPHandle[Index].RxFrameOutputDataBufferSize = ValidOutputDataStorageBufferInfo->FrameDataSize;
                LeanB2CAPHandle[Index].RxFrameIndex = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].RxFramePendingProcessSize = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].Callback = Callback;
                *Handle = &LeanB2CAPHandle[Index];
                break;
            }
        }

        if (LEAN_B2CAP_INSTANCE_COUNT == Index)
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_OUT_OF_RESOURCES);
        }
    }
    else
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_NULL_POINTER);
    }

    return (Rc);
}

/* The API description is in Interface header */
Retcode_T LeanB2CAP_RxDataProcessor(LeanB2CAP_HandlePtr_T * Handle, LeanB2CAP_FrameData_T * PayLoad)
{
    Retcode_T Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_LEANB2CAP_RX_PROCESSOR_FAIL);
    uint16_t Index = LEANB2CAP_UNSIGNED_LONG_ZERO;
    uint16_t Length = LEANB2CAP_UNSIGNED_LONG_ZERO;
    uint16_t PayLoadIndex = LEANB2CAP_UNSIGNED_LONG_ZERO;
    LeanB2CAP_RawData_T ValidFrameDataInfo;
    uint16_t BytesToProcess = LEANB2CAP_UNSIGNED_LONG_ZERO;

    if ((NULL != Handle) && (NULL != PayLoad) && (NULL != PayLoad->FrameDataPointer))
    {
        for (Index = LEANB2CAP_UNSIGNED_LONG_ZERO; Index < LEAN_B2CAP_INSTANCE_COUNT; Index++)
        {
            if (*Handle == &LeanB2CAPHandle[Index])
            {
                BytesToProcess = PayLoad->FrameDataSize;

                while (BytesToProcess != LEANB2CAP_UNSIGNED_LONG_ZERO)
                {
                    switch (LeanB2CAPHandle[Index].FrameProcState)
                    {
                        case LEAN_B2CAP_FRMPRC_SD:
                        case LEAN_B2CAP_FRMPRC_ERROR:
                        if (LEANB2CAP_START_DELIMITER == PayLoad->FrameDataPointer[PayLoadIndex])
                        {
                            LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress = LEANB2CAP_UNSIGNED_LONG_ZERO;
                            LeanB2CAPHandle[Index].CRC2OfRxFrameInProgress = LEANB2CAP_UNSIGNED_LONG_ZERO;
                            Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL,&LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress,
                                    &PayLoad->FrameDataPointer[PayLoadIndex], LEANB2CAP_NUM_DATA_PROCESSED);
                            if (RETCODE_OK == Rc)
                            {
                                LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_LEN0;
                            }
                        }
                        else
                        {
                            LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_ERROR;
                            Rc = RETCODE(RETCODE_SEVERITY_WARNING,(Retcode_T)RETCODE_LEANB2CAP_SD_ERROR);
                        }
                        break;

                        case LEAN_B2CAP_FRMPRC_LEN0:
                        LeanB2CAPHandle[Index].RxFramePendingProcessSize = PayLoad->FrameDataPointer[PayLoadIndex];
                        Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL,&LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress,
                                &PayLoad->FrameDataPointer[PayLoadIndex], LEANB2CAP_NUM_DATA_PROCESSED);
                        if (RETCODE_OK == Rc)
                        {
                            LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_LEN1;
                        }
                        break;

                        case LEAN_B2CAP_FRMPRC_LEN1:
                        Length = LeanB2CAPHandle[Index].RxFramePendingProcessSize;
                        Length |= ((uint16_t)(PayLoad->FrameDataPointer[PayLoadIndex]) << LEANB2CAP_NUM_OF_BITS_PER_BYTE);
                        LeanB2CAPHandle[Index].RxFramePendingProcessSize = Length;
                        if (Length >= LEANB2CAP_MIN_DATA_TO_RX_4M_LEN)
                        {
                            if(Length <= LeanB2CAPHandle[Index].RxFrameOutputDataBufferSize + LEANB2CAP_CT_CS1_CS2_OVERHEAD)
                            {
                                Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL,&LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress,
                                        &PayLoad->FrameDataPointer[PayLoadIndex], LEANB2CAP_NUM_DATA_PROCESSED);
                                if (RETCODE_OK == Rc)
                                {
                                    LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_CMD_TYPE;
                                }
                            }
                            else
                            {
                                Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_LEANB2CAP_OP_BUFFER_INSUFFICIENT);
                            }
                        }
                        else
                        {
                            Rc = RETCODE(RETCODE_SEVERITY_WARNING,(Retcode_T)RETCODE_LEANB2CAP_LENGTH_ERROR);
                        }
                        break;

                        case LEAN_B2CAP_FRMPRC_CMD_TYPE:
                        LeanB2CAPHandle[Index].RxFramePendingProcessSize--;
                        Rc = LeanB2CAPIsCommandTypeValid((LeanB2CAP_CommandType_T)PayLoad->FrameDataPointer[PayLoadIndex]);
                        if (RETCODE_OK == Rc)
                        {
                            LeanB2CAPHandle[Index].CmdTypeOfRxFrameInProgress = (LeanB2CAP_CommandType_T)PayLoad->FrameDataPointer[PayLoadIndex];
                            Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL,&LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress,
                                    &PayLoad->FrameDataPointer[PayLoadIndex], LEANB2CAP_NUM_DATA_PROCESSED);
                            if (RETCODE_OK == Rc)
                            {
                                LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_CHECKSUM1;
                            }
                        }
                        break;

                        case LEAN_B2CAP_FRMPRC_CHECKSUM1:
                        LeanB2CAPHandle[Index].RxFramePendingProcessSize--;
                        if(LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress == PayLoad->FrameDataPointer[PayLoadIndex])
                        {
                            Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL,&LeanB2CAPHandle[Index].CRC2OfRxFrameInProgress,
                                    &PayLoad->FrameDataPointer[PayLoadIndex], LEANB2CAP_NUM_DATA_PROCESSED);
                            if (RETCODE_OK == Rc)
                            {
                                LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_PAYLOAD_BYTE;
                                LeanB2CAPHandle[Index].RxFrameIndex = LEANB2CAP_UNSIGNED_LONG_ZERO;
                            }
                        }
                        else
                        {
                            Rc = RETCODE(RETCODE_SEVERITY_WARNING,(Retcode_T)RETCODE_LEANB2CAP_CRC_ERROR);
                        }
                        break;
                        case LEAN_B2CAP_FRMPRC_PAYLOAD_BYTE:
                        LeanB2CAPHandle[Index].RxFramePendingProcessSize--;
                        if(LeanB2CAPHandle[Index].RxFrameIndex < LeanB2CAPHandle[Index].RxFrameOutputDataBufferSize)
                        {
                            LeanB2CAPHandle[Index].RxFrameOutputDataPtr[LeanB2CAPHandle[Index].RxFrameIndex] = PayLoad->FrameDataPointer[PayLoadIndex];
                            LeanB2CAPHandle[Index].RxFrameIndex++;
                            Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL,&LeanB2CAPHandle[Index].CRC2OfRxFrameInProgress,
                                    &PayLoad->FrameDataPointer[PayLoadIndex], LEANB2CAP_NUM_DATA_PROCESSED);
                            if (RETCODE_OK == Rc)
                            {
                                if (LeanB2CAPHandle[Index].RxFramePendingProcessSize <= LEANB2CAP_BYTES_FOR_PAYLOAD_END)
                                {
                                    LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_CHECKSUM2;
                                }
                            }
                        }
                        else
                        {
                            Rc = RETCODE(RETCODE_SEVERITY_FATAL,(Retcode_T)RETCODE_LEANB2CAP_OP_BUFFER_OVERFLOW);
                        }
                        break;

                        case LEAN_B2CAP_FRMPRC_CHECKSUM2:
                        if (LeanB2CAPHandle[Index].CRC2OfRxFrameInProgress == PayLoad->FrameDataPointer[PayLoadIndex])
                        {
                            if (NULL != LeanB2CAPHandle[Index].Callback)
                            {
                                ValidFrameDataInfo.CommandType = LeanB2CAPHandle[Index].CmdTypeOfRxFrameInProgress;
                                ValidFrameDataInfo.DataPointer = LeanB2CAPHandle[Index].RxFrameOutputDataPtr;
                                ValidFrameDataInfo.DataSize = LeanB2CAPHandle[Index].RxFrameIndex;
                                LeanB2CAPHandle[Index].Callback(LEAN_B2CAP_EVENT_FRAME_RX_COMPLETE, &ValidFrameDataInfo);
                                Rc = RETCODE_OK;
                                LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_SD;
                            }
                            else
                            {
                                Rc = RETCODE(RETCODE_SEVERITY_FATAL,(Retcode_T)RETCODE_LEANB2CAP_INVALID_CALLBACK);
                            }
                        }
                        else
                        {
                            Rc = RETCODE(RETCODE_SEVERITY_WARNING,(Retcode_T)RETCODE_LEANB2CAP_CRC_ERROR);
                        }
                        break;

                        default:
                        Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_LEANB2CAP_FRAME_ERROR);
                        break;
                    }

                    if(Rc != RETCODE_OK)
                    {
                        if (NULL != LeanB2CAPHandle[Index].Callback)
                        {
                            LeanB2CAPHandle[Index].Callback(LEAN_B2CAP_EVENT_FRAME_RX_ERROR, &Rc);
                            Rc = LeanB2CAP_ResetRxDataProcessor(Handle);
                            BytesToProcess--;
                            PayLoadIndex++;
                        }
                        else
                        {
                            Rc = RETCODE(RETCODE_SEVERITY_FATAL,(Retcode_T)RETCODE_LEANB2CAP_INVALID_CALLBACK);
                            BytesToProcess = LEANB2CAP_UNSIGNED_LONG_ZERO;
                        }
                        LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_ERROR;
                    }
                    else
                    {
                        BytesToProcess--;
                        PayLoadIndex++;
                    }
                }

                break;
            }
        }
        if ((LEAN_B2CAP_INSTANCE_COUNT == Index)||(LEANB2CAP_UNSIGNED_LONG_ZERO == PayLoad->FrameDataSize))
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_INVALID_PARAM);
        }
    }
    else
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_NULL_POINTER);
    }

    return (Rc);
}

/* The API description is in Interface header */
Retcode_T LeanB2CAP_Builder(LeanB2CAP_FrameData_T * LeanB2CAPBuildOuput, LeanB2CAP_RawData_T * RawDataInput)
{
    Retcode_T Rc = RETCODE_OK;
    uint16_t Index = LEANB2CAP_UNSIGNED_LONG_ZERO, CopyIndex;

    if ((NULL != LeanB2CAPBuildOuput) && (NULL != LeanB2CAPBuildOuput->FrameDataPointer) && (NULL != RawDataInput) && (NULL != RawDataInput->DataPointer))
    {
        if(LeanB2CAPBuildOuput->FrameDataSize >= RawDataInput->DataSize + LEANB2CAP_PROTOCOL_OVERHEAD)
        {
            Rc = LeanB2CAPIsCommandTypeValid((LeanB2CAP_CommandType_T)RawDataInput->CommandType);
            if (RETCODE_OK == Rc)
            {
                if (RawDataInput->DataSize > LEANB2CAP_UNSIGNED_LONG_ZERO)
                {
                    LeanB2CAPBuildOuput->FrameDataPointer[Index++] = LEANB2CAP_START_DELIMITER;
                    LeanB2CAPBuildOuput->FrameDataPointer[Index++] = (uint8_t) (RawDataInput->DataSize + LEANB2CAP_CT_CS1_CS2_OVERHEAD);
                    LeanB2CAPBuildOuput->FrameDataPointer[Index++] = (uint8_t) ((RawDataInput->DataSize + LEANB2CAP_CT_CS1_CS2_OVERHEAD) >> LEANB2CAP_NUM_OF_BITS_PER_BYTE);
                    LeanB2CAPBuildOuput->FrameDataPointer[Index++] = (uint8_t) RawDataInput->CommandType;

                    LeanB2CAPBuildOuput->FrameDataPointer[Index] = LEANB2CAP_UNSIGNED_LONG_ZERO;
                    Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL,&LeanB2CAPBuildOuput->FrameDataPointer[Index++],
                            &LeanB2CAPBuildOuput->FrameDataPointer[LEANB2CAP_SD_INDEX_4_RAW_DATA], LEANB2CAP_SD_LEN_CS1_OVERHEAD);

                    for (CopyIndex = LEANB2CAP_UNSIGNED_LONG_ZERO; CopyIndex < RawDataInput->DataSize; CopyIndex++)
                    {
                        LeanB2CAPBuildOuput->FrameDataPointer[Index++] = RawDataInput->DataPointer[CopyIndex];
                    }

                    LeanB2CAPBuildOuput->FrameDataPointer[Index] = LEANB2CAP_UNSIGNED_LONG_ZERO;
                    Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL,&LeanB2CAPBuildOuput->FrameDataPointer[Index],
                            &LeanB2CAPBuildOuput->FrameDataPointer[LEANB2CAP_CS1_INDEX_4_RAW_DATA], RawDataInput->DataSize + LEANB2CAP_CS1_OVERHEAD);
                    LeanB2CAPBuildOuput->FrameDataSize = RawDataInput->DataSize + LEANB2CAP_PROTOCOL_OVERHEAD;
                }
                else
                {
                    Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_LEANB2CAP_LENGTH_ERROR);
                }
            }
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_LEANB2CAP_OP_BUFFER_INSUFFICIENT);
        }
    }
    else
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_NULL_POINTER);
    }
    return (Rc);
}

/* The API description is in Interface header */
Retcode_T LeanB2CAP_OverheadBuilder(LeanB2CAP_Overhead_T * OutputOverhead, LeanB2CAP_RawData_T * RawDataInput)
{
    Retcode_T Rc = RETCODE_OK;
    uint16_t Index = LEANB2CAP_UNSIGNED_LONG_ZERO;

    if ((NULL != RawDataInput) && (NULL != RawDataInput->DataPointer) && (NULL != OutputOverhead)
            && (NULL != OutputOverhead->HeaderPointer) && (NULL != OutputOverhead->FooterPointer))
    {
        if((OutputOverhead->HeaderLength >= LEANB2CAP_HEADER_OVERHEAD)
                && (OutputOverhead->FooterLength >= LEANB2CAP_FOOTER_OVERHEAD)
                && (RawDataInput->DataSize > LEANB2CAP_UNSIGNED_LONG_ZERO))
        {
            Rc = LeanB2CAPIsCommandTypeValid((LeanB2CAP_CommandType_T) RawDataInput->CommandType);
            if (RETCODE_OK == Rc)
            {
                OutputOverhead->HeaderPointer[Index++] = LEANB2CAP_START_DELIMITER;
                OutputOverhead->HeaderPointer[Index++] = (uint8_t) (RawDataInput->DataSize + LEANB2CAP_CT_CS1_CS2_OVERHEAD);
                OutputOverhead->HeaderPointer[Index++] = (uint8_t) ((RawDataInput->DataSize + LEANB2CAP_CT_CS1_CS2_OVERHEAD) >> LEANB2CAP_NUM_OF_BITS_PER_BYTE);
                OutputOverhead->HeaderPointer[Index++] = (uint8_t) RawDataInput->CommandType;

                OutputOverhead->HeaderPointer[Index] = LEANB2CAP_UNSIGNED_LONG_ZERO;
                Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL, &OutputOverhead->HeaderPointer[Index],
                        &OutputOverhead->HeaderPointer[LEANB2CAP_SD_INDEX_4_RAW_DATA], LEANB2CAP_SD_LEN_CS1_OVERHEAD);

                OutputOverhead->HeaderLength = LEANB2CAP_HEADER_OVERHEAD;

                OutputOverhead->FooterPointer[LEANB2CAP_UNSIGNED_LONG_ZERO] = LEANB2CAP_UNSIGNED_LONG_ZERO;
                Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL, &OutputOverhead->FooterPointer[LEANB2CAP_UNSIGNED_LONG_ZERO],
                        &OutputOverhead->HeaderPointer[Index], LEANB2CAP_CS1_OVERHEAD);

                if (RETCODE_OK == Rc)
                {
                    Rc = CRC_8(LEANB2CAP_CRC_POLYNOMIAL, &OutputOverhead->FooterPointer[LEANB2CAP_UNSIGNED_LONG_ZERO],
                            &RawDataInput->DataPointer[LEANB2CAP_UNSIGNED_LONG_ZERO], RawDataInput->DataSize);

                    OutputOverhead->FooterLength = LEANB2CAP_FOOTER_OVERHEAD;
                }
            }
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T) RETCODE_INVALID_PARAM);
        }
    }
    else
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T) RETCODE_NULL_POINTER);
    }

    return (Rc);
}

/* The API description is in Interface header */
Retcode_T LeanB2CAP_ResetRxDataProcessor(LeanB2CAP_HandlePtr_T * Handle)
{
    Retcode_T Rc = RETCODE_OK;
    uint8_t Index = LEANB2CAP_UNSIGNED_LONG_ZERO;
    uint32_t * HandlePtr = (uint32_t *) Handle;

    if (NULL != Handle)
    {
        for (Index = LEANB2CAP_UNSIGNED_LONG_ZERO; Index < LEAN_B2CAP_INSTANCE_COUNT; Index++)
        {
            if (*HandlePtr == (uint32_t)&LeanB2CAPHandle[Index])
            {
                LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_SD;
                LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].CRC2OfRxFrameInProgress = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].CmdTypeOfRxFrameInProgress = (LeanB2CAP_CommandType_T)LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].RxFrameIndex = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].RxFramePendingProcessSize = LEANB2CAP_UNSIGNED_LONG_ZERO;
                break;
            }
        }
        if (LEAN_B2CAP_INSTANCE_COUNT == Index)
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_INVALID_PARAM);
        }
    }
    else
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_NULL_POINTER);
    }

    return (Rc);
}

/* The API description is in Interface header */
Retcode_T LeanB2CAP_Deinitialize(LeanB2CAP_HandlePtr_T * Handle)
{
    Retcode_T Rc = RETCODE_OK;
    uint8_t Index = LEANB2CAP_UNSIGNED_LONG_ZERO;

    if (NULL != Handle)
    {
        for (Index = LEANB2CAP_UNSIGNED_LONG_ZERO; Index < LEAN_B2CAP_INSTANCE_COUNT; Index++)
        {
            if (*Handle == &LeanB2CAPHandle[Index])
            {
                LeanB2CAPHandle[Index].HandleOccupance = false;
                LeanB2CAPHandle[Index].FrameProcState = LEAN_B2CAP_FRMPRC_SD;
                LeanB2CAPHandle[Index].CRC1OfRxFrameInProgress = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].CRC2OfRxFrameInProgress = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].CmdTypeOfRxFrameInProgress = (LeanB2CAP_CommandType_T)LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].RxFrameOutputDataPtr = (uint8_t*)NULL;
                LeanB2CAPHandle[Index].RxFrameIndex = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].RxFramePendingProcessSize = LEANB2CAP_UNSIGNED_LONG_ZERO;
                LeanB2CAPHandle[Index].Callback = NULL;
                *Handle = NULL;
                break;
            }
        }
        if (LEAN_B2CAP_INSTANCE_COUNT == Index)
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_INVALID_PARAM);
        }
    }
    else
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR,(Retcode_T)RETCODE_NULL_POINTER);
    }

    return (Rc);
}

#endif /* if BCDS_FEATURE_LEANB2CAP */
