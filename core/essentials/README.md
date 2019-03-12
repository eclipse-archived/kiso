# BCDS Essentials #

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

This package provides the basic components of the BCDS software platform, such as
basic data types, error handling primitives and the Hardware Abstraction Layer.

You can find more information on the [Essentials](https://bcds01.de.bosch.com/wiki/display/BSA/Essentials+Home) wiki page.

### Features ###

* It supports : 
	* Basic data types including the necessary compiler attribute abstraction and [common error handing and reporting](https://bcds01.de.bosch.com/wiki/display/BSA/Retcode) primitives that includes assertion are provided.
	* [Hardware Abstraction Layer](https://bcds01.de.bosch.com/wiki/pages/viewpage.action?pageId=74547297) for the below targets.
		* efm32
		* stm32
		* TI-cc26xx
	* [Board Support Package](https://bcds01.de.bosch.com/wiki/pages/viewpage.action?pageId=61212238) interface is provided. Implementation is expected from the application user based on the target board.

### Limitations ###

* HAL Delay API is only available for GCC
* Functions API is only available for GCC
* Mutex API is actually empty
* Signal API is only available for GCC
* UART for STM32F7 variant implements only Transmit function in Hardware Polling mode

### Known Issues ###

* Mismatch of between behaviour of MCU_SPI between platforms. Operations are non-blocking in EFM32 and STM32 and blocking with OS support in TI cc26xx

## Changelog ##

### Ongoing ###

* [XDK110SW-4166](https://bcds02.de.bosch.com/jira/browse/XDK110SW-4166)
	* Implemented the _MCU\_GPIO\_Initialize_ API for efm32 variant as per the interface description
* [CDDK-272](https://bcds02.de.bosch.com/jira/browse/CDDK-272)
    * Use _RETCODE\_SUCCESS_ in _RETCODE\_OK_ macro instead of hard coded 0
* [SENSG-27](https://bcds02.de.bosch.com/jira/browse/SENSG-27)
	* Added FlashIntern and CRC features support for STM32L4
	* added MCU_FlashIntern_GetMinRWSize to the interface to inform upper layers about the boundary constraints
* [XDK110SW-3208](https://bcds02.de.bosch.com/jira/browse/XDK110SW-3208)
	* Fake header is added for BCDS_MCU_Sleep.h file	
* [DEM-9](https://bcds02.de.bosch.com/jira/browse/DEM-9)
	* Added MAX4466 BSP Interfaces
* [XDK110SW-3958](https://bcds02.de.bosch.com/jira/browse/XDK110SW-3958)
	* Watchdog reset cause from RMU register for efm32 mcu
* [CDDK-194](https://bcds02.de.bosch.com/jira/browse/CDDK-194)
	* Add an interface driver for internal non-volatile memory for STM32F7 under essential, linked to [IVASW-42](https://rb-tracker.bosch.com/tracker01/browse/IVASW-42)
* [SCD-299](https://bcds02.de.bosch.com/jira/browse/SCD-299)
	* Changed SCD BSP components to current style BSP

### 1.6.0 ###
[IVASW-469](https://rb-tracker.bosch.com/tracker01/browse/IVASW-469)
	* Adding support for STM32F7 UART: Currently, only polling mode transmission is supported

### 1.5.1 ###
* [CDDK-212](https://bcds02.de.bosch.com/jira/browse/CDDK-212)
	* fixed an issue where callback type for watchdog does not follow CDDK coding guidelines

### 1.5.0 ###
* [IVASW-19](https://rb-tracker.bosch.com/tracker01/browse/IVASW-19)
	* Implemented timer for stm32f7 mcu

### 1.4.0 ###
* [XDK110SW-560](https://bcds02.de.bosch.com/jira/browse/XDK110SW-560)
	* Redesigned MCU_Watchdog_Initialize behaviour with backward compatibility garanteed 
	* Implemented watchdog timer for efm32 mcu
* [IVASW-105](https://rb-tracker.bosch.com/tracker01/browse/IVASW-105)
	* Added HAL interface for MCU CRC peripheral
	* Added HAL MCU implementation for CRC peripheral in STM32F7
* [IVASW-14](https://rb-tracker.bosch.com/tracker01/browse/IVASW-14)
	* Added BSP interface for TMP112 Temperature Sensor.

### v1.3.4 ###

* [CDDK-168](https://bcds02.de.bosch.com/jira/browse/CDDK-168)
	* Created sub-directories for STM32 mcu families and added support for STM32F7
	* Added more unit test coverage for STM32 MCU families

* [XDK110SW-3798](https://bcds02.de.bosch.com/jira/browse/XDK110SW-3798)
	* BSP extension GPIO is not supported anymore.
* [XDK110SW-3622](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3622)
	* Deprecated BSP_Mic_AKU340_Sense() API in BSP_Mic_AKU340.

### v1.3.3 ###

* [CDDK-147](https://bcds02.de.bosch.com/jira/browse/CDDK-147)
	* Changing C-standard check to allow static assertion for compilers that support C11
	* Removing unnecessary preprocessor warning in BCDS_Assert.h
	* Adding static assert to test compiler capability
	* Improving support for IAR Workbench 8

### v1.3.2 ###

* [PFM-752](https://bcds01.de.bosch.com/jira/browse/PFM-752)
	* Integrated FreeRTOS v10.0.1 to Essentials
* [XDK110SW-3562](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3562)
	* Removed BCDS\_FEATURE\_LEUART predefined feature in platform level and this can be defined in application end
* [XDK110SW-3535](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3535)
	* Added new API to check Battery status
* [IRO-868] (https://bcds02.de.bosch.com/jira/browse/IRO-868)
	* Adds an API to disable H3LIS331DL interrupt.

### v1.3.1 ###

* [XDK110SW-3533](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3533)
	* Deprecated Extension bus GPIO module.
* [PFM-722](https://bcds01.de.bosch.com/jira/browse/PFM-722)
	* Drivers Package Improvement is taken from RefContexts.
* [PFM-716](https://bcds01.de.bosch.com/jira/browse/PFM-716)
	* Removed the version header file
* [XDK110SW-3164](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3164)
	* Added additional configuration members (baudrate, parity & stop bits) to the `MCU_UART_Handle_S` structure.
* [XDK110SW-3164](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3164)
	* Added additional configuration member I2cClockFrequ in MCU_I2C_Handle structure.
	* Added additional configuration members (mode, baudrate & bit order) in MCU_SPI_Handle structure.
* [XDK110SW-3412](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3412)
	* The `BSP_Charger_BQ2407X_MeasureSignal()` is modified based on battery voltage ADC pin configurations.
* [XDK110SW-3207](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3207)
	* New API Introduced the Get the GPIO handle based on the pre defined identifier.
* [XDK110SW-3356](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3356)
	* Battery voltage status monitor issue fixed.
	* API Implemented to get the battery voltage in mvolts.
	* API return type changed to Retcode_T.

### v1.3.0 ###

* [PFM-677](https://bcds01.de.bosch.com/jira/browse/PFM-677)
	* Fix integration test for efm32.
* [PFM-646](https://bcds01.de.bosch.com/jira/browse/PFM-646)
	* Improved api documentation to have explicit description of expected return codes
* [PFM-647](https://bcds01.de.bosch.com/jira/browse/PFM-647)
	* Unify the doxygen configuration
* [PFM-641](https://bcds01.de.bosch.com/jira/browse/PFM-641)
	* Harmonized function names in public headers
		* Old function names are marked as deprecated and will be removed as part of [PFM-659](https://bcds01.de.bosch.com/jira/browse/PFM-659)		
* [XDK110SW-3162](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3162)
	* fixed the corner case in API's **MCU_I2C_ReadRegister()** and **MCU_I2C_WriteRegister()**
		*  **Corner case:** I2C read gets completed before returning from these function call, which means I2C transfer is success. Hence appropriate return value provided for this case.	
		
### v1.2.0 ###

* [PFM-584](https://bcds01.de.bosch.com/jira/browse/PFM-584)
	* Migrate LoRa Sx1272 BSP and LEUART feature to Essentials
		- BSP Modification done for LoRa Stack version 4.3
* [BSE-2716](https://bcds01.de.bosch.com/jira/browse/BSE-2716)
	* Fix invalid HAL-`CriticalSection` and -`Signal` unittest-headers
	* Add unittest-headers for HAL-`Delay` and -`Mutex`
* [BSE-2660](https://bcds01.de.bosch.com/jira/browse/BSE-2660)
	* Add missing unittest-headers for MCU/BSP-RNG component
* [BSE-2706](https://bcds01.de.bosch.com/jira/browse/BSE-2706)
	* Adopt to new STM32Cube test-headers
* [XDK110SW-3103](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3103)
	* Doxygen documentation improvement.

### v1.1.0 ###

* [BSE-2660](https://bcds01.de.bosch.com/jira/browse/BSE-2660)
	* Migrate [BSE_HAL](https://bcds01.de.bosch.com/bitbucket/projects/BSE/repos/hal/browse?at=refs%2Ftags%2Fv0.5.3) features to Essentials
		* Migrate RNG to Essentials
	* Disable compilation of HAL `CiriticalSection`/`Delay`/`Functions` for non GNU-C compilers
	* Clean-up of `BCDS_Assert` test-header
* [XDK110SW-3041](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3041)
	* Improved Doxygen Documentation

### v1.0.1 ###
* [PFM-564](https://bcds01.de.bosch.com/jira/browse/PFM-564)
	* Include paths are refactored for CoSP 2.0

### v1.0.0 ###

#### Feature

* [PFM-544](https://bcds01.de.bosch.com/jira/browse/PFM-544)
	* Merge HAL(v1.3.0) and Basics(v1.2.0) into Essentials

#### Note

* The Hardware Abstraction Layer implementation for TI target is not part of this release. It will be there as a preview.
