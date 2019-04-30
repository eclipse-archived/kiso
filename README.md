# Kiso #
**Note**: Not available for product development, DoD not fulfilled yet.

## Installation guide (manual) ##
### Basic tooling ###
* Download and install anaconda version [Anaconda3-2019.03](https://repo.continuum.io/archive/)
* Download and install arm-none-eabi version [7-2017-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* Download and install doxygen version [v1.8.8](https://sourceforge.net/projects/doxygen/files/rel-1.8.8/)

**Note**: python, arm-none-eabi, doxygen need to be accessible via cmd-line. Therefore, you may need to modify the environment variable *PATH* on windows if it is not done in the installation.

### Extension tooling ###
* ```pip3 install meson```
* ```pip3 install ninja```

## Installation guide (eclipse) ##
* Use the provided Eclipse IDE provided via USB

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
cd examples/CommonGateway/application1
make makefile=Makefile COMMONGATEWAY_HW_PLATFORM=STM32L486VG_V4 BCDS_TARGET_PLATFORM=stm32 BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
```
**Clean**:
```bash
cd examples/CommonGateway/application1
make makefile=Makefile COMMONGATEWAY_HW_PLATFORM=STM32L486VG_V4 BCDS_TARGET_PLATFORM=stm32 BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
```

**Note**: *$(CURDIR)* is the absolute path to *examples/CommonGateway/application1*

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

### pre_v0.1.0 ###
* Initial Kiso code drop
* Add BSP for CommonGateway
* Cleanup modules to optimize UX for BCX
* Improve documentation
* Add build-possibilities with Makefiles & meson
