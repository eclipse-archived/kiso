BCDS_CELLULAR_PLATFORM ?= ublox
BCDS_HW_VERSION ?= HW_XDK_v1

#Paths to BCDS shared packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_UTILS_PATH ?= $(BCDS_SHARED_PATH)/Utils

#Paths to library packages on which this package depends
BCDS_EMLIB_PATH ?= $(THIRD_PARTY_SHARED_PATH)/EMlib
BCDS_FREERTOS_PATH ?= $(THIRD_PARTY_SHARED_PATH)/FreeRTOS

# Define the path for include directories.
BCDS_INCLUDES = \
	-I$(BCDS_PACKAGE_HOME)/include \
	-I$(BCDS_PACKAGE_HOME)/source/protected \
	-I$(BCDS_CONFIG_PATH) \
	-I$(BCDS_CONFIG_PATH)/Cellular \
	-I$(BCDS_CONFIG_PATH)/Essentials \
	-I$(BCDS_CONFIG_PATH)/Utils \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_BOARD_PATH)/include \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_ESSENTIALS_PATH)/include/bsp \
	-I$(BCDS_ESSENTIALS_PATH)/include/mcu \
	-I$(BCDS_ESSENTIALS_PATH)/include/mcu/$(BCDS_TARGET_PLATFORM) \
	-I$(BCDS_UTILS_PATH)/include \
	-I$(BCDS_EMLIB_PATH)/Emlib/Device/SiliconLabs/$(BCDS_EFM32_DEVICE_TYPE)/Include \
	-I$(BCDS_EMLIB_PATH)/Emlib/emlib/inc \
	-I$(BCDS_EMLIB_PATH)/Emlib/CMSIS/Include \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/include \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/portable/GCC/ARM_CM3


# Source files
BCDS_SOURCE_FILES = \
	source/common/AtParser.c \
	source/common/AtQueue.c \
	source/common/Utils.c \
	source/common/Engine.c \
	source/$(BCDS_CELLULAR_PLATFORM)/Cellular.c \
	source/$(BCDS_CELLULAR_PLATFORM)/Control.c \
	source/$(BCDS_CELLULAR_PLATFORM)/Network.c \
	source/$(BCDS_CELLULAR_PLATFORM)/Socket.c \
	source/$(BCDS_CELLULAR_PLATFORM)/Dns.c \
	source/$(BCDS_CELLULAR_PLATFORM)/Urc.c \
	source/$(BCDS_CELLULAR_PLATFORM)/Sms.c \

ifeq ($(BCDS_HW_VERSION),HW_XDK_v1)
	BCDS_SOURCE_FILES += source/$(BCDS_CELLULAR_PLATFORM)/CellularBsp.c
endif

BCDS_MACROS_DEBUG += \
	-D $(BCDS_CELLULAR_PLATFORM) \
	-D BCDS_CELLULAR_PLATFORM=$(BCDS_CELLULAR_PLATFORM)

ifeq ($(BCDS_CELLULAR_PLATFORM),ublox)
BCDS_MACROS_DEBUG += -D BCDS_FEATURE_BSP_UMTS_LISAU2=1
BCDS_MACROS_DEBUG += -D BCDS_FEATURE_BSP_UMTS_QUECTELUC20=0
endif
ifeq ($(BCDS_CELLULAR_PLATFORM),quectel)
BCDS_MACROS_DEBUG += -D BCDS_FEATURE_BSP_UMTS_QUECTELUC20=1
BCDS_MACROS_DEBUG += -D BCDS_FEATURE_BSP_UMTS_LISAU2=0
endif
