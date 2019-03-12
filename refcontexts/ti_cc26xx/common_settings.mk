################### Common Makefile for ti_cc26xx ##################################

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

export BCDS_TARGET_PLATFORM = ti_cc26xx
export BCDS_ARCH_CPU = ARM_CM3
export BCDS_DEVICE_ID ?= R01

# The absolute path to the BCDS_Shared folder. 
BCDS_SHARED_PATH ?= $(BCDS_PACKAGE_HOME)/../../bcds_shared

# The absolute path to the 3rd party shared folder. 
THIRD_PARTY_SHARED_PATH ?= $(BCDS_PACKAGE_HOME)/../../3rd_party_shared

# The absolute path to the tools folder. 
BCDS_TOOLS_PATH ?= $(BCDS_DEVELOPMENT_TOOLS)

# The absolute path to the debug build destination folder. 
BCDS_DEBUG_PATH = $(BCDS_PACKAGE_HOME)/debug

# The absolute path to the release build destination folder. 
BCDS_RELEASE_PATH = $(BCDS_PACKAGE_HOME)/release

# The absolute path to the lint
BCDS_LINT_PATH = $(BCDS_TOOLS_PATH)/PCLint/v9.00-0.2.5
BCDS_LINT_CONFIG_PATH = $(BCDS_LINT_PATH)/config

# For more 
# details about the makefile variables check the page 
# https://bcds01.de.bosch.com/wiki/display/BSA/Build+System

DOXYGEN ?= $(BCDS_TOOLS_PATH)/Doxygen/V1.8.8_32Bit/doxygen
HHC ?= $(BCDS_TOOLS_PATH)/Doxygen/V1.8.8_32Bit/hhc.exe
GRAPHVIZ ?= $(BCDS_TOOLS_PATH)/Graphviz/V2.38-1/bin

BCDS_DEBUG_LIB = $(BCDS_DEBUG_PATH)/lib$(BCDS_PACKAGE_NAME)_ti_cc26xx_debug.a
BCDS_RELEASE_LIB = $(BCDS_RELEASE_PATH)/lib$(BCDS_PACKAGE_NAME)_ti_cc26xx.a

# The absolute path to the configuration folder. 
BCDS_CONFIG_PATH = $(BCDS_SHARED_PATH)/RefContexts/ti_cc26xx/config

# The absolute path to the BSP implementation folder. 
BCDS_BOARD_PATH = $(BCDS_SHARED_PATH)/RefContexts/ti_cc26xx/BSP_Iroll

# The absolute path to the common gtest makefile
BCDS_COMMON_GTEST_MAKEFILE =  $(BCDS_SHARED_PATH)/RefContexts/ti_cc26xx/common_gtest.mk

BCDS_PACKAGE_REVISION_ID = `git rev-parse HEAD`

# Compiler settings
# The variables BCDS_CFLAGS_DEBUG and BCDS_CFLAGS_RELEASE are usually defined 
# by the root makefile (e.g. project makefile) in order to pass the desired 
# compiler flags. If they are not passed, then the following default flags will
# be taken.
BCDS_CFLAGS_DEBUG ?= \
	--aeabi --cpu=Cortex-M3 --diag_suppress=Pa050,Go005 --endian=little \
	-e --thumb --fpu=None -Dxdc_target_types__="iar/targets/arm/std.h" \
	-Dxdc_target_name__=M3 \
	-Ohz --debug

BCDS_CFLAGS_RELEASE ?= \
	--aeabi --cpu=Cortex-M3 --diag_suppress=Pa050,Go005 --endian=little \
	-e --thumb --fpu=None -Dxdc_target_types__="iar/targets/arm/std.h" \
	-Dxdc_target_name__=M3 \
	-Ohz -DRELEASE_BUILD -DNDEBUG

BCDS_MACROS_DEBUG ?= \
	-D BCDS_PACKAGE_ID=$(BCDS_PACKAGE_ID) \
	-D BCDS_PACKAGE_NAME=$(BCDS_PACKAGE_NAME) \
	-D BCDS_PACKAGE_REVISION_ID=$(BCDS_PACKAGE_REVISION_ID) \
	-D $(BCDS_TARGET_PLATFORM) \
	-D BCDS_TARGET_PLATFORM=$(BCDS_TARGET_PLATFORM) \
	-D $(BCDS_DEVICE_ID) 

# we extend the macros here as per test needs
BCDS_MACROS_DEBUG += $(BCDS_TEST_MACROS)

# Note that BCDS_MACROS_RELEASE is set to be equal BCDS_MACROS_DEBUG
BCDS_MACROS_RELEASE += $(BCDS_MACROS_DEBUG)

CFLAGS_DEBUG = $(BCDS_CFLAGS_DEBUG)
CFLAGS_DEBUG += $(BCDS_MACROS_DEBUG)

CFLAGS_RELEASE = $(BCDS_CFLAGS_RELEASE)
CFLAGS_RELEASE += $(BCDS_MACROS_RELEASE)

BCDS_ARFLAGS = -cr

BCDS_IAR_TOOLCHAIN_PATH ?= "C:/IARSystems/V_8_11_2/EMBEDD~1.0/arm/bin"

# Build chain settings
CC = $(BCDS_IAR_TOOLCHAIN_PATH)/iccarm
LD = $(BCDS_IAR_TOOLCHAIN_PATH)/ilinkarm
OBJ = $(BCDS_IAR_TOOLCHAIN_PATH)/objmanip
HEXCOPY = $(BCDS_IAR_TOOLCHAIN_PATH)/ielftool
#SIZE = $(BCDS_TOOLS_PATH)/gcc-arm-none-eabi/bin/arm-none-eabi-size
LINT = $(BCDS_LINT_PATH)/exe/lint-nt.launch

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

BCDS_LINT_CONFIG_FILE = $(BCDS_LINT_CONFIG_PATH)/bcds_iar.lnt
BCDS_LOB_PATH = $(BCDS_DEBUG_PATH)/lob
BCDS_LINT_EXCLUDE_PATH = $(BCDS_LINT_PATH)/exe 

BCDS_LIBDIRS_LINT = $(foreach DIR, $(subst -I,,$(BCDS_INCLUDES)), +libdir\($(DIR)\))
BCDS_INCLUDES_LINT = $(subst -I,-i,$(BCDS_INCLUDES))
BCDS_INCLUDES_LINT += -i$(BCDS_LINT_CONFIG_PATH)
BCDS_MACRO_LINT = $(subst -D ,-d,$(BCDS_MACROS_DEBUG))

# Create lint options passed to lint exe.
LINTFLAGS = -i"$(BCDS_LINT_CONFIG_PATH)" \
	-i"$(BCDS_IAR_TOOLCHAIN_PATH)/../../arm/inc/c" \
	$(BCDS_MACRO_LINT) \
	$(BCDS_INCLUDES_LINT) \
	$(BCDS_LIBDIRS_LINT) \
	+rw[__packed] +fem -e537 -e502 -elib\(49\) -elib\(91\) -elib\(101\) -elib\(132\)

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
