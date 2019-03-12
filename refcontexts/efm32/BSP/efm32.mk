#Root paths to library packages on which this package depends
BCDS_EMLIB_PATH ?= $(THIRD_PARTY_SHARED_PATH)/EMlib

#Paths to platform packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_UTILS_PATH ?= $(BCDS_SHARED_PATH)/Utils

# Define the path for include directories.
BCDS_INCLUDES = \
	-I$(BCDS_PACKAGE_HOME)/include \
	-I$(BCDS_PACKAGE_HOME)/source/protected \
	-I$(BCDS_CONFIG_PATH) \
	-I$(BCDS_CONFIG_PATH)/Essentials \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_ESSENTIALS_PATH)/include/mcu \
	-I$(BCDS_ESSENTIALS_PATH)/include/mcu/$(BCDS_TARGET_PLATFORM) \
	-I$(BCDS_ESSENTIALS_PATH)/include/bsp \
	-I$(BCDS_EMLIB_PATH)/3rd-party/EMLib/emlib/inc \
	-I$(BCDS_EMLIB_PATH)/3rd-party/EMLib/usb/inc \
	-I$(BCDS_EMLIB_PATH)/3rd-party/EMLib/CMSIS/Include \
	-I$(BCDS_EMLIB_PATH)/3rd-party/EMLib/Device/SiliconLabs/$(BCDS_EFM32_DEVICE_TYPE)/Include
	
# Source and object files
BCDS_SOURCE_FILES = \
	$(wildcard source/*.c)
