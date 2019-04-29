################### Common Makefile for CommonGateway ##################################

# The following variables are expected to be passed as input variables 
# from outside.
# BCDS_PACKAGE_NAME
# BCDS_PACKAGE_ID
# BCDS_PACKAGE_HOME
# BCDS_INCLUDES
# BCDS_SOURCE_FILES

# At the time the below variables are defined (read-in phase),
# source files are not yet necessarly declared. Those could get
# defined later (by the target platform make file for instance).
# We therefore have to enable the .SECONDEXPANSION: feature of
# make. This will tell make to expand variables escaped with an
# additional "$" during the target-run phase.
.SECONDEXPANSION:

export BCDS_TARGET_PLATFORM = stm32
export BCDS_ARCH_CPU = ARM_CM4F
export BCDS_STM32_DEVICE_TYPE ?= STM32L4xx
export BCDS_MCU_FAMILY ?= stm32l4


ifeq ($(COMMONGATEWAY_HW_PLATFORM),$(filter $(COMMONGATEWAY_HW_PLATFORM),STM32L476RG_NUCLEO))
export BCDS_STM32_DEVICE_ID ?= STM32L476RG
export BCDS_STM32_DEVICE_PACKAGE_TYPE ?= STM32L476xx
endif


ifeq ($(COMMONGATEWAY_HW_PLATFORM),$(filter $(COMMONGATEWAY_HW_PLATFORM),STM32L486VG_V4))
export BCDS_STM32_DEVICE_ID ?= STM32L486VG
export BCDS_STM32_DEVICE_PACKAGE_TYPE ?= STM32L486xx
endif

# The absolute path to the debug build destination folder. 
BCDS_DEBUG_PATH = $(BCDS_PACKAGE_HOME)/debug

# The absolute path to the release build destination folder. 
BCDS_RELEASE_PATH = $(BCDS_PACKAGE_HOME)/release

# The absolute path to the gcc arm.
BCDS_GCC_ARM_PATH = $(BCDS_TOOLS_PATH)/gcc-arm-none-eabi/v7.0_2017q4/

# For more 
# details about the makefile variables check the page 
# https://bcds01.de.bosch.com/wiki/display/BSA/Build+System

DOXYGEN ?= $(BCDS_TOOLS_PATH)/Doxygen/V1.8.8_32Bit/doxygen
HHC ?= $(BCDS_TOOLS_PATH)/Doxygen/V1.8.8_32Bit/hhc.exe
GRAPHVIZ ?= $(BCDS_TOOLS_PATH)/Graphviz/V2.38-1/bin
BCDS_FOTA_TOOL_PATH ?= $(BCDS_TOOLS_PATH)/Fota_Tools


# The below defined values is to update firmware version formed by given MAJOR MINOR and PATCH 
MAJOR_SW_NO ?= 01#Defines MAJOR number and maximum value is 255
MINOR_SW_NO ?= 02#Defines MINOR number and maximum value is 255
PATCH_SW_NO ?= 03#Defines PATCH number and maximum value is 255
HEADER_VERSION =4#Defines the current used header version
PRODUCT_CLASS =0001#(Productcode[12bit]=001 for APLM, Minimum Hardwarerevision[4bit]
PRODUCT_VARIANT =0001
FIRMWARE_VERSION = $(MAJOR_SW_NO).$(MINOR_SW_NO).$(PATCH_SW_NO)
CREATE_CONTAINER_SCRIPT = $(BCDS_FOTA_TOOL_PATH)/create_fota_container.py


BCDS_DEBUG_LIB = $(BCDS_DEBUG_PATH)/lib$(BCDS_PACKAGE_NAME)_stm32_debug.a
BCDS_RELEASE_LIB = $(BCDS_RELEASE_PATH)/lib$(BCDS_PACKAGE_NAME)_stm32_release.a

# The absolute path to the configuration folder. 
BCDS_CONFIG_PATH = $(abspath $(BCDS_PACKAGE_HOME)/../../../boards/CommonGateway/kiso_config)

# The absolute path to the BSP implementation folder. 
BCDS_BOARD_PATH = $(abspath $(BCDS_PACKAGE_HOME)/../../../boards/CommonGateway/bsp)

# The absolute path to the BSP implementation folder. 
BCDS_BOOTLOADER_PATH = $(abspath $(BCDS_PACKAGE_HOME)/../Bootloader/Bootloader_Sensgate/Application)

BCDS_PACKAGE_REVISION_ID = `git rev-parse HEAD`

BCDS_SERVALSTACK_MACROS ?= \

# Compiler settings
# The variables BCDS_CFLAGS_DEBUG and BCDS_CFLAGS_RELEASE are usually defined 
# by the root makefile (e.g. project makefile) in order to pass the desired 
# compiler flags. If they are not passed, then the following default flags will
# be taken.
BCDS_CFLAGS_DEBUG ?= \
    -mcpu=cortex-m4 -mthumb \
    -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
    -std=c99 -Wall -Wextra -Wmissing-prototypes -Wimplicit-function-declaration -Wstrict-prototypes \
    -ffunction-sections -fdata-sections \
    -Wa,-ahlms=$(@:.o=.lst) \
    -MMD -MP -MF $(@:.o=.d) \
    -O0 -g

BCDS_CFLAGS_RELEASE ?= \
    -mcpu=cortex-m4 -mthumb \
    -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
    -std=c99 -Wall -Wextra -Wmissing-prototypes -Wimplicit-function-declaration -Wstrict-prototypes \
    -ffunction-sections -fdata-sections \
    -Wa,-ahlms=$(@:.o=.lst) \
    -MMD -MP -MF $(@:.o=.d) \
    -Os -DNDEBUG

BCDS_MACROS_DEBUG += \
	-D BCDS_PACKAGE_ID=$(BCDS_PACKAGE_ID) \
	-D BCDS_PACKAGE_NAME=$(BCDS_PACKAGE_NAME) \
	-D BCDS_PACKAGE_REVISION_ID=$(BCDS_PACKAGE_REVISION_ID) \
	-D $(BCDS_TARGET_PLATFORM) -D BCDS_TARGET_PLATFORM=$(BCDS_TARGET_PLATFORM) \
	-D $(COMMONGATEWAY_HW_PLATFORM) -D COMMONGATEWAY_HW_PLATFORM=$(COMMONGATEWAY_HW_PLATFORM) \
	-D $(BCDS_STM32_DEVICE_TYPE) -D $(BCDS_STM32_DEVICE_ID) -D $(BCDS_MCU_FAMILY) \
	-D $(BCDS_STM32_DEVICE_PACKAGE_TYPE) -D GCC_ARMCM4 -D USE_HAL_DRIVER=1 \
	-D APPLICATION_ENTRY_ADDRESS=0x08000000 -D APPLICATION_SECTION_SIZE=0x80000\

# we extend the macros here as per test needs
BCDS_MACROS_DEBUG += $(BCDS_TEST_MACROS)

# Note that BCDS_MACROS_RELEASE is set to be equal BCDS_MACROS_DEBUG
BCDS_MACROS_RELEASE = $(BCDS_MACROS_DEBUG)

CFLAGS_DEBUG = $(BCDS_CFLAGS_DEBUG)
CFLAGS_DEBUG += $(BCDS_MACROS_DEBUG)

CFLAGS_RELEASE = $(BCDS_CFLAGS_RELEASE)
CFLAGS_RELEASE += $(BCDS_MACROS_RELEASE)

BCDS_ARFLAGS = -cr

# Build chain settings
ifndef BCDS_BUILD_LINUX
CC = $(BCDS_GCC_ARM_PATH)bin/arm-none-eabi-gcc
AR = $(BCDS_GCC_ARM_PATH)bin/arm-none-eabi-ar
SIZE = $(BCDS_GCC_ARM_PATH)bin/arm-none-eabi-size
else
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
SIZE = arm-none-eabi-size
endif

RMDIRS = rm -rf

# Note that BCDS_SOURCE_FILES must be defined outside.
BCDS_OBJECT_FILES = \
	$(patsubst %.c, %.o, $(filter %.c,$(BCDS_SOURCE_FILES)))\

BCDS_OBJECT_FILES += \
	$(patsubst %.s, %.o, $(filter %.s,$(BCDS_SOURCE_FILES))) \

# Path of the generated static library and its object files
BCDS_DEBUG_OBJECT_PATH = $(BCDS_DEBUG_PATH)/object
BCDS_RELEASE_OBJECT_PATH = $(BCDS_RELEASE_PATH)/object

# including this file, will force the execution of the sourceFile.list
# rule.
#include sourceFile.list

BCDS_DEBUG_OBJECT_FILES = \
	$(addprefix $(BCDS_DEBUG_OBJECT_PATH)/,$(BCDS_OBJECT_FILES))

BCDS_DEBUG_DEPENDENCY_FILES = \
	$(addprefix $(BCDS_DEBUG_OBJECT_PATH)/, $(BCDS_OBJECT_FILES:.o=.d))

BCDS_RELEASE_OBJECT_FILES = \
	$(addprefix $(BCDS_RELEASE_OBJECT_PATH)/,$(BCDS_OBJECT_FILES))

BCDS_RELEASE_DEPENDENCY_FILES = \
	$(addprefix $(BCDS_RELEASE_OBJECT_PATH)/, $(BCDS_OBJECT_FILES:.o=.d))

# target to force all variables to be field before executing any rules
# and before including the dep files
.PHONY: sourceFile.list
sourceFile.list:
	@echo 'BCDS_SOURCE_FILES := $(BCDS_SOURCE_FILES)' > '$@'