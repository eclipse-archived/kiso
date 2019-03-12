# BCDS - FreeRTOS Library

# Description
The repository hosts [FreeRTOS](http://www.freertos.org/), a 3rd party RTOS kernel. 

The 3rd Party software evaluation page of FreeRTOS can be found [here](https://bcds01.de.bosch.com/wiki/display/BSA/FreeRTOS). Original Source Location : [FreeRTOS V10.0.1 SVN Tag](https://sourceforge.net/projects/freertos/files/FreeRTOS/V10.0.1/). Change History : [History.txt](http://www.freertos.org/History.txt)

Note that software projects targeting OpenRTOS must not use the FreeRTOS library.

----

# Release Notes

### v10.0.1-1.0.2

* [SENSG-44](https://bcds01.de.bosch.com/jira/browse/SENSG-44)
	* Added stream-buffer in the compilation files
	* Added fake header for the stream-buffer
	* Added fake header for message-buffer

### v10.0.1-1.0.1

* IVASW-290
	* Corrected the argument for xTaskResumeFromISR mockup function in _task\_th.hh_ file.

### v10.0.1-1.0.0

* [PFM-752](https://bcds01.de.bosch.com/jira/browse/PFM-752)
	* Upgraded FreeRTOS to v10.0.1

### v8.2.2-1.1.0

* [PFM-564](https://bcds01.de.bosch.com/jira/browse/PFM-564)
	* Include paths are refactored for CoSP 2.0

### v8.2.2-1.0.0
* [PFM-547](https://bcds01.de.bosch.com/jira/browse/PFM-547)
	* Renamed BCDS\_PLATFORM\_PATH to BCDS\_SHARED\_PATH

### v8.2.2-0.4.0
* Migrated to the new version control model for 3rd-party software FreeRTOS
* Migrated to the new build system

### v8.2.2-v0.3.3
* BCDS_Customization folder:
	* consists of an eclipse project that allows the compilation of the software package via the integrated development environment
	* contains a config sub-folder that provides a default configuration-set for the library which can be overridden by makefile settings
	* the source sub-folder contents implement a compatibility test between FreeRTOS and the selected FreeRTOSConfig.h
	* the build configurations currently support efm32 (cm3) & stm32 (cm4f) MCUs and the GCC ARM Embedded compiler suit
* FreeRTOS folder:
	* FreeRTOS/License and FreeRTOS/Source folders are mirrored from the 3rd party public source repositories
	* FreeRTOS Demo projects and FreeRTOS+ Middleware components are stripped from the package
