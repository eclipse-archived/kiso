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
 * @file This file contains all configurable Parameters needed to perform
 * a digital signature validation
 */

#ifndef BCDS_FOTACONFIG_H
#define BCDS_FOTACONFIG_H

/* Include all headers which are needed by this file. */

/* Put the type and macro definitions here */
#define FWCONTAINER_BLOCK_SIZE UINT16_C(512)
#define FOTA_PARTITION_DOWNLOAD	1
#define FOTA_PARTITION_BACKUP	2
#define SDCARDAGENT_NUMBER_OF_ALLOWED_INSTANCES 2
#define BCDS_FEATURE_COAP_DOWNLOAD   1

#define READ_BLOCK_SIZE UINT16_C(256)


#define FOTA_BLOCK_STORAGE_UPDATE_FREQUENCY         (UINT8_C(0x08))
#define FOTA_PRIMARY_PARTTITION_START_ADDRESS       UINT32_C(0x0008D000)
#define FOTA_FIRMWARE_MAX_SIZE                      UINT32_C(0x1c8)
#define FOTA_PARTITION_MAX_SIZE                     UINT32_C(0x1000)
#define FOTA_WITH_SIGNATURE                         UINT8_C(0)
#define FOTA_COAP_BLOCK_SIZE                        UINT8_C(128)     /* CoAp Block size is fixed as per the Physical layer Support */
#define FOTA_MAX_PACKAGE_URL                        UINT8_C(120)     /* Maximum value of URL */

#ifndef FWCONTAINER_VARIANT /* Needed because via integration tests, we are testing 2 different containers */
#define FWCONTAINER_VARIANT 4 /* Firmware container version used to generate the bin with fota-tool */
#endif

/* BlockCopyAgent needs this definition */
#define BLOCKCOPY_MAX_SIZE 4096

/* Put the function declarations here */
#endif /* BCDS_FOTACONFIG_H */
