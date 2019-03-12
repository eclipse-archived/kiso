# BCDS Reference Contexts #

----------
## Copyright Notice ##

Copyright (C) Bosch Connected Devices and Solutions GmbH.
All Rights Reserved. Confidential.
 
Distribution only to people who need to know this information in
order to do their job.(Need-to-know principle).
Distribution to persons outside the company, only if these persons 
signed a non-disclosure agreement.
Electronic transmission, e.g. via electronic mail, must be made in encrypted form.

----------

## Description ##

This package contains the reference context to build and test shared packages for different target platforms.

### Features ###

* It supports the below target platforms
	* efm32
	* stm32
	* ti_cc26xx
* It consists of a reference set of common makefiles for building the application, unit testing, static code analyzing and document generation.
* It consists of **platform.dep** file using which baselines are created where-in the inter-dependency between all the shared packages is validated.
* It can generate documentation for the completed shared packages available as part of **platform.dep** at once.  

### Limitations ###

* This is only a reference and the BSP / Test Application used are not updated unless there is a necessity.

### Known Issues ###

* None.

## Changelog ##

### Ongoing Version ###
* [SENSG-50](https://bcds02.de.bosch.com/jira/browse/SENSG-50)
	* Added BLOCKCOPY_MAX_SIZE definition in BCDS_FotaConfig.h
* [SENSG-27](https://bcds02.de.bosch.com/jira/browse/SENSG-27)
	* Added FlashIntern and CRC features in BCDS_HALConfig.h
* [CDDK-168](https://bcds02.de.bosch.com/jira/browse/CDDK-168)
	* Added sub-directories for the STM32 family variants and updated the depending toolchain.
* [XDK110SW-3784](https://bcds02.de.bosch.com/jira/browse/XDK110SW-3784)
	* WlanConnect_Init set role to ROLE_STA.
* [XDK110SW-3620](https://bcds02.de.bosch.com/jira/browse/XDK110SW-3620)
	* Emlib USB is made as feature control.
* [XDK110SW-3798](https://bcds02.de.bosch.com/jira/browse/XDK110SW-3798)
	* BSP extension GPIO is not supported anymore.
* [XDK110SW-3662](https://bcds02.de.bosch.com/jira/browse/XDK110SW-3662)
	* Updated the essentials commit in Platform.dep. 
	* Deprecated BSP_Mic_AKU340_Sense() API in BSP_Mic_AKU340.
* [CDDK-160](https://bcds01.de.bosch.com/jira/browse/CDDK-160)
	* Cleanup of the drivers reference configuration in order to remove old and unused features
* [CDDK-137](https://bcds02.de.bosch.com/jira/browse/CDDK-137)
	* Memory report folder added
* [CDDK-147](https://bcds02.de.bosch.com/jira/browse/CDDK-147) 
	* Adding better linting support for IAR Workbench 8
    * Pointing to static_assert fix in essentials
* [CDDK-137](https://bcds02.de.bosch.com/jira/browse/CDDK-137) 
	* Adding memory profile script
    * This enables the shared packages to show the memory profile
* [CDDK-62](https://bcds01.de.bosch.com/jira/browse/CDDK-62)
	* Updated Essential and Drivers configuration headers to compile PCT2075 driver.
* [CDDK-31](https://bcds01.de.bosch.com/jira/browse/CDDK-31)
	* FreeRTOS updated to v10.0.1-1.0.0
	* FreeRTOSConfig.h file modified to fix compilation errors
	* By default _BCDS\_FEATURE\_BSP\_ACCEL\_ADXL362_ and _BCDS\_FEATURE\_BSP\_MEMORY\_AT45DB_ are enabled for ti_cc26xx variant
* [XDK110SW-3562](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3562)
	* By default _BCDS\_FEATURE\_I2C_ is enabled for ti_cc26xx variant
* [XDK110SW-3580](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3580)
	* Added feature control macro in makefile to compile ServalStack source files as part of ServalPAL.
	* Added feature control macro in makefile to compile BCDS custom source files of ServalStack.
* [IRO-541](https://bcds01.de.bosch.com/jira/browse/IRO-541) 
	* Integration test done for Fota Download agent specific to ti_cc26xx variant
* [PFM-722](https://bcds01.de.bosch.com/jira/browse/PFM-722)
	* Drivers Package Improvement
		* Enabled the NVM and FlashW25Q256FV Drivers module in efm32 and stm32 Integration frame work
		* Moved source/LM2755LedDriver.c to source/LM2755/LM2755LedDriver.c
		* Removed MiscDrivers.h file from BCDS_Shared/Drivers
		* Deleted the unwanted modules for RadarCF350 and Memory_S25FL116K
* [PFM-716](https://bcds01.de.bosch.com/jira/browse/PFM-716)
	* Removed workspace directory path in doxygen diagram
	* Added only one level of file usage in dependency diagram
	* Deprecated files removed from doxygen
	* Removed guarded by feature macro in header file for doxygen
* [XDK110SW-3412](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3412)
	* BCDS_BSP_Charger_BQ2407X.h Apis changes are fixed.
* [XDK110SW-3326](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3326)
	*  Added I2CTransceiver support feature in Utils configuration.
* [PFM-698](https://bcds01.de.bosch.com/jira/browse/PFM-698)
	* Replaced BCDS_SensorDriversConfig.h and BCDS_MiscDriversConfig.h with BCDS_DriversConfig.h 
* [XDK110SW-3356](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3356)
	* Battery voltage status monitor issue fixed.
	* API Implemented to get the battery voltage in mvolts.
	* API return type changed to Retcode_T.
* [TDL2-97](https://bcds01.de.bosch.com/jira/browse/TDL2-97)
	* Updated AT45DB RTOS support feature in configuration for drivers
* [SENSG-44](https://bcds01.de.bosch.com/jira/browse/SENSG-4)
	* Add pipe & filter configuration pattern
* [CDDK-271](https://bcds01.de.bosch.com/jira/browse/CDDK-271)
	* Updated for FOTA ECDSA verification agent the configuration
	
### v0.8.0 ###

* [PFM-701](https://bcds01.de.bosch.com/jira/browse/PFM-701)
	* Create Jenkins Job to build doxygen and get the doxygen documents for all BCDS shared packages
* [PFM-601](https://bcds01.de.bosch.com/jira/browse/PFM-601)
	* Update all shared packages to master Tags for CoSP baseline
* [PFM-663](https://bcds01.de.bosch.com/jira/browse/PFM-663)
	* ServalPAL comm buffer configuration is added.
* [PFM-647](https://bcds01.de.bosch.com/jira/browse/PFM-647)
	* Unify the doxygen configuration
* [IRO-538](https://bcds01.de.bosch.com/jira/browse/IRO-538) [IRO-539](https://bcds01.de.bosch.com/jira/browse/IRO-539)
	* Implementation of TestApp for Ti platform has been done.
	* Referencing BSP_iRoll direclty for ti_cc26xx using submodules. 
	* Build settings are added to generate integration test binary for ti variant.
* [TDL2-25](https://bcds01.de.bosch.com/jira/browse/TDL2-25) [TDL2-26](https://bcds01.de.bosch.com/jira/browse/TDL2-26)
	* Updated reference for Essentials repo to enable ADXL362 and AT45DB BSP functional
    * Updated Config files for Drivers and Essentials repo related to ti_cc26xx platform.
* [XDK110SW-3162](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3162)
	* Updated reference for Essentials repo to take the I2C fix for efm32 platform	

### v0.7.0 ###

* [BSE-2653](https://bcds01.de.bosch.com/jira/browse/BSE-2653)
	* Migrate bse/gps to BCDS_Shared/drivers
* [BSE-2709](https://bcds01.de.bosch.com/jira/browse/BSE-2709)
	* STM32Cube test headers steamlined
* [BSE-2690](https://bcds01.de.bosch.com/jira/browse/BSE-2690)
	* Feature macros implemented for Utils
* [PFM-629](https://bcds01.de.bosch.com/jira/browse/PFM-629)
	* Implementation Updated as per finalized design
* [PFM-579](https://bcds01.de.bosch.com/jira/browse/PFM-579)
	* FOTA has been implemented for Carbon
* [PFM-551](https://bcds01.de.bosch.com/jira/browse/PFM-551)
	* ServalPAL_WiFi removed from platform dependency and config of efm32
		* Reason: ServalPAL_WiFi is migrated to ServalPAL

### v0.6.0 ###

* [PFM-564](https://bcds01.de.bosch.com/jira/browse/PFM-564)
	* Include paths are refactored for CoSP 2.0
	* _Basics_ and _HAL_ are merged into a new repo _Essentials_.
	* _Control_ is merged into _Utils_
	* _Drivers_ is filled with _MiscDrivers_ and _SensorDrivers_
* [APLM-2688](https://bcds01.de.bosch.com/jira/browse/APLM-2688)
	* Enqueued Testing functionality with high priority.
	* Updated reference for testing repo to enable reboot test feature in integration tests.
	
### v0.5.0 ###

* [PFM-560](https://bcds01.de.bosch.com/jira/browse/PFM-560)
	* Configuration for central Tool usage is updated.
* [PFM-547](https://bcds01.de.bosch.com/jira/browse/PFM-547)
	* Refactored the path and naming conventions for the new folder structures.
		* The bitbucket Platform project has been renamed to BCDS_Shared. _BCDS\_SHARED\_PATH_ is used to define / overwrite this.
		* The bitbucket Libraries project has been renamed to 3rd_Party_Shared. _THIRD\_PARTY\_SHARED\_PATH_ is used to define / overwrite this.
			* BLEStack has been renamed to Alpwise_BLE. _BCDS\_BLE\_STACK\_PATH_ is used to define / overwrite this.
			* FATfs has been renamed to Elm_Chan_FatFs. _BCDS\_FATFS\_LIB\_PATH_ is used to define / overwrite this.
			* WiFi has been renamed to TI_Simplelink_Wlan. _BCDS\_SIMPLELINK\_PATH_ is used to define / overwrite this.
* [PFM-565](https://bcds01.de.bosch.com/jira/browse/PFM-565)
	* Updated the Serval stack version v1.8.1-v0.1.0.
* [BSE-2198](https://bcds01.de.bosch.com/jira/browse/BSE-2198)
	* Removed old GPS config header and replaced by new GnssUblox7 config header

### v0.4.0 ###

* The incremental build is fixed
* [PFM-505](https://bcds01.de.bosch.com/jira/browse/PFM-505)
	* Baseline phase I dependency file is added
* [PFM-463](https://bcds01.de.bosch.com/jira/browse/PFM-463)
	* Make files of efm32,stm32 and ti are cleaned up
* [BSE-2054](https://bcds01.de.bosch.com/jira/browse/BSE-2054)
	* Release target is fixed for efm32 and stm32
	* Math library is added for stm32
	* Library link order is made as linker flags, package libraries, followed by in-built libraries.
	* Fault Handler is added to the Test App
	* PWM dependency from BSP has been removed
	* BSP in stm32 is updated for HAL v1.0.0 changes
	* Config files for GSM, BLE and FOTA are corrected for stm32
	* Config files for MiscDrivers and FOTA are corrected for efm32
* _BCDS\_SERVALSTACK\_MACROS_ is introduced to have provision for individual packages test application to define their own values.
* Fixed the guard for default definition of _BCDS\_FEATURE\_BSP\_CHARGER\_BQ2405X_ in HAL config of efm32
* updated stm32 test app makefile to adpat to new stm32cube library folder structure (v1.5.2-0.2.0)
* [ATEAM-376](https://bcds01.de.bosch.com/jira/browse/ATEAM-376)
	* Fixed the lint config File for IAR compiler

### v0.3.1 ###
* Integration test app for efm32 is fixed by using fixed systick handler in BSP

### v0.3.0 ###
* working stm32 test app is provided
* referencing BSE BSP direclty as a reference for stm32 (using submodules)

### v0.2.0 ###
* using XDK BSP directly as a reference for efm32 (using submodules)
* working efm32 test app provided

### v0.1.1 ###
* Removal of Package version Major, Minor and Patch from refcontexts

### v0.1.0 ###
* Initial version of Common Makefiles for building and unittesting shared packages
