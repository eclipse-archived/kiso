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


/* header definition ******************************************************** */
#ifndef BCDS_USERPAGE_CONFIG_H_
#define BCDS_USERPAGE_CONFIG_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */
#define USERPAGE_GET_PAGE_START_ADDRESS      	 (UINT32_C(0x0FE00000)) /**< the starting address of the user page (EFM32GG395F1024)*/
#define USERPAGE_GET_PAGE_END_ADDRESS        	 (UINT32_C(0X0FE00800)) /**< the ending address of the user page (EFM32GG395F1024)*/
#define USERPAGE_SIZE_OF_STRUCTURE					 (UINT32_C(69))		  /**< the size of structure based on size of its members Ex: 1+4+4+4 */

/* local function prototype declarations */
#define UPA_LORA_APPLICATION_KEY_LENGTH		UINT8_C(16)     /**< lora Application key length */
#define UPA_NETWORK_SESSION_KEY_LENGTH		UINT8_C(16)     /**< Network session key length */
#define UPA_APPLICATION_SESSION_KEY_LENGTH	UINT8_C(16)     /**< Application session key length */
#define UPA_DEVICE_EUI_LENGTH				UINT8_C(8)      /**< Device EUI length */

/* local module global variable declarations */

/** Union for user page information. The size of the members must be a multiple of DWORD (unit32_t)
 *  Create user specific union and include it in app > config folder
 *  @note The union written below is a generic example union for references*/
union UserPage_PageLayout_U
{
    /* only the first member of the union can be initialized at compilation time */
    struct UserPage_PageLayout_S
    {
    	uint8_t testMemberOne;
    	uint8_t testMemberTwo[4];
    	uint8_t testMemberThree[4];
    	uint8_t testMemberFour[4];
		uint8_t upaloraapplicationKey[UPA_LORA_APPLICATION_KEY_LENGTH]; /**<Application key for lora */
		uint8_t upanetworkSessionKey[UPA_NETWORK_SESSION_KEY_LENGTH]; /**< Network session Key for LoRa */
        uint8_t upaapplicationSessionKey[UPA_APPLICATION_SESSION_KEY_LENGTH]; /**< Application session key for LoRa */
        uint64_t upadeviceEUI; /**< Device EUI for LoRa */

    }__attribute__((packed)) UserPage_PageLayout_S;

    /**
     * @note This USERPAGE_SIZE_OF_STRUCTURE macro have to modified according to the size of the struct UserPage_PageLayout_S
     */
    uint8_t data[sizeof(USERPAGE_SIZE_OF_STRUCTURE)];/* access provider for programming aligned DWORDs */
}__attribute__((packed, aligned(sizeof(uint32_t))));
typedef union UserPage_PageLayout_U UserPage_PageLayout_T, *UserPage_PageLayoutPtr_T;

/** UserPage_PageLayout_S structure element size */
#define USERPAGE_TEST_MEMBER_ONE_SIZE	    (UINT8_C(1)) /** Size of testMemberOne */
#define USERPAGE_TEST_MEMBER_TWO_SIZE		(UINT8_C(4)) /** Size of testMemberTwo */
#define USERPAGE_TEST_MEMBER_THREE_SIZE		(UINT8_C(4)) /** Size of testMemberThree */
#define USERPAGE_TEST_MEMBER_FOUR_SIZE		(UINT8_C(4)) /** Size of testMemberFour */

/** UserPage_PageLayout_S structure element offset */
#define USERPAGE_TEST_MEMBER_ONE_FIELD		(UINT8_C(0))  	/** offset of testMemberOne */
#define USERPAGE_TEST_MEMBER_TWO_FIELD		(USERPAGE_TEST_MEMBER_ONE_FIELD + USERPAGE_TEST_MEMBER_ONE_SIZE)	 /** offset of testMemberTwo */
#define USERPAGE_TEST_MEMBER_THREE_FIELD	(USERPAGE_TEST_MEMBER_TWO_FIELD + USERPAGE_TEST_MEMBER_TWO_SIZE)	 /** offset of testMemberThree */
#define USERPAGE_TEST_MEMBER_FOUR_FIELD		(USERPAGE_TEST_MEMBER_THREE_FIELD + USERPAGE_TEST_MEMBER_THREE_SIZE) /** offset of testMemberFour */
#define UPA_LORA_APPLICATION_KEY_FIELD		(USERPAGE_TEST_MEMBER_FOUR_FIELD + USERPAGE_TEST_MEMBER_FOUR_SIZE)
#define UPA_NETWORK_SESSION_KEY_FIELD		UPA_LORA_APPLICATION_KEY_FIELD + UPA_LORA_APPLICATION_KEY_LENGTH     /**<offset for network session key 0x0048*/
#define UPA_APPLICATION_SESSION_KEY_FIELD	UPA_NETWORK_SESSION_KEY_FIELD + UPA_NETWORK_SESSION_KEY_LENGTH  /**<offset for network session key 0x004c*/
#define UPA_DEVICE_EUI_FIELD				UPA_APPLICATION_SESSION_KEY_FIELD + UPA_APPLICATION_SESSION_KEY_LENGTH    /**<offset for upadevice eui*/
/* local inline function definitions */

#endif /* BCDS_USERPAGE_CONFIG_H_ */

/** ************************************************************************* */
