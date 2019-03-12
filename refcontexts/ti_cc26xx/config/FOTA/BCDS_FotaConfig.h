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
 * @file This file contains all configurable Parameters needed to perform
 * a digital signature validation
 */

#ifndef BCDS_FOTACONFIG_H
#define BCDS_FOTACONFIG_H

/* Include all headers which are needed by this file. */

/* Put the function declarations here */

#define FWCONTAINER_VARIANT 3

#define READ_BLOCK_SIZE UINT16_C(256)
#define FWCONTAINER_BLOCK_SIZE UINT16_C(16)

#define FOTA_PRIMARY_PARTTITION_START_ADDRESS		UINT32_C(0x000AE000)	  
#define FOTA_FIRMWARE_MAX_SIZE						UINT32_C(307200)			

#define FOTA_BLOCK_STORAGE_UPDATE_FREQUENCY         (UINT8_C(0X08))
#define FOTA_PARTITION_MAX_SIZE                     UINT32_C(0x1000)
#define FOTA_WITH_SIGNATURE                         UINT8_C(0)
#define FOTA_COAP_BLOCK_SIZE                        UINT8_C(128)     /* CoAp Block size is fixed as per the Physical layer Support */
#define FOTA_MAX_PACKAGE_URL                        UINT8_C(120)     /* Maximum value of URL */

#define FOTA_URIPATH_OBJECT_INSTANCE            (UINT8_C(2))         /* Object Instance Instance is used to notify the fota object information */
#define FOTA_URIPATH_OBJECT_INDEX               (UINT8_C(2))		 /* Object Index Value Represents the Index value of DeviceResource Inofrmation Structure while connecting to server */

/* BlockCopyAgent needs this definition */
#define BLOCKCOPY_MAX_SIZE 4096
#endif /* BCDS_FOTACONFIG_H */

