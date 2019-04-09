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


#ifndef BCDS_BLE_PROFILE_CONFIG_H_
#define BCDS_BLE_PROFILE_CONFIG_H_

/* public interface declaration */

/* public type and macro definitions */

/** Handle that used to define ble Profile characteristics */
#define BLE_PROFILE_HANDLE                                              UINT32_C(0)

/** No of attribute required for profile service */
#define BLE_PROFILE_ATTRIB_NUM_FOR_PROFILE_SERVICE                      UINT8_C(1)

/** No of attributes required for each character */
#define BLE_PROFILE_ATTRIB_NUM_FOR_EACH_CHAR                            UINT8_C(2)

/** Read and write GATT prop of each characteristics */
#define BLE_PROFILE_RW_CHAR_GATT_PROP                                  UINT8_C(GATT_PROP_WRITE | GATT_PROP_READ )

/** Read GATT prop of each characteristics */
#define BLE_PROFILE_R_CHAR_GATT_PROP                                   UINT8_C( GATT_PROP_READ )

/** Write GATT prop of each characteristics */
#define BLE_PROFILE_W_CHAR_GATT_PROP                                   UINT8_C( GATT_PROP_WRITE )

/** Read write and notify GATT prop of each characteristics */
#define BLE_PROFILE_RWN_CHAR_GATT_PROP                                  UINT8_C(GATT_PROP_WRITE | GATT_PROP_READ | GATT_PROP_NOTIFY)

/** Read and notify GATT prop of each characteristics */
#define BLE_PROFILE_RN_CHAR_GATT_PROP                                   UINT8_C( GATT_PROP_READ | GATT_PROP_NOTIFY)

/** Write and notify GATT prop of each characteristics */
#define BLE_PROFILE_WN_CHAR_GATT_PROP                                   UINT8_C( GATT_PROP_WRITE | GATT_PROP_NOTIFY)

/** Macro that creates the attribute table for a character which can read */
#define BLE_PROFILE_CHAR_ATTRIB_R_RECORD(charGattProp, charUUID , charValue ) \
                {   /* character - declaration */               \
                    { ATT_BT_UUID_SIZE, characterUUID },        \
                    GATT_PERMIT_READ,                           \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charGattProp,                   \
                },                                              \
                {   /* character  - value */                    \
                    { ATT_UUID_SIZE, charUUID },                \
                    GATT_PERMIT_READ,                           \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charValue,                      \
               }

/** Macro that creates the attribute table for a character which can write */
#define BLE_PROFILE_CHAR_ATTRIB_W_RECORD(charGattProp, charUUID , charValue ) \
                {   /* character - declaration */               \
                    { ATT_BT_UUID_SIZE, characterUUID },        \
                    GATT_PERMIT_READ,                           \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charGattProp,                   \
                },                                              \
                {   /* character  - value */                    \
                    { ATT_UUID_SIZE, charUUID },                \
                    GATT_PERMIT_WRITE|GATT_PERMIT_WRITE,                         \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charValue,                      \
               }

/** Macro that creates the attribute table for a character which can read and write */
#define BLE_PROFILE_CHAR_ATTRIB_RW_RECORD(charGattProp, charUUID , charValue ) \
                {   /* character - declaration */               \
                    { ATT_BT_UUID_SIZE, characterUUID },        \
                    GATT_PERMIT_READ,                           \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charGattProp,                   \
                },                                              \
                {   /* character  - value */                    \
                    { ATT_UUID_SIZE, charUUID },                \
                    GATT_PERMIT_READ | GATT_PERMIT_WRITE,       \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charValue,                      \
                }

/** Macro that creates the attribute table for a character which can read and notify */
#define BLE_PROFILE_CHAR_ATTRIB_RN_RECORD(charGattProp, charUUID , charValue , charClientConfig ) \
                {   /* character - declaration */               \
                    { ATT_BT_UUID_SIZE, characterUUID },        \
                    GATT_PERMIT_READ,                           \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charGattProp,                   \
                },                                              \
                {   /* character  - value */                    \
                    { ATT_UUID_SIZE, charUUID },                \
                    GATT_PERMIT_READ,                           \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charValue,                      \
                },                                              \
                {   /* character - configuration */             \
                    { ATT_BT_UUID_SIZE, clientCharCfgUUID },    \
                    GATT_PERMIT_READ | GATT_PERMIT_WRITE,       \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t *) &charClientConfig,              \
               }

/** Macro that creates the attribute table for a character which can write and notify */
#define BLE_PROFILE_CHAR_ATTRIB_WN_RECORD(charGattProp, charUUID , charValue , charClientConfig ) \
                {   /* character - declaration */               \
                    { ATT_BT_UUID_SIZE, characterUUID },        \
                    GATT_PERMIT_READ,                           \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charGattProp,                   \
                },                                              \
                {   /* character  - value */                    \
                    { ATT_UUID_SIZE, charUUID },                \
                    GATT_PERMIT_WRITE|GATT_PERMIT_WRITE,                         \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charValue,                      \
                },                                              \
                {   /* character - configuration */             \
                    { ATT_BT_UUID_SIZE, clientCharCfgUUID },    \
                    GATT_PERMIT_READ |GATT_PERMIT_WRITE,       \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t *) &charClientConfig,              \
               }

/** Macro that creates the attribute table for a character which can read, write and notify */
#define BLE_PROFILE_CHAR_ATTRIB_RWN_RECORD(charGattProp, charUUID , charValue , charClientConfig ) \
                {   /* character - declaration */               \
                    { ATT_BT_UUID_SIZE, characterUUID },        \
                    GATT_PERMIT_READ,                           \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charGattProp,                   \
                },                                              \
                {   /* character  - value */                    \
                    { ATT_UUID_SIZE, charUUID },                \
                    GATT_PERMIT_READ | GATT_PERMIT_WRITE,       \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t*) &charValue,                      \
                },                                              \
                {   /* character - configuration */             \
                    { ATT_BT_UUID_SIZE, clientCharCfgUUID },    \
                    GATT_PERMIT_READ | GATT_PERMIT_WRITE,       \
                    BLE_PROFILE_HANDLE,                         \
                    (uint8_t *) &charClientConfig,              \
                }

/* public function prototype declarations */

#endif /* BCDS_BLE_PROFILE_CONFIG_H_ */
