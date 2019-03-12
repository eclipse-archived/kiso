/* WARNING: This file is generated. Modifications are not preserved. */
#ifndef NVM_CONFIG_INTEGRATIONTESTVARIABLE_H_
#define NVM_CONFIG_INTEGRATIONTESTVARIABLE_H_

/* note: this header file is project specific */

#include "BCDS_Basics.h"
#include "BCDS_NVM.h"

/* Item Description: Store the state of the last received message token of the test runner */
#define NVM_ITEM_TESTRUNNER_LASTRECEIVEDMESSAGETOKEN_SIZE UINT32_C(1)
#define NVM_ITEM_TESTRUNNER_LASTRECEIVEDMESSAGETOKEN (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_LITTLE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(0), \
   .length_byte = UINT32_C(1), \
   .start_address = UINT32_C(0xfe000) \
}

/* Item Description: Store the state of the received message */
#define NVM_ITEM_CCMSG_SIZE UINT32_C(424)
#define NVM_ITEM_CCMSG (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(0), \
   .length_byte = UINT32_C(424), \
   .start_address = UINT32_C(0xfe001) \
}

/* Item Description: Store the state of the acknowledgement message */
#define NVM_ITEM_ACKMESSAGE_SIZE UINT32_C(424)
#define NVM_ITEM_ACKMESSAGE (struct NVM_Item_S) { \
   .endianness = NVM_ENDIANNESS_NONE, \
   .mode = NVM_MODE_RW, \
   .section_id = UINT8_C(0), \
   .length_byte = UINT32_C(424), \
   .start_address = UINT32_C(0xfe1a9) \
}

#define NVM_SECTION_IntegrationTestVariable_BUFFER_SIZE UINT32_C(852)
#define NVM_SECTION_IntegrationTestVariable { \
    .section_id = UINT8_C(0), \
    .length_byte = UINT32_C(852), \
    .start_address = UINT32_C(0xfe000), \
    .page_length_byte = UINT32_C(4096) \
}

#endif
