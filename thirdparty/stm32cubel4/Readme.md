# Description
The repository hosts [STM32CubeL4](http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-embedded-software/stm32cubel4.html), a 3rd party Library for the STM32 L4 series target.

----

# Release Notes
## Changelog

## Ongoing Version

## Version v1.5.2-v1.3.2
* [CDDK-92](https://bcds02.de.bosch.com/jira/browse/CDDK-92)
	* Renaming stm32cube to stm32cubel4
* [SENSG-37] (https://bcds02.de.bosch.com/jira/browse/SENSG-37)
    * Fixed fake headers for core_cm4_th.hh and stm32l4xx_hal_gpio_th.hh
    * Added the power and power extra fake headers
 
## Version v1.5.2-v1.3.1

* [SENSG-27](https://bcds02.de.bosch.com/jira/browse/SENSG-27)
	* Add unittest-headers for `stm32l4xx_hal_crc_th.hh` 

## Version v1.5.2-v1.3.0

* [BSE-2677](https://bcds01.de.bosch.com/jira/browse/BSE-2677)
	* Add unittest-headers for `stm32l4xx_hal_flash_ex.h`
	* Add missing fakes in `stm32l4xx_hal_flash_th.hh`
* [BSE-2706](https://bcds01.de.bosch.com/jira/browse/BSE-2706)
	* Re-implement unittest-headers for STM32Cube

## Version v1.5.2-1.2.0
* [BSE-2636](https://bcds01.de.bosch.com/jira/browse/BSE-2636)
	* Add unittest-headers for `stm32l4xx_hal_rng.h`
* [BSE-2612](https://bcds01.de.bosch.com/jira/browse/BSE-2612)
	* Generalize BCDS implementation of DMA IRQ-Handler

## Version v1.5.2-1.1.0

* [PFM-564](https://bcds01.de.bosch.com/jira/browse/PFM-564)
	* Include paths are refactored for CoSP 2.0

## Version v1.5.2-1.0.0

* [PFM-547](https://bcds01.de.bosch.com/jira/browse/PFM-547)
	* Renamed BCDS\_PLATFORM\_PATH to BCDS\_SHARED\_PATH

## Version 1.5.2-0.4.0
### 2017-05-24
* Libraries\STM32Cube\source\BCDS_stm32l4xx_hal_dma.c added back in stm32.mk which as removed in previous release accidentally 
* BCDS_diskio.c, BCDS_diskio.h, BCDS_diskio_th.hh, ff_th.hh files removed as they are no longer will be used from here and will be used from
  Libraries\FATfs

## Version 1.5.2-0.3.0
### 2017-05-22
* Removed FATfs dependency files stm32.mk as way to use Libraries\FATfs in the BSE application

## Version 1.5.2-0.2.0
* Migrated to the new version control model for 3rd-party software
* Migrated to the new build system

### 2017-05-15
* Patched static API's FLASH_Program_DoubleWord and FLASH_Program_Fast of stm32cube\Drivers\STM32L4xx_HAL_Driver\Src\stm32l4xx_hal_flash.c based on errata DocID026121 Rev 5, Date April 2017

## Modifications in Older Versions

### 2017-02-20
* Added workaround to silence incorrectly triggering explicit warning generation of Basics package header in stand alone builds of 3rd party packages to comply with zero warnings policy of BCDS.

### 2017-02-02
* Updated ffconf.h and stm32l4xx_hal_conf.h in BCDS_Customization/config folder to match latest template files from STM32CubeL4 release 1.5.2.
* Added USE_HAL_DRIVER define to compiler switches in stm32.mk

### 2016-12-07
* Included stm32l4xx.h in BCDS_Customization\include\BCDS_stm32l4xx_hal_dma.h to resolve inclusion order error when USE_HAL_DRIVER is defined.

### 2016-11-14
* Updated for f_rename fake API ff header.

### 2016-11-11
* Updated for f_truncate fake API updated in ff header.

### 2016-10-20
* Updated to STM32CubeL4 Firmware Package release version 1.5.2 (V1.5.2 / 12-September-2016)
   en.stm32cubel4.zip(1.5.0) and patch en.patch_cubefw_l4.zip to make it 1.5.2
   Release Notes can be found inside stm32cube/stm32cube/Release_Notes.html
   License aggreement can be found STM32Cube/stm32cube/Documentation/en.DM00218346.pdf

### 2016-08-30
* Updated `BCDS_diskio.c` with new "AwaitReady" API from flash driver interface.

### 2016-08-19
* Since the library's generic DMA ISR handler takes a lot of time to find the channel, customized version of the same for DMA2 is implemented where the channel number is passed as an additional parameter.

### 2015-05-20
* V0.4.0 (10-February-2015) is the Fourth Beta release of STM32Cube L4 
   Firmware Package for STM32L471xx/STM32L476xx/STM32L486xx devices.
* BCDS_Customization directory is added, where the makefiles and the generated
   files such as static library and object files will be stored.

### 2015-07-16
* 3rd party warning are removed

### 2016-01-14
* V1.2.0 (STM32Cube_FW_L4_V1.2.0)
