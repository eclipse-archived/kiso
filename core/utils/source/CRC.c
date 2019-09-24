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
 *      CRC Interface Implementation
 *
 * @details
 *      This source file implements following features:
 *      - CRC_8()
 *      - CRC_16()
 *      - CRC_32()
 *      - CRC_32_Reverse()
 * 
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_CRC

/* Include Kiso_CRC interface header */
#include "Kiso_CRC.h"

#if KISO_FEATURE_CRC

#define CRC_SHIFT_VAL UINT8_C(1)                     /**< used to shift data by one time */
#define CRC8_MASK_VAL UINT8_C(0X80)                  /**< used to mask MSB bit of the byte */
#define CRC16_MASK_VAL UINT16_C(0X8000)              /**< used to mask MSB bit of the byte */
#define CRC32_MASK_VAL UINT32_C(0X80000000)          /**< used to mask MSB bit of the byte */
#define CRC32_ETHERNET_MASK_VAL UINT32_C(0x00000001) /**< used to mask LSB bit of the byte */

/*  The description of the function is available in Kiso_CRC.h */
Retcode_T CRC_8(uint8_t poly, uint8_t *shifter, const uint8_t *data_p, uint16_t len)
{
    uint8_t lftmstShftrBit;
    uint16_t octetIdx = UINT8_C(0);
    int8_t bitIdx;
    uint8_t dataStrmOctt;
    uint8_t octetMsk;
    uint8_t dataStrmBit;
    Retcode_T retVal = RETCODE_OK;

    if ((NULL == shifter) || (NULL == data_p))
    {
        retVal = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
    }

    /* As long as there are bit in data stream: check every octet */
    else
    {
        while (len--)
        {
            /* Get next octet from data stream */
            dataStrmOctt = *(data_p + octetIdx);

            /* Next bit in data stream */
            for (bitIdx = INT8_C(7); bitIdx >= INT8_C(0); bitIdx--)
            {
                /* Leftmost bit in shifter */
                lftmstShftrBit = *shifter & CRC8_MASK_VAL;
                octetMsk = (CRC_SHIFT_VAL << bitIdx);

                /* Bit in datastream */
                dataStrmBit = dataStrmOctt & octetMsk;

                if ((lftmstShftrBit != UINT8_C(0)) != (dataStrmBit != UINT8_C(0)))
                {
                    *shifter = (*shifter << CRC_SHIFT_VAL) ^ poly;
                }
                else
                {
                    *shifter = (*shifter << CRC_SHIFT_VAL);
                }
            }

            /* Next byte in datastream */
            octetIdx++;
        }

        retVal = RETCODE_OK;
    }
    return (retVal);
}

/*  The description of the function is available in Kiso_CRC.h */
Retcode_T CRC_16(uint16_t poly, uint16_t *shifter, const uint8_t *data_p, uint16_t len)
{
    uint16_t lftmstShftrBit;
    uint16_t octetIdx = UINT8_C(0);
    int8_t bitIdx;
    uint8_t dataStrmOctt;
    uint8_t octetMsk;
    uint8_t dataStrmBit;
    Retcode_T retVal = RETCODE_OK;

    if ((NULL == shifter) || (NULL == data_p))
    {
        retVal = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
    }
    else
    {
        /* As long as there are bit in data stream: check every octet */
        while (len--)
        {
            /* Get next octet from data stream */
            dataStrmOctt = *(data_p + octetIdx);

            /* Next bit in data stream */
            for (bitIdx = INT8_C(7); bitIdx >= INT8_C(0); bitIdx--)
            {
                /* Leftmost bit in shifter*/
                lftmstShftrBit = *shifter & CRC16_MASK_VAL;
                octetMsk = (CRC_SHIFT_VAL << bitIdx);
                /* Bit in data stream */
                dataStrmBit = dataStrmOctt & octetMsk;

                if ((lftmstShftrBit != UINT16_C(0)) != (dataStrmBit != UINT8_C(0)))
                {
                    *shifter = (*shifter << CRC_SHIFT_VAL) ^ poly;
                }
                else
                {
                    *shifter = (*shifter << CRC_SHIFT_VAL);
                }
            }

            /* Next byte in data stream */
            octetIdx++;
        }

        retVal = RETCODE_OK;
    }
    return (retVal);
}

/*  The description of the function is available in Kiso_CRC.h */
Retcode_T CRC_32(uint32_t poly, uint32_t *shifter, const uint8_t *data_p, uint16_t len)
{
    uint32_t lftmstShftrBit;
    uint16_t octetIdx = UINT8_C(0);
    int8_t bitIdx;
    uint8_t dataStrmOctt;
    uint8_t octetMsk;
    uint8_t dataStrmBit;
    Retcode_T retVal = RETCODE_OK;

    if ((NULL == shifter) || (NULL == data_p))
    {
        retVal = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
    }
    else
    {
        /* As long as there are bit in data stream: check every octet */
        while (len--)
        {
            /* Get next octet from data stream */
            dataStrmOctt = *(data_p + octetIdx);

            /* Next bit in data stream */
            for (bitIdx = INT8_C(7); bitIdx >= INT8_C(0); bitIdx--)
            {
                /* Leftmost bit in shifter */
                lftmstShftrBit = *shifter & CRC32_MASK_VAL;

                octetMsk = (CRC_SHIFT_VAL << bitIdx);
                /* Bit in data stream */
                dataStrmBit = dataStrmOctt & octetMsk;

                if ((lftmstShftrBit != UINT32_C(0)) != (dataStrmBit != UINT8_C(0)))
                {
                    *shifter = (*shifter << CRC_SHIFT_VAL) ^ poly;
                }
                else
                {
                    *shifter = (*shifter << CRC_SHIFT_VAL);
                }
            }

            /* Next byte in data stream */
            octetIdx++;
        }

        retVal = RETCODE_OK;
    }
    /* Result in shifter */
    return (retVal);
}

/*  The description of the function is available in Kiso_CRC.h */
Retcode_T CRC_32_Reverse(uint32_t poly, uint32_t *shifter, const uint8_t *data_p, uint16_t len)
{
    uint32_t lftmstShftrBit;
    uint16_t octetIdx = UINT8_C(0);
    int8_t bitIdx;
    uint8_t dataStrmOctt;
    uint8_t octetMsk;
    uint8_t dataStrmBit;
    Retcode_T retVal = RETCODE_OK;

    if ((NULL == shifter) || (NULL == data_p))
    {
        retVal = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /* As long as there are bit in data stream: check every octet */
        while (len--)
        {
            /* Get next octet from data stream */
            dataStrmOctt = *(data_p + octetIdx);

            /* Next bit in data stream */
            for (bitIdx = INT8_C(0); bitIdx <= INT8_C(7); bitIdx++)
            {
                /* Leftmost bit in shifter */
                lftmstShftrBit = *shifter & CRC32_ETHERNET_MASK_VAL;

                /* Bit in data stream */
                octetMsk = (CRC_SHIFT_VAL << bitIdx);
                /* Bit in data stream */
                dataStrmBit = dataStrmOctt & octetMsk;

                if ((lftmstShftrBit != UINT32_C(0)) != (dataStrmBit != UINT8_C(0)))
                {
                    *shifter = (*shifter >> CRC_SHIFT_VAL) ^ poly;
                }
                else
                {
                    *shifter = (*shifter >> CRC_SHIFT_VAL);
                }
            }

            /* Next byte in data stream */
            octetIdx++;
        }

        /* Result in shifter */
        retVal = RETCODE_OK;
    }
    /* Result in shifter */
    return (retVal);
}

#endif /* if KISO_FEATURE_CRC */
