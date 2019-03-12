##################################################################
#   Makefile for generating the ti_cc26xx integration test app   #
##################################################################

BCDS_PACKAGE_HOME = $(CURDIR)
BCDS_PACKAGE_NAME = IntegrationTestApp
BCDS_PACKAGE_ID = 160

# The absolute path to the platform folder.
export BCDS_SHARED_PATH = $(BCDS_PACKAGE_HOME)/../../../../bcds_shared
export THIRD_PARTY_SHARED_PATH = $(BCDS_PACKAGE_HOME)/../../../../3rd_party_shared
export BCDS_TOOLS_PATH = $(BCDS_DEVELOPMENT_TOOLS)

include ../common_settings.mk

export BCDS_TARGET_PLATFORM
export BCDS_CONFIG_PATH
export BCDS_BOARD_PATH

export BCDS_CCHANNEL ?= SERIAL_CCHANNEL #TODO: check if really required

export BCDS_TEST_APP_SOURCE_PATH = $(BCDS_PACKAGE_HOME)
BCDS_TEST_APP_OBJECT_DIR := objects
BCDS_TEST_STACK_OBJECT_DIR :=stack

export BCDS_ESSENTIALS_PATH = $(BCDS_SHARED_PATH)/Essentials
BCDS_TESTLING_PATH   = $(BCDS_SHARED_PATH)/Testing/Testling
BCDS_UTILS_PATH      = $(BCDS_SHARED_PATH)/Utils
BCDS_HYDROGEN_PATH   = $(BCDS_SHARED_PATH)/Hydrogen
BCDS_FOTA_PATH       = $(BCDS_SHARED_PATH)/FOTA
BCDS_ESCRYPT_PATH    = $(THIRD_PARTY_SHARED_PATH)/Escrypt_CycurTLS
BCDS_TIRTOS_PATH     = $(BCDS_PACKAGE_HOME)/ti/rtos
export BCDS_TIBLE_PATH = $(BCDS_TEST_APP_SOURCE_PATH)/ti/ble/Tools

#Debug and Release paths for app, Stack and Ti_ble source codes
BCDS_DEBUG_PATH = $(BCDS_TEST_APP_SOURCE_PATH)/debug
BCDS_RELEASE_PATH = $(BCDS_TEST_APP_SOURCE_PATH)/release

export BCDS_TIBLE_DEBUG_PATH =  $(BCDS_TIBLE_PATH)/debug
export BCDS_TIBLE_RELEASE_PATH = $(BCDS_TIBLE_PATH)/release

export BCDS_TIBLE_DEBUG_OBJECT_PATH = $(BCDS_TIBLE_DEBUG_PATH)/$(BCDS_TEST_APP_OBJECT_DIR)
export BCDS_TIBLE_RELEASE_OBJECT_PATH = $(BCDS_TIBLE_RELEASE_PATH)/$(BCDS_TEST_APP_OBJECT_DIR)

export BCDS_STACK_DEBUG_OBJECT_PATH = $(BCDS_DEBUG_PATH)/$(BCDS_TEST_STACK_OBJECT_DIR)
export BCDS_TI_STACKEXE_PATH ?= $(BCDS_TEST_APP_SOURCE_PATH)/stackhex

BCDS_COMMON_MAKEFILE = $(BCDS_PACKAGE_HOME)/../common.mk

include $(BCDS_TEST_SECTION_PATH)/TestSection.mk
include Libraries.mk

export BCDS_TEST_MACROS

SH = $(dir $(shell which bash | shell sed -n 1p))bash

# Test make compatiblity
# We're running on top of Windows, ensure to have certain make version
ifneq (3.82.90,$(firstword $(sort $(MAKE_VERSION) 3.82.90)))
$(info Minimum make 3.82.90 is required to execute this makefile.)
$(info Use make from the compiler's bin folder or update your mingw32-make.)
$(error GNU Make version $(MAKE_VERSION) is not supported by the makefile.)
endif
$(info Running on $(OS); variable SH set to $(SH))

#Setting to verify the Compiler version
export BCDS_IAR_WORKBENCH_VERSION ?= 8.0
export BCDS_IAR_TOOLCHAIN_PATH ?= "C:\IARSystems\V_8_11_2\Embedded Workbench $(BCDS_IAR_WORKBENCH_VERSION)\arm\bin"
IAR_VERSION_REQUIRED ?= V8.11.2.13589/W32
IAR_VERSION = $(shell $(BCDS_IAR_TOOLCHAIN_PATH)/iccarm)
IAR_VERSION_STRING = $(word 5, $(IAR_VERSION))

#External Path information
export TIBLE_STACK_VERSION ?= simplelink_cc2640r2_sdk_1_50_00_58
export TIRTOS_STACK_VERSION?= tirtos_cc13xx_cc26xx_2_20_01_08
export TIRTOS_DRIVERS_VERSION?=tidrivers_cc13xx_cc26xx_2_20_01_10
export TIXDC_TOOLS_VERSION?=xdctools_3_50_03_33_core
export TIRTOS_BIOS_VERSION?=bios_6_46_01_38
export TIRTOS_CC26XXWARE_VERSION?=cc26xxware_2_24_02_17393
export TI_CYCURTLS_VERSION ?= cycurtls-1.0.1

export BCDS_TI_RTOS_PATH ?= $(BCDS_TOOLS_PATH)/TI_BLE_Stack/v3.1.1-0.1.0/$(TIRTOS_STACK_VERSION)
export BCDS_TI_RTOS_DRIVERS_PATH ?= $(BCDS_TI_RTOS_PATH)/products/$(TIRTOS_DRIVERS_VERSION)
export BCDS_TI_RTOS_CC26XXWARE_PATH ?= $(BCDS_TI_RTOS_PATH)/products/$(TIRTOS_CC26XXWARE_VERSION)
export BCDS_TI_BLE_PATH ?= $(BCDS_TOOLS_PATH)/TI_BLE_Stack/v3.1.1-0.1.0/$(TIBLE_STACK_VERSION)
export BCDS_TI_RTOS_BIOS_PATH ?= $(BCDS_TI_RTOS_PATH)/products/$(TIRTOS_BIOS_VERSION)
export BCDS_TI_XDC_PATH ?= $(BCDS_TOOLS_PATH)/TI_BLE_Stack/v3.1.1-0.1.0/$(TIXDC_TOOLS_VERSION)
export BCDS_TI_COMPILER_PATH ?=  "C:/IARSystems/V_8_11_2/Embedded Workbench $(BCDS_IAR_WORKBENCH_VERSION)/arm/inc/c"
BCDS_TI_COMPILER_PATH_IN_XDC ?= $(BCDS_TOOLS_DIR)/ti_xdctools/compiler/ti-cgt-arm_5.2.2/include

export BCDS_TI_RTOS_CC26XXWARE_PATH ?= $(BCDS_TI_RTOS_PATH)/products/$(TIRTOS_CC26XXWARE_VERSION)
export BCDS_TI_RTOS_CC26XXWARE_DRIVERLIB_PATH := $(BCDS_TI_RTOS_CC26XXWARE_PATH)/driverlib/bin/iar
export BCDS_TI_SDK_PATH ?= $(BCDS_TOOLS_PATH)/TI_BLE_Stack/v3.1.1-0.1.0/$(TIBLE_STACK_VERSION)

#Rule to create Ti_Ble library
BCDS_TI_PACKAGE_NAME = Ti_simplelink
BCDS_TIBLE_LIB_BASE_NAME = lib$(BCDS_TI_PACKAGE_NAME)_ble_cc26xx

export BCDS_TIBLE_DEBUG_LIB_NAME = $(BCDS_TIBLE_LIB_BASE_NAME)_debug.a
export BCDS_TIBLE_DEBUG_LIB   = $(BCDS_TIBLE_DEBUG_PATH)/$(BCDS_TIBLE_DEBUG_LIB_NAME)
$(BCDS_TIBLE_DEBUG_LIB):
	$(MAKE) -f ti_cc26xx_ble.mk debug

export BCDS_TIBLE_RELEASE_LIB_NAME = $(BCDS_TIBLE_LIB_BASE_NAME).a
export BCDS_TIBLE_RELEASE_LIB = $(BCDS_TIBLE_RELEASE_PATH)/$(BCDS_TIBLE_RELEASE_LIB_NAME)
$(BCDS_TIBLE_RELEASE_LIB):
	$(MAKE) -f ti_cc26xx_ble.mk release

# Define the path for the include directories.
export BCDS_EXTERNAL_INCLUDES = \
	-I$(BCDS_TI_COMPILER_PATH) \
	-I$(BCDS_TI_COMPILER_PATH_IN_XDC) \
	-I$(BCDS_ESCRYPT_LIB_DIR)/CycurTLS/src/cycurlib/lib/inc \
	-I$(BCDS_CMSIS_LIB_DIR)/Include

export BCDS_TIBLE_INCLUDES = \
	-I$(BCDS_TI_XDC_PATH)/packages\
	-I$(BCDS_TI_XDC_PATH)/packages/xdc\
	-I$(BCDS_TI_XDC_PATH)/packages/xdc/runtime\
	-I$(BCDS_TI_BLE_PATH)/source \
	-I$(BCDS_TI_BLE_PATH)/source/ti \
	-I$(BCDS_TI_BLE_PATH)/source/ti/drivers \
	-I$(BCDS_TI_BLE_PATH)/source/ti/devices/cc26x0r2 \
	-I$(BCDS_TI_BLE_PATH)/source/ti/devices/cc26x0r2/driverlib \
	-I$(BCDS_TI_BLE_PATH)/kernel/tirtos/packages \
	-I$(BCDS_TI_BLE_PATH)/kernel/tirtos/packages/ti/sysbios/knl \
	-I$(BCDS_TI_BLE_PATH)/kernel/tirtos/packages/ti/sysbios/hal \
	-I$(BCDS_TI_BLE_PATH)/kernel/tirtos/packages/ti/sysbios \
	-I$(BCDS_TI_BLE_PATH)/source/ti/mw/sensors \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/target \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/heapmgr \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/src/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/controller/cc26xx_r2/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/target/_common \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/target/_common/cc26xx \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/inc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/npi/src \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/services/src/saddr \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/roles \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/oad/cc26xx \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/roles/cc26xx \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/ecc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/crc \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/flash_interface \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/npi/stack \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/rom \
	-I$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/simple_profile

# Includes path definition
BCDS_INCLUDES = \
	-I$(BCDS_PACKAGE_HOME)/include \
	-I$(BCDS_CONFIG_PATH) \
	-I$(BCDS_CONFIG_PATH)/R01 \
	-I$(BCDS_CONFIG_PATH)/Hydrogen \
	-I$(BCDS_CONFIG_PATH)/Utils \
	-I$(BCDS_CONFIG_PATH)/Fota \
	-I$(BCDS_CONFIG_PATH)/Testling \
	-I$(BCDS_CONFIG_PATH)/Essentials \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_ESSENTIALS_PATH)/include/bsp \
	-I$(BCDS_TESTLING_PATH)/include \
	-I$(BCDS_UTILS_PATH)/include \
	-I$(BCDS_HYDROGEN_PATH)/include

# A variable which evaluates to a list of all source code files (*.c)
# Source files
BCDS_APPLICATION_SOURCE_FILES = \
	source/main.c \
	source/Startup/Board.c \
	source/Startup/BleUserConfig.c \
	source/Startup/ccfg_appBLE.c \
	source/Application/App.c


BCDS_APP_OBJECT_FILES = $(patsubst source/%.c, %.o, $(BCDS_APPLICATION_SOURCE_FILES))
BCDS_TEST_C_OBJECT_FILES = $(BCDS_TEST_SOURCE_FILES:.c=.o)

BCDS_APP_DEBUG_OBJECT_FILES = $(addprefix $(BCDS_DEBUG_OBJECT_PATH)/, $(BCDS_APP_OBJECT_FILES))
BCDS_APP_DEBUG_OBJECT_FILES += $(addprefix $(BCDS_DEBUG_OBJECT_PATH)/, $(subst $(BCDS_TEST_PATH)/,,$(BCDS_TEST_C_OBJECT_FILES)))
BCDS_APP_RELEASE_OBJECT_FILES = $(addprefix $(BCDS_RELEASE_OBJECT_PATH)/, $(BCDS_APP_OBJECT_FILES))
BCDS_APP_RELEASE_OBJECT_FILES += $(addprefix $(BCDS_RELEASE_OBJECT_PATH)/, $(subst $(BCDS_TEST_PATH)/,,$(BCDS_TEST_C_OBJECT_FILES)))

# Flash settings
FLASH = "$(BCDS_TI_FLASHTOOL_PATH)/srfprog.exe"

PRJ_DEFINES = \
	-D HEAPMGR_SIZE=0 \
	-D CC2640R2_LAUNCHXL \
	-D CC26XX \
    -D CC26XX_R2 \
    -D OSALMEM_METRICS \
    -D DeviceFamily_CC26X0R2 \
    -D ICALL_EVENTS \
    -D ICALL_JT \
    -D ICALL_LITE \
    -D ICALL_MAX_NUM_ENTITIES=6 \
    -D ICALL_MAX_NUM_TASKS=3 \
    -D MAX_NUM_BLE_CONNS=1 \
    -D POWER_SAVING \
    -D RF_SINGLEMODE \
    -D USE_ICALL \
    -D xdc_runtime_Assert_DISABLE_ALL \
    -D xdc_runtime_Log_DISABLE_ALL \
    -D BCDS_PACKAGE_ID=$(BCDS_PACKAGE_ID) \
    -D BCDS_DEVICE_ID=$(BCDS_DEVICE_ID) \
    -D $(BCDS_DEVICE_ID) \
    -D SERIAL_CCHANNEL \
    -D BCDS_TARGET_PLATFORM=$(BCDS_TARGET_PLATFORM)\
    -D $(BCDS_TARGET_PLATFORM) \
    -D BSP_SUPPORT

CC_CMD_FILE = \
	-f buildComponents.opt \
	-f IAR-Boundary.bdef \
	-f $(BCDS_CONFIG_PATH)/stack/factoryConfig.opt\
	-f $(BCDS_CONFIG_PATH)/stack/buildConfig.opt
# compiler include path
CC_INC_FILE = \
	-I"$(BCDS_TI_XDC_PATH)/packages" \
	-I"$(BCDS_CONFIG_PATH)/configPkg/.."

# Linker library path
LD_LIB_FILES = \
	-f $(BCDS_CONFIG_PATH)/configPkg/linker.cmd \
	-f IAR-Boundary.xcl \
	"$(BCDS_TI_BLE_PATH)/source/ti/blestack/rom/ble_rom_releases/cc26xx_r2/Final_Release/common_r2.symbols" \
	"$(BCDS_TI_BLE_PATH)/source/ti/devices/cc26x0r2/driverlib/bin/iar/driverlib.lib" \
	"$(BCDS_TI_BLE_PATH)/kernel/tirtos/packages/ti/dpl/lib/dpl_cc26x0r2.arm3" \
	"$(BCDS_TI_BLE_PATH)/source/ti/drivers/lib/drivers_cc26x0r2.arm3" \
	"$(BCDS_TI_BLE_PATH)/source/ti/drivers/rf/lib/rf_singleMode_cc26x0r2.arm3" \
	"$(BCDS_TI_BLE_PATH)/source/ti/display/lib/display.arm3" \
	"$(BCDS_TI_BLE_PATH)/source/ti/grlib/lib/grlib.arm3" \
	"$(BCDS_TI_BLE_PATH)/source/ti/mw/lcd/lib/lcd.arm3" \
	"$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/sha2/SHA2CC26XX.a"

# compiler command files
BCDS_CFLAGS_COMMON = \
    --dlib_config "C:/IARSystems/V_8_11_2/Embedded Workbench $(BCDS_IAR_WORKBENCH_VERSION)/arm/inc/c/DLib_Config_Normal.h"\
    $(PRJ_DEFINES) \
    $(CC_INC_FILE) \
    $(CC_CMD_FILE) \
    $(XDC_DEFINES) \
    -D_DLIB_ASSERT_ABORT

XDC_DEFINES = -Dxdc_cfg__xheader__="\"$(BCDS_PACKAGE_HOME)/$(BCDS_CONFIG_PATH)/configPkg/package/cfg/appBLE_prm3.h\""
CFLAGS_DEBUG +=$(BCDS_CFLAGS_COMMON)
CFLAGS_RELEASE +=$(BCDS_CFLAGS_COMMON)


# Linker settings
LDFLAGS_COMMON = \
	--config_def AUX_AS_RAM=1 \
	--config_def CC2650=2 \
	--config "cc26xx_ble_app.icf"  \
	--keep __vector_table \
	$(LD_LIB_FILES) --entry __iar_program_start --vfe \
	--diag_suppress=Lp023 

# Linker settings for debug
LDFLAGS_DEBUG = \
	$(LDFLAGS_COMMON)\
	--map $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).map

# Linker settings for release
LDFLAGS_RELEASE = \
	$(LDFLAGS_COMMON)\
	--map $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).map

############################ Build Targets #######################################
# extracts the path of TestApp repo
prjRootPath:
	@echo $(BCDS_DEVELOPMENT_TOOLS) > temp.txt
	@sed -i 's,/,\\\\,g' temp.txt
    BCDS_DEVELOPMENT_TOOLS_WIN=$(shell cat temp.txt)

# extracts the path of TestApp app  
prjAppPath:
	@echo $(BCDS_PACKAGE_HOME)/.. > temp1.txt
	@sed -i 's,/,\\\\,g' temp1.txt
    PRJ_APP_DIR=$(shell cat temp1.txt)

#rule to generate stack hex file
$(BCDS_TI_STACKEXE_PATH):
	$(MAKE) -f BleStack.mk debug
#-------------------------------- debug build -----------------------------------#
# Generate .hex - debug build
$(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).hex: $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).out
	@echo "Creating hexfile $@"
	@$(HEXCOPY) --ihex --verbose $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).out $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).hex
	@echo "hex file generated at $@"
	@echo "Debug Build - Built Successfully"

# Generate .out - debug build
$(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).out: $(BCDS_APP_DEBUG_OBJECT_FILES) $(BCDS_LIBS_DEBUG) $(BCDS_TIBLE_DEBUG_LIB)
	@echo "Creating .out $@"
	$(LD) $(LDFLAGS_DEBUG) $(BCDS_APP_DEBUG_OBJECT_FILES) $(BCDS_LIBS_DEBUG) $(BCDS_TIBLE_DEBUG_LIB) -o $@
	@echo ".out created sucessfully $@"

#compile C files for debug build
$(BCDS_DEBUG_OBJECT_PATH)/%.o: source/%.c
	@echo "Build $@"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS_DEBUG) $(BCDS_EXTERNAL_INCLUDES) $(BCDS_TIBLE_INCLUDES) $(BCDS_INCLUDES) $< -o $@

#compile Test Section C files for debug build
$(BCDS_DEBUG_OBJECT_PATH)/%.o: $(BCDS_TEST_PATH)/%.c
	@echo "Build $@"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS_DEBUG) $(BCDS_TEST_INCLUDES) $(BCDS_INCLUDES) $< -o $@

#--------------------------------- release build -------------------------------------#
# Generate .hex - release build
$(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).hex: $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).out
	@echo "Creating hexfile $@"
	@$(HEXCOPY) --ihex --verbose $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).out $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).hex
	@echo "hex file generated at $@"
	@echo "Release Build - Built Successfully"

# Generate .out - Release build
$(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).out: $(BCDS_APP_RELEASE_OBJECT_FILES) $(BCDS_LIBS_RELEASE) $(BCDS_TIBLE_RELEASE_LIB)
	@echo "Creating .out $@"
	$(LD) $(LDFLAGS_RELEASE) $(BCDS_APP_RELEASE_OBJECT_FILES) $(BCDS_LIBS_RELEASE) $(BCDS_TIBLE_RELEASE_LIB) -o $@
	@echo ".out created sucessfully $@"	

#compile C files for Release  build
$(BCDS_RELEASE_OBJECT_PATH)/%.o: source/%.c
	@echo "Build $@"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS_RELEASE) $(BCDS_EXTERNAL_INCLUDES) $(BCDS_TIBLE_INCLUDES) $(BCDS_INCLUDES) $< -o $@

#compile Test Section C files for Release build
$(BCDS_RELEASE_OBJECT_PATH)/%.o: $(BCDS_TEST_PATH)/%.c
	@echo "Build $@"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS_RELEASE) $(BCDS_TEST_INCLUDES) $(BCDS_INCLUDES) $< -o $@

################################ MAKE TARGETS #####################################
.PHONY: debug
debug: stack_build xdcBuild $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).hex

.PHONY: release
release: stack_build xdcBuild $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).hex

.PHONY: clean_libraries
# Clean libraries of platform and third party code
clean_libraries:
	@echo "Cleaning libraries of platform and third party sources"
	rm -rf $(BCDS_LIBS_DEBUG_PATH) \
	rm -rf $(BCDS_LIBS_RELEASE_PATH) \
	rm -rf $(BCDS_TIBLE_PATH)

.PHONY: clean
clean: stack_clean xdcClean
	@echo "Cleaning debug and release directories"
	rm -rf $(BCDS_DEBUG_PATH) \
	rm -rf $(BCDS_RELEASE_PATH) \
	rm -rf $(BCDS_TIBLE_PATH)

# Prebuild - executes xdcBuildCmdIar.cmd 
xdcBuild: prjRootPath prjAppPath
	@sed -i 's/BCDS_DEVELOPMENT_TOOLS_WIN/$(BCDS_DEVELOPMENT_TOOLS_WIN)/g' xdcBuildCmdIar.cmd
	@sed -i 's/BCDS_TEST_APP_SOURCE_PATH/$(PRJ_APP_DIR)/g' xdcBuildCmdIar.cmd
	rm -rf ./temp.txt	
	rm -rf ./temp1.txt	
	$(SH) ./xdcBuildCmdIar.cmd >/dev/null 2>/dev/null
	@echo XDC Build done

# Cleans all files generated through xdcBuild
xdcClean: prjRootPath prjAppPath
	@echo "Cleaning files generated through xdc build"
	@sed -i 's/$(BCDS_DEVELOPMENT_TOOLS_WIN)/BCDS_DEVELOPMENT_TOOLS_WIN/g' xdcBuildCmdIar.cmd
	@sed -i 's/$(PRJ_APP_DIR)/BCDS_TEST_APP_SOURCE_PATH/g' xdcBuildCmdIar.cmd
	rm -rf ./temp.txt
	rm -rf ./temp1.txt
	rm -rf $(BCDS_CONFIG_PATH)/src 
	rm -rf $(BCDS_CONFIG_PATH)/configPkg
	@echo XDC Clean done

#generates ti stack hex
stack_build: $(BCDS_TI_STACKEXE_PATH)

#cleans the stack binary
stack_clean:
	@echo "Cleaning stack path"
	rm -rf $(BCDS_TI_STACKEXE_PATH)
	rm -rf $(BCDS_STACK_DEBUG_OBJECT_PATH)

# List of Test source files
diagnosis:
	@echo "=================="
	@echo "Entering diagnosis"
	@echo "=================="
	@echo "BCDS_PACKAGE_NAME   	: " $(BCDS_PACKAGE_NAME)
	@echo "BCDS_PACKAGE_HOME   	: " $(BCDS_PACKAGE_HOME)
	@echo "BCDS_PACKAGE_ID     	: " $(BCDS_PACKAGE_ID)
	@echo "BCDS_ARCH_CPU			: " $(BCDS_ARCH_CPU)
	@echo "BCDS_TARGET_PLATFORM	: " $(BCDS_TARGET_PLATFORM)
	@echo "BCDS_TEST_SECTION_PATH	: " $(BCDS_TEST_SECTION_PATH)
	@echo "BCDS_SHARED_PATH    	: " $(BCDS_SHARED_PATH)
	@echo "THIRD_PARTY_SHARED_PATH	: " $(THIRD_PARTY_SHARED_PATH)
	@echo "BCDS_TOOLS_PATH			: " $(BCDS_TOOLS_PATH)
	@echo "BCDS_CONFIG_PATH		: " $(BCDS_CONFIG_PATH)
	@echo "BCDS_DEBUG_PATH			: " $(BCDS_DEBUG_PATH)
	@echo "BCDS_RELEASE_PATH		: " $(BCDS_RELEASE_PATH)
	@echo "BCDS_DEBUG_LIB			: " $(BCDS_DEBUG_LIB)
	@echo "BCDS_RELEASE_LIB		: " $(BCDS_RELEASE_LIB)
	@echo "BCDS_INCLUDES			: " $(BCDS_INCLUDES)
	@echo "BCDS_SOURCE_FILES		: " $(BCDS_APPLICATION_SOURCE_FILES)
	@echo "BCDS_TEST_SOURCE_FILES	: " $(BCDS_TEST_SOURCE_FILES)
	@echo "BCDS_LIBS_DEBUG_PATH : " $(BCDS_LIBS_DEBUG_PATH)
	@echo "BCDS_LIBS_RELEASE_PATH	: " $(BCDS_LIBS_RELEASE_PATH)
	@echo "BCDS_DEBUG_LIBS_GROUP	: " $(BCDS_LIBS_DEBUG) $(BCDS_TIBLE_DEBUG_LIB)
	@echo "BCDS_RELEASE_LIBS_GROUP	: " $(BCDS_LIBS_RELEASE) $(BCDS_TIBLE_RELEASE_LIB)
	@echo "=================="

# CDT for setting up eclipse indexer
cdt:
	$(BCDS_GCC_COMPILER_FOR_CDT) $(PRJ_DEFINES) $(XDC_DEFINES) -O0 --debug $(CC_INC_FILE) $(BCDS_EXTERNAL_INCLUDES) -E -P -v -dD -c ${CDT_INPUT_FILE}