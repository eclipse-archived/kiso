#Root paths to 3rd party shared packages on which this package depends
BCDS_EMLIB_PATH ?= $(THIRD_PARTY_SHARED_PATH)/EMlib
BCDS_FREERTOS_PATH ?= $(THIRD_PARTY_SHARED_PATH)/FreeRTOS

#Paths to BCDS shared packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_UTILS_PATH ?= $(BCDS_SHARED_PATH)/Utils

# Define the path for the include directories.
BCDS_INCLUDES = \
	-I$(BCDS_PACKAGE_HOME)/include \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_UTILS_PATH)/include \
	-I$(BCDS_CONFIG_PATH) \
	-I$(BCDS_CONFIG_PATH)/Essentials \
	-I$(BCDS_CONFIG_PATH)/Utils \
	-I$(BCDS_EMLIB_PATH)/3rd-party/EMLib/CMSIS/Include \
	-I$(BCDS_EMLIB_PATH)/3rd-party/EMLib/emlib/inc \
	-I$(BCDS_EMLIB_PATH)/3rd-party/EMLib/Device/SiliconLabs/$(BCDS_EFM32_DEVICE_TYPE)/Include \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/include \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/portable/GCC/ARM_CM3 \
	-I$(BCDS_ESSENTIALS_PATH)/include/mcu \

	
# Source and object files
BCDS_SOURCE_FILES = \
	source/CRC.c \
	source/XProtocol.c \
	source/LeanB2CAP.c \
	source/RingBuffer.c \
	source/TLV.c \
	source/UartTransceiver.c \
	source/Logging/Logging.c \
	source/Logging/Filter.c \
	source/Logging/Carbon/LogRecordAsyncCompact.c \
	source/Logging/Carbon/$(BCDS_TARGET_PLATFORM)/UartAppender.c \
	source/ErrorLogger.c \
	source/Carbon/CmdProcessor.c \
	source/Carbon/GuardedTask.c \
	source/Carbon/Queue.c \
	source/Carbon/EventHub.c \
	source/Carbon/I2CTransceiver.c \
	source/Carbon/PipeAndFilter.c
	