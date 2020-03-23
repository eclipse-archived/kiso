# Changelog

## [Release v0.1.0] - 2020-03-13

### Added

* CMake target for flashing binaries flash onto target hardware.
* Support for building user-defined apps located outside the Kiso repository, using [`KISO_APPLICATION_PATH`](http://kiso.rempler.de:1313/3.-user-guide/advanced_guide.html#building-a-custom-application).
* Support for building user-defined boards located outside the Kiso repository, using [`KISO_BOARD_PATH`](http://kiso.rempler.de:1313/3.-user-guide/advanced_guide.html#building-a-custom-board).
* [PlantUML](https://plantuml.com/de/) support for Doxygen documentation.
* More unit-tests for all packages, improving test-coverage.
* List of Contributors in `NOTICE.md`.
* Nucleo-F767 `c-leds` example, showcasing a simple *"hello world"* equivalent with LEDs.
* STM32CubeF7 integration into Kiso build-environment.

### Changed

* Move CI/CD infrastructure over to [Eclipse CI](https://ci.eclipse.org/kiso/).
* Configuration of Kiso packages and third-party library builds now done via `cmake/KisoLibsConfig.cmake`.
* Folder structure of `config/` now reflecting Kiso package structure.
* Connectivity no longer built as single package, CMake build files moved into Cellular.
* Renamed CMake b `default_config.cmake` to `kiso_defaults.cmake`
* Introduce `const`-correctiveness in Cellular internals.
* Restructure of Cellular Doxygen documentation page, now clear distinction between *Common* and *Variant*.
* Hugo site structure and theme.

### Removed

* Module `Logging_AsyncRecorder` from Utils, please use `Logging_SyncRecorder` for the time being.
* Module `TLV` from Utils.

### Known issues  
  * No multi-OS support
  * No automated HW integration tests  
  * Limited static code check (no complexity metrics)


## [Initial code drop(update) (ICD_UPDATE)] - 2020-01-22

### Added
* Board support for STM Nucleo-F767
* More unit-tests for all packages, improving test-coverage


### Changed 
* improved documentation for module documentation and website

## Known issues 
  * STM32F7 example requires manual added STM32F7 cube library to be built
  * No multi-OS support
  * No HW integration tests
  * Limited configuration documentation  
  * Limited static code check (no complexity metrics)



## [Initial code drop (ICD)] - 2019-12-18

### Added

* Initial code drop
* Initial documentation on Doxygen and Hugo
* BSP/MCU hardware abstraction for STM32L4 as part of Essentials package.
* Cellular connectivity driver for u-blox SARA R4/N4 modems.
* CMake based build system for building Kiso.
* STM32 BSP based on STM Nucleo-L4.
* CommonGateway `c-leds` example, showcasing a simple *"hello world"* equivalent with LEDs.
* CommonGateway `c-cellular` example, showcasing the feature-set provided by the Cellular connectivity package.
* CommonGateway `c-sensors` example, showcasing the use of different inertial sensors through Kiso's hardware abstraction.
* XDK `c-leds` example, showcasing a simple *"hello world"* equivalent with LEDs.
* FreeRTOS integration into Kiso build-environment.
* STM32CubeL4 integration into Kiso build-environment.
* BSTLib integration into Kiso build-environment.
* Gtest as unit-test framework into Kiso build-environment.
* **\[WIP\]** CI/CD based on private Jenkins instance.
* **\[WIP\]** EFM32 BSP based on Bosch XDK.
* **\[WIP\]** MbedTLS code.
* **\[WIP\]** EMLib build-script for Kiso.

### Known issues 
  * No multi-OS support
  * No HW integration tests
  * Limited Cellular (SW Module) documentation
  * Limited configuration documentation
  * Limited code snipped documentation
  * Limited static code check (no complexity metrics)


\* \[WIP\] = **W**ork **I**n **P**rogress, incomplete features not to be considered *done* in this release.
