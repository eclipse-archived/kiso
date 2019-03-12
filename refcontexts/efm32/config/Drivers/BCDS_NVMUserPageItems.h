/* WARNING: This file is generated. Modifications are not preserved. */
#ifndef BCDS_NVM_USERPAGEITEMS_H_
#define BCDS_NVM_USERPAGEITEMS_H_

/* note: this header file is project specific */

#include "BCDS_Basics.h"
#include "BCDS_NVM.h"

/* Item Description : Value to write to NVM internal Buffer */
#define NVM_ITEM_BOOTLOADER_INDEX (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00000) \
}
/* Item Description: Value to write in register LUVCO1 */
#define NVM_ITEM_ID_RADAR_LUVCO1_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO1 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00000) \
}

/* Item Description: Value to write in register LUVCO2 */
#define NVM_ITEM_ID_RADAR_LUVCO2_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO2 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00002) \
}

/* Item Description: Value to write in register LUVCO3 */
#define NVM_ITEM_ID_RADAR_LUVCO3_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO3 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00004) \
}

/* Item Description: Value to write in register LUVCO4 */
#define NVM_ITEM_ID_RADAR_LUVCO4_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO4 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00006) \
}

/* Item Description: Value to write in register LUVCO5 */
#define NVM_ITEM_ID_RADAR_LUVCO5_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO5 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00008) \
}

/* Item Description: Value to write in register LUVCO6 */
#define NVM_ITEM_ID_RADAR_LUVCO6_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO6 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0000a) \
}

/* Item Description: Value to write in register LUVCO7 */
#define NVM_ITEM_ID_RADAR_LUVCO7_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO7 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0000c) \
}

/* Item Description: Value to write in register LUVCO8 */
#define NVM_ITEM_ID_RADAR_LUVCO8_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO8 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0000e) \
}

/* Item Description: Value to write in register LUVCO9 */
#define NVM_ITEM_ID_RADAR_LUVCO9_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO9 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00010) \
}

/* Item Description: Value to write in register LUVCO10 */
#define NVM_ITEM_ID_RADAR_LUVCO10_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO10 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00012) \
}

/* Item Description: Value to write in register LUVCO11 */
#define NVM_ITEM_ID_RADAR_LUVCO11_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO11 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00014) \
}

/* Item Description: Value to write in register LUVCO12 */
#define NVM_ITEM_ID_RADAR_LUVCO12_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO12 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00016) \
}

/* Item Description: Value to write in register LUVCO13 */
#define NVM_ITEM_ID_RADAR_LUVCO13_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO13 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00018) \
}

/* Item Description: Value to write in register LUVCO14 */
#define NVM_ITEM_ID_RADAR_LUVCO14_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO14 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0001a) \
}

/* Item Description: Value to write in register LUVCO15 */
#define NVM_ITEM_ID_RADAR_LUVCO15_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO15 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0001c) \
}

/* Item Description: Value to write in register LUVCO16 */
#define NVM_ITEM_ID_RADAR_LUVCO16_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO16 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0001e) \
}

/* Item Description: Value to write in register LUVCO17 */
#define NVM_ITEM_ID_RADAR_LUVCO17_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO17 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00020) \
}

/* Item Description: Value to write in register LUVCO18 */
#define NVM_ITEM_ID_RADAR_LUVCO18_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO18 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00022) \
}

/* Item Description: Value to write in register LUVCO19 */
#define NVM_ITEM_ID_RADAR_LUVCO19_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO19 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00024) \
}

/* Item Description: Value to write in register LUVCO20 */
#define NVM_ITEM_ID_RADAR_LUVCO20_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_LUVCO20 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00026) \
}

/* Item Description: Calibration register for radar configuration register */
#define NVM_ITEM_ID_RADAR_CAL_SIZE UINT32_C(2)
#define NVM_ITEM_ID_RADAR_CAL (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00028) \
}

/* Item Description: Value for device Eui */
#define NVM_ITEM_ID_DEVICE_EUI_SIZE UINT32_C(8)
#define NVM_ITEM_ID_DEVICE_EUI (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(8), \
   .start_address = UINT32_C(0xfe0002a) \
}

/* Item Description: Product class */
#define NVM_ITEM_ID_PRODUCT_CLASS_SIZE UINT32_C(2)
#define NVM_ITEM_ID_PRODUCT_CLASS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00032) \
}

/* Item Description: 2 digit numeric plant code, e.g. 68 for RBAC */
#define NVM_ITEM_ID_DMC_PLANT_SIZE UINT32_C(2)
#define NVM_ITEM_ID_DMC_PLANT (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00034) \
}

/* Item Description: 3 digit numeric SMT line number, e.g. 008 */
#define NVM_ITEM_ID_DMC_LINE_SIZE UINT32_C(2)
#define NVM_ITEM_ID_DMC_LINE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00036) \
}

/* Item Description: 8 digit numeric batch production counter, e.g. 00000001 */
#define NVM_ITEM_ID_DMC_COUNTER_SIZE UINT32_C(4)
#define NVM_ITEM_ID_DMC_COUNTER (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe00038) \
}

/* Item Description: 3 digit numeric PCB panel location, e.g. 001 */
#define NVM_ITEM_ID_DMC_POS_SIZE UINT32_C(2)
#define NVM_ITEM_ID_DMC_POS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0003c) \
}

/* Item Description: 6 digit numeric manufacturing date, e.g. 150820 */
#define NVM_ITEM_ID_DMC_MFG_DATE_SIZE UINT32_C(4)
#define NVM_ITEM_ID_DMC_MFG_DATE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe0003e) \
}

/* Item Description: 10 digit alphanumeric part number of assembled PCBA, e.g. 1038308051 */
#define NVM_ITEM_ID_DMC_PCBA_PN_SIZE UINT32_C(10)
#define NVM_ITEM_ID_DMC_PCBA_PN (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(10), \
   .start_address = UINT32_C(0xfe00042) \
}

/* Item Description: Reserved for possible future patched items */
#define NVM_ITEM_ID_PATCHED_RESERVED_SIZE UINT32_C(52)
#define NVM_ITEM_ID_PATCHED_RESERVED (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_R, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(52), \
   .start_address = UINT32_C(0xfe0004c) \
}

/* Item Description: Air measurement Reading for RADAR */
#define NVM_ITEM_ID_AIRMEASUREMENT_SIZE UINT32_C(80)
#define NVM_ITEM_ID_AIRMEASUREMENT (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(80), \
   .start_address = UINT32_C(0xfe00080) \
}

/* Item Description: Value for Device address */
#define NVM_ITEM_ID_DEVICE_ADDRESS_SIZE UINT32_C(4)
#define NVM_ITEM_ID_DEVICE_ADDRESS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000d0) \
}

/* Item Description: Address of the primary binary */
#define NVM_ITEM_ID_PRIMARY_BINARY_ADDR_SIZE UINT32_C(4)
#define NVM_ITEM_ID_PRIMARY_BINARY_ADDR (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000d4) \
}

/* Item Description: Address of the backup binary */
#define NVM_ITEM_ID_BACKUP_BINARY_ADDR_SIZE UINT32_C(4)
#define NVM_ITEM_ID_BACKUP_BINARY_ADDR (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000d8) \
}

/* Item Description: Address of the download binary */
#define NVM_ITEM_ID_DOWNLOAD_BINARY_ADDR_SIZE UINT32_C(4)
#define NVM_ITEM_ID_DOWNLOAD_BINARY_ADDR (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000dc) \
}

/* Item Description: Size of the primary binary */
#define NVM_ITEM_ID_PRIMARY_BINARY_MAX_SIZE_SIZE UINT32_C(4)
#define NVM_ITEM_ID_PRIMARY_BINARY_MAX_SIZE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000e0) \
}

/* Item Description: Size of the backup binary */
#define NVM_ITEM_ID_BACKUP_BINARY_MAX_SIZE_SIZE UINT32_C(4)
#define NVM_ITEM_ID_BACKUP_BINARY_MAX_SIZE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000e4) \
}

/* Item Description: Size of the download binary */
#define NVM_ITEM_ID_DOWNLOAD_BINARY_MAX_SIZE_SIZE UINT32_C(4)
#define NVM_ITEM_ID_DOWNLOAD_BINARY_MAX_SIZE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000e8) \
}

/* Item Description: CRC of the primary binary */
#define NVM_ITEM_ID_PRIMARY_BINARY_CRC_SIZE UINT32_C(4)
#define NVM_ITEM_ID_PRIMARY_BINARY_CRC (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000ec) \
}

/* Item Description: CRC of the backup binary */
#define NVM_ITEM_ID_BACKUP_BINARY_CRC_SIZE UINT32_C(4)
#define NVM_ITEM_ID_BACKUP_BINARY_CRC (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000f0) \
}

/* Item Description: CRC of the download binary */
#define NVM_ITEM_ID_DOWNLOAD_BINARY_CRC_SIZE UINT32_C(4)
#define NVM_ITEM_ID_DOWNLOAD_BINARY_CRC (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe000f4) \
}

/* Item Description: Value for Network session key */
#define NVM_ITEM_ID_NETWORK_SESSION_KEY_SIZE UINT32_C(16)
#define NVM_ITEM_ID_NETWORK_SESSION_KEY (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(16), \
   .start_address = UINT32_C(0xfe000f8) \
}

/* Item Description: Value for application session key */
#define NVM_ITEM_ID_APPLICATION_SESSION_KEY_SIZE UINT32_C(16)
#define NVM_ITEM_ID_APPLICATION_SESSION_KEY (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(16), \
   .start_address = UINT32_C(0xfe00108) \
}

/* Item Description: Value for application selection key */
#define NVM_ITEM_ID_APPLICATION_SELECTION_SIZE UINT32_C(1)
#define NVM_ITEM_ID_APPLICATION_SELECTION (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00118) \
}

/* Item Description: Value for application Eui */
#define NVM_ITEM_ID_APPLICATION_EUI_SIZE UINT32_C(8)
#define NVM_ITEM_ID_APPLICATION_EUI (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(8), \
   .start_address = UINT32_C(0xfe00119) \
}

/* Item Description: Current parking status */
#define NVM_ITEM_ID_PARKING_STATUS_SIZE UINT32_C(1)
#define NVM_ITEM_ID_PARKING_STATUS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00121) \
}

/* Item Description: Fingerprint empty value */
#define NVM_ITEM_ID_FINGERPRINT_EMPTY1_SIZE UINT32_C(6)
#define NVM_ITEM_ID_FINGERPRINT_EMPTY1 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(6), \
   .start_address = UINT32_C(0xfe00122) \
}

/* Item Description: Fingerprint empty value */
#define NVM_ITEM_ID_FINGERPRINT_EMPTY2_SIZE UINT32_C(6)
#define NVM_ITEM_ID_FINGERPRINT_EMPTY2 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(6), \
   .start_address = UINT32_C(0xfe00128) \
}

/* Item Description: Fingerprint empty value */
#define NVM_ITEM_ID_FINGERPRINT_EMPTY3_SIZE UINT32_C(6)
#define NVM_ITEM_ID_FINGERPRINT_EMPTY3 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(6), \
   .start_address = UINT32_C(0xfe0012e) \
}

/* Item Description: Fingerprint empty value */
#define NVM_ITEM_ID_FINGERPRINT_EMPTY4_SIZE UINT32_C(6)
#define NVM_ITEM_ID_FINGERPRINT_EMPTY4 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(6), \
   .start_address = UINT32_C(0xfe00134) \
}

/* Item Description: Fingerprint empty value */
#define NVM_ITEM_ID_FINGERPRINT_EMPTY5_SIZE UINT32_C(6)
#define NVM_ITEM_ID_FINGERPRINT_EMPTY5 (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(6), \
   .start_address = UINT32_C(0xfe0013a) \
}

/* Item Description: Server address */
#define NVM_ITEM_ID_SERVERADDRESS_SIZE UINT32_C(4)
#define NVM_ITEM_ID_SERVERADDRESS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe00140) \
}

/* Item Description: Destination port */
#define NVM_ITEM_ID_DESTINATION_PORT_SIZE UINT32_C(2)
#define NVM_ITEM_ID_DESTINATION_PORT (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe00144) \
}

/* Item Description: Bootstrap server address */
#define NVM_ITEM_ID_BOOTSTRAP_SERVERADDRESS_SIZE UINT32_C(4)
#define NVM_ITEM_ID_BOOTSTRAP_SERVERADDRESS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe00146) \
}

/* Item Description: Bootstrap server port */
#define NVM_ITEM_ID_BOOTSTRAP_PORT_SIZE UINT32_C(2)
#define NVM_ITEM_ID_BOOTSTRAP_PORT (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0014a) \
}

/* Item Description: System timestamp */
#define NVM_ITEM_ID_ALGO_SYSTEMTIMESTAMP_SIZE UINT32_C(4)
#define NVM_ITEM_ID_ALGO_SYSTEMTIMESTAMP (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe0014c) \
}

/* Item Description: Status time */
#define NVM_ITEM_ID_ALGO_STATUS_TIME_SIZE UINT32_C(4)
#define NVM_ITEM_ID_ALGO_STATUS_TIME (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe00150) \
}

/* Item Description: Region Variant (EU, US,...) */
#define NVM_ITEM_ID_PRODUCT_CLASS_VARIANT_EXT_SIZE UINT32_C(1)
#define NVM_ITEM_ID_PRODUCT_CLASS_VARIANT_EXT (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00154) \
}

/* Item Description: Next block number of firmware to be collected from server */
#define NVM_ITEM_ID_NEXT_BLOCK_NUMBER_SIZE UINT32_C(4)
#define NVM_ITEM_ID_NEXT_BLOCK_NUMBER (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe00155) \
}

/* Item Description: Firmware package url */
#define NVM_ITEM_ID_FIRMWARE_PACKAGE_URL_SIZE UINT32_C(120)
#define NVM_ITEM_ID_FIRMWARE_PACKAGE_URL (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(120), \
   .start_address = UINT32_C(0xfe00159) \
}

/* Item Description: Firmware download is in progress flag */
#define NVM_ITEM_ID_DOWNLOAD_IN_PROGRESS_SIZE UINT32_C(1)
#define NVM_ITEM_ID_DOWNLOAD_IN_PROGRESS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe001d1) \
}

/* Item Description: Whenever error are logged via ErrorLogger module. It will log here. */
#define NVM_ITEM_ID_ERRORLOGGER_START_SIZE UINT32_C(120)
#define NVM_ITEM_ID_ERRORLOGGER_START (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(120), \
   .start_address = UINT32_C(0xfe001d2) \
}

/* Item Description: Fota Container header size */
#define NVM_ITEM_ID_FOTA_CONTAINER_HEADER_SIZE UINT32_C(2)
#define NVM_ITEM_ID_FOTA_CONTAINER_HEADER (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(2), \
   .start_address = UINT32_C(0xfe0024a) \
}

/* Item Description: Error Code */
#define NVM_ITEM_ID_ERR_ERRORCODE_SIZE UINT32_C(4)
#define NVM_ITEM_ID_ERR_ERRORCODE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe0024c) \
}

/* Item Description: Wait time to reboot */
#define NVM_ITEM_ID_ERR_REBOOTTIMES_SIZE UINT32_C(1)
#define NVM_ITEM_ID_ERR_REBOOTTIMES (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00250) \
}

/* Item Description: Fota current state */
#define NVM_ITEM_ID_FOTA_CURRENT_STATE_SIZE UINT32_C(4)
#define NVM_ITEM_ID_FOTA_CURRENT_STATE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe00251) \
}

/* Item Description: Is there a new firmware in the download partition */
#define NVM_ITEM_ID_IS_NEW_FW_SIZE UINT32_C(1)
#define NVM_ITEM_ID_IS_NEW_FW (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00255) \
}

/* Item Description: Is firmware waiting */
#define NVM_ITEM_ID_IS_FW_WAITING_ACCEPTANCE_SIZE UINT32_C(1)
#define NVM_ITEM_ID_IS_FW_WAITING_ACCEPTANCE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00256) \
}

/* Item Description: Flag for battery low warning */
#define NVM_ITEM_ID_BAT_STATUS_SIZE UINT32_C(1)
#define NVM_ITEM_ID_BAT_STATUS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00257) \
}

/* Item Description: It tells us if persistent algo values are saved */
#define NVM_ITEM_ID_ALGO_REF_VAL_ISVALID_SIZE UINT32_C(1)
#define NVM_ITEM_ID_ALGO_REF_VAL_ISVALID (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00258) \
}

/* Item Description: Table where Radar Calibration Values are stored */
#define NVM_ITEM_ID_RADAR_CAL_VAL_SIZE UINT32_C(64)
#define NVM_ITEM_ID_RADAR_CAL_VAL (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(64), \
   .start_address = UINT32_C(0xfe00259) \
}

/* Item Description: Flag to track the evolution of the Firmware Swap routine  */
#define NVM_ITEM_ID_FW_SWAP_STATUS_SIZE UINT32_C(1)
#define NVM_ITEM_ID_FW_SWAP_STATUS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe00299) \
}

/* Item Description: Flag to track the evolution of the Firmware Recovery routine  */
#define NVM_ITEM_ID_FW_RECOVERY_STATUS_SIZE UINT32_C(1)
#define NVM_ITEM_ID_FW_RECOVERY_STATUS (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe0029a) \
}

/* Item Description: FOTA result status flag  */
#define NVM_ITEM_ID_FOTA_CURRENT_RESULT_SIZE UINT32_C(1)
#define NVM_ITEM_ID_FOTA_CURRENT_RESULT (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe0029b) \
}

/* Item Description: If different than 0x00 the firmware assumes a power on reset or other not self triggered reset */
#define NVM_ITEM_ID_FW_TRIGGERED_REBOOT_SIZE UINT32_C(1)
#define NVM_ITEM_ID_FW_TRIGGERED_REBOOT (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe0029c) \
}
/* Item Description: To store the CRC of the firmware in download partition */
#define NVM_ITEM_ID_NEW_FW_CRC_SIZE UINT32_C(4)
#define NVM_ITEM_ID_NEW_FW_CRC (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(1), \
   .length_byte = UINT32_C(4), \
   .start_address = UINT32_C(0xfe0029d) \
}


#define NVM_SECTION_UserPage_BUFFER_SIZE UINT32_C(672)
#define NVM_SECTION_UserPage { \
    .section_id = UINT8_C(1), \
    .length_byte = UINT32_C(672), \
    .start_address = UINT32_C(0xfe00000), \
    .page_length_byte = UINT32_C(4096) \
}

#endif /* BCDS_NVM_USERPAGEITEMS_H_ */
