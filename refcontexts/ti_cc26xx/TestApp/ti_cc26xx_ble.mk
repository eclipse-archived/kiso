##################################################################
#      Makefile for generating the static library(.a files)      #
##################################################################
include ../common_settings.mk
BCDS_PACKAGE_ID = 160     #Duplicate this Package ID from main makefile.
# Build chain settings	
CC = $(BCDS_IAR_TOOLCHAIN_PATH)/iccarm
LD = $(BCDS_IAR_TOOLCHAIN_PATH)/ilinkarm
OBJ = $(BCDS_IAR_TOOLCHAIN_PATH)/objmanip
HEXCOPY = $(BCDS_IAR_TOOLCHAIN_PATH)/ielftool
AR = $(BCDS_IAR_TOOLCHAIN_PATH)/iarchive

ARFLAGS = --create

# Macro defines
PRJ_DEFINES = \
	-D HEAPMGR_SIZE=0 \
	-D CC2640R2_LAUNCHXL \
	-D CC2650EM_7ID \
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
	-D USE_CORE_SDK \
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

		
# compiler command path
PRJ_CC_CMD_FILE = \
	-f buildComponents.opt \
	-f IAR-Boundary.bdef \
	-f $(BCDS_CONFIG_PATH)/stack/factoryConfig.opt \
	-f $(BCDS_CONFIG_PATH)/stack/buildConfig.opt
	
# compiler include path	
PRJ_CC_INC_FILE = \
	-I"$(BCDS_TI_XDC_PATH)/packages" \
	-I"$(BCDS_CONFIG_PATH)/configPkg/.." 
	
# Compiler settings
#The following CFLAG settings will be taken when this module is not compiled from a main project.
#If this make is invoked from the main project, then the CFLAG settings will be taken over from that main project.
ifndef BCDS_CFLAGS_COMMON
BCDS_CFLAGS_COMMON = \
	--aeabi --cpu=Cortex-M3 --diag_suppress=Pa050,Go005 -Ohz --endian=little -e --thumb -r --fpu=None \
	$(PRJ_DEFINES) $(PRJ_CC_CMD_FILE) $(PRJ_CC_INC_FILE)  \
	-Dxdc_target_types__="iar/targets/arm/std.h" \
	-Dxdc_target_name__=M3	
endif

ifndef BCDS_CFLAGS_DEBUG_COMMON
BCDS_CFLAGS_DEBUG_COMMON = $(BCDS_CFLAGS_COMMON) --debug
endif

ifndef BCDS_CFLAGS_RELEASE_COMMON
BCDS_CFLAGS_RELEASE_COMMON = $(BCDS_CFLAGS_COMMON) -DRELEASE_BUILD 
endif

#If there is any additional CFLAG that is required to compile this module, it can be added under the following variable.
CFLAGS_COMMON  = $(BCDS_CFLAGS_COMMON)
CFLAGS_DEBUG = $(CFLAGS_COMMON) $(BCDS_CFLAGS_DEBUG_COMMON)
CFLAGS_RELEASE = $(CFLAGS_COMMON) $(BCDS_CFLAGS_RELEASE_COMMON)

BCDS_INCLUDES = \
	-I$(BCDS_ESSENTIALS_PATH)/include

# A variable which evaluates to a list of all source code files (*.c)
# of TIBLE(TI BLE stack) 
BCDS_TIBLE_SOURCE_FILES = \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/host/gatt_uuid.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/ecc/ECCROMCC26XX.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/host/gattservapp_util.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/src/icall_cc2650.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/src/icall.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/app/icall_api_lite.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/icall/src/icall_user_config.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/simple_profile/cc26xx/simple_gatt_profile.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/oad/cc26xx/oad.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/profiles/roles/cc26xx/peripheral.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/hal/src/target/_common/TRNGCC26XX.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/common/cc26xx/util.c \
	$(BCDS_TI_BLE_PATH)/source/ti/mw/sensors/SensorI2C.c \
	$(BCDS_TI_BLE_PATH)/source/ti/blestack/osal/src/common/osal.c

# Objects
BCDS_TIBLE_SOURCE_OBJECT_FILES = \
	$(patsubst $(BCDS_TI_BLE_PATH)/%.c, %.o, $(BCDS_TIBLE_SOURCE_FILES))

BCDS_TIBLE_DEBUG_OBJECT_FILES = \
	$(addprefix $(BCDS_TIBLE_DEBUG_OBJECT_PATH)/, $(BCDS_TIBLE_SOURCE_OBJECT_FILES)) 
	
BCDS_TIBLE_RELEASE_OBJECT_FILES = \
	$(addprefix $(BCDS_TIBLE_RELEASE_OBJECT_PATH)/, $(BCDS_TIBLE_SOURCE_OBJECT_FILES)) 
	
##################### Build Targets #########################
debug: $(BCDS_TIBLE_DEBUG_LIB)
	
release: $(BCDS_TIBLE_RELEASE_LIB)
	
$(BCDS_TIBLE_DEBUG_LIB): $(BCDS_TIBLE_DEBUG_OBJECT_FILES)
	@echo
	@echo "========================================"
	@mkdir -p $(@D)
	@echo "Build TI BLE for ti_26xx (debug)"
	@$(AR) $(ARFLAGS) $@ $(BCDS_TIBLE_DEBUG_OBJECT_FILES)
	@echo "Library created: $@"
	@echo "========================================"

$(BCDS_TIBLE_RELEASE_LIB): $(BCDS_TIBLE_RELEASE_OBJECT_FILES)
	@echo
	@echo "========================================"
	@mkdir -p $(@D)
	@echo "Build TI BLE for ti_26xx (release)"
	@$(AR) $(ARFLAGS) $@ $(BCDS_TIBLE_RELEASE_OBJECT_FILES)
	@echo "Library created: $@"
	@echo "========================================"

$(BCDS_TIBLE_DEBUG_OBJECT_PATH)/%.o: $(BCDS_TI_BLE_PATH)/%.c
	@echo "Build $?"
	@mkdir -p $(@D)
	@$(CC) -c $(BCDS_CFLAGS_DEBUG_COMMON) \
        -I . $(BCDS_INCLUDES) $(BCDS_EXTERNAL_INCLUDES) $(BCDS_TIBLE_INCLUDES) $< -o $@


$(BCDS_TIBLE_RELEASE_OBJECT_PATH)/%.o: $(BCDS_TI_BLE_PATH)/%.c
	@echo "Build $?"
	@mkdir -p $(@D)
	@$(CC) -c $(BCDS_CFLAGS_RELEASE_COMMON) \
		-I . $(BCDS_INCLUDES) $(BCDS_EXTERNAL_INCLUDES) $(BCDS_TIBLE_INCLUDES) $< -o $@