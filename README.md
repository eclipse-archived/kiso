# Kiso

## Installation guide (manual)
### Basic tooling
* Download and install anaconda version **4.4.11**
* Download and install arm-none-eabi version **7.0_2017q4**

**Note**: python and arm-none-eabi need to be accessible via cmd-line. Therefore, you may need to modify the environment variable *PATH* on windows if it is not done in the installation.

### Extension tooling
* ```pip3 install meson```
* ```pip3 install ninja```

## Installation guide (eclipse)
* Use the provided Eclipse IDE provided via USB

## Build CommonGateway (via meson):
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

### Build CommonGateway (via Makefile)
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
