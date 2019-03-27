BCDS_CELLULAR_PLATFORM ?= ublox

#Paths to BCDS shared packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_UTILS_PATH ?= $(BCDS_SHARED_PATH)/Utils
BCDS_REFCONTEXTS_PATH ?= $(BCDS_SHARED_PATH)/RefContexts
#Paths to library packages on which this package depends
BCDS_FREERTOS_PATH ?= $(THIRD_PARTY_SHARED_PATH)/FreeRTOS/3rd-party/FreeRTOS
BCDS_STM32CUBE_DIR ?= $(THIRD_PARTY_SHARED_PATH)/STM32Cube/3rd-party/stm32cube

# Define the path for include directories.
BCDS_INCLUDES = \
	-I$(BCDS_PACKAGE_HOME)/include \
	-I$(BCDS_PACKAGE_HOME)/source/protected \
	-I$(BCDS_CONFIG_PATH) \
	-I$(BCDS_CONFIG_PATH)/Cellular \
	-I$(BCDS_CONFIG_PATH)/Utils \
	-I$(BCDS_UTILS_PATH)/include \
	-I$(BCDS_CONFIG_PATH)/HAL \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_ESSENTIALS_PATH)/include/mcu \
	-I$(BCDS_FREERTOS_PATH)/Source/include \
	-I$(BCDS_FREERTOS_PATH)/Source/portable/GCC/ARM_CM4F

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
