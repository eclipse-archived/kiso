##################################################################
#    Makefile for generating the sensgate app      				 #
##################################################################
# The absolute path to the platform folder.
export BCDS_COMMON_MAKEFILE=$(BCDS_PACKAGE_HOME)/common.mk
export BCDS_SHARED_PATH = $(BCDS_PACKAGE_HOME)/../../../core
export THIRD_PARTY_SHARED_PATH = $(BCDS_PACKAGE_HOME)/../../../thirdparty
export BCDS_TOOLS_PATH = $(BCDS_DEVELOPMENT_TOOLS)

BCDS_CONFIG_PATH=$(BCDS_PACKAGE_HOME)/../../../boards/SensGate/kiso_config
export BCDS_TARGET_PLATFORM
export BCDS_CONFIG_PATH

OBJCOPY = $(BCDS_GCC_ARM_PATH)/bin/arm-none-eabi-objcopy
FLASH_TOOL_PATH = $(BCDS_TOOLS_PATH)/JLinkDebugger/v6.32g/JLink
BCDS_TOOL_NVM_GENERATOR = $(BCDS_TOOLS_PATH)/nvmgenerator

# Append the memory layout defines to the build macros
BCDS_MACROS_DEBUG += \
	$(SENSG_MEM_DEFINES) 
# Since BCDS_MACROS_RELEASE is a duplicate of BCDS_MACROS_DEBUG, no need to add this definition there
	
# The path to the debug and release binaries
SENSGATE_DEBUG_BIN = $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME)_$(SENSG_HW_PLATFORM)_debug.bin
SENSGATE_DEBUG_BIN_CONT = $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME)_cont_debug.bin
SENSGATE_RELEASE_BIN = $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME)_$(SENSG_HW_PLATFORM).bin
SENSGATE_RELEASE_BIN_CONT = $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME)_cont_release.bin

BCDS_BSP_PATH = $(BCDS_BOARD_PATH)


BCDS_ESSENTIALS_PATH = $(BCDS_SHARED_PATH)/Essentials
BCDS_TESTLING_PATH = $(BCDS_SHARED_PATH)/Testing/Testling
BCDS_UTILS_PATH = $(BCDS_SHARED_PATH)/Utils
BCDS_CELLULAR_PATH = $(BCDS_SHARED_PATH)/Cellular

BCDS_FREERTOS_PATH  = $(THIRD_PARTY_SHARED_PATH)/FreeRTOS
BCDS_STM32CUBE_PATH = $(THIRD_PARTY_SHARED_PATH)/stm32cubel4
BCDS_STM32CUBEL4_PATH = $(BCDS_STM32CUBE_PATH)
BCDS_SEGGERRTT_PATH = $(THIRD_PARTY_SHARED_PATH)/SeggerRTT

include Libraries.mk

# Includes path definition
BCDS_INCLUDES =\
    -I$(BCDS_CONFIG_PATH) \
    -I$(BCDS_PACKAGE_HOME)/include \
    -I$(BCDS_BOARD_PATH)/include \
    -I$(BCDS_ESSENTIALS_PATH)/include \
    -I$(BCDS_ESSENTIALS_PATH)/include/bsp \
    -I$(BCDS_ESSENTIALS_PATH)/include/mcu \
    -I$(BCDS_ESSENTIALS_PATH)/include/mcu/stm32 \
    -I$(BCDS_FOTA_PATH)/include \
    -I$(BCDS_TESTLING_PATH)/include \
    -I$(BCDS_UTILS_PATH)/include \
    -I$(BCDS_CELLULAR_PATH)/include \
    -I$(BCDS_LIBRARIES_PATH)/include \
    -I$(BCDS_CONFIG_PATH)/Essentials \
    -I$(BCDS_CONFIG_PATH)/Utils \
    -I$(BCDS_CONFIG_PATH)/Cellular \
    -I$(BCDS_CONFIG_PATH)/SeggerRTT \
    -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/include \
    -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/source/portable/GCC/ARM_CM4F \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/$(BCDS_STM32_DEVICE_TYPE)_HAL_Driver/Inc \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/$(BCDS_STM32_DEVICE_TYPE)_HAL_Driver/Inc/Legacy \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Include \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Device/ST/$(BCDS_STM32_DEVICE_TYPE)/Include \
    -I$(BCDS_SEGGERRTT_PATH)/3rd-party/SEGGER_RTT/RTT \
    -Isource/

BCDS_PROJECT_LD_FILE = $(BCDS_PACKAGE_HOME)/../../../boards/SensGate/Application_$(BCDS_STM32_DEVICE_TYPE)_FLASH.ld

BCDS_SOURCE_FILES = \
    source/main.c \
    source/blinky_led.c

# Application specifc object files
BCDS_APP_C_OBJECT_FILES = $(patsubst source/%.c, %.o, $(filter %.c ,$(BCDS_SOURCE_FILES)))
BCDS_APP_C_OBJECT_FILES += $(patsubst source/%.s, %.o,  $(filter %.s , $(BCDS_SOURCE_FILES)))

BCDS_APP_OBJECT_FILES = $(BCDS_APP_C_OBJECT_FILES)

BCDS_APP_DEBUG_OBJECT_FILES = $(addprefix $(BCDS_DEBUG_OBJECT_PATH)/, $(BCDS_APP_OBJECT_FILES))
BCDS_APP_DEBUG_OBJECT_FILES += $(addprefix $(BCDS_DEBUG_OBJECT_PATH)/, $(subst $(BCDS_TEST_PATH)/,,$(BCDS_TEST_C_OBJECT_FILES)))
BCDS_APP_RELEASE_OBJECT_FILES = $(addprefix $(BCDS_RELEASE_OBJECT_PATH)/, $(BCDS_APP_OBJECT_FILES))
BCDS_APP_RELEASE_OBJECT_FILES += $(addprefix $(BCDS_RELEASE_OBJECT_PATH)/, $(subst $(BCDS_TEST_PATH)/,,$(BCDS_TEST_C_OBJECT_FILES)))

#### Do we need all of this?
LDFLAGS_DEBUG ?= \
    --specs=nano.specs -Xlinker -Map=$(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).map \
    -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -D GCC_ARMCM4 \
    -T$(BCDS_PROJECT_LD_FILE)

LDFLAGS_RELEASE ?= \
    --specs=nano.specs -Xlinker -Map=$(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).map \
    -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -D GCC_ARMCM4 \
    -T$(BCDS_PROJECT_LD_FILE)

LD_LIBS_DEBUG ?= -Wl,--gc-sections -Wl,--whole-archive $(patsubst $(dirname $(BCDS_LIBS_DEBUG)), $(BCDS_DEBUG_PATH), $(foreach path, $(BCDS_LIBS_DEBUG), $(join $(BCDS_DEBUG_PATH)/, $(notdir $(path))))) -Wl,--no-whole-archive \
    -Wl,--start-group -lgcc -lc -lnosys -lm -Wl,--end-group -Wl,-u,_printf_float

LD_LIBS_RELEASE ?= -Wl,--gc-sections -Wl,--whole-archive $(BCDS_LIBS_RELEASE) -Wl,--no-whole-archive \
    -Wl,--start-group -lgcc -lc -lnosys -lm -Wl,--end-group -Wl,-u,_printf_float

#lint configured for release files only

#this exports should disappear once all shared packages have been migrated
BCDS_ARCH_FLAGS = \
    -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -D GCC_ARMCM4

# Compiler settings
export BCDS_CFLAGS_COMMON = -std=c99 -Wall -Wextra -Wstrict-prototypes -Wshadow -Werror=uninitialized -Wmissing-prototypes \
    $(BCDS_ARCH_FLAGS) -ffunction-sections -fdata-sections -D $(BCDS_TARGET_PLATFORM)=1 \
    -D $(BCDS_STM32_DEVICE_PACKAGE_TYPE)=1 -D HSE_VALUE=48000000 -DUSE_HAL_DRIVER=1 

export BCDS_CFLAGS_DEBUG_COMMON = $(BCDS_CFLAGS_COMMON) -O0 -DBCDS_LOGGING=0 -DLOG_LEVEL_MODULE=1 -g
export BCDS_CFLAGS_RELEASE_COMMON = $(BCDS_CFLAGS_COMMON) -Os -DNDEBUG

#Get Application package dependencies (move execution to root folder and then run spider script)
getPackageDep:
	cd $(BCDS_PACKAGE_HOME)/.. && $(PYTHON_34)/python.exe -u -mspider sync -d Package.dep

#Get BTL package dependencies (move execution to root folder and then run spider script)
getPackageDepBTL:
	cd $(BCDS_BOOTLOADER_PATH)/.. && $(PYTHON_34)/python.exe -u -mspider sync -d Package.dep

# Set the application address and size in the ld file
.PHONY: exp_start_address
exp_start_address::
	@printf "APPLICATION_ENTRY_ADDRESS = $(APPLICATION_ENTRY_ADDRESS);\nAPPLICATION_SECTION_SIZE = $(APPLICATION_SECTION_SIZE);"> start_addr.ld
	
######################## Build Targets #######################################
###### Application build Targets
.PHONY: debugAll	
debugAll: debug debugBTL
.PHONY: releaseAll
releaseAll: release releaseBTL
###### Application build Targets
.PHONY: debug	
debug: $(SENSGATE_DEBUG_BIN) exp_start_address
.PHONY: release	
release: $(SENSGATE_DEBUG_BIN) exp_start_address
.PHONY: all exp_start_address
all: $(SENSGATE_RELEASE_BIN_CONT)

###### Bootloader build Targets
.PHONY: debugBTL	
debugBTL: cleanBTL
	@echo "Building Bootloader debug from SensGate"
	$(MAKE) -C $(BCDS_BOOTLOADER_PATH) debug BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
.PHONY: releaseBTL
releaseBTL: cleanBTL
	@echo "Building Bootloader release from SensGate"
	$(MAKE) -C $(BCDS_BOOTLOADER_PATH) release BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk

.PHONY: cleanBTL	
cleanBTL:
	@echo "Building Bootloader debug from SensGate"
	$(MAKE) -C $(BCDS_BOOTLOADER_PATH) clean_all BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
	
##### Application Clean libraries of platform and third party code
.PHONY: clean_libraries
clean_libraries:
	@echo "Cleaning libraries of platform and third party sources"
	$(RMDIRS) $(BCDS_LIBS_DEBUG_PATH) $(BCDS_LIBS_RELEASE_PATH)

# Cleans all files
clean_all: clean clean_libraries
##### Application debug related rules #####
$(SENSGATE_DEBUG_BIN_CONT): $(SENSGATE_DEBUG_BIN)
	@echo "Creating binary with header $@"
	@python $(CREATE_CONTAINER_SCRIPT) v4 openssl $(SENSGATE_DEBUG_BIN) $(SENSGATE_DEBUG_BIN_CONT) $(HEADER_VERSION) $(PRODUCT_CLASS) $(PRODUCT_VARIANT) $(FIRMWARE_VERSION)

$(SENSGATE_DEBUG_BIN): $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).out
	@echo "Creating binary $@"
	@$(OBJCOPY) -R .usrpg -O binary $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).out $@

$(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).out: $(BCDS_APP_DEBUG_OBJECT_FILES) $(BCDS_LIBS_DEBUG)
	@echo "Creating .out $@"
	$(CC) $(LDFLAGS_DEBUG) $(BCDS_APP_DEBUG_OBJECT_FILES) $(LD_LIBS_DEBUG) -o $@
	
$(BCDS_DEBUG_OBJECT_PATH)/%.o: source/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS_DEBUG) -I . $(BCDS_INCLUDES) $< -o $@

$(BCDS_DEBUG_OBJECT_PATH)/%.o: $(BCDS_TEST_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS_DEBUG) -I . $(BCDS_INCLUDES) $< -o $@

##### Application release related rules #####
$(SENSGATE_RELEASE_BIN_CONT): $(SENSGATE_RELEASE_BIN)
	@echo "Creating binary with header $@"
	@python $(CREATE_CONTAINER_SCRIPT) v4 openssl $(SENSGATE_RELEASE_BIN) $(SENSGATE_RELEASE_BIN_CONT) $(HEADER_VERSION) $(PRODUCT_CLASS) $(PRODUCT_VARIANT) $(FIRMWARE_VERSION)

$(SENSGATE_RELEASE_BIN): $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).out
	@echo "Creating binary $@"
	@$(OBJCOPY) -R .usrpg -O binary $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).out $@

$(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).out: $(BCDS_APP_RELEASE_OBJECT_FILES) $(BCDS_LIBS_RELEASE)
	@echo "Creating .out $@"
	$(CC) $(LDFLAGS_RELEASE) $(BCDS_APP_RELEASE_OBJECT_FILES) $(LD_LIBS_RELEASE) -o $@

$(BCDS_RELEASE_OBJECT_PATH)/%.o: source/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS_RELEASE) -I . $(BCDS_INCLUDES) $< -o $@

$(BCDS_RELEASE_OBJECT_PATH)/%.o: $(BCDS_TEST_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS_RELEASE) -I . $(BCDS_INCLUDES) $< -o $@

##### Flashing rules #####
.PHONY: flash_erase
flash_erase:
	rm -rf CommandFile.jlink
	@echo "Speed 12000" >> CommandFile.jlink                                             #speed
	@echo "device $(BCDS_STM32_DEVICE_ID)" >> CommandFile.jlink                          #device
	@echo "erase" >> CommandFile.jlink                                                   #erase
	@echo "r" >> CommandFile.jlink                                                       #reset
	@echo "qc" >> CommandFile.jlink                                                      #quit
	@"$(FLASH_TOOL_PATH)" -device $(BCDS_STM32_DEVICE_ID) -if SWD -CommanderScript CommandFile.jlink
	@echo "Flash erase successfull"

# Flash application to primary FW partition
define flash_rule_template =
.PHONY: $(1)
$(1): $(2)
	rm -rf CommandFile.jlink
	@echo "Speed 12000" > CommandFile.jlink                                              #speed
	@echo "r" >> CommandFile.jlink                                                       #reset
	@echo "h" >> CommandFile.jlink                                                       #halt
	@echo "loadbin "$(2)" "$(APPLICATION_FLASH_ADDRESS) >> CommandFile.jlink               #load bin
	@echo "verifybin "$(2)" "$(APPLICATION_FLASH_ADDRESS) >> CommandFile.jlink             #verify bin
	@echo "r" >> CommandFile.jlink                                                       #reset
	@echo "qc" >> CommandFile.jlink                                                      #quit
	@$(FLASH_TOOL_PATH) -device $(BCDS_STM32_DEVICE_ID) -if SWD -CommanderScript CommandFile.jlink
	@echo "Flashing $(2) is completed successfully"
endef

$(eval $(call flash_rule_template,flash_debug_bin,$(SENSGATE_DEBUG_BIN_CONT)))
$(eval $(call flash_rule_template,flash_release_bin,$(SENSGATE_RELEASE_BIN_CONT)))

# Flash application to upload partition
define flash_update_rule_template =
.PHONY: $(1)
$(1): $(2)
	rm -rf CommandFile.jlink
	@echo "Speed 12000" > CommandFile.jlink                                              #speed
	@echo "r" >> CommandFile.jlink                                                       #reset
	@echo "h" >> CommandFile.jlink                                                       #halt
	@echo "loadbin "$(2)" "$(SENSG_DOWNLOAD_SEC_START_ADR) >> CommandFile.jlink          #load bin
	@echo "verifybin "$(2)" "$(SENSG_DOWNLOAD_SEC_START_ADR) >> CommandFile.jlink        #verify bin
	@echo "r" >> CommandFile.jlink                                                       #reset
	@echo "qc" >> CommandFile.jlink                                                      #quit
	@$(FLASH_TOOL_PATH) -device $(BCDS_STM32_DEVICE_ID) -if SWD -CommanderScript CommandFile.jlink
	@echo "Flashing $(2) is completed successfully"
endef

$(eval $(call flash_update_rule_template,flash_update_debug_cont,$(SENSGATE_DEBUG_BIN_CONT)))
$(eval $(call flash_update_rule_template,flash_update_release_cont,$(SENSGATE_RELEASE_BIN_CONT)))

# Flash BTL
define flash_btl_rule_template =
.PHONY: flash_btl_$(1)
flash_btl_$(1):
	@echo "Flashing Bootloader $(1) from SensGate"
	$(MAKE) -C $(BCDS_BOOTLOADER_PATH) flash_$(1)_bin BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk
endef

$(eval $(call flash_btl_rule_template,debug))
$(eval $(call flash_btl_rule_template,release))

# Flash application and BTL
.PHONY: flash_all_debug
flash_all_debug: flash_btl_debug flash_debug_bin

.PHONY: flash_all_release
flash_all_release: flash_btl_release flash_release_bin

# Rules to extend the Build Targets of the app
define BuildTargetExtend_rule_template
$(1):: bsp_$(1) libraries_$(1)
.PHONY: bsp_$(1) 

bsp_$(1)::	
	$(MAKE) -C $(BCDS_BOARD_PATH) $(1) BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk

.PHONY: libraries_$(1)

libraries_$(1)::	
	$(MAKE) -C $(BCDS_LIBRARIES_PATH) $(1) BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk	
	
endef

Target_Extend_builds = lint lint_clean gtest gtest_clean lcov lcov_clean
$(foreach build, $(Target_Extend_builds), $(eval $(call BuildTargetExtend_rule_template,$(build))))

