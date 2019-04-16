#Paths to BCDS shared packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_UTILS_PATH ?= $(BCDS_SHARED_PATH)/Utils

#Root paths to 3rd party shared packages on which this package depends
BCDS_FREERTOS_PATH ?= $(THIRD_PARTY_SHARED_PATH)/freertos
BCDS_SEGGERRTT_PATH = $(THIRD_PARTY_SHARED_PATH)/SeggerRTT

#Root paths to library packages on which this package depends
ifeq ($(BCDS_MCU_FAMILY),stm32l4)
	BCDS_STM32CUBE_PATH ?= $(THIRD_PARTY_SHARED_PATH)/stm32cubel4
endif

ifeq ($(BCDS_MCU_FAMILY),stm32f7)
	BCDS_STM32CUBE_PATH ?= $(THIRD_PARTY_SHARED_PATH)/stm32cubef7
endif
	
# Define the path for the include directories.
BCDS_INCLUDES = \
    -I./include \
    -I$(BCDS_ESSENTIALS_PATH)/include \
    -I$(BCDS_ESSENTIALS_PATH)/include/mcu \
    -I$(BCDS_UTILS_PATH)/include \
    -I$(BCDS_CONFIG_PATH) \
    -I$(BCDS_CONFIG_PATH)/Essentials \
    -I$(BCDS_CONFIG_PATH)/Utils \
    -I$(BCDS_CONFIG_PATH)/stm32cube \
    -I$(BCDS_CONFIG_PATH)/SeggerRTT \
    -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/include \
    -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Include \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Inc \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Device/ST/STM32L4xx/Include \
    -I$(BCDS_SEGGERRTT_PATH)/3rd-party/SEGGER_RTT/RTT

# Source and object files
BCDS_SOURCE_FILES = \
    source/CRC.c \
    source/XProtocol.c \
    source/LeanB2CAP.c \
    source/RingBuffer.c \
    source/TLV.c \
    source/Logging/Logging.c \
    source/Logging/Filter.c \
    source/Carbon/UartTransceiver.c \
    source/Logging/freertos/Logging_SyncRecorder.c \
    source/Logging/freertos/Logging_AsyncRecorder.c \
    source/Logging/freertos/Logging_RttAppender.c \
    source/Carbon/CmdProcessor.c \
	source/Carbon/GuardedTask.c \
	source/Carbon/Queue.c \
	source/Carbon/EventHub.c \
	source/Carbon/PipeAndFilter.c