#Paths to platform packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials

#External Path information
TI_STACK_VERSION ?= v3.1.1
TIBLE_STACK_VERSION ?= simplelink_cc2640r2_sdk_1_50_00_58
TIRTOS_STACK_VERSION?= tirtos_cc13xx_cc26xx_2_20_01_08
TIRTOS_DRIVERS_VERSION?=tidrivers_cc13xx_cc26xx_2_20_01_10
TIXDC_TOOLS_VERSION?=xdctools_3_50_03_33_core

BCDS_TI_XDC_PATH ?= $(BCDS_TOOLS_PATH)/TI_BLE_Stack/$(TI_STACK_VERSION)/$(TIXDC_TOOLS_VERSION)
BCDS_TI_BLE_PATH ?= $(BCDS_TOOLS_PATH)/TI_BLE_Stack/$(TI_STACK_VERSION)/$(TIBLE_STACK_VERSION)

LINTFLAGS += -e309 -e641 -e19 -e26 -e148 -e38 -e506 -e613 -e526 -e129 +dxdc_target_types__="iar/targets/arm/std.h"

# Define the path for include directories.
BCDS_INCLUDES = \
	-I$(CURDIR)/include \
	-I$(BCDS_PACKAGE_HOME)/include \
	-I$(BCDS_CONFIG_PATH)/Utils \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_TI_XDC_PATH) \
	-I$(BCDS_TI_XDC_PATH)/packages\
	-I$(BCDS_TI_XDC_PATH)/packages/xdc \
	-I$(BCDS_TI_XDC_PATH)/packages/xdc/runtime \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/src/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/target/_common \
	-I$(BCDS_TI_BLE_PATH)/kernel/tirtos/packages \
	-I$(BCDS_TI_BLE_PATH)/kernel/tirtos/packages/ti/sysbios/knl \
	-I$(BCDS_TI_BLE_PATH)/kernel/tirtos/packages/ti/sysbios


# Source and object files
BCDS_SOURCE_FILES = \
	source/CRC.c \
	source/Hydrogen/CmdProcessor.c \
	source/Hydrogen/GuardedTask.c \
	source/Hydrogen/EventHub.c \
	source/RingBuffer.c \
	source/Logging/Logging.c \
	source/Logging/Filter.c \
	source/CmdLineDebugger.c