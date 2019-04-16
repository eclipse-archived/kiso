BCDS_FREERTOS_PATH ?= $(THIRD_PARTY_SHARED_PATH)/FreeRTOS
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_SEGGERRTT_PATH = 3rd-party/SEGGER_RTT

# Define the path for include directories.
BCDS_INCLUDES = \
       -I$(BCDS_CONFIG_PATH) \
       -I$(BCDS_CONFIG_PATH)/SeggerRTT \
       -I$(BCDS_ESSENTIALS_PATH)/include \
   	   -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/include \
       -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/source/portable/GCC/ARM_CM4F \
       -I$(BCDS_SEGGERRTT_PATH)/RTT

# Source and object files
BCDS_SOURCE_FILES = \
	$(BCDS_SEGGERRTT_PATH)/RTT/SEGGER_RTT.c \
	$(BCDS_SEGGERRTT_PATH)/RTT/SEGGER_RTT_printf.c
