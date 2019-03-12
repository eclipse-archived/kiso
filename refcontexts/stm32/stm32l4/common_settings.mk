################### Common Makefile for STM32 ##################################

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
export BCDS_MCU_FAMILY ?= stm32l4
export BCDS_STM32_DEVICE_TYPE ?= STM32L4xx
export BCDS_STM32_DEVICE_ID ?= STM32L486QG
export BCDS_STM32_DEVICE_PACKAGE_TYPE ?= STM32L486xx

BCDS_SERVALPAL_ARCHIVE_SERVAL_STACK=1
BCDS_SERVALSTACK_EXTENSION=1

# The absolute path to the platform folder. 
BCDS_SHARED_PATH ?= $(BCDS_PACKAGE_HOME)/../../bcds_shared

# The absolute path to the libraries folder. 
THIRD_PARTY_SHARED_PATH ?= $(BCDS_PACKAGE_HOME)/../../3rd_party_shared

# The absolute path to the tools folder. 
BCDS_TOOLS_PATH ?= $(BCDS_DEVELOPMENT_TOOLS)

# The absolute path to the debug build destination folder. 
BCDS_DEBUG_PATH = $(BCDS_PACKAGE_HOME)/debug

# The absolute path to the release build destination folder. 
BCDS_RELEASE_PATH = $(BCDS_PACKAGE_HOME)/release

# The absolute path to the lint and gcc arm.
BCDS_LINT_PATH = $(BCDS_TOOLS_PATH)/PCLint/v9.00-0.2.3
BCDS_LINT_CONFIG_PATH = $(BCDS_LINT_PATH)/config
BCDS_GCC_ARM_PATH = $(BCDS_TOOLS_PATH)/gcc-arm-none-eabi/V4.7_2014q2/

# For more 
# details about the makefile variables check the page 
# https://bcds01.de.bosch.com/wiki/display/BSA/Build+System

DOXYGEN ?= $(BCDS_TOOLS_PATH)/Doxygen/V1.8.8_32Bit/doxygen
HHC ?= $(BCDS_TOOLS_PATH)/Doxygen/V1.8.8_32Bit/hhc.exe
GRAPHVIZ ?= $(BCDS_TOOLS_PATH)/Graphviz/V2.38-1/bin

BCDS_DEBUG_LIB = $(BCDS_DEBUG_PATH)/lib$(BCDS_PACKAGE_NAME)_stm32_debug.a
BCDS_RELEASE_LIB = $(BCDS_RELEASE_PATH)/lib$(BCDS_PACKAGE_NAME)_stm32.a

# The absolute path to the configuration folder. 
BCDS_CONFIG_PATH = $(BCDS_SHARED_PATH)/RefContexts/stm32/stm32l4/config

# The absolute path to the BSP implementation folder. 
BCDS_BOARD_PATH = $(BCDS_SHARED_PATH)/RefContexts/stm32/stm32l4/BSP

# The absolute path to the common gtest makefile
BCDS_COMMON_GTEST_MAKEFILE =  $(BCDS_SHARED_PATH)/RefContexts/stm32/common_gtest.mk

BCDS_PACKAGE_REVISION_ID = `git rev-parse HEAD`

BCDS_SERVALSTACK_MACROS ?= \
	-DSERVAL_ENABLE_HTTP_CLIENT=0 \
	-DSERVAL_ENABLE_HTTP_SERVER=0 \
	-DSERVAL_HTTP_MAX_NUM_SESSIONS=50 \
	-DSERVAL_ENABLE_WEBSERVER=0 \
	-DSERVAL_ENABLE_COAP_CLIENT=1 \
	-DSERVAL_ENABLE_COAP_SERVER=1 \
	-DSERVAL_ENABLE_REST_CLIENT=0 \
	-DSERVAL_ENABLE_REST_SERVER=0 \
	-DSERVAL_ENABLE_REST_HTTP_BINDING=0 \
	-DSERVAL_ENABLE_REST_COAP_BINDING=0 \
	-DSERVAL_ENABLE_XUDP=0 \
	-DSERVAL_ENABLE_DPWS=0 \
	-DSERVAL_ENABLE_DTLS_ECC=0 \
	-DSERVAL_ENABLE_TLS_CLIENT=0 \
	-DSERVAL_ENABLE_TLS_SERVER=0 \
	-DSERVAL_ENABLE_HTTP_AUTH=0 \
	-DSERVAL_ENABLE_HTTP_AUTH_DIGEST=0 \
	-DSERVAL_MAX_NUM_MESSAGES=2 \
	-DSERVAL_ENABLE_DUTY_CYCLING=1 \
	-DSERVAL_ENABLE_APP_DATA_ACCESS=0 \
	-DSERVAL_POLICY_COAP_STRICT_TOKEN_API=1 \
	-DSERVAL_ENABLE_COAP_COMBINED_SERVER_AND_CLIENT=1 \
	-DSERVAL_ENABLE_COAP_OBSERVE=1 \
	-DSERVAL_LOG_LEVEL=0 \
	-DSERVAL_ENABLE_LWM2M=1 \
	-DCOAP_ACK_TIMEOUT=2 \
	-DBCDS_SERVAL_ENABLE_UDP_HOOK=0 \
	-DBCDS_SERVAL_COMMBUFF_SEND_BUFFER_MAX_LEN=256 \
	-DCOAP_ACK_CHECK_INTERVAL=200 \
	-DSERVAL_RESOURCE_MONITOR_PERIODE=200 \
	-DSERVAL_TLS_CYASSL=0 \
	-DSERVALPAL_DEFAULT_CHANNEL=0

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
    -O2 -DNDEBUG

BCDS_MACROS_DEBUG ?= \
	-D BCDS_PACKAGE_ID=$(BCDS_PACKAGE_ID) \
	-D BCDS_PACKAGE_NAME=$(BCDS_PACKAGE_NAME) \
	-D BCDS_PACKAGE_REVISION_ID=$(BCDS_PACKAGE_REVISION_ID) \
	-D $(BCDS_TARGET_PLATFORM) -D BCDS_TARGET_PLATFORM=$(BCDS_TARGET_PLATFORM) \
	-D $(BCDS_MCU_FAMILY) -D $(BCDS_STM32_DEVICE_TYPE) -D $(BCDS_STM32_DEVICE_ID) \
	-D $(BCDS_STM32_DEVICE_PACKAGE_TYPE) -D GCC_ARMCM4 -D USE_HAL_DRIVER=1

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
LINT = $(BCDS_LINT_PATH)/exe/lint-nt.launch
else
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
SIZE = arm-none-eabi-size
LINT = NO_LINT_ON_LINUX
endif

RMDIRS = rm -rf

# Note that BCDS_SOURCE_FILES must be defined outside.
BCDS_OBJECT_FILES = \
	$(patsubst %.c, %.o, $(BCDS_SOURCE_FILES))

BCDS_LINT_FILES = \
	$(patsubst %.c, %.lob, $(BCDS_SOURCE_FILES))

# Path of the generated static library and its object files
BCDS_DEBUG_OBJECT_PATH = $(BCDS_DEBUG_PATH)/object
BCDS_RELEASE_OBJECT_PATH = $(BCDS_RELEASE_PATH)/object

# including this file, will force the execution of the sourceFile.list
# rule.
include sourceFile.list

BCDS_DEBUG_OBJECT_FILES = \
	$(addprefix $(BCDS_DEBUG_OBJECT_PATH)/,$(BCDS_OBJECT_FILES))

BCDS_DEBUG_DEPENDENCY_FILES = \
	$(addprefix $(BCDS_DEBUG_OBJECT_PATH)/, $(BCDS_OBJECT_FILES:.o=.d))

BCDS_RELEASE_OBJECT_FILES = \
	$(addprefix $(BCDS_RELEASE_OBJECT_PATH)/,$(BCDS_OBJECT_FILES))

BCDS_RELEASE_DEPENDENCY_FILES = \
	$(addprefix $(BCDS_RELEASE_OBJECT_PATH)/, $(BCDS_OBJECT_FILES:.o=.d))

BCDS_DEBUG_LINT_FILES = \
	$(addprefix $(BCDS_LOB_PATH)/, $(BCDS_LINT_FILES))

# target to force all variables to be field before executing any rules
# and before including the dep files
.PHONY: sourceFile.list
sourceFile.list:
	@echo 'BCDS_SOURCE_FILES := $(BCDS_SOURCE_FILES)' > '$@'

BCDS_LINT_CONFIG_FILE = $(BCDS_LINT_CONFIG_PATH)/bcds.lnt
BCDS_LOB_PATH = $(BCDS_DEBUG_PATH)/lob
BCDS_LINT_EXCLUDE_PATH = $(BCDS_LINT_PATH)/exe 

BCDS_LIBDIRS_LINT = $(foreach DIR, $(subst -I,,$(BCDS_INCLUDES)), +libdir\($(DIR)\))
BCDS_INCLUDES_LINT = $(subst -I,-i,$(BCDS_INCLUDES))
BCDS_INCLUDES_LINT += -i$(BCDS_LINT_CONFIG_PATH)
BCDS_MACRO_LINT = $(subst -D ,-d,$(BCDS_MACROS_DEBUG))

# Create lint options passed to lint exe.
LINTFLAGS = -i"$(BCDS_LINT_CONFIG_PATH)" \
	$(BCDS_MACRO_LINT) \
	$(BCDS_INCLUDES_LINT) \
	$(BCDS_LIBDIRS_LINT) \
	-e537 -e502 -elib\(49\) -elib\(91\)

# To create doxygen reports, define paths to access source, include and doxygen folders of shared packaes	
DOXYFILE_INPUT_PLATFORM = \
	$(BCDS_PACKAGE_HOME)/$(BCDS_TARGET_PLATFORM)/doxygen/MainPage.dox \
	$(wildcard $(BCDS_SHARED_PATH)/**/source) \
    $(wildcard $(BCDS_SHARED_PATH)/**/include)

DOX_OUTPUT_FOLDER_PLATFORM = $(BCDS_PACKAGE_HOME)/$(BCDS_TARGET_PLATFORM)/doxygen/html

# Necessary for doxygen to recognise the path properly
DOXYGEN_PACKAGE_HOME = $(subst /,\,$(BCDS_PACKAGE_HOME))

DOXYFILE_INPUT_PACKAGE = \
	$(DOXYGEN_PACKAGE_HOME)/source \
	$(DOXYGEN_PACKAGE_HOME)/include \
	$(DOXYGEN_PACKAGE_HOME)/doxygen/MainPage.dox

DOX_OUTPUT_FOLDER_PACKAGE = $(DOXYGEN_PACKAGE_HOME)/doxygen/html
# Predefined symbols for doxygen can be extended by the package makefile using "DOXYGEN_PREDEFS += \".
DOXYGEN_PREDEFS = \