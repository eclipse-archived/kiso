# Kiso

## Installation guide (manual)
### Basic tooling
* Download and install anaconda version **4.4.11**
* Download and install arm-none-eabi version **4.7_2014q2**

**Note**: python and arm-none-eabi need to be accessible via cmd-line. Therefore, you may need to modify the environment variable *PATH* on windows if it is not done in the installation.

### Extention tooling
* ```pip3 install meson```
* ```pip3 install ninja```

## Installation guide (eclipse)
* Use the provided Eclipse IDE provided via USB

## Build Sensgate (via meson):
**Build**:
```bash
meson debug --cross-file boards/SensGate/meson_config_stm32l4_gcc8.ini -Db_pch=false -Db_staticpic=false
cd debug
ninja hex
```
**Clean**:
```bash
cd debug
ninja clean
```

### Build Sensgate (via Makefile)
**Build**:
```bash
cd examples/SensGate/application1
make makefile=Makefile SENSG_HW_PLATFORM=STM32L486VG_V4 BCDS_TARGET_PLATFORM=stm32 BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
```
**Clean**:
```bash
cd examples/SensGate/application1
make makefile=Makefile SENSG_HW_PLATFORM=STM32L486VG_V4 BCDS_TARGET_PLATFORM=stm32 BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
```

**Note**: *$(CURDIR)* is the absolute path to *xamples/SensGate/application1*
