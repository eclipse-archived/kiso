#This file invokes the make based on the target name, 
include ../common_settings.mk

# A variable which evaluates to a list of all source code files (*.c) for generating Stack HEX file
BCDS_BLESTACK_SOURCE_FILES = \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/stack/ble_dispatch_JT.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/stack/ble_dispatch_lite.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/rom/r2/common_rom_init.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/roles/gap.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/stack/ble_user_config.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/host/gatt_uuid.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/host/gattservapp_util.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/roles/gapbondmgr.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/target/_common/hal_flash_wrapper.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/common/hal_assert.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/target/_common/hal_trng_wrapper.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/src/icall_lite_translation.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/icall_startup.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/npi/stack/npi.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/onboard.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/common/osal_bufmgr.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/common/osal.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/common/osal_cbtimer.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/common/osal_clock.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/common/osal_memory_icall.c \
	$(BCDS_TI_BLE_PATH)/examples/rtos/CC2640R2_LAUNCHXL/blestack/simple_peripheral/src/stack/osal_icall_ble.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/common/osal_pwrmgr.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/common/osal_timers.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/mcu/cc26xx/osal_snv_wrapper.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/host/sm_ecc.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/rom/r2/rom_init.c

BCDS_BLESTACK_OBJECT_FILES = \
	$(patsubst $(BCDS_TI_BLE_PATH)/%.c, %.o, $(BCDS_BLESTACK_SOURCE_FILES))

BCDS_BLESTACK_DEBUG_OBJECT_FILES = \
	$(addprefix $(BCDS_STACK_DEBUG_OBJECT_PATH)/,$(BCDS_BLESTACK_OBJECT_FILES))

# IAR compiler package tools
IAR_SYMEXPORT = isymexport
IAR_CC = $(BCDS_IAR_TOOLCHAIN_PATH)/iccarm
IAR_LD = $(BCDS_IAR_TOOLCHAIN_PATH)/ilinkarm
IAR_OBJ = $(BCDS_IAR_TOOLCHAIN_PATH)/objmanip
IAR_HEXCOPY = $(BCDS_IAR_TOOLCHAIN_PATH)/ielftool

# Macro defines
IAR_PRJ_DEFINES = \
	-D CC26XX \
	-D CC26XX_R2 \
	-D DeviceFamily_CC26X0R2 \
	-D EXT_HAL_ASSERT \
	-D FLASH_ROM_BUILD \
	-D GATT_NO_CLIENT \
	-D ICALL_EVENTS \
	-D ICALL_JT \
	-D ICALL_LITE \
	-D OSAL_CBTIMER_NUM_TASKS=1 \
	-D OSAL_SNV=1 \
	-D POWER_SAVING \
	-D RF_SINGLEMODE \
	-D USE_ICALL \
	-D CC2640R2_LAUNCHXL

# package name for stack
BCDS_PRJ_STACK_PACKAGE_NAME = SimpleBlePeripheralIarStack

# compiler command path
PRJ_CC_CMD_FILE = \
	-f ./buildComponents.opt \
	-f ./IAR-Boundary.bdef \
	-f $(BCDS_CONFIG_PATH)/stack/factoryConfig.opt \
	-f $(BCDS_CONFIG_PATH)/stack/buildConfig.opt

# Linker include file path
PRJ_LD_INCLUDE_FILES = \
	-f	$(BCDS_TI_RTOS_CC26XXWARE_DRIVERLIB_PATH)/ \
	-f	$(BCDS_TI_RTOS_CC26XXWARE_DRIVERLIB_PATH)/include \
	-f  $(BCDS_TI_RTOS_CC26XXWARE_PATH)/driverlib

# Linker library path	
PRJ_LIB_FILES = \
	"$(BCDS_CONFIG_PATH)/stack/ble_r2.symbols" \
	"$(BCDS_TI_BLE_PATH)/source/ti/devices/cc26x0r2/driverlib/bin/iar/driverlib.lib"

PRJ_XDC_DEFINES = \
	-Dxdc_target_types__="iar/targets/arm/std.h" \
	-Dxdc_target_name__=M3

# Compiler settings
BCDS_CFLAGS_COMMON = \
	--dlib_config "C:/IARSystems/V_8_11_2/Embedded Workbench $(BCDS_IAR_WORKBENCH_VERSION)/arm/inc/c/DLib_Config_Normal.h" \
	--aeabi --cpu=Cortex-M3 --diag_suppress=Pa050,Go005,Pe047 --endian=little -e --thumb --fpu=None --make_all_definitions_weak \
	$(IAR_PRJ_DEFINES) \
	$(PRJ_XDC_DEFINES) \
	$(PRJ_CC_CMD_FILE)

BCDS_CFLAGS_DEBUG_COMMON = -Ohz -DNDEBUG

# Linker settings
LDFLAGS_COMMON = \
	--config_def FLASH_ROM_BUILD=2 \
	--config_def CC2650=2 \
	--config "$(BCDS_CONFIG_PATH)/stack/cc26xx_ble_stack.icf" \
	-f ./IAR-Boundary.xcl --semihosting \
	$(PRJ_LIB_FILES) --entry startup_entry --vfe

LDFLAGS_DEBUG = \
	$(LDFLAGS_COMMON)\
	--map $(BCDS_STACK_DEBUG_OBJECT_PATH)/$(BCDS_PRJ_STACK_PACKAGE_NAME).map

############# Generate Hex file  for TI  ###################
# debug build
$(BCDS_DEBUG_PATH)/$(BCDS_PRJ_STACK_PACKAGE_NAME).hex: $(BCDS_STACK_DEBUG_OBJECT_PATH)/$(BCDS_PRJ_STACK_PACKAGE_NAME).out
	mkdir -p $(BCDS_TI_STACKEXE_PATH)
	@echo "Creating hexfile $@"
	@$(IAR_HEXCOPY) --ihex --verbose $(BCDS_STACK_DEBUG_OBJECT_PATH)/$(BCDS_PRJ_STACK_PACKAGE_NAME).out $(BCDS_TI_STACKEXE_PATH)/$(BCDS_PRJ_STACK_PACKAGE_NAME).hex
	@echo "hex file generated at $@"
	@echo "Debug Build - Built Successfully"

$(BCDS_STACK_DEBUG_OBJECT_PATH)/$(BCDS_PRJ_STACK_PACKAGE_NAME).out: $(BCDS_BLESTACK_DEBUG_OBJECT_FILES)
	@echo "Creating .out $@"
	cp $(BCDS_TI_BLE_PATH)/examples/rtos/CC2640R2_LAUNCHXL/blestack/simple_peripheral/tirtos/iar/config/ble_r2.symbols $(CURDIR)/../config/stack/ble_r2.symbols
	$(IAR_LD) $(BCDS_BLESTACK_DEBUG_OBJECT_FILES) "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/ctrl/cc2640_hci_pxxx.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/ctrl/cc2640_ll_pxxx.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/hci_tl/cc2640_hci_tl_none.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/host/cc26xx_att_pxxx.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/host/cc26xx_gap_pxxx.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/host/cc26xx_gatt_pxxx.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/host/cc26xx_l2cap_pxxx.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/host/cc26xx_profiles_pxxx.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/host/cc26xx_smp_pxxx.a" "$(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib/host/cc26xx_sm_pxxx.a" -o $@ $(LDFLAGS_DEBUG) 
	@echo ".out created sucessfully $@"

#################### compile C files ########################
# debug build
$(BCDS_STACK_DEBUG_OBJECT_PATH)/%.o: $(BCDS_TI_BLE_PATH)/%.c
	@echo "Build $@"
	@mkdir -p $(@D)
	@$(IAR_CC) $(BCDS_CFLAGS_COMMON) $(BCDS_CFLAGS_DEBUG_COMMON) $(BCDS_TIBLE_INCLUDES) $(BCDS_EXTERNAL_INCLUDES) $< -o $@
	
############### Build Targets #######################################
prebuild:
	$(BCDS_TI_BLE_PATH)/tools/blestack/lib_search/lib_search.exe $(BCDS_CONFIG_PATH)/stack/buildConfig.opt $(BCDS_TI_BLE_PATH)/source/ti/blestack/blelib $(BCDS_TI_BLE_PATH)/examples/rtos/CC2640R2_LAUNCHXL/blestack/simple_peripheral/tirtos/iar/config/lib_linker.cmd cc2640 $(BCDS_TI_BLE_PATH)/tools/blestack/lib_search/lib_search.xml $(BCDS_TI_BLE_PATH)/source/ti/blestack/symbols
	
postbuild_debug:
	"$(BCDS_TI_BLE_PATH)/tools/blestack/frontier/frontier.exe" iar "$(BCDS_STACK_DEBUG_OBJECT_PATH)/$(BCDS_PRJ_STACK_PACKAGE_NAME).map" ./IAR-Boundary.bdef ./IAR-Boundary.xcl

debug:prebuild $(BCDS_DEBUG_PATH)/$(BCDS_PRJ_STACK_PACKAGE_NAME).hex #postbuild_debug
	@rm -rf ./Boundary.log
	@rm -rf ./Boundary.log.pos
