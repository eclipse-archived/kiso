#Paths to BCDS shared and 3rd party shared packages on which this package depends
BCDS_ESSENTIALS_PATH = $(BCDS_SHARED_PATH)/Essentials

TI_STACK_VERSION ?= v3.0.1
TIBLE_STACK_VERSION ?= simplelink_cc2640r2_sdk_1_30_00_25
TIRTOS_STACK_VERSION?= simplelink_cc2640r2_sdk_1_30_00_25
TIRTOS_DRIVERS_VERSION?=simplelink_cc2640r2_sdk_1_30_00_25
TIXDC_TOOLS_VERSION?=xdctools_3_50_00_10_core
TIRTOS_BIOS_VERSION?=bios_6_46_01_38
TIRTOS_CC26XXWARE_VERSION?=cc26xxware_2_24_02_17393
TI_CYCURTLS_VERSION ?= cycurtls-1.0.1

BCDS_TI_XDC_PATH ?= $(BCDS_TOOLS_PATH)/TI_BLE_Stack/$(TI_STACK_VERSION)/$(TIXDC_TOOLS_VERSION)
BCDS_TI_BLE_PATH ?= $(BCDS_TOOLS_PATH)/TI_BLE_Stack/$(TI_STACK_VERSION)/$(TIBLE_STACK_VERSION)
BCDS_HYDROGEN_PATH = $(BCDS_SHARED_PATH)/Hydrogen

CXXFLAGS += \
    -Dxdc_target_types__="iar/targets/arm/std.h" \

# Include paths of platform and libraries
BCDS_GTEST_INCLUDES = \
	-I $(BCDS_HYDROGEN_PATH)/test/unit/include/ti_rtos \
	-I $(BCDS_HYDROGEN_PATH)/test/unit/include/ti_ble \
	-I $(BCDS_PACKAGE_HOME)/source/Hydrogen \
	-I $(BCDS_PACKAGE_HOME)/include \
	-I $(BCDS_CONFIG_PATH)/$(BCDS_PACKAGE_NAME) \
	-I $(BCDS_ESSENTIALS_PATH)/include \
	-I $(BCDS_ESSENTIALS_PATH)/test/unit/include \
	-I $(BCDS_TI_XDC_PATH)/packages\
	-I $(BCDS_TI_XDC_PATH)/packages/xdc \
	-I $(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/src/inc \
	-I $(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/inc \
	-I $(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/target/_common \
	-I $(BCDS_TI_BLE_PATH)/kernel/tirtos/packages \
	-I $(BCDS_TI_BLE_PATH)/kernel/tirtos/packages/ti/sysbios/knl \
	-I $(BCDS_TI_BLE_PATH)/kernel/tirtos/packages/ti/sysbios \
	-Isource \

	
# Unit test files to be run
BCDS_GTEST_TEST_FILES = \
	test/unit/source/hydrogen/CmdProcessor_unittest.cc \
	test/unit/source/hydrogen/GuardedTask_unittest.cc \
	test/unit/source/hydrogen/EventHub_unittest.cc \
	test/unit/source/CmdLineDebugger_unittest.cc  \
	test/unit/source/CRC_unittest.cc \
	test/unit/source/RingBuffer_unittest.cc \
	