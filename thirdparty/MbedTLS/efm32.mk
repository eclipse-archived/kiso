LOWER_CASE = $(shell echo $1 | tr A-Z a-z)

BCDS_MBEDTLS_INCLUDE_PATH = 3rd-party/mbedtls/include
BCDS_MBEDTLS_LIB_PATH = 3rd-party/mbedtls

BCDS_FREERTOS_COMPILER_TYPE ?= GCC

BCDS_MACROS_DEBUG += -D MBEDTLS_CONFIG_FILE='<MbedtlsConfig.h>'

BCDS_MACROS_RELEASE += -D MBEDTLS_CONFIG_FILE='<MbedtlsConfig.h>'

# Define the path for the include directories.
BCDS_INCLUDES += \
	-I$(BCDS_MBEDTLS_INCLUDE_PATH) \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_CONFIG_PATH) \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/include \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/portable/$(BCDS_FREERTOS_COMPILER_TYPE)/$(BCDS_ARCH_CPU) \
	-I$(BCDS_MBEDTLS_INCLUDE_PATH)/mbedtls \
	-I$(BCDS_CONFIG_PATH)/MbedTLS
	
# A variable which evaluates to a list of all source code files (*.c)
BCDS_SOURCE_FILES = \
	$(wildcard $(BCDS_MBEDTLS_LIB_PATH)/source/*.c )
