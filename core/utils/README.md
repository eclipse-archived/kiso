# BCDS Utilities #

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

This package consists of helping data structures and utilities such as Log, Lean B2CAP utility, RingBuffer, etc.

You can find more information on the [Utils](https://bcds01.de.bosch.com/wiki/display/BSA/Utils+Home) wiki page.
	
### Features ###

* [Command Line Debugger](https://bcds01.de.bosch.com/wiki/display/BSA/Command+Line+Interface)
* [Command Processor](https://bcds01.de.bosch.com/wiki/display/BSA/Command+Processor)
* CRC
* Error Logger
* [Event Hub](https://bcds01.de.bosch.com/wiki/display/BSA/EventHub)
* [Guarded Task](https://bcds01.de.bosch.com/wiki/display/BSA/GuardedTask)
* I2C Transceiver
* [Lean B2CAP](https://bcds01.de.bosch.com/wiki/display/BSA/Lean+B2CAP)
* [Logging](https://bcds01.de.bosch.com/wiki/display/BSA/Logging)
* Queue
* Ring Buffer
* Sleep Control
* [Task Monitor](https://bcds01.de.bosch.com/wiki/display/BSA/Task+Monitoring)
* TLV
* UART Transceiver
* [X-Protocol](https://bcds01.de.bosch.com/wiki/display/BSA/xProtocol)
* [Pipe and filter](https://bcds02.de.bosch.com/confluence/display/CDDK/Utils)

### Limitations ###

* Ring Buffer for multiple producer and consumer per instance is not supported, yet.
* Hydrogen `CmdProcessor` is still not non-singleton.

### Known Issues ###

* I2C Transciever module triggers Lint warnings in Carbon

## Changelog ##

### v1.6.0 ###

* [SENSG-44](https://bcds02.de.bosch.com/jira/browse/SENSG-44)
    * Added Pipe and filter architecture pattern

### v1.5.0 ###

* [XDK110SW-4068](https://bcds02.de.bosch.com/jira/browse/XDK110SW-4068)
    * Added the new api to calculate CRC32 based on reverse polynomial as per IEEE 802.3 (Ethernet) standard.

### v1.4.2 ###

* [IRO-721](https://bcds02.de.bosch.com/jira/browse/IRO-721)
	* logRecorder writer API signature changed as like Logging_Log API. 

### v1.4.1 ###

* [CDDK-92](https://bcds02.de.bosch.com/jira/browse/CDDK-92)
	* REVERTING CHANGES - Renaming stm32 to stm32l4 in all shared repos includes Utils package  
	
### v1.4.0 ###

* [INF-2657](https://bcds02.de.bosch.com/jira/browse/INF-2657)
	* Stabilized EFM32 unit tests of the I2C transciever  

* [CDDK-92](https://bcds02.de.bosch.com/jira/browse/CDDK-92)
	* Renaming stm32 to stm32l4 in all shared repos includes Utils package  
 
* [CDDK-13](https://bcds01.de.bosch.com/jira/browse/CDDK-13)
	* Corrected the wrong typedefs
    
* [CDDK-31](https://bcds01.de.bosch.com/jira/browse/CDDK-31)
	* Integrated FreeRTOS v10.0.1 to Utils
    * Fixed Hydrogen compiler issues
    * Lint warnings enabled for `I2C_Transciever` module

* [IRO-803](https://bcds01.de.bosch.com/jira/browse/IRO-803) 
	* Changed Hydrogen port of the `CmdProcessor` to use SYS/BIOS Mailboxes instead of linked list. 
    * Preparation of a future non-singleton version

### v1.3.1 ###

* [PFM-716](https://bcds01.de.bosch.com/jira/browse/PFM-716)
	* Removed the version header file
* [XDK110SW-3496](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3496)
	* Updated the Path variables in efm32 & stm32 makefile  
* [TB-168](https://bcds01.de.bosch.com/jira/browse/TB-168)
	* Fix issue with initializing linked list and adding unit test files.

### v1.3.0 ###
* [XDK110SW-3326](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3326)
	*  Added I2CTransceiver module to handle communication over I2C interfaces.

### v1.2.0 ###

* [PFM-679](https://bcds01.de.bosch.com/jira/browse/PFM-679)
	* Improved the unit test coverage
* [PFM-647](https://bcds01.de.bosch.com/jira/browse/PFM-647)
	* Unify the doxygen configuration
* [PFM-641](https://bcds01.de.bosch.com/jira/browse/PFM-641)
	* Harmonize function names in public headers
		* Old function names are marked as deprecated and will be removed as part of [PFM-659](https://bcds01.de.bosch.com/jira/browse/PFM-659)
* [TB-170](https://bcds01.de.bosch.com/jira/browse/TB-170)
	* Adding Command Line Debugger for ti_cc26xx target

### v1.1.1 ###

* [XDK110SW-3103](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3103)
    * Updated Doxygen Documentation to group the modules properly
* [PFM-644](https://bcds01.de.bosch.com/jira/browse/PFM-644)
	* LEUART Compilation issues are fixed for UARTTransceiver
* [BSE-2718](https://bcds01.de.bosch.com/jira/browse/BSE-2718)
	* Make EventHub unittest aware of config flag for observes
* [BSE-2690](https://bcds01.de.bosch.com/jira/browse/BSE-2690)
	* Fix unittest building for disable modules

### v1.1.0 ###

* [BSE-2690](https://bcds01.de.bosch.com/jira/browse/BSE-2690)
	* Implement feature defines for all modules

### v1.0.1 ###

* [APLM-2688](https://bcds01.de.bosch.com/jira/browse/APLM-2688)
	* Added ErrorLogger module to log errors with timestamp, error code, and sequence number.
	* The implementation is generic and any storage medium can be mapped using function pointers.
	* Integration test for efm target using NVM has been implemented.
* [XDK110SW-3006](https://bcds01.de.bosch.com/jira/browse/XDK110SW-3006)
	* Removed RetargetStdio module.

### v1.0.0 ###

* [PFM-564](https://bcds01.de.bosch.com/jira/browse/PFM-564)
	* Include paths are refactored for CoSP 2.0
* [PFM-545](https://bcds01.de.bosch.com/jira/browse/PFM-545)
	* Merge Control(v0.7.0) and Utils(v0.6.1) into Utils
	* Control package custom retcodes are renamed

### v0.6.1 ###

* [PFM-560](https://bcds01.de.bosch.com/jira/browse/PFM-560)
	* Addressed compiler warnings for redefinition of _BCDS\_MODULE\_ID_.

### v0.6.0 ###

* [PFM-560](https://bcds01.de.bosch.com/jira/browse/PFM-560)
	* The centralized tool management is utilized.
* [PFM-547](https://bcds01.de.bosch.com/jira/browse/PFM-547)
	* Renamed Platform and Library bitbucket projects to bcds\_shared and 3rd\_party\_shared respectively
* [PFM-507](https://bcds01.de.bosch.com/jira/browse/PFM-507)
	* Dependency file (platform.dep) from RefContexts is used.
* [PFM-498](https://bcds01.de.bosch.com/jira/browse/PFM-498)
	* Streamline dependency of Utils
* Makefile improvements. 
* removed settings make file.
* In Makefile, source directory instead of source file variable name for the Debug and release object files,  
* [PFM-399](https://bcds01.de.bosch.com/jira/browse/PFM-399)
	*  Added UARTTransceiver module to handle communication over UART interfaces on stm32 and efm32 platforms
	* As of 02-03-2017 the UARTTransceiver module is in beta release and not field tested.

### v0.5.1 ###

* [BSE-2053](https://bcds01.de.bosch.com/jira/browse/BSE-2053) 
	* Added SWO appender for stm32 target
	* Unittest for Logging module is limited only for efm32 target
	* Jenkins build script and package configurations are corrected to support efm32, stm32 and ti_cc26xx
* [PFM-352](https://bcds01.de.bosch.com/jira/browse/PFM-352) 
	* Added CommandLine debugging interface

### v0.5.0 ###

* [COSP-927](https://bcds01.de.bosch.com/jira/browse/COSP-927)
	* Added API to support resetting of the ring-buffer.
		* We do not guard the buffer. Application is expected to take care of this if needed.
	* Deprecated the RingBuffer_getUsed and RingBuffer_getFree API's.
		* Since RingBuffer_Read and RingBuffer_Write now return the actual read and written byte status compared to previous implementation where the Retcode_T was returned.

### v0.4.2 ###

#### Feature Increments
* [CD-31] (https://bcds01.de.bosch.com/jira/browse/CD-31)
    * Add logging module with compact asynchronous log record and appender over USART DMA for efm32 target

#### Improvements, Bug Fixes

* None

#### Known Issues

* None

#### Jenkins Result

* [Utils-Master](http://sgpvm158.apac.bosch.com:8008/jenkins/job/Shared/job/Utils/job/Master/23/)
    * Status `OK` but PC lint warnings are present due to Jenkins MINGW versions.
* [Utils-Master-OSSScan](http://sgpvm158.apac.bosch.com:8008/jenkins/job/Shared/job/Utils/job/OSS/13/)
    * Status `OK`

### v0.4.1 ###

#### Feature Increments

* None

#### Improvements, Bug Fixes
* [COSP-881](https://bcds01.de.bosch.com/jira/browse/COSP-881)
	* Ringbuffer is refactor ( to address context switch issues and runtime optimization )
 
#### Known Issues

* None

#### Jenkins Result

* [Utils-Master](http://sgpvm158.apac.bosch.com:8008/jenkins/job/Shared/job/Utils/job/Master/18/)
	* Status `OK` but PC lint warnings are present due to Jenkins MINGW versions.
* [Utils-Master-OSSScan](http://sgpvm158.apac.bosch.com:8008/jenkins/job/Shared/job/Utils/job/OSS/8/) 
	* Status `OK`
