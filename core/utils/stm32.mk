#Paths to BCDS shared packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_UTILS_PATH ?= $(BCDS_SHARED_PATH)/Utils

#Root paths to 3rd party shared packages on which this package depends
BCDS_FREERTOS_PATH ?= $(THIRD_PARTY_SHARED_PATH)/FreeRTOS
BCDS_STM32CUBE_PATH ?= $(THIRD_PARTY_SHARED_PATH)/STM32Cube
	
# Define the path for the include directories.
BCDS_INCLUDES = \
    -I$(BCDS_PACKAGE_HOME)/include \
    -I$(BCDS_ESSENTIALS_PATH)/include \
    -I$(BCDS_UTILS_PATH)/include \
    -I$(BCDS_CONFIG_PATH) \
    -I$(BCDS_CONFIG_PATH)/Essentials \
    -I$(BCDS_CONFIG_PATH)/Utils \
    -I$(BCDS_CONFIG_PATH)/stm32cube \
    -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/include \
    -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Include \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Inc \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Device/ST/STM32L4xx/Include \
    -I$(BCDS_ESSENTIALS_PATH)/include/mcu \

# Source and object files
BCDS_SOURCE_FILES = \
    source/CRC.c \
    source/XProtocol.c \
    source/LeanB2CAP.c \
    source/RingBuffer.c \
    source/TLV.c \
    source/Logging/Logging.c \
    source/Logging/Filter.c \
    source/UartTransceiver.c \
    source/Logging/Carbon/LogRecordAsyncCompact.c \
    source/Logging/Carbon/stm32/SwoAppender.c \
    source/Carbon/CmdProcessor.c \
	source/Carbon/GuardedTask.c \
	source/Carbon/Queue.c \
	source/Carbon/EventHub.c \
	source/Carbon/PipeAndFilter.c