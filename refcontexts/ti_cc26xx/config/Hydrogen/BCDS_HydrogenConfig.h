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

#ifndef BCDS_HYDROGENCONFIG_H_
#define BCDS_HYDROGENCONFIG_H_

/** 1 = Enabled, 0 = Disabled */
#define BCDS_FEATURE_H2_BLE		    				1		  /**< Enable BLE support in CoSP H2 */
#define BCDS_FEATURE_H2_WATCHDOG 		        	1         /**< Enable Watchdog support in CoSP H2 */
#define BCDS_FEATURE_H2_UTILS	 		        	1         /**< Enable Utils support in CoSP H2 */
#define BCDS_FEATURE_H2_INT_MEM_FLASH		    	1         /**< Enable the internal flash driver in CoSP H2 (named HalFlash) */
#define BCDS_FEATURE_H2_EXT_MEM_FLASH_AT45DB    	1         /**< Enable the flash driver AT45DB in CoSP H2 */
#define BCDS_FEATURE_H2_EXT_MEM_FLASH_W25x40CL  	1         /**< Enable the flash driver W25x40CL in CoSP H2 */


#endif /* BCDS_HYDROGENCONFIG_H_ */
