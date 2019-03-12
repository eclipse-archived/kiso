##################################################################
#    Makefile for generating the stm32 integration test app      #
##################################################################

BCDS_PACKAGE_HOME = $(CURDIR)
BCDS_PACKAGE_NAME = IntegrationTestApp
BCDS_PACKAGE_ID = 160

# The absolute path to the platform folder.
export BCDS_SHARED_PATH = $(BCDS_PACKAGE_HOME)/../../../../../bcds_shared
export THIRD_PARTY_SHARED_PATH = $(BCDS_PACKAGE_HOME)/../../../../../3rd_party_shared
export BCDS_TOOLS_PATH = $(BCDS_DEVELOPMENT_TOOLS)

include ../common_settings.mk

export BCDS_TARGET_PLATFORM
export BCDS_CONFIG_PATH

export BCDS_CCHANNEL = SERIAL_CCHANNEL #TODO: check if really required

# The absolute path to the debug and release binaries
BCDS_TESTAPP_DEBUG_BIN = $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME)_debug.bin
BCDS_TESTAPP_RELEASE_BIN = $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).bin

BCDS_ESSENTIALS_PATH = $(BCDS_SHARED_PATH)/Essentials
BCDS_TESTLING_PATH = $(BCDS_SHARED_PATH)/Testing/Testling
BCDS_UTILS_PATH = $(BCDS_SHARED_PATH)/Utils

BCDS_FREERTOS_PATH = $(THIRD_PARTY_SHARED_PATH)/FreeRTOS
BCDS_STM32CUBE_PATH = $(THIRD_PARTY_SHARED_PATH)/STM32Cube

BCDS_COMMON_MAKEFILE = $(BCDS_PACKAGE_HOME)/../../common.mk

include $(BCDS_TEST_SECTION_PATH)/TestSection.mk
include Libraries.mk

export BCDS_TEST_MACROS

# Includes path definition
BCDS_INCLUDES = \
    -I$(BCDS_PACKAGE_HOME)/include \
    -I$(BCDS_BOARD_PATH)/include \
    -I$(BCDS_ESSENTIALS_PATH)/include \
    -I$(BCDS_ESSENTIALS_PATH)/include/bsp \
    -I$(BCDS_ESSENTIALS_PATH)/include/mcu \
    -I$(BCDS_ESSENTIALS_PATH)/include/mcu/stm32 \
    -I$(BCDS_TESTLING_PATH)/include \
    -I$(BCDS_UTILS_PATH)/include \
    -I$(BCDS_CONFIG_PATH) \
    -I$(BCDS_CONFIG_PATH)/Testling \
    -I$(BCDS_CONFIG_PATH)/Essentials \
    -I$(BCDS_CONFIG_PATH)/Utils \
    -I$(BCDS_CONFIG_PATH)/stm32cube \
    -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/Source/include \
    -I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/source/portable/GCC/ARM_CM4F \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/$(BCDS_STM32_DEVICE_TYPE)_HAL_Driver/Inc \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/$(BCDS_STM32_DEVICE_TYPE)_HAL_Driver/Inc/Legacy \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Include \
    -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Device/ST/$(BCDS_STM32_DEVICE_TYPE)/Include

BCDS_INCLUDES += $(BCDS_TEST_INCLUDES)

BCDS_PROJECT_LD_FILE = Application_$(BCDS_STM32_DEVICE_ID)_FLASH.ld

BCDS_APPLICATION_SOURCE_FILES = \
    source/Led/Led.c \
    source/Main/App.c \
    source/Main/main.c \
    source/Main/System.c \
    source/FaultHandler.c

# Application specifc object files
BCDS_APP_C_OBJECT_FILES = $(patsubst source/%.c, %.o, $(BCDS_APPLICATION_SOURCE_FILES))

BCDS_APP_OBJECT_FILES = $(BCDS_APP_C_OBJECT_FILES)
BCDS_TEST_C_OBJECT_FILES = $(BCDS_TEST_SOURCE_FILES:.c=.o)

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

LD_LIBS_DEBUG ?= -Wl,--gc-sections -Wl,--whole-archive $(BCDS_LIBS_DEBUG) -Wl,--no-whole-archive \
    -Wl,--start-group -lgcc -lc -lnosys -lm -Wl,--end-group

LD_LIBS_RELEASE ?= -Wl,--gc-sections -Wl,--whole-archive $(BCDS_LIBS_RELEASE) -Wl,--no-whole-archive \
    -Wl,--start-group -lgcc -lc -lnosys -lm -Wl,--end-group

OBJCOPY = $(BCDS_TOOLS_PATH)/gcc-arm-none-eabi/V4.7_2014q2/bin/arm-none-eabi-objcopy
FLASH_TOOL_PATH = $(BCDS_TOOLS_PATH)/JLinkDebugger/v5.10l/JLink
APPLICATION_ADDRESS = 0x08000000 # @see Application_STM32L486QG_FLASH.ld
BCDS_TOOL_NVM_GENERATOR = $(BCDS_TOOLS_PATH)/nvmgenerator

#this exports should disappear once all shared packages have been migrated
BCDS_ARCH_FLAGS = \
    -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -D GCC_ARMCM4

# Compiler settings
export BCDS_CFLAGS_COMMON = -std=c99 -Wall -Wextra -Wstrict-prototypes -Wshadow -Werror=uninitialized -Wmissing-prototypes \
    $(BCDS_ARCH_FLAGS) -ffunction-sections -fdata-sections -D $(BCDS_TARGET_PLATFORM) \
    -D $(BCDS_STM32_DEVICE_PACKAGE_TYPE) -D HSE_VALUE=48000000 -D VECT_TAB_OFFSET=0x00 -DUSE_HAL_DRIVER=1

export BCDS_CFLAGS_DEBUG_COMMON = $(BCDS_CFLAGS_COMMON) -O0 -DBCDS_LOGGING=1 -DLOG_LEVEL_MODULE=1 -g
export BCDS_CFLAGS_RELEASE_COMMON = $(BCDS_CFLAGS_COMMON) -Os -DNDEBUG

######################## Build Targets #######################################
.PHONY: debug
debug: $(BCDS_TESTAPP_DEBUG_BIN)

.PHONY: release	
release: $(BCDS_TESTAPP_RELEASE_BIN)

.PHONY: clean_libraries
# Clean libraries of platform and third party code
clean_libraries:
	@echo "Cleaning libraries of platform and third party sources"
	$(RMDIRS) $(BCDS_LIBS_DEBUG_PATH) $(BCDS_LIBS_RELEASE_PATH)

.PHONY: clean
clean:
	rm -rf $(BCDS_DEBUG_PATH) $(BCDS_RELEASE_PATH)

##### debug related rules #####
$(BCDS_TESTAPP_DEBUG_BIN): $(BCDS_DEBUG_PATH)/$(BCDS_PACKAGE_NAME).out
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

##### release related rules #####
$(BCDS_TESTAPP_RELEASE_BIN): $(BCDS_RELEASE_PATH)/$(BCDS_PACKAGE_NAME).out
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

diagnosis:
	@echo "=================="
	@echo "Entering diagnosis"
	@echo "=================="
	@echo "BCDS_TEST_SOURCE_FILES:"
	@echo $(BCDS_TEST_SOURCE_FILES)
	@echo "=================="
	@echo "Leaving diagnosis"
	@echo "=================="
.PHONY: diagnosis

##### Flashing rules #####
.PHONY: flash_erase
flash_erase:
	@echo "Speed 2000" >> CommandFile.jlink                                             #speed
	@echo "device $(call CONVERT_TO_LOWER_CASE,$(BCDS_DEVICE_ID))" >> CommandFile.jlink  #device
	@echo "erase" >> CommandFile.jlink                                                   #erase
	@echo "qc" >> CommandFile.jlink                                                      #quit
	@"$(FLASH_TOOL_PATH)" -device $(BCDS_DEVICE_ID) -if SWD -CommanderScript CommandFile.jlink
	@echo "Flash erase successfull"

define flash_rule_template =
.PHONY: $(1)
$(1): $(2)
	@echo "Speed 2000" > CommandFile.jlink                                     #speed
	@echo "r" >> CommandFile.jlink                                              #reset
	@echo "h" >> CommandFile.jlink                                              #halt
	@echo "loadbin "$(2)" "$(APPLICATION_ADDRESS) >> CommandFile.jlink          #load bin
	@echo "verifybin "$(2)" "$(APPLICATION_ADDRESS) >> CommandFile.jlink        #verify bin
	@echo "r" >> CommandFile.jlink                                              #reset
	@echo "qc" >> CommandFile.jlink                                             #quit
	@$(FLASH_TOOL_PATH) -device $(BCDS_STM32_DEVICE_ID) -if SWD -CommanderScript CommandFile.jlink
	@echo "Flashing $(2) is completed successfully"
endef

$(eval $(call flash_rule_template,flash_debug_bin,$(BCDS_TESTAPP_DEBUG_BIN)))
$(eval $(call flash_rule_template,flash_release_bin,$(BCDS_TESTAPP_RELEASE_BIN)))

##### NVM rule #####
.PHONY: generate_integration_nvm
generate_integration_nvm:
	@echo "========================================"
	@echo "Generation of the required files"
	@python $(BCDS_TOOL_NVM_GENERATOR)/nvm_item_configuration_converter.py -i $(BCDS_NVM_XML_CONFIG) -o $(BCDS_NVM_GENERATED_PATH) -a 1
	@echo "Files generated here: " $(BCDS_NVM_GENERATED_PATH)
	@echo "========================================"

.PHONY: cdt
cdt:
	$(CC) $(CFLAGS_DEBUG) -O0 $(BCDS_INCLUDES) -E -P -v -dD -c ${CDT_INPUT_FILE}