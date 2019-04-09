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

#ifndef BCDS_BLEINITCONFIG_H_
#define BCDS_BLEINITCONFIG_H_

/* public interface declaration */
#include "GapInitConfig.h"

/* public type and macro definitions */

/* Task configuration*/
#define SBP_TASK_PRIORITY                       UINT32_C(2)
#ifndef SBP_TASK_STACK_SIZE
#define SBP_TASK_STACK_SIZE                     UINT32_C(644)
#endif

/* Events identifiers for application queue message callback */
#define GAPROLE_ICALL_EVT             							ICALL_MSG_EVENT_ID   /**< Event_Id_31 */
#define GAPROLE_QUEUE_EVT                         				UTIL_QUEUE_EVENT_ID  /**< Event_Id_30 */


#define GAPROLE_STATE_CHANGE_EVT                                UINT8_C(0x01) /**< Event for gap role state change callback */
#define GAPBONDMGR_PAIR_STATE_EVT                               UINT8_C(0x02) /**< Event for gap bond pair state callback */
#define DATASERVICE_CHAR_CHANGE_EVT 			                UINT8_C(0x03) /**< Event for data service char write callback */
#define DEBUGINFOSERVICE_CHAR_CHANGE_EVT     	                UINT8_C(0x04) /**< Event for debug information service char write callback */

/* Events identifiers for BLE task event callback
 * Task events must be in power of 2 and maximum allowed events are 16
 * (refer Page:49 in BLE software developer guide document) */
#define FOTA_QUEUE_EVT	  					  Event_Id_07

#define GAPROLE_ALL_EVENTS            							(GAPROLE_ICALL_EVT     			| \
																 FOTA_QUEUE_EVT					| \
                                       	   	   	   	   	   	   	 GAPROLE_QUEUE_EVT				)


#endif /* BCDS_BLEINITCONFIG_H_ */

