/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/* system header files */

/* additional interface header files */

/* own header files */
#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_CRC

#include "BCDS_CRC.h"

#if BCDS_FEATURE_CRC

#define CRC_SHIFT_VAL                   UINT8_C(1)                /**< used to shift data by one time */
#define CRC8_MASK_VAL                   UINT8_C(0X80)             /**< used to mask MSB bit of the byte */
#define CRC16_MASK_VAL                  UINT16_C(0X8000)          /**< used to mask MSB bit of the byte */
#define CRC32_MASK_VAL                  UINT32_C(0X80000000)      /**< used to mask MSB bit of the byte */
#define CRC32_ETHERNET_MASK_VAL         UINT32_C(0x00000001)      /**< used to mask LSB bit of the byte */

/* functions */
/* API description is in interface header BCDS_ComoCRC.h */
Retcode_T CRC_8(uint8_t poly, uint8_t * shifter, uint8_t * data_p, uint16_t len)
{
    uint8_t lftmstShftrBit;
    uint16_t octetIdx = UINT8_C(0);
    int8_t bitIdx;
    uint8_t dataStrmOctt;
    uint8_t octetMsk;
    uint8_t dataStrmBit;
    Retcode_T retVal;

    if ((NULL == shifter) || (NULL == data_p))
    {
        retVal = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
    }

    /* as long as there are bit in data stream: check every octet */
    else
    {
        while (len--)
        {
            /* get next octet from data stream */
            dataStrmOctt = *(data_p + octetIdx);

            /*next bit in data stream */
            for (bitIdx = INT8_C(7); bitIdx >= INT8_C(0); bitIdx--)
            {
                /* leftmost bit in shifter */
                lftmstShftrBit = *shifter & CRC8_MASK_VAL;
                octetMsk = (CRC_SHIFT_VAL << bitIdx);

                /*bit in datastream */
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

            /* next byte in datastream */
            octetIdx++;
        }

        retVal = RETCODE_OK;
    }
    return (retVal);
}

/* API description is in interface header BCDS_ComoCRC.h */
Retcode_T CRC_16(uint16_t poly, uint16_t * shifter, uint8_t * data_p, uint16_t len)
{
    uint16_t lftmstShftrBit;
    uint16_t octetIdx = UINT8_C(0);
    int8_t bitIdx;
    uint8_t dataStrmOctt;
    uint8_t octetMsk;
    uint8_t dataStrmBit;
    Retcode_T retVal;

    if ((NULL == shifter) || (NULL == data_p))
    {
        retVal = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
    }
    else
    {
        /* as long as there are bit in data stream: check every octet */
        while (len--)
        {
            /* get next octet from data stream */
            dataStrmOctt = *(data_p + octetIdx);

            /* next bit in data stream */
            for (bitIdx = INT8_C(7); bitIdx >= INT8_C(0); bitIdx--)
            {
                /*leftmost bit in shifter*/
                lftmstShftrBit = *shifter & CRC16_MASK_VAL;

                octetMsk = (CRC_SHIFT_VAL << bitIdx);
                /*bit in data stream */
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

            /* next byte in data stream */
            octetIdx++;
        }

        retVal = RETCODE_OK;
    }
    return (retVal);
}

/* API description is in interface header BCDS_ComoCRC.h */
Retcode_T CRC_32(uint32_t poly, uint32_t * shifter, uint8_t * data_p, uint16_t len)
{
    uint32_t lftmstShftrBit;
    uint16_t octetIdx = UINT8_C(0);
    int8_t bitIdx;
    uint8_t dataStrmOctt;
    uint8_t octetMsk;
    uint8_t dataStrmBit;
    Retcode_T retVal;

    if ((NULL == shifter) || (NULL == data_p))
    {
        retVal = RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_FAILURE);
    }
    else
    {
        /*as long as there are bit in data stream: check every octet */
        while (len--)
        {
            /* get next octet from data stream */
            dataStrmOctt = *(data_p + octetIdx);

            /* next bit in data stream */
            for (bitIdx = INT8_C(7); bitIdx >= INT8_C(0); bitIdx--)
            {
                /* leftmost bit in shifter */
                lftmstShftrBit = *shifter & CRC32_MASK_VAL;

                octetMsk = (CRC_SHIFT_VAL << bitIdx);
                /* bit in data stream */
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

            /* next byte in data stream */
            octetIdx++;
        }

        retVal = RETCODE_OK;
    }
    /* result in shifter */
    return (retVal);
}

Retcode_T CRC_32_Reverse(uint32_t poly, uint32_t * shifter, uint8_t * data_p, uint16_t len)
{
    uint32_t lftmstShftrBit;
    uint16_t octetIdx = UINT8_C(0);
    int8_t bitIdx;
    uint8_t dataStrmOctt;
    uint8_t octetMsk;
    uint8_t dataStrmBit;
    Retcode_T retVal;

    if ((NULL == shifter) || (NULL == data_p))
    {
        retVal = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        /*as long as there are bit in data stream: check every octet */
        while (len--)
        {
            /* get next octet from data stream */
            dataStrmOctt = *(data_p + octetIdx);

            /* next bit in data stream */
            for (bitIdx = INT8_C(0); bitIdx <= INT8_C(7); bitIdx++)
            {
                /* leftmost bit in shifter */
                lftmstShftrBit = *shifter & CRC32_ETHERNET_MASK_VAL;

                octetMsk = (CRC_SHIFT_VAL << bitIdx);
                /* bit in data stream */
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

            /* next byte in data stream */
            octetIdx++;
        }

        retVal = RETCODE_OK;
    }
    /* result in shifter */
    return (retVal);
}

#endif /* if BCDS_FEATURE_CRC */
