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

/**
 * @file The DevInfoConfig header file has the Device information service for CC26xx
 *
 */

#ifndef DevInfoConfig_H
#define DevInfoConfig_H

/* Include all headers which are needed by this file. */

/* Put the type and macro definitions here */

#define DEVINFO_SYSTEM_ID_LEN             (UINT8_C(16))      /**< Macro that defines System ID Length */
#define DEVINFO_SERIAL_NUMBER_ID_LEN      (UINT8_C(13))
#define DEVINFO_HARDWARE_REV_ID_LEN       (UINT8_C(3))

/* PnP ID length */
#define DEVINFO_PNP_ID_LEN                (UINT8_C(7))      /**< Macro that defines PNP ID Length */

#define DEVINFO_MODEL_NUM               "SmartRF06EB"
#define DEVINFO_SERIAL_NUM              "0X02929"
#define DEVINFO_FIRMWARE_REVISION       "V0.0"
#define DEVINFO_HARDWARE_REVISION       "HW_REV_R1"
#define DEVINFO_SOFTWARE_REVISION       "0x03"
#define DEVINFO_MANUFACTURE_NAME        "BOSCH"
#define SYS_ID_LENGTH_ZERO               UINT8_C(0)
#define SYS_ID_LENGTH_ONE                UINT8_C(0)
#define SYS_ID_LENGTH_TWO                UINT8_C(0)
#define SYS_ID_LENGTH_THREE              UINT8_C(0)
#define SYS_ID_LENGTH_FOUR               UINT8_C(0)
#define SYS_ID_LENGTH_FIVE               UINT8_C(0)
#define SYS_ID_LENGTH_SIX                UINT8_C(0)
#define SYS_ID_LENGTH_SEVEN              UINT8_C(0)
#define AUTHORITATIVE_BODY_TYPE          UINT8_C(0) /**< authoritative body structure type */
#define AUTHORITATIVE_BODY_DATA         'e', 'x', 'p', 'e', 'r', 'i', 'm', 'e', 'n', 't', 'a', 'l' /**< authoritative body data */
#define VENDOR_ID_SOURCE                 1      /**< Vendor ID source (1=Bluetooth SIG) */
#define  VENDOR_ID                       LO_UINT16(0x000D), HI_UINT16(0x000D) /**< Vendor ID (Texas Instruments) */
#define PRODUCT_ID                       LO_UINT16(0x0000), HI_UINT16(0x0000) /**< Product ID (vendor-specific) */
#define PRODUCT_VERSION                  LO_UINT16(0x0110), HI_UINT16(0x0110) /**< Product version (JJ.M.N) */

/* Put the function declarations here */

#endif
