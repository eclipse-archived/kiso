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
#ifndef BCDS_UPA_USERPAGEACCESS_PH_H_
#define BCDS_UPA_USERPAGEACCESS_PH_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */
#define UPA_GET_USER_PAGE_START_ADDRESS      	 UINT32_C(0x0FE00000) /**< the starting address of the user page (EFM32GG395F1024)*/
#define UPA_GET_USER_PAGE_END_ADDRESS        	 UINT32_C(0X0FE00800) /**< the ending address of the user page (EFM32GG395F1024)*/
#define UPA_SIZE_OF_STRUCTURE					 UINT32_C(13)		  /**< the size of structure based on size of its members Ex: 1+4+4+4 */
/* local function prototype declarations */

/* local module global variable declarations */

/** Union for user page information. The size of the members must be a multiple of DWORD (unit32_t)
 *  Create user specific union and include it in app > config folder
 *  @note The union written below is a generic example union for references*/
union UPA_userPageAccess_u
{
    /* only the first member of the union can be initialized at compilation time */
    struct UPA_userPageAccess_s
    {
    	uint8_t testMemberOne;
    	uint8_t testMemberTwo[4];
    	uint8_t testMemberThree[4];
    	uint8_t testMemberFour[4];

    }__attribute__((packed)) UPA_userPageAccess_s;

    /**
     * @note This UPA_SIZE_OF_STRUCTURE macro have to modified according to the size of the struct UPA_userPageAccess_s
     */
    uint8_t data[sizeof(UPA_SIZE_OF_STRUCTURE)];/* access provider for programming aligned DWORDs */
}__attribute__((packed, aligned(sizeof(uint32_t))));

typedef union UPA_userPageAccess_u UPA_userPageAccess_t, *UPA_userPageAccess_tp;

/** UPA_userPageAccess_s structure element size */
#define UPA_TEST_MEMBER_ONE_SIZE								(UINT8_C(1)) /** Size of testMemberOne */
#define UPA_TEST_MEMBER_TWO_SIZE								(UINT8_C(4)) /** Size of testMemberTwo */
#define UPA_TEST_MEMBER_THREE_SIZE								(UINT8_C(4)) /** Size of testMemberThree */
#define UPA_TEST_MEMBER_FOUR_SIZE								(UINT8_C(4)) /** Size of testMemberFour */

/** UPA_userPageAccess_s structure element offset */
#define UPA_TEST_MEMBER_ONE_FIELD						(UINT8_C(0))  											 /** offset of testMemberOne */
#define UPA_TEST_MEMBER_TWO_FIELD						UPA_TEST_MEMBER_ONE_FIELD + UPA_TEST_MEMBER_ONE_SIZE	 /** offset of testMemberTwo */
#define UPA_TEST_MEMBER_THREE_FIELD						UPA_TEST_MEMBER_TWO_FIELD + UPA_TEST_MEMBER_TWO_SIZE	 /** offset of testMemberThree */
#define UPA_TEST_MEMBER_FOUR_FIELD						UPA_TEST_MEMBER_THREE_FIELD + UPA_TEST_MEMBER_THREE_SIZE /** offset of testMemberFour */

/* local inline function definitions */

#endif /* BCDS_UPA_USERPAGEACCESS_PH_H_ */

/** ************************************************************************* */
