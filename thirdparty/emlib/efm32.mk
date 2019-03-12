LOWER_CASE = $(shell echo $1 | tr A-Z a-z)

BCDS_EMLIB_DEVICE_INCLUDE_PATH = 3rd-party/EMLib/Device/SiliconLabs/$(BCDS_EFM32_DEVICE_TYPE)/Include
BCDS_EMLIB_DEVICE_SOURCE_PATH = 3rd-party/EMLib/Device/SiliconLabs/$(BCDS_EFM32_DEVICE_TYPE)/Source
BCDS_EMLIB_CMSIS_INCLUDE_PATH = 3rd-party/EMLib/CMSIS/Include
BCDS_EMLIB_LIB_PATH = 3rd-party/EMLib/emlib
BCDS_EMLIB_USB_PATH = 3rd-party/EMLib/usb

#macro to compile the USB 
BCDS_EMLIB_INCLUDE_USB ?= 1

# Define the path for the include directories.
BCDS_INCLUDES += \
	-I$(BCDS_EMLIB_DEVICE_INCLUDE_PATH) \
	-I$(BCDS_EMLIB_CMSIS_INCLUDE_PATH) \
	-I$(BCDS_EMLIB_LIB_PATH)/inc \
	-I$(BCDS_BOARD_PATH)/include

# A variable which evaluates to a list of all source code files (*.c)
BCDS_SOURCE_FILES = \
	$(wildcard $(BCDS_EMLIB_LIB_PATH)/src/*.c ) \
	$(BCDS_EMLIB_DEVICE_SOURCE_PATH)/system_$(call LOWER_CASE,$(BCDS_EFM32_DEVICE_TYPE)).c
	
ifeq ($(BCDS_EMLIB_INCLUDE_USB),1)		
BCDS_INCLUDES += \
	-I$(BCDS_EMLIB_USB_PATH)/inc \

BCDS_SOURCE_FILES += \
	$(wildcard $(BCDS_EMLIB_USB_PATH)/src/*.c ) 
endif
