# BCDS Cellular Driver #

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

This package provides the support for Driver(s) for cellular communication (2G/3G), e.g. ublox driver.

You can find more information on the [Cellular](https://bcds01.de.bosch.com/wiki/display/BSA/Cellular+Home) wiki page.

### Features ###

* It provides cellular communication support for the below drivers 
	* UBlox LISA-U200
	* Quectel UC-20
* It uses the standard AT commands to communicate with the Host.
* It abstracts the below Cellular features
	* Socket communication (UDP based)
	* SMS

### Limitations ###

* None

### Known Issues ###

* Hardware instability causes issues in Quectel integration tests.

## Changelog ##

### Ongoing Version ###

* [PFM-752](https://bcds01.de.bosch.com/jira/browse/PFM-752)
	* Integrated FreeRTOS v10.0.1 to Cellular
* [PFM-731](https://bcds01.de.bosch.com/jira/browse/PFM-731)
	* If unit test fails Jenkins should not show green
* [PFM-716](https://bcds01.de.bosch.com/jira/browse/PFM-716)
	* Removed the version header file

### v0.4.0 ###

* [PFM-679](https://bcds01.de.bosch.com/jira/browse/PFM-679)
	* Improved the unit test coverage

* [PFM-678](https://bcds01.de.bosch.com/jira/browse/PFM-678)
	* Public header files name updated as per coding guideline. Function names are prefixed by their corresponding module name and old functions are deprecated

* [PFM-646](https://bcds01.de.bosch.com/jira/browse/PFM-646)
	* Improve CoSP documentation

* [PFM-647](https://bcds01.de.bosch.com/jira/browse/PFM-647)
	* Unify the doxygen configuration

* [PFM-641](https://bcds01.de.bosch.com/jira/browse/PFM-641)
	* Harmonized function names in public headers
		* Old function names are marked as deprecated and will be removed as part of [PFM-659](https://bcds01.de.bosch.com/jira/browse/PFM-659)

### v0.3.1 ###

* [XDK110SW-2965](https://bcds01.de.bosch.com/jira/browse/XDK110SW-2965)
    * Cleanup remaining obsolete comments and GSM references

### v0.3.0 ###

* [XDK110SW-2949](https://bcds01.de.bosch.com/jira/browse/XDK110SW-2949)
    * Remove GSM shared package dependencies

### v0.2.0 ###

* [PFM-564](https://bcds01.de.bosch.com/jira/browse/PFM-564)
	* Include paths are refactored for CoSP 2.0

### v0.1.0 ###

* [PFM-560](https://bcds01.de.bosch.com/jira/browse/PFM-560)
	* The centralized tool management is utilized.
	
* [PFM-547](https://bcds01.de.bosch.com/jira/browse/PFM-547)
	* Renamed Platform and Library bitbucket projects to bcds\_shared and 3rd\_party\_shared respectively

* [PFM-537](https://bcds01.de.bosch.com/jira/browse/PFM-537)
	* Dependency file (platform.dep) from RefContexts is used.
	
* [PFM-497](https://bcds01.de.bosch.com/jira/browse/PFM-497)
	* Streamline dependency of Cellular

#### Migration to HAL/BSP

	* [PFM-250] {https://bcds01.de.bosch.com/jira/browse/PFM-250}
		* Adapted to use the new HAL (BSP) with RefBoards
		* Adapted the use the new TestApp_XDK in Testing

#### Makefile improvements:
	* [PFM-320] {https://bcds01.de.bosch.com/jira/browse/PFM-320}
		* using Common.mk from context. Removed settings makefile.

### V0.0.2 ###

#### Features:

* UBlox LISA-U200 Driver
* Quectel UC-20 Driver

#### Known Issues and Open Points:
* Things to do for platform integration:
 - Improve stability of the integration test
 - Improve unit test coverage
 - Perform a feature technical review

#### Notes:

* Things to do after platform integration:
 - Look at Serial MUX channels for URC and Socket handling

#### Technical Review:

* https://bcds01.de.bosch.com/wiki/display/BSA/Cellular

#### Jenkins:

* http://sgpvm158.apac.bosch.com:8008/jenkins/job/Shared/job/Cellular/

#### OSS Scan:

* OSS scan was performed for Cellular package and no copyright violations found:
 - http://sgpvm158.apac.bosch.com:8008/jenkins/job/Shared/job/Cellular/job/OSS/

#### Testing:
* Integration test setup from reused from the GSM package