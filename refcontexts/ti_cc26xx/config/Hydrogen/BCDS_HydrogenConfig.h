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
