BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_FREERTOS_PATH = 3rd-party/FreeRTOS

BCDS_FREERTOS_COMPILER_TYPE ?= GCC

#Include Path information
BCDS_INCLUDES = \
    -I$(BCDS_ESSENTIALS_PATH)/include \
    -I$(BCDS_CONFIG_PATH) \
	-I$(BCDS_FREERTOS_PATH)/Source/include \
	-I$(BCDS_FREERTOS_PATH)/Source/portable/$(BCDS_FREERTOS_COMPILER_TYPE)/$(BCDS_ARCH_CPU) \
	-I$(BCDS_FREERTOS_PATH)/Source/portable/MemMang

# A variable which evaluates to a list of all source code files (*.c)
BCDS_SOURCE_FILES = \
	./source/FreeRTOSConfigCompatibilityCheck.c \
	$(BCDS_FREERTOS_PATH)/Source/event_groups.c \
	$(BCDS_FREERTOS_PATH)/Source/list.c \
	$(BCDS_FREERTOS_PATH)/Source/queue.c \
	$(BCDS_FREERTOS_PATH)/Source/stream_buffer.c \
	$(BCDS_FREERTOS_PATH)/Source/tasks.c \
	$(BCDS_FREERTOS_PATH)/Source/timers.c \
	$(BCDS_FREERTOS_PATH)/Source/portable/MemMang/heap_4.c \
	$(BCDS_FREERTOS_PATH)/Source/portable/$(BCDS_FREERTOS_COMPILER_TYPE)/ARM_CM3/port.c

