/******************************************************************************/
/*
 *  Copyright (C) Bosch Connected Devices and Solutions GmbH
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file       BLECustomConfig.h
 *
 *
 *  This file permits the user to configure/customize the BLE stack and
 *  profile settings. This will override the previous definitions.
 *
 * ****************************************************************************/


/* header definition ******************************************************** */
#ifndef BCDS_BLE_CUSTOM_CONFIG_PH_H_
#define BCDS_BLE_CUSTOM_CONFIG_PH_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */

/** Macros to override the platform specific options */
#define BLE_EM9301_LOADPATCH                                    0
#define BLE_PROTECT_ISR_FUNCTION_CALL                           1
#define BLE_USE_RESTRICTED_LOCAL_MEMORY                         1
#define BLETYPES_ALREADY_DEFINED                                0
#define BLECONTROLLER_NEED_SPECIFIC_INIT                        1
#define BLECONTROLLER_USED                       BLECONTROLLER_EM

/** The declaration of the constant is to be placed in the code section */
#define CONST_DECL                                              const
#define UNUSED_PARAMETER(P)                                     (P = P)

/** Macros to override the core stack options */
#define BLE_ROLE_SCANNER                                        0
#define BLE_ROLE_ADVERTISER                                     1
#define BLE_SM_SLAVE_RESOLVE_MASTER_RANDOM_ADDRESS              1
#define BLE_ADVERTSING_TX_POWER_SUPPORT                         0
#define BLE_PARMS_CHECK                                         1
#define BLEERROR_HANDLER                                        0
#define BLE_CONNECTION_SUPPORT                                  1
#define BLESPECIFICATION_COMPLIANCE                             BLESPECIFICATION_4_1
#define BLE_RANDOM_ADDRESSING_SUPPORT                           0
#define BLE_PUBLIC_ADDRESS_GETTER_CALLBACK                      0
#define BLE_SECURITY                                            1
#define BLE_SM_DYNAMIC_IO_CAPABILITIES                          0
#define BLE_SM_SIGNATURE_SUPPORT                                0
#define BLE_WHITE_LIST_SUPPORT                                  0
#define BLE_ENABLE_TEST_COMMANDS                                0
#define BLE_USE_HOST_CHANNEL_CLASSIFICATION                     0
#define BLE_ENABLE_VENDOR_SPECIFIC                              1
#define BLEL2CAP_ENABLE_API                                     0
#define BLEL2CAP_CONNECTION_ORIENTED_CHANNELS                   0

#define SM_IO_CAPABILITIES                  SM_IO_NOINPUTNOOUTPUT
#define BLE_ADVERTISING_DIRECTED_LOW_DUTY_CYCLE_SUPPORT         0

/** Macros to override the ATT options */
#define ATTRIBUTE_PROTOCOL                                      1
#define ATT_ROLE_CLIENT                                         0
#define ATT_SERVER_SUPPORT_32_BIT_UUID                          0

/** Macros to override the GATT options */
#define GENERIC_ATTRIBUTE_PROFILE                               0

#define BLEGATT_SUPPORT_ALL_SERVICE_DISCOVERY                   0
#define BLEGATT_SUPPORT_SINGLE_SERVICE_DISCOVERY                1
#define BLEGATT_SUPPORT_RELATIONSHIP_DISCOVERY                  0
#define BLEGATT_SUPPORT_SINGLE_CHARACTERISTIC_DISCOVERY         0
#define BLEGATT_SUPPORT_ALL_CHARACTERISTIC_DISCOVERY            1
#define BLEGATT_SUPPORT_READ_CHARACTERISTIC_VALUE_BY_HANDLE     0
#define BLEGATT_SUPPORT_READ_CHARACTERISTIC_VALUE_BY_TYPE       1
#define BLEGATT_SUPPORT_READ_LONG_CHARACTERISTIC_VALUE          0
#define BLEGATT_SUPPORT_GET_CHARACTERISTIC_DESCRIPTOR_LIST      0
#define BLEGATT_SUPPORT_WRITE_CHARACTERISTIC_WITHOUT_RESPONSE   0
#define BLEGATT_SUPPORT_WRITE_CHARACTERISTIC_VALUE              1
#define BLEGATT_SUPPORT_WRITE_LONG_CHARACTERISTIC_VALUE         0

/** Macros to override the LOGGING options */
#define BLE_PRINT_DEBUG_TRACES                                  0
#define ATT_DEBUG_TRACES                                        0
#define SMP_DEBUG_TRACES                                        0
#if (BLE_PRINT_DEBUG_TRACES == 1)
#define BLEHCI_HANDLER                                          0
#endif //(BLE_PRINT_DEBUG_TRACES == 1)

/** Macros to override the PROFILES options */
#define BLEALPWDATAEXCHANGE_SUPPORT_SERVER                      1
#define BLE_SUPPORT_ALPWDATAEXCHANGE_SERVICE                    1

/* local function prototype declarations */

/* local module global variable declarations */

/* local inline function definitions */

#endif /* BCDS_BLE_CUSTOM_CONFIG_PH_H_ */
