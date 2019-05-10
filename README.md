# Kiso #
**Note**: Not available for product development yet.
![alt text](./docs/doxygen/Kiso-logo.png)

## Introduction ##
Eclipse Kiso was designed from scratch as a software development kit for IoT devices and has already been used and verified on a handful of existing products in the market. Eclipse Kiso’s reusability, robustness and portability is a key factor which enables fast development and quick time to market for almost all kinds of IoT “Things” product development.

An overview of what the repository contains and what you can do with it can be found in the doxygen documentation.
To quickly start working on it, you can go to the next section.

## Installation guide (manual) ##
### Basic tooling ###
* Download and install anaconda version [Anaconda3-2019.03](https://repo.continuum.io/archive/)
* Download and install arm-none-eabi version [7-2017-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* Download and install doxygen version [v1.8.8](https://sourceforge.net/projects/doxygen/files/rel-1.8.8/)
* Download and use [HTerm](https://www.heise.de/download/product/hterm-53283/download) or another uart-console.
* In case the uart-driver does not work, please download [here](https://www.ftdichip.com/Drivers/VCP.htm) the right driver for your machine

**Note**: python, arm-none-eabi, doxygen need to be accessible via cmd-line. Therefore, you may need to modify the environment variable *PATH* on windows if it is not done in the installation.

### Extension tooling ###
* ```pip3 install meson```
* ```pip3 install ninja```

## Installation guide (eclipse) ##
* Use the provided Eclipse IDE provided via USB

## Supported hardwares ##
* CommonGateway
* ... more to come

## Build CommonGateway (via meson) ###
**Build**:
```bash
meson debug --cross-file boards/CommonGateway/meson_config_stm32l4_gcc8.ini
cd debug
ninja hex
```
**Clean**:
```bash
cd debug
ninja clean
```

### Build CommonGateway (via Makefile) ###
**Build**:
```bash
cd examples/CommonGateway/c-leds
make makefile=Makefile COMMONGATEWAY_HW_PLATFORM=STM32L486VG_V4 BCDS_TARGET_PLATFORM=stm32 BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
```
**Clean**:
```bash
cd examples/CommonGateway/c-leds
make makefile=Makefile COMMONGATEWAY_HW_PLATFORM=STM32L486VG_V4 BCDS_TARGET_PLATFORM=stm32 BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
```

**Note**: *$(CURDIR)* is the absolute path to *examples/CommonGateway/c-leds*

### Generate Documentation (via meson, after building the application) ###
**Build**:
```bash
cd debug
ninja doxygen
```

### Generate Documentation (via cmd) ###
**Build**:
```bash
cd docs/doxygen
doxygen Doxyfile
```

## Changelog ##

### v0.1.0 ###
* Examples running with Mita
* Improvement documentation
* Cellular added
* Test specs and test-implementations via mita added

### pre_v0.1.0 ###
* Initial Kiso code drop
* Add BSP for CommonGateway
* Cleanup modules to optimize UX for BCX
* Improve documentation
* Add build-possibilities with Makefiles & meson
